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
#include "QJSSVGPathSegCurvetoQuadraticAbs.h"

#include "SVGPathSegCurvetoQuadratic.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[4];
static bool JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions_initialized = false;

static void init_JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions()
{
    if (JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions_initialized) return;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions_initialized = true;
    memset(JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions, 0, sizeof(JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions));
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[0].name = "y";
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[0].magic = JSSVGPathSegCurvetoQuadraticAbs::YAttrNum;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticAbs::getValueProperty;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticAbs::putValueProperty;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[1].name = "x";
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[1].magic = JSSVGPathSegCurvetoQuadraticAbs::XAttrNum;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticAbs::getValueProperty;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticAbs::putValueProperty;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[2].name = "x1";
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[2].magic = JSSVGPathSegCurvetoQuadraticAbs::X1AttrNum;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticAbs::getValueProperty;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[2].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticAbs::putValueProperty;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[3].name = "y1";
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[3].magic = JSSVGPathSegCurvetoQuadraticAbs::Y1AttrNum;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticAbs::getValueProperty;
    JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticAbs::putValueProperty;
}

JSValue JSSVGPathSegCurvetoQuadraticAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticAbs.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoQuadraticAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegCurvetoQuadraticAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions, countof(JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoQuadraticAbsClassDefine;
static bool JSSVGPathSegCurvetoQuadraticAbsClassDefine_initialized = false;

static void init_JSSVGPathSegCurvetoQuadraticAbsClassDefine()
{
    if (JSSVGPathSegCurvetoQuadraticAbsClassDefine_initialized) return;
    JSSVGPathSegCurvetoQuadraticAbsClassDefine_initialized = true;
    memset(&JSSVGPathSegCurvetoQuadraticAbsClassDefine, 0, sizeof(JSSVGPathSegCurvetoQuadraticAbsClassDefine));
    JSSVGPathSegCurvetoQuadraticAbsClassDefine.class_name = "SVGPathSegCurvetoQuadraticAbs";
    JSSVGPathSegCurvetoQuadraticAbsClassDefine.finalizer = JSSVGPathSegCurvetoQuadraticAbs::finalizer;
    JSSVGPathSegCurvetoQuadraticAbsClassDefine.gc_mark = JSSVGPathSegCurvetoQuadraticAbs::mark;
}

JSClassID JSSVGPathSegCurvetoQuadraticAbs::js_class_id = 0;

void JSSVGPathSegCurvetoQuadraticAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoQuadraticAbs::js_class_id == 0) {
        init_JSSVGPathSegCurvetoQuadraticAbsClassDefine();
        JS_NewClassID(&JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoQuadraticAbs::js_class_id, &JSSVGPathSegCurvetoQuadraticAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoQuadraticAbs::js_class_id, JSSVGPathSegCurvetoQuadraticAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoQuadraticAbs::create(JSContext* ctx, SVGPathSegCurvetoQuadraticAbs* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoQuadraticAbs::init(ctx);
    JSValue _proto = JSSVGPathSegCurvetoQuadraticAbsPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoQuadraticAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoQuadraticAbs* impl = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoQuadraticAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoQuadraticAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
        case X1AttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x1());
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y1());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoQuadraticAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X1AttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            imp->setX1(valueToFloat(ctx, value));
            break;
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            imp->setY1(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
