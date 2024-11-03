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
#include "QJSSVGFilterElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedInteger.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFilterElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFilterElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGFilterElement::getValueProperty, JSSVGFilterElement::putValueProperty, JSSVGFilterElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("filterResY", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::FilterResYAttrNum),
    JS_CGETSET_MAGIC_DEF("filterUnits", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::FilterUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("primitiveUnits", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::PrimitiveUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("filterResX", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::FilterResXAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGFilterElement::getValueProperty, JSSVGFilterElement::putValueProperty, JSSVGFilterElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGFilterElement::getValueProperty, NULL, JSSVGFilterElement::ExternalResourcesRequiredAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFilterElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("setFilterRes", 2, JSSVGFilterElementPrototypeFunction::callAsFunction, JSSVGFilterElement::SetFilterResFuncNum),
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFilterElementPrototypeFunction::callAsFunction, JSSVGFilterElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFilterElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFilterElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFilterElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFilterElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFilterElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFilterElementAttributesFunctions, countof(JSSVGFilterElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFilterElementPrototypeFunctions, countof(JSSVGFilterElementPrototypeFunctions));
}

static JSClassDef JSSVGFilterElementClassDefine = 
{
    "SVGFilterElement",
    .finalizer = JSSVGFilterElement::finalizer,
    .gc_mark = JSSVGFilterElement::mark,
};

JSClassID JSSVGFilterElement::js_class_id = 0;

void JSSVGFilterElement::init(JSContext* ctx)
{
    if (JSSVGFilterElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFilterElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFilterElement::js_class_id, &JSSVGFilterElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFilterElement::js_class_id, JSSVGFilterElementPrototype::self(ctx));
    }
}

JSValue JSSVGFilterElement::create(JSContext* ctx, SVGFilterElement* impl)
{
    JSSVGFilterElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFilterElementPrototype::self(ctx), JSSVGFilterElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFilterElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFilterElement* impl = (SVGFilterElement*)JS_GetOpaque(val, JSSVGFilterElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFilterElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFilterElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FilterUnitsAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->filterUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PrimitiveUnitsAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->primitiveUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case FilterResXAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedInteger> obj = imp->filterResXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case FilterResYAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedInteger> obj = imp->filterResYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XmllangAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFilterElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGFilterElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque2(ctx, this_val, JSSVGFilterElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFilterElement::SetFilterResFuncNum: {
            bool filterResXOk;
            unsigned filterResX = valueToInt32(ctx, argv[0], filterResXOk);
            if (!filterResXOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool filterResYOk;
            unsigned filterResY = valueToInt32(ctx, argv[1], filterResYOk);
            if (!filterResYOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setFilterRes(filterResX, filterResY);
            return JS_UNDEFINED;
        }
        case JSSVGFilterElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
