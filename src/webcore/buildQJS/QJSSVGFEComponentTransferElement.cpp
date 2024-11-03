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
#include "QJSSVGFEComponentTransferElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEComponentTransferElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEComponentTransferElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEComponentTransferElement::getValueProperty, NULL, JSSVGFEComponentTransferElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEComponentTransferElement::getValueProperty, NULL, JSSVGFEComponentTransferElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEComponentTransferElement::getValueProperty, NULL, JSSVGFEComponentTransferElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEComponentTransferElement::getValueProperty, NULL, JSSVGFEComponentTransferElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEComponentTransferElement::getValueProperty, NULL, JSSVGFEComponentTransferElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEComponentTransferElement::getValueProperty, NULL, JSSVGFEComponentTransferElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEComponentTransferElement::getValueProperty, NULL, JSSVGFEComponentTransferElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEComponentTransferElement::getValueProperty, NULL, JSSVGFEComponentTransferElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEComponentTransferElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEComponentTransferElementPrototypeFunction::callAsFunction, JSSVGFEComponentTransferElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEComponentTransferElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEComponentTransferElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEComponentTransferElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEComponentTransferElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEComponentTransferElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEComponentTransferElementAttributesFunctions, countof(JSSVGFEComponentTransferElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEComponentTransferElementPrototypeFunctions, countof(JSSVGFEComponentTransferElementPrototypeFunctions));
}

static JSClassDef JSSVGFEComponentTransferElementClassDefine = 
{
    "SVGFEComponentTransferElement",
    .finalizer = JSSVGFEComponentTransferElement::finalizer,
    .gc_mark = JSSVGFEComponentTransferElement::mark,
};

JSClassID JSSVGFEComponentTransferElement::js_class_id = 0;

void JSSVGFEComponentTransferElement::init(JSContext* ctx)
{
    if (JSSVGFEComponentTransferElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEComponentTransferElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEComponentTransferElement::js_class_id, &JSSVGFEComponentTransferElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEComponentTransferElement::js_class_id, JSSVGFEComponentTransferElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEComponentTransferElement::create(JSContext* ctx, SVGFEComponentTransferElement* impl)
{
    JSSVGFEComponentTransferElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEComponentTransferElementPrototype::self(ctx), JSSVGFEComponentTransferElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEComponentTransferElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEComponentTransferElement* impl = (SVGFEComponentTransferElement*)JS_GetOpaque(val, JSSVGFEComponentTransferElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEComponentTransferElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEComponentTransferElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEComponentTransferElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEComponentTransferElement* imp = (SVGFEComponentTransferElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEComponentTransferElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEComponentTransferElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
