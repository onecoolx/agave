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
#include "QJSSVGAnimatedNumber.h"


using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGAnimatedNumberAttributesFunctions[2];
static bool JSSVGAnimatedNumberAttributesFunctions_initialized = false;

static void init_JSSVGAnimatedNumberAttributesFunctions()
{
    if (JSSVGAnimatedNumberAttributesFunctions_initialized) return;
    JSSVGAnimatedNumberAttributesFunctions_initialized = true;
    memset(JSSVGAnimatedNumberAttributesFunctions, 0, sizeof(JSSVGAnimatedNumberAttributesFunctions));
    JSSVGAnimatedNumberAttributesFunctions[0].name = "baseVal";
    JSSVGAnimatedNumberAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedNumberAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedNumberAttributesFunctions[0].magic = JSSVGAnimatedNumber::BaseValAttrNum;
    JSSVGAnimatedNumberAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAnimatedNumber::getValueProperty;
    JSSVGAnimatedNumberAttributesFunctions[0].u.getset.set.setter_magic = JSSVGAnimatedNumber::putValueProperty;
    JSSVGAnimatedNumberAttributesFunctions[1].name = "animVal";
    JSSVGAnimatedNumberAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedNumberAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedNumberAttributesFunctions[1].magic = JSSVGAnimatedNumber::AnimValAttrNum;
    JSSVGAnimatedNumberAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAnimatedNumber::getValueProperty;
    JSSVGAnimatedNumberAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGAnimatedNumberPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedNumber.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedNumberPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedNumber.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnimatedNumberPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAnimatedNumberAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedNumberAttributesFunctions, countof(JSSVGAnimatedNumberAttributesFunctions));
}

static JSClassDef JSSVGAnimatedNumberClassDefine;
static bool JSSVGAnimatedNumberClassDefine_initialized = false;

static void init_JSSVGAnimatedNumberClassDefine()
{
    if (JSSVGAnimatedNumberClassDefine_initialized) return;
    JSSVGAnimatedNumberClassDefine_initialized = true;
    memset(&JSSVGAnimatedNumberClassDefine, 0, sizeof(JSSVGAnimatedNumberClassDefine));
    JSSVGAnimatedNumberClassDefine.class_name = "SVGAnimatedNumber";
    JSSVGAnimatedNumberClassDefine.finalizer = JSSVGAnimatedNumber::finalizer;
    JSSVGAnimatedNumberClassDefine.gc_mark = JSSVGAnimatedNumber::mark;
}

JSClassID JSSVGAnimatedNumber::js_class_id = 0;

void JSSVGAnimatedNumber::init(JSContext* ctx)
{
    if (JSSVGAnimatedNumber::js_class_id == 0) {
        init_JSSVGAnimatedNumberClassDefine();
        JS_NewClassID(&JSSVGAnimatedNumber::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedNumber::js_class_id, &JSSVGAnimatedNumberClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedNumber::js_class_id, JSSVGAnimatedNumberPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedNumber::create(JSContext* ctx, SVGAnimatedNumber* impl, SVGElement* context)
{
    JSSVGAnimatedNumber::init(ctx);
    JSValue _proto = JSSVGAnimatedNumberPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAnimatedNumber::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedNumber::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedNumber* impl = (SVGAnimatedNumber*)JS_GetOpaque(val, JSSVGAnimatedNumber::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedNumber::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedNumber::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedNumber* imp = (SVGAnimatedNumber*)JS_GetOpaque(this_val, JSSVGAnimatedNumber::js_class_id);
            return JS_NewInt32(ctx, imp->baseVal());
        }
        case AnimValAttrNum: {
            SVGAnimatedNumber* imp = (SVGAnimatedNumber*)JS_GetOpaque(this_val, JSSVGAnimatedNumber::js_class_id);
            return JS_NewInt32(ctx, imp->animVal());
        }
    }
    return JS_NULL;
}

JSValue JSSVGAnimatedNumber::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedNumber* imp = (SVGAnimatedNumber*)JS_GetOpaque(this_val, JSSVGAnimatedNumber::js_class_id);
            imp->setBaseVal(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue toJS(JSContext *ctx, SVGAnimatedNumber* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedNumber, JSSVGAnimatedNumber>(ctx, obj, context);
}

SVGAnimatedNumber* toSVGAnimatedNumber(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedNumber* impl = (SVGAnimatedNumber*)JS_GetOpaque(val, JSSVGAnimatedNumber::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
