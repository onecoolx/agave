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

#include "QJSDocument.h"

#include "Attr.h"
#include "CDATASection.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "Comment.h"
#include "DOMImplementation.h"
#include "Document.h"
#include "DocumentFragment.h"
#include "DocumentType.h"
#include "Element.h"
#include "EntityReference.h"
#include "Event.h"
#include "ExceptionCode.h"
#include "HTMLCollection.h"
#include "HTMLElement.h"
#include "NameNodeList.h"
#include "Node.h"
#include "NodeIterator.h"
#include "NodeList.h"
#include "ProcessingInstruction.h"
#include "QJSAttr.h"
#include "QJSCDATASection.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSComment.h"
#include "QJSDOMImplementation.h"
#include "QJSDocumentFragment.h"
#include "QJSDocumentType.h"
#include "QJSElement.h"
#include "QJSEntityReference.h"
#include "QJSEvent.h"
#include "QJSHTMLCollection.h"
#include "QJSHTMLElement.h"
#include "QJSNode.h"
#include "QJSNodeFilter.h"
#include "QJSNodeIterator.h"
#include "QJSNodeList.h"
#include "QJSProcessingInstruction.h"
#include "QJSRange.h"
#include "QJSStyleSheetList.h"
#include "QJSText.h"
#include "QJSTreeWalker.h"
#include "Range.h"
#include "StyleSheetList.h"
#include "Text.h"
#include "TreeWalker.h"
#include "qjs_window.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSDocumentAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("title", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::TitleAttrNum),
    JS_CGETSET_MAGIC_DEF("anchors", JSDocument::getValueProperty, NULL, JSDocument::AnchorsAttrNum),
    JS_CGETSET_MAGIC_DEF("domain", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::DomainAttrNum),
    JS_CGETSET_MAGIC_DEF("documentURI", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::DocumentURIAttrNum),
    JS_CGETSET_MAGIC_DEF("documentElement", JSDocument::getValueProperty, NULL, JSDocument::DocumentElementAttrNum),
    JS_CGETSET_MAGIC_DEF("URL", JSDocument::getValueProperty, NULL, JSDocument::URLAttrNum),
    JS_CGETSET_MAGIC_DEF("async", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::AsyncAttrNum),
    JS_CGETSET_MAGIC_DEF("doctype", JSDocument::getValueProperty, NULL, JSDocument::DoctypeAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlEncoding", JSDocument::getValueProperty, NULL, JSDocument::XMLEncodingAttrNum),
    JS_CGETSET_MAGIC_DEF("applets", JSDocument::getValueProperty, NULL, JSDocument::AppletsAttrNum),
    JS_CGETSET_MAGIC_DEF("defaultView", JSDocument::getValueProperty, NULL, JSDocument::DefaultViewAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlVersion", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::XMLVersionAttrNum),
    JS_CGETSET_MAGIC_DEF("implementation", JSDocument::getValueProperty, NULL, JSDocument::ImplementationAttrNum),
    JS_CGETSET_MAGIC_DEF("cookie", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::CookieAttrNum),
    JS_CGETSET_MAGIC_DEF("images", JSDocument::getValueProperty, NULL, JSDocument::ImagesAttrNum),
    JS_CGETSET_MAGIC_DEF("inputEncoding", JSDocument::getValueProperty, NULL, JSDocument::InputEncodingAttrNum),
    JS_CGETSET_MAGIC_DEF("links", JSDocument::getValueProperty, NULL, JSDocument::LinksAttrNum),
    JS_CGETSET_MAGIC_DEF("location", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::LocationAttrNum),
    JS_CGETSET_MAGIC_DEF("defaultCharset", JSDocument::getValueProperty, NULL, JSDocument::DefaultCharsetAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlStandalone", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::XMLStandaloneAttrNum),
    JS_CGETSET_MAGIC_DEF("styleSheets", JSDocument::getValueProperty, NULL, JSDocument::StyleSheetsAttrNum),
    JS_CGETSET_MAGIC_DEF("referrer", JSDocument::getValueProperty, NULL, JSDocument::ReferrerAttrNum),
    JS_CGETSET_MAGIC_DEF("body", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::BodyAttrNum),
    JS_CGETSET_MAGIC_DEF("forms", JSDocument::getValueProperty, NULL, JSDocument::FormsAttrNum),
    JS_CGETSET_MAGIC_DEF("charset", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::CharsetAttrNum),
    JS_CGETSET_MAGIC_DEF("readyState", JSDocument::getValueProperty, NULL, JSDocument::ReadyStateAttrNum),
    JS_CGETSET_MAGIC_DEF("characterSet", JSDocument::getValueProperty, NULL, JSDocument::CharacterSetAttrNum),
    JS_CGETSET_MAGIC_DEF("preferredStylesheetSet", JSDocument::getValueProperty, NULL, JSDocument::PreferredStylesheetSetAttrNum),
    JS_CGETSET_MAGIC_DEF("selectedStylesheetSet", JSDocument::getValueProperty, JSDocument::putValueProperty, JSDocument::SelectedStylesheetSetAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSDocument::getValueProperty, NULL, JSDocument::ConstructorAttrNum)
};

class JSDocumentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSDocumentConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSDocumentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Document.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSDocumentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Document.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDocumentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSDocumentPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("importNode", 2, JSDocumentPrototypeFunction::callAsFunction, JSDocument::ImportNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("getOverrideStyle", 2, JSDocumentPrototypeFunction::callAsFunction, JSDocument::GetOverrideStyleFuncNum),
    JS_CFUNC_MAGIC_DEF("createAttributeNS", 2, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateAttributeNSFuncNum),
    JS_CFUNC_MAGIC_DEF("queryCommandIndeterm", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::QueryCommandIndetermFuncNum),
    JS_CFUNC_MAGIC_DEF("createEntityReference", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateEntityReferenceFuncNum),
    JS_CFUNC_MAGIC_DEF("adoptNode", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::AdoptNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("createRange", 0, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateRangeFuncNum),
    JS_CFUNC_MAGIC_DEF("queryCommandState", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::QueryCommandStateFuncNum),
    JS_CFUNC_MAGIC_DEF("createElement", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateElementFuncNum),
    JS_CFUNC_MAGIC_DEF("createTextNode", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateTextNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("createNodeIterator", 4, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateNodeIteratorFuncNum),
    JS_CFUNC_MAGIC_DEF("elementFromPoint", 2, JSDocumentPrototypeFunction::callAsFunction, JSDocument::ElementFromPointFuncNum),
    JS_CFUNC_MAGIC_DEF("createAttribute", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("createComment", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateCommentFuncNum),
    JS_CFUNC_MAGIC_DEF("getElementById", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::GetElementByIdFuncNum),
    JS_CFUNC_MAGIC_DEF("createDocumentFragment", 0, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateDocumentFragmentFuncNum),
    JS_CFUNC_MAGIC_DEF("createCDATASection", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateCDATASectionFuncNum),
    JS_CFUNC_MAGIC_DEF("createProcessingInstruction", 2, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateProcessingInstructionFuncNum),
    JS_CFUNC_MAGIC_DEF("getElementsByTagName", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::GetElementsByTagNameFuncNum),
    JS_CFUNC_MAGIC_DEF("createElementNS", 2, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateElementNSFuncNum),
    JS_CFUNC_MAGIC_DEF("getElementsByTagNameNS", 2, JSDocumentPrototypeFunction::callAsFunction, JSDocument::GetElementsByTagNameNSFuncNum),
    JS_CFUNC_MAGIC_DEF("load", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::LoadFuncNum),
    JS_CFUNC_MAGIC_DEF("createEvent", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateEventFuncNum),
    JS_CFUNC_MAGIC_DEF("createTreeWalker", 4, JSDocumentPrototypeFunction::callAsFunction, JSDocument::CreateTreeWalkerFuncNum),
    JS_CFUNC_MAGIC_DEF("execCommand", 3, JSDocumentPrototypeFunction::callAsFunction, JSDocument::ExecCommandFuncNum),
    JS_CFUNC_MAGIC_DEF("queryCommandEnabled", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::QueryCommandEnabledFuncNum),
    JS_CFUNC_MAGIC_DEF("queryCommandSupported", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::QueryCommandSupportedFuncNum),
    JS_CFUNC_MAGIC_DEF("queryCommandValue", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::QueryCommandValueFuncNum),
    JS_CFUNC_MAGIC_DEF("getElementsByName", 1, JSDocumentPrototypeFunction::callAsFunction, JSDocument::GetElementsByNameFuncNum)
};

JSValue JSDocumentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDocument.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSDocumentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDocument.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDocumentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSDocumentAttributesFunctions, countof(JSDocumentAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSDocumentPrototypeFunctions, countof(JSDocumentPrototypeFunctions));
}

static JSClassDef JSDocumentClassDefine = 
{
    "Document",
    .finalizer = JSDocument::finalizer,
    .gc_mark = JSDocument::mark,
};

JSClassID JSDocument::js_class_id = 0;

void JSDocument::init(JSContext* ctx)
{
    if (JSDocument::js_class_id == 0) {
        JS_NewClassID(&JSDocument::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDocument::js_class_id, &JSDocumentClassDefine);
        JS_SetConstructor(ctx, JSDocumentConstructor::self(ctx), JSDocumentPrototype::self(ctx));
        JS_SetClassProto(ctx, JSDocument::js_class_id, JSDocumentPrototype::self(ctx));
    }
}

JSValue JSDocument::create(JSContext* ctx, Document* impl)
{
    JSDocument::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSDocumentPrototype::self(ctx), JSDocument::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDocument::finalizer(JSRuntime* rt, JSValue val)
{
    Document* impl = (Document*)JS_GetOpaque(val, JSDocument::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

JSValue JSDocument::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DoctypeAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->doctype()));
        }
        case ImplementationAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->implementation()));
        }
        case DocumentElementAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->documentElement()));
        }
        case InputEncodingAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrNull(ctx, imp->inputEncoding());
        }
        case XMLEncodingAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrNull(ctx, imp->xmlEncoding());
        }
        case XMLVersionAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrNull(ctx, imp->xmlVersion());
        }
        case XMLStandaloneAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return JS_NewBool(ctx, imp->xmlStandalone() ? 1 : 0);
        }
        case DocumentURIAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrNull(ctx, imp->documentURI());
        }
        case AsyncAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return JS_NewBool(ctx, imp->async() ? 1 : 0);
        }
        case DefaultViewAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->defaultView()));
        }
        case StyleSheetsAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->styleSheets()));
        }
        case TitleAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->title()).utf8().data());
        }
        case ReferrerAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->referrer()).utf8().data());
        }
        case DomainAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->domain()).utf8().data());
        }
        case URLAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->URL()).utf8().data());
        }
        case CookieAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cookie()).utf8().data());
        }
        case BodyAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->body()));
        }
        case ImagesAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->images()));
        }
        case AppletsAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->applets()));
        }
        case LinksAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->links()));
        }
        case FormsAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->forms()));
        }
        case AnchorsAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->anchors()));
        }
        case LocationAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return JSDocument::location(ctx, imp);
        }
        case CharsetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrUndefined(ctx, imp->charset());
        }
        case DefaultCharsetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrUndefined(ctx, imp->defaultCharset());
        }
        case ReadyStateAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrUndefined(ctx, imp->readyState());
        }
        case CharacterSetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrNull(ctx, imp->characterSet());
        }
        case PreferredStylesheetSetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrNull(ctx, imp->preferredStylesheetSet());
        }
        case SelectedStylesheetSetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            return jsStringOrNull(ctx, imp->selectedStylesheetSet());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSDocument::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XMLVersionAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            ExceptionCode ec = 0;
            imp->setXMLVersion(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case XMLStandaloneAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            ExceptionCode ec = 0;
            imp->setXMLStandalone(valueToBoolean(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case DocumentURIAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            imp->setDocumentURI(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AsyncAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            ExceptionCode ec = 0;
            imp->setAsync(valueToBoolean(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case TitleAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            imp->setTitle(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DomainAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            imp->setDomain(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CookieAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            imp->setCookie(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BodyAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            ExceptionCode ec = 0;
            imp->setBody(toHTMLElement(value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case LocationAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            JSDocument::setLocation(ctx, value, imp);
            break;
        }
        case CharsetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            imp->setCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SelectedStylesheetSetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
            imp->setSelectedStylesheetSet(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSDocument::getConstructor(JSContext *ctx)
{
    return JSDocumentConstructor::self(ctx);
}

JSValue JSDocumentPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Document* imp = (Document*)JS_GetOpaque2(ctx, this_val, JSDocument::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSDocument::CreateElementFuncNum: {
            ExceptionCode ec = 0;
            String tagName = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createElement(tagName, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateDocumentFragmentFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->createDocumentFragment()));
            return result;
        }
        case JSDocument::CreateTextNodeFuncNum: {
            String data = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createTextNode(data)));
            return result;
        }
        case JSDocument::CreateCommentFuncNum: {
            String data = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createComment(data)));
            return result;
        }
        case JSDocument::CreateCDATASectionFuncNum: {
            ExceptionCode ec = 0;
            String data = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createCDATASection(data, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateProcessingInstructionFuncNum: {
            ExceptionCode ec = 0;
            String target = valueToString(ctx, argv[0]);
            String data = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createProcessingInstruction(target, data, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateAttributeFuncNum: {
            ExceptionCode ec = 0;
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createAttribute(name, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateEntityReferenceFuncNum: {
            ExceptionCode ec = 0;
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createEntityReference(name, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::GetElementsByTagNameFuncNum: {
            String tagname = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getElementsByTagName(tagname)));
            return result;
        }
        case JSDocument::ImportNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* importedNode = toNode(argv[0]);
            bool deep = valueToBoolean(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->importNode(importedNode, deep, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateElementNSFuncNum: {
            ExceptionCode ec = 0;
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String qualifiedName = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createElementNS(namespaceURI, qualifiedName, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateAttributeNSFuncNum: {
            ExceptionCode ec = 0;
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String qualifiedName = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createAttributeNS(namespaceURI, qualifiedName, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::GetElementsByTagNameNSFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getElementsByTagNameNS(namespaceURI, localName)));
            return result;
        }
        case JSDocument::GetElementByIdFuncNum: {
            String elementId = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getElementById(elementId)));
            return result;
        }
        case JSDocument::AdoptNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* source = toNode(argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->adoptNode(source, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::LoadFuncNum: {
            String url = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->load(url) ? 1 : 0);
            return result;
        }
        case JSDocument::CreateEventFuncNum: {
            ExceptionCode ec = 0;
            String eventType = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createEvent(eventType, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateRangeFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->createRange()));
            return result;
        }
        case JSDocument::CreateNodeIteratorFuncNum: {
            ExceptionCode ec = 0;
            Node* root = toNode(argv[0]);
            bool whatToShowOk;
            unsigned whatToShow = valueToInt32(ctx, argv[1], whatToShowOk);
            if (!whatToShowOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            NodeFilter* filter = toNodeFilter(ctx, argv[2]);
            bool entityReferenceExpansion = valueToBoolean(ctx, argv[3]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createNodeIterator(root, whatToShow, filter, entityReferenceExpansion, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateTreeWalkerFuncNum: {
            ExceptionCode ec = 0;
            Node* root = toNode(argv[0]);
            bool whatToShowOk;
            unsigned whatToShow = valueToInt32(ctx, argv[1], whatToShowOk);
            if (!whatToShowOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            NodeFilter* filter = toNodeFilter(ctx, argv[2]);
            bool entityReferenceExpansion = valueToBoolean(ctx, argv[3]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createTreeWalker(root, whatToShow, filter, entityReferenceExpansion, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::GetOverrideStyleFuncNum: {
            Element* element = toElement(argv[0]);
            String pseudoElement = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getOverrideStyle(element, pseudoElement)));
            return result;
        }
        case JSDocument::ExecCommandFuncNum: {
            String command = valueToString(ctx, argv[0]);
            bool userInterface = valueToBoolean(ctx, argv[1]);
            String value = valueToStringWithUndefinedOrNullCheck(ctx, argv[2]);

            JSValue result = JS_NewBool(ctx, imp->execCommand(command, userInterface, value) ? 1 : 0);
            return result;
        }
        case JSDocument::QueryCommandEnabledFuncNum: {
            String command = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->queryCommandEnabled(command) ? 1 : 0);
            return result;
        }
        case JSDocument::QueryCommandIndetermFuncNum: {
            String command = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->queryCommandIndeterm(command) ? 1 : 0);
            return result;
        }
        case JSDocument::QueryCommandStateFuncNum: {
            String command = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->queryCommandState(command) ? 1 : 0);
            return result;
        }
        case JSDocument::QueryCommandSupportedFuncNum: {
            String command = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->queryCommandSupported(command) ? 1 : 0);
            return result;
        }
        case JSDocument::QueryCommandValueFuncNum: {
            String command = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrFalse(ctx, imp->queryCommandValue(command));
            return result;
        }
        case JSDocument::GetElementsByNameFuncNum: {
            String elementName = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getElementsByName(elementName)));
            return result;
        }
        case JSDocument::ElementFromPointFuncNum: {
            bool xOk;
            int x = valueToInt32(ctx, argv[0], xOk);
            if (!xOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool yOk;
            int y = valueToInt32(ctx, argv[1], yOk);
            if (!yOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->elementFromPoint(x, y)));
            return result;
        }
    }
    return JS_NULL;
}


}
