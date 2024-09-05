/*
 * Copyright (C) 2004, 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
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
#include "HTMLCanvasElement.h"

#include "CanvasGradient.h"
#include "CanvasPattern.h"
#include "CanvasRenderingContext2D.h"
#include "CanvasStyle.h"
#include "Chrome.h"
#include "Document.h"
#include "Frame.h"
#include "AffineTransform.h"
#include "GraphicsContext.h"
#include "HTMLNames.h"
#include "Page.h"
#include "RenderHTMLCanvas.h"
#include "Settings.h"
#include <math.h>

namespace WebCore {

using namespace HTMLNames;

// These values come from the WhatWG spec.
static const int defaultWidth = 300;
static const int defaultHeight = 150;

// Firefox limits width/height to 32767 pixels, but slows down dramatically before it 
// reaches that limit. We limit by area instead, giving us larger maximum dimensions,
// in exchange for a smaller maximum canvas size.
static const float maxCanvasArea = 32768 * 8192; // Maximum canvas area in CSS pixels

extern ps_color_format pixelFormat(void);

HTMLCanvasElement::HTMLCanvasElement(Document* doc)
    : HTMLElement(canvasTag, doc)
    , m_size(defaultWidth, defaultHeight)
    , m_createdDrawingContext(false)
#if 1
    , m_pdc(0)
    , m_data(0)
#endif
    , m_drawingContext(0)
{
}

HTMLCanvasElement::~HTMLCanvasElement()
{
    if (m_2DContext)
        m_2DContext->detachCanvas();

#if 1
    if (m_pdc)
        ps_context_unref(m_pdc);
#endif

#if 1
    if (m_data)
        ps_canvas_unref(m_data);
#endif

    delete m_drawingContext;
}

HTMLTagStatus HTMLCanvasElement::endTagRequirement() const 
{
    Settings* settings = document()->settings();
    if (settings && settings->usesDashboardBackwardCompatibilityMode())
        return TagStatusForbidden; 

    return HTMLElement::endTagRequirement();
}

int HTMLCanvasElement::tagPriority() const 
{ 
    Settings* settings = document()->settings();
    if (settings && settings->usesDashboardBackwardCompatibilityMode())
        return 0; 

    return HTMLElement::tagPriority();
}

void HTMLCanvasElement::parseMappedAttribute(MappedAttribute* attr)
{
    const QualifiedName& attrName = attr->name();
    if (attrName == widthAttr || attrName == heightAttr)
        reset();
    HTMLElement::parseMappedAttribute(attr);
}

RenderObject* HTMLCanvasElement::createRenderer(RenderArena* arena, RenderStyle* style)
{
    Settings* settings = document()->settings();
    if (settings && settings->isJavaScriptEnabled()) {
        m_rendererIsCanvas = true;
        return new (arena) RenderHTMLCanvas(this);
    }

    m_rendererIsCanvas = false;
    return HTMLElement::createRenderer(arena, style);
}

void HTMLCanvasElement::setHeight(int value)
{
    setAttribute(heightAttr, String::number(value));
}

void HTMLCanvasElement::setWidth(int value)
{
    setAttribute(widthAttr, String::number(value));
}

CanvasRenderingContext* HTMLCanvasElement::getContext(const String& type)
{
    if (type == "2d") {
        if (!m_2DContext)
            m_2DContext = new CanvasRenderingContext2D(this);
        return m_2DContext.get();
    }
    return 0;
}

void HTMLCanvasElement::willDraw(const FloatRect&)
{
    // FIXME: Change to repaint just the dirty rect for speed.
    // Until we start doing this, we won't know if the rects passed in are
    // accurate. Also don't forget to take into account the transform
    // on the context when determining what needs to be repainted.
    if (renderer())
        renderer()->repaint();
}

void HTMLCanvasElement::reset()
{
    bool ok;
    int w = getAttribute(widthAttr).toInt(&ok);
    if (!ok)
        w = defaultWidth;
    int h = getAttribute(heightAttr).toInt(&ok);
    if (!ok)
        h = defaultHeight;

    IntSize oldSize = m_size;
    m_size = IntSize(w, h);

    bool hadDrawingContext = m_createdDrawingContext;
    m_createdDrawingContext = false;

#if 1
    if (m_pdc) {
        ps_context_unref(m_pdc);
        m_pdc = 0;
    }
#endif

#if 1
    if (m_data) {
        ps_canvas_unref(m_data);
        m_data = 0;
    }
#endif

    delete m_drawingContext;
    m_drawingContext = 0;
    if (m_2DContext)
        m_2DContext->reset();

    if (RenderObject* ro = renderer())
        if (m_rendererIsCanvas) {
            if (oldSize != m_size)
                static_cast<RenderHTMLCanvas*>(ro)->canvasSizeChanged();
            if (hadDrawingContext)
                ro->repaint();
        }
}

void HTMLCanvasElement::paint(GraphicsContext* p, const IntRect& rect)
{
    if (p->paintingDisabled())
        return;
#if 1
    if (m_data) {
        ps_context *gc = p->platformContext();
		ps_rect rc = {rect.x(), rect.y(), rect.width(), rect.height()};
        ps_save(gc);
		ps_set_source_canvas(gc, m_data);
		ps_rectangle(gc, &rc);
		ps_fill(gc);
		ps_restore(gc);
    }
#endif
}

void HTMLCanvasElement::createImageData() const
{
	m_data = ps_canvas_create(pixelFormat(), m_size.width(), m_size.height());
	ps_context * tg = ps_context_create(m_data, NULL);
	ps_color c = {1,1,1,1};
	ps_set_source_color(tg, &c);
	ps_clear(tg); //clear canvas with white color
	ps_context_unref(tg);
}

void HTMLCanvasElement::createDrawingContext() const
{
    m_createdDrawingContext = true;

    float unscaledWidth = width();
    float unscaledHeight = height();
    float pageScaleFactor = document()->frame() ? document()->frame()->page()->chrome()->scaleFactor() : 1.0f;
    float wf = ceilf(unscaledWidth * pageScaleFactor);
    float hf = ceilf(unscaledHeight * pageScaleFactor);

    if (!(wf >= 1 && hf >= 1 && wf * hf <= maxCanvasArea))
        return;

    unsigned w = static_cast<unsigned>(wf);
    unsigned h = static_cast<unsigned>(hf);

    size_t bytesPerRow = w * 4; //4 is not byte pre pixel, is alignment factor
    if (bytesPerRow / 4 != w) // check for overflow
        return;
#if 1
    if (!m_data)
		createImageData();

    m_pdc = ps_context_create(m_data, NULL);
#endif
    m_drawingContext = new GraphicsContext(m_pdc);
    m_drawingContext->scale(FloatSize(w / unscaledWidth, h / unscaledHeight));
}

GraphicsContext* HTMLCanvasElement::drawingContext() const
{
    if (!m_createdDrawingContext)
        createDrawingContext();
    return m_drawingContext;
}

#if 1
ps_canvas* HTMLCanvasElement::createPlatformImage() const
{
    if (!m_data)
        createImageData();
    return m_data;
}
#endif

}
