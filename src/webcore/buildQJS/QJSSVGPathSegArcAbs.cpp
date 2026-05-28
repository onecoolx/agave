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
#include "QJSSVGPathSegArcAbs.h"

#include "SVGPathSegArc.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegArcAbsAttributesFunctions[7];
static bool JSSVGPathSegArcAbsAttributesFunctions_initialized = false;

static void init_JSSVGPathSegArcAbsAttributesFunctions()
{
    if (JSSVGPathSegArcAbsAttributesFunctions_initialized) return;
    JSSVGPathSegArcAbsAttributesFunctions_initialized = true;
    memset(JSSVGPathSegArcAbsAttributesFunctions, 0, sizeof(JSSVGPathSegArcAbsAttributesFunctions));
    JSSVGPathSegArcAbsAttributesFunctions[0].name = "r2";
    JSSVGPathSegArcAbsAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcAbsAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcAbsAttributesFunctions[0].magic = JSSVGPathSegArcAbs::R2AttrNum;
    JSSVGPathSegArcAbsAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegArcAbs::getValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegArcAbs::putValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[1].name = "x";
    JSSVGPathSegArcAbsAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcAbsAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcAbsAttributesFunctions[1].magic = JSSVGPathSegArcAbs::XAttrNum;
    JSSVGPathSegArcAbsAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegArcAbs::getValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegArcAbs::putValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[2].name = "y";
    JSSVGPathSegArcAbsAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcAbsAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcAbsAttributesFunctions[2].magic = JSSVGPathSegArcAbs::YAttrNum;
    JSSVGPathSegArcAbsAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathSegArcAbs::getValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[2].u.getset.set.setter_magic = JSSVGPathSegArcAbs::putValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[3].name = "r1";
    JSSVGPathSegArcAbsAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcAbsAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcAbsAttributesFunctions[3].magic = JSSVGPathSegArcAbs::R1AttrNum;
    JSSVGPathSegArcAbsAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPathSegArcAbs::getValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPathSegArcAbs::putValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[4].name = "angle";
    JSSVGPathSegArcAbsAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcAbsAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcAbsAttributesFunctions[4].magic = JSSVGPathSegArcAbs::AngleAttrNum;
    JSSVGPathSegArcAbsAttributesFunctions[4].u.getset.get.getter_magic = JSSVGPathSegArcAbs::getValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[4].u.getset.set.setter_magic = JSSVGPathSegArcAbs::putValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[5].name = "largeArcFlag";
    JSSVGPathSegArcAbsAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcAbsAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcAbsAttributesFunctions[5].magic = JSSVGPathSegArcAbs::LargeArcFlagAttrNum;
    JSSVGPathSegArcAbsAttributesFunctions[5].u.getset.get.getter_magic = JSSVGPathSegArcAbs::getValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[5].u.getset.set.setter_magic = JSSVGPathSegArcAbs::putValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[6].name = "sweepFlag";
    JSSVGPathSegArcAbsAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegArcAbsAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegArcAbsAttributesFunctions[6].magic = JSSVGPathSegArcAbs::SweepFlagAttrNum;
    JSSVGPathSegArcAbsAttributesFunctions[6].u.getset.get.getter_magic = JSSVGPathSegArcAbs::getValueProperty;
    JSSVGPathSegArcAbsAttributesFunctions[6].u.getset.set.setter_magic = JSSVGPathSegArcAbs::putValueProperty;
}

JSValue JSSVGPathSegArcAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegArcAbs.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegArcAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegArcAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegArcAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegArcAbsAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegArcAbsAttributesFunctions, countof(JSSVGPathSegArcAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegArcAbsClassDefine;
static bool JSSVGPathSegArcAbsClassDefine_initialized = false;

static void init_JSSVGPathSegArcAbsClassDefine()
{
    if (JSSVGPathSegArcAbsClassDefine_initialized) return;
    JSSVGPathSegArcAbsClassDefine_initialized = true;
    memset(&JSSVGPathSegArcAbsClassDefine, 0, sizeof(JSSVGPathSegArcAbsClassDefine));
    JSSVGPathSegArcAbsClassDefine.class_name = "SVGPathSegArcAbs";
    JSSVGPathSegArcAbsClassDefine.finalizer = JSSVGPathSegArcAbs::finalizer;
    JSSVGPathSegArcAbsClassDefine.gc_mark = JSSVGPathSegArcAbs::mark;
}

JSClassID JSSVGPathSegArcAbs::js_class_id = 0;

void JSSVGPathSegArcAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegArcAbs::js_class_id == 0) {
        init_JSSVGPathSegArcAbsClassDefine();
        JS_NewClassID(&JSSVGPathSegArcAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegArcAbs::js_class_id, &JSSVGPathSegArcAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegArcAbs::js_class_id, JSSVGPathSegArcAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegArcAbs::create(JSContext* ctx, SVGPathSegArcAbs* impl, SVGElement* context)
{
    JSSVGPathSegArcAbs::init(ctx);
    JSValue _proto = JSSVGPathSegArcAbsPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegArcAbs::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegArcAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegArcAbs* impl = (SVGPathSegArcAbs*)JS_GetOpaque(val, JSSVGPathSegArcAbs::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegArcAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegArcAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
        case R1AttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewInt32(ctx, imp->r1());
        }
        case R2AttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewInt32(ctx, imp->r2());
        }
        case AngleAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewInt32(ctx, imp->angle());
        }
        case LargeArcFlagAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBool(ctx, imp->largeArcFlag() ? 1 : 0);
        }
        case SweepFlagAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBool(ctx, imp->sweepFlag() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegArcAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case R1AttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setR1(valueToFloat(ctx, value));
            break;
        }
        case R2AttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setR2(valueToFloat(ctx, value));
            break;
        }
        case AngleAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setAngle(valueToFloat(ctx, value));
            break;
        }
        case LargeArcFlagAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setLargeArcFlag(valueToBoolean(ctx, value));
            break;
        }
        case SweepFlagAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque(this_val, JSSVGPathSegArcAbs::js_class_id);
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
