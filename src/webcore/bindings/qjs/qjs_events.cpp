/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
 * All rights reserved.
 */

#include "config.h"

#if ENABLE(QJS)

#include "qjs_events.h"
#include "qjs_window.h"
#include "qjs_script.h"

#include "Document.h"
#include "DOMWindow.h"
#include "Event.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "Page.h"

using namespace QJS;

namespace WebCore {

// JSAbstractEventListener

JSAbstractEventListener::JSAbstractEventListener(bool html)
    : m_html(html)
{
}

void JSAbstractEventListener::handleEvent(Event* event, bool isWindowEvent)
{
    JSValue listener = listenerObj();
    if (JS_IsNull(listener) || JS_IsUndefined(listener))
        return;

    Window* window = windowObj();
    if (!window)
        return;

    Frame* frame = window->impl()->frame();
    if (!frame)
        return;

    ScriptController* script = frame->script();
    if (!script)
        return;

    JSContext* ctx = script->context();
    if (!ctx)
        return;

    JSValue jsEvent = ScriptInterpreter::getDOMObject(event);
    if (JS_IsNull(jsEvent))
        return;

    JSValue thisObj = isWindowEvent ? script->interpreter()->globalObject() : jsEvent;
    JSValue ret = JS_Call(ctx, listener, thisObj, 1, &jsEvent);
    if (JS_IsException(ret)) {
        JS_FreeValue(ctx, JS_GetException(ctx));
    }
    JS_FreeValue(ctx, ret);
}

bool JSAbstractEventListener::isHTMLEventListener() const
{
    return m_html;
}

// JSUnprotectedEventListener

JSUnprotectedEventListener::JSUnprotectedEventListener(JSValue listener, Window* win, bool html)
    : JSAbstractEventListener(html)
    , m_listener(listener)
    , m_win(win)
{
}

JSUnprotectedEventListener::~JSUnprotectedEventListener()
{
}

JSValue JSUnprotectedEventListener::listenerObj() const
{
    return m_listener;
}

Window* JSUnprotectedEventListener::windowObj() const
{
    return m_win;
}

void JSUnprotectedEventListener::clearWindowObj()
{
    m_win = 0;
}

void JSUnprotectedEventListener::mark()
{
}

// JSEventListener

JSEventListener::JSEventListener(JSValue listener, Window* win, bool html)
    : JSAbstractEventListener(html)
    , m_listener(listener)
    , m_ctx(0)
    , m_win(win)
{
    // Protect the listener function from GC by holding a strong reference.
    if (win && win->impl() && win->impl()->frame() && win->impl()->frame()->script()) {
        m_ctx = win->impl()->frame()->script()->context();
        if (m_ctx && !JS_IsNull(m_listener) && !JS_IsUndefined(m_listener))
            m_listener = JS_DupValue(m_ctx, m_listener);
    }
    // Register in the window's listener map so findJSEventListener() can locate
    // this wrapper again (needed by removeEventListener). Keyed by the callback
    // object pointer, matching findJSEventListener's lookup.
    if (win && JS_IsObject(listener)) {
        Window::ListenersMap& listeners = html ? win->jsHTMLEventListeners() : win->jsEventListeners();
        listeners.set(JS_VALUE_GET_PTR(listener), this);
    }
}

JSEventListener::~JSEventListener()
{
    // Unregister from the window's listener map (mirrors the constructor).
    if (m_win && !JS_IsNull(m_listener) && !JS_IsUndefined(m_listener) && JS_IsObject(m_listener)) {
        Window::ListenersMap& listeners = isHTMLEventListener() ? m_win->jsHTMLEventListeners() : m_win->jsEventListeners();
        listeners.remove(JS_VALUE_GET_PTR(m_listener));
    }
    if (m_ctx && !JS_IsNull(m_listener) && !JS_IsUndefined(m_listener))
        JS_FreeValue(m_ctx, m_listener);
}

JSValue JSEventListener::listenerObj() const
{
    return m_listener;
}

Window* JSEventListener::windowObj() const
{
    return m_win;
}

void JSEventListener::clearWindowObj()
{
    m_win = 0;
}

// JSLazyEventListener

JSLazyEventListener::JSLazyEventListener(const String& functionName, const String& code, Window* win, Node* node, int lineNumber)
    : JSEventListener(JS_NULL, win, true)
    , m_functionName(functionName)
    , m_code(code)
    , m_parsed(false)
    , m_lineNumber(lineNumber)
    , m_originalNode(node)
{
}

JSValue JSLazyEventListener::listenerObj() const
{
    parseCode();
    return m_listener;
}

JSValue JSLazyEventListener::eventParameterName() const
{
    return JS_NULL;
}

void JSLazyEventListener::parseCode() const
{
    if (m_parsed)
        return;
    m_parsed = true;

    Window* win = windowObj();
    if (!win)
        return;

    Frame* frame = win->impl()->frame();
    if (!frame)
        return;

    ScriptController* script = frame->script();
    if (!script)
        return;

    JSContext* ctx = script->context();
    m_ctx = ctx;
    String code = "function " + m_functionName + "(event){" + m_code + "}";
    JSValue result = JS_Eval(ctx, code.utf8().data(), code.utf8().length(), "", JS_EVAL_TYPE_GLOBAL);
    if (!JS_IsException(result)) {
        JSValue global = script->interpreter()->globalObject();
        m_listener = JS_GetPropertyStr(ctx, global, m_functionName.utf8().data());
    }
    JS_FreeValue(ctx, result);
}

JSValue getNodeEventListener(Node* node, const AtomicString& eventType)
{
    return JS_NULL;
}

} // namespace WebCore

#endif
