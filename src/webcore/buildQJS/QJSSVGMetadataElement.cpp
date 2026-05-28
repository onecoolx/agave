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
#include "QJSSVGMetadataElement.h"

#include "SVGMetadataElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
JSValue JSSVGMetadataElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGMetadataElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGMetadataElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGMetadataElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGMetadataElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
}

static JSClassDef JSSVGMetadataElementClassDefine;
static bool JSSVGMetadataElementClassDefine_initialized = false;

static void init_JSSVGMetadataElementClassDefine()
{
    if (JSSVGMetadataElementClassDefine_initialized) return;
    JSSVGMetadataElementClassDefine_initialized = true;
    memset(&JSSVGMetadataElementClassDefine, 0, sizeof(JSSVGMetadataElementClassDefine));
    JSSVGMetadataElementClassDefine.class_name = "SVGMetadataElement";
    JSSVGMetadataElementClassDefine.finalizer = JSSVGMetadataElement::finalizer;
    JSSVGMetadataElementClassDefine.gc_mark = JSSVGMetadataElement::mark;
}

JSClassID JSSVGMetadataElement::js_class_id = 0;

void JSSVGMetadataElement::init(JSContext* ctx)
{
    if (JSSVGMetadataElement::js_class_id == 0) {
        init_JSSVGMetadataElementClassDefine();
        JS_NewClassID(&JSSVGMetadataElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGMetadataElement::js_class_id, &JSSVGMetadataElementClassDefine);
        JS_SetClassProto(ctx, JSSVGMetadataElement::js_class_id, JSSVGMetadataElementPrototype::self(ctx));
    }
}

JSValue JSSVGMetadataElement::create(JSContext* ctx, SVGMetadataElement* impl)
{
    JSSVGMetadataElement::init(ctx);
    JSValue _proto = JSSVGMetadataElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGMetadataElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGMetadataElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGMetadataElement* impl = (SVGMetadataElement*)JS_GetOpaque(val, JSSVGMetadataElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGMetadataElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}


}

#endif // ENABLE(SVG)
