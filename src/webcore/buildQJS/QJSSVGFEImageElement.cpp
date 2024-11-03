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
#include "QJSSVGFEImageElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEImageElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEImageElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGFEImageElement::getValueProperty, JSSVGFEImageElement::putValueProperty, JSSVGFEImageElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGFEImageElement::getValueProperty, JSSVGFEImageElement::putValueProperty, JSSVGFEImageElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEImageElement::getValueProperty, NULL, JSSVGFEImageElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEImageElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEImageElementPrototypeFunction::callAsFunction, JSSVGFEImageElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEImageElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEImageElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEImageElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEImageElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEImageElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEImageElementAttributesFunctions, countof(JSSVGFEImageElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEImageElementPrototypeFunctions, countof(JSSVGFEImageElementPrototypeFunctions));
}

static JSClassDef JSSVGFEImageElementClassDefine = 
{
    "SVGFEImageElement",
    .finalizer = JSSVGFEImageElement::finalizer,
    .gc_mark = JSSVGFEImageElement::mark,
};

JSClassID JSSVGFEImageElement::js_class_id = 0;

void JSSVGFEImageElement::init(JSContext* ctx)
{
    if (JSSVGFEImageElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEImageElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEImageElement::js_class_id, &JSSVGFEImageElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEImageElement::js_class_id, JSSVGFEImageElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEImageElement::create(JSContext* ctx, SVGFEImageElement* impl)
{
    JSSVGFEImageElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEImageElementPrototype::self(ctx), JSSVGFEImageElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEImageElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEImageElement* impl = (SVGFEImageElement*)JS_GetOpaque(val, JSSVGFEImageElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEImageElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEImageElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case HrefAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XmllangAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEImageElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGFEImageElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEImageElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEImageElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
