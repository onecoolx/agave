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
#include "QJSSVGTextContentElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "FloatPoint.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGPoint.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGStringList.h"
#include "SVGTextContentElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGTextContentElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGTextContentElement::getValueProperty, JSSVGTextContentElement::putValueProperty, JSSVGTextContentElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("textLength", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::TextLengthAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("lengthAdjust", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::LengthAdjustAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGTextContentElement::getValueProperty, JSSVGTextContentElement::putValueProperty, JSSVGTextContentElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGTextContentElement::getValueProperty, NULL, JSSVGTextContentElement::ClassNameAttrNum)
};

class JSSVGTextContentElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGTextContentElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGTextContentElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("LENGTHADJUST_SPACING", JSSVGTextContentElementConstructor::getValueProperty, NULL, SVGTextContentElement::LENGTHADJUST_SPACING),
    JS_CGETSET_MAGIC_DEF("LENGTHADJUST_UNKNOWN", JSSVGTextContentElementConstructor::getValueProperty, NULL, SVGTextContentElement::LENGTHADJUST_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("LENGTHADJUST_SPACINGANDGLYPHS", JSSVGTextContentElementConstructor::getValueProperty, NULL, SVGTextContentElement::LENGTHADJUST_SPACINGANDGLYPHS)
};

JSValue JSSVGTextContentElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGTextContentElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTextContentElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGTextContentElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTextContentElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextContentElementConstructorFunctions, countof(JSSVGTextContentElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGTextContentElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("LENGTHADJUST_SPACING", JSSVGTextContentElementPrototype::getValueProperty, NULL, SVGTextContentElement::LENGTHADJUST_SPACING),
    JS_CGETSET_MAGIC_DEF("LENGTHADJUST_UNKNOWN", JSSVGTextContentElementPrototype::getValueProperty, NULL, SVGTextContentElement::LENGTHADJUST_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("LENGTHADJUST_SPACINGANDGLYPHS", JSSVGTextContentElementPrototype::getValueProperty, NULL, SVGTextContentElement::LENGTHADJUST_SPACINGANDGLYPHS)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGTextContentElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getCharNumAtPosition", 1, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetCharNumAtPositionFuncNum),
    JS_CFUNC_MAGIC_DEF("getExtentOfChar", 1, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetExtentOfCharFuncNum),
    JS_CFUNC_MAGIC_DEF("getEndPositionOfChar", 1, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetEndPositionOfCharFuncNum),
    JS_CFUNC_MAGIC_DEF("getRotationOfChar", 1, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetRotationOfCharFuncNum),
    JS_CFUNC_MAGIC_DEF("getComputedTextLength", 0, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetComputedTextLengthFuncNum),
    JS_CFUNC_MAGIC_DEF("getNumberOfChars", 0, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetNumberOfCharsFuncNum),
    JS_CFUNC_MAGIC_DEF("getSubStringLength", 2, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetSubStringLengthFuncNum),
    JS_CFUNC_MAGIC_DEF("getStartPositionOfChar", 1, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetStartPositionOfCharFuncNum),
    JS_CFUNC_MAGIC_DEF("selectSubString", 2, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::SelectSubStringFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGTextContentElementPrototypeFunction::callAsFunction, JSSVGTextContentElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGTextContentElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTextContentElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGTextContentElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTextContentElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTextContentElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextContentElementAttributesFunctions, countof(JSSVGTextContentElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextContentElementPrototypeConstantsFunctions, countof(JSSVGTextContentElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextContentElementPrototypeFunctions, countof(JSSVGTextContentElementPrototypeFunctions));
}

JSValue JSSVGTextContentElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGTextContentElementClassDefine = 
{
    "SVGTextContentElement",
    .finalizer = JSSVGTextContentElement::finalizer,
    .gc_mark = JSSVGTextContentElement::mark,
};

JSClassID JSSVGTextContentElement::js_class_id = 0;

void JSSVGTextContentElement::init(JSContext* ctx)
{
    if (JSSVGTextContentElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGTextContentElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTextContentElement::js_class_id, &JSSVGTextContentElementClassDefine);
        JS_SetConstructor(ctx, JSSVGTextContentElementConstructor::self(ctx), JSSVGTextContentElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGTextContentElement::js_class_id, JSSVGTextContentElementPrototype::self(ctx));
    }
}

JSValue JSSVGTextContentElement::create(JSContext* ctx, SVGTextContentElement* impl)
{
    JSSVGTextContentElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGTextContentElementPrototype::self(ctx), JSSVGTextContentElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTextContentElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTextContentElement* impl = (SVGTextContentElement*)JS_GetOpaque(val, JSSVGTextContentElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTextContentElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGTextContentElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TextLengthAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->textLengthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case LengthAdjustAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->lengthAdjustAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGTextContentElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGTextContentElement::getConstructor(JSContext *ctx)
{
    return JSSVGTextContentElementConstructor::self(ctx);
}

JSValue JSSVGTextContentElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextContentElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGTextContentElement::GetNumberOfCharsFuncNum: {

            JSValue result = JS_NewBigUint64(ctx, imp->getNumberOfChars());
            return result;
        }
        case JSSVGTextContentElement::GetComputedTextLengthFuncNum: {

            JSValue result = JS_NewBigUint64(ctx, imp->getComputedTextLength());
            return result;
        }
        case JSSVGTextContentElement::GetSubStringLengthFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool lengthOk;
            unsigned length = valueToInt32(ctx, argv[1], lengthOk);
            if (!lengthOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewBigUint64(ctx, imp->getSubStringLength(offset, length, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetStartPositionOfCharFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->getStartPositionOfChar(offset, ec)), imp);
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetEndPositionOfCharFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->getEndPositionOfChar(offset, ec)), imp);
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetExtentOfCharFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getExtentOfChar(offset, ec)), imp);
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetRotationOfCharFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewBigUint64(ctx, imp->getRotationOfChar(offset, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetCharNumAtPositionFuncNum: {
            FloatPoint point = toSVGPoint(argv[0]);

            JSValue result = JS_NewBigUint64(ctx, imp->getCharNumAtPosition(point));
            return result;
        }
        case JSSVGTextContentElement::SelectSubStringFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool lengthOk;
            unsigned length = valueToInt32(ctx, argv[1], lengthOk);
            if (!lengthOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->selectSubString(offset, length, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGTextContentElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGTextContentElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
