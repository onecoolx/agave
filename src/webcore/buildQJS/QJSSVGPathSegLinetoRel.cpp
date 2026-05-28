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
#include "QJSSVGPathSegLinetoRel.h"

#include "SVGPathSegLineto.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegLinetoRelAttributesFunctions[2];
static bool JSSVGPathSegLinetoRelAttributesFunctions_initialized = false;

static void init_JSSVGPathSegLinetoRelAttributesFunctions()
{
    if (JSSVGPathSegLinetoRelAttributesFunctions_initialized) return;
    JSSVGPathSegLinetoRelAttributesFunctions_initialized = true;
    memset(JSSVGPathSegLinetoRelAttributesFunctions, 0, sizeof(JSSVGPathSegLinetoRelAttributesFunctions));
    JSSVGPathSegLinetoRelAttributesFunctions[0].name = "x";
    JSSVGPathSegLinetoRelAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegLinetoRelAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegLinetoRelAttributesFunctions[0].magic = JSSVGPathSegLinetoRel::XAttrNum;
    JSSVGPathSegLinetoRelAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegLinetoRel::getValueProperty;
    JSSVGPathSegLinetoRelAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegLinetoRel::putValueProperty;
    JSSVGPathSegLinetoRelAttributesFunctions[1].name = "y";
    JSSVGPathSegLinetoRelAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegLinetoRelAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegLinetoRelAttributesFunctions[1].magic = JSSVGPathSegLinetoRel::YAttrNum;
    JSSVGPathSegLinetoRelAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegLinetoRel::getValueProperty;
    JSSVGPathSegLinetoRelAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegLinetoRel::putValueProperty;
}

JSValue JSSVGPathSegLinetoRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoRel.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegLinetoRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegLinetoRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegLinetoRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegLinetoRelAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegLinetoRelAttributesFunctions, countof(JSSVGPathSegLinetoRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegLinetoRelClassDefine;
static bool JSSVGPathSegLinetoRelClassDefine_initialized = false;

static void init_JSSVGPathSegLinetoRelClassDefine()
{
    if (JSSVGPathSegLinetoRelClassDefine_initialized) return;
    JSSVGPathSegLinetoRelClassDefine_initialized = true;
    memset(&JSSVGPathSegLinetoRelClassDefine, 0, sizeof(JSSVGPathSegLinetoRelClassDefine));
    JSSVGPathSegLinetoRelClassDefine.class_name = "SVGPathSegLinetoRel";
    JSSVGPathSegLinetoRelClassDefine.finalizer = JSSVGPathSegLinetoRel::finalizer;
    JSSVGPathSegLinetoRelClassDefine.gc_mark = JSSVGPathSegLinetoRel::mark;
}

JSClassID JSSVGPathSegLinetoRel::js_class_id = 0;

void JSSVGPathSegLinetoRel::init(JSContext* ctx)
{
    if (JSSVGPathSegLinetoRel::js_class_id == 0) {
        init_JSSVGPathSegLinetoRelClassDefine();
        JS_NewClassID(&JSSVGPathSegLinetoRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegLinetoRel::js_class_id, &JSSVGPathSegLinetoRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegLinetoRel::js_class_id, JSSVGPathSegLinetoRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegLinetoRel::create(JSContext* ctx, SVGPathSegLinetoRel* impl, SVGElement* context)
{
    JSSVGPathSegLinetoRel::init(ctx);
    JSValue _proto = JSSVGPathSegLinetoRelPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegLinetoRel::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegLinetoRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegLinetoRel* impl = (SVGPathSegLinetoRel*)JS_GetOpaque(val, JSSVGPathSegLinetoRel::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegLinetoRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegLinetoRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegLinetoRel* imp = (SVGPathSegLinetoRel*)JS_GetOpaque(this_val, JSSVGPathSegLinetoRel::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegLinetoRel* imp = (SVGPathSegLinetoRel*)JS_GetOpaque(this_val, JSSVGPathSegLinetoRel::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegLinetoRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegLinetoRel* imp = (SVGPathSegLinetoRel*)JS_GetOpaque(this_val, JSSVGPathSegLinetoRel::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegLinetoRel* imp = (SVGPathSegLinetoRel*)JS_GetOpaque(this_val, JSSVGPathSegLinetoRel::js_class_id);
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
