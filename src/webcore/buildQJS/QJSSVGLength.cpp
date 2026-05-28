/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#include <string.h>


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

static JSCFunctionListEntry JSSVGLengthAttributesFunctions[5];
static bool JSSVGLengthAttributesFunctions_initialized = false;

static void init_JSSVGLengthAttributesFunctions()
{
    if (JSSVGLengthAttributesFunctions_initialized) return;
    JSSVGLengthAttributesFunctions_initialized = true;
    memset(JSSVGLengthAttributesFunctions, 0, sizeof(JSSVGLengthAttributesFunctions));
    JSSVGLengthAttributesFunctions[0].name = "unitType";
    JSSVGLengthAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLengthAttributesFunctions[0].magic = JSSVGLength::UnitTypeAttrNum;
    JSSVGLengthAttributesFunctions[0].u.getset.get.getter_magic = JSSVGLength::getValueProperty;
    JSSVGLengthAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGLengthAttributesFunctions[1].name = "value";
    JSSVGLengthAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLengthAttributesFunctions[1].magic = JSSVGLength::ValueAttrNum;
    JSSVGLengthAttributesFunctions[1].u.getset.get.getter_magic = JSSVGLength::getValueProperty;
    JSSVGLengthAttributesFunctions[1].u.getset.set.setter_magic = JSSVGLength::putValueProperty;
    JSSVGLengthAttributesFunctions[2].name = "valueInSpecifiedUnits";
    JSSVGLengthAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLengthAttributesFunctions[2].magic = JSSVGLength::ValueInSpecifiedUnitsAttrNum;
    JSSVGLengthAttributesFunctions[2].u.getset.get.getter_magic = JSSVGLength::getValueProperty;
    JSSVGLengthAttributesFunctions[2].u.getset.set.setter_magic = JSSVGLength::putValueProperty;
    JSSVGLengthAttributesFunctions[3].name = "valueAsString";
    JSSVGLengthAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLengthAttributesFunctions[3].magic = JSSVGLength::ValueAsStringAttrNum;
    JSSVGLengthAttributesFunctions[3].u.getset.get.getter_magic = JSSVGLength::getValueProperty;
    JSSVGLengthAttributesFunctions[3].u.getset.set.setter_magic = JSSVGLength::putValueProperty;
    JSSVGLengthAttributesFunctions[4].name = "constructor";
    JSSVGLengthAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLengthAttributesFunctions[4].magic = JSSVGLength::ConstructorAttrNum;
    JSSVGLengthAttributesFunctions[4].u.getset.get.getter_magic = JSSVGLength::getValueProperty;
    JSSVGLengthAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

class JSSVGLengthConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGLengthConstructorFunctions[11];
static bool JSSVGLengthConstructorFunctions_initialized = false;

static void init_JSSVGLengthConstructorFunctions()
{
    if (JSSVGLengthConstructorFunctions_initialized) return;
    JSSVGLengthConstructorFunctions_initialized = true;
    memset(JSSVGLengthConstructorFunctions, 0, sizeof(JSSVGLengthConstructorFunctions));
    JSSVGLengthConstructorFunctions[0].name = "SVG_LENGTHTYPE_UNKNOWN";
    JSSVGLengthConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[0].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_UNKNOWN;
    JSSVGLengthConstructorFunctions[1].name = "SVG_LENGTHTYPE_NUMBER";
    JSSVGLengthConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[1].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_NUMBER;
    JSSVGLengthConstructorFunctions[2].name = "SVG_LENGTHTYPE_PERCENTAGE";
    JSSVGLengthConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[2].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_PERCENTAGE;
    JSSVGLengthConstructorFunctions[3].name = "SVG_LENGTHTYPE_EMS";
    JSSVGLengthConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[3].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_EMS;
    JSSVGLengthConstructorFunctions[4].name = "SVG_LENGTHTYPE_EXS";
    JSSVGLengthConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[4].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_EXS;
    JSSVGLengthConstructorFunctions[5].name = "SVG_LENGTHTYPE_PX";
    JSSVGLengthConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[5].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_PX;
    JSSVGLengthConstructorFunctions[6].name = "SVG_LENGTHTYPE_CM";
    JSSVGLengthConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[6].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_CM;
    JSSVGLengthConstructorFunctions[7].name = "SVG_LENGTHTYPE_MM";
    JSSVGLengthConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[7].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_MM;
    JSSVGLengthConstructorFunctions[8].name = "SVG_LENGTHTYPE_IN";
    JSSVGLengthConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[8].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_IN;
    JSSVGLengthConstructorFunctions[9].name = "SVG_LENGTHTYPE_PT";
    JSSVGLengthConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[9].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_PT;
    JSSVGLengthConstructorFunctions[10].name = "SVG_LENGTHTYPE_PC";
    JSSVGLengthConstructorFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthConstructorFunctions[10].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthConstructorFunctions[10].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_PC;
}

JSValue JSSVGLengthConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGLength.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGLengthConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGLength.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGLengthConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGLengthConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthConstructorFunctions, countof(JSSVGLengthConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGLengthPrototypeConstantsFunctions[11];
static bool JSSVGLengthPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGLengthPrototypeConstantsFunctions()
{
    if (JSSVGLengthPrototypeConstantsFunctions_initialized) return;
    JSSVGLengthPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGLengthPrototypeConstantsFunctions, 0, sizeof(JSSVGLengthPrototypeConstantsFunctions));
    JSSVGLengthPrototypeConstantsFunctions[0].name = "SVG_LENGTHTYPE_UNKNOWN";
    JSSVGLengthPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_UNKNOWN;
    JSSVGLengthPrototypeConstantsFunctions[1].name = "SVG_LENGTHTYPE_NUMBER";
    JSSVGLengthPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_NUMBER;
    JSSVGLengthPrototypeConstantsFunctions[2].name = "SVG_LENGTHTYPE_PERCENTAGE";
    JSSVGLengthPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_PERCENTAGE;
    JSSVGLengthPrototypeConstantsFunctions[3].name = "SVG_LENGTHTYPE_EMS";
    JSSVGLengthPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_EMS;
    JSSVGLengthPrototypeConstantsFunctions[4].name = "SVG_LENGTHTYPE_EXS";
    JSSVGLengthPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[4].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_EXS;
    JSSVGLengthPrototypeConstantsFunctions[5].name = "SVG_LENGTHTYPE_PX";
    JSSVGLengthPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[5].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_PX;
    JSSVGLengthPrototypeConstantsFunctions[6].name = "SVG_LENGTHTYPE_CM";
    JSSVGLengthPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[6].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_CM;
    JSSVGLengthPrototypeConstantsFunctions[7].name = "SVG_LENGTHTYPE_MM";
    JSSVGLengthPrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[7].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_MM;
    JSSVGLengthPrototypeConstantsFunctions[8].name = "SVG_LENGTHTYPE_IN";
    JSSVGLengthPrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[8].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_IN;
    JSSVGLengthPrototypeConstantsFunctions[9].name = "SVG_LENGTHTYPE_PT";
    JSSVGLengthPrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[9].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_PT;
    JSSVGLengthPrototypeConstantsFunctions[10].name = "SVG_LENGTHTYPE_PC";
    JSSVGLengthPrototypeConstantsFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeConstantsFunctions[10].def_type = JS_DEF_PROP_INT32;
    JSSVGLengthPrototypeConstantsFunctions[10].u.i32 = (int32_t)SVGLength::SVG_LENGTHTYPE_PC;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGLengthPrototypeFunctions[2];
static bool JSSVGLengthPrototypeFunctions_initialized = false;

static void init_JSSVGLengthPrototypeFunctions()
{
    if (JSSVGLengthPrototypeFunctions_initialized) return;
    JSSVGLengthPrototypeFunctions_initialized = true;
    memset(JSSVGLengthPrototypeFunctions, 0, sizeof(JSSVGLengthPrototypeFunctions));
    JSSVGLengthPrototypeFunctions[0].name = "newValueSpecifiedUnits";
    JSSVGLengthPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGLengthPrototypeFunctions[0].magic = JSSVGLength::NewValueSpecifiedUnitsFuncNum;
    JSSVGLengthPrototypeFunctions[0].u.func.length = 2;
    JSSVGLengthPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGLengthPrototypeFunction::callAsFunction;
    JSSVGLengthPrototypeFunctions[1].name = "convertToSpecifiedUnits";
    JSSVGLengthPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGLengthPrototypeFunctions[1].magic = JSSVGLength::ConvertToSpecifiedUnitsFuncNum;
    JSSVGLengthPrototypeFunctions[1].u.func.length = 1;
    JSSVGLengthPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGLengthPrototypeFunction::callAsFunction;
}

JSValue JSSVGLengthPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGLength.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGLengthPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGLength.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGLengthPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGLengthAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthAttributesFunctions, countof(JSSVGLengthAttributesFunctions));
    init_JSSVGLengthPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthPrototypeConstantsFunctions, countof(JSSVGLengthPrototypeConstantsFunctions));
    init_JSSVGLengthPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthPrototypeFunctions, countof(JSSVGLengthPrototypeFunctions));
}

static JSClassDef JSSVGLengthClassDefine;
static bool JSSVGLengthClassDefine_initialized = false;

static void init_JSSVGLengthClassDefine()
{
    if (JSSVGLengthClassDefine_initialized) return;
    JSSVGLengthClassDefine_initialized = true;
    memset(&JSSVGLengthClassDefine, 0, sizeof(JSSVGLengthClassDefine));
    JSSVGLengthClassDefine.class_name = "SVGLength";
    JSSVGLengthClassDefine.finalizer = JSSVGLength::finalizer;
    JSSVGLengthClassDefine.gc_mark = JSSVGLength::mark;
}

JSClassID JSSVGLength::js_class_id = 0;

void JSSVGLength::init(JSContext* ctx)
{
    if (JSSVGLength::js_class_id == 0) {
        init_JSSVGLengthClassDefine();
        JS_NewClassID(&JSSVGLength::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGLength::js_class_id, &JSSVGLengthClassDefine);
        JS_SetConstructor(ctx, JSSVGLengthConstructor::self(ctx), JSSVGLengthPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGLength::js_class_id, JSSVGLengthPrototype::self(ctx));
    }
}

JSValue JSSVGLength::create(JSContext* ctx, JSSVGPODTypeWrapper<SVGLength>* impl, SVGElement* context)
{
    JSSVGLength::init(ctx);
    JSValue _proto = JSSVGLengthPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGLength::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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

            return JS_NewInt32(ctx, imp.unitType());
        }
        case ValueAttrNum: {
            SVGLength imp(*impl());

            return JS_NewInt32(ctx, imp.value());
        }
        case ValueInSpecifiedUnitsAttrNum: {
            SVGLength imp(*impl());

            return JS_NewInt32(ctx, imp.valueInSpecifiedUnits());
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
    SVGLength* imp = (SVGLength*)JS_GetOpaque(this_val, JSSVGLength::js_class_id);
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
