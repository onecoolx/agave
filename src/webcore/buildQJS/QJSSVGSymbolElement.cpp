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


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGSymbolElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedRect.h"
#include "QJSSVGAnimatedString.h"
#include "SVGSymbolElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGSymbolElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGSymbolElement::getValueProperty, JSSVGSymbolElement::putValueProperty, JSSVGSymbolElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGSymbolElement::getValueProperty, NULL, JSSVGSymbolElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGSymbolElement::getValueProperty, NULL, JSSVGSymbolElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("preserveAspectRatio", JSSVGSymbolElement::getValueProperty, NULL, JSSVGSymbolElement::PreserveAspectRatioAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGSymbolElement::getValueProperty, JSSVGSymbolElement::putValueProperty, JSSVGSymbolElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGSymbolElement::getValueProperty, NULL, JSSVGSymbolElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("viewBox", JSSVGSymbolElement::getValueProperty, NULL, JSSVGSymbolElement::ViewBoxAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGSymbolElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGSymbolElementPrototypeFunction::callAsFunction, JSSVGSymbolElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGSymbolElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGSymbolElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGSymbolElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGSymbolElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGSymbolElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSymbolElementAttributesFunctions, countof(JSSVGSymbolElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSymbolElementPrototypeFunctions, countof(JSSVGSymbolElementPrototypeFunctions));
}

static JSClassDef JSSVGSymbolElementClassDefine = 
{
    "SVGSymbolElement",
    .finalizer = JSSVGSymbolElement::finalizer,
    .gc_mark = JSSVGSymbolElement::mark,
};

JSClassID JSSVGSymbolElement::js_class_id = 0;

void JSSVGSymbolElement::init(JSContext* ctx)
{
    if (JSSVGSymbolElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGSymbolElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGSymbolElement::js_class_id, &JSSVGSymbolElementClassDefine);
        JS_SetClassProto(ctx, JSSVGSymbolElement::js_class_id, JSSVGSymbolElementPrototype::self(ctx));
    }
}

JSValue JSSVGSymbolElement::create(JSContext* ctx, SVGSymbolElement* impl)
{
    JSSVGSymbolElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGSymbolElementPrototype::self(ctx), JSSVGSymbolElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGSymbolElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGSymbolElement* impl = (SVGSymbolElement*)JS_GetOpaque(val, JSSVGSymbolElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGSymbolElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGSymbolElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ViewBoxAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGSymbolElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGSymbolElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque2(ctx, this_val, JSSVGSymbolElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGSymbolElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
