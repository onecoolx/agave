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
#include "QJSNode.h"

#include "Attr.h"
#include "CDATASection.h"
#include "Comment.h"
#include "Document.h"
#include "DocumentFragment.h"
#include "DocumentType.h"
#include "Entity.h"
#include "EntityReference.h"
#include "HTMLElement.h"
#include "QJSAttr.h"
#include "QJSCDATASection.h"
#include "QJSComment.h"
#include "QJSDocument.h"
#include "QJSDocumentFragment.h"
#include "QJSDocumentType.h"
#include "QJSEntity.h"
#include "QJSEntityReference.h"
#include "QJSHTMLElement.h"
#include "QJSHTMLElementWrapperFactory.h"
#include "QJSNotation.h"
#include "QJSProcessingInstruction.h"
#include "QJSText.h"
#include "Node.h"
#include "Notation.h"
#include "ProcessingInstruction.h"
#include "Text.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

#if ENABLE(SVG)
#include "JSSVGElementWrapperFactory.h"
#include "SVGElement.h"
#endif

namespace WebCore {

typedef int ExpectionCode;

JSValue JSNode::insertBefore(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, Node *impl)
{
    ExceptionCode ec = 0;
    bool ok = impl->insertBefore(toNode(argv[0]), toNode(argv[1]), ec);
    QJS::setDOMException(ctx, ec);
    if (ok)
        return argv[0];
    return JS_NULL;
}

JSValue JSNode::replaceChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, Node *impl)
{
    ExceptionCode ec = 0;
    bool ok = impl->replaceChild(toNode(argv[0]), toNode(argv[1]), ec);
    QJS::setDOMException(ctx, ec);
    if (ok)
        return argv[1];
    return JS_NULL;
}

JSValue JSNode::removeChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, Node *impl)
{
    ExceptionCode ec = 0;
    bool ok = impl->removeChild(toNode(argv[0]), ec);
    QJS::setDOMException(ctx, ec);
    if (ok)
        return argv[0];
    return JS_NULL;
}

JSValue JSNode::appendChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, Node *impl)
{
    ExceptionCode ec = 0;
    bool ok = impl->appendChild(toNode(argv[0]), ec);
    QJS::setDOMException(ctx, ec);
    if (ok)
        return argv[0];
    return JS_NULL;
}

void JSNode::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    Node* node = (Node*)JS_GetOpaque(val, JSNode::js_class_id);

    if (node->inDocument()) {
        JS_MarkValue(rt, val, mark_func);
        return;
    }

    Node* root = node;
    for (Node* current = node; current; current = current->parentNode())
        root = current;

    if (root->m_inSubtreeMark) {
        JS_MarkValue(rt, val, mark_func);
        return;
    }

    root->m_inSubtreeMark = true;
    for (Node* nodeToMark = root; nodeToMark; nodeToMark = nodeToMark->traverseNextNode()) {
        JSValue wrapper = QJS::ScriptInterpreter::getDOMNodeForDocument(node->document(), nodeToMark);
        if (!JS_IsNull(wrapper)) {
            if (JS_IsLiveObject(rt, wrapper))
                JS_MarkValue(rt, wrapper, mark_func);
        } else if (nodeToMark == node) {
            // This is the case where the map from the document to wrappers has
            // been cleared out, but a wrapper is being marked. For now, we'll
            // let the rest of the tree of wrappers get collected, because we have
            // no good way of finding them. Later we should test behavior of other
            // browsers and see if we need to preserve other wrappers in this case.
            if (JS_IsLiveObject(rt, val))
                JS_MarkValue(rt, val, mark_func);
        }
    }
    root->m_inSubtreeMark = false;
}

JSValue toJS(JSContext *ctx, PassRefPtr<Node> n)
{
    Node* node = n.get(); 
    if (!node)
        return JS_NULL;

    Document* doc = node->document();
    JSValue ret = QJS::ScriptInterpreter::getDOMNodeForDocument(doc, node);
    if (!JS_IsNull(ret)) {
        return ret;
    }

    switch (node->nodeType()) {
        case Node::ELEMENT_NODE:
            if (node->isHTMLElement())
                ret = createJSHTMLWrapper(ctx, static_pointer_cast<HTMLElement>(n));
#if ENABLE(SVG)
            else if (node->isSVGElement())
                ret = createJSSVGWrapper(ctx, static_pointer_cast<SVGElement>(n));
#endif
            else
                ret = JSElement::create(ctx, static_cast<Element*>(node));
            break;
        case Node::ATTRIBUTE_NODE:
            ret = JSAttr::create(ctx, static_cast<Attr*>(node));
            break;
        case Node::TEXT_NODE:
            ret = JSText::create(ctx, static_cast<Text*>(node));
            break;
        case Node::CDATA_SECTION_NODE:
            ret = JSCDATASection::create(ctx, static_cast<CDATASection*>(node));
            break;
        case Node::ENTITY_NODE:
            ret = JSEntity::create(ctx, static_cast<Entity*>(node));
            break;
        case Node::PROCESSING_INSTRUCTION_NODE:
            ret = JSProcessingInstruction::create(ctx, static_cast<ProcessingInstruction*>(node));
            break;
        case Node::COMMENT_NODE:
            ret = JSComment::create(ctx, static_cast<Comment*>(node));
            break;
        case Node::DOCUMENT_NODE:
            // we don't want to cache the document itself in the per-document dictionary
            return toJS(ctx, static_cast<Document*>(node));
        case Node::DOCUMENT_TYPE_NODE:
            ret = JSDocumentType::create(ctx, static_cast<DocumentType*>(node));
            break;
        case Node::NOTATION_NODE:
            ret = JSNotation::create(ctx, static_cast<Notation*>(node));
            break;
        case Node::DOCUMENT_FRAGMENT_NODE:
            ret = JSDocumentFragment::create(ctx, static_cast<DocumentFragment*>(node));
            break;
        case Node::ENTITY_REFERENCE_NODE:
            ret = JSEntityReference::create(ctx, static_cast<EntityReference*>(node));
            break;
        default:
            ret = JSNode::create(ctx, node);
    }

    QJS::ScriptInterpreter::putDOMNodeForDocument(doc, node, ret);
    return ret;
}

} // namespace WebCore
#endif
