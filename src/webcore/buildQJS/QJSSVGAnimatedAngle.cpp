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
#include "QJSSVGAnimatedAngle.h"

#include "QJSSVGAngle.h"
#include "SVGAngle.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGAnimatedAngleAttributesFunctions[2];
static bool JSSVGAnimatedAngleAttributesFunctions_initialized = false;

static void init_JSSVGAnimatedAngleAttributesFunctions()
{
    if (JSSVGAnimatedAngleAttributesFunctions_initialized) return;
    JSSVGAnimatedAngleAttributesFunctions_initialized = true;
    memset(JSSVGAnimatedAngleAttributesFunctions, 0, sizeof(JSSVGAnimatedAngleAttributesFunctions));
    JSSVGAnimatedAngleAttributesFunctions[0].name = "baseVal";
    JSSVGAnimatedAngleAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedAngleAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedAngleAttributesFunctions[0].magic = JSSVGAnimatedAngle::BaseValAttrNum;
    JSSVGAnimatedAngleAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAnimatedAngle::getValueProperty;
    JSSVGAnimatedAngleAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGAnimatedAngleAttributesFunctions[1].name = "animVal";
    JSSVGAnimatedAngleAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedAngleAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedAngleAttributesFunctions[1].magic = JSSVGAnimatedAngle::AnimValAttrNum;
    JSSVGAnimatedAngleAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAnimatedAngle::getValueProperty;
    JSSVGAnimatedAngleAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGAnimatedAnglePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedAngle.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedAnglePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedAngle.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnimatedAnglePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAnimatedAngleAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedAngleAttributesFunctions, countof(JSSVGAnimatedAngleAttributesFunctions));
}

static JSClassDef JSSVGAnimatedAngleClassDefine;
static bool JSSVGAnimatedAngleClassDefine_initialized = false;

static void init_JSSVGAnimatedAngleClassDefine()
{
    if (JSSVGAnimatedAngleClassDefine_initialized) return;
    JSSVGAnimatedAngleClassDefine_initialized = true;
    memset(&JSSVGAnimatedAngleClassDefine, 0, sizeof(JSSVGAnimatedAngleClassDefine));
    JSSVGAnimatedAngleClassDefine.class_name = "SVGAnimatedAngle";
    JSSVGAnimatedAngleClassDefine.finalizer = JSSVGAnimatedAngle::finalizer;
    JSSVGAnimatedAngleClassDefine.gc_mark = JSSVGAnimatedAngle::mark;
}

JSClassID JSSVGAnimatedAngle::js_class_id = 0;

void JSSVGAnimatedAngle::init(JSContext* ctx)
{
    if (JSSVGAnimatedAngle::js_class_id == 0) {
        init_JSSVGAnimatedAngleClassDefine();
        JS_NewClassID(&JSSVGAnimatedAngle::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedAngle::js_class_id, &JSSVGAnimatedAngleClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedAngle::js_class_id, JSSVGAnimatedAnglePrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedAngle::create(JSContext* ctx, SVGAnimatedAngle* impl, SVGElement* context)
{
    JSSVGAnimatedAngle::init(ctx);
    JSValue _proto = JSSVGAnimatedAnglePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAnimatedAngle::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedAngle::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedAngle* impl = (SVGAnimatedAngle*)JS_GetOpaque(val, JSSVGAnimatedAngle::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedAngle::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedAngle::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedAngle* imp = (SVGAnimatedAngle*)JS_GetOpaque(this_val, JSSVGAnimatedAngle::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->baseVal()), context());
        }
        case AnimValAttrNum: {
            SVGAnimatedAngle* imp = (SVGAnimatedAngle*)JS_GetOpaque(this_val, JSSVGAnimatedAngle::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animVal()), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedAngle* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedAngle, JSSVGAnimatedAngle>(ctx, obj, context);
}

SVGAnimatedAngle* toSVGAnimatedAngle(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedAngle* impl = (SVGAnimatedAngle*)JS_GetOpaque(val, JSSVGAnimatedAngle::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
