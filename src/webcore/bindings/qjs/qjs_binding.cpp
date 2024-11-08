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
#include "Event.h"
#include "EventNames.h"
#include "Frame.h"
#include "Page.h"
#include "Range.h"
#include "RangeException.h"
#include "XMLHttpRequest.h"
#include "QJSNode.h"
#include "QJSDOMWindow.h"

#include <text/String.h>
#include "qjs_dom.h"
#include "qjs_window.h"
#include "qjs_script.h"

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
    domObjects()->set(objectHandle, obj);
}

void ScriptInterpreter::forgetDOMObject(void* objectHandle)
{
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
    if (!document) {
        domObjects()->remove(node);
        return;
    }
    NodeMap* documentDict = domNodesPerDocument()->get(document);
    if (documentDict)
        documentDict->remove(node);
}

void ScriptInterpreter::putDOMNodeForDocument(Document* document, Node* node, JSValue obj)
{
    if (!document) {
        domObjects()->set(node, obj);
        return;
    }
    NodeMap* documentDict = domNodesPerDocument()->get(document);
    if (!documentDict) {
        documentDict = new NodeMap;
        domNodesPerDocument()->set(document, documentDict);
    }
    documentDict->set(node, obj);
}

void ScriptInterpreter::forgetAllDOMNodesForDocument(Document* document)
{
    ASSERT(document);
    NodePerDocMap::iterator it = domNodesPerDocument()->find(document);
    if (it != domNodesPerDocument()->end()) {
        delete it->second;
        domNodesPerDocument()->remove(it);
    }
}

JSValue ScriptInterpreter::globalObject() const
{
    // we need to make sure that any script execution happening in this
    // frame does not destroy it
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

}
