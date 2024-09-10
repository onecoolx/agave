/* RenderThread.cpp - MaCross application
 *
 * Vector Graphic Component button.
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "renderthread.h"

#if THREAD_RENDER
#include <vgcl.h>
#include "macross.h"

using namespace VGCL;

static void create_view(WebView* view, const WebEventParam* param)
{
	MC_SIZE lsize = {param->desc.width, param->desc.height};
	MaCrossView* mv = macross_view_create(param->desc.buffer,
                      param->desc.buffer_width, param->desc.buffer_height, param->desc.buffer_pitch, view);

	macross_view_set_minimum_layout_size(mv, &lsize);
	macross_view_set_scale_factor(mv, (int)(100* param->desc.default_scale));
    view->setPlatformView(mv);
}

static void destroy_view(WebView* view, const WebEventParam* param)
{
	macross_view_destroy(param->cmd.view);
}

static void load_url(WebView* view, const WebEventParam* param)
{
    if (view->getPlatformView())
        macross_view_open_url(view->getPlatformView(), param->cmd.url.c_str());
}

static void paint_view(WebView* view, const WebEventParam* param)
{
    if (view->getPlatformView())
		macross_view_update(view->getPlatformView(), 0);
}

// render thread

RenderThread::RenderThread()
{
}

RenderThread::~RenderThread()
{
}

void RenderThread::Run(void)
{
    WebEvent e;
    if (!m_events.empty()) {
        VMutexLocker<VMutex> locker(m_mutex);
        e = m_events.front();
		m_events.pop_front();
    }
    // dispatch event.

    switch (e.getType()) {
        case WebEvent::CreateView:
           create_view(e.getView(), e.getParam());
           break; 
        case WebEvent::DestroyView:
           destroy_view(e.getView(), e.getParam());
           removeAllEventsByView(e.getView());
           break;
        case WebEvent::LoadUrl:
           load_url(e.getView(), e.getParam());
           break;
        case WebEvent::PaintView:
           paint_view(e.getView(), e.getParam());
           break;
        default:
            // unknown event do nothing.
           break;
    }

	macross_event_dispatch();
    Sleep(10);
}

void RenderThread::sendEvent(const WebEvent& e)
{
    VMutexLocker<VMutex> locker(m_mutex);
    
	if (e.getLevel() == WebEvent::HighLevel)
		m_events.push_front(e);
	else
		m_events.push_back(e);
}

void RenderThread::removeAllEventsByView(const WebView* view)
{
    VMutexLocker<VMutex> locker(m_mutex);
    std::deque<WebEvent>::iterator itr;
    for(itr = m_events.begin(); itr != m_events.end();){
        if (itr->getView() == view)
            itr = m_events.erase(itr);
        else
            itr++;
    }
}

#endif

