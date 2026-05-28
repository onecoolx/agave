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
#include "QJSXPathEvaluator.h"
#include "QJSXPathResult.h"
#include "Screen.h"
#include "qjs_dom.h"
#include "qjs_window.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSDOMWindowAttributesFunctions[139];
static bool JSDOMWindowAttributesFunctions_initialized = false;

static void init_JSDOMWindowAttributesFunctions()
{
    if (JSDOMWindowAttributesFunctions_initialized) return;
    JSDOMWindowAttributesFunctions_initialized = true;
    memset(JSDOMWindowAttributesFunctions, 0, sizeof(JSDOMWindowAttributesFunctions));
    JSDOMWindowAttributesFunctions[0].name = "screenX";
    JSDOMWindowAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[0].magic = JSDOMWindow::ScreenXAttrNum;
    JSDOMWindowAttributesFunctions[0].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[1].name = "HTMLBaseFontElement";
    JSDOMWindowAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[1].magic = JSDOMWindow::HTMLBaseFontElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[1].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[1].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[2].name = "HTMLParamElement";
    JSDOMWindowAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[2].magic = JSDOMWindow::HTMLParamElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[2].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[2].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[3].name = "history";
    JSDOMWindowAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[3].magic = JSDOMWindow::HistoryAttrNum;
    JSDOMWindowAttributesFunctions[3].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[4].name = "personalbar";
    JSDOMWindowAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[4].magic = JSDOMWindow::PersonalbarAttrNum;
    JSDOMWindowAttributesFunctions[4].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[5].name = "opener";
    JSDOMWindowAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[5].magic = JSDOMWindow::OpenerAttrNum;
    JSDOMWindowAttributesFunctions[5].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[6].name = "menubar";
    JSDOMWindowAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[6].magic = JSDOMWindow::MenubarAttrNum;
    JSDOMWindowAttributesFunctions[6].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[7].name = "frames";
    JSDOMWindowAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[7].magic = JSDOMWindow::FramesAttrNum;
    JSDOMWindowAttributesFunctions[7].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[8].name = "CSSRule";
    JSDOMWindowAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[8].magic = JSDOMWindow::CSSRuleConstructorAttrNum;
    JSDOMWindowAttributesFunctions[8].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[8].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[9].name = "locationbar";
    JSDOMWindowAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[9].magic = JSDOMWindow::LocationbarAttrNum;
    JSDOMWindowAttributesFunctions[9].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[10].name = "HTMLTitleElement";
    JSDOMWindowAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[10].magic = JSDOMWindow::HTMLTitleElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[10].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[10].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[11].name = "toolbar";
    JSDOMWindowAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[11].magic = JSDOMWindow::ToolbarAttrNum;
    JSDOMWindowAttributesFunctions[11].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[12].name = "CSSStyleRule";
    JSDOMWindowAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[12].magic = JSDOMWindow::CSSStyleRuleConstructorAttrNum;
    JSDOMWindowAttributesFunctions[12].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[12].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[13].name = "screenTop";
    JSDOMWindowAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[13].magic = JSDOMWindow::ScreenTopAttrNum;
    JSDOMWindowAttributesFunctions[13].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[14].name = "CSSFontFaceRule";
    JSDOMWindowAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[14].magic = JSDOMWindow::CSSFontFaceRuleConstructorAttrNum;
    JSDOMWindowAttributesFunctions[14].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[14].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[15].name = "CSSValueList";
    JSDOMWindowAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[15].magic = JSDOMWindow::CSSValueListConstructorAttrNum;
    JSDOMWindowAttributesFunctions[15].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[15].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[16].name = "document";
    JSDOMWindowAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[16].magic = JSDOMWindow::DocumentAttrNum;
    JSDOMWindowAttributesFunctions[16].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[16].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[17].name = "HTMLEmbedElement";
    JSDOMWindowAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[17].magic = JSDOMWindow::HTMLEmbedElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[17].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[17].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[18].name = "HTMLTableElement";
    JSDOMWindowAttributesFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[18].magic = JSDOMWindow::HTMLTableElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[18].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[18].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[19].name = "CSSCharsetRule";
    JSDOMWindowAttributesFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[19].magic = JSDOMWindow::CSSCharsetRuleConstructorAttrNum;
    JSDOMWindowAttributesFunctions[19].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[19].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[20].name = "HTMLHeadElement";
    JSDOMWindowAttributesFunctions[20].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[20].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[20].magic = JSDOMWindow::HTMLHeadElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[20].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[20].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[21].name = "HTMLTableCellElement";
    JSDOMWindowAttributesFunctions[21].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[21].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[21].magic = JSDOMWindow::HTMLTableCellElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[21].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[21].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[22].name = "HTMLMapElement";
    JSDOMWindowAttributesFunctions[22].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[22].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[22].magic = JSDOMWindow::HTMLMapElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[22].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[22].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[23].name = "outerHeight";
    JSDOMWindowAttributesFunctions[23].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[23].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[23].magic = JSDOMWindow::OuterHeightAttrNum;
    JSDOMWindowAttributesFunctions[23].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[23].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[24].name = "DOMImplementation";
    JSDOMWindowAttributesFunctions[24].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[24].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[24].magic = JSDOMWindow::DOMImplementationConstructorAttrNum;
    JSDOMWindowAttributesFunctions[24].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[24].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[25].name = "screen";
    JSDOMWindowAttributesFunctions[25].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[25].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[25].magic = JSDOMWindow::ScreenAttrNum;
    JSDOMWindowAttributesFunctions[25].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[25].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[26].name = "CSSValue";
    JSDOMWindowAttributesFunctions[26].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[26].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[26].magic = JSDOMWindow::CSSValueConstructorAttrNum;
    JSDOMWindowAttributesFunctions[26].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[26].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[27].name = "parent";
    JSDOMWindowAttributesFunctions[27].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[27].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[27].magic = JSDOMWindow::ParentAttrNum;
    JSDOMWindowAttributesFunctions[27].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[27].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[28].name = "statusbar";
    JSDOMWindowAttributesFunctions[28].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[28].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[28].magic = JSDOMWindow::StatusbarAttrNum;
    JSDOMWindowAttributesFunctions[28].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[28].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[29].name = "pageYOffset";
    JSDOMWindowAttributesFunctions[29].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[29].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[29].magic = JSDOMWindow::PageYOffsetAttrNum;
    JSDOMWindowAttributesFunctions[29].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[29].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[30].name = "Counter";
    JSDOMWindowAttributesFunctions[30].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[30].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[30].magic = JSDOMWindow::CounterConstructorAttrNum;
    JSDOMWindowAttributesFunctions[30].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[30].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[31].name = "CSSStyleSheet";
    JSDOMWindowAttributesFunctions[31].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[31].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[31].magic = JSDOMWindow::CSSStyleSheetConstructorAttrNum;
    JSDOMWindowAttributesFunctions[31].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[31].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[32].name = "pageXOffset";
    JSDOMWindowAttributesFunctions[32].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[32].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[32].magic = JSDOMWindow::PageXOffsetAttrNum;
    JSDOMWindowAttributesFunctions[32].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[32].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[33].name = "HTMLFontElement";
    JSDOMWindowAttributesFunctions[33].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[33].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[33].magic = JSDOMWindow::HTMLFontElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[33].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[33].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[34].name = "HTMLQuoteElement";
    JSDOMWindowAttributesFunctions[34].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[34].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[34].magic = JSDOMWindow::HTMLQuoteElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[34].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[34].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[35].name = "HTMLLabelElement";
    JSDOMWindowAttributesFunctions[35].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[35].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[35].magic = JSDOMWindow::HTMLLabelElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[35].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[35].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[36].name = "DocumentType";
    JSDOMWindowAttributesFunctions[36].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[36].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[36].magic = JSDOMWindow::DocumentTypeConstructorAttrNum;
    JSDOMWindowAttributesFunctions[36].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[36].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[37].name = "Element";
    JSDOMWindowAttributesFunctions[37].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[37].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[37].magic = JSDOMWindow::ElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[37].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[37].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[38].name = "HTMLTableRowElement";
    JSDOMWindowAttributesFunctions[38].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[38].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[38].magic = JSDOMWindow::HTMLTableRowElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[38].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[38].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[39].name = "CSSMediaRule";
    JSDOMWindowAttributesFunctions[39].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[39].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[39].magic = JSDOMWindow::CSSMediaRuleConstructorAttrNum;
    JSDOMWindowAttributesFunctions[39].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[39].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[40].name = "Document";
    JSDOMWindowAttributesFunctions[40].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[40].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[40].magic = JSDOMWindow::DocumentConstructorAttrNum;
    JSDOMWindowAttributesFunctions[40].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[40].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[41].name = "defaultStatus";
    JSDOMWindowAttributesFunctions[41].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[41].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[41].magic = JSDOMWindow::DefaultStatusAttrNum;
    JSDOMWindowAttributesFunctions[41].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[41].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[42].name = "offscreenBuffering";
    JSDOMWindowAttributesFunctions[42].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[42].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[42].magic = JSDOMWindow::OffscreenBufferingAttrNum;
    JSDOMWindowAttributesFunctions[42].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[42].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[43].name = "status";
    JSDOMWindowAttributesFunctions[43].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[43].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[43].magic = JSDOMWindow::StatusAttrNum;
    JSDOMWindowAttributesFunctions[43].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[43].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[44].name = "HTMLHeadingElement";
    JSDOMWindowAttributesFunctions[44].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[44].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[44].magic = JSDOMWindow::HTMLHeadingElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[44].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[44].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[45].name = "self";
    JSDOMWindowAttributesFunctions[45].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[45].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[45].magic = JSDOMWindow::SelfAttrNum;
    JSDOMWindowAttributesFunctions[45].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[45].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[46].name = "HTMLFormElement";
    JSDOMWindowAttributesFunctions[46].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[46].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[46].magic = JSDOMWindow::HTMLFormElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[46].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[46].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[47].name = "name";
    JSDOMWindowAttributesFunctions[47].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[47].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[47].magic = JSDOMWindow::NameAttrNum;
    JSDOMWindowAttributesFunctions[47].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[47].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[48].name = "HTMLImageElement";
    JSDOMWindowAttributesFunctions[48].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[48].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[48].magic = JSDOMWindow::HTMLImageElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[48].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[48].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[49].name = "outerWidth";
    JSDOMWindowAttributesFunctions[49].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[49].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[49].magic = JSDOMWindow::OuterWidthAttrNum;
    JSDOMWindowAttributesFunctions[49].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[49].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[50].name = "HTMLDivElement";
    JSDOMWindowAttributesFunctions[50].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[50].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[50].magic = JSDOMWindow::HTMLDivElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[50].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[50].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[51].name = "CDATASection";
    JSDOMWindowAttributesFunctions[51].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[51].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[51].magic = JSDOMWindow::CDATASectionConstructorAttrNum;
    JSDOMWindowAttributesFunctions[51].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[51].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[52].name = "HTMLInputElement";
    JSDOMWindowAttributesFunctions[52].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[52].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[52].magic = JSDOMWindow::HTMLInputElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[52].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[52].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[53].name = "HTMLCanvasElement";
    JSDOMWindowAttributesFunctions[53].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[53].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[53].magic = JSDOMWindow::HTMLCanvasElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[53].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[53].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[54].name = "NamedNodeMap";
    JSDOMWindowAttributesFunctions[54].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[54].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[54].magic = JSDOMWindow::NamedNodeMapConstructorAttrNum;
    JSDOMWindowAttributesFunctions[54].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[54].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[55].name = "HTMLBodyElement";
    JSDOMWindowAttributesFunctions[55].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[55].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[55].magic = JSDOMWindow::HTMLBodyElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[55].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[55].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[56].name = "HTMLBaseElement";
    JSDOMWindowAttributesFunctions[56].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[56].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[56].magic = JSDOMWindow::HTMLBaseElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[56].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[56].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[57].name = "HTMLMenuElement";
    JSDOMWindowAttributesFunctions[57].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[57].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[57].magic = JSDOMWindow::HTMLMenuElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[57].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[57].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[58].name = "XMLSerializer";
    JSDOMWindowAttributesFunctions[58].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[58].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[58].magic = JSDOMWindow::XMLSerializerConstructorAttrNum;
    JSDOMWindowAttributesFunctions[58].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[58].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[59].name = "HTMLTableCaptionElement";
    JSDOMWindowAttributesFunctions[59].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[59].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[59].magic = JSDOMWindow::HTMLTableCaptionElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[59].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[59].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[60].name = "XPathResult";
    JSDOMWindowAttributesFunctions[60].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[60].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[60].magic = JSDOMWindow::XPathResultConstructorAttrNum;
    JSDOMWindowAttributesFunctions[60].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[60].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[61].name = "HTMLAppletElement";
    JSDOMWindowAttributesFunctions[61].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[61].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[61].magic = JSDOMWindow::HTMLAppletElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[61].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[61].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[62].name = "Entity";
    JSDOMWindowAttributesFunctions[62].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[62].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[62].magic = JSDOMWindow::EntityConstructorAttrNum;
    JSDOMWindowAttributesFunctions[62].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[62].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[63].name = "scrollY";
    JSDOMWindowAttributesFunctions[63].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[63].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[63].magic = JSDOMWindow::ScrollYAttrNum;
    JSDOMWindowAttributesFunctions[63].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[63].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[64].name = "HTMLButtonElement";
    JSDOMWindowAttributesFunctions[64].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[64].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[64].magic = JSDOMWindow::HTMLButtonElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[64].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[64].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[65].name = "Node";
    JSDOMWindowAttributesFunctions[65].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[65].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[65].magic = JSDOMWindow::NodeConstructorAttrNum;
    JSDOMWindowAttributesFunctions[65].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[65].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[66].name = "HTMLBRElement";
    JSDOMWindowAttributesFunctions[66].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[66].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[66].magic = JSDOMWindow::HTMLBRElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[66].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[66].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[67].name = "CSSImportRule";
    JSDOMWindowAttributesFunctions[67].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[67].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[67].magic = JSDOMWindow::CSSImportRuleConstructorAttrNum;
    JSDOMWindowAttributesFunctions[67].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[67].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[68].name = "frameElement";
    JSDOMWindowAttributesFunctions[68].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[68].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[68].magic = JSDOMWindow::FrameElementAttrNum;
    JSDOMWindowAttributesFunctions[68].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[68].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[69].name = "NodeList";
    JSDOMWindowAttributesFunctions[69].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[69].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[69].magic = JSDOMWindow::NodeListConstructorAttrNum;
    JSDOMWindowAttributesFunctions[69].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[69].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[70].name = "window";
    JSDOMWindowAttributesFunctions[70].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[70].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[70].magic = JSDOMWindow::WindowAttrNum;
    JSDOMWindowAttributesFunctions[70].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[70].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[71].name = "innerHeight";
    JSDOMWindowAttributesFunctions[71].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[71].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[71].magic = JSDOMWindow::InnerHeightAttrNum;
    JSDOMWindowAttributesFunctions[71].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[71].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[72].name = "Attr";
    JSDOMWindowAttributesFunctions[72].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[72].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[72].magic = JSDOMWindow::AttrConstructorAttrNum;
    JSDOMWindowAttributesFunctions[72].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[72].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[73].name = "CSSStyleDeclaration";
    JSDOMWindowAttributesFunctions[73].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[73].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[73].magic = JSDOMWindow::CSSStyleDeclarationConstructorAttrNum;
    JSDOMWindowAttributesFunctions[73].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[73].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[74].name = "HTMLMarqueeElement";
    JSDOMWindowAttributesFunctions[74].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[74].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[74].magic = JSDOMWindow::HTMLMarqueeElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[74].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[74].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[75].name = "HTMLFrameElement";
    JSDOMWindowAttributesFunctions[75].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[75].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[75].magic = JSDOMWindow::HTMLFrameElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[75].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[75].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[76].name = "HTMLAreaElement";
    JSDOMWindowAttributesFunctions[76].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[76].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[76].magic = JSDOMWindow::HTMLAreaElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[76].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[76].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[77].name = "screenY";
    JSDOMWindowAttributesFunctions[77].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[77].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[77].magic = JSDOMWindow::ScreenYAttrNum;
    JSDOMWindowAttributesFunctions[77].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[77].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[78].name = "CSSPageRule";
    JSDOMWindowAttributesFunctions[78].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[78].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[78].magic = JSDOMWindow::CSSPageRuleConstructorAttrNum;
    JSDOMWindowAttributesFunctions[78].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[78].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[79].name = "HTMLSelectElement";
    JSDOMWindowAttributesFunctions[79].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[79].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[79].magic = JSDOMWindow::HTMLSelectElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[79].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[79].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[80].name = "EntityReference";
    JSDOMWindowAttributesFunctions[80].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[80].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[80].magic = JSDOMWindow::EntityReferenceConstructorAttrNum;
    JSDOMWindowAttributesFunctions[80].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[80].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[81].name = "DOMParser";
    JSDOMWindowAttributesFunctions[81].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[81].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[81].magic = JSDOMWindow::DOMParserConstructorAttrNum;
    JSDOMWindowAttributesFunctions[81].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[81].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[82].name = "Notation";
    JSDOMWindowAttributesFunctions[82].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[82].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[82].magic = JSDOMWindow::NotationConstructorAttrNum;
    JSDOMWindowAttributesFunctions[82].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[82].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[83].name = "scrollX";
    JSDOMWindowAttributesFunctions[83].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[83].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[83].magic = JSDOMWindow::ScrollXAttrNum;
    JSDOMWindowAttributesFunctions[83].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[83].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[84].name = "HTMLFieldSetElement";
    JSDOMWindowAttributesFunctions[84].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[84].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[84].magic = JSDOMWindow::HTMLFieldSetElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[84].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[84].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[85].name = "CharacterData";
    JSDOMWindowAttributesFunctions[85].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[85].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[85].magic = JSDOMWindow::CharacterDataConstructorAttrNum;
    JSDOMWindowAttributesFunctions[85].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[85].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[86].name = "NodeFilter";
    JSDOMWindowAttributesFunctions[86].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[86].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[86].magic = JSDOMWindow::NodeFilterConstructorAttrNum;
    JSDOMWindowAttributesFunctions[86].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[86].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[87].name = "Rect";
    JSDOMWindowAttributesFunctions[87].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[87].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[87].magic = JSDOMWindow::RectConstructorAttrNum;
    JSDOMWindowAttributesFunctions[87].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[87].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[88].name = "CSSPrimitiveValue";
    JSDOMWindowAttributesFunctions[88].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[88].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[88].magic = JSDOMWindow::CSSPrimitiveValueConstructorAttrNum;
    JSDOMWindowAttributesFunctions[88].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[88].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[89].name = "screenLeft";
    JSDOMWindowAttributesFunctions[89].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[89].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[89].magic = JSDOMWindow::ScreenLeftAttrNum;
    JSDOMWindowAttributesFunctions[89].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[89].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[90].name = "defaultstatus";
    JSDOMWindowAttributesFunctions[90].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[90].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[90].magic = JSDOMWindow::DefaultstatusAttrNum;
    JSDOMWindowAttributesFunctions[90].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[90].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[91].name = "HTMLDocument";
    JSDOMWindowAttributesFunctions[91].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[91].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[91].magic = JSDOMWindow::HTMLDocumentConstructorAttrNum;
    JSDOMWindowAttributesFunctions[91].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[91].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[92].name = "DocumentFragment";
    JSDOMWindowAttributesFunctions[92].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[92].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[92].magic = JSDOMWindow::DocumentFragmentConstructorAttrNum;
    JSDOMWindowAttributesFunctions[92].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[92].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[93].name = "MutationEvent";
    JSDOMWindowAttributesFunctions[93].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[93].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[93].magic = JSDOMWindow::MutationEventConstructorAttrNum;
    JSDOMWindowAttributesFunctions[93].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[93].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[94].name = "closed";
    JSDOMWindowAttributesFunctions[94].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[94].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[94].magic = JSDOMWindow::ClosedAttrNum;
    JSDOMWindowAttributesFunctions[94].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[94].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[95].name = "scrollbars";
    JSDOMWindowAttributesFunctions[95].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[95].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[95].magic = JSDOMWindow::ScrollbarsAttrNum;
    JSDOMWindowAttributesFunctions[95].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[95].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[96].name = "innerWidth";
    JSDOMWindowAttributesFunctions[96].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[96].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[96].magic = JSDOMWindow::InnerWidthAttrNum;
    JSDOMWindowAttributesFunctions[96].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[96].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[97].name = "length";
    JSDOMWindowAttributesFunctions[97].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[97].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[97].magic = JSDOMWindow::LengthAttrNum;
    JSDOMWindowAttributesFunctions[97].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[97].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[98].name = "top";
    JSDOMWindowAttributesFunctions[98].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[98].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[98].magic = JSDOMWindow::TopAttrNum;
    JSDOMWindowAttributesFunctions[98].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[98].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[99].name = "devicePixelRatio";
    JSDOMWindowAttributesFunctions[99].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[99].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[99].magic = JSDOMWindow::DevicePixelRatioAttrNum;
    JSDOMWindowAttributesFunctions[99].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[99].u.getset.set.setter_magic = NULL;
    JSDOMWindowAttributesFunctions[100].name = "StyleSheet";
    JSDOMWindowAttributesFunctions[100].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[100].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[100].magic = JSDOMWindow::StyleSheetConstructorAttrNum;
    JSDOMWindowAttributesFunctions[100].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[100].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[101].name = "MediaList";
    JSDOMWindowAttributesFunctions[101].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[101].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[101].magic = JSDOMWindow::MediaListConstructorAttrNum;
    JSDOMWindowAttributesFunctions[101].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[101].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[102].name = "CSSRuleList";
    JSDOMWindowAttributesFunctions[102].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[102].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[102].magic = JSDOMWindow::CSSRuleListConstructorAttrNum;
    JSDOMWindowAttributesFunctions[102].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[102].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[103].name = "StyleSheetList";
    JSDOMWindowAttributesFunctions[103].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[103].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[103].magic = JSDOMWindow::StyleSheetListConstructorAttrNum;
    JSDOMWindowAttributesFunctions[103].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[103].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[104].name = "Text";
    JSDOMWindowAttributesFunctions[104].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[104].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[104].magic = JSDOMWindow::TextConstructorAttrNum;
    JSDOMWindowAttributesFunctions[104].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[104].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[105].name = "Comment";
    JSDOMWindowAttributesFunctions[105].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[105].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[105].magic = JSDOMWindow::CommentConstructorAttrNum;
    JSDOMWindowAttributesFunctions[105].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[105].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[106].name = "ProcessingInstruction";
    JSDOMWindowAttributesFunctions[106].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[106].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[106].magic = JSDOMWindow::ProcessingInstructionConstructorAttrNum;
    JSDOMWindowAttributesFunctions[106].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[106].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[107].name = "HTMLElement";
    JSDOMWindowAttributesFunctions[107].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[107].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[107].magic = JSDOMWindow::HTMLElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[107].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[107].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[108].name = "HTMLAnchorElement";
    JSDOMWindowAttributesFunctions[108].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[108].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[108].magic = JSDOMWindow::HTMLAnchorElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[108].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[108].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[109].name = "HTMLBlockquoteElement";
    JSDOMWindowAttributesFunctions[109].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[109].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[109].magic = JSDOMWindow::HTMLBlockquoteElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[109].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[109].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[110].name = "HTMLDListElement";
    JSDOMWindowAttributesFunctions[110].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[110].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[110].magic = JSDOMWindow::HTMLDListElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[110].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[110].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[111].name = "HTMLDirectoryElement";
    JSDOMWindowAttributesFunctions[111].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[111].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[111].magic = JSDOMWindow::HTMLDirectoryElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[111].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[111].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[112].name = "HTMLFrameSetElement";
    JSDOMWindowAttributesFunctions[112].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[112].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[112].magic = JSDOMWindow::HTMLFrameSetElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[112].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[112].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[113].name = "HTMLHRElement";
    JSDOMWindowAttributesFunctions[113].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[113].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[113].magic = JSDOMWindow::HTMLHRElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[113].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[113].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[114].name = "HTMLHtmlElement";
    JSDOMWindowAttributesFunctions[114].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[114].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[114].magic = JSDOMWindow::HTMLHtmlElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[114].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[114].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[115].name = "HTMLIFrameElement";
    JSDOMWindowAttributesFunctions[115].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[115].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[115].magic = JSDOMWindow::HTMLIFrameElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[115].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[115].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[116].name = "HTMLIsIndexElement";
    JSDOMWindowAttributesFunctions[116].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[116].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[116].magic = JSDOMWindow::HTMLIsIndexElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[116].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[116].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[117].name = "HTMLLIElement";
    JSDOMWindowAttributesFunctions[117].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[117].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[117].magic = JSDOMWindow::HTMLLIElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[117].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[117].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[118].name = "HTMLLegendElement";
    JSDOMWindowAttributesFunctions[118].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[118].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[118].magic = JSDOMWindow::HTMLLegendElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[118].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[118].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[119].name = "HTMLLinkElement";
    JSDOMWindowAttributesFunctions[119].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[119].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[119].magic = JSDOMWindow::HTMLLinkElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[119].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[119].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[120].name = "HTMLMetaElement";
    JSDOMWindowAttributesFunctions[120].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[120].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[120].magic = JSDOMWindow::HTMLMetaElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[120].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[120].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[121].name = "HTMLModElement";
    JSDOMWindowAttributesFunctions[121].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[121].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[121].magic = JSDOMWindow::HTMLModElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[121].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[121].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[122].name = "HTMLOListElement";
    JSDOMWindowAttributesFunctions[122].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[122].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[122].magic = JSDOMWindow::HTMLOListElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[122].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[122].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[123].name = "HTMLObjectElement";
    JSDOMWindowAttributesFunctions[123].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[123].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[123].magic = JSDOMWindow::HTMLObjectElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[123].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[123].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[124].name = "HTMLOptGroupElement";
    JSDOMWindowAttributesFunctions[124].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[124].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[124].magic = JSDOMWindow::HTMLOptGroupElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[124].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[124].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[125].name = "HTMLOptionElement";
    JSDOMWindowAttributesFunctions[125].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[125].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[125].magic = JSDOMWindow::HTMLOptionElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[125].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[125].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[126].name = "HTMLParagraphElement";
    JSDOMWindowAttributesFunctions[126].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[126].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[126].magic = JSDOMWindow::HTMLParagraphElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[126].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[126].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[127].name = "HTMLPreElement";
    JSDOMWindowAttributesFunctions[127].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[127].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[127].magic = JSDOMWindow::HTMLPreElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[127].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[127].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[128].name = "HTMLScriptElement";
    JSDOMWindowAttributesFunctions[128].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[128].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[128].magic = JSDOMWindow::HTMLScriptElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[128].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[128].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[129].name = "HTMLStyleElement";
    JSDOMWindowAttributesFunctions[129].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[129].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[129].magic = JSDOMWindow::HTMLStyleElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[129].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[129].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[130].name = "HTMLTableColElement";
    JSDOMWindowAttributesFunctions[130].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[130].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[130].magic = JSDOMWindow::HTMLTableColElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[130].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[130].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[131].name = "HTMLTableSectionElement";
    JSDOMWindowAttributesFunctions[131].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[131].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[131].magic = JSDOMWindow::HTMLTableSectionElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[131].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[131].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[132].name = "HTMLTextAreaElement";
    JSDOMWindowAttributesFunctions[132].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[132].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[132].magic = JSDOMWindow::HTMLTextAreaElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[132].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[132].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[133].name = "HTMLUListElement";
    JSDOMWindowAttributesFunctions[133].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[133].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[133].magic = JSDOMWindow::HTMLUListElementConstructorAttrNum;
    JSDOMWindowAttributesFunctions[133].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[133].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[134].name = "Event";
    JSDOMWindowAttributesFunctions[134].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[134].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[134].magic = JSDOMWindow::EventConstructorAttrNum;
    JSDOMWindowAttributesFunctions[134].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[134].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[135].name = "Range";
    JSDOMWindowAttributesFunctions[135].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[135].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[135].magic = JSDOMWindow::RangeConstructorAttrNum;
    JSDOMWindowAttributesFunctions[135].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[135].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[136].name = "RangeException";
    JSDOMWindowAttributesFunctions[136].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[136].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[136].magic = JSDOMWindow::RangeExceptionConstructorAttrNum;
    JSDOMWindowAttributesFunctions[136].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[136].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[137].name = "XMLDocument";
    JSDOMWindowAttributesFunctions[137].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[137].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[137].magic = JSDOMWindow::XMLDocumentConstructorAttrNum;
    JSDOMWindowAttributesFunctions[137].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[137].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
    JSDOMWindowAttributesFunctions[138].name = "XPathEvaluator";
    JSDOMWindowAttributesFunctions[138].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMWindowAttributesFunctions[138].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMWindowAttributesFunctions[138].magic = JSDOMWindow::XPathEvaluatorConstructorAttrNum;
    JSDOMWindowAttributesFunctions[138].u.getset.get.getter_magic = JSDOMWindow::getValueProperty;
    JSDOMWindowAttributesFunctions[138].u.getset.set.setter_magic = JSDOMWindow::putValueProperty;
}

/* Prototype functions table */

static JSCFunctionListEntry JSDOMWindowPrototypeFunctions[12];
static bool JSDOMWindowPrototypeFunctions_initialized = false;

static void init_JSDOMWindowPrototypeFunctions()
{
    if (JSDOMWindowPrototypeFunctions_initialized) return;
    JSDOMWindowPrototypeFunctions_initialized = true;
    memset(JSDOMWindowPrototypeFunctions, 0, sizeof(JSDOMWindowPrototypeFunctions));
    JSDOMWindowPrototypeFunctions[0].name = "alert";
    JSDOMWindowPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[0].magic = JSDOMWindow::AlertFuncNum;
    JSDOMWindowPrototypeFunctions[0].u.func.length = 1;
    JSDOMWindowPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[0].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[1].name = "blur";
    JSDOMWindowPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[1].magic = JSDOMWindow::BlurFuncNum;
    JSDOMWindowPrototypeFunctions[1].u.func.length = 0;
    JSDOMWindowPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[1].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[2].name = "getSelection";
    JSDOMWindowPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[2].magic = JSDOMWindow::GetSelectionFuncNum;
    JSDOMWindowPrototypeFunctions[2].u.func.length = 0;
    JSDOMWindowPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[2].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[3].name = "find";
    JSDOMWindowPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[3].magic = JSDOMWindow::FindFuncNum;
    JSDOMWindowPrototypeFunctions[3].u.func.length = 7;
    JSDOMWindowPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[3].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[4].name = "close";
    JSDOMWindowPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[4].magic = JSDOMWindow::CloseFuncNum;
    JSDOMWindowPrototypeFunctions[4].u.func.length = 0;
    JSDOMWindowPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[4].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[5].name = "getComputedStyle";
    JSDOMWindowPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[5].magic = JSDOMWindow::GetComputedStyleFuncNum;
    JSDOMWindowPrototypeFunctions[5].u.func.length = 2;
    JSDOMWindowPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[5].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[6].name = "focus";
    JSDOMWindowPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[6].magic = JSDOMWindow::FocusFuncNum;
    JSDOMWindowPrototypeFunctions[6].u.func.length = 0;
    JSDOMWindowPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[6].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[7].name = "prompt";
    JSDOMWindowPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[7].magic = JSDOMWindow::PromptFuncNum;
    JSDOMWindowPrototypeFunctions[7].u.func.length = 2;
    JSDOMWindowPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[7].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[8].name = "print";
    JSDOMWindowPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[8].magic = JSDOMWindow::PrintFuncNum;
    JSDOMWindowPrototypeFunctions[8].u.func.length = 0;
    JSDOMWindowPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[8].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[9].name = "stop";
    JSDOMWindowPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[9].magic = JSDOMWindow::StopFuncNum;
    JSDOMWindowPrototypeFunctions[9].u.func.length = 0;
    JSDOMWindowPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[9].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[10].name = "confirm";
    JSDOMWindowPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[10].magic = JSDOMWindow::ConfirmFuncNum;
    JSDOMWindowPrototypeFunctions[10].u.func.length = 1;
    JSDOMWindowPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[10].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
    JSDOMWindowPrototypeFunctions[11].name = "getMatchedCSSRules";
    JSDOMWindowPrototypeFunctions[11].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMWindowPrototypeFunctions[11].def_type = JS_DEF_CFUNC;
    JSDOMWindowPrototypeFunctions[11].magic = JSDOMWindow::GetMatchedCSSRulesFuncNum;
    JSDOMWindowPrototypeFunctions[11].u.func.length = 3;
    JSDOMWindowPrototypeFunctions[11].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMWindowPrototypeFunctions[11].u.func.cfunc.generic_magic = JSDOMWindowPrototypeFunction::callAsFunction;
}

JSValue JSDOMWindowPrototype::self(JSContext * ctx)
{
    JSValue obj = JS_NewObjectProto(ctx, WindowPrototype::self(ctx));
    JSDOMWindowPrototype::initPrototype(ctx, obj);
    return obj;
}

void JSDOMWindowPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSDOMWindowAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMWindowAttributesFunctions, countof(JSDOMWindowAttributesFunctions));
    init_JSDOMWindowPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMWindowPrototypeFunctions, countof(JSDOMWindowPrototypeFunctions));
}

static JSClassDef JSDOMWindowClassDefine;
static bool JSDOMWindowClassDefine_initialized = false;

static void init_JSDOMWindowClassDefine()
{
    if (JSDOMWindowClassDefine_initialized) return;
    JSDOMWindowClassDefine_initialized = true;
    memset(&JSDOMWindowClassDefine, 0, sizeof(JSDOMWindowClassDefine));
    JSDOMWindowClassDefine.class_name = "DOMWindow";
    JSDOMWindowClassDefine.finalizer = JSDOMWindow::finalizer;
    JSDOMWindowClassDefine.gc_mark = JSDOMWindow::mark;
}

JSClassID JSDOMWindow::js_class_id = 0;

void JSDOMWindow::init(JSContext* ctx)
{
    if (JSDOMWindow::js_class_id == 0) {
        init_JSDOMWindowClassDefine();
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
    if (!impl)
        return;
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
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->screen()));
        }
        case HistoryAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->history()));
        }
        case LocationbarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->locationbar()));
        }
        case MenubarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->menubar()));
        }
        case PersonalbarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->personalbar()));
        }
        case ScrollbarsAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->scrollbars()));
        }
        case StatusbarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->statusbar()));
        }
        case ToolbarAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->toolbar()));
        }
        case FrameElementAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return checkNodeSecurity(ctx, imp->frameElement()) ? toJS(ctx, QJS::getPtr(imp->frameElement())) : JS_UNDEFINED;
        }
        case OffscreenBufferingAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewBool(ctx, imp->offscreenBuffering() ? 1 : 0);
        }
        case OuterHeightAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->outerHeight());
        }
        case OuterWidthAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->outerWidth());
        }
        case InnerHeightAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->innerHeight());
        }
        case InnerWidthAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->innerWidth());
        }
        case ScreenXAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->screenX());
        }
        case ScreenYAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->screenY());
        }
        case ScreenLeftAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->screenLeft());
        }
        case ScreenTopAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->screenTop());
        }
        case ScrollXAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->scrollX());
        }
        case ScrollYAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->scrollY());
        }
        case PageXOffsetAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->pageXOffset());
        }
        case PageYOffsetAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->pageYOffset());
        }
        case ClosedAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewBool(ctx, imp->closed() ? 1 : 0);
        }
        case LengthAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
        case NameAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case StatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->status()).utf8().data());
        }
        case DefaultStatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->defaultStatus()).utf8().data());
        }
        case DefaultstatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->defaultstatus()).utf8().data());
        }
        case SelfAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->self()));
        }
        case WindowAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->window()));
        }
        case FramesAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->frames()));
        }
        case OpenerAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->opener()));
        }
        case ParentAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parent()));
        }
        case TopAttrNum: {
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->top()));
        }
        case DocumentAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->document()));
        }
        case DevicePixelRatioAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            return JS_NewInt32(ctx, imp->devicePixelRatio());
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
        case XPathEvaluatorConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSXPathEvaluator::getConstructor(ctx);
        }
        case XPathResultConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            return JSXPathResult::getConstructor(ctx);
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
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            imp->setName(valueToString(ctx, value));
            break;
        }
        case StatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            imp->setStatus(valueToString(ctx, value));
            break;
        }
        case DefaultStatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
            imp->setDefaultStatus(valueToString(ctx, value));
            break;
        }
        case DefaultstatusAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
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
        case XPathEvaluatorConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "XPathEvaluator", value);
            break;
        }
        case XPathResultConstructorAttrNum: {
            if (!isSafeScript(ctx, this_val))
                return JS_UNDEFINED;
            // Shadowing a built-in constructor
            JS_SetPropertyStr(ctx, this_val, "XPathResult", value);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSDOMWindowPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    DOMWindow* imp = (DOMWindow*)JS_GetOpaque(this_val, JSDOMWindow::js_class_id);
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
