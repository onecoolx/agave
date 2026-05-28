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
#include "QJSSVGFETileElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFETileElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFETileElementAttributesFunctions[8];
static bool JSSVGFETileElementAttributesFunctions_initialized = false;

static void init_JSSVGFETileElementAttributesFunctions()
{
    if (JSSVGFETileElementAttributesFunctions_initialized) return;
    JSSVGFETileElementAttributesFunctions_initialized = true;
    memset(JSSVGFETileElementAttributesFunctions, 0, sizeof(JSSVGFETileElementAttributesFunctions));
    JSSVGFETileElementAttributesFunctions[0].name = "x";
    JSSVGFETileElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETileElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETileElementAttributesFunctions[0].magic = JSSVGFETileElement::XAttrNum;
    JSSVGFETileElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFETileElement::getValueProperty;
    JSSVGFETileElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFETileElementAttributesFunctions[1].name = "className";
    JSSVGFETileElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETileElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETileElementAttributesFunctions[1].magic = JSSVGFETileElement::ClassNameAttrNum;
    JSSVGFETileElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFETileElement::getValueProperty;
    JSSVGFETileElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFETileElementAttributesFunctions[2].name = "width";
    JSSVGFETileElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETileElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETileElementAttributesFunctions[2].magic = JSSVGFETileElement::WidthAttrNum;
    JSSVGFETileElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFETileElement::getValueProperty;
    JSSVGFETileElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFETileElementAttributesFunctions[3].name = "y";
    JSSVGFETileElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETileElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETileElementAttributesFunctions[3].magic = JSSVGFETileElement::YAttrNum;
    JSSVGFETileElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFETileElement::getValueProperty;
    JSSVGFETileElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFETileElementAttributesFunctions[4].name = "in1";
    JSSVGFETileElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETileElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETileElementAttributesFunctions[4].magic = JSSVGFETileElement::In1AttrNum;
    JSSVGFETileElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFETileElement::getValueProperty;
    JSSVGFETileElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFETileElementAttributesFunctions[5].name = "height";
    JSSVGFETileElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETileElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETileElementAttributesFunctions[5].magic = JSSVGFETileElement::HeightAttrNum;
    JSSVGFETileElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFETileElement::getValueProperty;
    JSSVGFETileElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFETileElementAttributesFunctions[6].name = "result";
    JSSVGFETileElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETileElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETileElementAttributesFunctions[6].magic = JSSVGFETileElement::ResultAttrNum;
    JSSVGFETileElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFETileElement::getValueProperty;
    JSSVGFETileElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFETileElementAttributesFunctions[7].name = "style";
    JSSVGFETileElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETileElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETileElementAttributesFunctions[7].magic = JSSVGFETileElement::StyleAttrNum;
    JSSVGFETileElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFETileElement::getValueProperty;
    JSSVGFETileElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFETileElementPrototypeFunctions[1];
static bool JSSVGFETileElementPrototypeFunctions_initialized = false;

static void init_JSSVGFETileElementPrototypeFunctions()
{
    if (JSSVGFETileElementPrototypeFunctions_initialized) return;
    JSSVGFETileElementPrototypeFunctions_initialized = true;
    memset(JSSVGFETileElementPrototypeFunctions, 0, sizeof(JSSVGFETileElementPrototypeFunctions));
    JSSVGFETileElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFETileElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFETileElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFETileElementPrototypeFunctions[0].magic = JSSVGFETileElement::GetPresentationAttributeFuncNum;
    JSSVGFETileElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFETileElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFETileElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFETileElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFETileElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFETileElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFETileElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFETileElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFETileElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFETileElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETileElementAttributesFunctions, countof(JSSVGFETileElementAttributesFunctions));
    init_JSSVGFETileElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETileElementPrototypeFunctions, countof(JSSVGFETileElementPrototypeFunctions));
}

static JSClassDef JSSVGFETileElementClassDefine;
static bool JSSVGFETileElementClassDefine_initialized = false;

static void init_JSSVGFETileElementClassDefine()
{
    if (JSSVGFETileElementClassDefine_initialized) return;
    JSSVGFETileElementClassDefine_initialized = true;
    memset(&JSSVGFETileElementClassDefine, 0, sizeof(JSSVGFETileElementClassDefine));
    JSSVGFETileElementClassDefine.class_name = "SVGFETileElement";
    JSSVGFETileElementClassDefine.finalizer = JSSVGFETileElement::finalizer;
    JSSVGFETileElementClassDefine.gc_mark = JSSVGFETileElement::mark;
}

JSClassID JSSVGFETileElement::js_class_id = 0;

void JSSVGFETileElement::init(JSContext* ctx)
{
    if (JSSVGFETileElement::js_class_id == 0) {
        init_JSSVGFETileElementClassDefine();
        JS_NewClassID(&JSSVGFETileElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFETileElement::js_class_id, &JSSVGFETileElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFETileElement::js_class_id, JSSVGFETileElementPrototype::self(ctx));
    }
}

JSValue JSSVGFETileElement::create(JSContext* ctx, SVGFETileElement* impl)
{
    JSSVGFETileElement::init(ctx);
    JSValue _proto = JSSVGFETileElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFETileElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFETileElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFETileElement* impl = (SVGFETileElement*)JS_GetOpaque(val, JSSVGFETileElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFETileElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFETileElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFETileElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque(this_val, JSSVGFETileElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFETileElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
