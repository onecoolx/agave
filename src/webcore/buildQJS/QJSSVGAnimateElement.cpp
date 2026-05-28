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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGAnimateElement.h"

#include "SVGAnimateElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
JSValue JSSVGAnimateElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimateElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGAnimationElementPrototype::self(ctx));
        JSSVGAnimateElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimateElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnimateElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
}

static JSClassDef JSSVGAnimateElementClassDefine;
static bool JSSVGAnimateElementClassDefine_initialized = false;

static void init_JSSVGAnimateElementClassDefine()
{
    if (JSSVGAnimateElementClassDefine_initialized) return;
    JSSVGAnimateElementClassDefine_initialized = true;
    memset(&JSSVGAnimateElementClassDefine, 0, sizeof(JSSVGAnimateElementClassDefine));
    JSSVGAnimateElementClassDefine.class_name = "SVGAnimateElement";
    JSSVGAnimateElementClassDefine.finalizer = JSSVGAnimateElement::finalizer;
    JSSVGAnimateElementClassDefine.gc_mark = JSSVGAnimateElement::mark;
}

JSClassID JSSVGAnimateElement::js_class_id = 0;

void JSSVGAnimateElement::init(JSContext* ctx)
{
    if (JSSVGAnimateElement::js_class_id == 0) {
        init_JSSVGAnimateElementClassDefine();
        JS_NewClassID(&JSSVGAnimateElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimateElement::js_class_id, &JSSVGAnimateElementClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimateElement::js_class_id, JSSVGAnimateElementPrototype::self(ctx));
    }
}

JSValue JSSVGAnimateElement::create(JSContext* ctx, SVGAnimateElement* impl)
{
    JSSVGAnimateElement::init(ctx);
    JSValue _proto = JSSVGAnimateElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAnimateElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimateElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimateElement* impl = (SVGAnimateElement*)JS_GetOpaque(val, JSSVGAnimateElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimateElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGAnimationElement::mark(rt, val, mark_func);
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
