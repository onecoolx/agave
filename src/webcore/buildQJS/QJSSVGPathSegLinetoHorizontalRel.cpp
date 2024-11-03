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
#include "QJSSVGPathSegLinetoHorizontalRel.h"

#include "SVGPathSegLinetoHorizontal.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegLinetoHorizontalRelAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegLinetoHorizontalRel::getValueProperty, JSSVGPathSegLinetoHorizontalRel::putValueProperty, JSSVGPathSegLinetoHorizontalRel::XAttrNum)
};

JSValue JSSVGPathSegLinetoHorizontalRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoHorizontalRel.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegLinetoHorizontalRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoHorizontalRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegLinetoHorizontalRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegLinetoHorizontalRelAttributesFunctions, countof(JSSVGPathSegLinetoHorizontalRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegLinetoHorizontalRelClassDefine = 
{
    "SVGPathSegLinetoHorizontalRel",
    .finalizer = JSSVGPathSegLinetoHorizontalRel::finalizer,
    .gc_mark = JSSVGPathSegLinetoHorizontalRel::mark,
};

JSClassID JSSVGPathSegLinetoHorizontalRel::js_class_id = 0;

void JSSVGPathSegLinetoHorizontalRel::init(JSContext* ctx)
{
    if (JSSVGPathSegLinetoHorizontalRel::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegLinetoHorizontalRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegLinetoHorizontalRel::js_class_id, &JSSVGPathSegLinetoHorizontalRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegLinetoHorizontalRel::js_class_id, JSSVGPathSegLinetoHorizontalRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegLinetoHorizontalRel::create(JSContext* ctx, SVGPathSegLinetoHorizontalRel* impl, SVGElement* context)
{
    JSSVGPathSegLinetoHorizontalRel::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegLinetoHorizontalRelPrototype::self(ctx), JSSVGPathSegLinetoHorizontalRel::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegLinetoHorizontalRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegLinetoHorizontalRel* impl = (SVGPathSegLinetoHorizontalRel*)JS_GetOpaque(val, JSSVGPathSegLinetoHorizontalRel::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegLinetoHorizontalRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegLinetoHorizontalRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegLinetoHorizontalRel* imp = (SVGPathSegLinetoHorizontalRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegLinetoHorizontalRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegLinetoHorizontalRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegLinetoHorizontalRel* imp = (SVGPathSegLinetoHorizontalRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegLinetoHorizontalRel::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
