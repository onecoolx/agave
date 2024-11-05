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

#if ENABLE(QJS)
#include "QJSElement.h"

#include "Attr.h"
#include "Document.h"
#include "Element.h"
#include "ExceptionCode.h"
#include "HTMLFrameElementBase.h"
#include "HTMLNames.h"
#include <text/String.h>
#include "qjs_binding.h"
#include "qjs_dom.h"

using namespace QJS;

namespace WebCore {

using namespace HTMLNames;

static inline bool allowSettingSrcToJavascriptURL(JSContext* ctx, Element* element, String name, String value)
{
    if ((element->hasTagName(iframeTag) || element->hasTagName(frameTag)) && equalIgnoringCase(name, "src")
        && value.startsWith("javascript:", false)) {
        HTMLFrameElementBase* frame = static_cast<HTMLFrameElementBase*>(element);
        if (!checkNodeSecurity(ctx, frame->contentDocument()))
            return false;
    }
    return true;
} 

JSValue JSElement::setAttribute(JSContext *ctx, int argc, JSValueConst *argv, Element *imp)
{
    ExceptionCode ec = 0;
    String name = valueToString(ctx, argv[0]);
    String value = valueToString(ctx, argv[1]);

    if (!allowSettingSrcToJavascriptURL(ctx, imp, name, value))
        return JS_UNDEFINED;

    imp->setAttribute(name, value, ec);
    setDOMException(ctx, ec);
    return JS_UNDEFINED;
}

JSValue JSElement::setAttributeNode(JSContext *ctx, int argc, JSValueConst *argv, Element *imp)
{
    ExceptionCode ec = 0;
    bool newAttrOk;
    Attr* newAttr = toAttr(argv[0], newAttrOk);
    if (!newAttrOk) {
        setDOMException(ctx, TYPE_MISMATCH_ERR);
        return JS_UNDEFINED;
    }

    if (!allowSettingSrcToJavascriptURL(ctx, imp, newAttr->name(), newAttr->value()))
        return JS_UNDEFINED;

    JSValue result = toJS(ctx, QJS::getPtr(imp->setAttributeNode(newAttr, ec)));
    setDOMException(ctx, ec);
    return result;
}

JSValue JSElement::setAttributeNS(JSContext *ctx, int argc, JSValueConst *argv, Element *imp)
{
    ExceptionCode ec = 0;
    String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
    String qualifiedName = valueToString(ctx, argv[1]);
    String value = valueToString(ctx, argv[2]);

    if (!allowSettingSrcToJavascriptURL(ctx, imp, qualifiedName, value))
        return JS_UNDEFINED;

    imp->setAttributeNS(namespaceURI, qualifiedName, value, ec);
    setDOMException(ctx, ec);
    return JS_UNDEFINED;
}

JSValue JSElement::setAttributeNodeNS(JSContext *ctx, int argc, JSValueConst *argv, Element *imp)
{
    ExceptionCode ec = 0;
    bool newAttrOk;
    Attr* newAttr = toAttr(argv[0], newAttrOk);
    if (!newAttrOk) {
        setDOMException(ctx, TYPE_MISMATCH_ERR);
        return JS_UNDEFINED;
    }

    if (!allowSettingSrcToJavascriptURL(ctx, imp, newAttr->name(), newAttr->value()))
        return JS_UNDEFINED;

    JSValue result = toJS(ctx, QJS::getPtr(imp->setAttributeNodeNS(newAttr, ec)));
    setDOMException(ctx, ec);
    return result;
}

} // namespace WebCore
#endif
