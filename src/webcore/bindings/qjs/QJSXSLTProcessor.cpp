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
#include <string.h>

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

static JSCFunctionListEntry JSXSLTProcessorFunctions[8];
static bool JSXSLTProcessorFunctions_inited = false;

static void init_JSXSLTProcessorFunctions()
{
    if (JSXSLTProcessorFunctions_inited) return;
    JSXSLTProcessorFunctions_inited = true;
    memset(JSXSLTProcessorFunctions, 0, sizeof(JSXSLTProcessorFunctions));
    struct { const char* n; int l; int m; } defs[] = {
        {"importStylesheet",1,JSXSLTProcessor::ImportStylesheet},
        {"transformToDocument",1,JSXSLTProcessor::TransformToDocument},
        {"transformToFragment",2,JSXSLTProcessor::TransformToFragment},
        {"setParameter",3,JSXSLTProcessor::SetParameter},
        {"getParameter",2,JSXSLTProcessor::GetParameter},
        {"removeParameter",2,JSXSLTProcessor::RemoveParameter},
        {"clearParameters",0,JSXSLTProcessor::ClearParameters},
        {"reset",0,JSXSLTProcessor::Reset},
    };
    for (int i = 0; i < 8; i++) {
        JSXSLTProcessorFunctions[i].name = defs[i].n;
        JSXSLTProcessorFunctions[i].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
        JSXSLTProcessorFunctions[i].def_type = JS_DEF_CFUNC;
        JSXSLTProcessorFunctions[i].magic = defs[i].m;
        JSXSLTProcessorFunctions[i].u.func.length = defs[i].l;
        JSXSLTProcessorFunctions[i].u.func.cproto = JS_CFUNC_generic_magic;
        JSXSLTProcessorFunctions[i].u.func.cfunc.generic_magic = JSXSLTProcessorPrototypeFunction::callAsFunction;
    }
}

static JSClassDef JSXSLTProcessorClassDefine;
static bool JSXSLTProcessorClassDefine_inited = false;

static void init_JSXSLTProcessorClassDefine()
{
    if (JSXSLTProcessorClassDefine_inited) return;
    JSXSLTProcessorClassDefine_inited = true;
    memset(&JSXSLTProcessorClassDefine, 0, sizeof(JSXSLTProcessorClassDefine));
    JSXSLTProcessorClassDefine.class_name = "XSLTProcessor";
    JSXSLTProcessorClassDefine.finalizer = JSXSLTProcessor::finalizer;
}

JSClassID JSXSLTProcessor::js_class_id = 0;

void JSXSLTProcessor::init(JSContext* ctx)
{
    if (JSXSLTProcessor::js_class_id == 0) {
        init_JSXSLTProcessorClassDefine();
        JS_NewClassID(&JSXSLTProcessor::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXSLTProcessor::js_class_id, &JSXSLTProcessorClassDefine);

        init_JSXSLTProcessorFunctions();
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
                return toJS(ctx, static_cast<Node*>(imp->transformToFragment(node, doc).get()));
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
