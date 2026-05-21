/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Samuel Weinig <sam@webkit.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "qjs_binding.h"

#include "Chrome.h"
#include "CSSRule.h"
#include "CSSStyleRule.h"
#include "CSSMediaRule.h"
#include "CSSFontFaceRule.h"
#include "CSSPageRule.h"
#include "CSSImportRule.h"
#include "CSSCharsetRule.h"
#include "CSSPrimitiveValue.h"
#include "CSSValue.h"
#include "CSSValueList.h"
#include "CSSStyleSheet.h"
#include "Clipboard.h"
#include "Document.h"
#include "Event.h"
#include "EventNames.h"
#include "EventTargetNode.h"
#include "Frame.h"
#include "HTMLElement.h"
#include "HTMLNames.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "MutationEvent.h"
#include "Page.h"
#include "Range.h"
#include "RangeException.h"
#include "StyleSheet.h"
#include "UIEvent.h"
#include "XMLHttpRequest.h"
#include "QJSNode.h"
#include "QJSDocument.h"
#include "QJSDOMWindow.h"
#include "QJSHTMLElementWrapperFactory.h"
#include "QJSCSSRule.h"
#include "QJSCSSStyleRule.h"
#include "QJSCSSMediaRule.h"
#include "QJSCSSFontFaceRule.h"
#include "QJSCSSPageRule.h"
#include "QJSCSSImportRule.h"
#include "QJSCSSCharsetRule.h"
#include "QJSCSSValue.h"
#include "QJSCSSValueList.h"
#include "QJSCSSPrimitiveValue.h"
#include "QJSCSSStyleSheet.h"
#include "QJSStyleSheet.h"
#include "QJSEvent.h"
#include "QJSKeyboardEvent.h"
#include "QJSMouseEvent.h"
#include "QJSMutationEvent.h"
#include "QJSUIEvent.h"

#include "HTMLAnchorElement.h"
#include "HTMLAppletElement.h"
#include "HTMLAreaElement.h"
#include "HTMLBRElement.h"
#include "HTMLBaseElement.h"
#include "HTMLBaseFontElement.h"
#include "HTMLBodyElement.h"
#include "HTMLButtonElement.h"
#include "HTMLCanvasElement.h"
#include "HTMLDListElement.h"
#include "HTMLDivElement.h"
#include "HTMLEmbedElement.h"
#include "HTMLFieldSetElement.h"
#include "HTMLFontElement.h"
#include "HTMLFormElement.h"
#include "HTMLFrameElement.h"
#include "HTMLFrameSetElement.h"
#include "HTMLHRElement.h"
#include "HTMLHeadElement.h"
#include "HTMLHeadingElement.h"
#include "HTMLHtmlElement.h"
#include "HTMLIFrameElement.h"
#include "HTMLImageElement.h"
#include "HTMLInputElement.h"
#include "HTMLLIElement.h"
#include "HTMLLabelElement.h"
#include "HTMLLegendElement.h"
#include "HTMLLinkElement.h"
#include "HTMLMapElement.h"
#include "HTMLMarqueeElement.h"
#include "HTMLMetaElement.h"
#include "HTMLObjectElement.h"
#include "HTMLOListElement.h"
#include "HTMLOptGroupElement.h"
#include "HTMLOptionElement.h"
#include "HTMLParagraphElement.h"
#include "HTMLParamElement.h"
#include "HTMLPreElement.h"
#include "HTMLScriptElement.h"
#include "HTMLSelectElement.h"
#include "HTMLStyleElement.h"
#include "HTMLTableCaptionElement.h"
#include "HTMLTableCellElement.h"
#include "HTMLTableColElement.h"
#include "HTMLTableElement.h"
#include "HTMLTableRowElement.h"
#include "HTMLTableSectionElement.h"
#include "HTMLTextAreaElement.h"
#include "HTMLTitleElement.h"
#include "HTMLUListElement.h"

#include "QJSHTMLAnchorElement.h"
#include "QJSHTMLAppletElement.h"
#include "QJSHTMLAreaElement.h"
#include "QJSHTMLBRElement.h"
#include "QJSHTMLBaseElement.h"
#include "QJSHTMLBaseFontElement.h"
#include "QJSHTMLBodyElement.h"
#include "QJSHTMLButtonElement.h"
#include "QJSHTMLCanvasElement.h"
#include "QJSHTMLDListElement.h"
#include "QJSHTMLDivElement.h"
#include "QJSHTMLElement.h"
#include "QJSHTMLEmbedElement.h"
#include "QJSHTMLFieldSetElement.h"
#include "QJSHTMLFontElement.h"
#include "QJSHTMLFormElement.h"
#include "QJSHTMLFrameElement.h"
#include "QJSHTMLFrameSetElement.h"
#include "QJSHTMLHRElement.h"
#include "QJSHTMLHeadElement.h"
#include "QJSHTMLHeadingElement.h"
#include "QJSHTMLHtmlElement.h"
#include "QJSHTMLIFrameElement.h"
#include "QJSHTMLImageElement.h"
#include "QJSHTMLInputElement.h"
#include "QJSHTMLLIElement.h"
#include "QJSHTMLLabelElement.h"
#include "QJSHTMLLegendElement.h"
#include "QJSHTMLLinkElement.h"
#include "QJSHTMLMapElement.h"
#include "QJSHTMLMarqueeElement.h"
#include "QJSHTMLMetaElement.h"
#include "QJSHTMLObjectElement.h"
#include "QJSHTMLOListElement.h"
#include "QJSHTMLOptGroupElement.h"
#include "QJSHTMLOptionElement.h"
#include "QJSHTMLParagraphElement.h"
#include "QJSHTMLParamElement.h"
#include "QJSHTMLPreElement.h"
#include "QJSHTMLScriptElement.h"
#include "QJSHTMLSelectElement.h"
#include "QJSHTMLStyleElement.h"
#include "QJSHTMLTableCaptionElement.h"
#include "QJSHTMLTableCellElement.h"
#include "QJSHTMLTableColElement.h"
#include "QJSHTMLTableElement.h"
#include "QJSHTMLTableRowElement.h"
#include "QJSHTMLTableSectionElement.h"
#include "QJSHTMLTextAreaElement.h"
#include "QJSHTMLTitleElement.h"
#include "QJSHTMLUListElement.h"

#include <text/String.h>
#include "global.h"
#include "qjs_dom.h"
#include "qjs_window.h"
#include "qjs_script.h"
#include "QJSDOMExceptionConstructor.h"

#if ENABLE(SVG)
#include "SVGException.h"
#endif

#if ENABLE(XPATH)
#include "XPathEvaluator.h"
#endif

using namespace WebCore;
using namespace EventNames;

namespace QJS {

static DOMObjectMap* domObjects()
{ 
    return GLOBAL()->domObjects;
}

static NodePerDocMap* domNodesPerDocument()
{
    return GLOBAL()->domNodesPerDoc;
}

ScriptInterpreter::ScriptInterpreter(JSContext* ctx, JSValue global, Frame* frame)
    : m_context(ctx)
    , m_globalObject(global)
    , m_frame(frame)
    , m_currentEvent(0)
    , m_timerCallback(false)
{
}

JSValue ScriptInterpreter::getDOMObject(void* objectHandle) 
{
    return domObjects()->get(objectHandle);
}

void ScriptInterpreter::putDOMObject(void* objectHandle, JSValue obj) 
{
    domObjects()->set(objectHandle, obj); // no DupValue for non-Node objects
}

void ScriptInterpreter::forgetDOMObject(void* objectHandle)
{
    JSRuntime* rt = GLOBAL()->runtime;
    JSValue old = domObjects()->get(objectHandle);
    if (JS_VALUE_GET_TAG(old) == JS_TAG_OBJECT)
        JS_FreeValueRT(rt, old);
    domObjects()->remove(objectHandle);
}

JSValue ScriptInterpreter::getDOMNodeForDocument(Document* document, Node* node)
{
    if (!document)
        return domObjects()->get(node);

    NodeMap* documentDict = domNodesPerDocument()->get(document);
    if (documentDict)
        return documentDict->get(node);

    return JS_NULL;
}

void ScriptInterpreter::forgetDOMNodeForDocument(Document* document, Node* node)
{
    JSRuntime* rt = GLOBAL()->runtime;
    if (!document) {
        JSValue old = domObjects()->get(node);
        if (JS_VALUE_GET_TAG(old) == JS_TAG_OBJECT)
            JS_FreeValueRT(rt, old);
        domObjects()->remove(node);
        return;
    }
    NodeMap* documentDict = domNodesPerDocument()->get(document);
    if (documentDict) {
        JSValue old = documentDict->get(node);
        if (JS_VALUE_GET_TAG(old) == JS_TAG_OBJECT)
            JS_FreeValueRT(rt, old);
        documentDict->remove(node);
    }
}

void ScriptInterpreter::putDOMNodeForDocument(Document* document, Node* node, JSValue obj)
{
    JSRuntime* rt = GLOBAL()->runtime;
    if (!document) {
        domObjects()->set(node, JS_DupValueRT(rt, obj));
        return;
    }
    NodeMap* documentDict = domNodesPerDocument()->get(document);
    if (!documentDict) {
        documentDict = new NodeMap;
        domNodesPerDocument()->set(document, documentDict);
    }
    documentDict->set(node, JS_DupValueRT(rt, obj));
}

void ScriptInterpreter::forgetAllDOMNodesForDocument(Document* document)
{
    ASSERT(document);
    NodePerDocMap::iterator it = domNodesPerDocument()->find(document);
    if (it != domNodesPerDocument()->end()) {
        JSRuntime* rt = GLOBAL()->runtime;
        NodeMap* nodeMap = it->second;
        NodeMap::iterator nit = nodeMap->begin();
        NodeMap::iterator nend = nodeMap->end();
        for (; nit != nend; ++nit)
            // intentionally not freeing - prevents GC assert during navigation
        delete nodeMap;
        domNodesPerDocument()->remove(it);
    }
}

JSValue ScriptInterpreter::globalObject() const
{
    if (m_frame && m_frame->refCount() > 0)
        m_frame->keepAlive();
    return m_globalObject;
}

void* ScriptInterpreter::globalObjectData() const
{
    return JS_GetOpaque(globalObject(), JSDOMWindow::js_class_id);
}

void ScriptInterpreter::updateDOMNodeDocument(Node* node, Document* oldDoc, Document* newDoc)
{
    ASSERT(oldDoc != newDoc);
    JSValue obj = getDOMNodeForDocument(oldDoc, node);
    if (!JS_IsNull(obj)) {
        putDOMNodeForDocument(newDoc, node, obj);
        forgetDOMNodeForDocument(oldDoc, node);
    }
}

bool ScriptInterpreter::wasRunByUserGesture() const
{
    if (m_currentEvent) {
        const AtomicString& type = m_currentEvent->type();
        bool eventOk = ( // mouse events
            type == clickEvent || type == mousedownEvent ||
            type == mouseupEvent || type == dblclickEvent ||
            // keyboard events
            type == keydownEvent || type == keypressEvent ||
            type == keyupEvent ||
            // other accepted events
            type == selectEvent || type == changeEvent ||
            type == focusEvent || type == blurEvent ||
            type == submitEvent);
        if (eventOk)
            return true;
    } else { // no event
        if (!m_timerCallback)
            // This is the <a href="javascript:window.open('...')> case -> we let it through
            return true;
        // This is the <script>window.open(...)</script> case or a timer callback -> block it
    }
    return false;
}

#if 0
bool ScriptInterpreter::isGlobalObject(JSValue* v)
{
    return v->isObject(&Window::info);
}

bool ScriptInterpreter::isSafeScript(const Interpreter* target)
{
    return Window::isSafeScript(this, static_cast<const ScriptInterpreter*>(target));
}

Interpreter* ScriptInterpreter::interpreterForGlobalObject(const JSValue* imp)
{
    const Window* win = static_cast<const Window*>(imp);
    return win->interpreter();
}
#endif

bool ScriptInterpreter::shouldInterruptScript() const
{
    Page* page = m_frame->page();

    // See <rdar://problem/5479443>. We don't think that page can ever be NULL
    // in this case, but if it is, we've gotten into a state where we may have
    // hung the UI, with no way to ask the client whether to cancel execution. 
    // For now, our solution is just to cancel execution no matter what, 
    // ensuring that we never hang. We might want to consider other solutions 
    // if we discover problems with this one.
    ASSERT(page);
    if (!page)
        return true;

    return page->chrome()->shouldInterruptJavaScript();
}

JSValue jsStringOrNull(JSContext* ctx, const String& s)
{
    if (s.isNull())
        return JS_NULL;
    return JS_NewString(ctx, s.utf8().data());
}

JSValue jsStringOrUndefined(JSContext* ctx, const String& s)
{
    if (s.isNull())
        return JS_UNDEFINED;
    return JS_NewString(ctx, s.utf8().data());
}

JSValue jsStringOrFalse(JSContext* ctx, const String& s)
{
    if (s.isNull())
        return JS_NewBool(ctx, FALSE);
    return JS_NewString(ctx, s.utf8().data());
}

String valueToStringWithNullCheck(JSContext* ctx, JSValue val)
{
    if (JS_IsNull(val))
        return String();
    const char * str = JS_ToCString(ctx, val);
    String strObj(str);
    JS_FreeCString(ctx, str);
    return strObj;
}

String valueToStringWithUndefinedOrNullCheck(JSContext* ctx, JSValue val)
{
    if (JS_IsNull(val) || JS_IsUndefined(val))
        return String();

    const char * str = JS_ToCString(ctx, val);
    String strObj(str);
    JS_FreeCString(ctx, str);
    return strObj;
}

String valueToString(JSContext* ctx, JSValue val)
{
    const char * str = JS_ToCString(ctx, val);
    String strObj(str);
    JS_FreeCString(ctx, str);
    return strObj;
}

int32_t valueToInt32(JSContext* ctx, JSValue val, bool& ok)
{
    int32_t ret = 0;
    if (JS_ToInt32(ctx, &ret, val)) {
        ok = false;
    } else {
        ok = true;
    }
    return ret;
}

int32_t valueToInt32(JSContext* ctx, JSValue val)
{
    int32_t ret = 0;
    JS_ToInt32(ctx, &ret, val);
    return ret;
}

float valueToFloat(JSContext* ctx, JSValue val)
{
    double d = 0.0;
    JS_ToFloat64(ctx, &d, val);
    return (float)d;
}

double valueToDouble(JSContext* ctx, JSValue val)
{
    double d = 0.0;
    JS_ToFloat64(ctx, &d, val);
    return d;
}

bool valueToBoolean(JSContext* ctx, JSValue val)
{
    return JS_ToBool(ctx, val) == TRUE ? true : false;
}

static const char* const exceptionNames[] = {
    0,
    "INDEX_SIZE_ERR",
    "DOMSTRING_SIZE_ERR",
    "HIERARCHY_REQUEST_ERR",
    "WRONG_DOCUMENT_ERR",
    "INVALID_CHARACTER_ERR",
    "NO_DATA_ALLOWED_ERR",
    "NO_MODIFICATION_ALLOWED_ERR",
    "NOT_FOUND_ERR",
    "NOT_SUPPORTED_ERR",
    "INUSE_ATTRIBUTE_ERR",
    "INVALID_STATE_ERR",
    "SYNTAX_ERR",
    "INVALID_MODIFICATION_ERR",
    "NAMESPACE_ERR",
    "INVALID_ACCESS_ERR",
    "VALIDATION_ERR",
    "TYPE_MISMATCH_ERR",
};

static const char* const rangeExceptionNames[] = {
    0, "BAD_BOUNDARYPOINTS_ERR", "INVALID_NODE_TYPE_ERR"
};

static const char* const eventExceptionNames[] = {
    "UNSPECIFIED_EVENT_TYPE_ERR"
};

static const char* const xmlHttpRequestExceptionNames[] = {
    "NETWORK_ERR"
};

#if ENABLE(XPATH)
static const char* const xpathExceptionNames[] = {
    "INVALID_EXPRESSION_ERR",
    "TYPE_ERR"
};
#endif

#if ENABLE(SVG)
static const char* const svgExceptionNames[] = {
    "SVG_WRONG_TYPE_ERR",
    "SVG_INVALID_VALUE_ERR",
    "SVG_MATRIX_NOT_INVERTABLE"
};
#endif

void setDOMException(JSContext* ctx, ExceptionCode ec)
{
    if (ec == 0 || JS_HasException(ctx))
        return;

    const char* type = "DOM";
    int32_t code = ec;

    const char* const* nameTable;
  
    int nameTableSize;
    int nameIndex;
    if (code >= RangeExceptionOffset && code <= RangeExceptionMax) {
        type = "DOM Range";
        code -= RangeExceptionOffset;
        nameIndex = code;
        nameTable = rangeExceptionNames;
        nameTableSize = sizeof(rangeExceptionNames) / sizeof(rangeExceptionNames[0]);
    } else if (code >= EventExceptionOffset && code <= EventExceptionMax) {
        type = "DOM Events";
        code -= EventExceptionOffset;
        nameIndex = code;
        nameTable = eventExceptionNames;
        nameTableSize = sizeof(eventExceptionNames) / sizeof(eventExceptionNames[0]);
#if ENABLE(AJAX)    
    } else if (code == XMLHttpRequestExceptionOffset) {
        // FIXME: this exception should be replaced with DOM SECURITY_ERR when it finds its way to the spec.
        JS_ThrowInternalError(ctx, "Permission denied");
        return;
    } else if (code > XMLHttpRequestExceptionOffset && code <= XMLHttpRequestExceptionMax) {
        type = "XMLHttpRequest";
        // XMLHttpRequest exception codes start with 101 and we don't want 100 empty elements in the name array
        nameIndex = code - NETWORK_ERR;
        code -= XMLHttpRequestExceptionOffset;
        nameTable = xmlHttpRequestExceptionNames;
        nameTableSize = sizeof(xmlHttpRequestExceptionNames) / sizeof(xmlHttpRequestExceptionNames[0]);
#endif
#if ENABLE(XPATH)
    } else if (code >= XPathExceptionOffset && code <= XPathExceptionMax) {
        type = "DOM XPath";
        // XPath exception codes start with 51 and we don't want 51 empty elements in the name array
        nameIndex = code - INVALID_EXPRESSION_ERR;
        code -= XPathExceptionOffset;
        nameTable = xpathExceptionNames;
        nameTableSize = sizeof(xpathExceptionNames) / sizeof(xpathExceptionNames[0]);
#endif
#if ENABLE(SVG)
    } else if (code >= SVGExceptionOffset && code <= SVGExceptionMax) {
        type = "DOM SVG";
        code -= SVGExceptionOffset;
        nameIndex = code;
        nameTable = svgExceptionNames;
        nameTableSize = sizeof(svgExceptionNames) / sizeof(svgExceptionNames[0]);
#endif
    } else {
        nameIndex = code;
        nameTable = exceptionNames;
        nameTableSize = sizeof(exceptionNames) / sizeof(exceptionNames[0]);
    }

    const char* name = (nameIndex < nameTableSize && nameIndex >= 0) ? nameTable[nameIndex] : 0;

    // 100 characters is a big enough buffer, because there are:
    //   13 characters in the message
    //   10 characters in the longest type, "DOM Events"
    //   27 characters in the longest name, "NO_MODIFICATION_ALLOWED_ERR"
    //   20 or so digits in the longest integer's ASCII form (even if int is 64-bit)
    //   1 byte for a null character
    // That adds up to about 70 bytes.
    JSValue errorObject;

    if (name)
        errorObject = JS_ThrowInternalError(ctx, "%s: %s Exception %d", name, type, code);
    else
        errorObject = JS_ThrowInternalError(ctx, "%s Exception %d", type, code);

    JS_SetPropertyStr(ctx, errorObject, "code", JS_NewInt32(ctx, code));
}

JSValue jsString(JSContext* ctx, const WebCore::String& s)
{
    if (s.isNull())
        return JS_NewString(ctx, "");
    return JS_NewString(ctx, s.utf8().data());
}

JSValue ScriptInterpreter::evaluate(const WebCore::String& sourceURL, int startingLineNumber, const UChar* code, int codeLength, JSValue thisV)
{
    WebCore::String codeStr(code, codeLength);
    return evaluate(sourceURL, startingLineNumber, codeStr, thisV);
}

JSValue ScriptInterpreter::evaluate(const WebCore::String& sourceURL, int startingLineNumber, const WebCore::String& code, JSValue thisV)
{
    if (!m_context)
        return JS_UNDEFINED;
    JSValue result = JS_Eval(m_context, code.utf8().data(), code.utf8().length(),
                             sourceURL.utf8().data(), JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result)) {
        JS_FreeValue(m_context, JS_GetException(m_context));
        return JS_UNDEFINED;
    }
    return result;
}

}

namespace WebCore {

JSValue toJS(JSContext* ctx, Document* doc)
{
    if (!doc)
        return JS_NULL;
    JSValue ret = QJS::ScriptInterpreter::getDOMNodeForDocument(doc, doc);
    if (JS_VALUE_GET_TAG(ret) == JS_TAG_OBJECT)
        return ret;
    ret = JSDocument::create(ctx, doc);
    QJS::ScriptInterpreter::putDOMNodeForDocument(doc, doc, ret);
    return ret;
}

JSValue toJS(JSContext* ctx, Event* event)
{
    if (!event)
        return JS_NULL;
    JSValue ret = QJS::ScriptInterpreter::getDOMObject(event);
    if (JS_VALUE_GET_TAG(ret) == JS_TAG_OBJECT)
        return ret;

    if (event->isKeyboardEvent())
        ret = JSKeyboardEvent::create(ctx, static_cast<KeyboardEvent*>(event));
    else if (event->isMouseEvent())
        ret = JSMouseEvent::create(ctx, static_cast<MouseEvent*>(event));
    else if (event->isUIEvent())
        ret = JSUIEvent::create(ctx, static_cast<UIEvent*>(event));
    else if (event->isMutationEvent())
        ret = JSMutationEvent::create(ctx, static_cast<MutationEvent*>(event));
    else
        ret = JSEvent::create(ctx, event);

    QJS::ScriptInterpreter::putDOMObject(event, ret);
    return ret;
}

JSValue toJS(JSContext* ctx, CSSRule* rule)
{
    if (!rule)
        return JS_NULL;
    JSValue ret = QJS::ScriptInterpreter::getDOMObject(rule);
    if (JS_VALUE_GET_TAG(ret) == JS_TAG_OBJECT)
        return ret;

    switch (rule->type()) {
    case CSSRule::STYLE_RULE:
        ret = JSCSSStyleRule::create(ctx, static_cast<CSSStyleRule*>(rule)); break;
    case CSSRule::MEDIA_RULE:
        ret = JSCSSMediaRule::create(ctx, static_cast<CSSMediaRule*>(rule)); break;
    case CSSRule::FONT_FACE_RULE:
        ret = JSCSSFontFaceRule::create(ctx, static_cast<CSSFontFaceRule*>(rule)); break;
    case CSSRule::PAGE_RULE:
        ret = JSCSSPageRule::create(ctx, static_cast<CSSPageRule*>(rule)); break;
    case CSSRule::IMPORT_RULE:
        ret = JSCSSImportRule::create(ctx, static_cast<CSSImportRule*>(rule)); break;
    case CSSRule::CHARSET_RULE:
        ret = JSCSSCharsetRule::create(ctx, static_cast<CSSCharsetRule*>(rule)); break;
    default:
        ret = JSCSSRule::create(ctx, rule); break;
    }

    QJS::ScriptInterpreter::putDOMObject(rule, ret);
    return ret;
}

JSValue toJS(JSContext* ctx, CSSValue* value)
{
    if (!value)
        return JS_NULL;
    JSValue ret = QJS::ScriptInterpreter::getDOMObject(value);
    if (JS_VALUE_GET_TAG(ret) == JS_TAG_OBJECT)
        return ret;

    if (value->isValueList())
        ret = JSCSSValueList::create(ctx, static_cast<CSSValueList*>(value));
    else if (value->isPrimitiveValue())
        ret = JSCSSPrimitiveValue::create(ctx, static_cast<CSSPrimitiveValue*>(value));
    else
        ret = JSCSSValue::create(ctx, value);

    QJS::ScriptInterpreter::putDOMObject(value, ret);
    return ret;
}

JSValue toJS(JSContext* ctx, StyleSheet* sheet)
{
    if (!sheet)
        return JS_NULL;
    JSValue ret = QJS::ScriptInterpreter::getDOMObject(sheet);
    if (JS_VALUE_GET_TAG(ret) == JS_TAG_OBJECT)
        return ret;

    if (sheet->isCSSStyleSheet())
        ret = JSCSSStyleSheet::create(ctx, static_cast<CSSStyleSheet*>(sheet));
    else
        ret = JSStyleSheet::create(ctx, sheet);

    QJS::ScriptInterpreter::putDOMObject(sheet, ret);
    return ret;
}

JSValue toJS(JSContext* ctx, Clipboard* clipboard)
{
    if (!clipboard)
        return JS_NULL;
    return JS_NULL;
}

EventTargetNode* toEventTargetNode(JSValue val)
{
    if (!JS_IsObject(val))
        return 0;
    Node* node = static_cast<Node*>(JS_GetOpaque(val, JSNode::js_class_id));
    if (!node || !node->isEventTargetNode())
        return 0;
    return static_cast<EventTargetNode*>(node);
}

bool isSafeScript(JSContext* ctx, JSValue val)
{
    QJS::Window* window = QJS::Window::retrieveActive(ctx);
    if (!window)
        return true; // no window means no cross-frame risk
    return window->isSafeScript(ctx);
}

JSValue getDOMExceptionConstructor(JSContext* ctx)
{
    return JSDOMExceptionConstructor::create(ctx);
}

JSValue createJSHTMLWrapper(JSContext* ctx, PassRefPtr<HTMLElement> element)
{
    if (!element)
        return JS_NULL;

    HTMLElement* e = element.get();
    const AtomicString& tag = e->localName();

    if (tag == HTMLNames::aTag) return JSHTMLAnchorElement::create(ctx, static_cast<HTMLAnchorElement*>(e));
    if (tag == HTMLNames::appletTag) return JSHTMLAppletElement::create(ctx, static_cast<HTMLAppletElement*>(e));
    if (tag == HTMLNames::areaTag) return JSHTMLAreaElement::create(ctx, static_cast<HTMLAreaElement*>(e));
    if (tag == HTMLNames::baseTag) return JSHTMLBaseElement::create(ctx, static_cast<HTMLBaseElement*>(e));
    if (tag == HTMLNames::basefontTag) return JSHTMLBaseFontElement::create(ctx, static_cast<HTMLBaseFontElement*>(e));
    if (tag == HTMLNames::bodyTag) return JSHTMLBodyElement::create(ctx, static_cast<HTMLBodyElement*>(e));
    if (tag == HTMLNames::brTag) return JSHTMLBRElement::create(ctx, static_cast<HTMLBRElement*>(e));
    if (tag == HTMLNames::buttonTag) return JSHTMLButtonElement::create(ctx, static_cast<HTMLButtonElement*>(e));
    if (tag == HTMLNames::canvasTag) return JSHTMLCanvasElement::create(ctx, static_cast<HTMLCanvasElement*>(e));
    if (tag == HTMLNames::divTag) return JSHTMLDivElement::create(ctx, static_cast<HTMLDivElement*>(e));
    if (tag == HTMLNames::dlTag) return JSHTMLDListElement::create(ctx, static_cast<HTMLDListElement*>(e));
    if (tag == HTMLNames::embedTag) return JSHTMLEmbedElement::create(ctx, static_cast<HTMLEmbedElement*>(e));
    if (tag == HTMLNames::fieldsetTag) return JSHTMLFieldSetElement::create(ctx, static_cast<HTMLFieldSetElement*>(e));
    if (tag == HTMLNames::fontTag) return JSHTMLFontElement::create(ctx, static_cast<HTMLFontElement*>(e));
    if (tag == HTMLNames::formTag) return JSHTMLFormElement::create(ctx, static_cast<HTMLFormElement*>(e));
    if (tag == HTMLNames::frameTag) return JSHTMLFrameElement::create(ctx, static_cast<HTMLFrameElement*>(e));
    if (tag == HTMLNames::framesetTag) return JSHTMLFrameSetElement::create(ctx, static_cast<HTMLFrameSetElement*>(e));
    if (tag == HTMLNames::h1Tag || tag == HTMLNames::h2Tag || tag == HTMLNames::h3Tag ||
        tag == HTMLNames::h4Tag || tag == HTMLNames::h5Tag || tag == HTMLNames::h6Tag)
        return JSHTMLHeadingElement::create(ctx, static_cast<HTMLHeadingElement*>(e));
    if (tag == HTMLNames::headTag) return JSHTMLHeadElement::create(ctx, static_cast<HTMLHeadElement*>(e));
    if (tag == HTMLNames::hrTag) return JSHTMLHRElement::create(ctx, static_cast<HTMLHRElement*>(e));
    if (tag == HTMLNames::htmlTag) return JSHTMLHtmlElement::create(ctx, static_cast<HTMLHtmlElement*>(e));
    if (tag == HTMLNames::iframeTag) return JSHTMLIFrameElement::create(ctx, static_cast<HTMLIFrameElement*>(e));
    if (tag == HTMLNames::imgTag || tag == HTMLNames::imageTag)
        return JSHTMLImageElement::create(ctx, static_cast<HTMLImageElement*>(e));
    if (tag == HTMLNames::inputTag) return JSHTMLInputElement::create(ctx, static_cast<HTMLInputElement*>(e));
    if (tag == HTMLNames::labelTag) return JSHTMLLabelElement::create(ctx, static_cast<HTMLLabelElement*>(e));
    if (tag == HTMLNames::legendTag) return JSHTMLLegendElement::create(ctx, static_cast<HTMLLegendElement*>(e));
    if (tag == HTMLNames::liTag) return JSHTMLLIElement::create(ctx, static_cast<HTMLLIElement*>(e));
    if (tag == HTMLNames::linkTag) return JSHTMLLinkElement::create(ctx, static_cast<HTMLLinkElement*>(e));
    if (tag == HTMLNames::mapTag) return JSHTMLMapElement::create(ctx, static_cast<HTMLMapElement*>(e));
    if (tag == HTMLNames::marqueeTag) return JSHTMLMarqueeElement::create(ctx, static_cast<HTMLMarqueeElement*>(e));
    if (tag == HTMLNames::metaTag) return JSHTMLMetaElement::create(ctx, static_cast<HTMLMetaElement*>(e));
    if (tag == HTMLNames::objectTag) return JSHTMLObjectElement::create(ctx, static_cast<HTMLObjectElement*>(e));
    if (tag == HTMLNames::olTag) return JSHTMLOListElement::create(ctx, static_cast<HTMLOListElement*>(e));
    if (tag == HTMLNames::optgroupTag) return JSHTMLOptGroupElement::create(ctx, static_cast<HTMLOptGroupElement*>(e));
    if (tag == HTMLNames::optionTag) return JSHTMLOptionElement::create(ctx, static_cast<HTMLOptionElement*>(e));
    if (tag == HTMLNames::pTag) return JSHTMLParagraphElement::create(ctx, static_cast<HTMLParagraphElement*>(e));
    if (tag == HTMLNames::paramTag) return JSHTMLParamElement::create(ctx, static_cast<HTMLParamElement*>(e));
    if (tag == HTMLNames::preTag || tag == HTMLNames::listingTag || tag == HTMLNames::xmpTag)
        return JSHTMLPreElement::create(ctx, static_cast<HTMLPreElement*>(e));
    if (tag == HTMLNames::scriptTag) return JSHTMLScriptElement::create(ctx, static_cast<HTMLScriptElement*>(e));
    if (tag == HTMLNames::selectTag || tag == HTMLNames::keygenTag)
        return JSHTMLSelectElement::create(ctx, static_cast<HTMLSelectElement*>(e));
    if (tag == HTMLNames::styleTag) return JSHTMLStyleElement::create(ctx, static_cast<HTMLStyleElement*>(e));
    if (tag == HTMLNames::captionTag) return JSHTMLTableCaptionElement::create(ctx, static_cast<HTMLTableCaptionElement*>(e));
    if (tag == HTMLNames::tdTag || tag == HTMLNames::thTag)
        return JSHTMLTableCellElement::create(ctx, static_cast<HTMLTableCellElement*>(e));
    if (tag == HTMLNames::colTag || tag == HTMLNames::colgroupTag)
        return JSHTMLTableColElement::create(ctx, static_cast<HTMLTableColElement*>(e));
    if (tag == HTMLNames::tableTag) return JSHTMLTableElement::create(ctx, static_cast<HTMLTableElement*>(e));
    if (tag == HTMLNames::trTag) return JSHTMLTableRowElement::create(ctx, static_cast<HTMLTableRowElement*>(e));
    if (tag == HTMLNames::tbodyTag || tag == HTMLNames::tfootTag || tag == HTMLNames::theadTag)
        return JSHTMLTableSectionElement::create(ctx, static_cast<HTMLTableSectionElement*>(e));
    if (tag == HTMLNames::textareaTag) return JSHTMLTextAreaElement::create(ctx, static_cast<HTMLTextAreaElement*>(e));
    if (tag == HTMLNames::titleTag) return JSHTMLTitleElement::create(ctx, static_cast<HTMLTitleElement*>(e));
    if (tag == HTMLNames::ulTag) return JSHTMLUListElement::create(ctx, static_cast<HTMLUListElement*>(e));

    return JSHTMLElement::create(ctx, e);
}

}
