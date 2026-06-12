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

#if ENABLE(FETCH)

#include "qjs_fetch.h"

#include "Document.h"
#include "DOMWindow.h"
#include "Frame.h"
#include "KURL.h"
#include "PlatformString.h"
#include "XMLHttpRequest.h"
#include "qjs_binding.h"
#include "qjs_window.h"

namespace WebCore {

using namespace QJS;

// Builds a JS Response object from a completed XHR. The body is captured as a
// string; text()/json() return already-resolved Promises (the network read has
// already happened, matching the simplified model).
static JSValue createResponse(JSContext* ctx, int status, const String& statusText, const String& body)
{
    JSValue resp = JS_NewObject(ctx);

    JS_SetPropertyStr(ctx, resp, "ok", JS_NewBool(ctx, status >= 200 && status < 300));
    JS_SetPropertyStr(ctx, resp, "status", JS_NewInt32(ctx, status));
    JS_SetPropertyStr(ctx, resp, "statusText", jsString(ctx, statusText));
    // Stash the raw body so text()/json() can read it.
    JS_SetPropertyStr(ctx, resp, "__body", jsString(ctx, body));

    // text(): returns Promise.resolve(this.__body)
    const char* textFn =
        "(function(){return Promise.resolve(this.__body);})";
    JSValue text = JS_Eval(ctx, textFn, strlen(textFn), "<fetch>", JS_EVAL_TYPE_GLOBAL);
    JS_SetPropertyStr(ctx, resp, "text", text);

    // json(): returns Promise.resolve(JSON.parse(this.__body)), rejecting on
    // malformed JSON (JSON.parse throws, which rejects the promise).
    const char* jsonFn =
        "(function(){return new Promise(function(res,rej){"
        "try{res(JSON.parse(this.__body));}catch(e){rej(e);}}.bind(this));})";
    JSValue json = JS_Eval(ctx, jsonFn, strlen(jsonFn), "<fetch>", JS_EVAL_TYPE_GLOBAL);
    JS_SetPropertyStr(ctx, resp, "json", json);

    return resp;
}

// fetch(input[, init]) -> Promise<Response>
static JSValue js_fetch(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    JSValue resolving[2];
    JSValue promise = JS_NewPromiseCapability(ctx, resolving);
    if (JS_IsException(promise))
        return promise;

    Window* window = Window::retrieveActive(ctx);
    if (!window || !window->impl() || !window->impl()->frame()
        || !window->impl()->frame()->document()) {
        JSValue err = JS_NewError(ctx);
        JS_Call(ctx, resolving[1], JS_UNDEFINED, 1, &err);
        JS_FreeValue(ctx, err);
        JS_FreeValue(ctx, resolving[0]);
        JS_FreeValue(ctx, resolving[1]);
        return promise;
    }
    Document* doc = window->impl()->frame()->document();

    if (argc < 1) {
        JSValue err = JS_NewError(ctx);
        JS_Call(ctx, resolving[1], JS_UNDEFINED, 1, &err);
        JS_FreeValue(ctx, err);
        JS_FreeValue(ctx, resolving[0]);
        JS_FreeValue(ctx, resolving[1]);
        return promise;
    }

    String url = valueToString(ctx, argv[0]);

    // Parse init options: method, headers, body.
    String method = "GET";
    String body;
    JSValue headers = JS_UNDEFINED;
    if (argc >= 2 && JS_IsObject(argv[1])) {
        JSValue m = JS_GetPropertyStr(ctx, argv[1], "method");
        if (JS_IsString(m))
            method = valueToString(ctx, m);
        JS_FreeValue(ctx, m);
        JSValue b = JS_GetPropertyStr(ctx, argv[1], "body");
        if (JS_IsString(b))
            body = valueToString(ctx, b);
        JS_FreeValue(ctx, b);
        headers = JS_GetPropertyStr(ctx, argv[1], "headers");
    }

    RefPtr<XMLHttpRequest> xhr = new XMLHttpRequest(doc);
    ExceptionCode ec = 0;
    // fetch resolves asynchronously to the caller, but internally we use a
    // synchronous load: it completes during send() (works for file:// and
    // sync HTTP) and we settle the Promise immediately afterwards. The Promise
    // itself preserves async semantics for the caller's .then() chains.
    xhr->open(method, doc->completeURL(url), false, ec);
    if (ec) {
        JSValue err = JS_NewError(ctx);
        JS_Call(ctx, resolving[1], JS_UNDEFINED, 1, &err);
        JS_FreeValue(ctx, err);
        JS_FreeValue(ctx, resolving[0]);
        JS_FreeValue(ctx, resolving[1]);
        if (!JS_IsUndefined(headers)) JS_FreeValue(ctx, headers);
        return promise;
    }

    // Apply request headers from a plain object { name: value }.
    if (JS_IsObject(headers)) {
        JSPropertyEnum* tab = 0;
        uint32_t len = 0;
        if (!JS_GetOwnPropertyNames(ctx, &tab, &len, headers, JS_GPN_STRING_MASK | JS_GPN_ENUM_ONLY)) {
            for (uint32_t i = 0; i < len; i++) {
                JSValue key = JS_AtomToString(ctx, tab[i].atom);
                JSValue val = JS_GetProperty(ctx, headers, tab[i].atom);
                String name = valueToString(ctx, key);
                String value = valueToString(ctx, val);
                ExceptionCode hec = 0;
                xhr->setRequestHeader(name, value, hec);
                JS_FreeValue(ctx, key);
                JS_FreeValue(ctx, val);
                JS_FreeAtom(ctx, tab[i].atom);
            }
            js_free(ctx, tab);
        }
    }
    if (!JS_IsUndefined(headers))
        JS_FreeValue(ctx, headers);

    // Synchronous load: completes during send(). Then settle the Promise with a
    // Response (or reject on network error). The Promise keeps async semantics
    // for the caller; no listener is needed since the result is ready here.
    xhr->send(body, ec);

    JSValue settleArg;
    JSValue settleFn;
    int status = xhr->getStatus(ec);
    bool loaded = (xhr->getReadyState() == Loaded);
    // A local (file://) resource loads with no HTTP status; treat a completed
    // load as success and synthesize status 200.
    if (!loaded && status == 0) {
        settleFn = resolving[1]; // reject
        settleArg = JS_NewError(ctx);
        JS_SetPropertyStr(ctx, settleArg, "message", jsString(ctx, "Network request failed"));
    } else {
        ExceptionCode sec = 0;
        int reportedStatus = status > 0 ? status : 200;
        String statusText = status > 0 ? xhr->getStatusText(sec) : String("OK");
        String respBody = xhr->getResponseText();
        settleFn = resolving[0]; // resolve
        settleArg = createResponse(ctx, reportedStatus, statusText, respBody);
    }
    JSValue callRet = JS_Call(ctx, settleFn, JS_UNDEFINED, 1, &settleArg);
    JS_FreeValue(ctx, callRet);
    JS_FreeValue(ctx, settleArg);
    JS_FreeValue(ctx, resolving[0]);
    JS_FreeValue(ctx, resolving[1]);

    return promise;
}

void registerFetch(JSContext* ctx, JSValue global)
{
    JSValue fn = JS_NewCFunction(ctx, js_fetch, "fetch", 2);
    JS_SetPropertyStr(ctx, global, "fetch", fn);
}

} // namespace WebCore

#endif // ENABLE(FETCH)
