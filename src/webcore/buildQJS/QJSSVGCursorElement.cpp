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
#include "QJSSVGCursorElement.h"

#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGStringList.h"
#include "SVGCursorElement.h"
#include "SVGStringList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGCursorElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGCursorElement::getValueProperty, NULL, JSSVGCursorElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGCursorElement::getValueProperty, NULL, JSSVGCursorElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGCursorElement::getValueProperty, NULL, JSSVGCursorElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGCursorElement::getValueProperty, NULL, JSSVGCursorElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSSVGCursorElement::getValueProperty, NULL, JSSVGCursorElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGCursorElement::getValueProperty, NULL, JSSVGCursorElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGCursorElement::getValueProperty, NULL, JSSVGCursorElement::ExternalResourcesRequiredAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGCursorElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGCursorElementPrototypeFunction::callAsFunction, JSSVGCursorElement::HasExtensionFuncNum)
};

JSValue JSSVGCursorElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGCursorElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGCursorElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGCursorElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGCursorElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGCursorElementAttributesFunctions, countof(JSSVGCursorElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGCursorElementPrototypeFunctions, countof(JSSVGCursorElementPrototypeFunctions));
}

static JSClassDef JSSVGCursorElementClassDefine = 
{
    "SVGCursorElement",
    .finalizer = JSSVGCursorElement::finalizer,
    .gc_mark = JSSVGCursorElement::mark,
};

JSClassID JSSVGCursorElement::js_class_id = 0;

void JSSVGCursorElement::init(JSContext* ctx)
{
    if (JSSVGCursorElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGCursorElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGCursorElement::js_class_id, &JSSVGCursorElementClassDefine);
        JS_SetClassProto(ctx, JSSVGCursorElement::js_class_id, JSSVGCursorElementPrototype::self(ctx));
    }
}

JSValue JSSVGCursorElement::create(JSContext* ctx, SVGCursorElement* impl)
{
    JSSVGCursorElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGCursorElementPrototype::self(ctx), JSSVGCursorElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGCursorElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGCursorElement* impl = (SVGCursorElement*)JS_GetOpaque(val, JSSVGCursorElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGCursorElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGCursorElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque2(ctx, this_val, JSSVGCursorElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque2(ctx, this_val, JSSVGCursorElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque2(ctx, this_val, JSSVGCursorElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque2(ctx, this_val, JSSVGCursorElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque2(ctx, this_val, JSSVGCursorElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque2(ctx, this_val, JSSVGCursorElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque2(ctx, this_val, JSSVGCursorElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGCursorElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque2(ctx, this_val, JSSVGCursorElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGCursorElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
