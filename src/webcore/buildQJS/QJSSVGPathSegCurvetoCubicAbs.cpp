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
#include "QJSSVGPathSegCurvetoCubicAbs.h"

#include "SVGPathSegCurvetoCubic.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegCurvetoCubicAbsAttributesFunctions[6];
static bool JSSVGPathSegCurvetoCubicAbsAttributesFunctions_initialized = false;

static void init_JSSVGPathSegCurvetoCubicAbsAttributesFunctions()
{
    if (JSSVGPathSegCurvetoCubicAbsAttributesFunctions_initialized) return;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions_initialized = true;
    memset(JSSVGPathSegCurvetoCubicAbsAttributesFunctions, 0, sizeof(JSSVGPathSegCurvetoCubicAbsAttributesFunctions));
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[0].name = "x";
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[0].magic = JSSVGPathSegCurvetoCubicAbs::XAttrNum;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicAbs::putValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[1].name = "y";
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[1].magic = JSSVGPathSegCurvetoCubicAbs::YAttrNum;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicAbs::putValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[2].name = "x1";
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[2].magic = JSSVGPathSegCurvetoCubicAbs::X1AttrNum;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[2].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicAbs::putValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[3].name = "y1";
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[3].magic = JSSVGPathSegCurvetoCubicAbs::Y1AttrNum;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicAbs::putValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[4].name = "x2";
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[4].magic = JSSVGPathSegCurvetoCubicAbs::X2AttrNum;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[4].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[4].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicAbs::putValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[5].name = "y2";
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[5].magic = JSSVGPathSegCurvetoCubicAbs::Y2AttrNum;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[5].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicAbs::getValueProperty;
    JSSVGPathSegCurvetoCubicAbsAttributesFunctions[5].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicAbs::putValueProperty;
}

JSValue JSSVGPathSegCurvetoCubicAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicAbs.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoCubicAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegCurvetoCubicAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegCurvetoCubicAbsAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoCubicAbsAttributesFunctions, countof(JSSVGPathSegCurvetoCubicAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoCubicAbsClassDefine;
static bool JSSVGPathSegCurvetoCubicAbsClassDefine_initialized = false;

static void init_JSSVGPathSegCurvetoCubicAbsClassDefine()
{
    if (JSSVGPathSegCurvetoCubicAbsClassDefine_initialized) return;
    JSSVGPathSegCurvetoCubicAbsClassDefine_initialized = true;
    memset(&JSSVGPathSegCurvetoCubicAbsClassDefine, 0, sizeof(JSSVGPathSegCurvetoCubicAbsClassDefine));
    JSSVGPathSegCurvetoCubicAbsClassDefine.class_name = "SVGPathSegCurvetoCubicAbs";
    JSSVGPathSegCurvetoCubicAbsClassDefine.finalizer = JSSVGPathSegCurvetoCubicAbs::finalizer;
    JSSVGPathSegCurvetoCubicAbsClassDefine.gc_mark = JSSVGPathSegCurvetoCubicAbs::mark;
}

JSClassID JSSVGPathSegCurvetoCubicAbs::js_class_id = 0;

void JSSVGPathSegCurvetoCubicAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoCubicAbs::js_class_id == 0) {
        init_JSSVGPathSegCurvetoCubicAbsClassDefine();
        JS_NewClassID(&JSSVGPathSegCurvetoCubicAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoCubicAbs::js_class_id, &JSSVGPathSegCurvetoCubicAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoCubicAbs::js_class_id, JSSVGPathSegCurvetoCubicAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoCubicAbs::create(JSContext* ctx, SVGPathSegCurvetoCubicAbs* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoCubicAbs::init(ctx);
    JSValue _proto = JSSVGPathSegCurvetoCubicAbsPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegCurvetoCubicAbs::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoCubicAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoCubicAbs* impl = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoCubicAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoCubicAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
        case X1AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x1());
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y1());
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x2());
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y2());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoCubicAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X1AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setX1(valueToFloat(ctx, value));
            break;
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setY1(valueToFloat(ctx, value));
            break;
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setX2(valueToFloat(ctx, value));
            break;
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
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
