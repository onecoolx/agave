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

#include "FontCache.h"
#include "WidgetView.h"
#include "NodeList.h"
#include "Document.h"
#include "HTMLNames.h"
#include "HTMLInputElement.h"
#include "HTMLSelectElement.h"
#include "HTMLTextAreaElement.h"
#include "HTMLFormElement.h"
#include "CSSStyleDeclaration.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSComputedStyleDeclaration.h"
#include "CSSPropertyNames.h"
#include "CSSStyleSelector.h"
#include "CSSValueKeywords.h"
#include "ExceptionCode.h"
#include "Page.h"
#include "Frame.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "FrameLoader.h"
#include "DocumentLoader.h"
#include "DocumentFragment.h"
#include "Editor.h"
#include "EventHandler.h"
#include "GraphicsContext.h"
#include "SegmentedString.h"
#include "Settings.h"
#include "Text.h"
#include "markup.h"

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

#endif
