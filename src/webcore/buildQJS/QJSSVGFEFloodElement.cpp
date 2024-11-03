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
#include "QJSSVGFEFloodElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEFloodElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEFloodElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEFloodElement::getValueProperty, NULL, JSSVGFEFloodElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEFloodElement::getValueProperty, NULL, JSSVGFEFloodElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEFloodElement::getValueProperty, NULL, JSSVGFEFloodElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEFloodElement::getValueProperty, NULL, JSSVGFEFloodElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEFloodElement::getValueProperty, NULL, JSSVGFEFloodElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEFloodElement::getValueProperty, NULL, JSSVGFEFloodElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEFloodElement::getValueProperty, NULL, JSSVGFEFloodElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEFloodElement::getValueProperty, NULL, JSSVGFEFloodElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEFloodElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEFloodElementPrototypeFunction::callAsFunction, JSSVGFEFloodElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEFloodElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEFloodElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEFloodElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEFloodElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEFloodElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEFloodElementAttributesFunctions, countof(JSSVGFEFloodElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEFloodElementPrototypeFunctions, countof(JSSVGFEFloodElementPrototypeFunctions));
}

static JSClassDef JSSVGFEFloodElementClassDefine = 
{
    "SVGFEFloodElement",
    .finalizer = JSSVGFEFloodElement::finalizer,
    .gc_mark = JSSVGFEFloodElement::mark,
};

JSClassID JSSVGFEFloodElement::js_class_id = 0;

void JSSVGFEFloodElement::init(JSContext* ctx)
{
    if (JSSVGFEFloodElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEFloodElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEFloodElement::js_class_id, &JSSVGFEFloodElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEFloodElement::js_class_id, JSSVGFEFloodElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEFloodElement::create(JSContext* ctx, SVGFEFloodElement* impl)
{
    JSSVGFEFloodElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEFloodElementPrototype::self(ctx), JSSVGFEFloodElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEFloodElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEFloodElement* impl = (SVGFEFloodElement*)JS_GetOpaque(val, JSSVGFEFloodElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEFloodElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEFloodElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEFloodElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEFloodElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEFloodElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
