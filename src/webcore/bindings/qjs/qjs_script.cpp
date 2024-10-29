/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"

#if ENABLE(QJS)

#include "qjs_script.h"

#include "Chrome.h"
#include "Document.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "GCController.h"
#include "QJSDocument.h"
#include "QJSDOMWindow.h"
#include "Page.h"
#include "Settings.h"
#include "qjs_events.h"
#include "qjs_window.h"

#include "GCController.h"

#if ENABLE(SVG)
#include "JSSVGLazyEventListener.h"
#endif

#if ENABLE(JSNATIVEBINDING)
#include "NativeBinding.h"
#endif

using namespace QJS;

namespace WebCore {

ScriptController::ScriptController(Frame* frame)
{
    m_frame = frame;
    m_handlerLineno = 0;
}

ScriptController::~ScriptController()
{
    // Check for <rdar://problem/4876466>. In theory, no JS should be executing
    // in our interpreter. 
    ASSERT(!m_script || !context());
    
    if (m_script) {
        m_script = 0;

        JS_FreeContext(m_context);
    
        // It's likely that destroying the interpreter has created a lot of garbage.
        gcController().garbageCollectSoon();
    }
}

JSValue ScriptController::evaluate(const String& filename, int baseLine, const String& str) 
{
    // evaluate code. Returns the JS return value or 0
    // if there was none, an error occured or the type couldn't be converted.

    initScriptIfNeeded();

    // Evaluating the JavaScript could cause the frame to be deallocated
    // so we start the keep alive timer here.
    m_frame->keepAlive();
    
    JSValue thisNode = Window::retrieve(m_frame);
  
    //<Debug> FIXME:time checkout
    //m_script->startTimeoutCheck();
    JSValue comp = m_script->evaluate(filename, baseLine, str.characters(), str.length(), thisNode);
    //m_script->stopTimeoutCheck();
  
    if (!JS_IsException(comp)) {
        return comp;
    } else {
        JSContext * ctx = context();
        JSValue strObj = JS_ToString(ctx, comp);
        const char * str = JS_ToCString(ctx, strObj);
        String errorMessage(str);

        JSValue line = JS_GetPropertyStr(ctx, comp, "line");
        int32_t lineNumber;
        JS_ToInt32(ctx, &lineNumber, line);

        JSValue url = JS_GetPropertyStr(ctx, comp, "sourceURL");
        const char * urlStr = JS_ToCString(ctx, url);
        String sourceURL(urlStr);

        if (Page* page = m_frame->page()) {
            page->chrome()->addMessageToConsole(JSMessageSource, ErrorMessageLevel, errorMessage, lineNumber, sourceURL);
        }

        JS_FreeCString(ctx, urlStr);
        JS_FreeValue(ctx, url);
        JS_FreeValue(ctx, line);
        JS_FreeCString(ctx, str);
        JS_FreeValue(ctx, strObj);
    }

    return JS_NULL;
}

void ScriptController::clear() {
  // clear resources allocated by the interpreter, and make it ready to be used by another page
  // We have to keep it, so that the Window object for the frame remains the same.
  // (we used to delete and re-create it, previously)
    if (m_script) {
        Window *win = Window::retrieveWindow(m_frame);
        if (win) {
            win->clear();
#if ENABLE(JSNATIVEBINDING)
            (void)initNativeJSObjects(m_script); 
#endif            
        }
    }
}

EventListener* ScriptController::createHTMLEventHandler(const String& functionName, const String& code, Node* node)
{
    initScriptIfNeeded();
    return new JSLazyEventListener(functionName, code, Window::retrieveWindow(m_frame), node, m_handlerLineno);
}

#if ENABLE(SVG)
EventListener* ScriptController::createSVGEventHandler(const String& functionName, const String& code, Node* node)
{
    initScriptIfNeeded();
    return new JSSVGLazyEventListener(functionName, code, Window::retrieveWindow(m_frame), node, m_handlerLineno);
}
#endif

void ScriptController::finishedWithEvent(Event* event)
{
  // This is called when the DOM implementation has finished with a particular event. This
  // is the case in sitations where an event has been created just for temporary usage,
  // e.g. an image load or mouse move. Once the event has been dispatched, it is forgotten
  // by the DOM implementation and so does not need to be cached still by the interpreter
  m_script->forgetDOMObject(event);
}

ScriptInterpreter* ScriptController::interpreter()
{
    initScriptIfNeeded();
    ASSERT(m_script);
    return m_script.get();
}

void ScriptController::initScriptIfNeeded()
{
    if (m_script)
        return;

    m_context = JS_NewContext(GLOBAL()->runtime);

    // Build the global object - which is a Window instance
    JSDOMWindow::init(m_context);
    JSValue contextObj = JS_GetGlobalObject(m_context);
    JSValue globalObject = JSDOMWindow::create(m_context, contextObj, m_frame->domWindow());
    JS_FreeValue(m_context, contextObj);

    // Create a KJS interpreter for this frame
    m_script = new ScriptInterpreter(globalObject, m_frame);

    //init dom object all Quickjs !!! <Debug>


    m_frame->loader()->dispatchWindowObjectAvailable();

#if ENABLE(JSNATIVEBINDING)            
    (void)initNativeJSObjects(m_script);
#endif
}
    
void ScriptController::clearDocumentWrapper() 
{
    if (!m_script)
        return;

    m_script->globalObject()->removeDirect("document");
}

}
#endif
