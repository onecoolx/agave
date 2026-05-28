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
#include "QJSSVGPathSegLinetoHorizontalAbs.h"

#include "SVGPathSegLinetoHorizontal.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegLinetoHorizontalAbsAttributesFunctions[1];
static bool JSSVGPathSegLinetoHorizontalAbsAttributesFunctions_initialized = false;

static void init_JSSVGPathSegLinetoHorizontalAbsAttributesFunctions()
{
    if (JSSVGPathSegLinetoHorizontalAbsAttributesFunctions_initialized) return;
    JSSVGPathSegLinetoHorizontalAbsAttributesFunctions_initialized = true;
    memset(JSSVGPathSegLinetoHorizontalAbsAttributesFunctions, 0, sizeof(JSSVGPathSegLinetoHorizontalAbsAttributesFunctions));
    JSSVGPathSegLinetoHorizontalAbsAttributesFunctions[0].name = "x";
    JSSVGPathSegLinetoHorizontalAbsAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegLinetoHorizontalAbsAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegLinetoHorizontalAbsAttributesFunctions[0].magic = JSSVGPathSegLinetoHorizontalAbs::XAttrNum;
    JSSVGPathSegLinetoHorizontalAbsAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegLinetoHorizontalAbs::getValueProperty;
    JSSVGPathSegLinetoHorizontalAbsAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegLinetoHorizontalAbs::putValueProperty;
}

JSValue JSSVGPathSegLinetoHorizontalAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoHorizontalAbs.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegLinetoHorizontalAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoHorizontalAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegLinetoHorizontalAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegLinetoHorizontalAbsAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegLinetoHorizontalAbsAttributesFunctions, countof(JSSVGPathSegLinetoHorizontalAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegLinetoHorizontalAbsClassDefine;
static bool JSSVGPathSegLinetoHorizontalAbsClassDefine_initialized = false;

static void init_JSSVGPathSegLinetoHorizontalAbsClassDefine()
{
    if (JSSVGPathSegLinetoHorizontalAbsClassDefine_initialized) return;
    JSSVGPathSegLinetoHorizontalAbsClassDefine_initialized = true;
    memset(&JSSVGPathSegLinetoHorizontalAbsClassDefine, 0, sizeof(JSSVGPathSegLinetoHorizontalAbsClassDefine));
    JSSVGPathSegLinetoHorizontalAbsClassDefine.class_name = "SVGPathSegLinetoHorizontalAbs";
    JSSVGPathSegLinetoHorizontalAbsClassDefine.finalizer = JSSVGPathSegLinetoHorizontalAbs::finalizer;
    JSSVGPathSegLinetoHorizontalAbsClassDefine.gc_mark = JSSVGPathSegLinetoHorizontalAbs::mark;
}

JSClassID JSSVGPathSegLinetoHorizontalAbs::js_class_id = 0;

void JSSVGPathSegLinetoHorizontalAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegLinetoHorizontalAbs::js_class_id == 0) {
        init_JSSVGPathSegLinetoHorizontalAbsClassDefine();
        JS_NewClassID(&JSSVGPathSegLinetoHorizontalAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegLinetoHorizontalAbs::js_class_id, &JSSVGPathSegLinetoHorizontalAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegLinetoHorizontalAbs::js_class_id, JSSVGPathSegLinetoHorizontalAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegLinetoHorizontalAbs::create(JSContext* ctx, SVGPathSegLinetoHorizontalAbs* impl, SVGElement* context)
{
    JSSVGPathSegLinetoHorizontalAbs::init(ctx);
    JSValue _proto = JSSVGPathSegLinetoHorizontalAbsPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegLinetoHorizontalAbs::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegLinetoHorizontalAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegLinetoHorizontalAbs* impl = (SVGPathSegLinetoHorizontalAbs*)JS_GetOpaque(val, JSSVGPathSegLinetoHorizontalAbs::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegLinetoHorizontalAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegLinetoHorizontalAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegLinetoHorizontalAbs* imp = (SVGPathSegLinetoHorizontalAbs*)JS_GetOpaque(this_val, JSSVGPathSegLinetoHorizontalAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegLinetoHorizontalAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegLinetoHorizontalAbs* imp = (SVGPathSegLinetoHorizontalAbs*)JS_GetOpaque(this_val, JSSVGPathSegLinetoHorizontalAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
