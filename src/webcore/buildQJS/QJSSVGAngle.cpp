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
#include "QJSSVGAngle.h"

#include "SVGAngle.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAngleAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("value", JSSVGAngle::getValueProperty, JSSVGAngle::putValueProperty, JSSVGAngle::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("valueInSpecifiedUnits", JSSVGAngle::getValueProperty, JSSVGAngle::putValueProperty, JSSVGAngle::ValueInSpecifiedUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("unitType", JSSVGAngle::getValueProperty, NULL, JSSVGAngle::UnitTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("valueAsString", JSSVGAngle::getValueProperty, JSSVGAngle::putValueProperty, JSSVGAngle::ValueAsStringAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGAngle::getValueProperty, NULL, JSSVGAngle::ConstructorAttrNum)
};

class JSSVGAngleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGAngleConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGAngleConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_UNSPECIFIED", JSSVGAngleConstructor::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_UNSPECIFIED),
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_DEG", JSSVGAngleConstructor::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_DEG),
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_UNKNOWN", JSSVGAngleConstructor::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_RAD", JSSVGAngleConstructor::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_RAD),
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_GRAD", JSSVGAngleConstructor::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_GRAD)
};

JSValue JSSVGAngleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGAngle.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAngleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGAngle.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAngleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAngleConstructorFunctions, countof(JSSVGAngleConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGAnglePrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_UNSPECIFIED", JSSVGAnglePrototype::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_UNSPECIFIED),
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_DEG", JSSVGAnglePrototype::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_DEG),
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_UNKNOWN", JSSVGAnglePrototype::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_RAD", JSSVGAnglePrototype::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_RAD),
    JS_CGETSET_MAGIC_DEF("SVG_ANGLETYPE_GRAD", JSSVGAnglePrototype::getValueProperty, NULL, SVGAngle::SVG_ANGLETYPE_GRAD)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGAnglePrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("newValueSpecifiedUnits", 2, JSSVGAnglePrototypeFunction::callAsFunction, JSSVGAngle::NewValueSpecifiedUnitsFuncNum),
    JS_CFUNC_MAGIC_DEF("convertToSpecifiedUnits", 1, JSSVGAnglePrototypeFunction::callAsFunction, JSSVGAngle::ConvertToSpecifiedUnitsFuncNum)
};

JSValue JSSVGAnglePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAngle.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnglePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAngle.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnglePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAngleAttributesFunctions, countof(JSSVGAngleAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnglePrototypeConstantsFunctions, countof(JSSVGAnglePrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnglePrototypeFunctions, countof(JSSVGAnglePrototypeFunctions));
}

JSValue JSSVGAnglePrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGAngleClassDefine = 
{
    "SVGAngle",
    .finalizer = JSSVGAngle::finalizer,
    .gc_mark = JSSVGAngle::mark,
};

JSClassID JSSVGAngle::js_class_id = 0;

void JSSVGAngle::init(JSContext* ctx)
{
    if (JSSVGAngle::js_class_id == 0) {
        JS_NewClassID(&JSSVGAngle::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAngle::js_class_id, &JSSVGAngleClassDefine);
        JS_SetConstructor(ctx, JSSVGAngleConstructor::self(ctx), JSSVGAnglePrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGAngle::js_class_id, JSSVGAnglePrototype::self(ctx));
    }
}

JSValue JSSVGAngle::create(JSContext* ctx, SVGAngle* impl, SVGElement* context)
{
    JSSVGAngle::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnglePrototype::self(ctx), JSSVGAngle::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAngle::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAngle* impl = (SVGAngle*)JS_GetOpaque(val, JSSVGAngle::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAngle::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAngle::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case UnitTypeAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque2(ctx, this_val, JSSVGAngle::js_class_id);
            return JS_NewBigUint64(ctx, imp->unitType());
        }
        case ValueAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque2(ctx, this_val, JSSVGAngle::js_class_id);
            return JS_NewBigUint64(ctx, imp->value());
        }
        case ValueInSpecifiedUnitsAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque2(ctx, this_val, JSSVGAngle::js_class_id);
            return JS_NewBigUint64(ctx, imp->valueInSpecifiedUnits());
        }
        case ValueAsStringAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque2(ctx, this_val, JSSVGAngle::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->valueAsString()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGAngle::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ValueAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque2(ctx, this_val, JSSVGAngle::js_class_id);
            imp->setValue(valueToFloat(ctx, value));
            break;
        }
        case ValueInSpecifiedUnitsAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque2(ctx, this_val, JSSVGAngle::js_class_id);
            imp->setValueInSpecifiedUnits(valueToFloat(ctx, value));
            break;
        }
        case ValueAsStringAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque2(ctx, this_val, JSSVGAngle::js_class_id);
            imp->setValueAsString(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue JSSVGAngle::getConstructor(JSContext *ctx)
{
    return JSSVGAngleConstructor::self(ctx);
}

JSValue JSSVGAnglePrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGAngle* imp = (SVGAngle*)JS_GetOpaque2(ctx, this_val, JSSVGAngle::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGAngle::NewValueSpecifiedUnitsFuncNum: {
            unsigned short unitType = valueToInt32(ctx, argv[0]);
            float valueInSpecifiedUnits = valueToFloat(ctx, argv[1]);
            imp->newValueSpecifiedUnits(unitType, valueInSpecifiedUnits);
            return JS_UNDEFINED;
        }
        case JSSVGAngle::ConvertToSpecifiedUnitsFuncNum: {
            unsigned short unitType = valueToInt32(ctx, argv[0]);
            imp->convertToSpecifiedUnits(unitType);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAngle* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAngle, JSSVGAngle>(ctx, obj, context);
}

SVGAngle* toSVGAngle(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAngle* impl = (SVGAngle*)JS_GetOpaque(val, JSSVGAngle::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
