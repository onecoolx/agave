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
#include "Event.h"
#include "HTMLSelectElement.h"
#include "HTMLOptionsCollection.h"
#include "NodeIterator.h"
#include "TreeWalker.h"

#include "qjs_window.h"
#include "qjs_binding.h"

using namespace QJS;

namespace WebCore {

// JSEventTargetNode

JSClassID JSEventTargetNode::js_class_id = 0;

void JSEventTargetNode::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
}

JSValue JSEventTargetNodePrototype::self(JSContext *ctx)
{
    return JS_NULL;
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

JSValue JSHTMLOptionElementConstructor::self(JSContext* ctx, Document* doc)
{
    return JS_NULL; // TODO: implement
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
    return 0; // TODO: implement
}

// JSDOMExceptionConstructor

JSClassID JSDOMExceptionConstructor::js_class_id = 0;

JSValue JSDOMExceptionConstructor::create(JSContext* ctx)
{
    return JS_NULL; // TODO: implement
}

void JSDOMExceptionConstructor::init(JSContext* ctx)
{
}

JSValue JSDOMExceptionConstructor::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    return JS_UNDEFINED;
}

} // namespace WebCore

#endif
