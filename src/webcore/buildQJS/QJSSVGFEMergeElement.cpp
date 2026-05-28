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

static JSCFunctionListEntry JSSVGFEMergeElementAttributesFunctions[7];
static bool JSSVGFEMergeElementAttributesFunctions_initialized = false;

static void init_JSSVGFEMergeElementAttributesFunctions()
{
    if (JSSVGFEMergeElementAttributesFunctions_initialized) return;
    JSSVGFEMergeElementAttributesFunctions_initialized = true;
    memset(JSSVGFEMergeElementAttributesFunctions, 0, sizeof(JSSVGFEMergeElementAttributesFunctions));
    JSSVGFEMergeElementAttributesFunctions[0].name = "width";
    JSSVGFEMergeElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEMergeElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEMergeElementAttributesFunctions[0].magic = JSSVGFEMergeElement::WidthAttrNum;
    JSSVGFEMergeElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEMergeElement::getValueProperty;
    JSSVGFEMergeElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEMergeElementAttributesFunctions[1].name = "x";
    JSSVGFEMergeElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEMergeElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEMergeElementAttributesFunctions[1].magic = JSSVGFEMergeElement::XAttrNum;
    JSSVGFEMergeElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEMergeElement::getValueProperty;
    JSSVGFEMergeElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEMergeElementAttributesFunctions[2].name = "y";
    JSSVGFEMergeElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEMergeElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEMergeElementAttributesFunctions[2].magic = JSSVGFEMergeElement::YAttrNum;
    JSSVGFEMergeElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEMergeElement::getValueProperty;
    JSSVGFEMergeElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEMergeElementAttributesFunctions[3].name = "result";
    JSSVGFEMergeElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEMergeElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEMergeElementAttributesFunctions[3].magic = JSSVGFEMergeElement::ResultAttrNum;
    JSSVGFEMergeElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEMergeElement::getValueProperty;
    JSSVGFEMergeElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEMergeElementAttributesFunctions[4].name = "height";
    JSSVGFEMergeElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEMergeElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEMergeElementAttributesFunctions[4].magic = JSSVGFEMergeElement::HeightAttrNum;
    JSSVGFEMergeElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEMergeElement::getValueProperty;
    JSSVGFEMergeElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEMergeElementAttributesFunctions[5].name = "className";
    JSSVGFEMergeElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEMergeElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEMergeElementAttributesFunctions[5].magic = JSSVGFEMergeElement::ClassNameAttrNum;
    JSSVGFEMergeElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEMergeElement::getValueProperty;
    JSSVGFEMergeElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEMergeElementAttributesFunctions[6].name = "style";
    JSSVGFEMergeElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEMergeElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEMergeElementAttributesFunctions[6].magic = JSSVGFEMergeElement::StyleAttrNum;
    JSSVGFEMergeElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEMergeElement::getValueProperty;
    JSSVGFEMergeElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEMergeElementPrototypeFunctions[1];
static bool JSSVGFEMergeElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEMergeElementPrototypeFunctions()
{
    if (JSSVGFEMergeElementPrototypeFunctions_initialized) return;
    JSSVGFEMergeElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEMergeElementPrototypeFunctions, 0, sizeof(JSSVGFEMergeElementPrototypeFunctions));
    JSSVGFEMergeElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEMergeElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEMergeElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEMergeElementPrototypeFunctions[0].magic = JSSVGFEMergeElement::GetPresentationAttributeFuncNum;
    JSSVGFEMergeElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEMergeElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEMergeElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEMergeElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEMergeElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEMergeElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEMergeElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEMergeElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEMergeElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEMergeElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEMergeElementAttributesFunctions, countof(JSSVGFEMergeElementAttributesFunctions));
    init_JSSVGFEMergeElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEMergeElementPrototypeFunctions, countof(JSSVGFEMergeElementPrototypeFunctions));
}

static JSClassDef JSSVGFEMergeElementClassDefine;
static bool JSSVGFEMergeElementClassDefine_initialized = false;

static void init_JSSVGFEMergeElementClassDefine()
{
    if (JSSVGFEMergeElementClassDefine_initialized) return;
    JSSVGFEMergeElementClassDefine_initialized = true;
    memset(&JSSVGFEMergeElementClassDefine, 0, sizeof(JSSVGFEMergeElementClassDefine));
    JSSVGFEMergeElementClassDefine.class_name = "SVGFEMergeElement";
    JSSVGFEMergeElementClassDefine.finalizer = JSSVGFEMergeElement::finalizer;
    JSSVGFEMergeElementClassDefine.gc_mark = JSSVGFEMergeElement::mark;
}

JSClassID JSSVGFEMergeElement::js_class_id = 0;

void JSSVGFEMergeElement::init(JSContext* ctx)
{
    if (JSSVGFEMergeElement::js_class_id == 0) {
        init_JSSVGFEMergeElementClassDefine();
        JS_NewClassID(&JSSVGFEMergeElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEMergeElement::js_class_id, &JSSVGFEMergeElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEMergeElement::js_class_id, JSSVGFEMergeElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEMergeElement::create(JSContext* ctx, SVGFEMergeElement* impl)
{
    JSSVGFEMergeElement::init(ctx);
    JSValue _proto = JSSVGFEMergeElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEMergeElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque(this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque(this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque(this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque(this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque(this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque(this_val, JSSVGFEMergeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque(this_val, JSSVGFEMergeElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEMergeElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEMergeElement* imp = (SVGFEMergeElement*)JS_GetOpaque(this_val, JSSVGFEMergeElement::js_class_id);
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
