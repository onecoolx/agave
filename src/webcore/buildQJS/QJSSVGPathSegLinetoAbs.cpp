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
#include "QJSSVGPathSegLinetoAbs.h"

#include "SVGPathSegLineto.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegLinetoAbsAttributesFunctions[2];
static bool JSSVGPathSegLinetoAbsAttributesFunctions_initialized = false;

static void init_JSSVGPathSegLinetoAbsAttributesFunctions()
{
    if (JSSVGPathSegLinetoAbsAttributesFunctions_initialized) return;
    JSSVGPathSegLinetoAbsAttributesFunctions_initialized = true;
    memset(JSSVGPathSegLinetoAbsAttributesFunctions, 0, sizeof(JSSVGPathSegLinetoAbsAttributesFunctions));
    JSSVGPathSegLinetoAbsAttributesFunctions[0].name = "y";
    JSSVGPathSegLinetoAbsAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegLinetoAbsAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegLinetoAbsAttributesFunctions[0].magic = JSSVGPathSegLinetoAbs::YAttrNum;
    JSSVGPathSegLinetoAbsAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegLinetoAbs::getValueProperty;
    JSSVGPathSegLinetoAbsAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegLinetoAbs::putValueProperty;
    JSSVGPathSegLinetoAbsAttributesFunctions[1].name = "x";
    JSSVGPathSegLinetoAbsAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegLinetoAbsAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegLinetoAbsAttributesFunctions[1].magic = JSSVGPathSegLinetoAbs::XAttrNum;
    JSSVGPathSegLinetoAbsAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegLinetoAbs::getValueProperty;
    JSSVGPathSegLinetoAbsAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegLinetoAbs::putValueProperty;
}

JSValue JSSVGPathSegLinetoAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoAbs.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegLinetoAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegLinetoAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegLinetoAbsAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegLinetoAbsAttributesFunctions, countof(JSSVGPathSegLinetoAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegLinetoAbsClassDefine;
static bool JSSVGPathSegLinetoAbsClassDefine_initialized = false;

static void init_JSSVGPathSegLinetoAbsClassDefine()
{
    if (JSSVGPathSegLinetoAbsClassDefine_initialized) return;
    JSSVGPathSegLinetoAbsClassDefine_initialized = true;
    memset(&JSSVGPathSegLinetoAbsClassDefine, 0, sizeof(JSSVGPathSegLinetoAbsClassDefine));
    JSSVGPathSegLinetoAbsClassDefine.class_name = "SVGPathSegLinetoAbs";
    JSSVGPathSegLinetoAbsClassDefine.finalizer = JSSVGPathSegLinetoAbs::finalizer;
    JSSVGPathSegLinetoAbsClassDefine.gc_mark = JSSVGPathSegLinetoAbs::mark;
}

JSClassID JSSVGPathSegLinetoAbs::js_class_id = 0;

void JSSVGPathSegLinetoAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegLinetoAbs::js_class_id == 0) {
        init_JSSVGPathSegLinetoAbsClassDefine();
        JS_NewClassID(&JSSVGPathSegLinetoAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegLinetoAbs::js_class_id, &JSSVGPathSegLinetoAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegLinetoAbs::js_class_id, JSSVGPathSegLinetoAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegLinetoAbs::create(JSContext* ctx, SVGPathSegLinetoAbs* impl, SVGElement* context)
{
    JSSVGPathSegLinetoAbs::init(ctx);
    JSValue _proto = JSSVGPathSegLinetoAbsPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegLinetoAbs::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegLinetoAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegLinetoAbs* impl = (SVGPathSegLinetoAbs*)JS_GetOpaque(val, JSSVGPathSegLinetoAbs::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegLinetoAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegLinetoAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegLinetoAbs* imp = (SVGPathSegLinetoAbs*)JS_GetOpaque(this_val, JSSVGPathSegLinetoAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegLinetoAbs* imp = (SVGPathSegLinetoAbs*)JS_GetOpaque(this_val, JSSVGPathSegLinetoAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegLinetoAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegLinetoAbs* imp = (SVGPathSegLinetoAbs*)JS_GetOpaque(this_val, JSSVGPathSegLinetoAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegLinetoAbs* imp = (SVGPathSegLinetoAbs*)JS_GetOpaque(this_val, JSSVGPathSegLinetoAbs::js_class_id);
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
