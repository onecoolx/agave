/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGFESpecularLightingElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFESpecularLightingElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFESpecularLightingElementAttributesFunctions[11];
static bool JSSVGFESpecularLightingElementAttributesFunctions_initialized = false;

static void init_JSSVGFESpecularLightingElementAttributesFunctions()
{
    if (JSSVGFESpecularLightingElementAttributesFunctions_initialized) return;
    JSSVGFESpecularLightingElementAttributesFunctions_initialized = true;
    memset(JSSVGFESpecularLightingElementAttributesFunctions, 0, sizeof(JSSVGFESpecularLightingElementAttributesFunctions));
    JSSVGFESpecularLightingElementAttributesFunctions[0].name = "in1";
    JSSVGFESpecularLightingElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[0].magic = JSSVGFESpecularLightingElement::In1AttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[1].name = "surfaceScale";
    JSSVGFESpecularLightingElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[1].magic = JSSVGFESpecularLightingElement::SurfaceScaleAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[2].name = "specularConstant";
    JSSVGFESpecularLightingElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[2].magic = JSSVGFESpecularLightingElement::SpecularConstantAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[3].name = "specularExponent";
    JSSVGFESpecularLightingElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[3].magic = JSSVGFESpecularLightingElement::SpecularExponentAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[4].name = "x";
    JSSVGFESpecularLightingElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[4].magic = JSSVGFESpecularLightingElement::XAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[5].name = "y";
    JSSVGFESpecularLightingElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[5].magic = JSSVGFESpecularLightingElement::YAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[6].name = "width";
    JSSVGFESpecularLightingElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[6].magic = JSSVGFESpecularLightingElement::WidthAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[7].name = "height";
    JSSVGFESpecularLightingElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[7].magic = JSSVGFESpecularLightingElement::HeightAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[8].name = "result";
    JSSVGFESpecularLightingElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[8].magic = JSSVGFESpecularLightingElement::ResultAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[9].name = "className";
    JSSVGFESpecularLightingElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[9].magic = JSSVGFESpecularLightingElement::ClassNameAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGFESpecularLightingElementAttributesFunctions[10].name = "style";
    JSSVGFESpecularLightingElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpecularLightingElementAttributesFunctions[10].magic = JSSVGFESpecularLightingElement::StyleAttrNum;
    JSSVGFESpecularLightingElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFESpecularLightingElement::getValueProperty;
    JSSVGFESpecularLightingElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFESpecularLightingElementPrototypeFunctions[1];
static bool JSSVGFESpecularLightingElementPrototypeFunctions_initialized = false;

static void init_JSSVGFESpecularLightingElementPrototypeFunctions()
{
    if (JSSVGFESpecularLightingElementPrototypeFunctions_initialized) return;
    JSSVGFESpecularLightingElementPrototypeFunctions_initialized = true;
    memset(JSSVGFESpecularLightingElementPrototypeFunctions, 0, sizeof(JSSVGFESpecularLightingElementPrototypeFunctions));
    JSSVGFESpecularLightingElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFESpecularLightingElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFESpecularLightingElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFESpecularLightingElementPrototypeFunctions[0].magic = JSSVGFESpecularLightingElement::GetPresentationAttributeFuncNum;
    JSSVGFESpecularLightingElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFESpecularLightingElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFESpecularLightingElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFESpecularLightingElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFESpecularLightingElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFESpecularLightingElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFESpecularLightingElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFESpecularLightingElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFESpecularLightingElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFESpecularLightingElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFESpecularLightingElementAttributesFunctions, countof(JSSVGFESpecularLightingElementAttributesFunctions));
    init_JSSVGFESpecularLightingElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFESpecularLightingElementPrototypeFunctions, countof(JSSVGFESpecularLightingElementPrototypeFunctions));
}

static JSClassDef JSSVGFESpecularLightingElementClassDefine;
static bool JSSVGFESpecularLightingElementClassDefine_initialized = false;

static void init_JSSVGFESpecularLightingElementClassDefine()
{
    if (JSSVGFESpecularLightingElementClassDefine_initialized) return;
    JSSVGFESpecularLightingElementClassDefine_initialized = true;
    memset(&JSSVGFESpecularLightingElementClassDefine, 0, sizeof(JSSVGFESpecularLightingElementClassDefine));
    JSSVGFESpecularLightingElementClassDefine.class_name = "SVGFESpecularLightingElement";
    JSSVGFESpecularLightingElementClassDefine.finalizer = JSSVGFESpecularLightingElement::finalizer;
    JSSVGFESpecularLightingElementClassDefine.gc_mark = JSSVGFESpecularLightingElement::mark;
}

JSClassID JSSVGFESpecularLightingElement::js_class_id = 0;

void JSSVGFESpecularLightingElement::init(JSContext* ctx)
{
    if (JSSVGFESpecularLightingElement::js_class_id == 0) {
        init_JSSVGFESpecularLightingElementClassDefine();
        JS_NewClassID(&JSSVGFESpecularLightingElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFESpecularLightingElement::js_class_id, &JSSVGFESpecularLightingElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFESpecularLightingElement::js_class_id, JSSVGFESpecularLightingElementPrototype::self(ctx));
    }
}

JSValue JSSVGFESpecularLightingElement::create(JSContext* ctx, SVGFESpecularLightingElement* impl)
{
    JSSVGFESpecularLightingElement::init(ctx);
    JSValue _proto = JSSVGFESpecularLightingElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFESpecularLightingElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFESpecularLightingElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFESpecularLightingElement* impl = (SVGFESpecularLightingElement*)JS_GetOpaque(val, JSSVGFESpecularLightingElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFESpecularLightingElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFESpecularLightingElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case SurfaceScaleAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->surfaceScaleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpecularConstantAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->specularConstantAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpecularExponentAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->specularExponentAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFESpecularLightingElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque(this_val, JSSVGFESpecularLightingElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFESpecularLightingElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
