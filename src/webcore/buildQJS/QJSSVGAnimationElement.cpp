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
#include "QJSSVGAnimationElement.h"

#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGElement.h"
#include "QJSSVGStringList.h"
#include "SVGAnimationElement.h"
#include "SVGElement.h"
#include "SVGStringList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGAnimationElementAttributesFunctions[5];
static bool JSSVGAnimationElementAttributesFunctions_initialized = false;

static void init_JSSVGAnimationElementAttributesFunctions()
{
    if (JSSVGAnimationElementAttributesFunctions_initialized) return;
    JSSVGAnimationElementAttributesFunctions_initialized = true;
    memset(JSSVGAnimationElementAttributesFunctions, 0, sizeof(JSSVGAnimationElementAttributesFunctions));
    JSSVGAnimationElementAttributesFunctions[0].name = "externalResourcesRequired";
    JSSVGAnimationElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimationElementAttributesFunctions[0].magic = JSSVGAnimationElement::ExternalResourcesRequiredAttrNum;
    JSSVGAnimationElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAnimationElement::getValueProperty;
    JSSVGAnimationElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGAnimationElementAttributesFunctions[1].name = "targetElement";
    JSSVGAnimationElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimationElementAttributesFunctions[1].magic = JSSVGAnimationElement::TargetElementAttrNum;
    JSSVGAnimationElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAnimationElement::getValueProperty;
    JSSVGAnimationElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGAnimationElementAttributesFunctions[2].name = "requiredExtensions";
    JSSVGAnimationElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimationElementAttributesFunctions[2].magic = JSSVGAnimationElement::RequiredExtensionsAttrNum;
    JSSVGAnimationElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGAnimationElement::getValueProperty;
    JSSVGAnimationElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGAnimationElementAttributesFunctions[3].name = "requiredFeatures";
    JSSVGAnimationElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimationElementAttributesFunctions[3].magic = JSSVGAnimationElement::RequiredFeaturesAttrNum;
    JSSVGAnimationElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGAnimationElement::getValueProperty;
    JSSVGAnimationElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGAnimationElementAttributesFunctions[4].name = "systemLanguage";
    JSSVGAnimationElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimationElementAttributesFunctions[4].magic = JSSVGAnimationElement::SystemLanguageAttrNum;
    JSSVGAnimationElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGAnimationElement::getValueProperty;
    JSSVGAnimationElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGAnimationElementPrototypeFunctions[4];
static bool JSSVGAnimationElementPrototypeFunctions_initialized = false;

static void init_JSSVGAnimationElementPrototypeFunctions()
{
    if (JSSVGAnimationElementPrototypeFunctions_initialized) return;
    JSSVGAnimationElementPrototypeFunctions_initialized = true;
    memset(JSSVGAnimationElementPrototypeFunctions, 0, sizeof(JSSVGAnimationElementPrototypeFunctions));
    JSSVGAnimationElementPrototypeFunctions[0].name = "getSimpleDuration";
    JSSVGAnimationElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGAnimationElementPrototypeFunctions[0].magic = JSSVGAnimationElement::GetSimpleDurationFuncNum;
    JSSVGAnimationElementPrototypeFunctions[0].u.func.length = 0;
    JSSVGAnimationElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAnimationElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGAnimationElementPrototypeFunction::callAsFunction;
    JSSVGAnimationElementPrototypeFunctions[1].name = "getCurrentTime";
    JSSVGAnimationElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGAnimationElementPrototypeFunctions[1].magic = JSSVGAnimationElement::GetCurrentTimeFuncNum;
    JSSVGAnimationElementPrototypeFunctions[1].u.func.length = 0;
    JSSVGAnimationElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAnimationElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGAnimationElementPrototypeFunction::callAsFunction;
    JSSVGAnimationElementPrototypeFunctions[2].name = "getStartTime";
    JSSVGAnimationElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGAnimationElementPrototypeFunctions[2].magic = JSSVGAnimationElement::GetStartTimeFuncNum;
    JSSVGAnimationElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGAnimationElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAnimationElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGAnimationElementPrototypeFunction::callAsFunction;
    JSSVGAnimationElementPrototypeFunctions[3].name = "hasExtension";
    JSSVGAnimationElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAnimationElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGAnimationElementPrototypeFunctions[3].magic = JSSVGAnimationElement::HasExtensionFuncNum;
    JSSVGAnimationElementPrototypeFunctions[3].u.func.length = 1;
    JSSVGAnimationElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAnimationElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGAnimationElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGAnimationElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimationElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGAnimationElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimationElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnimationElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAnimationElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimationElementAttributesFunctions, countof(JSSVGAnimationElementAttributesFunctions));
    init_JSSVGAnimationElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimationElementPrototypeFunctions, countof(JSSVGAnimationElementPrototypeFunctions));
}

static JSClassDef JSSVGAnimationElementClassDefine;
static bool JSSVGAnimationElementClassDefine_initialized = false;

static void init_JSSVGAnimationElementClassDefine()
{
    if (JSSVGAnimationElementClassDefine_initialized) return;
    JSSVGAnimationElementClassDefine_initialized = true;
    memset(&JSSVGAnimationElementClassDefine, 0, sizeof(JSSVGAnimationElementClassDefine));
    JSSVGAnimationElementClassDefine.class_name = "SVGAnimationElement";
    JSSVGAnimationElementClassDefine.finalizer = JSSVGAnimationElement::finalizer;
    JSSVGAnimationElementClassDefine.gc_mark = JSSVGAnimationElement::mark;
}

JSClassID JSSVGAnimationElement::js_class_id = 0;

void JSSVGAnimationElement::init(JSContext* ctx)
{
    if (JSSVGAnimationElement::js_class_id == 0) {
        init_JSSVGAnimationElementClassDefine();
        JS_NewClassID(&JSSVGAnimationElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimationElement::js_class_id, &JSSVGAnimationElementClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimationElement::js_class_id, JSSVGAnimationElementPrototype::self(ctx));
    }
}

JSValue JSSVGAnimationElement::create(JSContext* ctx, SVGAnimationElement* impl)
{
    JSSVGAnimationElement::init(ctx);
    JSValue _proto = JSSVGAnimationElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAnimationElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimationElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimationElement* impl = (SVGAnimationElement*)JS_GetOpaque(val, JSSVGAnimationElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimationElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGAnimationElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TargetElementAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque(this_val, JSSVGAnimationElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->targetElement()));
        }
        case RequiredFeaturesAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque(this_val, JSSVGAnimationElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque(this_val, JSSVGAnimationElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque(this_val, JSSVGAnimationElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque(this_val, JSSVGAnimationElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGAnimationElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque(this_val, JSSVGAnimationElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGAnimationElement::GetStartTimeFuncNum: {

            JSValue result = JS_NewInt32(ctx, imp->getStartTime());
            return result;
        }
        case JSSVGAnimationElement::GetCurrentTimeFuncNum: {

            JSValue result = JS_NewInt32(ctx, imp->getCurrentTime());
            return result;
        }
        case JSSVGAnimationElement::GetSimpleDurationFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = JS_NewInt32(ctx, imp->getSimpleDuration(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGAnimationElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
