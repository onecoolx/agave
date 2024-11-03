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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGFEMergeNodeElement.h"

#include "QJSSVGAnimatedString.h"
#include "SVGFEMergeNodeElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEMergeNodeElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEMergeNodeElement::getValueProperty, NULL, JSSVGFEMergeNodeElement::In1AttrNum)
};

JSValue JSSVGFEMergeNodeElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEMergeNodeElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEMergeNodeElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEMergeNodeElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEMergeNodeElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEMergeNodeElementAttributesFunctions, countof(JSSVGFEMergeNodeElementAttributesFunctions));
}

static JSClassDef JSSVGFEMergeNodeElementClassDefine = 
{
    "SVGFEMergeNodeElement",
    .finalizer = JSSVGFEMergeNodeElement::finalizer,
    .gc_mark = JSSVGFEMergeNodeElement::mark,
};

JSClassID JSSVGFEMergeNodeElement::js_class_id = 0;

void JSSVGFEMergeNodeElement::init(JSContext* ctx)
{
    if (JSSVGFEMergeNodeElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEMergeNodeElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEMergeNodeElement::js_class_id, &JSSVGFEMergeNodeElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEMergeNodeElement::js_class_id, JSSVGFEMergeNodeElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEMergeNodeElement::create(JSContext* ctx, SVGFEMergeNodeElement* impl)
{
    JSSVGFEMergeNodeElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEMergeNodeElementPrototype::self(ctx), JSSVGFEMergeNodeElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEMergeNodeElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEMergeNodeElement* impl = (SVGFEMergeNodeElement*)JS_GetOpaque(val, JSSVGFEMergeNodeElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEMergeNodeElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEMergeNodeElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEMergeNodeElement* imp = (SVGFEMergeNodeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEMergeNodeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
