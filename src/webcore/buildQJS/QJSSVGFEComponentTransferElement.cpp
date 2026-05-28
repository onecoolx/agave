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
#include "QJSSVGFEComponentTransferElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEComponentTransferElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFEComponentTransferElementAttributesFunctions[8];
static bool JSSVGFEComponentTransferElementAttributesFunctions_initialized = false;

static void init_JSSVGFEComponentTransferElementAttributesFunctions()
{
    if (JSSVGFEComponentTransferElementAttributesFunctions_initialized) return;
    JSSVGFEComponentTransferElementAttributesFunctions_initialized = true;
    memset(JSSVGFEComponentTransferElementAttributesFunctions, 0, sizeof(JSSVGFEComponentTransferElementAttributesFunctions));
    JSSVGFEComponentTransferElementAttributesFunctions[0].name = "x";
    JSSVGFEComponentTransferElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEComponentTransferElementAttributesFunctions[0].magic = JSSVGFEComponentTransferElement::XAttrNum;
    JSSVGFEComponentTransferElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEComponentTransferElement::getValueProperty;
    JSSVGFEComponentTransferElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEComponentTransferElementAttributesFunctions[1].name = "className";
    JSSVGFEComponentTransferElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEComponentTransferElementAttributesFunctions[1].magic = JSSVGFEComponentTransferElement::ClassNameAttrNum;
    JSSVGFEComponentTransferElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEComponentTransferElement::getValueProperty;
    JSSVGFEComponentTransferElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEComponentTransferElementAttributesFunctions[2].name = "width";
    JSSVGFEComponentTransferElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEComponentTransferElementAttributesFunctions[2].magic = JSSVGFEComponentTransferElement::WidthAttrNum;
    JSSVGFEComponentTransferElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEComponentTransferElement::getValueProperty;
    JSSVGFEComponentTransferElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEComponentTransferElementAttributesFunctions[3].name = "y";
    JSSVGFEComponentTransferElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEComponentTransferElementAttributesFunctions[3].magic = JSSVGFEComponentTransferElement::YAttrNum;
    JSSVGFEComponentTransferElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEComponentTransferElement::getValueProperty;
    JSSVGFEComponentTransferElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEComponentTransferElementAttributesFunctions[4].name = "in1";
    JSSVGFEComponentTransferElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEComponentTransferElementAttributesFunctions[4].magic = JSSVGFEComponentTransferElement::In1AttrNum;
    JSSVGFEComponentTransferElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEComponentTransferElement::getValueProperty;
    JSSVGFEComponentTransferElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEComponentTransferElementAttributesFunctions[5].name = "height";
    JSSVGFEComponentTransferElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEComponentTransferElementAttributesFunctions[5].magic = JSSVGFEComponentTransferElement::HeightAttrNum;
    JSSVGFEComponentTransferElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEComponentTransferElement::getValueProperty;
    JSSVGFEComponentTransferElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEComponentTransferElementAttributesFunctions[6].name = "result";
    JSSVGFEComponentTransferElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEComponentTransferElementAttributesFunctions[6].magic = JSSVGFEComponentTransferElement::ResultAttrNum;
    JSSVGFEComponentTransferElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEComponentTransferElement::getValueProperty;
    JSSVGFEComponentTransferElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEComponentTransferElementAttributesFunctions[7].name = "style";
    JSSVGFEComponentTransferElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEComponentTransferElementAttributesFunctions[7].magic = JSSVGFEComponentTransferElement::StyleAttrNum;
    JSSVGFEComponentTransferElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEComponentTransferElement::getValueProperty;
    JSSVGFEComponentTransferElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEComponentTransferElementPrototypeFunctions[1];
static bool JSSVGFEComponentTransferElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEComponentTransferElementPrototypeFunctions()
{
    if (JSSVGFEComponentTransferElementPrototypeFunctions_initialized) return;
    JSSVGFEComponentTransferElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEComponentTransferElementPrototypeFunctions, 0, sizeof(JSSVGFEComponentTransferElementPrototypeFunctions));
    JSSVGFEComponentTransferElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEComponentTransferElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEComponentTransferElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEComponentTransferElementPrototypeFunctions[0].magic = JSSVGFEComponentTransferElement::GetPresentationAttributeFuncNum;
    JSSVGFEComponentTransferElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEComponentTransferElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEComponentTransferElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEComponentTransferElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEComponentTransferElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEComponentTransferElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEComponentTransferElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEComponentTransferElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEComponentTransferElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEComponentTransferElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEComponentTransferElementAttributesFunctions, countof(JSSVGFEComponentTransferElementAttributesFunctions));
    init_JSSVGFEComponentTransferElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEComponentTransferElementPrototypeFunctions, countof(JSSVGFEComponentTransferElementPrototypeFunctions));
}

static JSClassDef JSSVGFEComponentTransferElementClassDefine;
static bool JSSVGFEComponentTransferElementClassDefine_initialized = false;

static void init_JSSVGFEComponentTransferElementClassDefine()
{
    if (JSSVGFEComponentTransferElementClassDefine_initialized) return;
    JSSVGFEComponentTransferElementClassDefine_initialized = true;
    memset(&JSSVGFEComponentTransferElementClassDefine, 0, sizeof(JSSVGFEComponentTransferElementClassDefine));
    JSSVGFEComponentTransferElementClassDefine.class_name = "SVGFEComponentTransferElement";
    JSSVGFEComponentTransferElementClassDefine.finalizer = JSSVGFEComponentTransferElement::finalizer;
    JSSVGFEComponentTransferElementClassDefine.gc_mark = JSSVGFEComponentTransferElement::mark;
}

JSClassID JSSVGFEComponentTransferElement::js_class_id = 0;

void JSSVGFEComponentTransferElement::init(JSContext* ctx)
{
    if (JSSVGFEComponentTransferElement::js_class_id == 0) {
        init_JSSVGFEComponentTransferElementClassDefine();
        JS_NewClassID(&JSSVGFEComponentTransferElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEComponentTransferElement::js_class_id, &JSSVGFEComponentTransferElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEComponentTransferElement::js_class_id, JSSVGFEComponentTransferElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEComponentTransferElement::create(JSContext* ctx, SVGFEComponentTransferElement* impl)
{
    JSSVGFEComponentTransferElement::init(ctx);
    JSValue _proto = JSSVGFEComponentTransferElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEComponentTransferElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEComponentTransferElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEComponentTransferElement* impl = (SVGFEComponentTransferElement*)JS_GetOpaque(val, JSSVGFEComponentTransferElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEComponentTransferElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEComponentTransferElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEComponentTransferElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque(this_val, JSSVGFEComponentTransferElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEComponentTransferElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
