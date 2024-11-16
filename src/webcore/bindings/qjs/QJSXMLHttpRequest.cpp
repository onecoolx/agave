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

#include "QJSXMLHttpRequest.h"

#include "DOMWindow.h"
#include "Event.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "HTMLDocument.h"
#include "XMLHttpRequest.h"
#include "QJSDocument.h"
#include "QJSEvent.h"
#include "qjs_events.h"
#include "qjs_window.h"

namespace QJS {

using namespace WebCore;

#if ENABLE(AJAX)

#define countof(x) (sizeof(x) / sizeof((x)[0]))

////////////////////// JSXMLHttpRequest Object ////////////////////////

/* Source for JSXMLHttpRequestPrototypeTable.
@begin JSXMLHttpRequestPrototypeTable 7
  abort                 JSXMLHttpRequest::Abort                   DontDelete|Function 0
  getAllResponseHeaders JSXMLHttpRequest::GetAllResponseHeaders   DontDelete|Function 0
  getResponseHeader     JSXMLHttpRequest::GetResponseHeader       DontDelete|Function 1
  open                  JSXMLHttpRequest::Open                    DontDelete|Function 5
  overrideMimeType      JSXMLHttpRequest::OverrideMIMEType        DontDelete|Function 1
  send                  JSXMLHttpRequest::Send                    DontDelete|Function 1
  setRequestHeader      JSXMLHttpRequest::SetRequestHeader        DontDelete|Function 2
# from the EventTarget interface
# FIXME: add DOM3 EventTarget methods (addEventListenerNS, removeEventListenerNS).
  addEventListener      JSXMLHttpRequest::AddEventListener        DontDelete|Function 3
  removeEventListener   JSXMLHttpRequest::RemoveEventListener     DontDelete|Function 3
  dispatchEvent         JSXMLHttpRequest::DispatchEvent           DontDelete|Function 1
@end
*/

/* Source for JSXMLHttpRequestTable.
@begin JSXMLHttpRequestTable 7
  readyState            JSXMLHttpRequest::ReadyState              DontDelete|ReadOnly
  responseText          JSXMLHttpRequest::ResponseText            DontDelete|ReadOnly
  responseXML           JSXMLHttpRequest::ResponseXML             DontDelete|ReadOnly
  status                JSXMLHttpRequest::Status                  DontDelete|ReadOnly
  statusText            JSXMLHttpRequest::StatusText              DontDelete|ReadOnly
  onreadystatechange    JSXMLHttpRequest::Onreadystatechange      DontDelete
  onload                JSXMLHttpRequest::Onload                  DontDelete
@end
*/

static const JSCFunctionListEntry JSXMLHttpRequestFunctions[] =
{
    //JS_CFUNC_MAGIC_DEF("importStylesheet", 1, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::ImportStylesheet),
    //JS_CGETSET_MAGIC_DEF("size", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::SizeAttrNum),
    //JS_CGETSET_MAGIC_DEF("form", JSHTMLSelectElement::getValueProperty, NULL, JSHTMLSelectElement::FormAttrNum),
};

static JSClassDef JSXMLHttpRequestClassDefine = 
{
    "XMLHttpRequest",
    .finalizer = JSXMLHttpRequest::finalizer,
    .gc_mark = JSXMLHttpRequest::mark,
};

JSClassID JSXMLHttpRequest::js_class_id = 0;

void JSXMLHttpRequest::init(JSContext* ctx, Document* d)
{
    if (JSXMLHttpRequest::js_class_id == 0) {
        JS_NewClassID(&JSXMLHttpRequest::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXMLHttpRequest::js_class_id, &JSXMLHttpRequestClassDefine);

        JSValue proto = JS_NewObject(ctx);
        JS_SetPropertyFunctionList(ctx, proto, JSXMLHttpRequestFunctions, countof(JSXMLHttpRequestFunctions));

        JS_SetConstructor(ctx, JSXMLHttpRequestConstructor::self(ctx, d), proto);
        JS_SetClassProto(ctx, JSXMLHttpRequest::js_class_id, proto);
    }
}

JSValue JSXMLHttpRequest::create(JSContext* ctx, Document* d)
{
    JSXMLHttpRequest::init(ctx, d);
    JSValue obj = JS_NewObjectClass(ctx, JSXMLHttpRequest::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }

    RefPtr<XMLHttpRequest> imp = new XMLHttpRequest(d);
    ScriptInterpreter::putDOMObject(imp.get(), obj);
    imp.get()->ref();
    JS_SetOpaque(obj, imp.get());
    return obj;
}

void JSXMLHttpRequest::finalizer(JSRuntime* rt, JSValue val)
{
    XMLHttpRequest* imp = (XMLHttpRequest*)JS_GetOpaque(val, JSXMLHttpRequest::js_class_id);

    imp->setOnReadyStateChangeListener(0);
    imp->setOnLoadListener(0);
    imp->eventListeners().clear();

    ScriptInterpreter::forgetDOMObject(imp);
    imp->deref();
}

JSValue JSXMLHttpRequest::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    ExceptionCode ec = 0;
    XMLHttpRequest* imp = (XMLHttpRequest*)JS_GetOpaque2(ctx, this_val, JSXMLHttpRequest::js_class_id);

    switch (token) {
        case JSXMLHttpRequest::ReadyState:
            return JS_NewInt32(ctx, imp->getReadyState());
        case JSXMLHttpRequest::ResponseText:
            return jsStringOrNull(ctx, imp->getResponseText());
        case JSXMLHttpRequest::ResponseXML:
            if (Document* responseXML = imp->getResponseXML())
                return toJS(ctx, responseXML);
            return JS_NULL;
        case JSXMLHttpRequest::Status: {
            JSValue result = JS_NewInt32(ctx, imp->getStatus(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSXMLHttpRequest::StatusText: {
            JSValue result = jsString(ctx, imp->getStatusText(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSXMLHttpRequest::Onreadystatechange:
            if (JSUnprotectedEventListener* listener = static_cast<JSUnprotectedEventListener*>(imp->onReadyStateChangeListener()))
                return listener->listenerObj();
            return JS_NULL;
        case JSXMLHttpRequest::Onload:
            if (JSUnprotectedEventListener* listener = static_cast<JSUnprotectedEventListener*>(imp->onLoadListener()))
                return listener->listenerObj();
            return JS_NULL;
        default:
            return JS_NULL;
    }
}

JSValue JSXMLHttpRequest::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token)
{
    XMLHttpRequest* imp = (XMLHttpRequest*)JS_GetOpaque2(ctx, this_val, JSXMLHttpRequest::js_class_id);

    switch (token) {
        case Onreadystatechange: {
            Document* doc = imp->document();
            if (!doc)
                return JS_UNDEFINED;
            Frame* frame = doc->frame();
            if (!frame)
                return JS_UNDEFINED;
            imp->setOnReadyStateChangeListener(Window::retrieveWindow(frame)->findOrCreateJSUnprotectedEventListener(val, true));
            break;
        }
        case Onload: {
            Document* doc = imp->document();
            if (!doc)
                return JS_UNDEFINED;
            Frame* frame = doc->frame();
            if (!frame)
                return JS_UNDEFINED;
            imp->setOnLoadListener(Window::retrieveWindow(frame)->findOrCreateJSUnprotectedEventListener(val, true));
            break;
        }
    }
    return JS_UNDEFINED;
}

void JSXMLHttpRequest::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);

    XMLHttpRequest* imp = (XMLHttpRequest*)JS_GetOpaque(val, JSXMLHttpRequest::js_class_id);

    JSUnprotectedEventListener* onReadyStateChangeListener = static_cast<JSUnprotectedEventListener*>(imp->onReadyStateChangeListener());
    JSUnprotectedEventListener* onLoadListener = static_cast<JSUnprotectedEventListener*>(imp->onLoadListener());

    if (onReadyStateChangeListener)
        onReadyStateChangeListener->mark();

    if (onLoadListener)
        onLoadListener->mark();
    
    typedef XMLHttpRequest::EventListenersMap EventListenersMap;
    typedef XMLHttpRequest::ListenerVector ListenerVector;
    EventListenersMap& eventListeners = imp->eventListeners();
    for (EventListenersMap::iterator mapIter = eventListeners.begin(); mapIter != eventListeners.end(); ++mapIter) {
        for (ListenerVector::iterator vecIter = mapIter->second.begin(); vecIter != mapIter->second.end(); ++vecIter) {
            JSUnprotectedEventListener* listener = static_cast<JSUnprotectedEventListener*>(vecIter->get());
            listener->mark();
        }
    }
}

JSValue JSXMLHttpRequestConstructor::self(JSContext* ctx, Document* d)
{
    JSValue func = JS_NewCFunction2(ctx, JSXMLHttpRequestConstructor::construct, "XMLHttpRequest", 0, JS_CFUNC_constructor, 0);
    JS_SetOpaque(func, d);
    return func;
}

JSValue JSXMLHttpRequestConstructor::construct(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{ 
    Document* doc = (Document*)JS_GetOpaque(new_target, 12/*JS_CLASS_C_FUNCTION*/);
    return JSXMLHttpRequest::create(ctx, doc);
}

JSValue JSXMLHttpRequestPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    XMLHttpRequest* request = (XMLHttpRequest*)JS_GetOpaque(this_val, JSXMLHttpRequest::js_class_id);
    if (!request)
        return JS_ThrowTypeError(ctx, "Type error");

    ExceptionCode ec = 0;

    switch (token) {
        case JSXMLHttpRequest::Abort:
            request->abort();
            return JS_UNDEFINED;

        case JSXMLHttpRequest::GetAllResponseHeaders:
            return jsStringOrUndefined(ctx, request->getAllResponseHeaders());

        case JSXMLHttpRequest::GetResponseHeader:
            if (argc < 1)
                return JS_ThrowSyntaxError(ctx, "Not enough arguments");

            return jsStringOrNull(ctx, request->getResponseHeader(valueToString(ctx, argv[0])));

        case JSXMLHttpRequest::Open: {
            if (argc < 2)
                return JS_ThrowSyntaxError(ctx, "Not enough arguments");

            String method = valueToString(ctx, argv[0]);
            Frame* frame = Window::retrieveActive(ctx)->impl()->frame();
            if (!frame)
                return JS_UNDEFINED;
            KURL url = frame->loader()->completeURL(valueToString(ctx, argv[1]));

            bool async = true;
            if (argc >= 3)
                async = valueToBoolean(ctx, argv[2]);

            if (argc >= 4 && !JS_IsUndefined(argv[3])) {
                String user = valueToStringWithNullCheck(ctx, argv[3]);

                if (argc >= 5 && !JS_IsUndefined(argv[4])) {
                    String password = valueToStringWithNullCheck(ctx, argv[4]);
                    request->open(method, url, async, user, password, ec);
                } else
                    request->open(method, url, async, user, ec);
            } else
                request->open(method, url, async, ec);

            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSXMLHttpRequest::Send: {
            String body;

            if (argc >= 1) {
                Document* doc = (Document*)JS_GetOpaque(argv[0], JSDocument::js_class_id);
                if (doc) {
                    body = doc->toString().deprecatedString();
                } else {
                    // converting certain values (like null) to object can set an exception
                    if (JS_HasException(ctx)) {
                        JSValue e = JS_GetException(ctx);
                        JS_FreeValue(ctx, e);
                    } else
                        body = valueToString(ctx, argv[0]);
                }
            }

            request->send(body, ec);
            setDOMException(ctx, ec);

            return JS_UNDEFINED;
        }
        case JSXMLHttpRequest::SetRequestHeader:
            if (argc < 2)
                return JS_ThrowSyntaxError(ctx, "Not enough arguments");

            request->setRequestHeader(valueToString(ctx, argv[0]), valueToString(ctx, argv[1]), ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;

        case JSXMLHttpRequest::OverrideMIMEType:
            if (argc < 1)
                return JS_ThrowSyntaxError(ctx, "Not enough arguments");

            request->overrideMIMEType(valueToString(ctx, argv[0]));
            return JS_UNDEFINED;
        
        case JSXMLHttpRequest::AddEventListener: {
            Document* doc = request->document();
            if (!doc)
                return JS_UNDEFINED;
            Frame* frame = doc->frame();
            if (!frame)
                return JS_UNDEFINED;
            JSUnprotectedEventListener* listener = QJS::Window::retrieveWindow(frame)->findOrCreateJSUnprotectedEventListener(argv[1], true);
            if (!listener)
                return JS_UNDEFINED;
            request->addEventListener(valueToString(ctx, argv[0]), listener, valueToBoolean(ctx, argv[2]));
            return JS_UNDEFINED;
        }
        case JSXMLHttpRequest::RemoveEventListener: {
            Document* doc = request->document();
            if (!doc)
                return JS_UNDEFINED;
            Frame* frame = doc->frame();
            if (!frame)
                return JS_UNDEFINED;
            JSUnprotectedEventListener* listener = QJS::Window::retrieveWindow(frame)->findOrCreateJSUnprotectedEventListener(argv[1], true);
            if (!listener)
                return JS_UNDEFINED;
            request->removeEventListener(valueToString(ctx, argv[0]), listener, valueToBoolean(ctx, argv[2]));
            return JS_UNDEFINED;
        }
        case JSXMLHttpRequest::DispatchEvent: {
            bool result = request->dispatchEvent(toEvent(argv[0]), ec);
            setDOMException(ctx, ec);
            return JS_NewBool(ctx, result);
        }
    }

    return JS_UNDEFINED;
}
#endif // ENABLE(AJAX)

} // end namespace
