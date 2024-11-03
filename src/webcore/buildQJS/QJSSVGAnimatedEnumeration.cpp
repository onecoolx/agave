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
#include "QJSSVGAnimatedEnumeration.h"


using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedEnumerationAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseVal", JSSVGAnimatedEnumeration::getValueProperty, JSSVGAnimatedEnumeration::putValueProperty, JSSVGAnimatedEnumeration::BaseValAttrNum),
    JS_CGETSET_MAGIC_DEF("animVal", JSSVGAnimatedEnumeration::getValueProperty, NULL, JSSVGAnimatedEnumeration::AnimValAttrNum)
};

JSValue JSSVGAnimatedEnumerationPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedEnumeration.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedEnumerationPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedEnumeration.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedEnumerationPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedEnumerationAttributesFunctions, countof(JSSVGAnimatedEnumerationAttributesFunctions));
}

static JSClassDef JSSVGAnimatedEnumerationClassDefine = 
{
    "SVGAnimatedEnumeration",
    .finalizer = JSSVGAnimatedEnumeration::finalizer,
    .gc_mark = JSSVGAnimatedEnumeration::mark,
};

JSClassID JSSVGAnimatedEnumeration::js_class_id = 0;

void JSSVGAnimatedEnumeration::init(JSContext* ctx)
{
    if (JSSVGAnimatedEnumeration::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedEnumeration::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedEnumeration::js_class_id, &JSSVGAnimatedEnumerationClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedEnumeration::js_class_id, JSSVGAnimatedEnumerationPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedEnumeration::create(JSContext* ctx, SVGAnimatedEnumeration* impl, SVGElement* context)
{
    JSSVGAnimatedEnumeration::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedEnumerationPrototype::self(ctx), JSSVGAnimatedEnumeration::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedEnumeration::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedEnumeration* impl = (SVGAnimatedEnumeration*)JS_GetOpaque(val, JSSVGAnimatedEnumeration::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedEnumeration::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedEnumeration::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedEnumeration* imp = (SVGAnimatedEnumeration*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedEnumeration::js_class_id);
            return JS_NewBigUint64(ctx, imp->baseVal());
        }
        case AnimValAttrNum: {
            SVGAnimatedEnumeration* imp = (SVGAnimatedEnumeration*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedEnumeration::js_class_id);
            return JS_NewBigUint64(ctx, imp->animVal());
        }
    }
    return JS_NULL;
}

JSValue JSSVGAnimatedEnumeration::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedEnumeration* imp = (SVGAnimatedEnumeration*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedEnumeration::js_class_id);
            imp->setBaseVal(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue toJS(JSContext *ctx, SVGAnimatedEnumeration* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedEnumeration, JSSVGAnimatedEnumeration>(ctx, obj, context);
}

SVGAnimatedEnumeration* toSVGAnimatedEnumeration(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedEnumeration* impl = (SVGAnimatedEnumeration*)JS_GetOpaque(val, JSSVGAnimatedEnumeration::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
