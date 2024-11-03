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
#include "QJSSVGAnimatedLengthList.h"

#include "QJSSVGLengthList.h"
#include "SVGLengthList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedLengthListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseVal", JSSVGAnimatedLengthList::getValueProperty, NULL, JSSVGAnimatedLengthList::BaseValAttrNum),
    JS_CGETSET_MAGIC_DEF("animVal", JSSVGAnimatedLengthList::getValueProperty, NULL, JSSVGAnimatedLengthList::AnimValAttrNum)
};

JSValue JSSVGAnimatedLengthListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedLengthList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedLengthListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedLengthList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedLengthListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedLengthListAttributesFunctions, countof(JSSVGAnimatedLengthListAttributesFunctions));
}

static JSClassDef JSSVGAnimatedLengthListClassDefine = 
{
    "SVGAnimatedLengthList",
    .finalizer = JSSVGAnimatedLengthList::finalizer,
    .gc_mark = JSSVGAnimatedLengthList::mark,
};

JSClassID JSSVGAnimatedLengthList::js_class_id = 0;

void JSSVGAnimatedLengthList::init(JSContext* ctx)
{
    if (JSSVGAnimatedLengthList::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedLengthList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedLengthList::js_class_id, &JSSVGAnimatedLengthListClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedLengthList::js_class_id, JSSVGAnimatedLengthListPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedLengthList::create(JSContext* ctx, SVGAnimatedLengthList* impl, SVGElement* context)
{
    JSSVGAnimatedLengthList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedLengthListPrototype::self(ctx), JSSVGAnimatedLengthList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedLengthList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedLengthList* impl = (SVGAnimatedLengthList*)JS_GetOpaque(val, JSSVGAnimatedLengthList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedLengthList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedLengthList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedLengthList* imp = (SVGAnimatedLengthList*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedLengthList::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->baseVal()), context());
        }
        case AnimValAttrNum: {
            SVGAnimatedLengthList* imp = (SVGAnimatedLengthList*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedLengthList::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animVal()), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedLengthList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedLengthList, JSSVGAnimatedLengthList>(ctx, obj, context);
}

SVGAnimatedLengthList* toSVGAnimatedLengthList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedLengthList* impl = (SVGAnimatedLengthList*)JS_GetOpaque(val, JSSVGAnimatedLengthList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
