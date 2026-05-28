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
#include "QJSSVGPathSegArcRel.h"

#include "SVGPathSegArc.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegArcRelAttributesFunctions[7];
static bool JSSVGPathSegArcRelAttributesFunctions_initialized = false;

static void init_JSSVGPathSegArcRelAttributesFunctions()
{
    if (JSSVGPathSegArcRelAttributesFunctions_initialized) return;
    JSSVGPathSegArcRelAttributesFunctions_initialized = true;
    memset(JSSVGPathSegArcRelAttributesFunctions, 0, sizeof(JSSVGPathSegArcRelAttributesFunctions));
    JSSVGPathSegArcRelAttributesFunctions[0].name = "x";
    JSSVGPathSegArcRelAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcRelAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcRelAttributesFunctions[0].magic = JSSVGPathSegArcRel::XAttrNum;
    JSSVGPathSegArcRelAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegArcRel::getValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegArcRel::putValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[1].name = "y";
    JSSVGPathSegArcRelAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcRelAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcRelAttributesFunctions[1].magic = JSSVGPathSegArcRel::YAttrNum;
    JSSVGPathSegArcRelAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegArcRel::getValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegArcRel::putValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[2].name = "r1";
    JSSVGPathSegArcRelAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcRelAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcRelAttributesFunctions[2].magic = JSSVGPathSegArcRel::R1AttrNum;
    JSSVGPathSegArcRelAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathSegArcRel::getValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[2].u.getset.set.setter_magic = JSSVGPathSegArcRel::putValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[3].name = "r2";
    JSSVGPathSegArcRelAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcRelAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcRelAttributesFunctions[3].magic = JSSVGPathSegArcRel::R2AttrNum;
    JSSVGPathSegArcRelAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPathSegArcRel::getValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPathSegArcRel::putValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[4].name = "angle";
    JSSVGPathSegArcRelAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcRelAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcRelAttributesFunctions[4].magic = JSSVGPathSegArcRel::AngleAttrNum;
    JSSVGPathSegArcRelAttributesFunctions[4].u.getset.get.getter_magic = JSSVGPathSegArcRel::getValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[4].u.getset.set.setter_magic = JSSVGPathSegArcRel::putValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[5].name = "largeArcFlag";
    JSSVGPathSegArcRelAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcRelAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcRelAttributesFunctions[5].magic = JSSVGPathSegArcRel::LargeArcFlagAttrNum;
    JSSVGPathSegArcRelAttributesFunctions[5].u.getset.get.getter_magic = JSSVGPathSegArcRel::getValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[5].u.getset.set.setter_magic = JSSVGPathSegArcRel::putValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[6].name = "sweepFlag";
    JSSVGPathSegArcRelAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcRelAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcRelAttributesFunctions[6].magic = JSSVGPathSegArcRel::SweepFlagAttrNum;
    JSSVGPathSegArcRelAttributesFunctions[6].u.getset.get.getter_magic = JSSVGPathSegArcRel::getValueProperty;
    JSSVGPathSegArcRelAttributesFunctions[6].u.getset.set.setter_magic = JSSVGPathSegArcRel::putValueProperty;
}

JSValue JSSVGPathSegArcRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegArcRel.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegArcRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegArcRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegArcRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegArcRelAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegArcRelAttributesFunctions, countof(JSSVGPathSegArcRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegArcRelClassDefine;
static bool JSSVGPathSegArcRelClassDefine_initialized = false;

static void init_JSSVGPathSegArcRelClassDefine()
{
    if (JSSVGPathSegArcRelClassDefine_initialized) return;
    JSSVGPathSegArcRelClassDefine_initialized = true;
    memset(&JSSVGPathSegArcRelClassDefine, 0, sizeof(JSSVGPathSegArcRelClassDefine));
    JSSVGPathSegArcRelClassDefine.class_name = "SVGPathSegArcRel";
    JSSVGPathSegArcRelClassDefine.finalizer = JSSVGPathSegArcRel::finalizer;
    JSSVGPathSegArcRelClassDefine.gc_mark = JSSVGPathSegArcRel::mark;
}

JSClassID JSSVGPathSegArcRel::js_class_id = 0;

void JSSVGPathSegArcRel::init(JSContext* ctx)
{
    if (JSSVGPathSegArcRel::js_class_id == 0) {
        init_JSSVGPathSegArcRelClassDefine();
        JS_NewClassID(&JSSVGPathSegArcRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegArcRel::js_class_id, &JSSVGPathSegArcRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegArcRel::js_class_id, JSSVGPathSegArcRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegArcRel::create(JSContext* ctx, SVGPathSegArcRel* impl, SVGElement* context)
{
    JSSVGPathSegArcRel::init(ctx);
    JSValue _proto = JSSVGPathSegArcRelPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegArcRel::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegArcRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegArcRel* impl = (SVGPathSegArcRel*)JS_GetOpaque(val, JSSVGPathSegArcRel::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegArcRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegArcRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
        case R1AttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewInt32(ctx, imp->r1());
        }
        case R2AttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewInt32(ctx, imp->r2());
        }
        case AngleAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewInt32(ctx, imp->angle());
        }
        case LargeArcFlagAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBool(ctx, imp->largeArcFlag() ? 1 : 0);
        }
        case SweepFlagAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBool(ctx, imp->sweepFlag() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegArcRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case R1AttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setR1(valueToFloat(ctx, value));
            break;
        }
        case R2AttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setR2(valueToFloat(ctx, value));
            break;
        }
        case AngleAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setAngle(valueToFloat(ctx, value));
            break;
        }
        case LargeArcFlagAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setLargeArcFlag(valueToBoolean(ctx, value));
            break;
        }
        case SweepFlagAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque(this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setSweepFlag(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
