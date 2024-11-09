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

#include "QJSDOMWindow.h"

#include "BarInfo.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSRuleList.h"
#include "CSSStyleDeclaration.h"
#include "DOMSelection.h"
#include "DOMWindow.h"
#include "Document.h"
#include "Element.h"
#include "History.h"
#include "QJSAttr.h"
#include "QJSBarInfo.h"
#include "QJSCDATASection.h"
#include "QJSCSSCharsetRule.h"
#include "QJSCSSFontFaceRule.h"
#include "QJSCSSImportRule.h"
#include "QJSCSSMediaRule.h"
#include "QJSCSSPageRule.h"
#include "QJSCSSPrimitiveValue.h"
#include "QJSCSSRule.h"
#include "QJSCSSRuleList.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSStyleRule.h"
#include "QJSCSSStyleSheet.h"
#include "QJSCSSValue.h"
#include "QJSCSSValueList.h"
#include "QJSCharacterData.h"
#include "QJSComment.h"
#include "QJSCounter.h"
#include "QJSDOMImplementation.h"
#include "QJSDOMParser.h"
#include "QJSDOMSelection.h"
#include "QJSDocument.h"
#include "QJSDocumentFragment.h"
#include "QJSDocumentType.h"
#include "QJSElement.h"
#include "QJSEntity.h"
#include "QJSEntityReference.h"
#include "QJSEvent.h"
#include "QJSHTMLAnchorElement.h"
#include "QJSHTMLAppletElement.h"
#include "QJSHTMLAreaElement.h"
#include "QJSHTMLBRElement.h"
#include "QJSHTMLBaseElement.h"
#include "QJSHTMLBaseFontElement.h"
#include "QJSHTMLBlockquoteElement.h"
#include "QJSHTMLBodyElement.h"
#include "QJSHTMLButtonElement.h"
#include "QJSHTMLCanvasElement.h"
#include "QJSHTMLDListElement.h"
#include "QJSHTMLDirectoryElement.h"
#include "QJSHTMLDivElement.h"
#include "QJSHTMLDocument.h"
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
#include "QJSHTMLIsIndexElement.h"
#include "QJSHTMLLIElement.h"
#include "QJSHTMLLabelElement.h"
#include "QJSHTMLLegendElement.h"
#include "QJSHTMLLinkElement.h"
#include "QJSHTMLMapElement.h"
#include "QJSHTMLMarqueeElement.h"
#include "QJSHTMLMenuElement.h"
#include "QJSHTMLMetaElement.h"
#include "QJSHTMLModElement.h"
#include "QJSHTMLOListElement.h"
#include "QJSHTMLObjectElement.h"
#include "QJSHTMLOptGroupElement.h"
#include "QJSHTMLOptionElement.h"
#include "QJSHTMLParagraphElement.h"
#include "QJSHTMLParamElement.h"
#include "QJSHTMLPreElement.h"
#include "QJSHTMLQuoteElement.h"
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
#include "QJSHistory.h"
#include "QJSMediaList.h"
#include "QJSMutationEvent.h"
#include "QJSNamedNodeMap.h"
#include "QJSNode.h"
#include "QJSNodeFilter.h"
#include "QJSNodeList.h"
#include "QJSNotation.h"
#include "QJSProcessingInstruction.h"
#include "QJSRange.h"
#include "QJSRangeException.h"
#include "QJSRect.h"
#include "QJSScreen.h"
#include "QJSStyleSheet.h"
#include "QJSStyleSheetList.h"
#include "QJSText.h"
#include "QJSXMLSerializer.h"
#include "Screen.h"
#include "qjs_dom.h"
#include "qjs_window.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSDOMWindowAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("HTMLDirectoryElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLDirectoryElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("screenY", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ScreenYAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSStyleDeclaration", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSStyleDeclarationConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLTableCellElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLTableCellElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("frameElement", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::FrameElementAttrNum),
    JS_CGETSET_MAGIC_DEF("outerHeight", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::OuterHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("XMLDocument", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::XMLDocumentConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSStyleSheet", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSStyleSheetConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("offscreenBuffering", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::OffscreenBufferingAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLDivElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLDivElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSValueList", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSValueListConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSRule", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSRuleConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSMediaRule", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSMediaRuleConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("NodeFilter", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::NodeFilterConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLEmbedElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLEmbedElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSValue", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSValueConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLHRElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLHRElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSRuleList", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSRuleListConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLIsIndexElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLIsIndexElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLAnchorElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLAnchorElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollbars", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ScrollbarsAttrNum),
    JS_CGETSET_MAGIC_DEF("innerWidth", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::InnerWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLObjectElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLObjectElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("DOMImplementation", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::DOMImplementationConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("document", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::DocumentAttrNum),
    JS_CGETSET_MAGIC_DEF("Rect", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::RectConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("defaultStatus", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::DefaultStatusAttrNum),
    JS_CGETSET_MAGIC_DEF("Text", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::TextConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("defaultstatus", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::DefaultstatusAttrNum),
    JS_CGETSET_MAGIC_DEF("status", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::StatusAttrNum),
    JS_CGETSET_MAGIC_DEF("screenLeft", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ScreenLeftAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLUListElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLUListElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("StyleSheet", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::StyleSheetConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("EntityReference", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::EntityReferenceConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Node", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::NodeConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("history", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::HistoryAttrNum),
    JS_CGETSET_MAGIC_DEF("personalbar", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::PersonalbarAttrNum),
    JS_CGETSET_MAGIC_DEF("devicePixelRatio", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::DevicePixelRatioAttrNum),
    JS_CGETSET_MAGIC_DEF("pageYOffset", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::PageYOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("menubar", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::MenubarAttrNum),
    JS_CGETSET_MAGIC_DEF("closed", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ClosedAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSPrimitiveValue", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSPrimitiveValueConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("innerHeight", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::InnerHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("parent", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ParentAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSPageRule", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSPageRuleConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLScriptElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLScriptElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("DocumentType", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::DocumentTypeConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("NodeList", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::NodeListConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("locationbar", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::LocationbarAttrNum),
    JS_CGETSET_MAGIC_DEF("screen", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ScreenAttrNum),
    JS_CGETSET_MAGIC_DEF("self", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::SelfAttrNum),
    JS_CGETSET_MAGIC_DEF("frames", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::FramesAttrNum),
    JS_CGETSET_MAGIC_DEF("Counter", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CounterConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLQuoteElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLQuoteElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("ProcessingInstruction", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::ProcessingInstructionConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("toolbar", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ToolbarAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLMarqueeElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLMarqueeElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLMenuElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLMenuElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLHeadElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLHeadElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLMapElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLMapElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("statusbar", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::StatusbarAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLDListElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLDListElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLImageElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLImageElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("top", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::TopAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLBaseElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLBaseElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLBlockquoteElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLBlockquoteElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("outerWidth", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::OuterWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("screenX", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ScreenXAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLTitleElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLTitleElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("DocumentFragment", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::DocumentFragmentConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Notation", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::NotationConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollX", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ScrollXAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLOListElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLOListElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Document", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::DocumentConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSCharsetRule", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSCharsetRuleConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLFormElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLFormElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSFontFaceRule", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSFontFaceRuleConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLHtmlElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLHtmlElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("pageXOffset", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::PageXOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLIFrameElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLIFrameElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollY", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ScrollYAttrNum),
    JS_CGETSET_MAGIC_DEF("CharacterData", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CharacterDataConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLDocument", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLDocumentConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLLegendElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLLegendElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("screenTop", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::ScreenTopAttrNum),
    JS_CGETSET_MAGIC_DEF("length", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("window", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::WindowAttrNum),
    JS_CGETSET_MAGIC_DEF("opener", JSDOMWindow::getValueProperty, NULL, JSDOMWindow::OpenerAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSImportRule", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSImportRuleConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CSSStyleRule", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CSSStyleRuleConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("MediaList", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::MediaListConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("StyleSheetList", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::StyleSheetListConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("NamedNodeMap", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::NamedNodeMapConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Attr", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::AttrConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Element", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::ElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Comment", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CommentConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("CDATASection", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::CDATASectionConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Entity", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::EntityConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLAppletElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLAppletElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLAreaElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLAreaElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLBRElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLBRElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLBaseFontElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLBaseFontElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLBodyElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLBodyElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLButtonElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLButtonElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLCanvasElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLCanvasElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLFieldSetElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLFieldSetElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLFontElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLFontElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLFrameElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLFrameElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLFrameSetElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLFrameSetElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLHeadingElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLHeadingElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLInputElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLInputElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLLIElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLLIElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLLabelElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLLabelElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLLinkElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLLinkElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLMetaElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLMetaElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLModElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLModElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLOptGroupElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLOptGroupElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLOptionElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLOptionElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLParagraphElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLParagraphElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLParamElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLParamElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLPreElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLPreElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLSelectElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLSelectElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLStyleElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLStyleElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLTableCaptionElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLTableCaptionElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLTableColElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLTableColElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLTableElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLTableElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLTableRowElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLTableRowElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLTableSectionElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLTableSectionElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("HTMLTextAreaElement", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::HTMLTextAreaElementConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Event", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::EventConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("MutationEvent", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::MutationEventConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("Range", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::RangeConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("RangeException", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::RangeExceptionConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("DOMParser", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::DOMParserConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("XMLSerializer", JSDOMWindow::getValueProperty, JSDOMWindow::putValueProperty, JSDOMWindow::XMLSerializerConstructorAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSDOMWindowPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("alert", 1, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::AlertFuncNum),
    JS_CFUNC_MAGIC_DEF("blur", 0, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::BlurFuncNum),
    JS_CFUNC_MAGIC_DEF("getSelection", 0, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::GetSelectionFuncNum),
    JS_CFUNC_MAGIC_DEF("find", 7, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::FindFuncNum),
    JS_CFUNC_MAGIC_DEF("close", 0, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::CloseFuncNum),
    JS_CFUNC_MAGIC_DEF("getComputedStyle", 2, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::GetComputedStyleFuncNum),
    JS_CFUNC_MAGIC_DEF("focus", 0, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::FocusFuncNum),
    JS_CFUNC_MAGIC_DEF("prompt", 2, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::PromptFuncNum),
    JS_CFUNC_MAGIC_DEF("print", 0, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::PrintFuncNum),
    JS_CFUNC_MAGIC_DEF("stop", 0, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::StopFuncNum),
    JS_CFUNC_MAGIC_DEF("confirm", 1, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::ConfirmFuncNum),
    JS_CFUNC_MAGIC_DEF("getMatchedCSSRules", 3, JSDOMWindowPrototypeFunction::callAsFunction, JSDOMWindow::GetMatchedCSSRulesFuncNum)
};

JSValue JSDOMWindowPrototype::self(JSContext * ctx)
{
    JSValue obj = JS_NewObjectProto(ctx, WindowPrototype::self(ctx));
    JSDOMWindowPrototype::initPrototype(ctx, obj);
    return obj;
}

void JSDOMWindowPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMWindowAttributesFunctions, countof(JSDOMWindowAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMWindowPrototypeFunctions, countof(JSDOMWindowPrototypeFunctions));
}

static JSClassDef JSDOMWindowClassDefine = 
{
    "DOMWindow",
    .finalizer = JSDOMWindow::finalizer,
    .gc_mark = JSDOMWindow::mark,
};

JSClassID JSDOMWindow::js_class_id = 0;

void JSDOMWindow::init(JSContext* ctx)
{
    if (JSDOMWindow::js_class_id == 0) {
        JS_NewClassID(&JSDOMWindow::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDOMWindow::js_class_id, &JSDOMWindowClassDefine);
    }
}

JSValue JSDOMWindow::create(JSContext* ctx, JSValue obj, DOMWindow* impl)
{
    JSDOMWindow::init(ctx);
    JS_SetPrototype(ctx, obj, JSDOMWindowPrototype::self(ctx));
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDOMWindow::finalizer(JSRuntime* rt, JSValue val)
{
    DOMWindow* impl = (DOMWindow*)JS_GetOpaque(val, JSDOMWindow::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSDOMWindow::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    Window::mark(rt, val, mark_func);
}

JSValue JSDOMWindow::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ScreenAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->screen()));
        }
        case HistoryAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->history()));
        }
        case LocationbarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->locationbar()));
        }
        case MenubarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->menubar()));
        }
        case PersonalbarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->personalbar()));
        }
        case ScrollbarsAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->scrollbars()));
        }
        case StatusbarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->statusbar()));
        }
        case ToolbarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->toolbar()));
        }
        case FrameElementAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return checkNodeSecurity(ctx, imp->frameElement()) ? toJS(ctx, QJS::getPtr(imp->frameElement())) : JS_UNDEFINED;
        }
        case OffscreenBufferingAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBool(ctx, imp->offscreenBuffering() ? 1 : 0);
        }
        case OuterHeightAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->outerHeight());
        }
        case OuterWidthAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->outerWidth());
        }
        case InnerHeightAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->innerHeight());
        }
        case InnerWidthAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->innerWidth());
        }
        case ScreenXAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenX());
        }
        case ScreenYAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenY());
        }
        case ScreenLeftAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenLeft());
        }
        case ScreenTopAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenTop());
        }
        case ScrollXAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollX());
        }
        case ScrollYAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollY());
        }
        case PageXOffsetAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->pageXOffset());
        }
        case PageYOffsetAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->pageYOffset());
        }
        case ClosedAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBool(ctx, imp->closed() ? 1 : 0);
        }
        case LengthAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case NameAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case StatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->status()).utf8().data());
        }
        case DefaultStatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->defaultStatus()).utf8().data());
        }
        case DefaultstatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->defaultstatus()).utf8().data());
        }
        case SelfAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->self()));
        }
        case WindowAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->window()));
        }
        case FramesAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->frames()));
        }
        case OpenerAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->opener()));
        }
        case ParentAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parent()));
        }
        case TopAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->top()));
        }
        case DocumentAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->document()));
        }
        case DevicePixelRatioAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            return JS_NewBigUint64(ctx, imp->devicePixelRatio());
        }
        case StyleSheetConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSStyleSheet::getConstructor(ctx);
        }
        case CSSStyleSheetConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSStyleSheet::getConstructor(ctx);
        }
        case CSSValueConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSValue::getConstructor(ctx);
        }
        case CSSPrimitiveValueConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSPrimitiveValue::getConstructor(ctx);
        }
        case CSSValueListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSValueList::getConstructor(ctx);
        }
        case CSSRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSRule::getConstructor(ctx);
        }
        case CSSCharsetRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSCharsetRule::getConstructor(ctx);
        }
        case CSSFontFaceRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSFontFaceRule::getConstructor(ctx);
        }
        case CSSImportRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSImportRule::getConstructor(ctx);
        }
        case CSSMediaRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSMediaRule::getConstructor(ctx);
        }
        case CSSPageRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSPageRule::getConstructor(ctx);
        }
        case CSSStyleRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSStyleRule::getConstructor(ctx);
        }
        case CSSStyleDeclarationConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSStyleDeclaration::getConstructor(ctx);
        }
        case MediaListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSMediaList::getConstructor(ctx);
        }
        case CounterConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCounter::getConstructor(ctx);
        }
        case CSSRuleListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCSSRuleList::getConstructor(ctx);
        }
        case RectConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSRect::getConstructor(ctx);
        }
        case StyleSheetListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSStyleSheetList::getConstructor(ctx);
        }
        case DOMImplementationConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSDOMImplementation::getConstructor(ctx);
        }
        case DocumentFragmentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSDocumentFragment::getConstructor(ctx);
        }
        case DocumentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSDocument::getConstructor(ctx);
        }
        case NodeConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSNode::getConstructor(ctx);
        }
        case NodeListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSNodeList::getConstructor(ctx);
        }
        case NamedNodeMapConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSNamedNodeMap::getConstructor(ctx);
        }
        case CharacterDataConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCharacterData::getConstructor(ctx);
        }
        case AttrConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSAttr::getConstructor(ctx);
        }
        case ElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSElement::getConstructor(ctx);
        }
        case TextConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSText::getConstructor(ctx);
        }
        case CommentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSComment::getConstructor(ctx);
        }
        case CDATASectionConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSCDATASection::getConstructor(ctx);
        }
        case DocumentTypeConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSDocumentType::getConstructor(ctx);
        }
        case NotationConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSNotation::getConstructor(ctx);
        }
        case EntityConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSEntity::getConstructor(ctx);
        }
        case EntityReferenceConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSEntityReference::getConstructor(ctx);
        }
        case ProcessingInstructionConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSProcessingInstruction::getConstructor(ctx);
        }
        case HTMLDocumentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLDocument::getConstructor(ctx);
        }
        case HTMLElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLElement::getConstructor(ctx);
        }
        case HTMLAnchorElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLAnchorElement::getConstructor(ctx);
        }
        case HTMLAppletElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLAppletElement::getConstructor(ctx);
        }
        case HTMLAreaElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLAreaElement::getConstructor(ctx);
        }
        case HTMLBRElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLBRElement::getConstructor(ctx);
        }
        case HTMLBaseElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLBaseElement::getConstructor(ctx);
        }
        case HTMLBaseFontElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLBaseFontElement::getConstructor(ctx);
        }
        case HTMLBlockquoteElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLBlockquoteElement::getConstructor(ctx);
        }
        case HTMLBodyElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLBodyElement::getConstructor(ctx);
        }
        case HTMLButtonElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLButtonElement::getConstructor(ctx);
        }
        case HTMLCanvasElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLCanvasElement::getConstructor(ctx);
        }
        case HTMLDListElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLDListElement::getConstructor(ctx);
        }
        case HTMLDirectoryElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLDirectoryElement::getConstructor(ctx);
        }
        case HTMLDivElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLDivElement::getConstructor(ctx);
        }
        case HTMLEmbedElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLEmbedElement::getConstructor(ctx);
        }
        case HTMLFieldSetElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLFieldSetElement::getConstructor(ctx);
        }
        case HTMLFontElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLFontElement::getConstructor(ctx);
        }
        case HTMLFormElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLFormElement::getConstructor(ctx);
        }
        case HTMLFrameElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLFrameElement::getConstructor(ctx);
        }
        case HTMLFrameSetElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLFrameSetElement::getConstructor(ctx);
        }
        case HTMLHRElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLHRElement::getConstructor(ctx);
        }
        case HTMLHeadElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLHeadElement::getConstructor(ctx);
        }
        case HTMLHeadingElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLHeadingElement::getConstructor(ctx);
        }
        case HTMLHtmlElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLHtmlElement::getConstructor(ctx);
        }
        case HTMLIFrameElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLIFrameElement::getConstructor(ctx);
        }
        case HTMLImageElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLImageElement::getConstructor(ctx);
        }
        case HTMLInputElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLInputElement::getConstructor(ctx);
        }
        case HTMLIsIndexElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLIsIndexElement::getConstructor(ctx);
        }
        case HTMLLIElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLLIElement::getConstructor(ctx);
        }
        case HTMLLabelElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLLabelElement::getConstructor(ctx);
        }
        case HTMLLegendElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLLegendElement::getConstructor(ctx);
        }
        case HTMLLinkElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLLinkElement::getConstructor(ctx);
        }
        case HTMLMapElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLMapElement::getConstructor(ctx);
        }
        case HTMLMarqueeElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLMarqueeElement::getConstructor(ctx);
        }
        case HTMLMenuElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLMenuElement::getConstructor(ctx);
        }
        case HTMLMetaElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLMetaElement::getConstructor(ctx);
        }
        case HTMLModElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLModElement::getConstructor(ctx);
        }
        case HTMLOListElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLOListElement::getConstructor(ctx);
        }
        case HTMLObjectElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLObjectElement::getConstructor(ctx);
        }
        case HTMLOptGroupElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLOptGroupElement::getConstructor(ctx);
        }
        case HTMLOptionElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLOptionElement::getConstructor(ctx);
        }
        case HTMLParagraphElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLParagraphElement::getConstructor(ctx);
        }
        case HTMLParamElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLParamElement::getConstructor(ctx);
        }
        case HTMLPreElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLPreElement::getConstructor(ctx);
        }
        case HTMLQuoteElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLQuoteElement::getConstructor(ctx);
        }
        case HTMLScriptElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLScriptElement::getConstructor(ctx);
        }
        case HTMLSelectElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLSelectElement::getConstructor(ctx);
        }
        case HTMLStyleElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLStyleElement::getConstructor(ctx);
        }
        case HTMLTableCaptionElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLTableCaptionElement::getConstructor(ctx);
        }
        case HTMLTableCellElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLTableCellElement::getConstructor(ctx);
        }
        case HTMLTableColElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLTableColElement::getConstructor(ctx);
        }
        case HTMLTableElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLTableElement::getConstructor(ctx);
        }
        case HTMLTableRowElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLTableRowElement::getConstructor(ctx);
        }
        case HTMLTableSectionElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLTableSectionElement::getConstructor(ctx);
        }
        case HTMLTextAreaElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLTextAreaElement::getConstructor(ctx);
        }
        case HTMLTitleElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLTitleElement::getConstructor(ctx);
        }
        case HTMLUListElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSHTMLUListElement::getConstructor(ctx);
        }
        case EventConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSEvent::getConstructor(ctx);
        }
        case MutationEventConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSMutationEvent::getConstructor(ctx);
        }
        case NodeFilterConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSNodeFilter::getConstructor(ctx);
        }
        case RangeConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSRange::getConstructor(ctx);
        }
        case RangeExceptionConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSRangeException::getConstructor(ctx);
        }
        case XMLDocumentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSDocument::getConstructor(ctx);
        }
        case DOMParserConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSDOMParser::getConstructor(ctx);
        }
        case XMLSerializerConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSXMLSerializer::getConstructor(ctx);
        }
    }
    return JS_NULL;
}

JSValue JSDOMWindow::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case NameAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            imp->setName(valueToString(ctx, value));
            break;
        }
        case StatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            imp->setStatus(valueToString(ctx, value));
            break;
        }
        case DefaultStatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            imp->setDefaultStatus(valueToString(ctx, value));
            break;
        }
        case DefaultstatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
            imp->setDefaultstatus(valueToString(ctx, value));
            break;
        }
        case StyleSheetConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "StyleSheet", value);
            break;
        }
        case CSSStyleSheetConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSStyleSheet", value);
            break;
        }
        case CSSValueConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSValue", value);
            break;
        }
        case CSSPrimitiveValueConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSPrimitiveValue", value);
            break;
        }
        case CSSValueListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSValueList", value);
            break;
        }
        case CSSRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSRule", value);
            break;
        }
        case CSSCharsetRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSCharsetRule", value);
            break;
        }
        case CSSFontFaceRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSFontFaceRule", value);
            break;
        }
        case CSSImportRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSImportRule", value);
            break;
        }
        case CSSMediaRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSMediaRule", value);
            break;
        }
        case CSSPageRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSPageRule", value);
            break;
        }
        case CSSStyleRuleConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSStyleRule", value);
            break;
        }
        case CSSStyleDeclarationConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSStyleDeclaration", value);
            break;
        }
        case MediaListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "MediaList", value);
            break;
        }
        case CounterConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Counter", value);
            break;
        }
        case CSSRuleListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CSSRuleList", value);
            break;
        }
        case RectConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Rect", value);
            break;
        }
        case StyleSheetListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "StyleSheetList", value);
            break;
        }
        case DOMImplementationConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "DOMImplementation", value);
            break;
        }
        case DocumentFragmentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "DocumentFragment", value);
            break;
        }
        case DocumentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Document", value);
            break;
        }
        case NodeConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Node", value);
            break;
        }
        case NodeListConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "NodeList", value);
            break;
        }
        case NamedNodeMapConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "NamedNodeMap", value);
            break;
        }
        case CharacterDataConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CharacterData", value);
            break;
        }
        case AttrConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Attr", value);
            break;
        }
        case ElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Element", value);
            break;
        }
        case TextConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Text", value);
            break;
        }
        case CommentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Comment", value);
            break;
        }
        case CDATASectionConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "CDATASection", value);
            break;
        }
        case DocumentTypeConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "DocumentType", value);
            break;
        }
        case NotationConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Notation", value);
            break;
        }
        case EntityConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Entity", value);
            break;
        }
        case EntityReferenceConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "EntityReference", value);
            break;
        }
        case ProcessingInstructionConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "ProcessingInstruction", value);
            break;
        }
        case HTMLDocumentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLDocument", value);
            break;
        }
        case HTMLElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLElement", value);
            break;
        }
        case HTMLAnchorElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLAnchorElement", value);
            break;
        }
        case HTMLAppletElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLAppletElement", value);
            break;
        }
        case HTMLAreaElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLAreaElement", value);
            break;
        }
        case HTMLBRElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLBRElement", value);
            break;
        }
        case HTMLBaseElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLBaseElement", value);
            break;
        }
        case HTMLBaseFontElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLBaseFontElement", value);
            break;
        }
        case HTMLBlockquoteElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLBlockquoteElement", value);
            break;
        }
        case HTMLBodyElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLBodyElement", value);
            break;
        }
        case HTMLButtonElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLButtonElement", value);
            break;
        }
        case HTMLCanvasElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLCanvasElement", value);
            break;
        }
        case HTMLDListElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLDListElement", value);
            break;
        }
        case HTMLDirectoryElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLDirectoryElement", value);
            break;
        }
        case HTMLDivElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLDivElement", value);
            break;
        }
        case HTMLEmbedElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLEmbedElement", value);
            break;
        }
        case HTMLFieldSetElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLFieldSetElement", value);
            break;
        }
        case HTMLFontElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLFontElement", value);
            break;
        }
        case HTMLFormElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLFormElement", value);
            break;
        }
        case HTMLFrameElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLFrameElement", value);
            break;
        }
        case HTMLFrameSetElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLFrameSetElement", value);
            break;
        }
        case HTMLHRElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLHRElement", value);
            break;
        }
        case HTMLHeadElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLHeadElement", value);
            break;
        }
        case HTMLHeadingElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLHeadingElement", value);
            break;
        }
        case HTMLHtmlElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLHtmlElement", value);
            break;
        }
        case HTMLIFrameElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLIFrameElement", value);
            break;
        }
        case HTMLImageElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLImageElement", value);
            break;
        }
        case HTMLInputElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLInputElement", value);
            break;
        }
        case HTMLIsIndexElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLIsIndexElement", value);
            break;
        }
        case HTMLLIElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLLIElement", value);
            break;
        }
        case HTMLLabelElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLLabelElement", value);
            break;
        }
        case HTMLLegendElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLLegendElement", value);
            break;
        }
        case HTMLLinkElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLLinkElement", value);
            break;
        }
        case HTMLMapElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLMapElement", value);
            break;
        }
        case HTMLMarqueeElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLMarqueeElement", value);
            break;
        }
        case HTMLMenuElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLMenuElement", value);
            break;
        }
        case HTMLMetaElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLMetaElement", value);
            break;
        }
        case HTMLModElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLModElement", value);
            break;
        }
        case HTMLOListElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLOListElement", value);
            break;
        }
        case HTMLObjectElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLObjectElement", value);
            break;
        }
        case HTMLOptGroupElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLOptGroupElement", value);
            break;
        }
        case HTMLOptionElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLOptionElement", value);
            break;
        }
        case HTMLParagraphElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLParagraphElement", value);
            break;
        }
        case HTMLParamElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLParamElement", value);
            break;
        }
        case HTMLPreElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLPreElement", value);
            break;
        }
        case HTMLQuoteElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLQuoteElement", value);
            break;
        }
        case HTMLScriptElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLScriptElement", value);
            break;
        }
        case HTMLSelectElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLSelectElement", value);
            break;
        }
        case HTMLStyleElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLStyleElement", value);
            break;
        }
        case HTMLTableCaptionElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLTableCaptionElement", value);
            break;
        }
        case HTMLTableCellElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLTableCellElement", value);
            break;
        }
        case HTMLTableColElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLTableColElement", value);
            break;
        }
        case HTMLTableElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLTableElement", value);
            break;
        }
        case HTMLTableRowElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLTableRowElement", value);
            break;
        }
        case HTMLTableSectionElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLTableSectionElement", value);
            break;
        }
        case HTMLTextAreaElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLTextAreaElement", value);
            break;
        }
        case HTMLTitleElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLTitleElement", value);
            break;
        }
        case HTMLUListElementConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "HTMLUListElement", value);
            break;
        }
        case EventConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Event", value);
            break;
        }
        case MutationEventConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "MutationEvent", value);
            break;
        }
        case NodeFilterConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "NodeFilter", value);
            break;
        }
        case RangeConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "Range", value);
            break;
        }
        case RangeExceptionConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "RangeException", value);
            break;
        }
        case XMLDocumentConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "XMLDocument", value);
            break;
        }
        case DOMParserConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "DOMParser", value);
            break;
        }
        case XMLSerializerConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "XMLSerializer", value);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSDOMWindowPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    DOMWindow* imp = (DOMWindow*)JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSDOMWindow::GetSelectionFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->getSelection()));
            return result;
        }
        case JSDOMWindow::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
        case JSDOMWindow::BlurFuncNum: {
            imp->blur();
            return JS_UNDEFINED;
        }
        case JSDOMWindow::CloseFuncNum: {
            imp->close();
            return JS_UNDEFINED;
        }
        case JSDOMWindow::PrintFuncNum: {
            imp->print();
            return JS_UNDEFINED;
        }
        case JSDOMWindow::StopFuncNum: {
            imp->stop();
            return JS_UNDEFINED;
        }
        case JSDOMWindow::AlertFuncNum: {
            String message = valueToString(ctx, argv[0]);
            imp->alert(message);
            return JS_UNDEFINED;
        }
        case JSDOMWindow::ConfirmFuncNum: {
            String message = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->confirm(message) ? 1 : 0);
            return result;
        }
        case JSDOMWindow::PromptFuncNum: {
            String message = valueToString(ctx, argv[0]);
            String defaultValue = valueToString(ctx, argv[1]);

            JSValue result = jsStringOrNull(ctx, imp->prompt(message, defaultValue));
            return result;
        }
        case JSDOMWindow::FindFuncNum: {
            String string = valueToString(ctx, argv[0]);
            bool caseSensitive = valueToBoolean(ctx, argv[1]);
            bool backwards = valueToBoolean(ctx, argv[2]);
            bool wrap = valueToBoolean(ctx, argv[3]);
            bool wholeWord = valueToBoolean(ctx, argv[4]);
            bool searchInFrames = valueToBoolean(ctx, argv[5]);
            bool showDialog = valueToBoolean(ctx, argv[6]);

            JSValue result = JS_NewBool(ctx, imp->find(string, caseSensitive, backwards, wrap, wholeWord, searchInFrames, showDialog) ? 1 : 0);
            return result;
        }
        case JSDOMWindow::GetComputedStyleFuncNum: {
            Element* element = toElement(argv[0]);
            String pseudoElement = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getComputedStyle(element, pseudoElement)));
            return result;
        }
        case JSDOMWindow::GetMatchedCSSRulesFuncNum: {
            Element* element = toElement(argv[0]);
            String pseudoElement = valueToString(ctx, argv[1]);
            if (argc < 3) {

                  JSValue result = toJS(ctx, QJS::getPtr(imp->getMatchedCSSRules(element, pseudoElement)));
                  return result;
            }

            bool authorOnly = valueToBoolean(ctx, argv[2]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getMatchedCSSRules(element, pseudoElement, authorOnly)));
            return result;
        }
    }
    return JS_NULL;
}


DOMWindow* toDOMWindow(JSValue val)
{
    if (JS_IsObject(val)) {
        DOMWindow* impl = (DOMWindow*)JS_GetOpaque(val, JSDOMWindow::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
