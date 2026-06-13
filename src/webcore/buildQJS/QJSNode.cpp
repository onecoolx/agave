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

#include "QJSNode.h"

#include "Document.h"
#include "NameNodeList.h"
#include "NamedAttrMap.h"
#include "NamedNodeMap.h"
#include "Node.h"
#include "NodeList.h"
#include "QJSDocument.h"
#include "QJSNamedNodeMap.h"
#include "QJSNode.h"
#include "QJSNodeList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSNodeAttributesFunctions[18];
static bool JSNodeAttributesFunctions_initialized = false;

static void init_JSNodeAttributesFunctions()
{
    if (JSNodeAttributesFunctions_initialized) return;
    JSNodeAttributesFunctions_initialized = true;
    memset(JSNodeAttributesFunctions, 0, sizeof(JSNodeAttributesFunctions));
    JSNodeAttributesFunctions[0].name = "nodeName";
    JSNodeAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[0].magic = JSNode::NodeNameAttrNum;
    JSNodeAttributesFunctions[0].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[1].name = "nodeValue";
    JSNodeAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[1].magic = JSNode::NodeValueAttrNum;
    JSNodeAttributesFunctions[1].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[1].u.getset.set.setter_magic = JSNode::putValueProperty;
    JSNodeAttributesFunctions[2].name = "nodeType";
    JSNodeAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[2].magic = JSNode::NodeTypeAttrNum;
    JSNodeAttributesFunctions[2].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[3].name = "parentNode";
    JSNodeAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[3].magic = JSNode::ParentNodeAttrNum;
    JSNodeAttributesFunctions[3].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[4].name = "childNodes";
    JSNodeAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[4].magic = JSNode::ChildNodesAttrNum;
    JSNodeAttributesFunctions[4].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[5].name = "firstChild";
    JSNodeAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[5].magic = JSNode::FirstChildAttrNum;
    JSNodeAttributesFunctions[5].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[6].name = "lastChild";
    JSNodeAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[6].magic = JSNode::LastChildAttrNum;
    JSNodeAttributesFunctions[6].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[7].name = "previousSibling";
    JSNodeAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[7].magic = JSNode::PreviousSiblingAttrNum;
    JSNodeAttributesFunctions[7].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[8].name = "nextSibling";
    JSNodeAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[8].magic = JSNode::NextSiblingAttrNum;
    JSNodeAttributesFunctions[8].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[9].name = "attributes";
    JSNodeAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[9].magic = JSNode::AttributesAttrNum;
    JSNodeAttributesFunctions[9].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[10].name = "ownerDocument";
    JSNodeAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[10].magic = JSNode::OwnerDocumentAttrNum;
    JSNodeAttributesFunctions[10].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[11].name = "namespaceURI";
    JSNodeAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[11].magic = JSNode::NamespaceURIAttrNum;
    JSNodeAttributesFunctions[11].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[12].name = "prefix";
    JSNodeAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[12].magic = JSNode::PrefixAttrNum;
    JSNodeAttributesFunctions[12].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[12].u.getset.set.setter_magic = JSNode::putValueProperty;
    JSNodeAttributesFunctions[13].name = "localName";
    JSNodeAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[13].magic = JSNode::LocalNameAttrNum;
    JSNodeAttributesFunctions[13].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[14].name = "baseURI";
    JSNodeAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[14].magic = JSNode::BaseURIAttrNum;
    JSNodeAttributesFunctions[14].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[15].name = "textContent";
    JSNodeAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[15].magic = JSNode::TextContentAttrNum;
    JSNodeAttributesFunctions[15].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[15].u.getset.set.setter_magic = JSNode::putValueProperty;
    JSNodeAttributesFunctions[16].name = "parentElement";
    JSNodeAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[16].magic = JSNode::ParentElementAttrNum;
    JSNodeAttributesFunctions[16].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[16].u.getset.set.setter_magic = NULL;
    JSNodeAttributesFunctions[17].name = "constructor";
    JSNodeAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeAttributesFunctions[17].magic = JSNode::ConstructorAttrNum;
    JSNodeAttributesFunctions[17].u.getset.get.getter_magic = JSNode::getValueProperty;
    JSNodeAttributesFunctions[17].u.getset.set.setter_magic = NULL;
}

class JSNodeConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSNodeConstructorFunctions[12];
static bool JSNodeConstructorFunctions_initialized = false;

static void init_JSNodeConstructorFunctions()
{
    if (JSNodeConstructorFunctions_initialized) return;
    JSNodeConstructorFunctions_initialized = true;
    memset(JSNodeConstructorFunctions, 0, sizeof(JSNodeConstructorFunctions));
    JSNodeConstructorFunctions[0].name = "ELEMENT_NODE";
    JSNodeConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[0].u.i32 = (int32_t)Node::ELEMENT_NODE;
    JSNodeConstructorFunctions[1].name = "ATTRIBUTE_NODE";
    JSNodeConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[1].u.i32 = (int32_t)Node::ATTRIBUTE_NODE;
    JSNodeConstructorFunctions[2].name = "TEXT_NODE";
    JSNodeConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[2].u.i32 = (int32_t)Node::TEXT_NODE;
    JSNodeConstructorFunctions[3].name = "CDATA_SECTION_NODE";
    JSNodeConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[3].u.i32 = (int32_t)Node::CDATA_SECTION_NODE;
    JSNodeConstructorFunctions[4].name = "ENTITY_REFERENCE_NODE";
    JSNodeConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[4].u.i32 = (int32_t)Node::ENTITY_REFERENCE_NODE;
    JSNodeConstructorFunctions[5].name = "ENTITY_NODE";
    JSNodeConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[5].u.i32 = (int32_t)Node::ENTITY_NODE;
    JSNodeConstructorFunctions[6].name = "PROCESSING_INSTRUCTION_NODE";
    JSNodeConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[6].u.i32 = (int32_t)Node::PROCESSING_INSTRUCTION_NODE;
    JSNodeConstructorFunctions[7].name = "COMMENT_NODE";
    JSNodeConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[7].u.i32 = (int32_t)Node::COMMENT_NODE;
    JSNodeConstructorFunctions[8].name = "DOCUMENT_NODE";
    JSNodeConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[8].u.i32 = (int32_t)Node::DOCUMENT_NODE;
    JSNodeConstructorFunctions[9].name = "DOCUMENT_TYPE_NODE";
    JSNodeConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[9].u.i32 = (int32_t)Node::DOCUMENT_TYPE_NODE;
    JSNodeConstructorFunctions[10].name = "DOCUMENT_FRAGMENT_NODE";
    JSNodeConstructorFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[10].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[10].u.i32 = (int32_t)Node::DOCUMENT_FRAGMENT_NODE;
    JSNodeConstructorFunctions[11].name = "NOTATION_NODE";
    JSNodeConstructorFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeConstructorFunctions[11].def_type = JS_DEF_PROP_INT32;
    JSNodeConstructorFunctions[11].u.i32 = (int32_t)Node::NOTATION_NODE;
}

JSValue JSNodeConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Node.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Node.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNodeConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSNodeConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeConstructorFunctions, countof(JSNodeConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSNodePrototypeConstantsFunctions[12];
static bool JSNodePrototypeConstantsFunctions_initialized = false;

static void init_JSNodePrototypeConstantsFunctions()
{
    if (JSNodePrototypeConstantsFunctions_initialized) return;
    JSNodePrototypeConstantsFunctions_initialized = true;
    memset(JSNodePrototypeConstantsFunctions, 0, sizeof(JSNodePrototypeConstantsFunctions));
    JSNodePrototypeConstantsFunctions[0].name = "ELEMENT_NODE";
    JSNodePrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[0].u.i32 = (int32_t)Node::ELEMENT_NODE;
    JSNodePrototypeConstantsFunctions[1].name = "ATTRIBUTE_NODE";
    JSNodePrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[1].u.i32 = (int32_t)Node::ATTRIBUTE_NODE;
    JSNodePrototypeConstantsFunctions[2].name = "TEXT_NODE";
    JSNodePrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[2].u.i32 = (int32_t)Node::TEXT_NODE;
    JSNodePrototypeConstantsFunctions[3].name = "CDATA_SECTION_NODE";
    JSNodePrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[3].u.i32 = (int32_t)Node::CDATA_SECTION_NODE;
    JSNodePrototypeConstantsFunctions[4].name = "ENTITY_REFERENCE_NODE";
    JSNodePrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[4].u.i32 = (int32_t)Node::ENTITY_REFERENCE_NODE;
    JSNodePrototypeConstantsFunctions[5].name = "ENTITY_NODE";
    JSNodePrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[5].u.i32 = (int32_t)Node::ENTITY_NODE;
    JSNodePrototypeConstantsFunctions[6].name = "PROCESSING_INSTRUCTION_NODE";
    JSNodePrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[6].u.i32 = (int32_t)Node::PROCESSING_INSTRUCTION_NODE;
    JSNodePrototypeConstantsFunctions[7].name = "COMMENT_NODE";
    JSNodePrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[7].u.i32 = (int32_t)Node::COMMENT_NODE;
    JSNodePrototypeConstantsFunctions[8].name = "DOCUMENT_NODE";
    JSNodePrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[8].u.i32 = (int32_t)Node::DOCUMENT_NODE;
    JSNodePrototypeConstantsFunctions[9].name = "DOCUMENT_TYPE_NODE";
    JSNodePrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[9].u.i32 = (int32_t)Node::DOCUMENT_TYPE_NODE;
    JSNodePrototypeConstantsFunctions[10].name = "DOCUMENT_FRAGMENT_NODE";
    JSNodePrototypeConstantsFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[10].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[10].u.i32 = (int32_t)Node::DOCUMENT_FRAGMENT_NODE;
    JSNodePrototypeConstantsFunctions[11].name = "NOTATION_NODE";
    JSNodePrototypeConstantsFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodePrototypeConstantsFunctions[11].def_type = JS_DEF_PROP_INT32;
    JSNodePrototypeConstantsFunctions[11].u.i32 = (int32_t)Node::NOTATION_NODE;
}

/* Prototype functions table */

static JSCFunctionListEntry JSNodePrototypeFunctions[17];
static bool JSNodePrototypeFunctions_initialized = false;

static void init_JSNodePrototypeFunctions()
{
    if (JSNodePrototypeFunctions_initialized) return;
    JSNodePrototypeFunctions_initialized = true;
    memset(JSNodePrototypeFunctions, 0, sizeof(JSNodePrototypeFunctions));
    JSNodePrototypeFunctions[0].name = "insertBefore";
    JSNodePrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[0].magic = JSNode::InsertBeforeFuncNum;
    JSNodePrototypeFunctions[0].u.func.length = 2;
    JSNodePrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[0].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[1].name = "replaceChild";
    JSNodePrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[1].magic = JSNode::ReplaceChildFuncNum;
    JSNodePrototypeFunctions[1].u.func.length = 2;
    JSNodePrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[1].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[2].name = "removeChild";
    JSNodePrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[2].magic = JSNode::RemoveChildFuncNum;
    JSNodePrototypeFunctions[2].u.func.length = 1;
    JSNodePrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[2].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[3].name = "appendChild";
    JSNodePrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[3].magic = JSNode::AppendChildFuncNum;
    JSNodePrototypeFunctions[3].u.func.length = 1;
    JSNodePrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[3].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[4].name = "hasChildNodes";
    JSNodePrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[4].magic = JSNode::HasChildNodesFuncNum;
    JSNodePrototypeFunctions[4].u.func.length = 0;
    JSNodePrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[4].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[5].name = "cloneNode";
    JSNodePrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[5].magic = JSNode::CloneNodeFuncNum;
    JSNodePrototypeFunctions[5].u.func.length = 1;
    JSNodePrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[5].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[6].name = "normalize";
    JSNodePrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[6].magic = JSNode::NormalizeFuncNum;
    JSNodePrototypeFunctions[6].u.func.length = 0;
    JSNodePrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[6].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[7].name = "isSupported";
    JSNodePrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[7].magic = JSNode::IsSupportedFuncNum;
    JSNodePrototypeFunctions[7].u.func.length = 2;
    JSNodePrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[7].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[8].name = "hasAttributes";
    JSNodePrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[8].magic = JSNode::HasAttributesFuncNum;
    JSNodePrototypeFunctions[8].u.func.length = 0;
    JSNodePrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[8].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[9].name = "isSameNode";
    JSNodePrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[9].magic = JSNode::IsSameNodeFuncNum;
    JSNodePrototypeFunctions[9].u.func.length = 1;
    JSNodePrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[9].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[10].name = "isEqualNode";
    JSNodePrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[10].magic = JSNode::IsEqualNodeFuncNum;
    JSNodePrototypeFunctions[10].u.func.length = 1;
    JSNodePrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[10].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[11].name = "lookupPrefix";
    JSNodePrototypeFunctions[11].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[11].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[11].magic = JSNode::LookupPrefixFuncNum;
    JSNodePrototypeFunctions[11].u.func.length = 1;
    JSNodePrototypeFunctions[11].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[11].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[12].name = "isDefaultNamespace";
    JSNodePrototypeFunctions[12].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[12].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[12].magic = JSNode::IsDefaultNamespaceFuncNum;
    JSNodePrototypeFunctions[12].u.func.length = 1;
    JSNodePrototypeFunctions[12].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[12].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[13].name = "lookupNamespaceURI";
    JSNodePrototypeFunctions[13].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[13].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[13].magic = JSNode::LookupNamespaceURIFuncNum;
    JSNodePrototypeFunctions[13].u.func.length = 1;
    JSNodePrototypeFunctions[13].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[13].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[14].name = "addEventListener";
    JSNodePrototypeFunctions[14].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[14].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[14].magic = JSNode::AddEventListenerFuncNum;
    JSNodePrototypeFunctions[14].u.func.length = 3;
    JSNodePrototypeFunctions[14].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[14].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[15].name = "removeEventListener";
    JSNodePrototypeFunctions[15].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[15].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[15].magic = JSNode::RemoveEventListenerFuncNum;
    JSNodePrototypeFunctions[15].u.func.length = 3;
    JSNodePrototypeFunctions[15].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[15].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
    JSNodePrototypeFunctions[16].name = "dispatchEvent";
    JSNodePrototypeFunctions[16].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodePrototypeFunctions[16].def_type = JS_DEF_CFUNC;
    JSNodePrototypeFunctions[16].magic = JSNode::DispatchEventFuncNum;
    JSNodePrototypeFunctions[16].u.func.length = 1;
    JSNodePrototypeFunctions[16].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodePrototypeFunctions[16].u.func.cfunc.generic_magic = JSNodePrototypeFunction::callAsFunction;
}

JSValue JSNodePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNode.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSNodePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNode.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNodePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSNodeAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeAttributesFunctions, countof(JSNodeAttributesFunctions));
    init_JSNodePrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodePrototypeConstantsFunctions, countof(JSNodePrototypeConstantsFunctions));
    init_JSNodePrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodePrototypeFunctions, countof(JSNodePrototypeFunctions));
}

static JSClassDef JSNodeClassDefine;
static bool JSNodeClassDefine_initialized = false;

static void init_JSNodeClassDefine()
{
    if (JSNodeClassDefine_initialized) return;
    JSNodeClassDefine_initialized = true;
    memset(&JSNodeClassDefine, 0, sizeof(JSNodeClassDefine));
    JSNodeClassDefine.class_name = "Node";
    JSNodeClassDefine.finalizer = JSNode::finalizer;
    JSNodeClassDefine.gc_mark = JSNode::mark;
}

JSClassID JSNode::js_class_id = 0;

void JSNode::init(JSContext* ctx)
{
    if (JSNode::js_class_id == 0) {
        init_JSNodeClassDefine();
        JS_NewClassID(&JSNode::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNode::js_class_id, &JSNodeClassDefine);
        JS_SetConstructor(ctx, JSNodeConstructor::self(ctx), JSNodePrototype::self(ctx));
        JS_SetClassProto(ctx, JSNode::js_class_id, JSNodePrototype::self(ctx));
    }
}

JSValue JSNode::create(JSContext* ctx, Node* impl)
{
    JSNode::init(ctx);
    JSValue _proto = JSNodePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSNode::finalizer(JSRuntime* rt, JSValue val)
{
    Node* impl = (Node*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

JSValue JSNode::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NodeNameAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->nodeName());
        }
        case NodeValueAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->nodeValue());
        }
        case NodeTypeAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->nodeType());
        }
        case ParentNodeAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentNode()));
        }
        case ChildNodesAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->childNodes()));
        }
        case FirstChildAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->firstChild()));
        }
        case LastChildAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->lastChild()));
        }
        case PreviousSiblingAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->previousSibling()));
        }
        case NextSiblingAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nextSibling()));
        }
        case AttributesAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->attributes()));
        }
        case OwnerDocumentAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerDocument()));
        }
        case NamespaceURIAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->namespaceURI());
        }
        case PrefixAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->prefix());
        }
        case LocalNameAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->localName());
        }
        case BaseURIAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->baseURI());
        }
        case TextContentAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->textContent());
        }
        case ParentElementAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentElement()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSNode::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case NodeValueAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setNodeValue(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case PrefixAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setPrefix(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case TextContentAttrNum: {
            Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setTextContent(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSNode::getConstructor(JSContext *ctx)
{
    return JSNodeConstructor::self(ctx);
}

JSValue JSNodePrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Node* imp = (Node*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSNode::InsertBeforeFuncNum: {
            return JSNode::insertBefore(ctx, this_val, argc, argv, imp);
        }
        case JSNode::ReplaceChildFuncNum: {
            return JSNode::replaceChild(ctx, this_val, argc, argv, imp);
        }
        case JSNode::RemoveChildFuncNum: {
            return JSNode::removeChild(ctx, this_val, argc, argv, imp);
        }
        case JSNode::AppendChildFuncNum: {
            return JSNode::appendChild(ctx, this_val, argc, argv, imp);
        }
        case JSNode::HasChildNodesFuncNum: {

            JSValue result = JS_NewBool(ctx, imp->hasChildNodes() ? 1 : 0);
            return result;
        }
        case JSNode::CloneNodeFuncNum: {
            bool deep = valueToBoolean(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->cloneNode(deep)));
            return result;
        }
        case JSNode::NormalizeFuncNum: {
            imp->normalize();
            return JS_UNDEFINED;
        }
        case JSNode::IsSupportedFuncNum: {
            String feature = valueToString(ctx, argv[0]);
            String version = valueToStringWithNullCheck(ctx, argv[1]);

            JSValue result = JS_NewBool(ctx, imp->isSupported(feature, version) ? 1 : 0);
            return result;
        }
        case JSNode::HasAttributesFuncNum: {

            JSValue result = JS_NewBool(ctx, imp->hasAttributes() ? 1 : 0);
            return result;
        }
        case JSNode::IsSameNodeFuncNum: {
            Node* other = toNode(argv[0]);

            JSValue result = JS_NewBool(ctx, imp->isSameNode(other) ? 1 : 0);
            return result;
        }
        case JSNode::IsEqualNodeFuncNum: {
            Node* other = toNode(argv[0]);

            JSValue result = JS_NewBool(ctx, imp->isEqualNode(other) ? 1 : 0);
            return result;
        }
        case JSNode::LookupPrefixFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->lookupPrefix(namespaceURI));
            return result;
        }
        case JSNode::IsDefaultNamespaceFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->isDefaultNamespace(namespaceURI) ? 1 : 0);
            return result;
        }
        case JSNode::LookupNamespaceURIFuncNum: {
            String prefix = valueToStringWithNullCheck(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->lookupNamespaceURI(prefix));
            return result;
        }
        case JSNode::AddEventListenerFuncNum: {
            return JSNode::addEventListener(ctx, this_val, argc, argv, imp);
        }
        case JSNode::RemoveEventListenerFuncNum: {
            return JSNode::removeEventListener(ctx, this_val, argc, argv, imp);
        }
        case JSNode::DispatchEventFuncNum: {
            return JSNode::dispatchEvent(ctx, this_val, argc, argv, imp);
        }
    }
    return JS_NULL;
}


Node* toNode(JSValue val)
{
    if (JS_IsObject(val)) {
        Node* impl = (Node*)JS_GetOpaque(val, JSNode::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
