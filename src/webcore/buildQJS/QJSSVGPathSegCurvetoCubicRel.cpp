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
#include "QJSSVGPathSegCurvetoCubicRel.h"

#include "SVGPathSegCurvetoCubic.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegCurvetoCubicRelAttributesFunctions[6];
static bool JSSVGPathSegCurvetoCubicRelAttributesFunctions_initialized = false;

static void init_JSSVGPathSegCurvetoCubicRelAttributesFunctions()
{
    if (JSSVGPathSegCurvetoCubicRelAttributesFunctions_initialized) return;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions_initialized = true;
    memset(JSSVGPathSegCurvetoCubicRelAttributesFunctions, 0, sizeof(JSSVGPathSegCurvetoCubicRelAttributesFunctions));
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[0].name = "x";
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[0].magic = JSSVGPathSegCurvetoCubicRel::XAttrNum;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicRel::getValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicRel::putValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[1].name = "y";
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[1].magic = JSSVGPathSegCurvetoCubicRel::YAttrNum;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicRel::getValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicRel::putValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[2].name = "x1";
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[2].magic = JSSVGPathSegCurvetoCubicRel::X1AttrNum;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicRel::getValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[2].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicRel::putValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[3].name = "y1";
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[3].magic = JSSVGPathSegCurvetoCubicRel::Y1AttrNum;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicRel::getValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicRel::putValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[4].name = "x2";
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[4].magic = JSSVGPathSegCurvetoCubicRel::X2AttrNum;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[4].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicRel::getValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[4].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicRel::putValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[5].name = "y2";
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[5].magic = JSSVGPathSegCurvetoCubicRel::Y2AttrNum;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[5].u.getset.get.getter_magic = JSSVGPathSegCurvetoCubicRel::getValueProperty;
    JSSVGPathSegCurvetoCubicRelAttributesFunctions[5].u.getset.set.setter_magic = JSSVGPathSegCurvetoCubicRel::putValueProperty;
}

JSValue JSSVGPathSegCurvetoCubicRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicRel.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoCubicRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegCurvetoCubicRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegCurvetoCubicRelAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoCubicRelAttributesFunctions, countof(JSSVGPathSegCurvetoCubicRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoCubicRelClassDefine;
static bool JSSVGPathSegCurvetoCubicRelClassDefine_initialized = false;

static void init_JSSVGPathSegCurvetoCubicRelClassDefine()
{
    if (JSSVGPathSegCurvetoCubicRelClassDefine_initialized) return;
    JSSVGPathSegCurvetoCubicRelClassDefine_initialized = true;
    memset(&JSSVGPathSegCurvetoCubicRelClassDefine, 0, sizeof(JSSVGPathSegCurvetoCubicRelClassDefine));
    JSSVGPathSegCurvetoCubicRelClassDefine.class_name = "SVGPathSegCurvetoCubicRel";
    JSSVGPathSegCurvetoCubicRelClassDefine.finalizer = JSSVGPathSegCurvetoCubicRel::finalizer;
    JSSVGPathSegCurvetoCubicRelClassDefine.gc_mark = JSSVGPathSegCurvetoCubicRel::mark;
}

JSClassID JSSVGPathSegCurvetoCubicRel::js_class_id = 0;

void JSSVGPathSegCurvetoCubicRel::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoCubicRel::js_class_id == 0) {
        init_JSSVGPathSegCurvetoCubicRelClassDefine();
        JS_NewClassID(&JSSVGPathSegCurvetoCubicRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoCubicRel::js_class_id, &JSSVGPathSegCurvetoCubicRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoCubicRel::js_class_id, JSSVGPathSegCurvetoCubicRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoCubicRel::create(JSContext* ctx, SVGPathSegCurvetoCubicRel* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoCubicRel::init(ctx);
    JSValue _proto = JSSVGPathSegCurvetoCubicRelPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegCurvetoCubicRel::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoCubicRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoCubicRel* impl = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(val, JSSVGPathSegCurvetoCubicRel::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoCubicRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoCubicRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
        case X1AttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            return JS_NewInt32(ctx, imp->x1());
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            return JS_NewInt32(ctx, imp->y1());
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            return JS_NewInt32(ctx, imp->x2());
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            return JS_NewInt32(ctx, imp->y2());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoCubicRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X1AttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            imp->setX1(valueToFloat(ctx, value));
            break;
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            imp->setY1(valueToFloat(ctx, value));
            break;
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
            imp->setX2(valueToFloat(ctx, value));
            break;
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicRel* imp = (SVGPathSegCurvetoCubicRel*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoCubicRel::js_class_id);
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
