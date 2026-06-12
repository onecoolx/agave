/*
 * Agave - A lightweight web browser engine
 *
 * Copyright (c) 2026, Zhang Ji Peng
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

#if ENABLE(WEB_SOCKETS)

#include "qjs_websocket.h"

#include "ExceptionCode.h"
#include "PlatformString.h"
#include "WebSocket.h"
#include "qjs_binding.h"

namespace WebCore {

using namespace QJS;

static JSClassID gWebSocketClassId = 0;

// Bridges WebSocket lifecycle events to the JS object's on* handlers. Owns a
// ref to the WebSocket and holds (does not own) the JS wrapper object so it can
// read onopen/onmessage/onclose/onerror at dispatch time.
class WSBinding : public WebSocketClient {
public:
    WSBinding(JSContext* ctx, JSValue jsObject)
        : m_ctx(ctx), m_jsObject(jsObject) { }
    ~WSBinding() { }

    void setSocket(PassRefPtr<WebSocket> ws) { m_socket = ws; }
    WebSocket* socket() const { return m_socket.get(); }

    virtual void didOpen() { dispatch("onopen", JS_UNDEFINED, 0); }
    virtual void didReceiveMessage(const String& message)
    {
        JSValue ev = JS_NewObject(m_ctx);
        JS_SetPropertyStr(m_ctx, ev, "data", jsString(m_ctx, message));
        dispatch("onmessage", ev, 1);
    }
    virtual void didClose() { dispatch("onclose", JS_UNDEFINED, 0); }
    virtual void didError() { dispatch("onerror", JS_UNDEFINED, 0); }

private:
    // Calls the named handler if it is callable, then drains microtasks so any
    // promises chained inside the handler settle.
    void dispatch(const char* handlerName, JSValue eventArg, int argc)
    {
        JSValue handler = JS_GetPropertyStr(m_ctx, m_jsObject, handlerName);
        if (JS_IsFunction(m_ctx, handler)) {
            JSValue r = JS_Call(m_ctx, handler, m_jsObject, argc, argc ? &eventArg : 0);
            JS_FreeValue(m_ctx, r);
            JSRuntime* rt = JS_GetRuntime(m_ctx);
            JSContext* cctx = 0;
            int guard = 100000;
            while (guard-- > 0 && JS_ExecutePendingJob(rt, &cctx) != 0) { }
        }
        JS_FreeValue(m_ctx, handler);
        if (argc)
            JS_FreeValue(m_ctx, eventArg);
    }

    JSContext* m_ctx;
    JSValue m_jsObject; // not owned (owned by the JS runtime)
    RefPtr<WebSocket> m_socket;
};

static void wsFinalizer(JSRuntime*, JSValue val)
{
    WSBinding* b = (WSBinding*)JS_GetOpaque(val, gWebSocketClassId);
    if (b) {
        if (b->socket())
            b->socket()->clearClient();
        delete b;
    }
}

static JSValue wsSend(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    WSBinding* b = (WSBinding*)JS_GetOpaque2(ctx, this_val, gWebSocketClassId);
    if (!b || !b->socket())
        return JS_ThrowTypeError(ctx, "Invalid WebSocket");
    if (argc < 1)
        return JS_UNDEFINED;
    String msg = valueToString(ctx, argv[0]);
    ExceptionCode ec = 0;
    b->socket()->send(msg, ec);
    if (ec)
        return JS_ThrowTypeError(ctx, "WebSocket is not open");
    return JS_UNDEFINED;
}

static JSValue wsClose(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    WSBinding* b = (WSBinding*)JS_GetOpaque2(ctx, this_val, gWebSocketClassId);
    if (b && b->socket())
        b->socket()->close();
    return JS_UNDEFINED;
}

static JSValue wsGetReadyState(JSContext* ctx, JSValueConst this_val)
{
    WSBinding* b = (WSBinding*)JS_GetOpaque2(ctx, this_val, gWebSocketClassId);
    if (!b || !b->socket())
        return JS_NewInt32(ctx, WebSocket::CLOSED);
    return JS_NewInt32(ctx, (int)b->socket()->readyState());
}

static JSValue wsGetUrl(JSContext* ctx, JSValueConst this_val)
{
    WSBinding* b = (WSBinding*)JS_GetOpaque2(ctx, this_val, gWebSocketClassId);
    if (!b || !b->socket())
        return jsString(ctx, String());
    return jsString(ctx, b->socket()->url());
}

static JSValue wsConstructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
    if (argc < 1)
        return JS_ThrowTypeError(ctx, "WebSocket requires a url argument");

    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, gWebSocketClassId);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        return obj;

    String url = valueToString(ctx, argv[0]);
    WSBinding* binding = new WSBinding(ctx, obj);
    RefPtr<WebSocket> ws = WebSocket::create(binding);
    binding->setSocket(ws);
    JS_SetOpaque(obj, binding);

    // Constants on the instance for convenience (CONNECTING/OPEN/CLOSING/CLOSED).
    JS_SetPropertyStr(ctx, obj, "CONNECTING", JS_NewInt32(ctx, WebSocket::CONNECTING));
    JS_SetPropertyStr(ctx, obj, "OPEN", JS_NewInt32(ctx, WebSocket::OPEN));
    JS_SetPropertyStr(ctx, obj, "CLOSING", JS_NewInt32(ctx, WebSocket::CLOSING));
    JS_SetPropertyStr(ctx, obj, "CLOSED", JS_NewInt32(ctx, WebSocket::CLOSED));

    ExceptionCode ec = 0;
    ws->connect(url, ec);
    return obj;
}

void registerWebSocket(JSContext* ctx, JSValue global)
{
    if (gWebSocketClassId == 0) {
        JS_NewClassID(&gWebSocketClassId);
        JSClassDef def;
        memset(&def, 0, sizeof(def));
        def.class_name = "WebSocket";
        def.finalizer = wsFinalizer;
        JS_NewClass(JS_GetRuntime(ctx), gWebSocketClassId, &def);
    }

    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, proto, "send", JS_NewCFunction(ctx, wsSend, "send", 1));
    JS_SetPropertyStr(ctx, proto, "close", JS_NewCFunction(ctx, wsClose, "close", 0));

    JSAtom rs = JS_NewAtom(ctx, "readyState");
    JS_DefinePropertyGetSet(ctx, proto, rs,
        JS_NewCFunction(ctx, (JSCFunction*)wsGetReadyState, "get readyState", 0), JS_UNDEFINED,
        JS_PROP_HAS_GET | JS_PROP_ENUMERABLE);
    JS_FreeAtom(ctx, rs);

    JSAtom ua = JS_NewAtom(ctx, "url");
    JS_DefinePropertyGetSet(ctx, proto, ua,
        JS_NewCFunction(ctx, (JSCFunction*)wsGetUrl, "get url", 0), JS_UNDEFINED,
        JS_PROP_HAS_GET | JS_PROP_ENUMERABLE);
    JS_FreeAtom(ctx, ua);

    JS_SetClassProto(ctx, gWebSocketClassId, proto);

    JSValue ctor = JS_NewCFunction2(ctx, wsConstructor, "WebSocket", 1, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, ctor, proto);
    JS_SetPropertyStr(ctx, global, "WebSocket", ctor);
}

} // namespace WebCore

#endif // ENABLE(WEB_SOCKETS)
