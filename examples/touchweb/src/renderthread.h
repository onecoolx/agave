/* RenderThread.h - Agave application
 *
 * Vector Graphic Component button.
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _RENDER_THREAD_H_
#define _RENDER_THREAD_H_

#include "config.h"
#if THREAD_RENDER

#include <deque>
#include <vgcl.h>

#include "webview.h"

struct WebEventParam
{
    struct ViewDesc 
    {
       ViewDesc(): width(0),height(0),buffer(0)
           , buffer_width(0), buffer_height(0), buffer_pitch(0)
           , default_scale(1.0) {}

       int width;
       int height;
       unsigned char* buffer; 
       int buffer_width;
       int buffer_height;
       int buffer_pitch;
       float default_scale;
    }desc;

    struct ViewCmd
    {
        ViewCmd(): view(0) {}
        MaCrossView* view;
        std::string url;
        Rect dirty;
    }cmd;
};

class WebEvent : public vgcl::VEvent
{
public:
    typedef enum {
        DefaultLevel,
        HighLevel,
    } EventLevel;

    typedef enum {
        Unknown     = 0,
        CreateView  = 1,
        DestroyView = 2,
        LoadUrl     = 3,
        PaintView   = 4,
    }EventType;

    WebEvent()
        : m_source(0)
        , m_type(Unknown)
        , m_level(DefaultLevel)
    {
    }

    WebEvent(WebView* view, EventType type, EventLevel level, const WebEventParam& param)
        : m_source(view)
        , m_type(type)
        , m_param(param)
        , m_level(level)
    {
    }

	WebEvent(const WebEvent& o)
	{
		m_source = o.m_source;
        m_type = o.m_type;
		m_param = o.m_param;
        m_level = o.m_level;
	}

	WebEvent& operator=(const WebEvent& o)
	{
		if (this == &o)
			return *this;

		m_source = o.m_source;
        m_type = o.m_type;
		m_param = o.m_param;
        m_level = o.m_level;

		return *this;
	}

    const WebEventParam* getParam(void) const { return &m_param; }
    WebView* getView(void) const { return m_source; }
    EventLevel getLevel(void) const { return m_level; }
    EventType getType(void) const { return m_type; }
private:
    WebView* m_source;
    EventType m_type;
    WebEventParam m_param;
    EventLevel m_level;
};


class RenderThread : public vgcl::VThread
{
public:
    RenderThread();
    virtual ~RenderThread();

    virtual void Run(void);

    void sendEvent(const WebEvent& e);

    void removeAllEventsByView(const WebView* view);

private:
    std::deque<WebEvent> m_events;
    vgcl::VMutex m_mutex;
};

#endif

#endif /*_RENDER_THREAD_H_*/

