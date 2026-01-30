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

#include "test.h"
#include "picasso/picasso.h"

#include "test_client.h"

class WebViewPrivate
{
public:
    WebViewPrivate(int width, int height)
        : m_width(width), m_height(height), m_drawRect(0, 0, 0, 0)
        , m_drawTimer(this, &WebViewPrivate::drawFired)
    {
        int32_t pitch = width * 4;
        m_buffer = new uint8_t[height * pitch];

        m_canvas = ps_canvas_create_with_data(m_buffer, COLOR_FORMAT_RGBA, width, height, pitch);
        m_context = ps_context_create(m_canvas, 0);
    }

    ~WebViewPrivate()
    {
        ps_context_unref(m_context);
        ps_canvas_unref(m_canvas);
        delete [] m_buffer;
    }

    void drawFired(Timer<WebViewPrivate>* timer)
    {
        m_frameView->layoutIfNeededRecursive();
        //last draw update rect
        if (!m_drawRect.isEmpty()) {
            GraphicsContext gc(m_context);
            gc.save();
            gc.clip(m_drawRect);
            m_frameView->paint(&gc, m_drawRect);
            gc.restore();
            m_drawRect = IntRect(0, 0, 0, 0); //clear the invalidate Rect
        }
    }

    uint8_t* m_buffer;
    int32_t m_width;
    int32_t m_height;
    ps_canvas* m_canvas;
    ps_context* m_context;
    IntRect m_drawRect;
    RefPtr<Frame> m_frame;
    RefPtr<FrameView> m_frameView;
    Timer<WebViewPrivate> m_drawTimer;
};

TestWebView::TestWebView() : d(new WebViewPrivate(WIDTH, HEIGHT))
{
    Settings::init();
    Page* page = new Page(new ChromeClientTest(WIDTH, HEIGHT), new ContextMenuClientTest(),
                          new EditorClientTest(), new DragClientTest());

    FrameLoaderClientTest* client = new FrameLoaderClientTest();
    Frame* frame = new Frame(page, 0, client);
    client->setFrame(frame);
    d->m_frame = frame;

    FrameView* frameView = new FrameView(frame);
    frameView->resize(WIDTH, HEIGHT);
    d->m_frameView = frameView;
    d->m_frame->setView(frameView);
    frameView->deref();

    d->m_frame->init();
    d->m_frame->page()->setGroupName(String("PageGroup"));

}

TestWebView::~TestWebView()
{
    stop();
    Page* page = d->m_frame->page();
    Frame* frame = page->mainFrame();
    if (frame) {
        frame->loader()->detachFromParent();
    }
    delete page;
}

Frame* TestWebView::mainframe(void) const
{
    return d->m_frame.get();
}

void TestWebView::loadHtml(const char* str, const char* rurl)
{
    stop();
    String urlstr = String::fromUTF8(rurl);
    KURL baseurl(urlstr.deprecatedString());
    ResourceRequest request(baseurl);
    RefPtr<WebCore::SharedBuffer> data = new SharedBuffer(str, strlen(str));
    SubstituteData substituteData(data, String("text/html"), String("utf-8"), KURL());
    d->m_frame->loader()->load(request, substituteData);
}

void TestWebView::stop(void)
{
    d->m_frame->loader()->stopForUserCancel();
}

void TestWebView::updateRect(int x, int y, int width, int height)
{
    d->m_drawRect.unite(IntRect(x, y, width, height));
    // notify draw
    d->m_drawTimer.startOneShot(0);
}

void TestWebView::scrollRect(int sx, int sy, int x, int y, int width, int height)
{
    updateRect(x, y, width, height);
}

void Test_Init()
{
    ASSERT_EQ(MC_STATUS_SUCCESS, macross_initialize(PIXEL_FORMAT_RGBA32, WIDTH, HEIGHT));
}

void Test_Shutdown()
{
    macross_shutdown();
}

void Test_EventDispatchOnce()
{
    macross_event_dispatch();
}
