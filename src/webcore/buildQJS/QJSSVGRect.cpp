/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#include <string.h>


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

static JSCFunctionListEntry JSSVGRectAttributesFunctions[4];
static bool JSSVGRectAttributesFunctions_initialized = false;

static void init_JSSVGRectAttributesFunctions()
{
    if (JSSVGRectAttributesFunctions_initialized) return;
    JSSVGRectAttributesFunctions_initialized = true;
    memset(JSSVGRectAttributesFunctions, 0, sizeof(JSSVGRectAttributesFunctions));
    JSSVGRectAttributesFunctions[0].name = "x";
    JSSVGRectAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectAttributesFunctions[0].magic = JSSVGRect::XAttrNum;
    JSSVGRectAttributesFunctions[0].u.getset.get.getter_magic = JSSVGRect::getValueProperty;
    JSSVGRectAttributesFunctions[0].u.getset.set.setter_magic = JSSVGRect::putValueProperty;
    JSSVGRectAttributesFunctions[1].name = "y";
    JSSVGRectAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectAttributesFunctions[1].magic = JSSVGRect::YAttrNum;
    JSSVGRectAttributesFunctions[1].u.getset.get.getter_magic = JSSVGRect::getValueProperty;
    JSSVGRectAttributesFunctions[1].u.getset.set.setter_magic = JSSVGRect::putValueProperty;
    JSSVGRectAttributesFunctions[2].name = "width";
    JSSVGRectAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectAttributesFunctions[2].magic = JSSVGRect::WidthAttrNum;
    JSSVGRectAttributesFunctions[2].u.getset.get.getter_magic = JSSVGRect::getValueProperty;
    JSSVGRectAttributesFunctions[2].u.getset.set.setter_magic = JSSVGRect::putValueProperty;
    JSSVGRectAttributesFunctions[3].name = "height";
    JSSVGRectAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectAttributesFunctions[3].magic = JSSVGRect::HeightAttrNum;
    JSSVGRectAttributesFunctions[3].u.getset.get.getter_magic = JSSVGRect::getValueProperty;
    JSSVGRectAttributesFunctions[3].u.getset.set.setter_magic = JSSVGRect::putValueProperty;
}

JSValue JSSVGRectPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGRect.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGRectPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGRect.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGRectPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGRectAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRectAttributesFunctions, countof(JSSVGRectAttributesFunctions));
}

static JSClassDef JSSVGRectClassDefine;
static bool JSSVGRectClassDefine_initialized = false;

static void init_JSSVGRectClassDefine()
{
    if (JSSVGRectClassDefine_initialized) return;
    JSSVGRectClassDefine_initialized = true;
    memset(&JSSVGRectClassDefine, 0, sizeof(JSSVGRectClassDefine));
    JSSVGRectClassDefine.class_name = "SVGRect";
    JSSVGRectClassDefine.finalizer = JSSVGRect::finalizer;
    JSSVGRectClassDefine.gc_mark = JSSVGRect::mark;
}

JSClassID JSSVGRect::js_class_id = 0;

void JSSVGRect::init(JSContext* ctx)
{
    if (JSSVGRect::js_class_id == 0) {
        init_JSSVGRectClassDefine();
        JS_NewClassID(&JSSVGRect::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGRect::js_class_id, &JSSVGRectClassDefine);
        JS_SetClassProto(ctx, JSSVGRect::js_class_id, JSSVGRectPrototype::self(ctx));
    }
}

JSValue JSSVGRect::create(JSContext* ctx, JSSVGPODTypeWrapper<FloatRect>* impl, SVGElement* context)
{
    JSSVGRect::init(ctx);
    JSValue _proto = JSSVGRectPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGRect::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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

            return JS_NewInt32(ctx, imp.x());
        }
        case YAttrNum: {
            FloatRect imp(*impl());

            return JS_NewInt32(ctx, imp.y());
        }
        case WidthAttrNum: {
            FloatRect imp(*impl());

            return JS_NewInt32(ctx, imp.width());
        }
        case HeightAttrNum: {
            FloatRect imp(*impl());

            return JS_NewInt32(ctx, imp.height());
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
