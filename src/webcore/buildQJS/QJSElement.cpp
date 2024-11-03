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

#include "QJSElement.h"

#include "Attr.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "Element.h"
#include "ExceptionCode.h"
#include "NameNodeList.h"
#include "Node.h"
#include "NodeList.h"
#include "QJSAttr.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSElement.h"
#include "QJSNode.h"
#include "QJSNodeList.h"
#include "qjs_dom.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("clientLeft", JSElement::getValueProperty, NULL, JSElement::ClientLeftAttrNum),
    JS_CGETSET_MAGIC_DEF("tagName", JSElement::getValueProperty, NULL, JSElement::TagNameAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetWidth", JSElement::getValueProperty, NULL, JSElement::OffsetWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetParent", JSElement::getValueProperty, NULL, JSElement::OffsetParentAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollWidth", JSElement::getValueProperty, NULL, JSElement::ScrollWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetTop", JSElement::getValueProperty, NULL, JSElement::OffsetTopAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetLeft", JSElement::getValueProperty, NULL, JSElement::OffsetLeftAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSElement::getValueProperty, NULL, JSElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollLeft", JSElement::getValueProperty, JSElement::putValueProperty, JSElement::ScrollLeftAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetHeight", JSElement::getValueProperty, NULL, JSElement::OffsetHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("clientTop", JSElement::getValueProperty, NULL, JSElement::ClientTopAttrNum),
    JS_CGETSET_MAGIC_DEF("clientWidth", JSElement::getValueProperty, NULL, JSElement::ClientWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("clientHeight", JSElement::getValueProperty, NULL, JSElement::ClientHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollTop", JSElement::getValueProperty, JSElement::putValueProperty, JSElement::ScrollTopAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollHeight", JSElement::getValueProperty, NULL, JSElement::ScrollHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSElement::getValueProperty, NULL, JSElement::ConstructorAttrNum)
};

class JSElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Element.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Element.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("removeAttributeNode", 1, JSElementPrototypeFunction::callAsFunction, JSElement::RemoveAttributeNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("removeAttribute", 1, JSElementPrototypeFunction::callAsFunction, JSElement::RemoveAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("getAttributeNode", 1, JSElementPrototypeFunction::callAsFunction, JSElement::GetAttributeNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("getAttributeNodeNS", 2, JSElementPrototypeFunction::callAsFunction, JSElement::GetAttributeNodeNSFuncNum),
    JS_CFUNC_MAGIC_DEF("getElementsByTagName", 1, JSElementPrototypeFunction::callAsFunction, JSElement::GetElementsByTagNameFuncNum),
    JS_CFUNC_MAGIC_DEF("getElementsByTagNameNS", 2, JSElementPrototypeFunction::callAsFunction, JSElement::GetElementsByTagNameNSFuncNum),
    JS_CFUNC_MAGIC_DEF("getAttributeNS", 2, JSElementPrototypeFunction::callAsFunction, JSElement::GetAttributeNSFuncNum),
    JS_CFUNC_MAGIC_DEF("setAttributeNode", 1, JSElementPrototypeFunction::callAsFunction, JSElement::SetAttributeNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("scrollByLines", 1, JSElementPrototypeFunction::callAsFunction, JSElement::ScrollByLinesFuncNum),
    JS_CFUNC_MAGIC_DEF("setAttribute", 2, JSElementPrototypeFunction::callAsFunction, JSElement::SetAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("scrollByPages", 1, JSElementPrototypeFunction::callAsFunction, JSElement::ScrollByPagesFuncNum),
    JS_CFUNC_MAGIC_DEF("removeAttributeNS", 2, JSElementPrototypeFunction::callAsFunction, JSElement::RemoveAttributeNSFuncNum),
    JS_CFUNC_MAGIC_DEF("setAttributeNS", 3, JSElementPrototypeFunction::callAsFunction, JSElement::SetAttributeNSFuncNum),
    JS_CFUNC_MAGIC_DEF("setAttributeNodeNS", 1, JSElementPrototypeFunction::callAsFunction, JSElement::SetAttributeNodeNSFuncNum),
    JS_CFUNC_MAGIC_DEF("getAttribute", 1, JSElementPrototypeFunction::callAsFunction, JSElement::GetAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("scrollIntoView", 1, JSElementPrototypeFunction::callAsFunction, JSElement::ScrollIntoViewFuncNum),
    JS_CFUNC_MAGIC_DEF("hasAttribute", 1, JSElementPrototypeFunction::callAsFunction, JSElement::HasAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasAttributeNS", 2, JSElementPrototypeFunction::callAsFunction, JSElement::HasAttributeNSFuncNum),
    JS_CFUNC_MAGIC_DEF("focus", 0, JSElementPrototypeFunction::callAsFunction, JSElement::FocusFuncNum),
    JS_CFUNC_MAGIC_DEF("blur", 0, JSElementPrototypeFunction::callAsFunction, JSElement::BlurFuncNum),
    JS_CFUNC_MAGIC_DEF("insertAdjacentElement", 2, JSElementPrototypeFunction::callAsFunction, JSElement::InsertAdjacentElementFuncNum),
    JS_CFUNC_MAGIC_DEF("contains", 1, JSElementPrototypeFunction::callAsFunction, JSElement::ContainsFuncNum),
    JS_CFUNC_MAGIC_DEF("scrollIntoViewIfNeeded", 1, JSElementPrototypeFunction::callAsFunction, JSElement::ScrollIntoViewIfNeededFuncNum)
};

JSValue JSElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSElementAttributesFunctions, countof(JSElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSElementPrototypeFunctions, countof(JSElementPrototypeFunctions));
}

static JSClassDef JSElementClassDefine = 
{
    "Element",
    .finalizer = JSElement::finalizer,
    .gc_mark = JSElement::mark,
};

JSClassID JSElement::js_class_id = 0;

void JSElement::init(JSContext* ctx)
{
    if (JSElement::js_class_id == 0) {
        JS_NewClassID(&JSElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSElement::js_class_id, &JSElementClassDefine);
        JS_SetConstructor(ctx, JSElementConstructor::self(ctx), JSElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSElement::js_class_id, JSElementPrototype::self(ctx));
    }
}

JSValue JSElement::create(JSContext* ctx, Element* impl)
{
    JSElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSElementPrototype::self(ctx), JSElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSElement::finalizer(JSRuntime* rt, JSValue val)
{
    Element* impl = (Element*)JS_GetOpaque(val, JSElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
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
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return jsStringOrNull(ctx, imp->tagName());
        }
        case StyleAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case OffsetLeftAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->offsetLeft());
        }
        case OffsetTopAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->offsetTop());
        }
        case OffsetWidthAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->offsetWidth());
        }
        case OffsetHeightAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->offsetHeight());
        }
        case OffsetParentAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->offsetParent()));
        }
        case ClientLeftAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->clientLeft());
        }
        case ClientTopAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->clientTop());
        }
        case ClientWidthAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->clientWidth());
        }
        case ClientHeightAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->clientHeight());
        }
        case ScrollLeftAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollLeft());
        }
        case ScrollTopAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollTop());
        }
        case ScrollWidthAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollWidth());
        }
        case ScrollHeightAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollHeight());
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
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
            imp->setScrollLeft(valueToInt32(ctx, value));
            break;
        }
        case ScrollTopAttrNum: {
            Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
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
    Element* imp = (Element*)JS_GetOpaque2(ctx, this_val, JSElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSElement::GetAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->getAttribute(name));
            return result;
        }
        case JSElement::SetAttributeFuncNum: {
            return JSElement::setAttribute(ctx, argc, argv, imp);
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
            return JSElement::setAttributeNode(ctx, argc, argv, imp);
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
        case JSElement::GetAttributeNSFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);

            JSValue result = JS_NewString(ctx, ((const String&)imp->getAttributeNS(namespaceURI, localName)).utf8().data());
            return result;
        }
        case JSElement::SetAttributeNSFuncNum: {
            return JSElement::setAttributeNS(ctx, argc, argv, imp);
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
            return JSElement::setAttributeNodeNS(ctx, argc, argv, imp);
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
