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
#include "QJSSVGFEOffsetElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEOffsetElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEOffsetElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("dy", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::DyAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("dx", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::DxAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEOffsetElement::getValueProperty, NULL, JSSVGFEOffsetElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEOffsetElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEOffsetElementPrototypeFunction::callAsFunction, JSSVGFEOffsetElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEOffsetElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEOffsetElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEOffsetElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEOffsetElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEOffsetElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEOffsetElementAttributesFunctions, countof(JSSVGFEOffsetElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEOffsetElementPrototypeFunctions, countof(JSSVGFEOffsetElementPrototypeFunctions));
}

static JSClassDef JSSVGFEOffsetElementClassDefine = 
{
    "SVGFEOffsetElement",
    .finalizer = JSSVGFEOffsetElement::finalizer,
    .gc_mark = JSSVGFEOffsetElement::mark,
};

JSClassID JSSVGFEOffsetElement::js_class_id = 0;

void JSSVGFEOffsetElement::init(JSContext* ctx)
{
    if (JSSVGFEOffsetElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEOffsetElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEOffsetElement::js_class_id, &JSSVGFEOffsetElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEOffsetElement::js_class_id, JSSVGFEOffsetElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEOffsetElement::create(JSContext* ctx, SVGFEOffsetElement* impl)
{
    JSSVGFEOffsetElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEOffsetElementPrototype::self(ctx), JSSVGFEOffsetElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEOffsetElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEOffsetElement* impl = (SVGFEOffsetElement*)JS_GetOpaque(val, JSSVGFEOffsetElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEOffsetElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEOffsetElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case DxAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->dxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case DyAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->dyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEOffsetElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEOffsetElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEOffsetElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
