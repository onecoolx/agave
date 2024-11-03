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
#include "QJSSVGAnimatedNumber.h"


using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedNumberAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseVal", JSSVGAnimatedNumber::getValueProperty, JSSVGAnimatedNumber::putValueProperty, JSSVGAnimatedNumber::BaseValAttrNum),
    JS_CGETSET_MAGIC_DEF("animVal", JSSVGAnimatedNumber::getValueProperty, NULL, JSSVGAnimatedNumber::AnimValAttrNum)
};

JSValue JSSVGAnimatedNumberPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedNumber.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedNumberPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedNumber.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedNumberPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedNumberAttributesFunctions, countof(JSSVGAnimatedNumberAttributesFunctions));
}

static JSClassDef JSSVGAnimatedNumberClassDefine = 
{
    "SVGAnimatedNumber",
    .finalizer = JSSVGAnimatedNumber::finalizer,
    .gc_mark = JSSVGAnimatedNumber::mark,
};

JSClassID JSSVGAnimatedNumber::js_class_id = 0;

void JSSVGAnimatedNumber::init(JSContext* ctx)
{
    if (JSSVGAnimatedNumber::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedNumber::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedNumber::js_class_id, &JSSVGAnimatedNumberClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedNumber::js_class_id, JSSVGAnimatedNumberPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedNumber::create(JSContext* ctx, SVGAnimatedNumber* impl, SVGElement* context)
{
    JSSVGAnimatedNumber::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedNumberPrototype::self(ctx), JSSVGAnimatedNumber::js_class_id);
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
            SVGAnimatedNumber* imp = (SVGAnimatedNumber*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedNumber::js_class_id);
            return JS_NewBigUint64(ctx, imp->baseVal());
        }
        case AnimValAttrNum: {
            SVGAnimatedNumber* imp = (SVGAnimatedNumber*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedNumber::js_class_id);
            return JS_NewBigUint64(ctx, imp->animVal());
        }
    }
    return JS_NULL;
}

JSValue JSSVGAnimatedNumber::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedNumber* imp = (SVGAnimatedNumber*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedNumber::js_class_id);
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
