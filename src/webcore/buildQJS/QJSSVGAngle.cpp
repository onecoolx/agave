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
#include "QJSSVGAngle.h"

#include "SVGAngle.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGAngleAttributesFunctions[5];
static bool JSSVGAngleAttributesFunctions_initialized = false;

static void init_JSSVGAngleAttributesFunctions()
{
    if (JSSVGAngleAttributesFunctions_initialized) return;
    JSSVGAngleAttributesFunctions_initialized = true;
    memset(JSSVGAngleAttributesFunctions, 0, sizeof(JSSVGAngleAttributesFunctions));
    JSSVGAngleAttributesFunctions[0].name = "unitType";
    JSSVGAngleAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAngleAttributesFunctions[0].magic = JSSVGAngle::UnitTypeAttrNum;
    JSSVGAngleAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAngle::getValueProperty;
    JSSVGAngleAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGAngleAttributesFunctions[1].name = "value";
    JSSVGAngleAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAngleAttributesFunctions[1].magic = JSSVGAngle::ValueAttrNum;
    JSSVGAngleAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAngle::getValueProperty;
    JSSVGAngleAttributesFunctions[1].u.getset.set.setter_magic = JSSVGAngle::putValueProperty;
    JSSVGAngleAttributesFunctions[2].name = "valueInSpecifiedUnits";
    JSSVGAngleAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAngleAttributesFunctions[2].magic = JSSVGAngle::ValueInSpecifiedUnitsAttrNum;
    JSSVGAngleAttributesFunctions[2].u.getset.get.getter_magic = JSSVGAngle::getValueProperty;
    JSSVGAngleAttributesFunctions[2].u.getset.set.setter_magic = JSSVGAngle::putValueProperty;
    JSSVGAngleAttributesFunctions[3].name = "valueAsString";
    JSSVGAngleAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAngleAttributesFunctions[3].magic = JSSVGAngle::ValueAsStringAttrNum;
    JSSVGAngleAttributesFunctions[3].u.getset.get.getter_magic = JSSVGAngle::getValueProperty;
    JSSVGAngleAttributesFunctions[3].u.getset.set.setter_magic = JSSVGAngle::putValueProperty;
    JSSVGAngleAttributesFunctions[4].name = "constructor";
    JSSVGAngleAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAngleAttributesFunctions[4].magic = JSSVGAngle::ConstructorAttrNum;
    JSSVGAngleAttributesFunctions[4].u.getset.get.getter_magic = JSSVGAngle::getValueProperty;
    JSSVGAngleAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

class JSSVGAngleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGAngleConstructorFunctions[5];
static bool JSSVGAngleConstructorFunctions_initialized = false;

static void init_JSSVGAngleConstructorFunctions()
{
    if (JSSVGAngleConstructorFunctions_initialized) return;
    JSSVGAngleConstructorFunctions_initialized = true;
    memset(JSSVGAngleConstructorFunctions, 0, sizeof(JSSVGAngleConstructorFunctions));
    JSSVGAngleConstructorFunctions[0].name = "SVG_ANGLETYPE_UNKNOWN";
    JSSVGAngleConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGAngleConstructorFunctions[0].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_UNKNOWN;
    JSSVGAngleConstructorFunctions[1].name = "SVG_ANGLETYPE_UNSPECIFIED";
    JSSVGAngleConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGAngleConstructorFunctions[1].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_UNSPECIFIED;
    JSSVGAngleConstructorFunctions[2].name = "SVG_ANGLETYPE_DEG";
    JSSVGAngleConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGAngleConstructorFunctions[2].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_DEG;
    JSSVGAngleConstructorFunctions[3].name = "SVG_ANGLETYPE_RAD";
    JSSVGAngleConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGAngleConstructorFunctions[3].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_RAD;
    JSSVGAngleConstructorFunctions[4].name = "SVG_ANGLETYPE_GRAD";
    JSSVGAngleConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAngleConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGAngleConstructorFunctions[4].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_GRAD;
}

JSValue JSSVGAngleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGAngle.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAngleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGAngle.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAngleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAngleConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAngleConstructorFunctions, countof(JSSVGAngleConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGAnglePrototypeConstantsFunctions[5];
static bool JSSVGAnglePrototypeConstantsFunctions_initialized = false;

static void init_JSSVGAnglePrototypeConstantsFunctions()
{
    if (JSSVGAnglePrototypeConstantsFunctions_initialized) return;
    JSSVGAnglePrototypeConstantsFunctions_initialized = true;
    memset(JSSVGAnglePrototypeConstantsFunctions, 0, sizeof(JSSVGAnglePrototypeConstantsFunctions));
    JSSVGAnglePrototypeConstantsFunctions[0].name = "SVG_ANGLETYPE_UNKNOWN";
    JSSVGAnglePrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnglePrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGAnglePrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_UNKNOWN;
    JSSVGAnglePrototypeConstantsFunctions[1].name = "SVG_ANGLETYPE_UNSPECIFIED";
    JSSVGAnglePrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnglePrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGAnglePrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_UNSPECIFIED;
    JSSVGAnglePrototypeConstantsFunctions[2].name = "SVG_ANGLETYPE_DEG";
    JSSVGAnglePrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnglePrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGAnglePrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_DEG;
    JSSVGAnglePrototypeConstantsFunctions[3].name = "SVG_ANGLETYPE_RAD";
    JSSVGAnglePrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnglePrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGAnglePrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_RAD;
    JSSVGAnglePrototypeConstantsFunctions[4].name = "SVG_ANGLETYPE_GRAD";
    JSSVGAnglePrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnglePrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGAnglePrototypeConstantsFunctions[4].u.i32 = (int32_t)SVGAngle::SVG_ANGLETYPE_GRAD;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGAnglePrototypeFunctions[2];
static bool JSSVGAnglePrototypeFunctions_initialized = false;

static void init_JSSVGAnglePrototypeFunctions()
{
    if (JSSVGAnglePrototypeFunctions_initialized) return;
    JSSVGAnglePrototypeFunctions_initialized = true;
    memset(JSSVGAnglePrototypeFunctions, 0, sizeof(JSSVGAnglePrototypeFunctions));
    JSSVGAnglePrototypeFunctions[0].name = "newValueSpecifiedUnits";
    JSSVGAnglePrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAnglePrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGAnglePrototypeFunctions[0].magic = JSSVGAngle::NewValueSpecifiedUnitsFuncNum;
    JSSVGAnglePrototypeFunctions[0].u.func.length = 2;
    JSSVGAnglePrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAnglePrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGAnglePrototypeFunction::callAsFunction;
    JSSVGAnglePrototypeFunctions[1].name = "convertToSpecifiedUnits";
    JSSVGAnglePrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAnglePrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGAnglePrototypeFunctions[1].magic = JSSVGAngle::ConvertToSpecifiedUnitsFuncNum;
    JSSVGAnglePrototypeFunctions[1].u.func.length = 1;
    JSSVGAnglePrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAnglePrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGAnglePrototypeFunction::callAsFunction;
}

JSValue JSSVGAnglePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAngle.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnglePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAngle.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnglePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAngleAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAngleAttributesFunctions, countof(JSSVGAngleAttributesFunctions));
    init_JSSVGAnglePrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnglePrototypeConstantsFunctions, countof(JSSVGAnglePrototypeConstantsFunctions));
    init_JSSVGAnglePrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnglePrototypeFunctions, countof(JSSVGAnglePrototypeFunctions));
}

static JSClassDef JSSVGAngleClassDefine;
static bool JSSVGAngleClassDefine_initialized = false;

static void init_JSSVGAngleClassDefine()
{
    if (JSSVGAngleClassDefine_initialized) return;
    JSSVGAngleClassDefine_initialized = true;
    memset(&JSSVGAngleClassDefine, 0, sizeof(JSSVGAngleClassDefine));
    JSSVGAngleClassDefine.class_name = "SVGAngle";
    JSSVGAngleClassDefine.finalizer = JSSVGAngle::finalizer;
    JSSVGAngleClassDefine.gc_mark = JSSVGAngle::mark;
}

JSClassID JSSVGAngle::js_class_id = 0;

void JSSVGAngle::init(JSContext* ctx)
{
    if (JSSVGAngle::js_class_id == 0) {
        init_JSSVGAngleClassDefine();
        JS_NewClassID(&JSSVGAngle::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAngle::js_class_id, &JSSVGAngleClassDefine);
        JS_SetConstructor(ctx, JSSVGAngleConstructor::self(ctx), JSSVGAnglePrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGAngle::js_class_id, JSSVGAnglePrototype::self(ctx));
    }
}

JSValue JSSVGAngle::create(JSContext* ctx, SVGAngle* impl, SVGElement* context)
{
    JSSVGAngle::init(ctx);
    JSValue _proto = JSSVGAnglePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAngle::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque(this_val, JSSVGAngle::js_class_id);
            return JS_NewInt32(ctx, imp->unitType());
        }
        case ValueAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque(this_val, JSSVGAngle::js_class_id);
            return JS_NewInt32(ctx, imp->value());
        }
        case ValueInSpecifiedUnitsAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque(this_val, JSSVGAngle::js_class_id);
            return JS_NewInt32(ctx, imp->valueInSpecifiedUnits());
        }
        case ValueAsStringAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque(this_val, JSSVGAngle::js_class_id);
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
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque(this_val, JSSVGAngle::js_class_id);
            imp->setValue(valueToFloat(ctx, value));
            break;
        }
        case ValueInSpecifiedUnitsAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque(this_val, JSSVGAngle::js_class_id);
            imp->setValueInSpecifiedUnits(valueToFloat(ctx, value));
            break;
        }
        case ValueAsStringAttrNum: {
            SVGAngle* imp = (SVGAngle*)JS_GetOpaque(this_val, JSSVGAngle::js_class_id);
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
    SVGAngle* imp = (SVGAngle*)JS_GetOpaque(this_val, JSSVGAngle::js_class_id);
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
