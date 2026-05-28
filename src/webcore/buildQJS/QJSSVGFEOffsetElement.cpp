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

static JSCFunctionListEntry JSSVGFEOffsetElementAttributesFunctions[10];
static bool JSSVGFEOffsetElementAttributesFunctions_initialized = false;

static void init_JSSVGFEOffsetElementAttributesFunctions()
{
    if (JSSVGFEOffsetElementAttributesFunctions_initialized) return;
    JSSVGFEOffsetElementAttributesFunctions_initialized = true;
    memset(JSSVGFEOffsetElementAttributesFunctions, 0, sizeof(JSSVGFEOffsetElementAttributesFunctions));
    JSSVGFEOffsetElementAttributesFunctions[0].name = "in1";
    JSSVGFEOffsetElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[0].magic = JSSVGFEOffsetElement::In1AttrNum;
    JSSVGFEOffsetElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[1].name = "dx";
    JSSVGFEOffsetElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[1].magic = JSSVGFEOffsetElement::DxAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[2].name = "dy";
    JSSVGFEOffsetElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[2].magic = JSSVGFEOffsetElement::DyAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[3].name = "x";
    JSSVGFEOffsetElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[3].magic = JSSVGFEOffsetElement::XAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[4].name = "y";
    JSSVGFEOffsetElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[4].magic = JSSVGFEOffsetElement::YAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[5].name = "width";
    JSSVGFEOffsetElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[5].magic = JSSVGFEOffsetElement::WidthAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[6].name = "height";
    JSSVGFEOffsetElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[6].magic = JSSVGFEOffsetElement::HeightAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[7].name = "result";
    JSSVGFEOffsetElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[7].magic = JSSVGFEOffsetElement::ResultAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[8].name = "className";
    JSSVGFEOffsetElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[8].magic = JSSVGFEOffsetElement::ClassNameAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFEOffsetElementAttributesFunctions[9].name = "style";
    JSSVGFEOffsetElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEOffsetElementAttributesFunctions[9].magic = JSSVGFEOffsetElement::StyleAttrNum;
    JSSVGFEOffsetElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFEOffsetElement::getValueProperty;
    JSSVGFEOffsetElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEOffsetElementPrototypeFunctions[1];
static bool JSSVGFEOffsetElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEOffsetElementPrototypeFunctions()
{
    if (JSSVGFEOffsetElementPrototypeFunctions_initialized) return;
    JSSVGFEOffsetElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEOffsetElementPrototypeFunctions, 0, sizeof(JSSVGFEOffsetElementPrototypeFunctions));
    JSSVGFEOffsetElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEOffsetElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEOffsetElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEOffsetElementPrototypeFunctions[0].magic = JSSVGFEOffsetElement::GetPresentationAttributeFuncNum;
    JSSVGFEOffsetElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEOffsetElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEOffsetElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEOffsetElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEOffsetElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEOffsetElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEOffsetElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEOffsetElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEOffsetElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEOffsetElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEOffsetElementAttributesFunctions, countof(JSSVGFEOffsetElementAttributesFunctions));
    init_JSSVGFEOffsetElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEOffsetElementPrototypeFunctions, countof(JSSVGFEOffsetElementPrototypeFunctions));
}

static JSClassDef JSSVGFEOffsetElementClassDefine;
static bool JSSVGFEOffsetElementClassDefine_initialized = false;

static void init_JSSVGFEOffsetElementClassDefine()
{
    if (JSSVGFEOffsetElementClassDefine_initialized) return;
    JSSVGFEOffsetElementClassDefine_initialized = true;
    memset(&JSSVGFEOffsetElementClassDefine, 0, sizeof(JSSVGFEOffsetElementClassDefine));
    JSSVGFEOffsetElementClassDefine.class_name = "SVGFEOffsetElement";
    JSSVGFEOffsetElementClassDefine.finalizer = JSSVGFEOffsetElement::finalizer;
    JSSVGFEOffsetElementClassDefine.gc_mark = JSSVGFEOffsetElement::mark;
}

JSClassID JSSVGFEOffsetElement::js_class_id = 0;

void JSSVGFEOffsetElement::init(JSContext* ctx)
{
    if (JSSVGFEOffsetElement::js_class_id == 0) {
        init_JSSVGFEOffsetElementClassDefine();
        JS_NewClassID(&JSSVGFEOffsetElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEOffsetElement::js_class_id, &JSSVGFEOffsetElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEOffsetElement::js_class_id, JSSVGFEOffsetElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEOffsetElement::create(JSContext* ctx, SVGFEOffsetElement* impl)
{
    JSSVGFEOffsetElement::init(ctx);
    JSValue _proto = JSSVGFEOffsetElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEOffsetElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case DxAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->dxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case DyAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->dyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEOffsetElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEOffsetElement* imp = (SVGFEOffsetElement*)JS_GetOpaque(this_val, JSSVGFEOffsetElement::js_class_id);
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
