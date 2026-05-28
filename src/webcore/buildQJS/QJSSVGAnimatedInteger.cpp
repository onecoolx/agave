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
#include "QJSSVGAnimatedInteger.h"


using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGAnimatedIntegerAttributesFunctions[2];
static bool JSSVGAnimatedIntegerAttributesFunctions_initialized = false;

static void init_JSSVGAnimatedIntegerAttributesFunctions()
{
    if (JSSVGAnimatedIntegerAttributesFunctions_initialized) return;
    JSSVGAnimatedIntegerAttributesFunctions_initialized = true;
    memset(JSSVGAnimatedIntegerAttributesFunctions, 0, sizeof(JSSVGAnimatedIntegerAttributesFunctions));
    JSSVGAnimatedIntegerAttributesFunctions[0].name = "baseVal";
    JSSVGAnimatedIntegerAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedIntegerAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedIntegerAttributesFunctions[0].magic = JSSVGAnimatedInteger::BaseValAttrNum;
    JSSVGAnimatedIntegerAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAnimatedInteger::getValueProperty;
    JSSVGAnimatedIntegerAttributesFunctions[0].u.getset.set.setter_magic = JSSVGAnimatedInteger::putValueProperty;
    JSSVGAnimatedIntegerAttributesFunctions[1].name = "animVal";
    JSSVGAnimatedIntegerAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedIntegerAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedIntegerAttributesFunctions[1].magic = JSSVGAnimatedInteger::AnimValAttrNum;
    JSSVGAnimatedIntegerAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAnimatedInteger::getValueProperty;
    JSSVGAnimatedIntegerAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGAnimatedIntegerPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedInteger.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedIntegerPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedInteger.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnimatedIntegerPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAnimatedIntegerAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedIntegerAttributesFunctions, countof(JSSVGAnimatedIntegerAttributesFunctions));
}

static JSClassDef JSSVGAnimatedIntegerClassDefine;
static bool JSSVGAnimatedIntegerClassDefine_initialized = false;

static void init_JSSVGAnimatedIntegerClassDefine()
{
    if (JSSVGAnimatedIntegerClassDefine_initialized) return;
    JSSVGAnimatedIntegerClassDefine_initialized = true;
    memset(&JSSVGAnimatedIntegerClassDefine, 0, sizeof(JSSVGAnimatedIntegerClassDefine));
    JSSVGAnimatedIntegerClassDefine.class_name = "SVGAnimatedInteger";
    JSSVGAnimatedIntegerClassDefine.finalizer = JSSVGAnimatedInteger::finalizer;
    JSSVGAnimatedIntegerClassDefine.gc_mark = JSSVGAnimatedInteger::mark;
}

JSClassID JSSVGAnimatedInteger::js_class_id = 0;

void JSSVGAnimatedInteger::init(JSContext* ctx)
{
    if (JSSVGAnimatedInteger::js_class_id == 0) {
        init_JSSVGAnimatedIntegerClassDefine();
        JS_NewClassID(&JSSVGAnimatedInteger::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedInteger::js_class_id, &JSSVGAnimatedIntegerClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedInteger::js_class_id, JSSVGAnimatedIntegerPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedInteger::create(JSContext* ctx, SVGAnimatedInteger* impl, SVGElement* context)
{
    JSSVGAnimatedInteger::init(ctx);
    JSValue _proto = JSSVGAnimatedIntegerPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAnimatedInteger::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedInteger::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedInteger* impl = (SVGAnimatedInteger*)JS_GetOpaque(val, JSSVGAnimatedInteger::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedInteger::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedInteger::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedInteger* imp = (SVGAnimatedInteger*)JS_GetOpaque(this_val, JSSVGAnimatedInteger::js_class_id);
            return JS_NewInt32(ctx, imp->baseVal());
        }
        case AnimValAttrNum: {
            SVGAnimatedInteger* imp = (SVGAnimatedInteger*)JS_GetOpaque(this_val, JSSVGAnimatedInteger::js_class_id);
            return JS_NewInt32(ctx, imp->animVal());
        }
    }
    return JS_NULL;
}

JSValue JSSVGAnimatedInteger::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedInteger* imp = (SVGAnimatedInteger*)JS_GetOpaque(this_val, JSSVGAnimatedInteger::js_class_id);
            imp->setBaseVal(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue toJS(JSContext *ctx, SVGAnimatedInteger* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedInteger, JSSVGAnimatedInteger>(ctx, obj, context);
}

SVGAnimatedInteger* toSVGAnimatedInteger(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedInteger* impl = (SVGAnimatedInteger*)JS_GetOpaque(val, JSSVGAnimatedInteger::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
