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

#include "QJSRange.h"

#include "DocumentFragment.h"
#include "ExceptionCode.h"
#include "Node.h"
#include "QJSDocumentFragment.h"
#include "QJSNode.h"
#include "QJSRange.h"
#include "Range.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSRangeAttributesFunctions[7];
static bool JSRangeAttributesFunctions_initialized = false;

static void init_JSRangeAttributesFunctions()
{
    if (JSRangeAttributesFunctions_initialized) return;
    JSRangeAttributesFunctions_initialized = true;
    memset(JSRangeAttributesFunctions, 0, sizeof(JSRangeAttributesFunctions));
    JSRangeAttributesFunctions[0].name = "endOffset";
    JSRangeAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeAttributesFunctions[0].magic = JSRange::EndOffsetAttrNum;
    JSRangeAttributesFunctions[0].u.getset.get.getter_magic = JSRange::getValueProperty;
    JSRangeAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSRangeAttributesFunctions[1].name = "startOffset";
    JSRangeAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeAttributesFunctions[1].magic = JSRange::StartOffsetAttrNum;
    JSRangeAttributesFunctions[1].u.getset.get.getter_magic = JSRange::getValueProperty;
    JSRangeAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSRangeAttributesFunctions[2].name = "endContainer";
    JSRangeAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeAttributesFunctions[2].magic = JSRange::EndContainerAttrNum;
    JSRangeAttributesFunctions[2].u.getset.get.getter_magic = JSRange::getValueProperty;
    JSRangeAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSRangeAttributesFunctions[3].name = "startContainer";
    JSRangeAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeAttributesFunctions[3].magic = JSRange::StartContainerAttrNum;
    JSRangeAttributesFunctions[3].u.getset.get.getter_magic = JSRange::getValueProperty;
    JSRangeAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSRangeAttributesFunctions[4].name = "collapsed";
    JSRangeAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeAttributesFunctions[4].magic = JSRange::CollapsedAttrNum;
    JSRangeAttributesFunctions[4].u.getset.get.getter_magic = JSRange::getValueProperty;
    JSRangeAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSRangeAttributesFunctions[5].name = "commonAncestorContainer";
    JSRangeAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeAttributesFunctions[5].magic = JSRange::CommonAncestorContainerAttrNum;
    JSRangeAttributesFunctions[5].u.getset.get.getter_magic = JSRange::getValueProperty;
    JSRangeAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSRangeAttributesFunctions[6].name = "constructor";
    JSRangeAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeAttributesFunctions[6].magic = JSRange::ConstructorAttrNum;
    JSRangeAttributesFunctions[6].u.getset.get.getter_magic = JSRange::getValueProperty;
    JSRangeAttributesFunctions[6].u.getset.set.setter_magic = NULL;
}

class JSRangeConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSRangeConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSRangeConstructorFunctions[8];
static bool JSRangeConstructorFunctions_initialized = false;

static void init_JSRangeConstructorFunctions()
{
    if (JSRangeConstructorFunctions_initialized) return;
    JSRangeConstructorFunctions_initialized = true;
    memset(JSRangeConstructorFunctions, 0, sizeof(JSRangeConstructorFunctions));
    JSRangeConstructorFunctions[0].name = "START_TO_START";
    JSRangeConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeConstructorFunctions[0].magic = Range::START_TO_START;
    JSRangeConstructorFunctions[0].u.getset.get.getter_magic = JSRangeConstructor::getValueProperty;
    JSRangeConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSRangeConstructorFunctions[1].name = "NODE_AFTER";
    JSRangeConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeConstructorFunctions[1].magic = Range::NODE_AFTER;
    JSRangeConstructorFunctions[1].u.getset.get.getter_magic = JSRangeConstructor::getValueProperty;
    JSRangeConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSRangeConstructorFunctions[2].name = "NODE_BEFORE";
    JSRangeConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeConstructorFunctions[2].magic = Range::NODE_BEFORE;
    JSRangeConstructorFunctions[2].u.getset.get.getter_magic = JSRangeConstructor::getValueProperty;
    JSRangeConstructorFunctions[2].u.getset.set.setter_magic = NULL;
    JSRangeConstructorFunctions[3].name = "START_TO_END";
    JSRangeConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeConstructorFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeConstructorFunctions[3].magic = Range::START_TO_END;
    JSRangeConstructorFunctions[3].u.getset.get.getter_magic = JSRangeConstructor::getValueProperty;
    JSRangeConstructorFunctions[3].u.getset.set.setter_magic = NULL;
    JSRangeConstructorFunctions[4].name = "END_TO_END";
    JSRangeConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeConstructorFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeConstructorFunctions[4].magic = Range::END_TO_END;
    JSRangeConstructorFunctions[4].u.getset.get.getter_magic = JSRangeConstructor::getValueProperty;
    JSRangeConstructorFunctions[4].u.getset.set.setter_magic = NULL;
    JSRangeConstructorFunctions[5].name = "END_TO_START";
    JSRangeConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeConstructorFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeConstructorFunctions[5].magic = Range::END_TO_START;
    JSRangeConstructorFunctions[5].u.getset.get.getter_magic = JSRangeConstructor::getValueProperty;
    JSRangeConstructorFunctions[5].u.getset.set.setter_magic = NULL;
    JSRangeConstructorFunctions[6].name = "NODE_BEFORE_AND_AFTER";
    JSRangeConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeConstructorFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeConstructorFunctions[6].magic = Range::NODE_BEFORE_AND_AFTER;
    JSRangeConstructorFunctions[6].u.getset.get.getter_magic = JSRangeConstructor::getValueProperty;
    JSRangeConstructorFunctions[6].u.getset.set.setter_magic = NULL;
    JSRangeConstructorFunctions[7].name = "NODE_INSIDE";
    JSRangeConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangeConstructorFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangeConstructorFunctions[7].magic = Range::NODE_INSIDE;
    JSRangeConstructorFunctions[7].u.getset.get.getter_magic = JSRangeConstructor::getValueProperty;
    JSRangeConstructorFunctions[7].u.getset.set.setter_magic = NULL;
}

JSValue JSRangeConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Range.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSRangeConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Range.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSRangeConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSRangeConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangeConstructorFunctions, countof(JSRangeConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSRangePrototypeConstantsFunctions[8];
static bool JSRangePrototypeConstantsFunctions_initialized = false;

static void init_JSRangePrototypeConstantsFunctions()
{
    if (JSRangePrototypeConstantsFunctions_initialized) return;
    JSRangePrototypeConstantsFunctions_initialized = true;
    memset(JSRangePrototypeConstantsFunctions, 0, sizeof(JSRangePrototypeConstantsFunctions));
    JSRangePrototypeConstantsFunctions[0].name = "START_TO_START";
    JSRangePrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangePrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangePrototypeConstantsFunctions[0].magic = Range::START_TO_START;
    JSRangePrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSRangePrototype::getValueProperty;
    JSRangePrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSRangePrototypeConstantsFunctions[1].name = "NODE_AFTER";
    JSRangePrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangePrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangePrototypeConstantsFunctions[1].magic = Range::NODE_AFTER;
    JSRangePrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSRangePrototype::getValueProperty;
    JSRangePrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSRangePrototypeConstantsFunctions[2].name = "NODE_BEFORE";
    JSRangePrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangePrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangePrototypeConstantsFunctions[2].magic = Range::NODE_BEFORE;
    JSRangePrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSRangePrototype::getValueProperty;
    JSRangePrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
    JSRangePrototypeConstantsFunctions[3].name = "START_TO_END";
    JSRangePrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangePrototypeConstantsFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangePrototypeConstantsFunctions[3].magic = Range::START_TO_END;
    JSRangePrototypeConstantsFunctions[3].u.getset.get.getter_magic = JSRangePrototype::getValueProperty;
    JSRangePrototypeConstantsFunctions[3].u.getset.set.setter_magic = NULL;
    JSRangePrototypeConstantsFunctions[4].name = "END_TO_END";
    JSRangePrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangePrototypeConstantsFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangePrototypeConstantsFunctions[4].magic = Range::END_TO_END;
    JSRangePrototypeConstantsFunctions[4].u.getset.get.getter_magic = JSRangePrototype::getValueProperty;
    JSRangePrototypeConstantsFunctions[4].u.getset.set.setter_magic = NULL;
    JSRangePrototypeConstantsFunctions[5].name = "END_TO_START";
    JSRangePrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangePrototypeConstantsFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangePrototypeConstantsFunctions[5].magic = Range::END_TO_START;
    JSRangePrototypeConstantsFunctions[5].u.getset.get.getter_magic = JSRangePrototype::getValueProperty;
    JSRangePrototypeConstantsFunctions[5].u.getset.set.setter_magic = NULL;
    JSRangePrototypeConstantsFunctions[6].name = "NODE_BEFORE_AND_AFTER";
    JSRangePrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangePrototypeConstantsFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangePrototypeConstantsFunctions[6].magic = Range::NODE_BEFORE_AND_AFTER;
    JSRangePrototypeConstantsFunctions[6].u.getset.get.getter_magic = JSRangePrototype::getValueProperty;
    JSRangePrototypeConstantsFunctions[6].u.getset.set.setter_magic = NULL;
    JSRangePrototypeConstantsFunctions[7].name = "NODE_INSIDE";
    JSRangePrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSRangePrototypeConstantsFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSRangePrototypeConstantsFunctions[7].magic = Range::NODE_INSIDE;
    JSRangePrototypeConstantsFunctions[7].u.getset.get.getter_magic = JSRangePrototype::getValueProperty;
    JSRangePrototypeConstantsFunctions[7].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSRangePrototypeFunctions[23];
static bool JSRangePrototypeFunctions_initialized = false;

static void init_JSRangePrototypeFunctions()
{
    if (JSRangePrototypeFunctions_initialized) return;
    JSRangePrototypeFunctions_initialized = true;
    memset(JSRangePrototypeFunctions, 0, sizeof(JSRangePrototypeFunctions));
    JSRangePrototypeFunctions[0].name = "toString";
    JSRangePrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[0].magic = JSRange::ToStringFuncNum;
    JSRangePrototypeFunctions[0].u.func.length = 0;
    JSRangePrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[0].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[1].name = "setEnd";
    JSRangePrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[1].magic = JSRange::SetEndFuncNum;
    JSRangePrototypeFunctions[1].u.func.length = 2;
    JSRangePrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[1].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[2].name = "setEndBefore";
    JSRangePrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[2].magic = JSRange::SetEndBeforeFuncNum;
    JSRangePrototypeFunctions[2].u.func.length = 1;
    JSRangePrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[2].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[3].name = "setStart";
    JSRangePrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[3].magic = JSRange::SetStartFuncNum;
    JSRangePrototypeFunctions[3].u.func.length = 2;
    JSRangePrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[3].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[4].name = "intersectsNode";
    JSRangePrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[4].magic = JSRange::IntersectsNodeFuncNum;
    JSRangePrototypeFunctions[4].u.func.length = 1;
    JSRangePrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[4].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[5].name = "surroundContents";
    JSRangePrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[5].magic = JSRange::SurroundContentsFuncNum;
    JSRangePrototypeFunctions[5].u.func.length = 1;
    JSRangePrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[5].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[6].name = "selectNode";
    JSRangePrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[6].magic = JSRange::SelectNodeFuncNum;
    JSRangePrototypeFunctions[6].u.func.length = 1;
    JSRangePrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[6].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[7].name = "setEndAfter";
    JSRangePrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[7].magic = JSRange::SetEndAfterFuncNum;
    JSRangePrototypeFunctions[7].u.func.length = 1;
    JSRangePrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[7].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[8].name = "collapse";
    JSRangePrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[8].magic = JSRange::CollapseFuncNum;
    JSRangePrototypeFunctions[8].u.func.length = 1;
    JSRangePrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[8].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[9].name = "setStartBefore";
    JSRangePrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[9].magic = JSRange::SetStartBeforeFuncNum;
    JSRangePrototypeFunctions[9].u.func.length = 1;
    JSRangePrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[9].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[10].name = "insertNode";
    JSRangePrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[10].magic = JSRange::InsertNodeFuncNum;
    JSRangePrototypeFunctions[10].u.func.length = 1;
    JSRangePrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[10].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[11].name = "setStartAfter";
    JSRangePrototypeFunctions[11].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[11].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[11].magic = JSRange::SetStartAfterFuncNum;
    JSRangePrototypeFunctions[11].u.func.length = 1;
    JSRangePrototypeFunctions[11].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[11].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[12].name = "compareBoundaryPoints";
    JSRangePrototypeFunctions[12].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[12].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[12].magic = JSRange::CompareBoundaryPointsFuncNum;
    JSRangePrototypeFunctions[12].u.func.length = 2;
    JSRangePrototypeFunctions[12].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[12].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[13].name = "selectNodeContents";
    JSRangePrototypeFunctions[13].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[13].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[13].magic = JSRange::SelectNodeContentsFuncNum;
    JSRangePrototypeFunctions[13].u.func.length = 1;
    JSRangePrototypeFunctions[13].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[13].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[14].name = "deleteContents";
    JSRangePrototypeFunctions[14].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[14].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[14].magic = JSRange::DeleteContentsFuncNum;
    JSRangePrototypeFunctions[14].u.func.length = 0;
    JSRangePrototypeFunctions[14].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[14].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[15].name = "extractContents";
    JSRangePrototypeFunctions[15].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[15].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[15].magic = JSRange::ExtractContentsFuncNum;
    JSRangePrototypeFunctions[15].u.func.length = 0;
    JSRangePrototypeFunctions[15].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[15].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[16].name = "cloneContents";
    JSRangePrototypeFunctions[16].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[16].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[16].magic = JSRange::CloneContentsFuncNum;
    JSRangePrototypeFunctions[16].u.func.length = 0;
    JSRangePrototypeFunctions[16].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[16].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[17].name = "cloneRange";
    JSRangePrototypeFunctions[17].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[17].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[17].magic = JSRange::CloneRangeFuncNum;
    JSRangePrototypeFunctions[17].u.func.length = 0;
    JSRangePrototypeFunctions[17].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[17].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[18].name = "detach";
    JSRangePrototypeFunctions[18].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[18].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[18].magic = JSRange::DetachFuncNum;
    JSRangePrototypeFunctions[18].u.func.length = 0;
    JSRangePrototypeFunctions[18].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[18].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[19].name = "createContextualFragment";
    JSRangePrototypeFunctions[19].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[19].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[19].magic = JSRange::CreateContextualFragmentFuncNum;
    JSRangePrototypeFunctions[19].u.func.length = 1;
    JSRangePrototypeFunctions[19].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[19].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[20].name = "compareNode";
    JSRangePrototypeFunctions[20].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[20].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[20].magic = JSRange::CompareNodeFuncNum;
    JSRangePrototypeFunctions[20].u.func.length = 1;
    JSRangePrototypeFunctions[20].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[20].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[21].name = "comparePoint";
    JSRangePrototypeFunctions[21].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[21].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[21].magic = JSRange::ComparePointFuncNum;
    JSRangePrototypeFunctions[21].u.func.length = 2;
    JSRangePrototypeFunctions[21].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[21].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
    JSRangePrototypeFunctions[22].name = "isPointInRange";
    JSRangePrototypeFunctions[22].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSRangePrototypeFunctions[22].def_type = JS_DEF_CFUNC;
    JSRangePrototypeFunctions[22].magic = JSRange::IsPointInRangeFuncNum;
    JSRangePrototypeFunctions[22].u.func.length = 2;
    JSRangePrototypeFunctions[22].u.func.cproto = JS_CFUNC_generic_magic;
    JSRangePrototypeFunctions[22].u.func.cfunc.generic_magic = JSRangePrototypeFunction::callAsFunction;
}

JSValue JSRangePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSRange.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSRangePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSRange.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSRangePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSRangeAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangeAttributesFunctions, countof(JSRangeAttributesFunctions));
    init_JSRangePrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangePrototypeConstantsFunctions, countof(JSRangePrototypeConstantsFunctions));
    init_JSRangePrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangePrototypeFunctions, countof(JSRangePrototypeFunctions));
}

JSValue JSRangePrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSRangeClassDefine;
static bool JSRangeClassDefine_initialized = false;

static void init_JSRangeClassDefine()
{
    if (JSRangeClassDefine_initialized) return;
    JSRangeClassDefine_initialized = true;
    memset(&JSRangeClassDefine, 0, sizeof(JSRangeClassDefine));
    JSRangeClassDefine.class_name = "Range";
    JSRangeClassDefine.finalizer = JSRange::finalizer;
    JSRangeClassDefine.gc_mark = JSRange::mark;
}

JSClassID JSRange::js_class_id = 0;

void JSRange::init(JSContext* ctx)
{
    if (JSRange::js_class_id == 0) {
        init_JSRangeClassDefine();
        JS_NewClassID(&JSRange::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSRange::js_class_id, &JSRangeClassDefine);
        JS_SetConstructor(ctx, JSRangeConstructor::self(ctx), JSRangePrototype::self(ctx));
        JS_SetClassProto(ctx, JSRange::js_class_id, JSRangePrototype::self(ctx));
    }
}

JSValue JSRange::create(JSContext* ctx, Range* impl)
{
    JSRange::init(ctx);
    JSValue _proto = JSRangePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSRange::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSRange::finalizer(JSRuntime* rt, JSValue val)
{
    Range* impl = (Range*)JS_GetOpaque(val, JSRange::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSRange::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSRange::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case StartContainerAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque(this_val, JSRange::js_class_id);
            JSValue result = toJS(ctx, QJS::getPtr(imp->startContainer(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case StartOffsetAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque(this_val, JSRange::js_class_id);
            JSValue result = JS_NewInt32(ctx, imp->startOffset(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case EndContainerAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque(this_val, JSRange::js_class_id);
            JSValue result = toJS(ctx, QJS::getPtr(imp->endContainer(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case EndOffsetAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque(this_val, JSRange::js_class_id);
            JSValue result = JS_NewInt32(ctx, imp->endOffset(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case CollapsedAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque(this_val, JSRange::js_class_id);
            JSValue result = JS_NewBool(ctx, imp->collapsed(ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
        case CommonAncestorContainerAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque(this_val, JSRange::js_class_id);
            JSValue result = toJS(ctx, QJS::getPtr(imp->commonAncestorContainer(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSRange::getConstructor(JSContext *ctx)
{
    return JSRangeConstructor::self(ctx);
}

JSValue JSRangePrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Range* imp = (Range*)JS_GetOpaque(this_val, JSRange::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSRange::SetStartFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setStart(refNode, offset, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetEndFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setEnd(refNode, offset, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetStartBeforeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->setStartBefore(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetStartAfterFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->setStartAfter(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetEndBeforeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->setEndBefore(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetEndAfterFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->setEndAfter(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::CollapseFuncNum: {
            ExceptionCode ec = 0;
            bool toStart = valueToBoolean(ctx, argv[0]);
            imp->collapse(toStart, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SelectNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->selectNode(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SelectNodeContentsFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->selectNodeContents(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::CompareBoundaryPointsFuncNum: {
            ExceptionCode ec = 0;
            Range::CompareHow how = static_cast<Range::CompareHow>(valueToInt32(ctx, argv[0]));
            Range* sourceRange = toRange(argv[1]);

            JSValue result = JS_NewInt32(ctx, imp->compareBoundaryPoints(how, sourceRange, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::DeleteContentsFuncNum: {
            ExceptionCode ec = 0;
            imp->deleteContents(ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::ExtractContentsFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->extractContents(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::CloneContentsFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->cloneContents(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::InsertNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* newNode = toNode(argv[0]);
            imp->insertNode(newNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SurroundContentsFuncNum: {
            ExceptionCode ec = 0;
            Node* newParent = toNode(argv[0]);
            imp->surroundContents(newParent, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::CloneRangeFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->cloneRange(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::ToStringFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = JS_NewString(ctx, ((const String&)imp->toString(ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::DetachFuncNum: {
            ExceptionCode ec = 0;
            imp->detach(ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::CreateContextualFragmentFuncNum: {
            ExceptionCode ec = 0;
            String html = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createContextualFragment(html, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::IntersectsNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);

            JSValue result = JS_NewBool(ctx, imp->intersectsNode(refNode, ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::CompareNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);

            JSValue result = JS_NewInt32(ctx, imp->compareNode(refNode, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::ComparePointFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewInt32(ctx, imp->comparePoint(refNode, offset, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::IsPointInRangeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewBool(ctx, imp->isPointInRange(refNode, offset, ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, Range* obj)
{
    return QJS::cacheDOMObject<Range, JSRange>(ctx, obj);
}

Range* toRange(JSValue val)
{
    if (JS_IsObject(val)) {
        Range* impl = (Range*)JS_GetOpaque(val, JSRange::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
