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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGFEMergeElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEMergeElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEMergeElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEMergeElement::getValueProperty, NULL, JSSVGFEMergeElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEMergeElement::getValueProperty, NULL, JSSVGFEMergeElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEMergeElement::getValueProperty, NULL, JSSVGFEMergeElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEMergeElement::getValueProperty, NULL, JSSVGFEMergeElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEMergeElement::getValueProperty, NULL, JSSVGFEMergeElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEMergeElement::getValueProperty, NULL, JSSVGFEMergeElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEMergeElement::getValueProperty, NULL, JSSVGFEMergeElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEMergeElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEMergeElementPrototypeFunction::callAsFunction, JSSVGFEMergeElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEMergeElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEMergeElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEMergeElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEMergeElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEMergeElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEMergeElementAttributesFunctions, countof(JSSVGFEMergeElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEMergeElementPrototypeFunctions, countof(JSSVGFEMergeElementPrototypeFunctions));
}

static JSClassDef JSSVGFEMergeElementClassDefine = 
{
    "SVGFEMergeElement",
    .finalizer = JSSVGFEMergeElement::finalizer,
    .gc_mark = JSSVGFEMergeElement::mark,
};

JSClassID JSSVGFEMergeElement::js_class_id = 0;

void JSSVGFEMergeElement::init(JSContext* ctx)
{
    if (JSSVGFEMergeElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEMergeElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEMergeElement::js_class_id, &JSSVGFEMergeElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEMergeElement::js_class_id, JSSVGFEMergeElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEMergeElement::create(JSContext* ctx, SVGFEMergeElement* impl)
{
    JSSVGFEMergeElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEMergeElementPrototype::self(ctx), JSSVGFEMergeElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEMergeElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEMergeElement* impl = (SVGFEMergeElement*)JS_GetOpaque(val, JSSVGFEMergeElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEMergeElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEMergeElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEMergeElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEMergeElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
