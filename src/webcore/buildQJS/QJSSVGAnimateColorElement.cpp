/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
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


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGAnimateColorElement.h"

#include "SVGAnimateColorElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
JSValue JSSVGAnimateColorElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimateColorElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGAnimationElementPrototype::self(ctx));
        JSSVGAnimateColorElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimateColorElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimateColorElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
}

static JSClassDef JSSVGAnimateColorElementClassDefine = 
{
    "SVGAnimateColorElement",
    .finalizer = JSSVGAnimateColorElement::finalizer,
    .gc_mark = JSSVGAnimateColorElement::mark,
};

JSClassID JSSVGAnimateColorElement::js_class_id = 0;

void JSSVGAnimateColorElement::init(JSContext* ctx)
{
    if (JSSVGAnimateColorElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimateColorElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimateColorElement::js_class_id, &JSSVGAnimateColorElementClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimateColorElement::js_class_id, JSSVGAnimateColorElementPrototype::self(ctx));
    }
}

JSValue JSSVGAnimateColorElement::create(JSContext* ctx, SVGAnimateColorElement* impl)
{
    JSSVGAnimateColorElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimateColorElementPrototype::self(ctx), JSSVGAnimateColorElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimateColorElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimateColorElement* impl = (SVGAnimateColorElement*)JS_GetOpaque(val, JSSVGAnimateColorElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimateColorElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGAnimationElement::mark(rt, val, mark_func);
}


}

#endif // ENABLE(SVG)
