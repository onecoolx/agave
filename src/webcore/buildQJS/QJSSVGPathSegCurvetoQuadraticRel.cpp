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
#include "QJSSVGPathSegCurvetoQuadraticRel.h"

#include "SVGPathSegCurvetoQuadratic.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[4];
static bool JSSVGPathSegCurvetoQuadraticRelAttributesFunctions_initialized = false;

static void init_JSSVGPathSegCurvetoQuadraticRelAttributesFunctions()
{
    if (JSSVGPathSegCurvetoQuadraticRelAttributesFunctions_initialized) return;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions_initialized = true;
    memset(JSSVGPathSegCurvetoQuadraticRelAttributesFunctions, 0, sizeof(JSSVGPathSegCurvetoQuadraticRelAttributesFunctions));
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[0].name = "x";
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[0].magic = JSSVGPathSegCurvetoQuadraticRel::XAttrNum;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticRel::getValueProperty;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticRel::putValueProperty;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[1].name = "y";
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[1].magic = JSSVGPathSegCurvetoQuadraticRel::YAttrNum;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticRel::getValueProperty;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticRel::putValueProperty;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[2].name = "x1";
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[2].magic = JSSVGPathSegCurvetoQuadraticRel::X1AttrNum;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticRel::getValueProperty;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[2].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticRel::putValueProperty;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[3].name = "y1";
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[3].magic = JSSVGPathSegCurvetoQuadraticRel::Y1AttrNum;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticRel::getValueProperty;
    JSSVGPathSegCurvetoQuadraticRelAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticRel::putValueProperty;
}

JSValue JSSVGPathSegCurvetoQuadraticRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticRel.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoQuadraticRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegCurvetoQuadraticRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegCurvetoQuadraticRelAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoQuadraticRelAttributesFunctions, countof(JSSVGPathSegCurvetoQuadraticRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoQuadraticRelClassDefine;
static bool JSSVGPathSegCurvetoQuadraticRelClassDefine_initialized = false;

static void init_JSSVGPathSegCurvetoQuadraticRelClassDefine()
{
    if (JSSVGPathSegCurvetoQuadraticRelClassDefine_initialized) return;
    JSSVGPathSegCurvetoQuadraticRelClassDefine_initialized = true;
    memset(&JSSVGPathSegCurvetoQuadraticRelClassDefine, 0, sizeof(JSSVGPathSegCurvetoQuadraticRelClassDefine));
    JSSVGPathSegCurvetoQuadraticRelClassDefine.class_name = "SVGPathSegCurvetoQuadraticRel";
    JSSVGPathSegCurvetoQuadraticRelClassDefine.finalizer = JSSVGPathSegCurvetoQuadraticRel::finalizer;
    JSSVGPathSegCurvetoQuadraticRelClassDefine.gc_mark = JSSVGPathSegCurvetoQuadraticRel::mark;
}

JSClassID JSSVGPathSegCurvetoQuadraticRel::js_class_id = 0;

void JSSVGPathSegCurvetoQuadraticRel::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoQuadraticRel::js_class_id == 0) {
        init_JSSVGPathSegCurvetoQuadraticRelClassDefine();
        JS_NewClassID(&JSSVGPathSegCurvetoQuadraticRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoQuadraticRel::js_class_id, &JSSVGPathSegCurvetoQuadraticRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoQuadraticRel::js_class_id, JSSVGPathSegCurvetoQuadraticRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoQuadraticRel::create(JSContext* ctx, SVGPathSegCurvetoQuadraticRel* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoQuadraticRel::init(ctx);
    JSValue _proto = JSSVGPathSegCurvetoQuadraticRelPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoQuadraticRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoQuadraticRel* impl = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoQuadraticRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoQuadraticRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticRel* imp = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticRel* imp = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
        case X1AttrNum: {
            SVGPathSegCurvetoQuadraticRel* imp = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
            return JS_NewInt32(ctx, imp->x1());
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoQuadraticRel* imp = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
            return JS_NewInt32(ctx, imp->y1());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoQuadraticRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticRel* imp = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticRel* imp = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X1AttrNum: {
            SVGPathSegCurvetoQuadraticRel* imp = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
            imp->setX1(valueToFloat(ctx, value));
            break;
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoQuadraticRel* imp = (SVGPathSegCurvetoQuadraticRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticRel::js_class_id);
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
