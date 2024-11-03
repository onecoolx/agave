/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(QJS)
#include "qjs_dom.h"

#include "Document.h"
#include "EventTarget.h"
#include "Frame.h"
#include "HTMLNames.h"
#include "HTMLPlugInElement.h"
#include "QJSAttr.h"
#include "QJSNode.h"
#include "XMLHttpRequest.h"
#include "qjs_events.h"
#include "qjs_window.h"

#if ENABLE(SVG)
#include "JSSVGElementInstance.h"
#endif

#if USE(JAVASCRIPTCORE_BINDINGS)
#include "runtime_object.h"
#endif

namespace WebCore {

using namespace QJS;
using namespace HTMLNames;

Attr* toAttr(JSValue val, bool& ok)
{
    if (!JS_IsObject(val)) {
        ok = false;
        return 0;
    }

    Attr* impl = (Attr*)JS_GetOpaque(val, JSAttr::js_class_id);
    if (!impl) {
        ok = false;
        return 0;
    }

    ok = true;
    return impl;
}

bool checkNodeSecurity(JSContext* ctx, Node* n)
{
  if (!n) 
    return false;

  // Check to see if the currently executing interpreter is allowed to access the specified node
  Window* win = Window::retrieveWindow(n->document()->frame());
  return win && win->isSafeScript(ctx);
}

JSValue toJS(JSContext* ctx, EventTarget* target)
{
    if (!target)
        return JS_NULL;
    
#if ENABLE(SVG)
    // SVGElementInstance supports both toSVGElementInstance and toNode since so much mouse handling code depends on toNode returning a valid node.
    SVGElementInstance* instance = target->toSVGElementInstance();
    if (instance)
        return toJS(ctx, instance);
#endif
    
    Node* node = target->toNode();
    if (node)
        return toJS(ctx, node);

#if ENABLE(AJAX)
    XMLHttpRequest* xhr = target->toXMLHttpRequest();
    if (xhr) {
        // XMLHttpRequest is always created via JS, so we don't need to use cacheDOMObject() here.
        QJS::ScriptInterpreter* interp = static_cast<QJS::ScriptInterpreter*>(JS_GetContextOpaque(ctx));
        return QJS::ScriptInterpreter::getDOMObject(interp, xhr);
    }
#endif

    // There are two kinds of EventTargets: EventTargetNode and XMLHttpRequest.
    // If SVG support is enabled, there is also SVGElementInstance.
    ASSERT(0);
    return JS_NULL;
}

JSValue getRuntimeObject(JSContext* ctx, Node* n)
{
    if (!n)
        return JS_NULL;

#if USE(JAVASCRIPTCORE_BINDINGS)
    if (n->hasTagName(objectTag) || n->hasTagName(embedTag) || n->hasTagName(appletTag)) {
        HTMLPlugInElement* plugInElement = static_cast<HTMLPlugInElement*>(n);
        if (plugInElement->getInstance() && plugInElement->getInstance()->rootObject())
            // The instance is owned by the PlugIn element.
            return RuntimeObjectImp::create(ctx, plugInElement->getInstance());
    }
#endif

    // If we don't have a runtime object return 0.
    return JS_NULL;
}

} // namespace WebCore
#endif
