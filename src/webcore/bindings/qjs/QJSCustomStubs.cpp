/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
 * All rights reserved.
 */

#include "config.h"

#if ENABLE(QJS)

#include "QJSEventTargetNode.h"
#include "QJSHTMLInputElementBase.h"
#include "QJSDocument.h"
#include "QJSEvent.h"
#include "QJSNodeIterator.h"
#include "QJSTreeWalker.h"
#include "QJSHTMLOptionElementConstructor.h"
#include "QJSHTMLOptionsCollection.h"
#include "QJSHTMLSelectElement.h"
#include "QJSCustomXPathNSResolver.h"
#include "QJSDOMExceptionConstructor.h"

#include "Document.h"
#include "DOMWindow.h"
#include "Event.h"
#include "ExceptionCode.h"
#include "Frame.h"
#include "HTMLOptionElement.h"
#include "HTMLSelectElement.h"
#include "HTMLOptionsCollection.h"
#include "NodeIterator.h"
#include "Text.h"
#include "TreeWalker.h"

#include "qjs_window.h"
#include "qjs_binding.h"
#include "qjs_script.h"
#include "QJSNode.h"

using namespace QJS;

namespace WebCore {

// JSEventTargetNode

JSClassID JSEventTargetNode::js_class_id = 0;

void JSEventTargetNode::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
}

JSValue JSEventTargetNodePrototype::self(JSContext *ctx)
{
    return JSNodePrototype::self(ctx);
}

// JSHTMLInputElementBase

JSClassID JSHTMLInputElementBase::js_class_id = 0;

void JSHTMLInputElementBase::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
}

JSValue JSHTMLInputElementBasePrototype::self(JSContext *ctx)
{
    return JS_NULL;
}

// JSDocument custom mark

void JSDocument::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
}

// JSEvent custom

JSValue JSEvent::clipboardData(JSContext *ctx, JSValueConst this_val, Event *impl)
{
    return JS_NULL;
}

// JSNodeIterator custom mark

void JSNodeIterator::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
}

// JSTreeWalker custom mark

void JSTreeWalker::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
}

// JSHTMLOptionElementConstructor

static JSValue qjs_option_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv)
{
    // Get document from the global Window object
    QJS::Window* window = QJS::Window::retrieveActive(ctx);
    if (!window || !window->impl()->frame())
        return JS_EXCEPTION;

    Document* doc = window->impl()->frame()->document();
    if (!doc)
        return JS_EXCEPTION;

    ExceptionCode ec = 0;
    RefPtr<Element> el = doc->createElement("option", ec);
    if (ec || !el)
        return JS_EXCEPTION;

    HTMLOptionElement* opt = static_cast<HTMLOptionElement*>(el.get());
    RefPtr<Text> text = doc->createTextNode("");
    opt->appendChild(text, ec);

    if (!ec && argc > 0)
        text->setData(valueToString(ctx, argv[0]), ec);
    if (!ec && argc > 1)
        opt->setValue(valueToString(ctx, argv[1]));
    if (!ec && argc > 2)
        opt->setDefaultSelected(JS_ToBool(ctx, argv[2]));
    if (!ec && argc > 3)
        opt->setSelected(JS_ToBool(ctx, argv[3]));

    if (ec) {
        setDOMException(ctx, ec);
        return JS_EXCEPTION;
    }

    return toJS(ctx, static_cast<Node*>(opt));
}

JSValue JSHTMLOptionElementConstructor::self(JSContext* ctx, Document* doc)
{
    return JS_NewCFunction2(ctx, qjs_option_constructor, "Option", 4, JS_CFUNC_constructor, 0);
}

// JSHTMLOptionsCollection custom

JSValue JSHTMLOptionsCollection::length(JSContext *ctx, JSValueConst this_val, HTMLOptionsCollection *impl)
{
    if (!impl)
        return JS_NewInt32(ctx, 0);
    return JS_NewInt32(ctx, impl->length());
}

void JSHTMLOptionsCollection::setLength(JSContext *ctx, JSValueConst this_val, JSValue value, HTMLOptionsCollection *impl)
{
    if (!impl)
        return;
    ExceptionCode ec = 0;
    impl->setLength(valueToInt32(ctx, value), ec);
    setDOMException(ctx, ec);
}

// JSHTMLSelectElement custom

JSValue JSHTMLSelectElement::remove(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, HTMLSelectElement *impl)
{
    if (!impl || argc < 1)
        return JS_UNDEFINED;
    impl->remove(valueToInt32(ctx, argv[0]));
    return JS_UNDEFINED;
}

// JSCustomXPathNSResolver

PassRefPtr<JSCustomXPathNSResolver> JSCustomXPathNSResolver::create(JSContext* ctx, JSValue resolver)
{
    if (JS_IsUndefined(resolver) || JS_IsNull(resolver))
        return 0;

    if (!JS_IsObject(resolver)) {
        setDOMException(ctx, TYPE_MISMATCH_ERR);
        return 0;
    }

    QJS::Window* window = QJS::Window::retrieveActive(ctx);
    Frame* frame = window ? window->impl()->frame() : 0;
    return new JSCustomXPathNSResolver(resolver, frame);
}

JSCustomXPathNSResolver::JSCustomXPathNSResolver(JSValue resolver, Frame* frame)
    : m_customResolver(resolver)
    , m_frame(frame)
{
}

JSCustomXPathNSResolver::~JSCustomXPathNSResolver()
{
}

String JSCustomXPathNSResolver::lookupNamespaceURI(const String& prefix)
{
    if (!m_frame)
        return String();

    ScriptController* script = m_frame->script();
    if (!script)
        return String();

    JSContext* ctx = script->context();
    JSValue func = JS_GetPropertyStr(ctx, m_customResolver, "lookupNamespaceURI");

    if (!JS_IsFunction(ctx, func)) {
        JS_FreeValue(ctx, func);
        if (!JS_IsFunction(ctx, m_customResolver))
            return String();
        func = JS_DupValue(ctx, m_customResolver);
    }

    JSValue arg = JS_NewString(ctx, prefix.utf8().data());
    JSValue ret = JS_Call(ctx, func, m_customResolver, 1, &arg);
    JS_FreeValue(ctx, arg);
    JS_FreeValue(ctx, func);

    String result;
    if (!JS_IsUndefined(ret) && !JS_IsNull(ret) && !JS_IsException(ret)) {
        const char* str = JS_ToCString(ctx, ret);
        if (str) {
            result = String(str);
            JS_FreeCString(ctx, str);
        }
    }
    JS_FreeValue(ctx, ret);
    return result;
}

// JSDOMExceptionConstructor

JSClassID JSDOMExceptionConstructor::js_class_id = 0;

JSValue JSDOMExceptionConstructor::create(JSContext* ctx)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "INDEX_SIZE_ERR", JS_NewInt32(ctx, INDEX_SIZE_ERR));
    JS_SetPropertyStr(ctx, obj, "DOMSTRING_SIZE_ERR", JS_NewInt32(ctx, DOMSTRING_SIZE_ERR));
    JS_SetPropertyStr(ctx, obj, "HIERARCHY_REQUEST_ERR", JS_NewInt32(ctx, HIERARCHY_REQUEST_ERR));
    JS_SetPropertyStr(ctx, obj, "WRONG_DOCUMENT_ERR", JS_NewInt32(ctx, WRONG_DOCUMENT_ERR));
    JS_SetPropertyStr(ctx, obj, "INVALID_CHARACTER_ERR", JS_NewInt32(ctx, INVALID_CHARACTER_ERR));
    JS_SetPropertyStr(ctx, obj, "NO_DATA_ALLOWED_ERR", JS_NewInt32(ctx, NO_DATA_ALLOWED_ERR));
    JS_SetPropertyStr(ctx, obj, "NO_MODIFICATION_ALLOWED_ERR", JS_NewInt32(ctx, NO_MODIFICATION_ALLOWED_ERR));
    JS_SetPropertyStr(ctx, obj, "NOT_FOUND_ERR", JS_NewInt32(ctx, NOT_FOUND_ERR));
    JS_SetPropertyStr(ctx, obj, "NOT_SUPPORTED_ERR", JS_NewInt32(ctx, NOT_SUPPORTED_ERR));
    JS_SetPropertyStr(ctx, obj, "INUSE_ATTRIBUTE_ERR", JS_NewInt32(ctx, INUSE_ATTRIBUTE_ERR));
    JS_SetPropertyStr(ctx, obj, "INVALID_STATE_ERR", JS_NewInt32(ctx, INVALID_STATE_ERR));
    JS_SetPropertyStr(ctx, obj, "SYNTAX_ERR", JS_NewInt32(ctx, SYNTAX_ERR));
    JS_SetPropertyStr(ctx, obj, "INVALID_MODIFICATION_ERR", JS_NewInt32(ctx, INVALID_MODIFICATION_ERR));
    JS_SetPropertyStr(ctx, obj, "NAMESPACE_ERR", JS_NewInt32(ctx, NAMESPACE_ERR));
    JS_SetPropertyStr(ctx, obj, "INVALID_ACCESS_ERR", JS_NewInt32(ctx, INVALID_ACCESS_ERR));
    return obj;
}

void JSDOMExceptionConstructor::init(JSContext* ctx)
{
}

JSValue JSDOMExceptionConstructor::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    return JS_NewInt32(ctx, token);
}

} // namespace WebCore

#endif
