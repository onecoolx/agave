/*
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "markup.h"

#include "CDATASection.h"
#include "CSSComputedStyleDeclaration.h"
#include "CSSPropertyNames.h"
#include "CSSRule.h"
#include "CSSRuleList.h"
#include "CSSStyleRule.h"
#include "CSSStyleSelector.h"
#include "CSSValueKeywords.h"
#include "Comment.h"
#include "DeleteButtonController.h"
#include "DeprecatedStringList.h"
#include "Document.h"
#include "DocumentFragment.h"
#include "DocumentType.h"
#include "Editor.h"
#include "Frame.h"
#include "HTMLElement.h"
#include "HTMLNames.h"
#include "InlineTextBox.h"
#include "KURL.h"
#include "Logging.h"
#include "ProcessingInstruction.h"
#include "QualifiedName.h"
#include "Range.h"
#include "Selection.h"
#include "TextIterator.h"
#include "htmlediting.h"
#include "visible_units.h"

using namespace std;

namespace WebCore {

using namespace HTMLNames;

static inline bool shouldSelfClose(const Node *node);

class AttributeChange {
public:
    AttributeChange()
        : m_name(nullAtom, nullAtom, nullAtom)
    {
    }

    AttributeChange(PassRefPtr<Element> element, const QualifiedName& name, const String& value)
        : m_element(element), m_name(name), m_value(value)
    {
    }

    void apply()
    {
        m_element->setAttribute(m_name, m_value);
    }

private:
    RefPtr<Element> m_element;
    QualifiedName m_name;
    String m_value;
};

static inline void appendString(Vector<UChar>& result, const String& str)
{
    result.append(str.characters(), str.length());
}
    
static void appendAttributeValue(Vector<UChar>& result, const String& attr)
{
    const UChar* uchars = attr.characters();
    unsigned len = attr.length();
    unsigned lastCopiedFrom = 0;

    static const String ampEntity("&amp;");
    static const String ltEntity("&lt;");
    static const String quotEntity("&quot;");
    
    for (unsigned i = 0; i < len; ++i) {
        UChar c = uchars[i];
        switch (c) {
            case '&':
                result.append(uchars + lastCopiedFrom, i - lastCopiedFrom);
                appendString(result, ampEntity);
                lastCopiedFrom = i + 1;
                break;
            case '<':
                result.append(uchars + lastCopiedFrom, i - lastCopiedFrom);
                appendString(result, ltEntity);
                lastCopiedFrom = i + 1;
                break;
            case '"':
                result.append(uchars + lastCopiedFrom, i - lastCopiedFrom);
                appendString(result, quotEntity);
                lastCopiedFrom = i + 1;
        }
    }
    
    result.append(uchars + lastCopiedFrom, len - lastCopiedFrom);
}
    
static DeprecatedString escapeContentText(const String& in)
{
    DeprecatedString s = "";

    unsigned len = in.length();
    unsigned lastCopiedFrom = 0;

    const UChar* uchars = in.characters();
    const DeprecatedChar* dchars = reinterpret_cast<const DeprecatedChar*>(uchars);

    for (unsigned i = 0; i < len; ++i) {
        UChar c = uchars[i];
        if ((c == '&') | (c == '<')) {
            s.append(dchars + lastCopiedFrom, i - lastCopiedFrom);
            if (c == '&')
                s += "&amp;";
            else 
                s += "&lt;";
            lastCopiedFrom = i + 1;
        }
    }

    s.append(dchars + lastCopiedFrom, len - lastCopiedFrom);

    return s;
}

    
static void appendEscapedContent(Vector<UChar>& result, pair<const UChar*, size_t> range)
{
    const UChar* uchars = range.first;
    unsigned len = range.second;
    unsigned lastCopiedFrom = 0;
    
    static const String ampEntity("&amp;");
    static const String ltEntity("&lt;");
    
    for (unsigned i = 0; i < len; ++i) {
        UChar c = uchars[i];
        if ((c == '&') | (c == '<')) {
            result.append(uchars + lastCopiedFrom, i - lastCopiedFrom);
            if (c == '&')
                appendString(result, ampEntity);
            else 
                appendString(result, ltEntity);
            lastCopiedFrom = i + 1;
        }
    }
    
    result.append(uchars + lastCopiedFrom, len - lastCopiedFrom);
}    

static inline void appendDeprecatedString(Vector<UChar>& result, const DeprecatedString& str)
{
    result.append(reinterpret_cast<const UChar*>(str.unicode()), str.length());
}    
    
static void appendQuotedURLAttributeValue(Vector<UChar>& result, String urlString)
{
    UChar quoteChar = '\"';
    if (urlString.stripWhiteSpace().startsWith("javascript:", false)) {
        // minimal escaping for javascript urls
        if (urlString.contains('\"')) {
            if (urlString.contains('\''))
                urlString.replace('\"', "&quot;");
            else
                quoteChar = '\'';
        }
        result.append(quoteChar);
        appendString(result, urlString);
        result.append(quoteChar);
        return;
    }

    // FIXME this does not fully match other browsers. Firefox escapes spaces and other special characters.
    result.append(quoteChar);
    appendAttributeValue(result, urlString);
    result.append(quoteChar);    
}
    
static String stringValueForRange(const Node *node, const Range *range)
{
    if (!range)
        return node->nodeValue();

    String str = node->nodeValue().copy();
    ExceptionCode ec;
    if (node == range->endContainer(ec))
        str.truncate(range->endOffset(ec));
    if (node == range->startContainer(ec))
        str.remove(0, range->startOffset(ec));
    return str;
}

static inline pair<const UChar*, size_t> ucharRange(const Node *node, const Range *range)
{
    String str = node->nodeValue();
    const UChar* characters = str.characters();
    size_t length = str.length();

    if (range) {
        ExceptionCode ec;
        if (node == range->endContainer(ec))
            length = range->endOffset(ec);
        if (node == range->startContainer(ec)) {
            size_t start = range->startOffset(ec);
            characters += start;
            length -= start;
        }
    }
    
    return make_pair(characters, length);
}
    
static inline void appendUCharRange(Vector<UChar>& result, const pair<const UChar*, size_t> range)
{
    result.append(range.first, range.second);
}
    
static String renderedText(const Node* node, const Range* range)
{
    if (!node->isTextNode())
        return String();

    ExceptionCode ec;
    const Text* textNode = static_cast<const Text*>(node);
    unsigned startOffset = 0;
    unsigned endOffset = textNode->length();

    if (range && node == range->startContainer(ec))
        startOffset = range->startOffset(ec);
    if (range && node == range->endContainer(ec))
        endOffset = range->endOffset(ec);
    
    Position start(const_cast<Node*>(node), startOffset);
    Position end(const_cast<Node*>(node), endOffset);
    Range r(node->document(), start, end);
    return plainText(&r);
}

static PassRefPtr<CSSMutableStyleDeclaration> styleFromMatchedRulesForElement(Element* element, bool authorOnly = true)
{
    RefPtr<CSSMutableStyleDeclaration> style = new CSSMutableStyleDeclaration();
    RefPtr<CSSRuleList> matchedRules = element->document()->styleSelector()->styleRulesForElement(element, authorOnly);
    if (matchedRules) {
        for (unsigned i = 0; i < matchedRules->length(); i++) {
            if (matchedRules->item(i)->type() == CSSRule::STYLE_RULE) {
                RefPtr<CSSMutableStyleDeclaration> s = static_cast<CSSStyleRule*>(matchedRules->item(i))->style();
                style->merge(s.get(), true);
            }
        }
    }
    
    return style.release();
}

static void removeEnclosingMailBlockquoteStyle(CSSMutableStyleDeclaration* style, Node* node)
{
    Node* blockquote = nearestMailBlockquote(node);
    if (!blockquote || !blockquote->parentNode())
        return;
            
    RefPtr<CSSMutableStyleDeclaration> parentStyle = Position(blockquote->parentNode(), 0).computedStyle()->copyInheritableProperties();
    RefPtr<CSSMutableStyleDeclaration> blockquoteStyle = Position(blockquote, 0).computedStyle()->copyInheritableProperties();
    parentStyle->diff(blockquoteStyle.get());
    blockquoteStyle->diff(style);
}

static bool shouldAddNamespaceElem(const Element* elem)
{
    // Don't add namespace attribute if it is already defined for this elem.
    const AtomicString& prefix = elem->prefix();
    AtomicString attr = !prefix.isEmpty() ? "xmlns:" + prefix : "xmlns";
    return !elem->hasAttribute(attr);
}

static bool shouldAddNamespaceAttr(const Attribute* attr, HashMap<AtomicStringImpl*, AtomicStringImpl*>& namespaces)
{
    // Don't add namespace attributes twice
    static const AtomicString xmlnsURI = "http://www.w3.org/2000/xmlns/";
    static const QualifiedName xmlnsAttr(nullAtom, "xmlns", xmlnsURI);
    if (attr->name() == xmlnsAttr) {
        namespaces.set(emptyAtom.impl(), attr->value().impl());
        return false;
    }
    
    QualifiedName xmlnsPrefixAttr("xmlns", attr->localName(), xmlnsURI);
    if (attr->name() == xmlnsPrefixAttr) {
        namespaces.set(attr->localName().impl(), attr->value().impl());
        return false;
    }
    
    return true;
}

static void appendNamespace(Vector<UChar>& result, const AtomicString& prefix, const AtomicString& ns, HashMap<AtomicStringImpl*, AtomicStringImpl*>& namespaces)
{
    if (ns.isEmpty())
        return;
        
    // Use emptyAtoms's impl() for both null and empty strings since the HashMap can't handle 0 as a key
    AtomicStringImpl* pre = prefix.isEmpty() ? emptyAtom.impl() : prefix.impl();
    AtomicStringImpl* foundNS = namespaces.get(pre);
    if (foundNS != ns.impl()) {
        namespaces.set(pre, ns.impl());
        static const String xmlns("xmlns");
        result.append(' ');
        appendString(result, xmlns);
        if (!prefix.isEmpty()) {
            result.append(':');
            appendString(result, prefix);
        }

        result.append('=');
        result.append('"');
        appendAttributeValue(result, ns);
        result.append('"');
    }
}
    
static void appendStartMarkup(Vector<UChar>& result, const Node *node, const Range *range, EAnnotateForInterchange annotate, bool convertBlocksToInlines = false, HashMap<AtomicStringImpl*, AtomicStringImpl*>* namespaces = 0)
{
    bool documentIsHTML = node->document()->isHTMLDocument();
    switch (node->nodeType()) {
        case Node::TEXT_NODE: {
            if (Node* parent = node->parentNode()) {
                if (parent->hasTagName(listingTag)
                    || parent->hasTagName(scriptTag)
                    || parent->hasTagName(styleTag)
                    || parent->hasTagName(textareaTag)
                    || parent->hasTagName(xmpTag)) {
                    appendUCharRange(result, ucharRange(node, range));
                    break;
                }
            }
            if (!annotate) {
                appendEscapedContent(result, ucharRange(node, range));
                break;
            }
            
            bool useRenderedText = !enclosingNodeWithTag(const_cast<Node*>(node), selectTag);
            DeprecatedString markup = escapeContentText(useRenderedText ? renderedText(node, range) : stringValueForRange(node, range));
            if (annotate)
                markup = convertHTMLTextToInterchangeFormat(markup, static_cast<const Text*>(node));
            appendDeprecatedString(result, markup);
            break;
        }
        case Node::COMMENT_NODE:
            appendString(result, static_cast<const Comment*>(node)->toString());
            break;
        case Node::DOCUMENT_NODE: {
            // FIXME: I think the comment below (and therefore this code) is wrong now
            // Documents do not normally contain a docType as a child node, force it to print here instead.
            const DocumentType* docType = static_cast<const Document*>(node)->doctype();
            if (docType)
                appendString(result, docType->toString());
            break;
        }
        case Node::DOCUMENT_FRAGMENT_NODE:
            break;
        case Node::DOCUMENT_TYPE_NODE:
            appendString(result, static_cast<const DocumentType*>(node)->toString());
            break;
        case Node::PROCESSING_INSTRUCTION_NODE:
            appendString(result, static_cast<const ProcessingInstruction*>(node)->toString());
            break;
        case Node::ELEMENT_NODE: {
            result.append('<');
            const Element* el = static_cast<const Element*>(node);
            bool convert = convertBlocksToInlines & isBlock(const_cast<Node*>(node));
            appendString(result, el->nodeNamePreservingCase());
            NamedAttrMap *attrs = el->attributes();
            unsigned length = attrs->length();
            if (!documentIsHTML && namespaces && shouldAddNamespaceElem(el))
                appendNamespace(result, el->prefix(), el->namespaceURI(), *namespaces);

            for (unsigned int i = 0; i < length; i++) {
                Attribute *attr = attrs->attributeItem(i);
                // We'll handle the style attribute separately, below.
                if (attr->name() == styleAttr && el->isHTMLElement() && (annotate || convert))
                    continue;
                result.append(' ');

                if (documentIsHTML)
                    appendString(result, attr->name().localName());
                else
                    appendString(result, attr->name().toString());

                result.append('=');

                if (el->isURLAttribute(attr))
                    appendQuotedURLAttributeValue(result, attr->value());
                else {
                    result.append('\"');
                    appendAttributeValue(result, attr->value());
                    result.append('\"');
                }

                if (!documentIsHTML && namespaces && shouldAddNamespaceAttr(attr, *namespaces))
                    appendNamespace(result, attr->prefix(), attr->namespaceURI(), *namespaces);
            }
            
            if (el->isHTMLElement() && (annotate || convert)) {
                Element* element = const_cast<Element*>(el);
                RefPtr<CSSMutableStyleDeclaration> style = static_cast<HTMLElement*>(element)->getInlineStyleDecl()->copy();
                if (annotate) {
                    RefPtr<CSSMutableStyleDeclaration> styleFromMatchedRules = styleFromMatchedRulesForElement(const_cast<Element*>(el));
                    style->merge(styleFromMatchedRules.get());
                }
                if (convert)
                    style->setProperty(CSS_PROP_DISPLAY, CSS_VAL_INLINE, true);
                if (style->length() > 0) {
                    static const String stylePrefix(" style=\"");
                    appendString(result, stylePrefix);
                    appendAttributeValue(result, style->cssText());
                    result.append('\"');
                }
            }
            
            if (shouldSelfClose(el)) {
                if (el->isHTMLElement())
                    result.append(' '); // XHTML 1.0 <-> HTML compatibility.
                result.append('/');
            }
            result.append('>');
            break;
        }
        case Node::CDATA_SECTION_NODE:
            appendString(result, static_cast<const CDATASection*>(node)->toString());
            break;
        case Node::ATTRIBUTE_NODE:
        case Node::ENTITY_NODE:
        case Node::ENTITY_REFERENCE_NODE:
        case Node::NOTATION_NODE:
        case Node::XPATH_NAMESPACE_NODE:
            ASSERT_NOT_REACHED();
            break;
    }
}

static String getStartMarkup(const Node *node, const Range *range, EAnnotateForInterchange annotate, bool convertBlocksToInlines = false, HashMap<AtomicStringImpl*, AtomicStringImpl*>* namespaces = 0)
{
    Vector<UChar> result;
    appendStartMarkup(result, node, range, annotate, convertBlocksToInlines, namespaces);
    return String::adopt(result);
}

static inline bool doesHTMLForbidEndTag(const Node *node)
{
    if (node->isHTMLElement()) {
        const HTMLElement* htmlElt = static_cast<const HTMLElement*>(node);
        return (htmlElt->endTagRequirement() == TagStatusForbidden);
    }
    return false;
}

// Rules of self-closure
// 1. No elements in HTML documents use the self-closing syntax.
// 2. Elements w/ children never self-close because they use a separate end tag.
// 3. HTML elements which do not have a "forbidden" end tag will close with a separate end tag.
// 4. Other elements self-close.
static inline bool shouldSelfClose(const Node *node)
{
    if (node->document()->isHTMLDocument())
        return false;
    if (node->hasChildNodes())
        return false;
    if (node->isHTMLElement() && !doesHTMLForbidEndTag(node))
        return false;
    return true;
}

static void appendEndMarkup(Vector<UChar>& result, const Node* node)
{
    if (!node->isElementNode() || shouldSelfClose(node) || (!node->hasChildNodes() && doesHTMLForbidEndTag(node)))
        return;

    result.append('<');
    result.append('/');
    appendString(result, static_cast<const Element*>(node)->nodeNamePreservingCase());
    result.append('>');
}

static String getEndMarkup(const Node *node)
{
    Vector<UChar> result;
    appendEndMarkup(result, node);
    return String::adopt(result);
}

static void appendMarkup(Vector<UChar>& result, Node* startNode, bool onlyIncludeChildren, Vector<Node*>* nodes, const HashMap<AtomicStringImpl*, AtomicStringImpl*>* namespaces = 0)
{
    HashMap<AtomicStringImpl*, AtomicStringImpl*> namespaceHash;
    if (namespaces)
        namespaceHash = *namespaces;
    
    if (!onlyIncludeChildren) {
        if (nodes)
            nodes->append(startNode);
        
        appendStartMarkup(result,startNode, 0, DoNotAnnotateForInterchange, false, &namespaceHash);
    }
    // print children
    if (!(startNode->document()->isHTMLDocument() && doesHTMLForbidEndTag(startNode)))
        for (Node* current = startNode->firstChild(); current; current = current->nextSibling())
            appendMarkup(result, current, false, nodes, &namespaceHash);
    
    // Print my ending tag
    if (!onlyIncludeChildren)
        appendEndMarkup(result, startNode);
}

static void completeURLs(Node* node, const String& baseURL)
{
    Vector<AttributeChange> changes;

    KURL baseURLAsKURL(baseURL.deprecatedString());

    Node* end = node->traverseNextSibling();
    for (Node* n = node; n != end; n = n->traverseNextNode()) {
        if (n->isElementNode()) {
            Element* e = static_cast<Element*>(n);
            NamedAttrMap* attrs = e->attributes();
            unsigned length = attrs->length();
            for (unsigned i = 0; i < length; i++) {
                Attribute* attr = attrs->attributeItem(i);
                if (e->isURLAttribute(attr)) {
                    String completedURL = KURL(baseURLAsKURL, attr->value().deprecatedString()).url();
                    changes.append(AttributeChange(e, attr->name(), completedURL));
                }
            }
        }
    }

    size_t numChanges = changes.size();
    for (size_t i = 0; i < numChanges; ++i)
        changes[i].apply();
}

static bool needInterchangeNewlineAfter(const VisiblePosition& v)
{
    VisiblePosition next = v.next();
    Node* upstreamNode = next.deepEquivalent().upstream().node();
    Node* downstreamNode = v.deepEquivalent().downstream().node();
    // Add an interchange newline if a paragraph break is selected and a br won't already be added to the markup to represent it.
    return isEndOfParagraph(v) && isStartOfParagraph(next) && !(upstreamNode->hasTagName(brTag) && upstreamNode == downstreamNode);
}

static PassRefPtr<CSSMutableStyleDeclaration> styleFromMatchedRulesAndInlineDecl(Node* node)
{
    if (!node->isHTMLElement())
        return 0;
    
    HTMLElement* element = static_cast<HTMLElement*>(node);
    RefPtr<CSSMutableStyleDeclaration> style = styleFromMatchedRulesForElement(element);
    RefPtr<CSSMutableStyleDeclaration> inlineStyleDecl = element->getInlineStyleDecl();
    style->merge(inlineStyleDecl.get());
    return style.release();
}

static bool propertyMissingOrEqualToNone(CSSMutableStyleDeclaration* style, int propertyID)
{
    if (!style)
        return false;
    RefPtr<CSSValue> value = style->getPropertyCSSValue(propertyID);
    if (!value)
        return true;
    if (!value->isPrimitiveValue())
        return false;
    return static_cast<CSSPrimitiveValue*>(value.get())->getIdent() == CSS_VAL_NONE;
}

static bool elementHasTextDecorationProperty(Node* node)
{
    RefPtr<CSSMutableStyleDeclaration> style = styleFromMatchedRulesAndInlineDecl(node);
    if (!style)
        return false;
    return !propertyMissingOrEqualToNone(style.get(), CSS_PROP_TEXT_DECORATION);
}

String joinMarkups(const Vector<String> preMarkups, const Vector<String>& postMarkups)
{
    size_t length = 0;

    size_t preCount = preMarkups.size();
    for (size_t i = 0; i < preCount; ++i)
        length += preMarkups[i].length();

    size_t postCount = postMarkups.size();
    for (size_t i = 0; i < postCount; ++i)
        length += postMarkups[i].length();

    Vector<UChar> result;
    result.reserveCapacity(length);

    for (size_t i = preCount; i > 0; --i)
        appendString(result, preMarkups[i - 1]);

    for (size_t i = 0; i < postCount; ++i)
        appendString(result, postMarkups[i]);

    return String::adopt(result);
}

// FIXME: Shouldn't we omit style info when annotate == DoNotAnnotateForInterchange? 
// FIXME: At least, annotation and style info should probably not be included in range.markupString()
String createMarkup(const Range* range, Vector<Node*>* nodes, EAnnotateForInterchange annotate, bool convertBlocksToInlines)
{
    static const String interchangeNewlineString = String("<br class=\"") + AppleInterchangeNewline + "\">";

    if (!range || range->isDetached())
        return "";

    Document* document = range->ownerDocument();
    if (!document)
        return "";

    // Disable the delete button so it's elements are not serialized into the markup,
    // but make sure neither endpoint is inside the delete user interface.
    Frame* frame = document->frame();
    DeleteButtonController* deleteButton = frame ? frame->editor()->deleteButtonController() : 0;
    RefPtr<Range> updatedRange = avoidIntersectionWithNode(range, deleteButton ? deleteButton->containerElement() : 0);
    if (deleteButton)
        deleteButton->disable();

    ExceptionCode ec = 0;
    bool collapsed = updatedRange->collapsed(ec);
    ASSERT(ec == 0);
    if (collapsed)
        return "";
    Node* commonAncestor = updatedRange->commonAncestorContainer(ec);
    ASSERT(ec == 0);
    if (!commonAncestor)
        return "";

    document->updateLayoutIgnorePendingStylesheets();

    Vector<String> markups;
    Vector<String> preMarkups;
    Node* pastEnd = updatedRange->pastEndNode();
    Node* lastClosed = 0;
    Vector<Node*> ancestorsToClose;
    
    Node* startNode = updatedRange->startNode();
    VisiblePosition visibleStart(updatedRange->startPosition(), VP_DEFAULT_AFFINITY);
    VisiblePosition visibleEnd(updatedRange->endPosition(), VP_DEFAULT_AFFINITY);
    if (annotate && needInterchangeNewlineAfter(visibleStart)) {
        if (visibleStart == visibleEnd.previous()) {
            if (deleteButton)
                deleteButton->enable();
            return interchangeNewlineString;
        }

        markups.append(interchangeNewlineString);
        startNode = visibleStart.next().deepEquivalent().node();
    }

    Node* next;
    for (Node* n = startNode; n != pastEnd; n = next) {
        next = n->traverseNextNode();
        bool skipDescendants = false;
        bool addMarkupForNode = true;
        
        if (!n->renderer() && !enclosingNodeWithTag(n, selectTag)) {
            skipDescendants = true;
            addMarkupForNode = false;
            next = n->traverseNextSibling();
            // Don't skip over pastEnd.
            if (pastEnd && pastEnd->isDescendantOf(n))
                next = pastEnd;
        }

        if (isBlock(n) && canHaveChildrenForEditing(n) && next == pastEnd)
            // Don't write out empty block containers that aren't fully selected.
            continue;
        
        // Add the node to the markup.
        if (addMarkupForNode) {
            markups.append(getStartMarkup(n, updatedRange.get(), annotate));
            if (nodes)
                nodes->append(n);
        }
        
        if (n->firstChild() == 0 || skipDescendants) {
            // Node has no children, or we are skipping it's descendants, add its close tag now.
            if (addMarkupForNode) {
                markups.append(getEndMarkup(n));
                lastClosed = n;
            }
            
            // Check if the node is the last leaf of a tree.
            if (!n->nextSibling() || next == pastEnd) {
                if (!ancestorsToClose.isEmpty()) {
                    // Close up the ancestors.
                    do {
                        Node *ancestor = ancestorsToClose.last();
                        if (next != pastEnd && next->isDescendantOf(ancestor))
                            break;
                        // Not at the end of the range, close ancestors up to sibling of next node.
                        markups.append(getEndMarkup(ancestor));
                        lastClosed = ancestor;
                        ancestorsToClose.removeLast();
                    } while (!ancestorsToClose.isEmpty());
                }
                
                // Surround the currently accumulated markup with markup for ancestors we never opened as we leave the subtree(s) rooted at those ancestors.
                Node* nextParent = next ? next->parentNode() : 0;
                if (next != pastEnd && n != nextParent) {
                    Node* lastAncestorClosedOrSelf = n->isDescendantOf(lastClosed) ? lastClosed : n;
                    for (Node *parent = lastAncestorClosedOrSelf->parent(); parent != 0 && parent != nextParent; parent = parent->parentNode()) {
                        // All ancestors that aren't in the ancestorsToClose list should either be a) unrendered:
                        if (!parent->renderer())
                            continue;
                        // or b) ancestors that we never encountered during a pre-order traversal starting at startNode:
                        ASSERT(startNode->isDescendantOf(parent));
                        preMarkups.append(getStartMarkup(parent, updatedRange.get(), annotate));
                        markups.append(getEndMarkup(parent));
                        if (nodes)
                            nodes->append(parent);
                        lastClosed = parent;
                    }
                }
            }
        } else if (addMarkupForNode && !skipDescendants)
            // We added markup for this node, and we're descending into it.  Set it to close eventually.
            ancestorsToClose.append(n);
    }
    
    // Include ancestors that aren't completely inside the range but are required to retain 
    // the structure and appearance of the copied markup.
    Node* specialCommonAncestor = 0;
    Node* commonAncestorBlock = commonAncestor ? enclosingBlock(commonAncestor) : 0;
    if (annotate && commonAncestorBlock) {
        if (commonAncestorBlock->hasTagName(tbodyTag) || commonAncestorBlock->hasTagName(trTag)) {
            Node* table = commonAncestorBlock->parentNode();
            while (table && !table->hasTagName(tableTag))
                table = table->parentNode();
            if (table)
                specialCommonAncestor = table;
        } else if (commonAncestorBlock->hasTagName(listingTag)
                    || commonAncestorBlock->hasTagName(olTag)
                    || commonAncestorBlock->hasTagName(preTag)
                    || commonAncestorBlock->hasTagName(tableTag)
                    || commonAncestorBlock->hasTagName(ulTag)
                    || commonAncestorBlock->hasTagName(xmpTag))
            specialCommonAncestor = commonAncestorBlock;
    }
    
    Node* checkAncestor = specialCommonAncestor ? specialCommonAncestor : commonAncestor;
    if (checkAncestor->renderer()) {
        RefPtr<CSSMutableStyleDeclaration> checkAncestorStyle = computedStyle(checkAncestor)->copyInheritableProperties();
        if (!propertyMissingOrEqualToNone(checkAncestorStyle.get(), CSS_PROP__WEBKIT_TEXT_DECORATIONS_IN_EFFECT))
            specialCommonAncestor = elementHasTextDecorationProperty(checkAncestor) ? checkAncestor : enclosingNodeOfType(checkAncestor, &elementHasTextDecorationProperty);
    }
    
    if (Node *enclosingAnchor = enclosingNodeWithTag(specialCommonAncestor ? specialCommonAncestor : commonAncestor, aTag))
        specialCommonAncestor = enclosingAnchor;
    
    Node* body = enclosingNodeWithTag(commonAncestor, bodyTag);
    // FIXME: Only include markup for a fully selected root (and ancestors of lastClosed up to that root) if
    // there are styles/attributes on those nodes that need to be included to preserve the appearance of the copied markup.
    // FIXME: Do this for all fully selected blocks, not just the body.
    Node* fullySelectedRoot = body && *Selection::selectionFromContentsOfNode(body).toRange() == *updatedRange ? body : 0;
    if (annotate && fullySelectedRoot)
        specialCommonAncestor = fullySelectedRoot;
        
    if (specialCommonAncestor) {
        // Also include all of the ancestors of lastClosed up to this special ancestor.
        for (Node* ancestor = lastClosed->parentNode(); ancestor; ancestor = ancestor->parentNode()) {
            if (ancestor == fullySelectedRoot && !convertBlocksToInlines) {
                RefPtr<CSSMutableStyleDeclaration> style = styleFromMatchedRulesAndInlineDecl(fullySelectedRoot);
                
                // Bring the background attribute over, but not as an attribute because a background attribute on a div
                // appears to have no effect.
                if (!style->getPropertyCSSValue(CSS_PROP_BACKGROUND_IMAGE) && static_cast<Element*>(fullySelectedRoot)->hasAttribute(backgroundAttr))
                    style->setProperty(CSS_PROP_BACKGROUND_IMAGE, "url('" + static_cast<Element*>(fullySelectedRoot)->getAttribute(backgroundAttr) + "')");
                
                if (style->length()) {
                    Vector<UChar> openTag;
                    static const String divStyle("<div style=\"");
                    appendString(openTag, divStyle);
                    appendAttributeValue(openTag, style->cssText());
                    openTag.append('\"');
                    openTag.append('>');
                    preMarkups.append(String::adopt(openTag));

                    static const String divCloseTag("</div>");
                    markups.append(divCloseTag);
                }
            } else {
                preMarkups.append(getStartMarkup(ancestor, updatedRange.get(), annotate, convertBlocksToInlines));
                markups.append(getEndMarkup(ancestor));
            }
            if (nodes)
                nodes->append(ancestor);
            
            lastClosed = ancestor;
            
            if (ancestor == specialCommonAncestor)
                break;
        }
    }
    
    // Add a wrapper span with the styles that all of the nodes in the markup inherit.
    Node* parentOfLastClosed = lastClosed ? lastClosed->parentNode() : 0;
    if (parentOfLastClosed && parentOfLastClosed->renderer()) {
        RefPtr<CSSMutableStyleDeclaration> style = computedStyle(parentOfLastClosed)->copyInheritableProperties();

        // Styles that Mail blockquotes contribute should only be placed on the Mail blockquote, to help
        // us differentiate those styles from ones that the user has applied.  This helps us
        // get the color of content pasted into blockquotes right.
        removeEnclosingMailBlockquoteStyle(style.get(), parentOfLastClosed);
        
        // Since we are converting blocks to inlines, remove any inherited block properties that are in the style.
        // This cuts out meaningless properties and prevents properties from magically affecting blocks later
        // if the style is cloned for a new block element during a future editing operation.
        if (convertBlocksToInlines)
            style->removeBlockProperties();

        if (style->length() > 0) {
            Vector<UChar> openTag;
            const String spanClassStyle = String("<span class=\"" AppleStyleSpanClass "\" style=\"");
            appendString(openTag, spanClassStyle);
            appendAttributeValue(openTag, style->cssText());
            openTag.append('\"');
            openTag.append('>');
            preMarkups.append(String::adopt(openTag));
            
            static const String spanCloseTag("</span>");
            markups.append(spanCloseTag);
        }
    }

    // FIXME: The interchange newline should be placed in the block that it's in, not after all of the content, unconditionally.
    if (annotate && needInterchangeNewlineAfter(visibleEnd.previous()))
        markups.append(interchangeNewlineString);

    bool selectedOneOrMoreParagraphs = startOfParagraph(visibleStart) != startOfParagraph(visibleEnd) ||
                                       isStartOfParagraph(visibleStart) && isEndOfParagraph(visibleEnd);
                                      
    // Retain the Mail quote level by including all ancestor mail block quotes.
    if (lastClosed && annotate && selectedOneOrMoreParagraphs) {
        for (Node *ancestor = lastClosed->parentNode(); ancestor; ancestor = ancestor->parentNode()) {
            if (isMailBlockquote(ancestor)) {
                preMarkups.append(getStartMarkup(ancestor, updatedRange.get(), annotate));
                markups.append(getEndMarkup(ancestor));
            }
        }
    }

    if (deleteButton)
        deleteButton->enable();

    return joinMarkups(preMarkups, markups);
}

PassRefPtr<DocumentFragment> createFragmentFromMarkup(Document* document, const String& markup, const String& baseURL)
{
    ASSERT(document->documentElement()->isHTMLElement());
    // FIXME: What if the document element is not an HTML element?
    HTMLElement *element = static_cast<HTMLElement*>(document->documentElement());

    RefPtr<DocumentFragment> fragment = element->createContextualFragment(markup);

    if (fragment && !baseURL.isEmpty() && baseURL != document->baseURL())
        completeURLs(fragment.get(), baseURL);

    return fragment.release();
}

String createMarkup(const Node* node, EChildrenOnly includeChildren, Vector<Node*>* nodes)
{
    Vector<UChar> result;

    if (!node)
        return "";

    Document* document = node->document();
    Frame* frame = document->frame();
    DeleteButtonController* deleteButton = frame ? frame->editor()->deleteButtonController() : 0;

    // disable the delete button so it's elements are not serialized into the markup
    if (deleteButton) {
        if (node->isDescendantOf(deleteButton->containerElement()))
            return "";
        deleteButton->disable();
    }

    appendMarkup(result, const_cast<Node*>(node), includeChildren, nodes);

    if (deleteButton)
        deleteButton->enable();

    return String::adopt(result);
}

static void fillContainerFromString(ContainerNode* paragraph, const DeprecatedString& string)
{
    Document* document = paragraph->document();

    ExceptionCode ec = 0;
    if (string.isEmpty()) {
        paragraph->appendChild(createBlockPlaceholderElement(document), ec);
        ASSERT(ec == 0);
        return;
    }

    ASSERT(string.find('\n') == -1);

    DeprecatedStringList tabList = DeprecatedStringList::split('\t', string, true);
    DeprecatedString tabText = "";
    bool first = true;
    while (!tabList.isEmpty()) {
        DeprecatedString s = tabList.first();
        tabList.pop_front();

        // append the non-tab textual part
        if (!s.isEmpty()) {
            if (!tabText.isEmpty()) {
                paragraph->appendChild(createTabSpanElement(document, tabText), ec);
                ASSERT(ec == 0);
                tabText = "";
            }
            RefPtr<Node> textNode = document->createTextNode(stringWithRebalancedWhitespace(s, first, tabList.isEmpty()));
            paragraph->appendChild(textNode.release(), ec);
            ASSERT(ec == 0);
        }

        // there is a tab after every entry, except the last entry
        // (if the last character is a tab, the list gets an extra empty entry)
        if (!tabList.isEmpty())
            tabText += '\t';
        else if (!tabText.isEmpty()) {
            paragraph->appendChild(createTabSpanElement(document, tabText), ec);
            ASSERT(ec == 0);
        }
        
        first = false;
    }
}

PassRefPtr<DocumentFragment> createFragmentFromText(Range* context, const String& text)
{
    if (!context)
        return 0;

    Node* styleNode = context->startNode();
    if (!styleNode) {
        styleNode = context->startPosition().node();
        if (!styleNode)
            return 0;
    }

    Document* document = styleNode->document();
    RefPtr<DocumentFragment> fragment = document->createDocumentFragment();
    
    if (text.isEmpty())
        return fragment.release();

    DeprecatedString string = text.deprecatedString();
    string.replace("\r\n", "\n");
    string.replace('\r', '\n');

    ExceptionCode ec = 0;
    RenderObject* renderer = styleNode->renderer();
    if (renderer && renderer->style()->preserveNewline()) {
        fragment->appendChild(document->createTextNode(string), ec);
        ASSERT(ec == 0);
        if (string.endsWith("\n")) {
            RefPtr<Element> element;
            element = document->createElementNS(xhtmlNamespaceURI, "br", ec);
            ASSERT(ec == 0);
            element->setAttribute(classAttr, AppleInterchangeNewline);            
            fragment->appendChild(element.release(), ec);
            ASSERT(ec == 0);
        }
        return fragment.release();
    }

    // A string with no newlines gets added inline, rather than being put into a paragraph.
    if (string.find('\n') == -1) {
        fillContainerFromString(fragment.get(), string);
        return fragment.release();
    }

    // Break string into paragraphs. Extra line breaks turn into empty paragraphs.
    DeprecatedStringList list = DeprecatedStringList::split('\n', string, true); // true gets us empty strings in the list
    while (!list.isEmpty()) {
        DeprecatedString s = list.first();
        list.pop_front();

        RefPtr<Element> element;
        if (s.isEmpty() && list.isEmpty()) {
            // For last line, use the "magic BR" rather than a P.
            element = document->createElementNS(xhtmlNamespaceURI, "br", ec);
            ASSERT(ec == 0);
            element->setAttribute(classAttr, AppleInterchangeNewline);            
        } else {
            element = createDefaultParagraphElement(document);
            fillContainerFromString(element.get(), s);
        }
        fragment->appendChild(element.release(), ec);
        ASSERT(ec == 0);
    }
    return fragment.release();
}

PassRefPtr<DocumentFragment> createFragmentFromNodes(Document *document, const Vector<Node*>& nodes)
{
    if (!document)
        return 0;

    // disable the delete button so it's elements are not serialized into the markup
    if (document->frame())
        document->frame()->editor()->deleteButtonController()->disable();

    RefPtr<DocumentFragment> fragment = document->createDocumentFragment();

    ExceptionCode ec = 0;
    size_t size = nodes.size();
    for (size_t i = 0; i < size; ++i) {
        RefPtr<Element> element = createDefaultParagraphElement(document);
        element->appendChild(nodes[i], ec);
        ASSERT(ec == 0);
        fragment->appendChild(element.release(), ec);
        ASSERT(ec == 0);
    }

    if (document->frame())
        document->frame()->editor()->deleteButtonController()->enable();

    return fragment.release();
}

}
