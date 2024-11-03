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
#include "QJSSVGRect.h"


using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGRectAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGRect::getValueProperty, JSSVGRect::putValueProperty, JSSVGRect::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGRect::getValueProperty, JSSVGRect::putValueProperty, JSSVGRect::XAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGRect::getValueProperty, JSSVGRect::putValueProperty, JSSVGRect::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGRect::getValueProperty, JSSVGRect::putValueProperty, JSSVGRect::HeightAttrNum)
};

JSValue JSSVGRectPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGRect.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGRectPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGRect.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGRectPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRectAttributesFunctions, countof(JSSVGRectAttributesFunctions));
}

static JSClassDef JSSVGRectClassDefine = 
{
    "SVGRect",
    .finalizer = JSSVGRect::finalizer,
    .gc_mark = JSSVGRect::mark,
};

JSClassID JSSVGRect::js_class_id = 0;

void JSSVGRect::init(JSContext* ctx)
{
    if (JSSVGRect::js_class_id == 0) {
        JS_NewClassID(&JSSVGRect::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGRect::js_class_id, &JSSVGRectClassDefine);
        JS_SetClassProto(ctx, JSSVGRect::js_class_id, JSSVGRectPrototype::self(ctx));
    }
}

JSValue JSSVGRect::create(JSContext* ctx, JSSVGPODTypeWrapper<FloatRect>* impl, SVGElement* context)
{
    JSSVGRect::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGRectPrototype::self(ctx), JSSVGRect::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGRect::finalizer(JSRuntime* rt, JSValue val)
{
    SVGRect* impl = (SVGRect*)JS_GetOpaque(val, JSSVGRect::js_class_id);
    JSSVGPODTypeWrapperCache<FloatRect, SVGAnimatedRect>::forgetWrapper(impl);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGRect::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGRect::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            FloatRect imp(*impl());

            return JS_NewBigUint64(ctx, imp.x());
        }
        case YAttrNum: {
            FloatRect imp(*impl());

            return JS_NewBigUint64(ctx, imp.y());
        }
        case WidthAttrNum: {
            FloatRect imp(*impl());

            return JS_NewBigUint64(ctx, imp.width());
        }
        case HeightAttrNum: {
            FloatRect imp(*impl());

            return JS_NewBigUint64(ctx, imp.height());
        }
    }
    return JS_NULL;
}

JSValue JSSVGRect::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            FloatRect imp(*impl());

            imp.setX(valueToFloat(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case YAttrNum: {
            FloatRect imp(*impl());

            imp.setY(valueToFloat(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case WidthAttrNum: {
            FloatRect imp(*impl());

            imp.setWidth(valueToFloat(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case HeightAttrNum: {
            FloatRect imp(*impl());

            imp.setHeight(valueToFloat(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<FloatRect>* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<JSSVGPODTypeWrapper<FloatRect>, JSSVGRect>(ctx, obj, context);
}

FloatRect toSVGRect(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGRect* impl = (SVGRect*)JS_GetOpaque(val, JSSVGRect::js_class_id);
        return (FloatRect) *impl;
    } else {
        return FloatRect();
}

}

#endif // ENABLE(SVG)
