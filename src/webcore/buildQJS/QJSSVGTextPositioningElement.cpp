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
#include "QJSSVGTextPositioningElement.h"

#include "QJSSVGAnimatedLengthList.h"
#include "QJSSVGAnimatedNumberList.h"
#include "SVGTextPositioningElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGTextPositioningElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGTextPositioningElement::getValueProperty, NULL, JSSVGTextPositioningElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGTextPositioningElement::getValueProperty, NULL, JSSVGTextPositioningElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("dx", JSSVGTextPositioningElement::getValueProperty, NULL, JSSVGTextPositioningElement::DxAttrNum),
    JS_CGETSET_MAGIC_DEF("dy", JSSVGTextPositioningElement::getValueProperty, NULL, JSSVGTextPositioningElement::DyAttrNum),
    JS_CGETSET_MAGIC_DEF("rotate", JSSVGTextPositioningElement::getValueProperty, NULL, JSSVGTextPositioningElement::RotateAttrNum)
};

JSValue JSSVGTextPositioningElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTextPositioningElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGTextContentElementPrototype::self(ctx));
        JSSVGTextPositioningElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTextPositioningElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTextPositioningElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextPositioningElementAttributesFunctions, countof(JSSVGTextPositioningElementAttributesFunctions));
}

static JSClassDef JSSVGTextPositioningElementClassDefine = 
{
    "SVGTextPositioningElement",
    .finalizer = JSSVGTextPositioningElement::finalizer,
    .gc_mark = JSSVGTextPositioningElement::mark,
};

JSClassID JSSVGTextPositioningElement::js_class_id = 0;

void JSSVGTextPositioningElement::init(JSContext* ctx)
{
    if (JSSVGTextPositioningElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGTextPositioningElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTextPositioningElement::js_class_id, &JSSVGTextPositioningElementClassDefine);
        JS_SetClassProto(ctx, JSSVGTextPositioningElement::js_class_id, JSSVGTextPositioningElementPrototype::self(ctx));
    }
}

JSValue JSSVGTextPositioningElement::create(JSContext* ctx, SVGTextPositioningElement* impl)
{
    JSSVGTextPositioningElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGTextPositioningElementPrototype::self(ctx), JSSVGTextPositioningElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTextPositioningElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTextPositioningElement* impl = (SVGTextPositioningElement*)JS_GetOpaque(val, JSSVGTextPositioningElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTextPositioningElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGTextContentElement::mark(rt, val, mark_func);
}

JSValue JSSVGTextPositioningElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedLengthList> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedLengthList> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case DxAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedLengthList> obj = imp->dxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case DyAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedLengthList> obj = imp->dyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RotateAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedNumberList> obj = imp->rotateAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
