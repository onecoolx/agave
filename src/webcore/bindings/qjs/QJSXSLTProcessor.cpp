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

#if ENABLE(XSLT)

#include "QJSXSLTProcessor.h"

#include "XSLTProcessor.h"
#include "Document.h"
#include "DocumentFragment.h"
#include "QJSDocument.h"
#include "qjs_dom.h"

using namespace WebCore;

namespace QJS {

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static const JSCFunctionListEntry JSXSLTProcessorFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("importStylesheet", 1, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::ImportStylesheet),
    JS_CFUNC_MAGIC_DEF("transformToDocument", 1, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::TransformToDocument),
    JS_CFUNC_MAGIC_DEF("transformToFragment", 2, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::TransformToFragment),
    JS_CFUNC_MAGIC_DEF("setParameter", 3, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::SetParameter),
    JS_CFUNC_MAGIC_DEF("getParameter", 2, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::GetParameter),
    JS_CFUNC_MAGIC_DEF("removeParameter", 2, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::RemoveParameter),
    JS_CFUNC_MAGIC_DEF("clearParameters", 0, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::ClearParameters),
    JS_CFUNC_MAGIC_DEF("reset", 0, JSXSLTProcessorPrototypeFunction::callAsFunction, JSXSLTProcessor::Reset),
};

static JSClassDef JSXSLTProcessorClassDefine = 
{
    "XSLTProcessor",
    .finalizer = JSXSLTProcessor::finalizer,
};

JSClassID JSXSLTProcessor::js_class_id = 0;

void JSXSLTProcessor::init(JSContext* ctx)
{
    if (JSXSLTProcessor::js_class_id == 0) {
        JS_NewClassID(&JSXSLTProcessor::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXSLTProcessor::js_class_id, &JSXSLTProcessorClassDefine);

        JSValue proto = JS_NewObject(ctx);
        JS_SetPropertyFunctionList(ctx, proto, JSXSLTProcessorFunctions, countof(JSXSLTProcessorFunctions));

        JS_SetConstructor(ctx, JSXSLTProcessorConstructor::self(ctx), proto);
        JS_SetClassProto(ctx, JSXSLTProcessor::js_class_id, proto);
    }
}

JSValue JSXSLTProcessor::create(JSContext* ctx)
{
    JSXSLTProcessor::init(ctx);
    JSValue obj = JS_NewObjectClass(ctx, JSXSLTProcessor::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }

    RefPtr<XSLTProcessor> imp = new XSLTProcessor();
    imp.get()->ref();
    JS_SetOpaque(obj, imp.get());
    return obj;
}

void JSXSLTProcessor::finalizer(JSRuntime* rt, JSValue val)
{
    XSLTProcessor* imp = (XSLTProcessor*)JS_GetOpaque(val, JSXSLTProcessor::js_class_id);
    ScriptInterpreter::forgetDOMObject(imp);
    imp->deref();
}

WebCore::XSLTProcessor* JSXSLTProcessor::impl(JSContext* ctx, JSValue val)
{
    XSLTProcessor* imp = (XSLTProcessor*)JS_GetOpaque2(ctx, val, JSXSLTProcessor::js_class_id);
    return imp;
}

JSValue JSXSLTProcessorConstructor::self(JSContext* ctx) {
    return JS_NewCFunction2(ctx, JSXSLTProcessorConstructor::construct, "XSLTProcessor", 0, JS_CFUNC_constructor, 0);
}

JSValue JSXSLTProcessorConstructor::construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv)
{
    return JSXSLTProcessor::create(ctx);
}

JSValue JSXSLTProcessorPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    XSLTProcessor* imp = (XSLTProcessor*)JS_GetOpaque2(ctx, this_val, JSXSLTProcessor::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSXSLTProcessor::ImportStylesheet:
        {
            Node* node = (Node*)JS_GetOpaque(argv[0], JSNode::js_class_id);
            if (node) {
                imp->importStylesheet(node);
                return JS_UNDEFINED;
            }
            break;
        }
        case JSXSLTProcessor::TransformToFragment:
        {
            Node* node = (Node*)JS_GetOpaque(argv[0], JSNode::js_class_id);
            Document* doc = (Document*)JS_GetOpaque(argv[1], JSDocument::js_class_id);
            if (node && doc) {
                return toJS(ctx, imp->transformToFragment(node, doc).get());
            }
            break;
        }
        case JSXSLTProcessor::TransformToDocument:
        {
            Node* node = (Node*)JS_GetOpaque(argv[0], JSNode::js_class_id);
            if (node) {
                RefPtr<Document> resultDocument = imp->transformToDocument(node);
                if (resultDocument)
                    return toJS(ctx, resultDocument.get());
                return JS_UNDEFINED;
            }
            break;
        }
        case JSXSLTProcessor::SetParameter:
        {
            if (JS_IsUndefined(argv[1]) || JS_IsNull(argv[1])
             || JS_IsUndefined(argv[2]) || JS_IsNull(argv[2]))
                return JS_UNDEFINED;

            String namespaceURI = valueToString(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);
            String value = valueToString(ctx, argv[2]);
            imp->setParameter(namespaceURI, localName, value);
            return JS_UNDEFINED;
        }
        case JSXSLTProcessor::GetParameter:
        {
            if (JS_IsUndefined(argv[1]) || JS_IsNull(argv[1]))
                return JS_UNDEFINED;

            String namespaceURI = valueToString(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);
            String value = imp->getParameter(namespaceURI, localName);
            if (!value.isNull())
                return JS_NewString(ctx, value.utf8().data());
            return JS_UNDEFINED;
        }
        case JSXSLTProcessor::RemoveParameter:
        {
            if (JS_IsUndefined(argv[1]) || JS_IsNull(argv[1]))
                return JS_UNDEFINED;

            String namespaceURI = valueToString(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);
            imp->removeParameter(namespaceURI, localName);
            return JS_UNDEFINED;
        }
        case JSXSLTProcessor::ClearParameters:
            imp->clearParameters();
            return JS_UNDEFINED;
        case JSXSLTProcessor::Reset:
            imp->reset();
            return JS_UNDEFINED;
    }
    return JS_UNDEFINED;
}

}
#endif // ENABLE(XSLT)
