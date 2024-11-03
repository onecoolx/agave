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
#include "QJSSVGAnimatedTransformList.h"

#include "QJSSVGTransformList.h"
#include "SVGTransformList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedTransformListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseVal", JSSVGAnimatedTransformList::getValueProperty, NULL, JSSVGAnimatedTransformList::BaseValAttrNum),
    JS_CGETSET_MAGIC_DEF("animVal", JSSVGAnimatedTransformList::getValueProperty, NULL, JSSVGAnimatedTransformList::AnimValAttrNum)
};

JSValue JSSVGAnimatedTransformListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedTransformList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedTransformListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedTransformList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedTransformListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedTransformListAttributesFunctions, countof(JSSVGAnimatedTransformListAttributesFunctions));
}

static JSClassDef JSSVGAnimatedTransformListClassDefine = 
{
    "SVGAnimatedTransformList",
    .finalizer = JSSVGAnimatedTransformList::finalizer,
    .gc_mark = JSSVGAnimatedTransformList::mark,
};

JSClassID JSSVGAnimatedTransformList::js_class_id = 0;

void JSSVGAnimatedTransformList::init(JSContext* ctx)
{
    if (JSSVGAnimatedTransformList::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedTransformList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedTransformList::js_class_id, &JSSVGAnimatedTransformListClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedTransformList::js_class_id, JSSVGAnimatedTransformListPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedTransformList::create(JSContext* ctx, SVGAnimatedTransformList* impl, SVGElement* context)
{
    JSSVGAnimatedTransformList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedTransformListPrototype::self(ctx), JSSVGAnimatedTransformList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedTransformList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedTransformList* impl = (SVGAnimatedTransformList*)JS_GetOpaque(val, JSSVGAnimatedTransformList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedTransformList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedTransformList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedTransformList* imp = (SVGAnimatedTransformList*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedTransformList::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->baseVal()), context());
        }
        case AnimValAttrNum: {
            SVGAnimatedTransformList* imp = (SVGAnimatedTransformList*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedTransformList::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animVal()), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedTransformList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedTransformList, JSSVGAnimatedTransformList>(ctx, obj, context);
}

SVGAnimatedTransformList* toSVGAnimatedTransformList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedTransformList* impl = (SVGAnimatedTransformList*)JS_GetOpaque(val, JSSVGAnimatedTransformList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
