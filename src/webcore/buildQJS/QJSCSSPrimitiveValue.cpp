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

#include "QJSCSSPrimitiveValue.h"

#include "CSSPrimitiveValue.h"
#include "Counter.h"
#include "QJSCounter.h"
#include "QJSRect.h"
#include "Rect.h"
#include "qjs_css.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCSSPrimitiveValueAttributesFunctions[2];
static bool JSCSSPrimitiveValueAttributesFunctions_initialized = false;

static void init_JSCSSPrimitiveValueAttributesFunctions()
{
    if (JSCSSPrimitiveValueAttributesFunctions_initialized) return;
    JSCSSPrimitiveValueAttributesFunctions_initialized = true;
    memset(JSCSSPrimitiveValueAttributesFunctions, 0, sizeof(JSCSSPrimitiveValueAttributesFunctions));
    JSCSSPrimitiveValueAttributesFunctions[0].name = "primitiveType";
    JSCSSPrimitiveValueAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueAttributesFunctions[0].magic = JSCSSPrimitiveValue::PrimitiveTypeAttrNum;
    JSCSSPrimitiveValueAttributesFunctions[0].u.getset.get.getter_magic = JSCSSPrimitiveValue::getValueProperty;
    JSCSSPrimitiveValueAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueAttributesFunctions[1].name = "constructor";
    JSCSSPrimitiveValueAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueAttributesFunctions[1].magic = JSCSSPrimitiveValue::ConstructorAttrNum;
    JSCSSPrimitiveValueAttributesFunctions[1].u.getset.get.getter_magic = JSCSSPrimitiveValue::getValueProperty;
    JSCSSPrimitiveValueAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSCSSPrimitiveValueConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSPrimitiveValueConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSCSSPrimitiveValueConstructorFunctions[26];
static bool JSCSSPrimitiveValueConstructorFunctions_initialized = false;

static void init_JSCSSPrimitiveValueConstructorFunctions()
{
    if (JSCSSPrimitiveValueConstructorFunctions_initialized) return;
    JSCSSPrimitiveValueConstructorFunctions_initialized = true;
    memset(JSCSSPrimitiveValueConstructorFunctions, 0, sizeof(JSCSSPrimitiveValueConstructorFunctions));
    JSCSSPrimitiveValueConstructorFunctions[0].name = "CSS_IN";
    JSCSSPrimitiveValueConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[0].magic = CSSPrimitiveValue::CSS_IN;
    JSCSSPrimitiveValueConstructorFunctions[0].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[1].name = "CSS_EMS";
    JSCSSPrimitiveValueConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[1].magic = CSSPrimitiveValue::CSS_EMS;
    JSCSSPrimitiveValueConstructorFunctions[1].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[2].name = "CSS_EXS";
    JSCSSPrimitiveValueConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[2].magic = CSSPrimitiveValue::CSS_EXS;
    JSCSSPrimitiveValueConstructorFunctions[2].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[2].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[3].name = "CSS_RAD";
    JSCSSPrimitiveValueConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[3].magic = CSSPrimitiveValue::CSS_RAD;
    JSCSSPrimitiveValueConstructorFunctions[3].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[3].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[4].name = "CSS_KHZ";
    JSCSSPrimitiveValueConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[4].magic = CSSPrimitiveValue::CSS_KHZ;
    JSCSSPrimitiveValueConstructorFunctions[4].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[4].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[5].name = "CSS_PX";
    JSCSSPrimitiveValueConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[5].magic = CSSPrimitiveValue::CSS_PX;
    JSCSSPrimitiveValueConstructorFunctions[5].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[5].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[6].name = "CSS_CM";
    JSCSSPrimitiveValueConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[6].magic = CSSPrimitiveValue::CSS_CM;
    JSCSSPrimitiveValueConstructorFunctions[6].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[6].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[7].name = "CSS_S";
    JSCSSPrimitiveValueConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[7].magic = CSSPrimitiveValue::CSS_S;
    JSCSSPrimitiveValueConstructorFunctions[7].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[7].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[8].name = "CSS_DIMENSION";
    JSCSSPrimitiveValueConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[8].magic = CSSPrimitiveValue::CSS_DIMENSION;
    JSCSSPrimitiveValueConstructorFunctions[8].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[8].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[9].name = "CSS_UNKNOWN";
    JSCSSPrimitiveValueConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[9].magic = CSSPrimitiveValue::CSS_UNKNOWN;
    JSCSSPrimitiveValueConstructorFunctions[9].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[9].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[10].name = "CSS_GRAD";
    JSCSSPrimitiveValueConstructorFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[10].magic = CSSPrimitiveValue::CSS_GRAD;
    JSCSSPrimitiveValueConstructorFunctions[10].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[10].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[11].name = "CSS_IDENT";
    JSCSSPrimitiveValueConstructorFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[11].magic = CSSPrimitiveValue::CSS_IDENT;
    JSCSSPrimitiveValueConstructorFunctions[11].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[11].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[12].name = "CSS_URI";
    JSCSSPrimitiveValueConstructorFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[12].magic = CSSPrimitiveValue::CSS_URI;
    JSCSSPrimitiveValueConstructorFunctions[12].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[12].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[13].name = "CSS_NUMBER";
    JSCSSPrimitiveValueConstructorFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[13].magic = CSSPrimitiveValue::CSS_NUMBER;
    JSCSSPrimitiveValueConstructorFunctions[13].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[13].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[14].name = "CSS_COUNTER";
    JSCSSPrimitiveValueConstructorFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[14].magic = CSSPrimitiveValue::CSS_COUNTER;
    JSCSSPrimitiveValueConstructorFunctions[14].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[14].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[15].name = "CSS_PERCENTAGE";
    JSCSSPrimitiveValueConstructorFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[15].magic = CSSPrimitiveValue::CSS_PERCENTAGE;
    JSCSSPrimitiveValueConstructorFunctions[15].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[15].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[16].name = "CSS_MM";
    JSCSSPrimitiveValueConstructorFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[16].magic = CSSPrimitiveValue::CSS_MM;
    JSCSSPrimitiveValueConstructorFunctions[16].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[16].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[17].name = "CSS_PT";
    JSCSSPrimitiveValueConstructorFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[17].magic = CSSPrimitiveValue::CSS_PT;
    JSCSSPrimitiveValueConstructorFunctions[17].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[17].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[18].name = "CSS_PC";
    JSCSSPrimitiveValueConstructorFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[18].magic = CSSPrimitiveValue::CSS_PC;
    JSCSSPrimitiveValueConstructorFunctions[18].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[18].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[19].name = "CSS_DEG";
    JSCSSPrimitiveValueConstructorFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[19].magic = CSSPrimitiveValue::CSS_DEG;
    JSCSSPrimitiveValueConstructorFunctions[19].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[19].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[20].name = "CSS_MS";
    JSCSSPrimitiveValueConstructorFunctions[20].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[20].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[20].magic = CSSPrimitiveValue::CSS_MS;
    JSCSSPrimitiveValueConstructorFunctions[20].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[20].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[21].name = "CSS_HZ";
    JSCSSPrimitiveValueConstructorFunctions[21].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[21].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[21].magic = CSSPrimitiveValue::CSS_HZ;
    JSCSSPrimitiveValueConstructorFunctions[21].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[21].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[22].name = "CSS_STRING";
    JSCSSPrimitiveValueConstructorFunctions[22].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[22].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[22].magic = CSSPrimitiveValue::CSS_STRING;
    JSCSSPrimitiveValueConstructorFunctions[22].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[22].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[23].name = "CSS_ATTR";
    JSCSSPrimitiveValueConstructorFunctions[23].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[23].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[23].magic = CSSPrimitiveValue::CSS_ATTR;
    JSCSSPrimitiveValueConstructorFunctions[23].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[23].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[24].name = "CSS_RECT";
    JSCSSPrimitiveValueConstructorFunctions[24].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[24].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[24].magic = CSSPrimitiveValue::CSS_RECT;
    JSCSSPrimitiveValueConstructorFunctions[24].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[24].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValueConstructorFunctions[25].name = "CSS_RGBCOLOR";
    JSCSSPrimitiveValueConstructorFunctions[25].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValueConstructorFunctions[25].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValueConstructorFunctions[25].magic = CSSPrimitiveValue::CSS_RGBCOLOR;
    JSCSSPrimitiveValueConstructorFunctions[25].u.getset.get.getter_magic = JSCSSPrimitiveValueConstructor::getValueProperty;
    JSCSSPrimitiveValueConstructorFunctions[25].u.getset.set.setter_magic = NULL;
}

JSValue JSCSSPrimitiveValueConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSPrimitiveValue.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSPrimitiveValueConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSPrimitiveValue.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSPrimitiveValueConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSPrimitiveValueConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPrimitiveValueConstructorFunctions, countof(JSCSSPrimitiveValueConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSCSSPrimitiveValuePrototypeConstantsFunctions[26];
static bool JSCSSPrimitiveValuePrototypeConstantsFunctions_initialized = false;

static void init_JSCSSPrimitiveValuePrototypeConstantsFunctions()
{
    if (JSCSSPrimitiveValuePrototypeConstantsFunctions_initialized) return;
    JSCSSPrimitiveValuePrototypeConstantsFunctions_initialized = true;
    memset(JSCSSPrimitiveValuePrototypeConstantsFunctions, 0, sizeof(JSCSSPrimitiveValuePrototypeConstantsFunctions));
    JSCSSPrimitiveValuePrototypeConstantsFunctions[0].name = "CSS_IN";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[0].magic = CSSPrimitiveValue::CSS_IN;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[1].name = "CSS_EMS";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[1].magic = CSSPrimitiveValue::CSS_EMS;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[2].name = "CSS_EXS";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[2].magic = CSSPrimitiveValue::CSS_EXS;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[3].name = "CSS_RAD";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[3].magic = CSSPrimitiveValue::CSS_RAD;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[3].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[3].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[4].name = "CSS_KHZ";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[4].magic = CSSPrimitiveValue::CSS_KHZ;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[4].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[4].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[5].name = "CSS_PX";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[5].magic = CSSPrimitiveValue::CSS_PX;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[5].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[5].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[6].name = "CSS_CM";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[6].magic = CSSPrimitiveValue::CSS_CM;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[6].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[6].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[7].name = "CSS_S";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[7].magic = CSSPrimitiveValue::CSS_S;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[7].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[7].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[8].name = "CSS_DIMENSION";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[8].magic = CSSPrimitiveValue::CSS_DIMENSION;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[8].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[8].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[9].name = "CSS_UNKNOWN";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[9].magic = CSSPrimitiveValue::CSS_UNKNOWN;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[9].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[9].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[10].name = "CSS_GRAD";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[10].magic = CSSPrimitiveValue::CSS_GRAD;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[10].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[10].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[11].name = "CSS_IDENT";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[11].magic = CSSPrimitiveValue::CSS_IDENT;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[11].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[11].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[12].name = "CSS_URI";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[12].magic = CSSPrimitiveValue::CSS_URI;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[12].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[12].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[13].name = "CSS_NUMBER";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[13].magic = CSSPrimitiveValue::CSS_NUMBER;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[13].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[13].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[14].name = "CSS_COUNTER";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[14].magic = CSSPrimitiveValue::CSS_COUNTER;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[14].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[14].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[15].name = "CSS_PERCENTAGE";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[15].magic = CSSPrimitiveValue::CSS_PERCENTAGE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[15].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[15].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[16].name = "CSS_MM";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[16].magic = CSSPrimitiveValue::CSS_MM;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[16].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[16].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[17].name = "CSS_PT";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[17].magic = CSSPrimitiveValue::CSS_PT;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[17].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[17].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[18].name = "CSS_PC";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[18].magic = CSSPrimitiveValue::CSS_PC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[18].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[18].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[19].name = "CSS_DEG";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[19].magic = CSSPrimitiveValue::CSS_DEG;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[19].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[19].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[20].name = "CSS_MS";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[20].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[20].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[20].magic = CSSPrimitiveValue::CSS_MS;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[20].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[20].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[21].name = "CSS_HZ";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[21].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[21].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[21].magic = CSSPrimitiveValue::CSS_HZ;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[21].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[21].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[22].name = "CSS_STRING";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[22].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[22].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[22].magic = CSSPrimitiveValue::CSS_STRING;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[22].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[22].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[23].name = "CSS_ATTR";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[23].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[23].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[23].magic = CSSPrimitiveValue::CSS_ATTR;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[23].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[23].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[24].name = "CSS_RECT";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[24].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[24].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[24].magic = CSSPrimitiveValue::CSS_RECT;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[24].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[24].u.getset.set.setter_magic = NULL;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[25].name = "CSS_RGBCOLOR";
    JSCSSPrimitiveValuePrototypeConstantsFunctions[25].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[25].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[25].magic = CSSPrimitiveValue::CSS_RGBCOLOR;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[25].u.getset.get.getter_magic = JSCSSPrimitiveValuePrototype::getValueProperty;
    JSCSSPrimitiveValuePrototypeConstantsFunctions[25].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSCSSPrimitiveValuePrototypeFunctions[7];
static bool JSCSSPrimitiveValuePrototypeFunctions_initialized = false;

static void init_JSCSSPrimitiveValuePrototypeFunctions()
{
    if (JSCSSPrimitiveValuePrototypeFunctions_initialized) return;
    JSCSSPrimitiveValuePrototypeFunctions_initialized = true;
    memset(JSCSSPrimitiveValuePrototypeFunctions, 0, sizeof(JSCSSPrimitiveValuePrototypeFunctions));
    JSCSSPrimitiveValuePrototypeFunctions[0].name = "setStringValue";
    JSCSSPrimitiveValuePrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSCSSPrimitiveValuePrototypeFunctions[0].magic = JSCSSPrimitiveValue::SetStringValueFuncNum;
    JSCSSPrimitiveValuePrototypeFunctions[0].u.func.length = 2;
    JSCSSPrimitiveValuePrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSPrimitiveValuePrototypeFunctions[0].u.func.cfunc.generic_magic = JSCSSPrimitiveValuePrototypeFunction::callAsFunction;
    JSCSSPrimitiveValuePrototypeFunctions[1].name = "getStringValue";
    JSCSSPrimitiveValuePrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSCSSPrimitiveValuePrototypeFunctions[1].magic = JSCSSPrimitiveValue::GetStringValueFuncNum;
    JSCSSPrimitiveValuePrototypeFunctions[1].u.func.length = 0;
    JSCSSPrimitiveValuePrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSPrimitiveValuePrototypeFunctions[1].u.func.cfunc.generic_magic = JSCSSPrimitiveValuePrototypeFunction::callAsFunction;
    JSCSSPrimitiveValuePrototypeFunctions[2].name = "getRGBColorValue";
    JSCSSPrimitiveValuePrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSCSSPrimitiveValuePrototypeFunctions[2].magic = JSCSSPrimitiveValue::GetRGBColorValueFuncNum;
    JSCSSPrimitiveValuePrototypeFunctions[2].u.func.length = 0;
    JSCSSPrimitiveValuePrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSPrimitiveValuePrototypeFunctions[2].u.func.cfunc.generic_magic = JSCSSPrimitiveValuePrototypeFunction::callAsFunction;
    JSCSSPrimitiveValuePrototypeFunctions[3].name = "getCounterValue";
    JSCSSPrimitiveValuePrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSCSSPrimitiveValuePrototypeFunctions[3].magic = JSCSSPrimitiveValue::GetCounterValueFuncNum;
    JSCSSPrimitiveValuePrototypeFunctions[3].u.func.length = 0;
    JSCSSPrimitiveValuePrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSPrimitiveValuePrototypeFunctions[3].u.func.cfunc.generic_magic = JSCSSPrimitiveValuePrototypeFunction::callAsFunction;
    JSCSSPrimitiveValuePrototypeFunctions[4].name = "setFloatValue";
    JSCSSPrimitiveValuePrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSCSSPrimitiveValuePrototypeFunctions[4].magic = JSCSSPrimitiveValue::SetFloatValueFuncNum;
    JSCSSPrimitiveValuePrototypeFunctions[4].u.func.length = 2;
    JSCSSPrimitiveValuePrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSPrimitiveValuePrototypeFunctions[4].u.func.cfunc.generic_magic = JSCSSPrimitiveValuePrototypeFunction::callAsFunction;
    JSCSSPrimitiveValuePrototypeFunctions[5].name = "getFloatValue";
    JSCSSPrimitiveValuePrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSCSSPrimitiveValuePrototypeFunctions[5].magic = JSCSSPrimitiveValue::GetFloatValueFuncNum;
    JSCSSPrimitiveValuePrototypeFunctions[5].u.func.length = 1;
    JSCSSPrimitiveValuePrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSPrimitiveValuePrototypeFunctions[5].u.func.cfunc.generic_magic = JSCSSPrimitiveValuePrototypeFunction::callAsFunction;
    JSCSSPrimitiveValuePrototypeFunctions[6].name = "getRectValue";
    JSCSSPrimitiveValuePrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSPrimitiveValuePrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSCSSPrimitiveValuePrototypeFunctions[6].magic = JSCSSPrimitiveValue::GetRectValueFuncNum;
    JSCSSPrimitiveValuePrototypeFunctions[6].u.func.length = 0;
    JSCSSPrimitiveValuePrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSPrimitiveValuePrototypeFunctions[6].u.func.cfunc.generic_magic = JSCSSPrimitiveValuePrototypeFunction::callAsFunction;
}

JSValue JSCSSPrimitiveValuePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSPrimitiveValue.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSValuePrototype::self(ctx));
        JSCSSPrimitiveValuePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSPrimitiveValue.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSPrimitiveValuePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSPrimitiveValueAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPrimitiveValueAttributesFunctions, countof(JSCSSPrimitiveValueAttributesFunctions));
    init_JSCSSPrimitiveValuePrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPrimitiveValuePrototypeConstantsFunctions, countof(JSCSSPrimitiveValuePrototypeConstantsFunctions));
    init_JSCSSPrimitiveValuePrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPrimitiveValuePrototypeFunctions, countof(JSCSSPrimitiveValuePrototypeFunctions));
}

JSValue JSCSSPrimitiveValuePrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSCSSPrimitiveValueClassDefine;
static bool JSCSSPrimitiveValueClassDefine_initialized = false;

static void init_JSCSSPrimitiveValueClassDefine()
{
    if (JSCSSPrimitiveValueClassDefine_initialized) return;
    JSCSSPrimitiveValueClassDefine_initialized = true;
    memset(&JSCSSPrimitiveValueClassDefine, 0, sizeof(JSCSSPrimitiveValueClassDefine));
    JSCSSPrimitiveValueClassDefine.class_name = "CSSPrimitiveValue";
    JSCSSPrimitiveValueClassDefine.finalizer = JSCSSPrimitiveValue::finalizer;
    JSCSSPrimitiveValueClassDefine.gc_mark = JSCSSPrimitiveValue::mark;
}

JSClassID JSCSSPrimitiveValue::js_class_id = 0;

void JSCSSPrimitiveValue::init(JSContext* ctx)
{
    if (JSCSSPrimitiveValue::js_class_id == 0) {
        init_JSCSSPrimitiveValueClassDefine();
        JS_NewClassID(&JSCSSPrimitiveValue::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSPrimitiveValue::js_class_id, &JSCSSPrimitiveValueClassDefine);
        JS_SetConstructor(ctx, JSCSSPrimitiveValueConstructor::self(ctx), JSCSSPrimitiveValuePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSPrimitiveValue::js_class_id, JSCSSPrimitiveValuePrototype::self(ctx));
    }
}

JSValue JSCSSPrimitiveValue::create(JSContext* ctx, CSSPrimitiveValue* impl)
{
    JSCSSPrimitiveValue::init(ctx);
    JSValue _proto = JSCSSPrimitiveValuePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCSSPrimitiveValue::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSPrimitiveValue::finalizer(JSRuntime* rt, JSValue val)
{
    CSSPrimitiveValue* impl = (CSSPrimitiveValue*)JS_GetOpaque(val, JSCSSPrimitiveValue::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSPrimitiveValue::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSValue::mark(rt, val, mark_func);
}

JSValue JSCSSPrimitiveValue::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PrimitiveTypeAttrNum: {
            CSSPrimitiveValue* imp = (CSSPrimitiveValue*)JS_GetOpaque(this_val, JSCSSPrimitiveValue::js_class_id);
            return JS_NewInt32(ctx, imp->primitiveType());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSPrimitiveValue::getConstructor(JSContext *ctx)
{
    return JSCSSPrimitiveValueConstructor::self(ctx);
}

JSValue JSCSSPrimitiveValuePrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CSSPrimitiveValue* imp = (CSSPrimitiveValue*)JS_GetOpaque(this_val, JSCSSPrimitiveValue::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCSSPrimitiveValue::SetFloatValueFuncNum: {
            ExceptionCode ec = 0;
            unsigned short unitType = valueToInt32(ctx, argv[0]);
            float floatValue = valueToFloat(ctx, argv[1]);
            imp->setFloatValue(unitType, floatValue, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCSSPrimitiveValue::GetFloatValueFuncNum: {
            unsigned short unitType = valueToInt32(ctx, argv[0]);

            JSValue result = JS_NewInt32(ctx, imp->getFloatValue(unitType));
            return result;
        }
        case JSCSSPrimitiveValue::SetStringValueFuncNum: {
            ExceptionCode ec = 0;
            unsigned short stringType = valueToInt32(ctx, argv[0]);
            String stringValue = valueToString(ctx, argv[1]);
            imp->setStringValue(stringType, stringValue, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCSSPrimitiveValue::GetStringValueFuncNum: {

            JSValue result = JS_NewString(ctx, ((const String&)imp->getStringValue()).utf8().data());
            return result;
        }
        case JSCSSPrimitiveValue::GetCounterValueFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->getCounterValue()));
            return result;
        }
        case JSCSSPrimitiveValue::GetRectValueFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->getRectValue()));
            return result;
        }
        case JSCSSPrimitiveValue::GetRGBColorValueFuncNum: {

            JSValue result = getJSRGBColor(ctx, imp->getRGBColorValue());
            return result;
        }
    }
    return JS_NULL;
}


}
