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
#include "QJSSVGTSpanElement.h"

#include "SVGTSpanElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
JSValue JSSVGTSpanElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTSpanElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGTextPositioningElementPrototype::self(ctx));
        JSSVGTSpanElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTSpanElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTSpanElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
}

static JSClassDef JSSVGTSpanElementClassDefine = 
{
    "SVGTSpanElement",
    .finalizer = JSSVGTSpanElement::finalizer,
    .gc_mark = JSSVGTSpanElement::mark,
};

JSClassID JSSVGTSpanElement::js_class_id = 0;

void JSSVGTSpanElement::init(JSContext* ctx)
{
    if (JSSVGTSpanElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGTSpanElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTSpanElement::js_class_id, &JSSVGTSpanElementClassDefine);
        JS_SetClassProto(ctx, JSSVGTSpanElement::js_class_id, JSSVGTSpanElementPrototype::self(ctx));
    }
}

JSValue JSSVGTSpanElement::create(JSContext* ctx, SVGTSpanElement* impl)
{
    JSSVGTSpanElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGTSpanElementPrototype::self(ctx), JSSVGTSpanElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTSpanElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTSpanElement* impl = (SVGTSpanElement*)JS_GetOpaque(val, JSSVGTSpanElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTSpanElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGTextPositioningElement::mark(rt, val, mark_func);
}


}

#endif // ENABLE(SVG)
