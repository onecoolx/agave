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
#include "DOMWindow.h"
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
    if (m_script) {
        JS_SetContextOpaque(m_context, 0);
        m_script = 0;

        // Free context first - releases all JS variable references
        JSRuntime* rt = JS_GetRuntime(m_context);
        JS_SetGCThreshold(rt, (size_t)-1);
        JS_FreeContext(m_context);

        // Now release DOM cache references - safe because context is gone,
        // no JS code can run, and GC threshold is disabled
        if (m_frame && m_frame->document())
            ScriptInterpreter::forgetAllDOMNodesForDocument(m_frame->document());

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
        gcController().garbageCollectSoon();
        return comp;
    } else {
        JSContext * ctx = context();
        JSValue exc = JS_GetException(ctx);
        const char * msg = JS_ToCString(ctx, exc);
        fprintf(stderr, "[QJS-ERR] %s\n", msg ? msg : "unknown");

        String errorMessage = msg ? String(msg) : String("unknown error");
        JS_FreeCString(ctx, msg);

        JSValue line = JS_GetPropertyStr(ctx, exc, "lineNumber");
        int32_t lineNumber = 0;
        JS_ToInt32(ctx, &lineNumber, line);
        JS_FreeValue(ctx, line);

        JS_FreeValue(ctx, exc);

        if (Page* page = m_frame->page()) {
            page->chrome()->addMessageToConsole(JSMessageSource, ErrorMessageLevel, errorMessage, lineNumber, String());
        }
    }

    gcController().garbageCollectSoon();
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

    // Build the global object - which is a Window/DOMWindow instance
    JSDOMWindow::init(m_context);

    // Set up global object with DOMWindow properties
    JSValue globalObject = JS_GetGlobalObject(m_context);

    // Register all properties directly on global object
    JSDOMWindow::init(m_context);
    // Register only essential DOMWindow properties on global
    {
        extern void initEssentialDOMWindowProperties(JSContext* ctx, JSValue global);
        initEssentialDOMWindowProperties(m_context, globalObject);
    }
    // Window functions override DOMWindow's generated versions (our impl has better null safety)
    WindowPrototype::initPrototype(m_context, globalObject);

    // Create a QJS interpreter for this frame
    m_script = new ScriptInterpreter(m_context, globalObject, m_frame);
    JS_SetContextOpaque(m_context, m_script.get());

    // Register Window functions (alert, setTimeout, etc.) on global object
    WindowPrototype::initPrototype(m_context, globalObject);

    // Create and register the Window object
    Window* window = new Window(m_frame->domWindow());
    Window::storeWindow(window);

    JS_FreeValue(m_context, globalObject);

    m_frame->loader()->dispatchWindowObjectAvailable();

#if ENABLE(JSNATIVEBINDING)            
    (void)initNativeJSObjects(m_script);
#endif
}
    
void ScriptController::clearDocumentWrapper() 
{
    if (!m_script)
        return;

    JSAtom atom = JS_NewAtom(m_context, "document");
    JS_DeleteProperty(m_context, m_script->globalObject(), atom, 0);
    JS_FreeAtom(m_context, atom);
}

}
#endif

// Minimal DOMWindow properties registered on global object
// Full list causes memory corruption due to QuickJS global object limitations
namespace WebCore {
static JSValue js_get_document(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    QJS::ScriptInterpreter* interp = (QJS::ScriptInterpreter*)JS_GetContextOpaque(ctx);
    if (!interp || !interp->frame() || !interp->frame()->domWindow())
        return JS_UNDEFINED;
    WebCore::Document* doc = interp->frame()->domWindow()->document(); return doc ? toJS(ctx, static_cast<WebCore::Node*>(doc)) : JS_NULL;
}

void initEssentialDOMWindowProperties(JSContext* ctx, JSValue global)
{
    JS_SetPropertyStr(ctx, global, "window", JS_DupValue(ctx, global));
    JS_SetPropertyStr(ctx, global, "self", JS_DupValue(ctx, global));

    // document getter
    JSAtom atom = JS_NewAtom(ctx, "document");
    JSValue getter = JS_NewCFunction(ctx, (JSCFunction*)js_get_document, "get document", 0);
    JS_DefinePropertyGetSet(ctx, global, atom, getter, JS_UNDEFINED, JS_PROP_HAS_GET | JS_PROP_ENUMERABLE);
    JS_FreeAtom(ctx, atom);
}
} // namespace WebCore
