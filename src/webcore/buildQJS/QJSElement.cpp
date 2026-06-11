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

#include "QJSElement.h"

#include "Attr.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "DOMTokenList.h"
#include "Element.h"
#include "ExceptionCode.h"
#include "NameNodeList.h"
#include "Node.h"
#include "NodeList.h"
#include "QJSAttr.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSDOMTokenList.h"
#include "QJSElement.h"
#include "QJSNode.h"
#include "QJSNodeList.h"
#include "qjs_dom.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSElementAttributesFunctions[17];
static bool JSElementAttributesFunctions_initialized = false;

static void init_JSElementAttributesFunctions()
{
    if (JSElementAttributesFunctions_initialized) return;
    JSElementAttributesFunctions_initialized = true;
    memset(JSElementAttributesFunctions, 0, sizeof(JSElementAttributesFunctions));
    JSElementAttributesFunctions[0].name = "tagName";
    JSElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[0].magic = JSElement::TagNameAttrNum;
    JSElementAttributesFunctions[0].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[1].name = "classList";
    JSElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[1].magic = JSElement::ClassListAttrNum;
    JSElementAttributesFunctions[1].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[2].name = "style";
    JSElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[2].magic = JSElement::StyleAttrNum;
    JSElementAttributesFunctions[2].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[3].name = "offsetLeft";
    JSElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[3].magic = JSElement::OffsetLeftAttrNum;
    JSElementAttributesFunctions[3].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[4].name = "offsetTop";
    JSElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[4].magic = JSElement::OffsetTopAttrNum;
    JSElementAttributesFunctions[4].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[5].name = "offsetWidth";
    JSElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[5].magic = JSElement::OffsetWidthAttrNum;
    JSElementAttributesFunctions[5].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[6].name = "offsetHeight";
    JSElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[6].magic = JSElement::OffsetHeightAttrNum;
    JSElementAttributesFunctions[6].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[7].name = "offsetParent";
    JSElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[7].magic = JSElement::OffsetParentAttrNum;
    JSElementAttributesFunctions[7].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[8].name = "clientLeft";
    JSElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[8].magic = JSElement::ClientLeftAttrNum;
    JSElementAttributesFunctions[8].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[9].name = "clientTop";
    JSElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[9].magic = JSElement::ClientTopAttrNum;
    JSElementAttributesFunctions[9].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[10].name = "clientWidth";
    JSElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[10].magic = JSElement::ClientWidthAttrNum;
    JSElementAttributesFunctions[10].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[11].name = "clientHeight";
    JSElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[11].magic = JSElement::ClientHeightAttrNum;
    JSElementAttributesFunctions[11].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[12].name = "scrollLeft";
    JSElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[12].magic = JSElement::ScrollLeftAttrNum;
    JSElementAttributesFunctions[12].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[12].u.getset.set.setter_magic = JSElement::putValueProperty;
    JSElementAttributesFunctions[13].name = "scrollTop";
    JSElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[13].magic = JSElement::ScrollTopAttrNum;
    JSElementAttributesFunctions[13].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[13].u.getset.set.setter_magic = JSElement::putValueProperty;
    JSElementAttributesFunctions[14].name = "scrollWidth";
    JSElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[14].magic = JSElement::ScrollWidthAttrNum;
    JSElementAttributesFunctions[14].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[15].name = "scrollHeight";
    JSElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[15].magic = JSElement::ScrollHeightAttrNum;
    JSElementAttributesFunctions[15].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSElementAttributesFunctions[16].name = "constructor";
    JSElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSElementAttributesFunctions[16].magic = JSElement::ConstructorAttrNum;
    JSElementAttributesFunctions[16].u.getset.get.getter_magic = JSElement::getValueProperty;
    JSElementAttributesFunctions[16].u.getset.set.setter_magic = NULL;
}

class JSElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Element.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Element.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSElementPrototypeFunctions[25];
static bool JSElementPrototypeFunctions_initialized = false;

static void init_JSElementPrototypeFunctions()
{
    if (JSElementPrototypeFunctions_initialized) return;
    JSElementPrototypeFunctions_initialized = true;
    memset(JSElementPrototypeFunctions, 0, sizeof(JSElementPrototypeFunctions));
    JSElementPrototypeFunctions[0].name = "getAttribute";
    JSElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[0].magic = JSElement::GetAttributeFuncNum;
    JSElementPrototypeFunctions[0].u.func.length = 1;
    JSElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[1].name = "setAttribute";
    JSElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[1].magic = JSElement::SetAttributeFuncNum;
    JSElementPrototypeFunctions[1].u.func.length = 2;
    JSElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[2].name = "removeAttribute";
    JSElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[2].magic = JSElement::RemoveAttributeFuncNum;
    JSElementPrototypeFunctions[2].u.func.length = 1;
    JSElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[3].name = "getAttributeNode";
    JSElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[3].magic = JSElement::GetAttributeNodeFuncNum;
    JSElementPrototypeFunctions[3].u.func.length = 1;
    JSElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[4].name = "setAttributeNode";
    JSElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[4].magic = JSElement::SetAttributeNodeFuncNum;
    JSElementPrototypeFunctions[4].u.func.length = 1;
    JSElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[5].name = "removeAttributeNode";
    JSElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[5].magic = JSElement::RemoveAttributeNodeFuncNum;
    JSElementPrototypeFunctions[5].u.func.length = 1;
    JSElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[6].name = "getElementsByTagName";
    JSElementPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[6].magic = JSElement::GetElementsByTagNameFuncNum;
    JSElementPrototypeFunctions[6].u.func.length = 1;
    JSElementPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[6].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[7].name = "querySelector";
    JSElementPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[7].magic = JSElement::QuerySelectorFuncNum;
    JSElementPrototypeFunctions[7].u.func.length = 1;
    JSElementPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[7].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[8].name = "querySelectorAll";
    JSElementPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[8].magic = JSElement::QuerySelectorAllFuncNum;
    JSElementPrototypeFunctions[8].u.func.length = 1;
    JSElementPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[8].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[9].name = "getAttributeNS";
    JSElementPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[9].magic = JSElement::GetAttributeNSFuncNum;
    JSElementPrototypeFunctions[9].u.func.length = 2;
    JSElementPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[9].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[10].name = "setAttributeNS";
    JSElementPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[10].magic = JSElement::SetAttributeNSFuncNum;
    JSElementPrototypeFunctions[10].u.func.length = 3;
    JSElementPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[10].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[11].name = "removeAttributeNS";
    JSElementPrototypeFunctions[11].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[11].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[11].magic = JSElement::RemoveAttributeNSFuncNum;
    JSElementPrototypeFunctions[11].u.func.length = 2;
    JSElementPrototypeFunctions[11].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[11].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[12].name = "getElementsByTagNameNS";
    JSElementPrototypeFunctions[12].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[12].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[12].magic = JSElement::GetElementsByTagNameNSFuncNum;
    JSElementPrototypeFunctions[12].u.func.length = 2;
    JSElementPrototypeFunctions[12].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[12].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[13].name = "getAttributeNodeNS";
    JSElementPrototypeFunctions[13].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[13].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[13].magic = JSElement::GetAttributeNodeNSFuncNum;
    JSElementPrototypeFunctions[13].u.func.length = 2;
    JSElementPrototypeFunctions[13].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[13].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[14].name = "setAttributeNodeNS";
    JSElementPrototypeFunctions[14].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[14].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[14].magic = JSElement::SetAttributeNodeNSFuncNum;
    JSElementPrototypeFunctions[14].u.func.length = 1;
    JSElementPrototypeFunctions[14].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[14].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[15].name = "hasAttribute";
    JSElementPrototypeFunctions[15].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[15].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[15].magic = JSElement::HasAttributeFuncNum;
    JSElementPrototypeFunctions[15].u.func.length = 1;
    JSElementPrototypeFunctions[15].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[15].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[16].name = "hasAttributeNS";
    JSElementPrototypeFunctions[16].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[16].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[16].magic = JSElement::HasAttributeNSFuncNum;
    JSElementPrototypeFunctions[16].u.func.length = 2;
    JSElementPrototypeFunctions[16].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[16].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[17].name = "focus";
    JSElementPrototypeFunctions[17].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[17].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[17].magic = JSElement::FocusFuncNum;
    JSElementPrototypeFunctions[17].u.func.length = 0;
    JSElementPrototypeFunctions[17].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[17].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[18].name = "blur";
    JSElementPrototypeFunctions[18].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[18].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[18].magic = JSElement::BlurFuncNum;
    JSElementPrototypeFunctions[18].u.func.length = 0;
    JSElementPrototypeFunctions[18].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[18].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[19].name = "scrollIntoView";
    JSElementPrototypeFunctions[19].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[19].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[19].magic = JSElement::ScrollIntoViewFuncNum;
    JSElementPrototypeFunctions[19].u.func.length = 1;
    JSElementPrototypeFunctions[19].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[19].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[20].name = "insertAdjacentElement";
    JSElementPrototypeFunctions[20].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[20].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[20].magic = JSElement::InsertAdjacentElementFuncNum;
    JSElementPrototypeFunctions[20].u.func.length = 2;
    JSElementPrototypeFunctions[20].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[20].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[21].name = "contains";
    JSElementPrototypeFunctions[21].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[21].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[21].magic = JSElement::ContainsFuncNum;
    JSElementPrototypeFunctions[21].u.func.length = 1;
    JSElementPrototypeFunctions[21].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[21].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[22].name = "scrollIntoViewIfNeeded";
    JSElementPrototypeFunctions[22].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[22].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[22].magic = JSElement::ScrollIntoViewIfNeededFuncNum;
    JSElementPrototypeFunctions[22].u.func.length = 1;
    JSElementPrototypeFunctions[22].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[22].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[23].name = "scrollByLines";
    JSElementPrototypeFunctions[23].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[23].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[23].magic = JSElement::ScrollByLinesFuncNum;
    JSElementPrototypeFunctions[23].u.func.length = 1;
    JSElementPrototypeFunctions[23].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[23].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
    JSElementPrototypeFunctions[24].name = "scrollByPages";
    JSElementPrototypeFunctions[24].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSElementPrototypeFunctions[24].def_type = JS_DEF_CFUNC;
    JSElementPrototypeFunctions[24].magic = JSElement::ScrollByPagesFuncNum;
    JSElementPrototypeFunctions[24].u.func.length = 1;
    JSElementPrototypeFunctions[24].u.func.cproto = JS_CFUNC_generic_magic;
    JSElementPrototypeFunctions[24].u.func.cfunc.generic_magic = JSElementPrototypeFunction::callAsFunction;
}

JSValue JSElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSElementAttributesFunctions, countof(JSElementAttributesFunctions));
    init_JSElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSElementPrototypeFunctions, countof(JSElementPrototypeFunctions));
}

JSClassID JSElement::js_class_id = 0;

void JSElement::init(JSContext* ctx)
{
    if (JSElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSElement::create(JSContext* ctx, Element* impl)
{
    JSElement::init(ctx);
    JSValue _proto = JSElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSElement::finalizer(JSRuntime* rt, JSValue val)
{
    Element* impl = (Element*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSEventTargetNode::mark(rt, val, mark_func);
}

JSValue JSElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TagNameAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->tagName());
        }
        case ClassListAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->classList()));
        }
        case StyleAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case OffsetLeftAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->offsetLeft());
        }
        case OffsetTopAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->offsetTop());
        }
        case OffsetWidthAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->offsetWidth());
        }
        case OffsetHeightAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->offsetHeight());
        }
        case OffsetParentAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->offsetParent()));
        }
        case ClientLeftAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->clientLeft());
        }
        case ClientTopAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->clientTop());
        }
        case ClientWidthAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->clientWidth());
        }
        case ClientHeightAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->clientHeight());
        }
        case ScrollLeftAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->scrollLeft());
        }
        case ScrollTopAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->scrollTop());
        }
        case ScrollWidthAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->scrollWidth());
        }
        case ScrollHeightAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->scrollHeight());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ScrollLeftAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setScrollLeft(valueToInt32(ctx, value));
            break;
        }
        case ScrollTopAttrNum: {
            Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setScrollTop(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSElement::getConstructor(JSContext *ctx)
{
    return JSElementConstructor::self(ctx);
}

JSValue JSElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Element* imp = (Element*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSElement::GetAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->getAttribute(name));
            return result;
        }
        case JSElement::SetAttributeFuncNum: {
            return JSElement::setAttribute(ctx, this_val, argc, argv, imp);
        }
        case JSElement::RemoveAttributeFuncNum: {
            ExceptionCode ec = 0;
            String name = valueToString(ctx, argv[0]);
            imp->removeAttribute(name, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSElement::GetAttributeNodeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getAttributeNode(name)));
            return result;
        }
        case JSElement::SetAttributeNodeFuncNum: {
            return JSElement::setAttributeNode(ctx, this_val, argc, argv, imp);
        }
        case JSElement::RemoveAttributeNodeFuncNum: {
            ExceptionCode ec = 0;
            bool oldAttrOk;
            Attr* oldAttr = toAttr(argv[0], oldAttrOk);
            if (!oldAttrOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->removeAttributeNode(oldAttr, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSElement::GetElementsByTagNameFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getElementsByTagName(name)));
            return result;
        }
        case JSElement::QuerySelectorFuncNum: {
            ExceptionCode ec = 0;
            String selectors = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->querySelector(selectors, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSElement::QuerySelectorAllFuncNum: {
            ExceptionCode ec = 0;
            String selectors = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->querySelectorAll(selectors, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSElement::GetAttributeNSFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);

            JSValue result = JS_NewString(ctx, ((const String&)imp->getAttributeNS(namespaceURI, localName)).utf8().data());
            return result;
        }
        case JSElement::SetAttributeNSFuncNum: {
            return JSElement::setAttributeNS(ctx, this_val, argc, argv, imp);
        }
        case JSElement::RemoveAttributeNSFuncNum: {
            ExceptionCode ec = 0;
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);
            imp->removeAttributeNS(namespaceURI, localName, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSElement::GetElementsByTagNameNSFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getElementsByTagNameNS(namespaceURI, localName)));
            return result;
        }
        case JSElement::GetAttributeNodeNSFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getAttributeNodeNS(namespaceURI, localName)));
            return result;
        }
        case JSElement::SetAttributeNodeNSFuncNum: {
            return JSElement::setAttributeNodeNS(ctx, this_val, argc, argv, imp);
        }
        case JSElement::HasAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasAttribute(name) ? 1 : 0);
            return result;
        }
        case JSElement::HasAttributeNSFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);

            JSValue result = JS_NewBool(ctx, imp->hasAttributeNS(namespaceURI, localName) ? 1 : 0);
            return result;
        }
        case JSElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
        case JSElement::BlurFuncNum: {
            imp->blur();
            return JS_UNDEFINED;
        }
        case JSElement::ScrollIntoViewFuncNum: {
            if (argc < 1) {
                  imp->scrollIntoView();
                  return JS_UNDEFINED;
            }

            bool alignWithTop = valueToBoolean(ctx, argv[0]);
            imp->scrollIntoView(alignWithTop);
            return JS_UNDEFINED;
        }
        case JSElement::InsertAdjacentElementFuncNum: {
            ExceptionCode ec = 0;
            String position = valueToString(ctx, argv[0]);
            Node* element = toNode(argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->insertAdjacentElement(position, element, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSElement::ContainsFuncNum: {
            Element* element = toElement(argv[0]);

            JSValue result = JS_NewBool(ctx, imp->contains(element) ? 1 : 0);
            return result;
        }
        case JSElement::ScrollIntoViewIfNeededFuncNum: {
            if (argc < 1) {
                  imp->scrollIntoViewIfNeeded();
                  return JS_UNDEFINED;
            }

            bool centerIfNeeded = valueToBoolean(ctx, argv[0]);
            imp->scrollIntoViewIfNeeded(centerIfNeeded);
            return JS_UNDEFINED;
        }
        case JSElement::ScrollByLinesFuncNum: {
            bool linesOk;
            int lines = valueToInt32(ctx, argv[0], linesOk);
            if (!linesOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->scrollByLines(lines);
            return JS_UNDEFINED;
        }
        case JSElement::ScrollByPagesFuncNum: {
            bool pagesOk;
            int pages = valueToInt32(ctx, argv[0], pagesOk);
            if (!pagesOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->scrollByPages(pages);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


Element* toElement(JSValue val)
{
    if (JS_IsObject(val)) {
        Element* impl = (Element*)JS_GetOpaque(val, JSElement::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
