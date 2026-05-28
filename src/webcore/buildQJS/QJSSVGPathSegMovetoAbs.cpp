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
#include "QJSSVGPathSegMovetoAbs.h"

#include "SVGPathSegMoveto.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegMovetoAbsAttributesFunctions[2];
static bool JSSVGPathSegMovetoAbsAttributesFunctions_initialized = false;

static void init_JSSVGPathSegMovetoAbsAttributesFunctions()
{
    if (JSSVGPathSegMovetoAbsAttributesFunctions_initialized) return;
    JSSVGPathSegMovetoAbsAttributesFunctions_initialized = true;
    memset(JSSVGPathSegMovetoAbsAttributesFunctions, 0, sizeof(JSSVGPathSegMovetoAbsAttributesFunctions));
    JSSVGPathSegMovetoAbsAttributesFunctions[0].name = "x";
    JSSVGPathSegMovetoAbsAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegMovetoAbsAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegMovetoAbsAttributesFunctions[0].magic = JSSVGPathSegMovetoAbs::XAttrNum;
    JSSVGPathSegMovetoAbsAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegMovetoAbs::getValueProperty;
    JSSVGPathSegMovetoAbsAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegMovetoAbs::putValueProperty;
    JSSVGPathSegMovetoAbsAttributesFunctions[1].name = "y";
    JSSVGPathSegMovetoAbsAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegMovetoAbsAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegMovetoAbsAttributesFunctions[1].magic = JSSVGPathSegMovetoAbs::YAttrNum;
    JSSVGPathSegMovetoAbsAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegMovetoAbs::getValueProperty;
    JSSVGPathSegMovetoAbsAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegMovetoAbs::putValueProperty;
}

JSValue JSSVGPathSegMovetoAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegMovetoAbs.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegMovetoAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegMovetoAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegMovetoAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegMovetoAbsAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegMovetoAbsAttributesFunctions, countof(JSSVGPathSegMovetoAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegMovetoAbsClassDefine;
static bool JSSVGPathSegMovetoAbsClassDefine_initialized = false;

static void init_JSSVGPathSegMovetoAbsClassDefine()
{
    if (JSSVGPathSegMovetoAbsClassDefine_initialized) return;
    JSSVGPathSegMovetoAbsClassDefine_initialized = true;
    memset(&JSSVGPathSegMovetoAbsClassDefine, 0, sizeof(JSSVGPathSegMovetoAbsClassDefine));
    JSSVGPathSegMovetoAbsClassDefine.class_name = "SVGPathSegMovetoAbs";
    JSSVGPathSegMovetoAbsClassDefine.finalizer = JSSVGPathSegMovetoAbs::finalizer;
    JSSVGPathSegMovetoAbsClassDefine.gc_mark = JSSVGPathSegMovetoAbs::mark;
}

JSClassID JSSVGPathSegMovetoAbs::js_class_id = 0;

void JSSVGPathSegMovetoAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegMovetoAbs::js_class_id == 0) {
        init_JSSVGPathSegMovetoAbsClassDefine();
        JS_NewClassID(&JSSVGPathSegMovetoAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegMovetoAbs::js_class_id, &JSSVGPathSegMovetoAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegMovetoAbs::js_class_id, JSSVGPathSegMovetoAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegMovetoAbs::create(JSContext* ctx, SVGPathSegMovetoAbs* impl, SVGElement* context)
{
    JSSVGPathSegMovetoAbs::init(ctx);
    JSValue _proto = JSSVGPathSegMovetoAbsPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegMovetoAbs::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegMovetoAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegMovetoAbs* impl = (SVGPathSegMovetoAbs*)JS_GetOpaque(val, JSSVGPathSegMovetoAbs::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegMovetoAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegMovetoAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegMovetoAbs* imp = (SVGPathSegMovetoAbs*)JS_GetOpaque(this_val, JSSVGPathSegMovetoAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegMovetoAbs* imp = (SVGPathSegMovetoAbs*)JS_GetOpaque(this_val, JSSVGPathSegMovetoAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegMovetoAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegMovetoAbs* imp = (SVGPathSegMovetoAbs*)JS_GetOpaque(this_val, JSSVGPathSegMovetoAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegMovetoAbs* imp = (SVGPathSegMovetoAbs*)JS_GetOpaque(this_val, JSSVGPathSegMovetoAbs::js_class_id);
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
