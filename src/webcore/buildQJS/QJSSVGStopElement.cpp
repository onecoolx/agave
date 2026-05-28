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


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGStopElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGStopElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGStopElementAttributesFunctions[3];
static bool JSSVGStopElementAttributesFunctions_initialized = false;

static void init_JSSVGStopElementAttributesFunctions()
{
    if (JSSVGStopElementAttributesFunctions_initialized) return;
    JSSVGStopElementAttributesFunctions_initialized = true;
    memset(JSSVGStopElementAttributesFunctions, 0, sizeof(JSSVGStopElementAttributesFunctions));
    JSSVGStopElementAttributesFunctions[0].name = "offset";
    JSSVGStopElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGStopElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGStopElementAttributesFunctions[0].magic = JSSVGStopElement::OffsetAttrNum;
    JSSVGStopElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGStopElement::getValueProperty;
    JSSVGStopElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGStopElementAttributesFunctions[1].name = "className";
    JSSVGStopElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGStopElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGStopElementAttributesFunctions[1].magic = JSSVGStopElement::ClassNameAttrNum;
    JSSVGStopElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGStopElement::getValueProperty;
    JSSVGStopElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGStopElementAttributesFunctions[2].name = "style";
    JSSVGStopElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGStopElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGStopElementAttributesFunctions[2].magic = JSSVGStopElement::StyleAttrNum;
    JSSVGStopElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGStopElement::getValueProperty;
    JSSVGStopElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGStopElementPrototypeFunctions[1];
static bool JSSVGStopElementPrototypeFunctions_initialized = false;

static void init_JSSVGStopElementPrototypeFunctions()
{
    if (JSSVGStopElementPrototypeFunctions_initialized) return;
    JSSVGStopElementPrototypeFunctions_initialized = true;
    memset(JSSVGStopElementPrototypeFunctions, 0, sizeof(JSSVGStopElementPrototypeFunctions));
    JSSVGStopElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGStopElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGStopElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGStopElementPrototypeFunctions[0].magic = JSSVGStopElement::GetPresentationAttributeFuncNum;
    JSSVGStopElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGStopElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGStopElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGStopElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGStopElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGStopElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGStopElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGStopElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGStopElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGStopElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStopElementAttributesFunctions, countof(JSSVGStopElementAttributesFunctions));
    init_JSSVGStopElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStopElementPrototypeFunctions, countof(JSSVGStopElementPrototypeFunctions));
}

static JSClassDef JSSVGStopElementClassDefine;
static bool JSSVGStopElementClassDefine_initialized = false;

static void init_JSSVGStopElementClassDefine()
{
    if (JSSVGStopElementClassDefine_initialized) return;
    JSSVGStopElementClassDefine_initialized = true;
    memset(&JSSVGStopElementClassDefine, 0, sizeof(JSSVGStopElementClassDefine));
    JSSVGStopElementClassDefine.class_name = "SVGStopElement";
    JSSVGStopElementClassDefine.finalizer = JSSVGStopElement::finalizer;
    JSSVGStopElementClassDefine.gc_mark = JSSVGStopElement::mark;
}

JSClassID JSSVGStopElement::js_class_id = 0;

void JSSVGStopElement::init(JSContext* ctx)
{
    if (JSSVGStopElement::js_class_id == 0) {
        init_JSSVGStopElementClassDefine();
        JS_NewClassID(&JSSVGStopElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGStopElement::js_class_id, &JSSVGStopElementClassDefine);
        JS_SetClassProto(ctx, JSSVGStopElement::js_class_id, JSSVGStopElementPrototype::self(ctx));
    }
}

JSValue JSSVGStopElement::create(JSContext* ctx, SVGStopElement* impl)
{
    JSSVGStopElement::init(ctx);
    JSValue _proto = JSSVGStopElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGStopElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGStopElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGStopElement* impl = (SVGStopElement*)JS_GetOpaque(val, JSSVGStopElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGStopElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGStopElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case OffsetAttrNum: {
            SVGStopElement* imp = (SVGStopElement*)JS_GetOpaque(this_val, JSSVGStopElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->offsetAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGStopElement* imp = (SVGStopElement*)JS_GetOpaque(this_val, JSSVGStopElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGStopElement* imp = (SVGStopElement*)JS_GetOpaque(this_val, JSSVGStopElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGStopElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGStopElement* imp = (SVGStopElement*)JS_GetOpaque(this_val, JSSVGStopElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGStopElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
