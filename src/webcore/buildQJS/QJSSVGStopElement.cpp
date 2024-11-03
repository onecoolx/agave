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
#include "QJSSVGStopElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGStopElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGStopElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("className", JSSVGStopElement::getValueProperty, NULL, JSSVGStopElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("offset", JSSVGStopElement::getValueProperty, NULL, JSSVGStopElement::OffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGStopElement::getValueProperty, NULL, JSSVGStopElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGStopElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGStopElementPrototypeFunction::callAsFunction, JSSVGStopElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGStopElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGStopElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGStopElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGStopElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGStopElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStopElementAttributesFunctions, countof(JSSVGStopElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStopElementPrototypeFunctions, countof(JSSVGStopElementPrototypeFunctions));
}

static JSClassDef JSSVGStopElementClassDefine = 
{
    "SVGStopElement",
    .finalizer = JSSVGStopElement::finalizer,
    .gc_mark = JSSVGStopElement::mark,
};

JSClassID JSSVGStopElement::js_class_id = 0;

void JSSVGStopElement::init(JSContext* ctx)
{
    if (JSSVGStopElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGStopElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGStopElement::js_class_id, &JSSVGStopElementClassDefine);
        JS_SetClassProto(ctx, JSSVGStopElement::js_class_id, JSSVGStopElementPrototype::self(ctx));
    }
}

JSValue JSSVGStopElement::create(JSContext* ctx, SVGStopElement* impl)
{
    JSSVGStopElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGStopElementPrototype::self(ctx), JSSVGStopElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGStopElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGStopElement* impl = (SVGStopElement*)JS_GetOpaque(val, JSSVGStopElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGStopElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGStopElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case OffsetAttrNum: {
            SVGStopElement* imp = (SVGStopElement*)JS_GetOpaque2(ctx, this_val, JSSVGStopElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->offsetAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGStopElement* imp = (SVGStopElement*)JS_GetOpaque2(ctx, this_val, JSSVGStopElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGStopElement* imp = (SVGStopElement*)JS_GetOpaque2(ctx, this_val, JSSVGStopElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGStopElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGStopElement* imp = (SVGStopElement*)JS_GetOpaque2(ctx, this_val, JSSVGStopElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGStopElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
