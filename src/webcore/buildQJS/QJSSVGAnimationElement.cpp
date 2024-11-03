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

static const JSCFunctionListEntry JSSVGAnimationElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGAnimationElement::getValueProperty, NULL, JSSVGAnimationElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("targetElement", JSSVGAnimationElement::getValueProperty, NULL, JSSVGAnimationElement::TargetElementAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGAnimationElement::getValueProperty, NULL, JSSVGAnimationElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGAnimationElement::getValueProperty, NULL, JSSVGAnimationElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGAnimationElement::getValueProperty, NULL, JSSVGAnimationElement::SystemLanguageAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGAnimationElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getSimpleDuration", 0, JSSVGAnimationElementPrototypeFunction::callAsFunction, JSSVGAnimationElement::GetSimpleDurationFuncNum),
    JS_CFUNC_MAGIC_DEF("getCurrentTime", 0, JSSVGAnimationElementPrototypeFunction::callAsFunction, JSSVGAnimationElement::GetCurrentTimeFuncNum),
    JS_CFUNC_MAGIC_DEF("getStartTime", 0, JSSVGAnimationElementPrototypeFunction::callAsFunction, JSSVGAnimationElement::GetStartTimeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGAnimationElementPrototypeFunction::callAsFunction, JSSVGAnimationElement::HasExtensionFuncNum)
};

JSValue JSSVGAnimationElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimationElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGAnimationElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimationElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimationElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimationElementAttributesFunctions, countof(JSSVGAnimationElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimationElementPrototypeFunctions, countof(JSSVGAnimationElementPrototypeFunctions));
}

static JSClassDef JSSVGAnimationElementClassDefine = 
{
    "SVGAnimationElement",
    .finalizer = JSSVGAnimationElement::finalizer,
    .gc_mark = JSSVGAnimationElement::mark,
};

JSClassID JSSVGAnimationElement::js_class_id = 0;

void JSSVGAnimationElement::init(JSContext* ctx)
{
    if (JSSVGAnimationElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimationElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimationElement::js_class_id, &JSSVGAnimationElementClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimationElement::js_class_id, JSSVGAnimationElementPrototype::self(ctx));
    }
}

JSValue JSSVGAnimationElement::create(JSContext* ctx, SVGAnimationElement* impl)
{
    JSSVGAnimationElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimationElementPrototype::self(ctx), JSSVGAnimationElement::js_class_id);
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
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque2(ctx, this_val, JSSVGAnimationElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->targetElement()));
        }
        case RequiredFeaturesAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque2(ctx, this_val, JSSVGAnimationElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque2(ctx, this_val, JSSVGAnimationElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque2(ctx, this_val, JSSVGAnimationElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque2(ctx, this_val, JSSVGAnimationElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGAnimationElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGAnimationElement* imp = (SVGAnimationElement*)JS_GetOpaque2(ctx, this_val, JSSVGAnimationElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGAnimationElement::GetStartTimeFuncNum: {

            JSValue result = JS_NewBigUint64(ctx, imp->getStartTime());
            return result;
        }
        case JSSVGAnimationElement::GetCurrentTimeFuncNum: {

            JSValue result = JS_NewBigUint64(ctx, imp->getCurrentTime());
            return result;
        }
        case JSSVGAnimationElement::GetSimpleDurationFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = JS_NewBigUint64(ctx, imp->getSimpleDuration(ec));
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
