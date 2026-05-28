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
#include "QJSSVGAnimatedBoolean.h"


using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGAnimatedBooleanAttributesFunctions[2];
static bool JSSVGAnimatedBooleanAttributesFunctions_initialized = false;

static void init_JSSVGAnimatedBooleanAttributesFunctions()
{
    if (JSSVGAnimatedBooleanAttributesFunctions_initialized) return;
    JSSVGAnimatedBooleanAttributesFunctions_initialized = true;
    memset(JSSVGAnimatedBooleanAttributesFunctions, 0, sizeof(JSSVGAnimatedBooleanAttributesFunctions));
    JSSVGAnimatedBooleanAttributesFunctions[0].name = "baseVal";
    JSSVGAnimatedBooleanAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedBooleanAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedBooleanAttributesFunctions[0].magic = JSSVGAnimatedBoolean::BaseValAttrNum;
    JSSVGAnimatedBooleanAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAnimatedBoolean::getValueProperty;
    JSSVGAnimatedBooleanAttributesFunctions[0].u.getset.set.setter_magic = JSSVGAnimatedBoolean::putValueProperty;
    JSSVGAnimatedBooleanAttributesFunctions[1].name = "animVal";
    JSSVGAnimatedBooleanAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedBooleanAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedBooleanAttributesFunctions[1].magic = JSSVGAnimatedBoolean::AnimValAttrNum;
    JSSVGAnimatedBooleanAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAnimatedBoolean::getValueProperty;
    JSSVGAnimatedBooleanAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGAnimatedBooleanPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedBoolean.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedBooleanPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedBoolean.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnimatedBooleanPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAnimatedBooleanAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedBooleanAttributesFunctions, countof(JSSVGAnimatedBooleanAttributesFunctions));
}

static JSClassDef JSSVGAnimatedBooleanClassDefine;
static bool JSSVGAnimatedBooleanClassDefine_initialized = false;

static void init_JSSVGAnimatedBooleanClassDefine()
{
    if (JSSVGAnimatedBooleanClassDefine_initialized) return;
    JSSVGAnimatedBooleanClassDefine_initialized = true;
    memset(&JSSVGAnimatedBooleanClassDefine, 0, sizeof(JSSVGAnimatedBooleanClassDefine));
    JSSVGAnimatedBooleanClassDefine.class_name = "SVGAnimatedBoolean";
    JSSVGAnimatedBooleanClassDefine.finalizer = JSSVGAnimatedBoolean::finalizer;
    JSSVGAnimatedBooleanClassDefine.gc_mark = JSSVGAnimatedBoolean::mark;
}

JSClassID JSSVGAnimatedBoolean::js_class_id = 0;

void JSSVGAnimatedBoolean::init(JSContext* ctx)
{
    if (JSSVGAnimatedBoolean::js_class_id == 0) {
        init_JSSVGAnimatedBooleanClassDefine();
        JS_NewClassID(&JSSVGAnimatedBoolean::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedBoolean::js_class_id, &JSSVGAnimatedBooleanClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedBoolean::js_class_id, JSSVGAnimatedBooleanPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedBoolean::create(JSContext* ctx, SVGAnimatedBoolean* impl, SVGElement* context)
{
    JSSVGAnimatedBoolean::init(ctx);
    JSValue _proto = JSSVGAnimatedBooleanPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAnimatedBoolean::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedBoolean::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedBoolean* impl = (SVGAnimatedBoolean*)JS_GetOpaque(val, JSSVGAnimatedBoolean::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedBoolean::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedBoolean::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedBoolean* imp = (SVGAnimatedBoolean*)JS_GetOpaque(this_val, JSSVGAnimatedBoolean::js_class_id);
            return JS_NewBool(ctx, imp->baseVal() ? 1 : 0);
        }
        case AnimValAttrNum: {
            SVGAnimatedBoolean* imp = (SVGAnimatedBoolean*)JS_GetOpaque(this_val, JSSVGAnimatedBoolean::js_class_id);
            return JS_NewBool(ctx, imp->animVal() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSSVGAnimatedBoolean::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedBoolean* imp = (SVGAnimatedBoolean*)JS_GetOpaque(this_val, JSSVGAnimatedBoolean::js_class_id);
            imp->setBaseVal(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue toJS(JSContext *ctx, SVGAnimatedBoolean* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedBoolean, JSSVGAnimatedBoolean>(ctx, obj, context);
}

SVGAnimatedBoolean* toSVGAnimatedBoolean(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedBoolean* impl = (SVGAnimatedBoolean*)JS_GetOpaque(val, JSSVGAnimatedBoolean::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
