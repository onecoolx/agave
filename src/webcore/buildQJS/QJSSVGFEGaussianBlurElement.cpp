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
#include "QJSSVGFEGaussianBlurElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEGaussianBlurElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFEGaussianBlurElementAttributesFunctions[10];
static bool JSSVGFEGaussianBlurElementAttributesFunctions_initialized = false;

static void init_JSSVGFEGaussianBlurElementAttributesFunctions()
{
    if (JSSVGFEGaussianBlurElementAttributesFunctions_initialized) return;
    JSSVGFEGaussianBlurElementAttributesFunctions_initialized = true;
    memset(JSSVGFEGaussianBlurElementAttributesFunctions, 0, sizeof(JSSVGFEGaussianBlurElementAttributesFunctions));
    JSSVGFEGaussianBlurElementAttributesFunctions[0].name = "stdDeviationY";
    JSSVGFEGaussianBlurElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[0].magic = JSSVGFEGaussianBlurElement::StdDeviationYAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[1].name = "width";
    JSSVGFEGaussianBlurElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[1].magic = JSSVGFEGaussianBlurElement::WidthAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[2].name = "x";
    JSSVGFEGaussianBlurElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[2].magic = JSSVGFEGaussianBlurElement::XAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[3].name = "stdDeviationX";
    JSSVGFEGaussianBlurElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[3].magic = JSSVGFEGaussianBlurElement::StdDeviationXAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[4].name = "in1";
    JSSVGFEGaussianBlurElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[4].magic = JSSVGFEGaussianBlurElement::In1AttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[5].name = "height";
    JSSVGFEGaussianBlurElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[5].magic = JSSVGFEGaussianBlurElement::HeightAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[6].name = "result";
    JSSVGFEGaussianBlurElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[6].magic = JSSVGFEGaussianBlurElement::ResultAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[7].name = "y";
    JSSVGFEGaussianBlurElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[7].magic = JSSVGFEGaussianBlurElement::YAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[8].name = "className";
    JSSVGFEGaussianBlurElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[8].magic = JSSVGFEGaussianBlurElement::ClassNameAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFEGaussianBlurElementAttributesFunctions[9].name = "style";
    JSSVGFEGaussianBlurElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEGaussianBlurElementAttributesFunctions[9].magic = JSSVGFEGaussianBlurElement::StyleAttrNum;
    JSSVGFEGaussianBlurElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFEGaussianBlurElement::getValueProperty;
    JSSVGFEGaussianBlurElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEGaussianBlurElementPrototypeFunctions[2];
static bool JSSVGFEGaussianBlurElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEGaussianBlurElementPrototypeFunctions()
{
    if (JSSVGFEGaussianBlurElementPrototypeFunctions_initialized) return;
    JSSVGFEGaussianBlurElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEGaussianBlurElementPrototypeFunctions, 0, sizeof(JSSVGFEGaussianBlurElementPrototypeFunctions));
    JSSVGFEGaussianBlurElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEGaussianBlurElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEGaussianBlurElementPrototypeFunctions[0].magic = JSSVGFEGaussianBlurElement::GetPresentationAttributeFuncNum;
    JSSVGFEGaussianBlurElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEGaussianBlurElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEGaussianBlurElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEGaussianBlurElementPrototypeFunction::callAsFunction;
    JSSVGFEGaussianBlurElementPrototypeFunctions[1].name = "setStdDeviation";
    JSSVGFEGaussianBlurElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEGaussianBlurElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGFEGaussianBlurElementPrototypeFunctions[1].magic = JSSVGFEGaussianBlurElement::SetStdDeviationFuncNum;
    JSSVGFEGaussianBlurElementPrototypeFunctions[1].u.func.length = 2;
    JSSVGFEGaussianBlurElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEGaussianBlurElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGFEGaussianBlurElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEGaussianBlurElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEGaussianBlurElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEGaussianBlurElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEGaussianBlurElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEGaussianBlurElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEGaussianBlurElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEGaussianBlurElementAttributesFunctions, countof(JSSVGFEGaussianBlurElementAttributesFunctions));
    init_JSSVGFEGaussianBlurElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEGaussianBlurElementPrototypeFunctions, countof(JSSVGFEGaussianBlurElementPrototypeFunctions));
}

static JSClassDef JSSVGFEGaussianBlurElementClassDefine;
static bool JSSVGFEGaussianBlurElementClassDefine_initialized = false;

static void init_JSSVGFEGaussianBlurElementClassDefine()
{
    if (JSSVGFEGaussianBlurElementClassDefine_initialized) return;
    JSSVGFEGaussianBlurElementClassDefine_initialized = true;
    memset(&JSSVGFEGaussianBlurElementClassDefine, 0, sizeof(JSSVGFEGaussianBlurElementClassDefine));
    JSSVGFEGaussianBlurElementClassDefine.class_name = "SVGFEGaussianBlurElement";
    JSSVGFEGaussianBlurElementClassDefine.finalizer = JSSVGFEGaussianBlurElement::finalizer;
    JSSVGFEGaussianBlurElementClassDefine.gc_mark = JSSVGFEGaussianBlurElement::mark;
}

JSClassID JSSVGFEGaussianBlurElement::js_class_id = 0;

void JSSVGFEGaussianBlurElement::init(JSContext* ctx)
{
    if (JSSVGFEGaussianBlurElement::js_class_id == 0) {
        init_JSSVGFEGaussianBlurElementClassDefine();
        JS_NewClassID(&JSSVGFEGaussianBlurElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEGaussianBlurElement::js_class_id, &JSSVGFEGaussianBlurElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEGaussianBlurElement::js_class_id, JSSVGFEGaussianBlurElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEGaussianBlurElement::create(JSContext* ctx, SVGFEGaussianBlurElement* impl)
{
    JSSVGFEGaussianBlurElement::init(ctx);
    JSValue _proto = JSSVGFEGaussianBlurElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEGaussianBlurElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEGaussianBlurElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEGaussianBlurElement* impl = (SVGFEGaussianBlurElement*)JS_GetOpaque(val, JSSVGFEGaussianBlurElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEGaussianBlurElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEGaussianBlurElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case StdDeviationXAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->stdDeviationXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StdDeviationYAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->stdDeviationYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEGaussianBlurElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque(this_val, JSSVGFEGaussianBlurElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEGaussianBlurElement::SetStdDeviationFuncNum: {
            float stdDeviationX = valueToFloat(ctx, argv[0]);
            float stdDeviationY = valueToFloat(ctx, argv[1]);
            imp->setStdDeviation(stdDeviationX, stdDeviationY);
            return JS_UNDEFINED;
        }
        case JSSVGFEGaussianBlurElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
