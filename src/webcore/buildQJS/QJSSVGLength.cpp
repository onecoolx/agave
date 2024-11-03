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
#include "QJSSVGLength.h"

#include "SVGLength.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGLengthAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("value", JSSVGLength::getValueProperty, JSSVGLength::putValueProperty, JSSVGLength::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("valueInSpecifiedUnits", JSSVGLength::getValueProperty, JSSVGLength::putValueProperty, JSSVGLength::ValueInSpecifiedUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("unitType", JSSVGLength::getValueProperty, NULL, JSSVGLength::UnitTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("valueAsString", JSSVGLength::getValueProperty, JSSVGLength::putValueProperty, JSSVGLength::ValueAsStringAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGLength::getValueProperty, NULL, JSSVGLength::ConstructorAttrNum)
};

class JSSVGLengthConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGLengthConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGLengthConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_CM", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_CM),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_PT", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_PT),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_EXS", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_EXS),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_PERCENTAGE", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_PERCENTAGE),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_EMS", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_EMS),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_UNKNOWN", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_MM", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_MM),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_NUMBER", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_NUMBER),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_PX", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_PX),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_IN", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_IN),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_PC", JSSVGLengthConstructor::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_PC)
};

JSValue JSSVGLengthConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGLength.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGLengthConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGLength.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGLengthConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthConstructorFunctions, countof(JSSVGLengthConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGLengthPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_CM", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_CM),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_PT", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_PT),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_EXS", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_EXS),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_PERCENTAGE", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_PERCENTAGE),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_EMS", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_EMS),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_UNKNOWN", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_MM", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_MM),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_NUMBER", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_NUMBER),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_PX", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_PX),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_IN", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_IN),
    JS_CGETSET_MAGIC_DEF("SVG_LENGTHTYPE_PC", JSSVGLengthPrototype::getValueProperty, NULL, SVGLength::SVG_LENGTHTYPE_PC)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGLengthPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("newValueSpecifiedUnits", 2, JSSVGLengthPrototypeFunction::callAsFunction, JSSVGLength::NewValueSpecifiedUnitsFuncNum),
    JS_CFUNC_MAGIC_DEF("convertToSpecifiedUnits", 1, JSSVGLengthPrototypeFunction::callAsFunction, JSSVGLength::ConvertToSpecifiedUnitsFuncNum)
};

JSValue JSSVGLengthPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGLength.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGLengthPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGLength.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGLengthPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthAttributesFunctions, countof(JSSVGLengthAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthPrototypeConstantsFunctions, countof(JSSVGLengthPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthPrototypeFunctions, countof(JSSVGLengthPrototypeFunctions));
}

JSValue JSSVGLengthPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGLengthClassDefine = 
{
    "SVGLength",
    .finalizer = JSSVGLength::finalizer,
    .gc_mark = JSSVGLength::mark,
};

JSClassID JSSVGLength::js_class_id = 0;

void JSSVGLength::init(JSContext* ctx)
{
    if (JSSVGLength::js_class_id == 0) {
        JS_NewClassID(&JSSVGLength::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGLength::js_class_id, &JSSVGLengthClassDefine);
        JS_SetConstructor(ctx, JSSVGLengthConstructor::self(ctx), JSSVGLengthPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGLength::js_class_id, JSSVGLengthPrototype::self(ctx));
    }
}

JSValue JSSVGLength::create(JSContext* ctx, JSSVGPODTypeWrapper<SVGLength>* impl, SVGElement* context)
{
    JSSVGLength::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGLengthPrototype::self(ctx), JSSVGLength::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGLength::finalizer(JSRuntime* rt, JSValue val)
{
    SVGLength* impl = (SVGLength*)JS_GetOpaque(val, JSSVGLength::js_class_id);
    JSSVGPODTypeWrapperCache<SVGLength, SVGAnimatedLength>::forgetWrapper(impl);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGLength::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGLength::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case UnitTypeAttrNum: {
            SVGLength imp(*impl());

            return JS_NewBigUint64(ctx, imp.unitType());
        }
        case ValueAttrNum: {
            SVGLength imp(*impl());

            return JS_NewBigUint64(ctx, imp.value());
        }
        case ValueInSpecifiedUnitsAttrNum: {
            SVGLength imp(*impl());

            return JS_NewBigUint64(ctx, imp.valueInSpecifiedUnits());
        }
        case ValueAsStringAttrNum: {
            SVGLength imp(*impl());

            return JS_NewString(ctx, ((const String&)imp.valueAsString()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGLength::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ValueAttrNum: {
            SVGLength imp(*impl());

            imp.setValue(valueToFloat(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case ValueInSpecifiedUnitsAttrNum: {
            SVGLength imp(*impl());

            imp.setValueInSpecifiedUnits(valueToFloat(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case ValueAsStringAttrNum: {
            SVGLength imp(*impl());

            imp.setValueAsString(valueToStringWithNullCheck(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue JSSVGLength::getConstructor(JSContext *ctx)
{
    return JSSVGLengthConstructor::self(ctx);
}

JSValue JSSVGLengthPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGLength* imp = (SVGLength*)JS_GetOpaque2(ctx, this_val, JSSVGLength::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    JSSVGPODTypeWrapper<SVGLength>* wrapper = castedThisObj->impl();
    SVGLength imp(*wrapper);

    switch (token) {
        case JSSVGLength::NewValueSpecifiedUnitsFuncNum: {
            unsigned short unitType = valueToInt32(ctx, argv[0]);
            float valueInSpecifiedUnits = valueToFloat(ctx, argv[1]);
            imp.newValueSpecifiedUnits(unitType, valueInSpecifiedUnits);
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return JS_UNDEFINED;
        }
        case JSSVGLength::ConvertToSpecifiedUnitsFuncNum: {
            unsigned short unitType = valueToInt32(ctx, argv[0]);
            imp.convertToSpecifiedUnits(unitType);
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<SVGLength>* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<JSSVGPODTypeWrapper<SVGLength>, JSSVGLength>(ctx, obj, context);
}

SVGLength toSVGLength(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGLength* impl = (SVGLength*)JS_GetOpaque(val, JSSVGLength::js_class_id);
        return (SVGLength) *impl;
    } else {
        return SVGLength();
}

}

#endif // ENABLE(SVG)
