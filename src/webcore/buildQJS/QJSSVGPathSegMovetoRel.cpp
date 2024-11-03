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
#include "QJSSVGPathSegMovetoRel.h"

#include "SVGPathSegMoveto.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegMovetoRelAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegMovetoRel::getValueProperty, JSSVGPathSegMovetoRel::putValueProperty, JSSVGPathSegMovetoRel::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegMovetoRel::getValueProperty, JSSVGPathSegMovetoRel::putValueProperty, JSSVGPathSegMovetoRel::XAttrNum)
};

JSValue JSSVGPathSegMovetoRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegMovetoRel.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegMovetoRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegMovetoRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegMovetoRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegMovetoRelAttributesFunctions, countof(JSSVGPathSegMovetoRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegMovetoRelClassDefine = 
{
    "SVGPathSegMovetoRel",
    .finalizer = JSSVGPathSegMovetoRel::finalizer,
    .gc_mark = JSSVGPathSegMovetoRel::mark,
};

JSClassID JSSVGPathSegMovetoRel::js_class_id = 0;

void JSSVGPathSegMovetoRel::init(JSContext* ctx)
{
    if (JSSVGPathSegMovetoRel::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegMovetoRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegMovetoRel::js_class_id, &JSSVGPathSegMovetoRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegMovetoRel::js_class_id, JSSVGPathSegMovetoRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegMovetoRel::create(JSContext* ctx, SVGPathSegMovetoRel* impl, SVGElement* context)
{
    JSSVGPathSegMovetoRel::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegMovetoRelPrototype::self(ctx), JSSVGPathSegMovetoRel::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegMovetoRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegMovetoRel* impl = (SVGPathSegMovetoRel*)JS_GetOpaque(val, JSSVGPathSegMovetoRel::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegMovetoRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegMovetoRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegMovetoRel* imp = (SVGPathSegMovetoRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegMovetoRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegMovetoRel* imp = (SVGPathSegMovetoRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegMovetoRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegMovetoRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegMovetoRel* imp = (SVGPathSegMovetoRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegMovetoRel::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegMovetoRel* imp = (SVGPathSegMovetoRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegMovetoRel::js_class_id);
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
