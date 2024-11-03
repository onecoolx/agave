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
#include "QJSSVGPathSegLinetoVerticalRel.h"

#include "SVGPathSegLinetoVertical.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegLinetoVerticalRelAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegLinetoVerticalRel::getValueProperty, JSSVGPathSegLinetoVerticalRel::putValueProperty, JSSVGPathSegLinetoVerticalRel::YAttrNum)
};

JSValue JSSVGPathSegLinetoVerticalRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoVerticalRel.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegLinetoVerticalRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoVerticalRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegLinetoVerticalRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegLinetoVerticalRelAttributesFunctions, countof(JSSVGPathSegLinetoVerticalRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegLinetoVerticalRelClassDefine = 
{
    "SVGPathSegLinetoVerticalRel",
    .finalizer = JSSVGPathSegLinetoVerticalRel::finalizer,
    .gc_mark = JSSVGPathSegLinetoVerticalRel::mark,
};

JSClassID JSSVGPathSegLinetoVerticalRel::js_class_id = 0;

void JSSVGPathSegLinetoVerticalRel::init(JSContext* ctx)
{
    if (JSSVGPathSegLinetoVerticalRel::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegLinetoVerticalRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegLinetoVerticalRel::js_class_id, &JSSVGPathSegLinetoVerticalRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegLinetoVerticalRel::js_class_id, JSSVGPathSegLinetoVerticalRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegLinetoVerticalRel::create(JSContext* ctx, SVGPathSegLinetoVerticalRel* impl, SVGElement* context)
{
    JSSVGPathSegLinetoVerticalRel::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegLinetoVerticalRelPrototype::self(ctx), JSSVGPathSegLinetoVerticalRel::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegLinetoVerticalRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegLinetoVerticalRel* impl = (SVGPathSegLinetoVerticalRel*)JS_GetOpaque(val, JSSVGPathSegLinetoVerticalRel::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegLinetoVerticalRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegLinetoVerticalRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case YAttrNum: {
            SVGPathSegLinetoVerticalRel* imp = (SVGPathSegLinetoVerticalRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegLinetoVerticalRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegLinetoVerticalRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case YAttrNum: {
            SVGPathSegLinetoVerticalRel* imp = (SVGPathSegLinetoVerticalRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegLinetoVerticalRel::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
