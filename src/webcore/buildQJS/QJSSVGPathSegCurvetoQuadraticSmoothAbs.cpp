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
#include "QJSSVGPathSegCurvetoQuadraticSmoothAbs.h"

#include "SVGPathSegCurvetoQuadraticSmooth.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[2];
static bool JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions_initialized = false;

static void init_JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions()
{
    if (JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions_initialized) return;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions_initialized = true;
    memset(JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions, 0, sizeof(JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions));
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[0].name = "y";
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[0].magic = JSSVGPathSegCurvetoQuadraticSmoothAbs::YAttrNum;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticSmoothAbs::getValueProperty;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticSmoothAbs::putValueProperty;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[1].name = "x";
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[1].magic = JSSVGPathSegCurvetoQuadraticSmoothAbs::XAttrNum;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSegCurvetoQuadraticSmoothAbs::getValueProperty;
    JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPathSegCurvetoQuadraticSmoothAbs::putValueProperty;
}

JSValue JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticSmoothAbs.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticSmoothAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions, countof(JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine;
static bool JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine_initialized = false;

static void init_JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine()
{
    if (JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine_initialized) return;
    JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine_initialized = true;
    memset(&JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine, 0, sizeof(JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine));
    JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine.class_name = "SVGPathSegCurvetoQuadraticSmoothAbs";
    JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine.finalizer = JSSVGPathSegCurvetoQuadraticSmoothAbs::finalizer;
    JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine.gc_mark = JSSVGPathSegCurvetoQuadraticSmoothAbs::mark;
}

JSClassID JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id = 0;

void JSSVGPathSegCurvetoQuadraticSmoothAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id == 0) {
        init_JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine();
        JS_NewClassID(&JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id, &JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id, JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoQuadraticSmoothAbs::create(JSContext* ctx, SVGPathSegCurvetoQuadraticSmoothAbs* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoQuadraticSmoothAbs::init(ctx);
    JSValue _proto = JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoQuadraticSmoothAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoQuadraticSmoothAbs* impl = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque(val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoQuadraticSmoothAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoQuadraticSmoothAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticSmoothAbs* imp = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticSmoothAbs* imp = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoQuadraticSmoothAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticSmoothAbs* imp = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticSmoothAbs* imp = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque(this_val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
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
