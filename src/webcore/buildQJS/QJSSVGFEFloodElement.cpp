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

static JSCFunctionListEntry JSSVGFEFloodElementAttributesFunctions[8];
static bool JSSVGFEFloodElementAttributesFunctions_initialized = false;

static void init_JSSVGFEFloodElementAttributesFunctions()
{
    if (JSSVGFEFloodElementAttributesFunctions_initialized) return;
    JSSVGFEFloodElementAttributesFunctions_initialized = true;
    memset(JSSVGFEFloodElementAttributesFunctions, 0, sizeof(JSSVGFEFloodElementAttributesFunctions));
    JSSVGFEFloodElementAttributesFunctions[0].name = "x";
    JSSVGFEFloodElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEFloodElementAttributesFunctions[0].magic = JSSVGFEFloodElement::XAttrNum;
    JSSVGFEFloodElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEFloodElement::getValueProperty;
    JSSVGFEFloodElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEFloodElementAttributesFunctions[1].name = "className";
    JSSVGFEFloodElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEFloodElementAttributesFunctions[1].magic = JSSVGFEFloodElement::ClassNameAttrNum;
    JSSVGFEFloodElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEFloodElement::getValueProperty;
    JSSVGFEFloodElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEFloodElementAttributesFunctions[2].name = "width";
    JSSVGFEFloodElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEFloodElementAttributesFunctions[2].magic = JSSVGFEFloodElement::WidthAttrNum;
    JSSVGFEFloodElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEFloodElement::getValueProperty;
    JSSVGFEFloodElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEFloodElementAttributesFunctions[3].name = "y";
    JSSVGFEFloodElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEFloodElementAttributesFunctions[3].magic = JSSVGFEFloodElement::YAttrNum;
    JSSVGFEFloodElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEFloodElement::getValueProperty;
    JSSVGFEFloodElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEFloodElementAttributesFunctions[4].name = "in1";
    JSSVGFEFloodElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEFloodElementAttributesFunctions[4].magic = JSSVGFEFloodElement::In1AttrNum;
    JSSVGFEFloodElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEFloodElement::getValueProperty;
    JSSVGFEFloodElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEFloodElementAttributesFunctions[5].name = "height";
    JSSVGFEFloodElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEFloodElementAttributesFunctions[5].magic = JSSVGFEFloodElement::HeightAttrNum;
    JSSVGFEFloodElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEFloodElement::getValueProperty;
    JSSVGFEFloodElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEFloodElementAttributesFunctions[6].name = "result";
    JSSVGFEFloodElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEFloodElementAttributesFunctions[6].magic = JSSVGFEFloodElement::ResultAttrNum;
    JSSVGFEFloodElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEFloodElement::getValueProperty;
    JSSVGFEFloodElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEFloodElementAttributesFunctions[7].name = "style";
    JSSVGFEFloodElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEFloodElementAttributesFunctions[7].magic = JSSVGFEFloodElement::StyleAttrNum;
    JSSVGFEFloodElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEFloodElement::getValueProperty;
    JSSVGFEFloodElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEFloodElementPrototypeFunctions[1];
static bool JSSVGFEFloodElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEFloodElementPrototypeFunctions()
{
    if (JSSVGFEFloodElementPrototypeFunctions_initialized) return;
    JSSVGFEFloodElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEFloodElementPrototypeFunctions, 0, sizeof(JSSVGFEFloodElementPrototypeFunctions));
    JSSVGFEFloodElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEFloodElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEFloodElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEFloodElementPrototypeFunctions[0].magic = JSSVGFEFloodElement::GetPresentationAttributeFuncNum;
    JSSVGFEFloodElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEFloodElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEFloodElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEFloodElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEFloodElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEFloodElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEFloodElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEFloodElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEFloodElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEFloodElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEFloodElementAttributesFunctions, countof(JSSVGFEFloodElementAttributesFunctions));
    init_JSSVGFEFloodElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEFloodElementPrototypeFunctions, countof(JSSVGFEFloodElementPrototypeFunctions));
}

static JSClassDef JSSVGFEFloodElementClassDefine;
static bool JSSVGFEFloodElementClassDefine_initialized = false;

static void init_JSSVGFEFloodElementClassDefine()
{
    if (JSSVGFEFloodElementClassDefine_initialized) return;
    JSSVGFEFloodElementClassDefine_initialized = true;
    memset(&JSSVGFEFloodElementClassDefine, 0, sizeof(JSSVGFEFloodElementClassDefine));
    JSSVGFEFloodElementClassDefine.class_name = "SVGFEFloodElement";
    JSSVGFEFloodElementClassDefine.finalizer = JSSVGFEFloodElement::finalizer;
    JSSVGFEFloodElementClassDefine.gc_mark = JSSVGFEFloodElement::mark;
}

JSClassID JSSVGFEFloodElement::js_class_id = 0;

void JSSVGFEFloodElement::init(JSContext* ctx)
{
    if (JSSVGFEFloodElement::js_class_id == 0) {
        init_JSSVGFEFloodElementClassDefine();
        JS_NewClassID(&JSSVGFEFloodElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEFloodElement::js_class_id, &JSSVGFEFloodElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEFloodElement::js_class_id, JSSVGFEFloodElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEFloodElement::create(JSContext* ctx, SVGFEFloodElement* impl)
{
    JSSVGFEFloodElement::init(ctx);
    JSValue _proto = JSSVGFEFloodElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEFloodElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEFloodElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEFloodElement* imp = (SVGFEFloodElement*)JS_GetOpaque(this_val, JSSVGFEFloodElement::js_class_id);
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
