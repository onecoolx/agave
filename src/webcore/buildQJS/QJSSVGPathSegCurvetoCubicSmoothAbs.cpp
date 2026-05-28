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
#include "QJSSVGPathSegCurvetoCubicSmoothAbs.h"

#include "SVGPathSegCurvetoCubicSmooth.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[4];
static bool JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions_initialized = false;

static void init_JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions()
{
    if (JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions_initialized) return;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions_initialized = true;
    memset(JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions, 0, sizeof(JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions));
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[0].name = "x";
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[0].magic = JSSVGPathSegCurvetoCubicSmoothAbs::XAttrNum;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[1].name = "y";
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[1].magic = JSSVGPathSegCurvetoCubicSmoothAbs::YAttrNum;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[2].name = "x2";
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[2].magic = JSSVGPathSegCurvetoCubicSmoothAbs::X2AttrNum;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[2].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[3].name = "y2";
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[3].magic = JSSVGPathSegCurvetoCubicSmoothAbs::Y2AttrNum;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty;
}

JSValue JSSVGPathSegCurvetoCubicSmoothAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicSmoothAbs.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoCubicSmoothAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicSmoothAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegCurvetoCubicSmoothAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions, countof(JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoCubicSmoothAbsClassDefine;
static bool JSSVGPathSegCurvetoCubicSmoothAbsClassDefine_initialized = false;

static void init_JSSVGPathSegCurvetoCubicSmoothAbsClassDefine()
{
    if (JSSVGPathSegCurvetoCubicSmoothAbsClassDefine_initialized) return;
    JSSVGPathSegCurvetoCubicSmoothAbsClassDefine_initialized = true;
    memset(&JSSVGPathSegCurvetoCubicSmoothAbsClassDefine, 0, sizeof(JSSVGPathSegCurvetoCubicSmoothAbsClassDefine));
    JSSVGPathSegCurvetoCubicSmoothAbsClassDefine.class_name = "SVGPathSegCurvetoCubicSmoothAbs";
    JSSVGPathSegCurvetoCubicSmoothAbsClassDefine.finalizer = JSSVGPathSegCurvetoCubicSmoothAbs::finalizer;
    JSSVGPathSegCurvetoCubicSmoothAbsClassDefine.gc_mark = JSSVGPathSegCurvetoCubicSmoothAbs::mark;
}

JSClassID JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id = 0;

void JSSVGPathSegCurvetoCubicSmoothAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id == 0) {
        init_JSSVGPathSegCurvetoCubicSmoothAbsClassDefine();
        JS_NewClassID(&JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id, &JSSVGPathSegCurvetoCubicSmoothAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id, JSSVGPathSegCurvetoCubicSmoothAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoCubicSmoothAbs::create(JSContext* ctx, SVGPathSegCurvetoCubicSmoothAbs* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoCubicSmoothAbs::init(ctx);
    JSValue _proto = JSSVGPathSegCurvetoCubicSmoothAbsPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoCubicSmoothAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoCubicSmoothAbs* impl = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoCubicSmoothAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x2());
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y2());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            imp->setX2(valueToFloat(ctx, value));
            break;
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            imp->setY2(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
