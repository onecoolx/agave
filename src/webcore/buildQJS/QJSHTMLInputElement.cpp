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

#include "QJSHTMLInputElement.h"

#include "HTMLFormElement.h"
#include "HTMLInputElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLInputElementAttributesFunctions[32];
static bool JSHTMLInputElementAttributesFunctions_initialized = false;

static void init_JSHTMLInputElementAttributesFunctions()
{
    if (JSHTMLInputElementAttributesFunctions_initialized) return;
    JSHTMLInputElementAttributesFunctions_initialized = true;
    memset(JSHTMLInputElementAttributesFunctions, 0, sizeof(JSHTMLInputElementAttributesFunctions));
    JSHTMLInputElementAttributesFunctions[0].name = "defaultValue";
    JSHTMLInputElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[0].magic = JSHTMLInputElement::DefaultValueAttrNum;
    JSHTMLInputElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[1].name = "defaultChecked";
    JSHTMLInputElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[1].magic = JSHTMLInputElement::DefaultCheckedAttrNum;
    JSHTMLInputElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[2].name = "form";
    JSHTMLInputElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[2].magic = JSHTMLInputElement::FormAttrNum;
    JSHTMLInputElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[3].name = "accept";
    JSHTMLInputElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[3].magic = JSHTMLInputElement::AcceptAttrNum;
    JSHTMLInputElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[4].name = "accessKey";
    JSHTMLInputElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[4].magic = JSHTMLInputElement::AccessKeyAttrNum;
    JSHTMLInputElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[5].name = "align";
    JSHTMLInputElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[5].magic = JSHTMLInputElement::AlignAttrNum;
    JSHTMLInputElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[6].name = "alt";
    JSHTMLInputElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[6].magic = JSHTMLInputElement::AltAttrNum;
    JSHTMLInputElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[7].name = "checked";
    JSHTMLInputElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[7].magic = JSHTMLInputElement::CheckedAttrNum;
    JSHTMLInputElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[8].name = "disabled";
    JSHTMLInputElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[8].magic = JSHTMLInputElement::DisabledAttrNum;
    JSHTMLInputElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[9].name = "maxLength";
    JSHTMLInputElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[9].magic = JSHTMLInputElement::MaxLengthAttrNum;
    JSHTMLInputElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[9].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[10].name = "name";
    JSHTMLInputElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[10].magic = JSHTMLInputElement::NameAttrNum;
    JSHTMLInputElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[11].name = "readOnly";
    JSHTMLInputElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[11].magic = JSHTMLInputElement::ReadOnlyAttrNum;
    JSHTMLInputElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[11].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[12].name = "size";
    JSHTMLInputElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[12].magic = JSHTMLInputElement::SizeAttrNum;
    JSHTMLInputElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[12].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[13].name = "src";
    JSHTMLInputElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[13].magic = JSHTMLInputElement::SrcAttrNum;
    JSHTMLInputElementAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[13].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[14].name = "tabIndex";
    JSHTMLInputElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[14].magic = JSHTMLInputElement::TabIndexAttrNum;
    JSHTMLInputElementAttributesFunctions[14].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[14].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[15].name = "type";
    JSHTMLInputElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[15].magic = JSHTMLInputElement::TypeAttrNum;
    JSHTMLInputElementAttributesFunctions[15].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[15].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[16].name = "useMap";
    JSHTMLInputElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[16].magic = JSHTMLInputElement::UseMapAttrNum;
    JSHTMLInputElementAttributesFunctions[16].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[16].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[17].name = "value";
    JSHTMLInputElementAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[17].magic = JSHTMLInputElement::ValueAttrNum;
    JSHTMLInputElementAttributesFunctions[17].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[17].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[18].name = "required";
    JSHTMLInputElementAttributesFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[18].magic = JSHTMLInputElement::RequiredAttrNum;
    JSHTMLInputElementAttributesFunctions[18].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[18].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[19].name = "valueMissing";
    JSHTMLInputElementAttributesFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[19].magic = JSHTMLInputElement::ValueMissingAttrNum;
    JSHTMLInputElementAttributesFunctions[19].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[19].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[20].name = "typeMismatch";
    JSHTMLInputElementAttributesFunctions[20].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[20].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[20].magic = JSHTMLInputElement::TypeMismatchAttrNum;
    JSHTMLInputElementAttributesFunctions[20].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[20].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[21].name = "patternMismatch";
    JSHTMLInputElementAttributesFunctions[21].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[21].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[21].magic = JSHTMLInputElement::PatternMismatchAttrNum;
    JSHTMLInputElementAttributesFunctions[21].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[21].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[22].name = "rangeUnderflow";
    JSHTMLInputElementAttributesFunctions[22].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[22].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[22].magic = JSHTMLInputElement::RangeUnderflowAttrNum;
    JSHTMLInputElementAttributesFunctions[22].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[22].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[23].name = "rangeOverflow";
    JSHTMLInputElementAttributesFunctions[23].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[23].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[23].magic = JSHTMLInputElement::RangeOverflowAttrNum;
    JSHTMLInputElementAttributesFunctions[23].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[23].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[24].name = "stepMismatch";
    JSHTMLInputElementAttributesFunctions[24].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[24].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[24].magic = JSHTMLInputElement::StepMismatchAttrNum;
    JSHTMLInputElementAttributesFunctions[24].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[24].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[25].name = "tooLong";
    JSHTMLInputElementAttributesFunctions[25].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[25].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[25].magic = JSHTMLInputElement::TooLongAttrNum;
    JSHTMLInputElementAttributesFunctions[25].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[25].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[26].name = "customError";
    JSHTMLInputElementAttributesFunctions[26].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[26].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[26].magic = JSHTMLInputElement::CustomErrorAttrNum;
    JSHTMLInputElementAttributesFunctions[26].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[26].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[27].name = "willValidate";
    JSHTMLInputElementAttributesFunctions[27].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[27].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[27].magic = JSHTMLInputElement::WillValidateAttrNum;
    JSHTMLInputElementAttributesFunctions[27].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[27].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[28].name = "valid";
    JSHTMLInputElementAttributesFunctions[28].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[28].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[28].magic = JSHTMLInputElement::ValidAttrNum;
    JSHTMLInputElementAttributesFunctions[28].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[28].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[29].name = "validationMessage";
    JSHTMLInputElementAttributesFunctions[29].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[29].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[29].magic = JSHTMLInputElement::ValidationMessageAttrNum;
    JSHTMLInputElementAttributesFunctions[29].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[29].u.getset.set.setter_magic = NULL;
    JSHTMLInputElementAttributesFunctions[30].name = "indeterminate";
    JSHTMLInputElementAttributesFunctions[30].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[30].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[30].magic = JSHTMLInputElement::IndeterminateAttrNum;
    JSHTMLInputElementAttributesFunctions[30].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[30].u.getset.set.setter_magic = JSHTMLInputElement::putValueProperty;
    JSHTMLInputElementAttributesFunctions[31].name = "constructor";
    JSHTMLInputElementAttributesFunctions[31].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLInputElementAttributesFunctions[31].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLInputElementAttributesFunctions[31].magic = JSHTMLInputElement::ConstructorAttrNum;
    JSHTMLInputElementAttributesFunctions[31].u.getset.get.getter_magic = JSHTMLInputElement::getValueProperty;
    JSHTMLInputElementAttributesFunctions[31].u.getset.set.setter_magic = NULL;
}

class JSHTMLInputElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLInputElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLInputElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLInputElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLInputElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLInputElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLInputElementPrototypeFunctions[6];
static bool JSHTMLInputElementPrototypeFunctions_initialized = false;

static void init_JSHTMLInputElementPrototypeFunctions()
{
    if (JSHTMLInputElementPrototypeFunctions_initialized) return;
    JSHTMLInputElementPrototypeFunctions_initialized = true;
    memset(JSHTMLInputElementPrototypeFunctions, 0, sizeof(JSHTMLInputElementPrototypeFunctions));
    JSHTMLInputElementPrototypeFunctions[0].name = "blur";
    JSHTMLInputElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLInputElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLInputElementPrototypeFunctions[0].magic = JSHTMLInputElement::BlurFuncNum;
    JSHTMLInputElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLInputElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLInputElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLInputElementPrototypeFunction::callAsFunction;
    JSHTMLInputElementPrototypeFunctions[1].name = "focus";
    JSHTMLInputElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLInputElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLInputElementPrototypeFunctions[1].magic = JSHTMLInputElement::FocusFuncNum;
    JSHTMLInputElementPrototypeFunctions[1].u.func.length = 0;
    JSHTMLInputElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLInputElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLInputElementPrototypeFunction::callAsFunction;
    JSHTMLInputElementPrototypeFunctions[2].name = "select";
    JSHTMLInputElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLInputElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSHTMLInputElementPrototypeFunctions[2].magic = JSHTMLInputElement::SelectFuncNum;
    JSHTMLInputElementPrototypeFunctions[2].u.func.length = 0;
    JSHTMLInputElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLInputElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSHTMLInputElementPrototypeFunction::callAsFunction;
    JSHTMLInputElementPrototypeFunctions[3].name = "click";
    JSHTMLInputElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLInputElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSHTMLInputElementPrototypeFunctions[3].magic = JSHTMLInputElement::ClickFuncNum;
    JSHTMLInputElementPrototypeFunctions[3].u.func.length = 0;
    JSHTMLInputElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLInputElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSHTMLInputElementPrototypeFunction::callAsFunction;
    JSHTMLInputElementPrototypeFunctions[4].name = "checkValidity";
    JSHTMLInputElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLInputElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSHTMLInputElementPrototypeFunctions[4].magic = JSHTMLInputElement::CheckValidityFuncNum;
    JSHTMLInputElementPrototypeFunctions[4].u.func.length = 0;
    JSHTMLInputElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLInputElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSHTMLInputElementPrototypeFunction::callAsFunction;
    JSHTMLInputElementPrototypeFunctions[5].name = "setCustomValidity";
    JSHTMLInputElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLInputElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSHTMLInputElementPrototypeFunctions[5].magic = JSHTMLInputElement::SetCustomValidityFuncNum;
    JSHTMLInputElementPrototypeFunctions[5].u.func.length = 1;
    JSHTMLInputElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLInputElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSHTMLInputElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLInputElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLInputElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLInputElementBasePrototype::self(ctx));
        JSHTMLInputElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLInputElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLInputElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLInputElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLInputElementAttributesFunctions, countof(JSHTMLInputElementAttributesFunctions));
    init_JSHTMLInputElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLInputElementPrototypeFunctions, countof(JSHTMLInputElementPrototypeFunctions));
}

JSClassID JSHTMLInputElement::js_class_id = 0;

void JSHTMLInputElement::init(JSContext* ctx)
{
    if (JSHTMLInputElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLInputElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLInputElement::create(JSContext* ctx, HTMLInputElement* impl)
{
    JSHTMLInputElement::init(ctx);
    JSValue _proto = JSHTMLInputElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLInputElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLInputElement* impl = (HTMLInputElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLInputElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLInputElementBase::mark(rt, val, mark_func);
}

JSValue JSHTMLInputElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DefaultValueAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->defaultValue()).utf8().data());
        }
        case DefaultCheckedAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->defaultChecked() ? 1 : 0);
        }
        case FormAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AcceptAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accept()).utf8().data());
        }
        case AccessKeyAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case AltAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alt()).utf8().data());
        }
        case CheckedAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->checked() ? 1 : 0);
        }
        case DisabledAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case MaxLengthAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->maxLength());
        }
        case NameAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ReadOnlyAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->readOnly() ? 1 : 0);
        }
        case SizeAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->size());
        }
        case SrcAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->tabIndex());
        }
        case TypeAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case UseMapAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->useMap()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case RequiredAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->required() ? 1 : 0);
        }
        case ValueMissingAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->valueMissing() ? 1 : 0);
        }
        case TypeMismatchAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->typeMismatch() ? 1 : 0);
        }
        case PatternMismatchAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->patternMismatch() ? 1 : 0);
        }
        case RangeUnderflowAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->rangeUnderflow() ? 1 : 0);
        }
        case RangeOverflowAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->rangeOverflow() ? 1 : 0);
        }
        case StepMismatchAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->stepMismatch() ? 1 : 0);
        }
        case TooLongAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->tooLong() ? 1 : 0);
        }
        case CustomErrorAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->customError() ? 1 : 0);
        }
        case WillValidateAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->willValidate() ? 1 : 0);
        }
        case ValidAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->valid() ? 1 : 0);
        }
        case ValidationMessageAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->validationMessage()).utf8().data());
        }
        case IndeterminateAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->indeterminate() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLInputElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DefaultValueAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDefaultValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DefaultCheckedAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDefaultChecked(valueToBoolean(ctx, value));
            break;
        }
        case AcceptAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAccept(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AccessKeyAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AltAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlt(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CheckedAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setChecked(valueToBoolean(ctx, value));
            break;
        }
        case DisabledAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case MaxLengthAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMaxLength(valueToInt32(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ReadOnlyAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setReadOnly(valueToBoolean(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSize(valueToInt32(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSrc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case UseMapAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setUseMap(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case IndeterminateAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setIndeterminate(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLInputElement::getConstructor(JSContext *ctx)
{
    return JSHTMLInputElementConstructor::self(ctx);
}

JSValue JSHTMLInputElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLInputElement::BlurFuncNum: {
            imp->blur();
            return JS_UNDEFINED;
        }
        case JSHTMLInputElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
        case JSHTMLInputElement::SelectFuncNum: {
            imp->select();
            return JS_UNDEFINED;
        }
        case JSHTMLInputElement::ClickFuncNum: {
            imp->click();
            return JS_UNDEFINED;
        }
        case JSHTMLInputElement::CheckValidityFuncNum: {

            JSValue result = JS_NewBool(ctx, imp->checkValidity() ? 1 : 0);
            return result;
        }
        case JSHTMLInputElement::SetCustomValidityFuncNum: {
            String message = valueToString(ctx, argv[0]);
            imp->setCustomValidity(message);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
