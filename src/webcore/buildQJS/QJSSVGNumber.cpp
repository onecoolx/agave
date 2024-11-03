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
#include "QJSSVGNumber.h"


using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGNumberAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("value", JSSVGNumber::getValueProperty, JSSVGNumber::putValueProperty, JSSVGNumber::ValueAttrNum)
};

JSValue JSSVGNumberPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGNumber.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGNumberPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGNumber.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGNumberPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGNumberAttributesFunctions, countof(JSSVGNumberAttributesFunctions));
}

static JSClassDef JSSVGNumberClassDefine = 
{
    "SVGNumber",
    .finalizer = JSSVGNumber::finalizer,
    .gc_mark = JSSVGNumber::mark,
};

JSClassID JSSVGNumber::js_class_id = 0;

void JSSVGNumber::init(JSContext* ctx)
{
    if (JSSVGNumber::js_class_id == 0) {
        JS_NewClassID(&JSSVGNumber::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGNumber::js_class_id, &JSSVGNumberClassDefine);
        JS_SetClassProto(ctx, JSSVGNumber::js_class_id, JSSVGNumberPrototype::self(ctx));
    }
}

JSValue JSSVGNumber::create(JSContext* ctx, JSSVGPODTypeWrapper<float>* impl, SVGElement* context)
{
    JSSVGNumber::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGNumberPrototype::self(ctx), JSSVGNumber::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGNumber::finalizer(JSRuntime* rt, JSValue val)
{
    SVGNumber* impl = (SVGNumber*)JS_GetOpaque(val, JSSVGNumber::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGNumber::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGNumber::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ValueAttrNum: {
            float imp(*impl());

            return JS_NewBigUint64(ctx, imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGNumber::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ValueAttrNum: {
            float imp(*impl());

            imp = valueToFloat(ctx, value);
            m_impl->commitChange(ctx, imp);
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<float>* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<JSSVGPODTypeWrapper<float>, JSSVGNumber>(ctx, obj, context);
}

float toSVGNumber(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGNumber* impl = (SVGNumber*)JS_GetOpaque(val, JSSVGNumber::js_class_id);
        return (float) *impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
