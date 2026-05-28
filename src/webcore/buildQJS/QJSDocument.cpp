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
#include "QJSCustomXPathNSResolver.h"
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
#include "QJSXPathExpression.h"
#include "QJSXPathNSResolver.h"
#include "QJSXPathResult.h"
#include "Range.h"
#include "StyleSheetList.h"
#include "Text.h"
#include "TreeWalker.h"
#include "XPathExpression.h"
#include "XPathNSResolver.h"
#include "XPathResult.h"
#include "qjs_window.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSDocumentAttributesFunctions[30];
static bool JSDocumentAttributesFunctions_initialized = false;

static void init_JSDocumentAttributesFunctions()
{
    if (JSDocumentAttributesFunctions_initialized) return;
    JSDocumentAttributesFunctions_initialized = true;
    memset(JSDocumentAttributesFunctions, 0, sizeof(JSDocumentAttributesFunctions));
    JSDocumentAttributesFunctions[0].name = "title";
    JSDocumentAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[0].magic = JSDocument::TitleAttrNum;
    JSDocumentAttributesFunctions[0].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[0].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[1].name = "anchors";
    JSDocumentAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[1].magic = JSDocument::AnchorsAttrNum;
    JSDocumentAttributesFunctions[1].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[2].name = "domain";
    JSDocumentAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[2].magic = JSDocument::DomainAttrNum;
    JSDocumentAttributesFunctions[2].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[2].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[3].name = "documentURI";
    JSDocumentAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[3].magic = JSDocument::DocumentURIAttrNum;
    JSDocumentAttributesFunctions[3].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[3].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[4].name = "documentElement";
    JSDocumentAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[4].magic = JSDocument::DocumentElementAttrNum;
    JSDocumentAttributesFunctions[4].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[5].name = "URL";
    JSDocumentAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[5].magic = JSDocument::URLAttrNum;
    JSDocumentAttributesFunctions[5].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[6].name = "async";
    JSDocumentAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[6].magic = JSDocument::AsyncAttrNum;
    JSDocumentAttributesFunctions[6].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[6].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[7].name = "doctype";
    JSDocumentAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[7].magic = JSDocument::DoctypeAttrNum;
    JSDocumentAttributesFunctions[7].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[8].name = "xmlEncoding";
    JSDocumentAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[8].magic = JSDocument::XMLEncodingAttrNum;
    JSDocumentAttributesFunctions[8].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[9].name = "applets";
    JSDocumentAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[9].magic = JSDocument::AppletsAttrNum;
    JSDocumentAttributesFunctions[9].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[10].name = "defaultView";
    JSDocumentAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[10].magic = JSDocument::DefaultViewAttrNum;
    JSDocumentAttributesFunctions[10].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[11].name = "xmlVersion";
    JSDocumentAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[11].magic = JSDocument::XMLVersionAttrNum;
    JSDocumentAttributesFunctions[11].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[11].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[12].name = "implementation";
    JSDocumentAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[12].magic = JSDocument::ImplementationAttrNum;
    JSDocumentAttributesFunctions[12].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[13].name = "cookie";
    JSDocumentAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[13].magic = JSDocument::CookieAttrNum;
    JSDocumentAttributesFunctions[13].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[13].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[14].name = "images";
    JSDocumentAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[14].magic = JSDocument::ImagesAttrNum;
    JSDocumentAttributesFunctions[14].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[15].name = "inputEncoding";
    JSDocumentAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[15].magic = JSDocument::InputEncodingAttrNum;
    JSDocumentAttributesFunctions[15].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[16].name = "links";
    JSDocumentAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[16].magic = JSDocument::LinksAttrNum;
    JSDocumentAttributesFunctions[16].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[16].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[17].name = "location";
    JSDocumentAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[17].magic = JSDocument::LocationAttrNum;
    JSDocumentAttributesFunctions[17].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[17].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[18].name = "defaultCharset";
    JSDocumentAttributesFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[18].magic = JSDocument::DefaultCharsetAttrNum;
    JSDocumentAttributesFunctions[18].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[18].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[19].name = "xmlStandalone";
    JSDocumentAttributesFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[19].magic = JSDocument::XMLStandaloneAttrNum;
    JSDocumentAttributesFunctions[19].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[19].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[20].name = "styleSheets";
    JSDocumentAttributesFunctions[20].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[20].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[20].magic = JSDocument::StyleSheetsAttrNum;
    JSDocumentAttributesFunctions[20].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[20].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[21].name = "referrer";
    JSDocumentAttributesFunctions[21].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[21].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[21].magic = JSDocument::ReferrerAttrNum;
    JSDocumentAttributesFunctions[21].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[21].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[22].name = "body";
    JSDocumentAttributesFunctions[22].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[22].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[22].magic = JSDocument::BodyAttrNum;
    JSDocumentAttributesFunctions[22].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[22].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[23].name = "forms";
    JSDocumentAttributesFunctions[23].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[23].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[23].magic = JSDocument::FormsAttrNum;
    JSDocumentAttributesFunctions[23].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[23].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[24].name = "charset";
    JSDocumentAttributesFunctions[24].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[24].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[24].magic = JSDocument::CharsetAttrNum;
    JSDocumentAttributesFunctions[24].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[24].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[25].name = "readyState";
    JSDocumentAttributesFunctions[25].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[25].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[25].magic = JSDocument::ReadyStateAttrNum;
    JSDocumentAttributesFunctions[25].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[25].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[26].name = "characterSet";
    JSDocumentAttributesFunctions[26].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[26].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[26].magic = JSDocument::CharacterSetAttrNum;
    JSDocumentAttributesFunctions[26].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[26].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[27].name = "preferredStylesheetSet";
    JSDocumentAttributesFunctions[27].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[27].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[27].magic = JSDocument::PreferredStylesheetSetAttrNum;
    JSDocumentAttributesFunctions[27].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[27].u.getset.set.setter_magic = NULL;
    JSDocumentAttributesFunctions[28].name = "selectedStylesheetSet";
    JSDocumentAttributesFunctions[28].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[28].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[28].magic = JSDocument::SelectedStylesheetSetAttrNum;
    JSDocumentAttributesFunctions[28].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[28].u.getset.set.setter_magic = JSDocument::putValueProperty;
    JSDocumentAttributesFunctions[29].name = "constructor";
    JSDocumentAttributesFunctions[29].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentAttributesFunctions[29].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentAttributesFunctions[29].magic = JSDocument::ConstructorAttrNum;
    JSDocumentAttributesFunctions[29].u.getset.get.getter_magic = JSDocument::getValueProperty;
    JSDocumentAttributesFunctions[29].u.getset.set.setter_magic = NULL;
}

class JSDocumentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSDocumentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Document.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSDocumentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Document.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDocumentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSDocumentPrototypeFunctions[32];
static bool JSDocumentPrototypeFunctions_initialized = false;

static void init_JSDocumentPrototypeFunctions()
{
    if (JSDocumentPrototypeFunctions_initialized) return;
    JSDocumentPrototypeFunctions_initialized = true;
    memset(JSDocumentPrototypeFunctions, 0, sizeof(JSDocumentPrototypeFunctions));
    JSDocumentPrototypeFunctions[0].name = "importNode";
    JSDocumentPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[0].magic = JSDocument::ImportNodeFuncNum;
    JSDocumentPrototypeFunctions[0].u.func.length = 2;
    JSDocumentPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[0].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[1].name = "createProcessingInstruction";
    JSDocumentPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[1].magic = JSDocument::CreateProcessingInstructionFuncNum;
    JSDocumentPrototypeFunctions[1].u.func.length = 2;
    JSDocumentPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[1].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[2].name = "createElementNS";
    JSDocumentPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[2].magic = JSDocument::CreateElementNSFuncNum;
    JSDocumentPrototypeFunctions[2].u.func.length = 2;
    JSDocumentPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[2].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[3].name = "createDocumentFragment";
    JSDocumentPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[3].magic = JSDocument::CreateDocumentFragmentFuncNum;
    JSDocumentPrototypeFunctions[3].u.func.length = 0;
    JSDocumentPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[3].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[4].name = "createComment";
    JSDocumentPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[4].magic = JSDocument::CreateCommentFuncNum;
    JSDocumentPrototypeFunctions[4].u.func.length = 1;
    JSDocumentPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[4].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[5].name = "createCDATASection";
    JSDocumentPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[5].magic = JSDocument::CreateCDATASectionFuncNum;
    JSDocumentPrototypeFunctions[5].u.func.length = 1;
    JSDocumentPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[5].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[6].name = "createAttribute";
    JSDocumentPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[6].magic = JSDocument::CreateAttributeFuncNum;
    JSDocumentPrototypeFunctions[6].u.func.length = 1;
    JSDocumentPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[6].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[7].name = "queryCommandIndeterm";
    JSDocumentPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[7].magic = JSDocument::QueryCommandIndetermFuncNum;
    JSDocumentPrototypeFunctions[7].u.func.length = 1;
    JSDocumentPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[7].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[8].name = "adoptNode";
    JSDocumentPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[8].magic = JSDocument::AdoptNodeFuncNum;
    JSDocumentPrototypeFunctions[8].u.func.length = 1;
    JSDocumentPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[8].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[9].name = "queryCommandEnabled";
    JSDocumentPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[9].magic = JSDocument::QueryCommandEnabledFuncNum;
    JSDocumentPrototypeFunctions[9].u.func.length = 1;
    JSDocumentPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[9].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[10].name = "getElementsByTagNameNS";
    JSDocumentPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[10].magic = JSDocument::GetElementsByTagNameNSFuncNum;
    JSDocumentPrototypeFunctions[10].u.func.length = 2;
    JSDocumentPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[10].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[11].name = "getElementsByTagName";
    JSDocumentPrototypeFunctions[11].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[11].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[11].magic = JSDocument::GetElementsByTagNameFuncNum;
    JSDocumentPrototypeFunctions[11].u.func.length = 1;
    JSDocumentPrototypeFunctions[11].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[11].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[12].name = "createEvent";
    JSDocumentPrototypeFunctions[12].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[12].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[12].magic = JSDocument::CreateEventFuncNum;
    JSDocumentPrototypeFunctions[12].u.func.length = 1;
    JSDocumentPrototypeFunctions[12].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[12].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[13].name = "createNodeIterator";
    JSDocumentPrototypeFunctions[13].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[13].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[13].magic = JSDocument::CreateNodeIteratorFuncNum;
    JSDocumentPrototypeFunctions[13].u.func.length = 4;
    JSDocumentPrototypeFunctions[13].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[13].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[14].name = "createTextNode";
    JSDocumentPrototypeFunctions[14].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[14].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[14].magic = JSDocument::CreateTextNodeFuncNum;
    JSDocumentPrototypeFunctions[14].u.func.length = 1;
    JSDocumentPrototypeFunctions[14].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[14].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[15].name = "createAttributeNS";
    JSDocumentPrototypeFunctions[15].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[15].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[15].magic = JSDocument::CreateAttributeNSFuncNum;
    JSDocumentPrototypeFunctions[15].u.func.length = 2;
    JSDocumentPrototypeFunctions[15].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[15].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[16].name = "createElement";
    JSDocumentPrototypeFunctions[16].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[16].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[16].magic = JSDocument::CreateElementFuncNum;
    JSDocumentPrototypeFunctions[16].u.func.length = 1;
    JSDocumentPrototypeFunctions[16].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[16].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[17].name = "createEntityReference";
    JSDocumentPrototypeFunctions[17].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[17].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[17].magic = JSDocument::CreateEntityReferenceFuncNum;
    JSDocumentPrototypeFunctions[17].u.func.length = 1;
    JSDocumentPrototypeFunctions[17].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[17].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[18].name = "getElementById";
    JSDocumentPrototypeFunctions[18].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[18].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[18].magic = JSDocument::GetElementByIdFuncNum;
    JSDocumentPrototypeFunctions[18].u.func.length = 1;
    JSDocumentPrototypeFunctions[18].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[18].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[19].name = "load";
    JSDocumentPrototypeFunctions[19].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[19].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[19].magic = JSDocument::LoadFuncNum;
    JSDocumentPrototypeFunctions[19].u.func.length = 1;
    JSDocumentPrototypeFunctions[19].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[19].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[20].name = "createRange";
    JSDocumentPrototypeFunctions[20].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[20].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[20].magic = JSDocument::CreateRangeFuncNum;
    JSDocumentPrototypeFunctions[20].u.func.length = 0;
    JSDocumentPrototypeFunctions[20].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[20].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[21].name = "createTreeWalker";
    JSDocumentPrototypeFunctions[21].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[21].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[21].magic = JSDocument::CreateTreeWalkerFuncNum;
    JSDocumentPrototypeFunctions[21].u.func.length = 4;
    JSDocumentPrototypeFunctions[21].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[21].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[22].name = "getOverrideStyle";
    JSDocumentPrototypeFunctions[22].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[22].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[22].magic = JSDocument::GetOverrideStyleFuncNum;
    JSDocumentPrototypeFunctions[22].u.func.length = 2;
    JSDocumentPrototypeFunctions[22].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[22].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[23].name = "createExpression";
    JSDocumentPrototypeFunctions[23].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[23].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[23].magic = JSDocument::CreateExpressionFuncNum;
    JSDocumentPrototypeFunctions[23].u.func.length = 2;
    JSDocumentPrototypeFunctions[23].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[23].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[24].name = "createNSResolver";
    JSDocumentPrototypeFunctions[24].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[24].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[24].magic = JSDocument::CreateNSResolverFuncNum;
    JSDocumentPrototypeFunctions[24].u.func.length = 1;
    JSDocumentPrototypeFunctions[24].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[24].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[25].name = "evaluate";
    JSDocumentPrototypeFunctions[25].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[25].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[25].magic = JSDocument::EvaluateFuncNum;
    JSDocumentPrototypeFunctions[25].u.func.length = 5;
    JSDocumentPrototypeFunctions[25].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[25].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[26].name = "execCommand";
    JSDocumentPrototypeFunctions[26].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[26].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[26].magic = JSDocument::ExecCommandFuncNum;
    JSDocumentPrototypeFunctions[26].u.func.length = 3;
    JSDocumentPrototypeFunctions[26].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[26].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[27].name = "queryCommandState";
    JSDocumentPrototypeFunctions[27].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[27].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[27].magic = JSDocument::QueryCommandStateFuncNum;
    JSDocumentPrototypeFunctions[27].u.func.length = 1;
    JSDocumentPrototypeFunctions[27].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[27].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[28].name = "queryCommandSupported";
    JSDocumentPrototypeFunctions[28].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[28].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[28].magic = JSDocument::QueryCommandSupportedFuncNum;
    JSDocumentPrototypeFunctions[28].u.func.length = 1;
    JSDocumentPrototypeFunctions[28].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[28].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[29].name = "queryCommandValue";
    JSDocumentPrototypeFunctions[29].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[29].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[29].magic = JSDocument::QueryCommandValueFuncNum;
    JSDocumentPrototypeFunctions[29].u.func.length = 1;
    JSDocumentPrototypeFunctions[29].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[29].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[30].name = "getElementsByName";
    JSDocumentPrototypeFunctions[30].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[30].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[30].magic = JSDocument::GetElementsByNameFuncNum;
    JSDocumentPrototypeFunctions[30].u.func.length = 1;
    JSDocumentPrototypeFunctions[30].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[30].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
    JSDocumentPrototypeFunctions[31].name = "elementFromPoint";
    JSDocumentPrototypeFunctions[31].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDocumentPrototypeFunctions[31].def_type = JS_DEF_CFUNC;
    JSDocumentPrototypeFunctions[31].magic = JSDocument::ElementFromPointFuncNum;
    JSDocumentPrototypeFunctions[31].u.func.length = 2;
    JSDocumentPrototypeFunctions[31].u.func.cproto = JS_CFUNC_generic_magic;
    JSDocumentPrototypeFunctions[31].u.func.cfunc.generic_magic = JSDocumentPrototypeFunction::callAsFunction;
}

JSValue JSDocumentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDocument.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSDocumentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDocument.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDocumentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSDocumentAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDocumentAttributesFunctions, countof(JSDocumentAttributesFunctions));
    init_JSDocumentPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDocumentPrototypeFunctions, countof(JSDocumentPrototypeFunctions));
}

static JSClassDef JSDocumentClassDefine;
static bool JSDocumentClassDefine_initialized = false;

static void init_JSDocumentClassDefine()
{
    if (JSDocumentClassDefine_initialized) return;
    JSDocumentClassDefine_initialized = true;
    memset(&JSDocumentClassDefine, 0, sizeof(JSDocumentClassDefine));
    JSDocumentClassDefine.class_name = "Document";
    JSDocumentClassDefine.finalizer = JSDocument::finalizer;
    JSDocumentClassDefine.gc_mark = JSDocument::mark;
}

JSClassID JSDocument::js_class_id = 0;

void JSDocument::init(JSContext* ctx)
{
    if (JSDocument::js_class_id == 0) {
        JSNode::init(ctx);
        JSDocument::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSDocument::create(JSContext* ctx, Document* impl)
{
    JSDocument::init(ctx);
    JSValue _proto = JSDocumentPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDocument::finalizer(JSRuntime* rt, JSValue val)
{
    Document* impl = (Document*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

JSValue JSDocument::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DoctypeAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->doctype()));
        }
        case ImplementationAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->implementation()));
        }
        case DocumentElementAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->documentElement()));
        }
        case InputEncodingAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->inputEncoding());
        }
        case XMLEncodingAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->xmlEncoding());
        }
        case XMLVersionAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->xmlVersion());
        }
        case XMLStandaloneAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->xmlStandalone() ? 1 : 0);
        }
        case DocumentURIAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->documentURI());
        }
        case AsyncAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->async() ? 1 : 0);
        }
        case DefaultViewAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->defaultView()));
        }
        case StyleSheetsAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->styleSheets()));
        }
        case TitleAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->title()).utf8().data());
        }
        case ReferrerAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->referrer()).utf8().data());
        }
        case DomainAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->domain()).utf8().data());
        }
        case URLAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->URL()).utf8().data());
        }
        case CookieAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cookie()).utf8().data());
        }
        case BodyAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->body()));
        }
        case ImagesAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->images()));
        }
        case AppletsAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->applets()));
        }
        case LinksAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->links()));
        }
        case FormsAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->forms()));
        }
        case AnchorsAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->anchors()));
        }
        case LocationAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JSDocument::location(ctx, this_val, imp);
        }
        case CharsetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrUndefined(ctx, imp->charset());
        }
        case DefaultCharsetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrUndefined(ctx, imp->defaultCharset());
        }
        case ReadyStateAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrUndefined(ctx, imp->readyState());
        }
        case CharacterSetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->characterSet());
        }
        case PreferredStylesheetSetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->preferredStylesheetSet());
        }
        case SelectedStylesheetSetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setXMLVersion(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case XMLStandaloneAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setXMLStandalone(valueToBoolean(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case DocumentURIAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDocumentURI(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AsyncAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setAsync(valueToBoolean(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case TitleAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTitle(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DomainAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDomain(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CookieAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCookie(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BodyAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setBody(toHTMLElement(value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case LocationAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            JSDocument::setLocation(ctx, this_val, value, imp);
            break;
        }
        case CharsetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SelectedStylesheetSetAttrNum: {
            Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    Document* imp = (Document*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
        case JSDocument::CreateExpressionFuncNum: {
            ExceptionCode ec = 0;
            String expression = valueToString(ctx, argv[0]);
            RefPtr<XPathNSResolver> customResolver;
            XPathNSResolver* resolver = toXPathNSResolver(argv[1]);
            if (!resolver) {
                customResolver = JSCustomXPathNSResolver::create(ctx, argv[1]);
                if (JS_HasException(ctx))
                    return JS_UNDEFINED;
                resolver = customResolver.get();
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->createExpression(expression, resolver, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDocument::CreateNSResolverFuncNum: {
            Node* nodeResolver = toNode(argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createNSResolver(nodeResolver)));
            return result;
        }
        case JSDocument::EvaluateFuncNum: {
            ExceptionCode ec = 0;
            String expression = valueToString(ctx, argv[0]);
            Node* contextNode = toNode(argv[1]);
            RefPtr<XPathNSResolver> customResolver;
            XPathNSResolver* resolver = toXPathNSResolver(argv[2]);
            if (!resolver) {
                customResolver = JSCustomXPathNSResolver::create(ctx, argv[2]);
                if (JS_HasException(ctx))
                    return JS_UNDEFINED;
                resolver = customResolver.get();
            }
            unsigned short type = valueToInt32(ctx, argv[3]);
            XPathResult* inResult = toXPathResult(argv[4]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->evaluate(expression, contextNode, resolver, type, inResult, ec)));
            setDOMException(ctx, ec);
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
