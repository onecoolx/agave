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

#ifndef _JSDOMWindow_H_
#define _JSDOMWindow_H_

#include "qjs_window.h"


namespace WebCore {

class DOMWindow;

class JSDOMWindow {
public:
    static void init(JSContext*);
    static JSValue create(JSContext*, JSValue, DOMWindow*);
    static void finalizer(JSRuntime *rt, JSValue val);

    static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
    static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);

    static JSClassID js_class_id;

    static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

    enum {
        // Attributes
        ScreenAttrNum, HistoryAttrNum, LocationbarAttrNum, MenubarAttrNum, 
        PersonalbarAttrNum, ScrollbarsAttrNum, StatusbarAttrNum, ToolbarAttrNum, 
        FrameElementAttrNum, OffscreenBufferingAttrNum, OuterHeightAttrNum, OuterWidthAttrNum, 
        InnerHeightAttrNum, InnerWidthAttrNum, ScreenXAttrNum, ScreenYAttrNum, 
        ScreenLeftAttrNum, ScreenTopAttrNum, ScrollXAttrNum, ScrollYAttrNum, 
        PageXOffsetAttrNum, PageYOffsetAttrNum, ClosedAttrNum, LengthAttrNum, 
        NameAttrNum, StatusAttrNum, DefaultStatusAttrNum, DefaultstatusAttrNum, 
        SelfAttrNum, WindowAttrNum, FramesAttrNum, OpenerAttrNum, 
        ParentAttrNum, TopAttrNum, DocumentAttrNum, DevicePixelRatioAttrNum, 
        StyleSheetConstructorAttrNum, CSSStyleSheetConstructorAttrNum, CSSValueConstructorAttrNum, CSSPrimitiveValueConstructorAttrNum, 
        CSSValueListConstructorAttrNum, CSSRuleConstructorAttrNum, CSSCharsetRuleConstructorAttrNum, CSSFontFaceRuleConstructorAttrNum, 
        CSSImportRuleConstructorAttrNum, CSSMediaRuleConstructorAttrNum, CSSPageRuleConstructorAttrNum, CSSStyleRuleConstructorAttrNum, 
        CSSStyleDeclarationConstructorAttrNum, MediaListConstructorAttrNum, CounterConstructorAttrNum, CSSRuleListConstructorAttrNum, 
        RectConstructorAttrNum, StyleSheetListConstructorAttrNum, DOMImplementationConstructorAttrNum, DocumentFragmentConstructorAttrNum, 
        DocumentConstructorAttrNum, NodeConstructorAttrNum, NodeListConstructorAttrNum, NamedNodeMapConstructorAttrNum, 
        CharacterDataConstructorAttrNum, AttrConstructorAttrNum, ElementConstructorAttrNum, TextConstructorAttrNum, 
        CommentConstructorAttrNum, CDATASectionConstructorAttrNum, DocumentTypeConstructorAttrNum, NotationConstructorAttrNum, 
        EntityConstructorAttrNum, EntityReferenceConstructorAttrNum, ProcessingInstructionConstructorAttrNum, HTMLDocumentConstructorAttrNum, 
        HTMLElementConstructorAttrNum, HTMLAnchorElementConstructorAttrNum, HTMLAppletElementConstructorAttrNum, HTMLAreaElementConstructorAttrNum, 
        HTMLBRElementConstructorAttrNum, HTMLBaseElementConstructorAttrNum, HTMLBaseFontElementConstructorAttrNum, HTMLBlockquoteElementConstructorAttrNum, 
        HTMLBodyElementConstructorAttrNum, HTMLButtonElementConstructorAttrNum, HTMLCanvasElementConstructorAttrNum, HTMLDListElementConstructorAttrNum, 
        HTMLDirectoryElementConstructorAttrNum, HTMLDivElementConstructorAttrNum, HTMLEmbedElementConstructorAttrNum, HTMLFieldSetElementConstructorAttrNum, 
        HTMLFontElementConstructorAttrNum, HTMLFormElementConstructorAttrNum, HTMLFrameElementConstructorAttrNum, HTMLFrameSetElementConstructorAttrNum, 
        HTMLHRElementConstructorAttrNum, HTMLHeadElementConstructorAttrNum, HTMLHeadingElementConstructorAttrNum, HTMLHtmlElementConstructorAttrNum, 
        HTMLIFrameElementConstructorAttrNum, HTMLImageElementConstructorAttrNum, HTMLInputElementConstructorAttrNum, HTMLIsIndexElementConstructorAttrNum, 
        HTMLLIElementConstructorAttrNum, HTMLLabelElementConstructorAttrNum, HTMLLegendElementConstructorAttrNum, HTMLLinkElementConstructorAttrNum, 
        HTMLMapElementConstructorAttrNum, HTMLMarqueeElementConstructorAttrNum, HTMLMenuElementConstructorAttrNum, HTMLMetaElementConstructorAttrNum, 
        HTMLModElementConstructorAttrNum, HTMLOListElementConstructorAttrNum, HTMLObjectElementConstructorAttrNum, HTMLOptGroupElementConstructorAttrNum, 
        HTMLOptionElementConstructorAttrNum, HTMLParagraphElementConstructorAttrNum, HTMLParamElementConstructorAttrNum, HTMLPreElementConstructorAttrNum, 
        HTMLQuoteElementConstructorAttrNum, HTMLScriptElementConstructorAttrNum, HTMLSelectElementConstructorAttrNum, HTMLStyleElementConstructorAttrNum, 
        HTMLTableCaptionElementConstructorAttrNum, HTMLTableCellElementConstructorAttrNum, HTMLTableColElementConstructorAttrNum, HTMLTableElementConstructorAttrNum, 
        HTMLTableRowElementConstructorAttrNum, HTMLTableSectionElementConstructorAttrNum, HTMLTextAreaElementConstructorAttrNum, HTMLTitleElementConstructorAttrNum, 
        HTMLUListElementConstructorAttrNum, EventConstructorAttrNum, MutationEventConstructorAttrNum, NodeFilterConstructorAttrNum, 
        RangeConstructorAttrNum, RangeExceptionConstructorAttrNum, XMLDocumentConstructorAttrNum, DOMParserConstructorAttrNum, 
        XMLSerializerConstructorAttrNum, 

        // Functions
        GetSelectionFuncNum, FocusFuncNum, BlurFuncNum, CloseFuncNum, 
        PrintFuncNum, StopFuncNum, AlertFuncNum, ConfirmFuncNum, 
        PromptFuncNum, FindFuncNum, GetComputedStyleFuncNum, GetMatchedCSSRulesFuncNum
    };
};

DOMWindow* toDOMWindow(JSValue);

class JSDOMWindowPrototype {
public:
    static JSValue self(JSContext * ctx);
    static void initPrototype(JSContext * ctx, JSValue this_obj);
};

class JSDOMWindowPrototypeFunction {
public:
    static JSValue callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token);
};

} // namespace WebCore

#endif
