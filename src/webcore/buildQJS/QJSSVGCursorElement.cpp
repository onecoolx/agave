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

static JSCFunctionListEntry JSSVGCursorElementAttributesFunctions[7];
static bool JSSVGCursorElementAttributesFunctions_initialized = false;

static void init_JSSVGCursorElementAttributesFunctions()
{
    if (JSSVGCursorElementAttributesFunctions_initialized) return;
    JSSVGCursorElementAttributesFunctions_initialized = true;
    memset(JSSVGCursorElementAttributesFunctions, 0, sizeof(JSSVGCursorElementAttributesFunctions));
    JSSVGCursorElementAttributesFunctions[0].name = "x";
    JSSVGCursorElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCursorElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCursorElementAttributesFunctions[0].magic = JSSVGCursorElement::XAttrNum;
    JSSVGCursorElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGCursorElement::getValueProperty;
    JSSVGCursorElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGCursorElementAttributesFunctions[1].name = "y";
    JSSVGCursorElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCursorElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCursorElementAttributesFunctions[1].magic = JSSVGCursorElement::YAttrNum;
    JSSVGCursorElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGCursorElement::getValueProperty;
    JSSVGCursorElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGCursorElementAttributesFunctions[2].name = "href";
    JSSVGCursorElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCursorElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCursorElementAttributesFunctions[2].magic = JSSVGCursorElement::HrefAttrNum;
    JSSVGCursorElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGCursorElement::getValueProperty;
    JSSVGCursorElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGCursorElementAttributesFunctions[3].name = "requiredFeatures";
    JSSVGCursorElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCursorElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCursorElementAttributesFunctions[3].magic = JSSVGCursorElement::RequiredFeaturesAttrNum;
    JSSVGCursorElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGCursorElement::getValueProperty;
    JSSVGCursorElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGCursorElementAttributesFunctions[4].name = "requiredExtensions";
    JSSVGCursorElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCursorElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCursorElementAttributesFunctions[4].magic = JSSVGCursorElement::RequiredExtensionsAttrNum;
    JSSVGCursorElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGCursorElement::getValueProperty;
    JSSVGCursorElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGCursorElementAttributesFunctions[5].name = "systemLanguage";
    JSSVGCursorElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCursorElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCursorElementAttributesFunctions[5].magic = JSSVGCursorElement::SystemLanguageAttrNum;
    JSSVGCursorElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGCursorElement::getValueProperty;
    JSSVGCursorElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGCursorElementAttributesFunctions[6].name = "externalResourcesRequired";
    JSSVGCursorElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCursorElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCursorElementAttributesFunctions[6].magic = JSSVGCursorElement::ExternalResourcesRequiredAttrNum;
    JSSVGCursorElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGCursorElement::getValueProperty;
    JSSVGCursorElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGCursorElementPrototypeFunctions[1];
static bool JSSVGCursorElementPrototypeFunctions_initialized = false;

static void init_JSSVGCursorElementPrototypeFunctions()
{
    if (JSSVGCursorElementPrototypeFunctions_initialized) return;
    JSSVGCursorElementPrototypeFunctions_initialized = true;
    memset(JSSVGCursorElementPrototypeFunctions, 0, sizeof(JSSVGCursorElementPrototypeFunctions));
    JSSVGCursorElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGCursorElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGCursorElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGCursorElementPrototypeFunctions[0].magic = JSSVGCursorElement::HasExtensionFuncNum;
    JSSVGCursorElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGCursorElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGCursorElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGCursorElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGCursorElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGCursorElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGCursorElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGCursorElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGCursorElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGCursorElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGCursorElementAttributesFunctions, countof(JSSVGCursorElementAttributesFunctions));
    init_JSSVGCursorElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGCursorElementPrototypeFunctions, countof(JSSVGCursorElementPrototypeFunctions));
}

static JSClassDef JSSVGCursorElementClassDefine;
static bool JSSVGCursorElementClassDefine_initialized = false;

static void init_JSSVGCursorElementClassDefine()
{
    if (JSSVGCursorElementClassDefine_initialized) return;
    JSSVGCursorElementClassDefine_initialized = true;
    memset(&JSSVGCursorElementClassDefine, 0, sizeof(JSSVGCursorElementClassDefine));
    JSSVGCursorElementClassDefine.class_name = "SVGCursorElement";
    JSSVGCursorElementClassDefine.finalizer = JSSVGCursorElement::finalizer;
    JSSVGCursorElementClassDefine.gc_mark = JSSVGCursorElement::mark;
}

JSClassID JSSVGCursorElement::js_class_id = 0;

void JSSVGCursorElement::init(JSContext* ctx)
{
    if (JSSVGCursorElement::js_class_id == 0) {
        init_JSSVGCursorElementClassDefine();
        JS_NewClassID(&JSSVGCursorElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGCursorElement::js_class_id, &JSSVGCursorElementClassDefine);
        JS_SetClassProto(ctx, JSSVGCursorElement::js_class_id, JSSVGCursorElementPrototype::self(ctx));
    }
}

JSValue JSSVGCursorElement::create(JSContext* ctx, SVGCursorElement* impl)
{
    JSSVGCursorElement::init(ctx);
    JSValue _proto = JSSVGCursorElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGCursorElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque(this_val, JSSVGCursorElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque(this_val, JSSVGCursorElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque(this_val, JSSVGCursorElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque(this_val, JSSVGCursorElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque(this_val, JSSVGCursorElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque(this_val, JSSVGCursorElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque(this_val, JSSVGCursorElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGCursorElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGCursorElement* imp = (SVGCursorElement*)JS_GetOpaque(this_val, JSSVGCursorElement::js_class_id);
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
