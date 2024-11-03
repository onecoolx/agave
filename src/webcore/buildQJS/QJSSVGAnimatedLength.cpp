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
#include "QJSSVGAnimatedLength.h"

#include "QJSSVGLength.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedLengthAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseVal", JSSVGAnimatedLength::getValueProperty, NULL, JSSVGAnimatedLength::BaseValAttrNum),
    JS_CGETSET_MAGIC_DEF("animVal", JSSVGAnimatedLength::getValueProperty, NULL, JSSVGAnimatedLength::AnimValAttrNum)
};

JSValue JSSVGAnimatedLengthPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedLength.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedLengthPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedLength.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedLengthPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedLengthAttributesFunctions, countof(JSSVGAnimatedLengthAttributesFunctions));
}

static JSClassDef JSSVGAnimatedLengthClassDefine = 
{
    "SVGAnimatedLength",
    .finalizer = JSSVGAnimatedLength::finalizer,
    .gc_mark = JSSVGAnimatedLength::mark,
};

JSClassID JSSVGAnimatedLength::js_class_id = 0;

void JSSVGAnimatedLength::init(JSContext* ctx)
{
    if (JSSVGAnimatedLength::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedLength::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedLength::js_class_id, &JSSVGAnimatedLengthClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedLength::js_class_id, JSSVGAnimatedLengthPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedLength::create(JSContext* ctx, SVGAnimatedLength* impl, SVGElement* context)
{
    JSSVGAnimatedLength::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedLengthPrototype::self(ctx), JSSVGAnimatedLength::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedLength::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedLength* impl = (SVGAnimatedLength*)JS_GetOpaque(val, JSSVGAnimatedLength::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedLength::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedLength::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedLength* imp = (SVGAnimatedLength*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedLength::js_class_id);
            return toJS(ctx, JSSVGPODTypeWrapperCache<SVGLength, SVGAnimatedLength>::lookupOrCreateWrapper(imp, &SVGAnimatedLength::baseVal, &SVGAnimatedLength::setBaseVal), context());
        }
        case AnimValAttrNum: {
            SVGAnimatedLength* imp = (SVGAnimatedLength*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedLength::js_class_id);
            return toJS(ctx, JSSVGPODTypeWrapperCache<SVGLength, SVGAnimatedLength>::lookupOrCreateWrapper(imp, &SVGAnimatedLength::animVal, &SVGAnimatedLength::setAnimVal), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedLength* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedLength, JSSVGAnimatedLength>(ctx, obj, context);
}

SVGAnimatedLength* toSVGAnimatedLength(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedLength* impl = (SVGAnimatedLength*)JS_GetOpaque(val, JSSVGAnimatedLength::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
