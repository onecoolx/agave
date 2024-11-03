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
#include "QJSSVGAnimatedNumberList.h"

#include "QJSSVGNumberList.h"
#include "SVGNumberList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedNumberListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseVal", JSSVGAnimatedNumberList::getValueProperty, NULL, JSSVGAnimatedNumberList::BaseValAttrNum),
    JS_CGETSET_MAGIC_DEF("animVal", JSSVGAnimatedNumberList::getValueProperty, NULL, JSSVGAnimatedNumberList::AnimValAttrNum)
};

JSValue JSSVGAnimatedNumberListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedNumberList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedNumberListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedNumberList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedNumberListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedNumberListAttributesFunctions, countof(JSSVGAnimatedNumberListAttributesFunctions));
}

static JSClassDef JSSVGAnimatedNumberListClassDefine = 
{
    "SVGAnimatedNumberList",
    .finalizer = JSSVGAnimatedNumberList::finalizer,
    .gc_mark = JSSVGAnimatedNumberList::mark,
};

JSClassID JSSVGAnimatedNumberList::js_class_id = 0;

void JSSVGAnimatedNumberList::init(JSContext* ctx)
{
    if (JSSVGAnimatedNumberList::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedNumberList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedNumberList::js_class_id, &JSSVGAnimatedNumberListClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedNumberList::js_class_id, JSSVGAnimatedNumberListPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedNumberList::create(JSContext* ctx, SVGAnimatedNumberList* impl, SVGElement* context)
{
    JSSVGAnimatedNumberList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedNumberListPrototype::self(ctx), JSSVGAnimatedNumberList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedNumberList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedNumberList* impl = (SVGAnimatedNumberList*)JS_GetOpaque(val, JSSVGAnimatedNumberList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedNumberList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedNumberList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedNumberList* imp = (SVGAnimatedNumberList*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedNumberList::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->baseVal()), context());
        }
        case AnimValAttrNum: {
            SVGAnimatedNumberList* imp = (SVGAnimatedNumberList*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedNumberList::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animVal()), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedNumberList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedNumberList, JSSVGAnimatedNumberList>(ctx, obj, context);
}

SVGAnimatedNumberList* toSVGAnimatedNumberList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedNumberList* impl = (SVGAnimatedNumberList*)JS_GetOpaque(val, JSSVGAnimatedNumberList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
