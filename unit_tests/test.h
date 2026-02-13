/*
 * Agave - A lightweight web browser engine
 *
 * Copyright (c) 2026, Zhang Ji Peng
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

#ifndef _AGAVE_TEST_H_
#define _AGAVE_TEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Common WTF headers
#include <wtf/Vector.h>
#include <wtf/Deque.h>
#include <wtf/RefPtr.h>

// Font related headers
#include "FontCache.h"
#include "Font.h"
#include "FontData.h"
#include "FontDescription.h"
#include "FontFamily.h"
#include "GlyphPageTreeNode.h"
#include "TextStyle.h"

// DOM headers
#include "NodeList.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "DocumentFragment.h"
#include "Text.h"

// HTML headers
#include "HTMLNames.h"
#include "HTMLInputElement.h"
#include "HTMLSelectElement.h"
#include "HTMLTextAreaElement.h"
#include "HTMLFormElement.h"
#include "HTMLTokenizer.h"
#include "HTMLDocument.h"
#include "HTMLParser.h"

// CSS headers
#include "CSSStyleDeclaration.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSComputedStyleDeclaration.h"
#include "CSSPropertyNames.h"
#include "CSSStyleSelector.h"
#include "CSSValueKeywords.h"
#include "CSSRuleList.h"
#include "CSSStyleRule.h"
#include "StyleSheetList.h"
#include "CSSStyleSheet.h"

// Loader headers
#include "loader.h"
#include "Request.h"
#include "DocLoader.h"
#include "CachedResource.h"

// Page and Frame headers
#include "Page.h"
#include "Frame.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "FrameLoader.h"

// Editor headers
#include "Editor.h"
#include "EditCommand.h"

// Other headers
#include "ExceptionCode.h"
#include "EventHandler.h"
#include "GraphicsContext.h"
#include "SegmentedString.h"
#include "Settings.h"
#include "markup.h"
#include "WidgetView.h"

#include "macross.h"

#define WIDTH 800
#define HEIGHT 600

using namespace WebCore;
using namespace WTF;

class WebViewPrivate;

class TestWebView : public WidgetView
{
public:
    TestWebView();
    ~TestWebView();

    void updateRect(int x, int y, int width, int height);
    void scrollRect(int sx, int sy, int x, int y, int width, int height);

    Frame* mainframe(void) const;
    void loadHtml(const char* str, const char* rurl);
    void stop(void);

    static void waitForDocumentComplete(TestWebView* view);
private:
    OwnPtr<WebViewPrivate> d;
};

void Test_Init();
void Test_Shutdown();
void Test_EventDispatchOnce();

#define DELETE_ARRAY_VALUES(arr, n) \
    do {\
        for (int i = 0; i < (n); i++) { \
            delete (arr)[i]; \
        } \
    } while(0)

#endif
