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
#include "QJSSVGTRefElement.h"

#include "QJSSVGAnimatedString.h"
#include "SVGTRefElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGTRefElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("href", JSSVGTRefElement::getValueProperty, NULL, JSSVGTRefElement::HrefAttrNum)
};

JSValue JSSVGTRefElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTRefElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGTextPositioningElementPrototype::self(ctx));
        JSSVGTRefElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTRefElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTRefElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTRefElementAttributesFunctions, countof(JSSVGTRefElementAttributesFunctions));
}

static JSClassDef JSSVGTRefElementClassDefine = 
{
    "SVGTRefElement",
    .finalizer = JSSVGTRefElement::finalizer,
    .gc_mark = JSSVGTRefElement::mark,
};

JSClassID JSSVGTRefElement::js_class_id = 0;

void JSSVGTRefElement::init(JSContext* ctx)
{
    if (JSSVGTRefElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGTRefElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTRefElement::js_class_id, &JSSVGTRefElementClassDefine);
        JS_SetClassProto(ctx, JSSVGTRefElement::js_class_id, JSSVGTRefElementPrototype::self(ctx));
    }
}

JSValue JSSVGTRefElement::create(JSContext* ctx, SVGTRefElement* impl)
{
    JSSVGTRefElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGTRefElementPrototype::self(ctx), JSSVGTRefElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTRefElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTRefElement* impl = (SVGTRefElement*)JS_GetOpaque(val, JSSVGTRefElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTRefElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGTextPositioningElement::mark(rt, val, mark_func);
}

JSValue JSSVGTRefElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case HrefAttrNum: {
            SVGTRefElement* imp = (SVGTRefElement*)JS_GetOpaque2(ctx, this_val, JSSVGTRefElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
