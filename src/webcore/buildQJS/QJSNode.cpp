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

static const JSCFunctionListEntry JSNodeAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("ownerDocument", JSNode::getValueProperty, NULL, JSNode::OwnerDocumentAttrNum),
    JS_CGETSET_MAGIC_DEF("firstChild", JSNode::getValueProperty, NULL, JSNode::FirstChildAttrNum),
    JS_CGETSET_MAGIC_DEF("previousSibling", JSNode::getValueProperty, NULL, JSNode::PreviousSiblingAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSNode::getValueProperty, NULL, JSNode::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("nodeName", JSNode::getValueProperty, NULL, JSNode::NodeNameAttrNum),
    JS_CGETSET_MAGIC_DEF("nodeValue", JSNode::getValueProperty, JSNode::putValueProperty, JSNode::NodeValueAttrNum),
    JS_CGETSET_MAGIC_DEF("lastChild", JSNode::getValueProperty, NULL, JSNode::LastChildAttrNum),
    JS_CGETSET_MAGIC_DEF("localName", JSNode::getValueProperty, NULL, JSNode::LocalNameAttrNum),
    JS_CGETSET_MAGIC_DEF("nextSibling", JSNode::getValueProperty, NULL, JSNode::NextSiblingAttrNum),
    JS_CGETSET_MAGIC_DEF("nodeType", JSNode::getValueProperty, NULL, JSNode::NodeTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("childNodes", JSNode::getValueProperty, NULL, JSNode::ChildNodesAttrNum),
    JS_CGETSET_MAGIC_DEF("textContent", JSNode::getValueProperty, JSNode::putValueProperty, JSNode::TextContentAttrNum),
    JS_CGETSET_MAGIC_DEF("baseURI", JSNode::getValueProperty, NULL, JSNode::BaseURIAttrNum),
    JS_CGETSET_MAGIC_DEF("parentNode", JSNode::getValueProperty, NULL, JSNode::ParentNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("attributes", JSNode::getValueProperty, NULL, JSNode::AttributesAttrNum),
    JS_CGETSET_MAGIC_DEF("namespaceURI", JSNode::getValueProperty, NULL, JSNode::NamespaceURIAttrNum),
    JS_CGETSET_MAGIC_DEF("prefix", JSNode::getValueProperty, JSNode::putValueProperty, JSNode::PrefixAttrNum),
    JS_CGETSET_MAGIC_DEF("parentElement", JSNode::getValueProperty, NULL, JSNode::ParentElementAttrNum)
};

class JSNodeConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSNodeConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSNodeConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("DOCUMENT_FRAGMENT_NODE", JSNodeConstructor::getValueProperty, NULL, Node::DOCUMENT_FRAGMENT_NODE),
    JS_CGETSET_MAGIC_DEF("ENTITY_REFERENCE_NODE", JSNodeConstructor::getValueProperty, NULL, Node::ENTITY_REFERENCE_NODE),
    JS_CGETSET_MAGIC_DEF("CDATA_SECTION_NODE", JSNodeConstructor::getValueProperty, NULL, Node::CDATA_SECTION_NODE),
    JS_CGETSET_MAGIC_DEF("TEXT_NODE", JSNodeConstructor::getValueProperty, NULL, Node::TEXT_NODE),
    JS_CGETSET_MAGIC_DEF("COMMENT_NODE", JSNodeConstructor::getValueProperty, NULL, Node::COMMENT_NODE),
    JS_CGETSET_MAGIC_DEF("ELEMENT_NODE", JSNodeConstructor::getValueProperty, NULL, Node::ELEMENT_NODE),
    JS_CGETSET_MAGIC_DEF("ATTRIBUTE_NODE", JSNodeConstructor::getValueProperty, NULL, Node::ATTRIBUTE_NODE),
    JS_CGETSET_MAGIC_DEF("ENTITY_NODE", JSNodeConstructor::getValueProperty, NULL, Node::ENTITY_NODE),
    JS_CGETSET_MAGIC_DEF("PROCESSING_INSTRUCTION_NODE", JSNodeConstructor::getValueProperty, NULL, Node::PROCESSING_INSTRUCTION_NODE),
    JS_CGETSET_MAGIC_DEF("DOCUMENT_NODE", JSNodeConstructor::getValueProperty, NULL, Node::DOCUMENT_NODE),
    JS_CGETSET_MAGIC_DEF("DOCUMENT_TYPE_NODE", JSNodeConstructor::getValueProperty, NULL, Node::DOCUMENT_TYPE_NODE),
    JS_CGETSET_MAGIC_DEF("NOTATION_NODE", JSNodeConstructor::getValueProperty, NULL, Node::NOTATION_NODE)
};

JSValue JSNodeConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Node.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Node.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNodeConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeConstructorFunctions, countof(JSNodeConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSNodePrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("DOCUMENT_FRAGMENT_NODE", JSNodePrototype::getValueProperty, NULL, Node::DOCUMENT_FRAGMENT_NODE),
    JS_CGETSET_MAGIC_DEF("ENTITY_REFERENCE_NODE", JSNodePrototype::getValueProperty, NULL, Node::ENTITY_REFERENCE_NODE),
    JS_CGETSET_MAGIC_DEF("CDATA_SECTION_NODE", JSNodePrototype::getValueProperty, NULL, Node::CDATA_SECTION_NODE),
    JS_CGETSET_MAGIC_DEF("TEXT_NODE", JSNodePrototype::getValueProperty, NULL, Node::TEXT_NODE),
    JS_CGETSET_MAGIC_DEF("COMMENT_NODE", JSNodePrototype::getValueProperty, NULL, Node::COMMENT_NODE),
    JS_CGETSET_MAGIC_DEF("ELEMENT_NODE", JSNodePrototype::getValueProperty, NULL, Node::ELEMENT_NODE),
    JS_CGETSET_MAGIC_DEF("ATTRIBUTE_NODE", JSNodePrototype::getValueProperty, NULL, Node::ATTRIBUTE_NODE),
    JS_CGETSET_MAGIC_DEF("ENTITY_NODE", JSNodePrototype::getValueProperty, NULL, Node::ENTITY_NODE),
    JS_CGETSET_MAGIC_DEF("PROCESSING_INSTRUCTION_NODE", JSNodePrototype::getValueProperty, NULL, Node::PROCESSING_INSTRUCTION_NODE),
    JS_CGETSET_MAGIC_DEF("DOCUMENT_NODE", JSNodePrototype::getValueProperty, NULL, Node::DOCUMENT_NODE),
    JS_CGETSET_MAGIC_DEF("DOCUMENT_TYPE_NODE", JSNodePrototype::getValueProperty, NULL, Node::DOCUMENT_TYPE_NODE),
    JS_CGETSET_MAGIC_DEF("NOTATION_NODE", JSNodePrototype::getValueProperty, NULL, Node::NOTATION_NODE)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSNodePrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("removeChild", 1, JSNodePrototypeFunction::callAsFunction, JSNode::RemoveChildFuncNum),
    JS_CFUNC_MAGIC_DEF("replaceChild", 2, JSNodePrototypeFunction::callAsFunction, JSNode::ReplaceChildFuncNum),
    JS_CFUNC_MAGIC_DEF("hasAttributes", 0, JSNodePrototypeFunction::callAsFunction, JSNode::HasAttributesFuncNum),
    JS_CFUNC_MAGIC_DEF("cloneNode", 1, JSNodePrototypeFunction::callAsFunction, JSNode::CloneNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasChildNodes", 0, JSNodePrototypeFunction::callAsFunction, JSNode::HasChildNodesFuncNum),
    JS_CFUNC_MAGIC_DEF("appendChild", 1, JSNodePrototypeFunction::callAsFunction, JSNode::AppendChildFuncNum),
    JS_CFUNC_MAGIC_DEF("isEqualNode", 1, JSNodePrototypeFunction::callAsFunction, JSNode::IsEqualNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("insertBefore", 2, JSNodePrototypeFunction::callAsFunction, JSNode::InsertBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("normalize", 0, JSNodePrototypeFunction::callAsFunction, JSNode::NormalizeFuncNum),
    JS_CFUNC_MAGIC_DEF("isSupported", 2, JSNodePrototypeFunction::callAsFunction, JSNode::IsSupportedFuncNum),
    JS_CFUNC_MAGIC_DEF("isSameNode", 1, JSNodePrototypeFunction::callAsFunction, JSNode::IsSameNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("lookupPrefix", 1, JSNodePrototypeFunction::callAsFunction, JSNode::LookupPrefixFuncNum),
    JS_CFUNC_MAGIC_DEF("isDefaultNamespace", 1, JSNodePrototypeFunction::callAsFunction, JSNode::IsDefaultNamespaceFuncNum),
    JS_CFUNC_MAGIC_DEF("lookupNamespaceURI", 1, JSNodePrototypeFunction::callAsFunction, JSNode::LookupNamespaceURIFuncNum)
};

JSValue JSNodePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNode.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNodePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNode.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNodePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeAttributesFunctions, countof(JSNodeAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodePrototypeConstantsFunctions, countof(JSNodePrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodePrototypeFunctions, countof(JSNodePrototypeFunctions));
}

JSValue JSNodePrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSNodeClassDefine = 
{
    "Node",
    .finalizer = JSNode::finalizer,
    .gc_mark = JSNode::mark,
};

JSClassID JSNode::js_class_id = 0;

void JSNode::init(JSContext* ctx)
{
    if (JSNode::js_class_id == 0) {
        JS_NewClassID(&JSNode::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNode::js_class_id, &JSNodeClassDefine);
        JS_SetConstructor(ctx, JSNodeConstructor::self(ctx), JSNodePrototype::self(ctx));
        JS_SetClassProto(ctx, JSNode::js_class_id, JSNodePrototype::self(ctx));
    }
}

JSValue JSNode::create(JSContext* ctx, Node* impl)
{
    JSNode::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSNodePrototype::self(ctx), JSNode::js_class_id);
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
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

JSValue JSNode::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NodeNameAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->nodeName());
        }
        case NodeValueAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->nodeValue());
        }
        case NodeTypeAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return JS_NewBigUint64(ctx, imp->nodeType());
        }
        case ParentNodeAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentNode()));
        }
        case ChildNodesAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->childNodes()));
        }
        case FirstChildAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->firstChild()));
        }
        case LastChildAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->lastChild()));
        }
        case PreviousSiblingAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->previousSibling()));
        }
        case NextSiblingAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nextSibling()));
        }
        case AttributesAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->attributes()));
        }
        case OwnerDocumentAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerDocument()));
        }
        case NamespaceURIAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->namespaceURI());
        }
        case PrefixAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->prefix());
        }
        case LocalNameAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->localName());
        }
        case BaseURIAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->baseURI());
        }
        case TextContentAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->textContent());
        }
        case ParentElementAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
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
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setNodeValue(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case PrefixAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setPrefix(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case TextContentAttrNum: {
            Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
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
    Node* imp = (Node*)JS_GetOpaque2(ctx, this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSNode::InsertBeforeFuncNum: {
            return JSNode::insertBefore(ctx, argc, argv, imp);
        }
        case JSNode::ReplaceChildFuncNum: {
            return JSNode::replaceChild(ctx, argc, argv, imp);
        }
        case JSNode::RemoveChildFuncNum: {
            return JSNode::removeChild(ctx, argc, argv, imp);
        }
        case JSNode::AppendChildFuncNum: {
            return JSNode::appendChild(ctx, argc, argv, imp);
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
