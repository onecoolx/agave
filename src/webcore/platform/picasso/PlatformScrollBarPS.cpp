/*
** PlatformScrollBarPS.cpp: PlatformScrollBar implements by picasso.
**
** Copyright(C) 2009 Zhang Ji Peng (onecoolx@gmail.com)
**
*/

#include "config.h"
#include "PlatformScrollBar.h"
#include "EventHandler.h"
#include "FrameView.h"
#include "Frame.h"
#include "Settings.h"
#include "AffineTransform.h"
#include "GraphicsContext.h"
#include "PlatformMouseEvent.h"
#include "Debug.h"

#include <picasso/picasso.h>

namespace WebCore {
using namespace std;

enum SCROLLPART{
    ScrollLeftArrowPart,
    ScrollRightArrowPart,
    ScrollUpArrowPart,
    ScrollDownArrowPart,
    HScrollTrackPart,
    VScrollTrackPart,
    HScrollThumbPart,
    VScrollThumbPart
};

enum PARTSTATE {
    LEAVE,
    PRESS,
    HOVER
};

static void paintScrollThumb (SCROLLPART part, ps_context* gc, const IntRect &rect, bool enable, PARTSTATE state)
{
	ps_point sp = {0 , 0};
	ps_point ep = {0 , 0};
	ps_color f = {0, 0, 0, 0};
	ps_color s = {0, 0, 0, 0};
	ps_color d = {0, 0, 0, 0};
	ps_color e = {1.00, 1.00, 1.00, 1.00};
	if (enable) {
 		s.r = 0.25; s.g = 0.75; s.b = 0.75; s.a = 1.00;
 		d.r = 0.75; d.g = 1.00; d.b = 1.00; d.a = 1.00;
 		f.r = 0.90; f.g = 1.00; f.b = 1.00; f.a = 1.00;
	} else {
 		s.r = 0.50; s.g = 0.50; s.b = 0.50; s.a = 1.00;
 		d.r = 0.65; d.g = 0.65; d.b = 0.65; d.a = 1.00;
 		f.r = 0.80; f.g = 0.80; f.b = 0.80; f.a = 1.00;
	}
	ps_rect r = {rect.x(), rect.y(), rect.width()-1, rect.height()-1};
	if (part == VScrollThumbPart) {
		sp.x = r.x; sp.y = r.y;
		ep.x = r.x+r.w/2; ep.y = r.y;
	} else {
		sp.x = r.x; sp.y = r.y;
		ep.x = r.x; ep.y = r.y+r.h/2;
	}
	ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_REFLECT, &sp, &ep);
	ps_gradient_add_color_stop(g, 0.3, &e);
	ps_gradient_add_color_stop(g, 0.8, &f);
	ps_gradient_add_color_stop(g, 1.0, &d);
	ps_set_stroke_color(gc, &s);
	ps_set_source_gradient(gc, g);
	ps_rectangle(gc, &r);
	ps_paint(gc);
	ps_gradient_unref(g);
}

static void paintScrollTrack (SCROLLPART part, ps_context* gc, const IntRect &rect, bool enable, PARTSTATE state)
{
	ps_color f = {0, 0, 0, 0};
	if (enable) {
 		f.r = 0.90; f.g = 1.00; f.b = 1.00; f.a = 1.00;
	} else {
 		f.r = 0.80; f.g = 0.80; f.b = 0.80; f.a = 1.00;
	}
	ps_rect r = {rect.x(), rect.y(), rect.width(), rect.height()};
	ps_rectangle(gc, &r);
	ps_set_source_color(gc, &f);
	ps_fill(gc);
}

static void paintScrollArrows (SCROLLPART part, ps_context* gc, const IntRect &rect, bool enable, PARTSTATE state)
{
	ps_point sp = {0 , 0};
	ps_point ep = {0 , 0};
	ps_color f = {0, 0, 0, 0};
	ps_color s = {0, 0, 0, 0};
	ps_color d = {0, 0, 0, 0};
	ps_color e = {0, 0, 0, 0};
	if (enable) {
		if (PRESS == state) {
				s.r = 0.25; s.g = 0.75; s.b = 0.75; s.a = 1.00;
				d.r = 0.65; d.g = 0.90; d.b = 0.90; d.a = 1.00;
				f.r = 0.75; f.g = 0.90; f.b = 0.90; f.a = 1.00;
				e.r = 0.85; e.g = 0.90; e.b = 0.90; e.a = 1.00;
			} else {
				s.r = 0.25; s.g = 0.75; s.b = 0.75; s.a = 1.00;
				d.r = 0.75; d.g = 1.00; d.b = 1.00; d.a = 1.00;
				f.r = 0.90; f.g = 1.00; f.b = 1.00; f.a = 1.00;
				e.r = 1.00; e.g = 1.00; e.b = 1.00; e.a = 1.00;
		}
	} else {
 		s.r = 0.50; s.g = 0.50; s.b = 0.50; s.a = 1.00;
 		d.r = 0.65; d.g = 0.65; d.b = 0.65; d.a = 1.00;
 		f.r = 0.80; f.g = 0.80; f.b = 0.80; f.a = 1.00;
		e.r = 0.95; e.g = 0.95; e.b = 0.95; e.a = 1.00;
	}
	ps_rect r = {rect.x(), rect.y(), rect.width()-1, rect.height()-1};
	if (part == ScrollLeftArrowPart || part == ScrollRightArrowPart) {
		sp.x = r.x; sp.y = r.y;
		ep.x = r.x; ep.y = r.y+r.h/2;
	} else {
		sp.x = r.x; sp.y = r.y;
		ep.x = r.x+r.w/2; ep.y = r.y;
	}
	ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_REFLECT, &sp, &ep);
	ps_gradient_add_color_stop(g, 0.3, &e);
	ps_gradient_add_color_stop(g, 0.8, &f);
	ps_gradient_add_color_stop(g, 1.0, &d);
	ps_set_stroke_color(gc, &s);
	ps_set_source_gradient(gc, g);
    switch (part)
    {
        case ScrollLeftArrowPart:
            {
				ps_rounded_rect(gc, &r, 2, 2, 0, 0, 2, 2, 0, 0); 
				sp.x = r.x + r.w/2 + 1.5; sp.y = r.y + 3;
				ps_move_to(gc, &sp);
				sp.x = r.x + r.w/2 - 1.5; sp.y = r.y + r.h/2;
				ps_line_to(gc, &sp);
				sp.x = r.x + r.w/2 + 1.5; sp.y = r.y + r.h - 3;
				ps_line_to(gc, &sp);
            }
            break;
        case ScrollRightArrowPart:
            {
				ps_rounded_rect(gc, &r, 0, 0, 2, 2, 0, 0, 2, 2); 
				sp.x = r.x + r.w/2 - 1.5; sp.y = r.y + 3;
				ps_move_to(gc, &sp);
				sp.x = r.x + r.w/2 + 1.5; sp.y = r.y + r.h/2;
				ps_line_to(gc, &sp);
				sp.x = r.x + r.w/2 - 1.5; sp.y = r.y + r.h - 3;
				ps_line_to(gc, &sp);
            }
            break;
        case ScrollUpArrowPart:
            {
				ps_rounded_rect(gc, &r, 2, 2, 2, 2, 0, 0, 0, 0); 
				sp.y = r.y + r.h/2 + 1.5; sp.x = r.x + 3;
				ps_move_to(gc, &sp);
				sp.y = r.y + r.h/2 - 1.5; sp.x = r.x + r.w/2;
				ps_line_to(gc, &sp);
				sp.y = r.y + r.h/2 + 1.5; sp.x = r.x + r.w - 3;
				ps_line_to(gc, &sp);
            }
            break;
        case ScrollDownArrowPart:
            {
				ps_rounded_rect(gc, &r, 0, 0, 0, 0, 2, 2, 2, 2); 
				sp.y = r.y + r.h/2 - 1.5; sp.x = r.x + 3;
				ps_move_to(gc, &sp);
				sp.y = r.y + r.h/2 + 1.5; sp.x = r.x + r.w/2;
				ps_line_to(gc, &sp);
				sp.y = r.y + r.h/2 - 1.5; sp.x = r.x + r.w - 3;
				ps_line_to(gc, &sp);
            }
            break;
        default:
            break;
    }

	ps_paint(gc);
	ps_gradient_unref(g);
}

static void paintScrollPart (SCROLLPART part, ps_context *gc, const IntRect &rect, bool enable, PARTSTATE state)
{
    switch (part)
    {
    case ScrollLeftArrowPart:
    case ScrollRightArrowPart:
    case ScrollUpArrowPart:
    case ScrollDownArrowPart:
        paintScrollArrows (part, gc, rect, enable, state);
        break;
    case HScrollTrackPart:
    case VScrollTrackPart:
        paintScrollTrack (part, gc, rect, enable, state);
        break;
    case HScrollThumbPart:
    case VScrollThumbPart:
        paintScrollThumb (part, gc, rect, enable, state);
        break;
    }
}

static int cHorizontalWidth = 0;
static int cHorizontalHeight = 0;
static int cVerticalWidth = 0;
static int cVerticalHeight = 0;
static int cRealButtonLength = 0;
static int cButtonLength = 0;
static int cThumbWidth = 0;
static int cThumbHeight = 0;
static int cThumbMinLength = 0;

const double cInitialTimerDelay = 0.25;
const double cNormalTimerDelay = 0.05;

Color PlatformScrollbar::g_cornerColor(229, 255, 255);

PlatformScrollbar::PlatformScrollbar(ScrollbarClient* client, ScrollbarOrientation orientation, ScrollbarControlSize size)
    : Scrollbar(client, orientation, size), m_hoveredPart(NoPart), m_pressedPart(NoPart), m_pressedPos(0),
      m_scrollTimer(this, &PlatformScrollbar::autoscrollTimerFired),
      m_overlapsResizer(false)
{
    cHorizontalWidth = Settings::globalSettings()->scrollBarWidth();
    cHorizontalHeight = Settings::globalSettings()->scrollBarWidth();
    cVerticalWidth = Settings::globalSettings()->scrollBarWidth();
    cVerticalHeight = Settings::globalSettings()->scrollBarWidth();
    cRealButtonLength = Settings::globalSettings()->scrollBarWidth();
    cButtonLength = Settings::globalSettings()->scrollBarWidth();
    cThumbWidth = Settings::globalSettings()->scrollBarWidth();
    cThumbHeight = Settings::globalSettings()->scrollBarWidth();
    cThumbMinLength = cThumbHeight/2;
    if (orientation == VerticalScrollbar)
        setFrameGeometry(IntRect(0, 0, cVerticalWidth, cVerticalHeight));
    else
        setFrameGeometry(IntRect(0, 0, cHorizontalWidth, cHorizontalHeight));
}

PlatformScrollbar::~PlatformScrollbar()
{
    stopTimerIfNeeded();
}

void PlatformScrollbar::updateThumbPosition()
{
    invalidateTrack();
}

void PlatformScrollbar::updateThumbProportion()
{
    invalidateTrack();
}

static IntRect trackRepaintRect(const IntRect& trackRect, ScrollbarOrientation orientation, ScrollbarControlSize controlSize)
{
    IntRect paintRect(trackRect);
    if (orientation == HorizontalScrollbar)
        paintRect.inflateX(cButtonLength);
    else
        paintRect.inflateY(cButtonLength);

    return paintRect;
}

static IntRect buttonRepaintRect(const IntRect& buttonRect, ScrollbarOrientation orientation, ScrollbarControlSize controlSize, bool start)
{
    IntRect paintRect(buttonRect);
    if (orientation == HorizontalScrollbar) {
        paintRect.setWidth(cRealButtonLength);
        if (!start)
            paintRect.setX(buttonRect.x() - (cRealButtonLength - buttonRect.width()));
    } else {
        paintRect.setHeight(cRealButtonLength);
        if (!start)
            paintRect.setY(buttonRect.y() - (cRealButtonLength - buttonRect.height()));
    }

    return paintRect;
}

void PlatformScrollbar::invalidateTrack()
{
    IntRect rect = trackRepaintRect(trackRect(), m_orientation, controlSize());
    rect.move(-x(), -y());
    invalidateRect(rect);
}

void PlatformScrollbar::invalidatePart(ScrollbarPart part)
{
    if (part == NoPart)
        return;

    IntRect result;    
    switch (part) {
        case BackButtonPart:
            result = buttonRepaintRect(backButtonRect(), m_orientation, controlSize(), true);
            break;
        case ForwardButtonPart:
            result = buttonRepaintRect(forwardButtonRect(), m_orientation, controlSize(), false);
            break;
        default: {
            IntRect beforeThumbRect, thumbRect, afterThumbRect;
            splitTrack(trackRect(), beforeThumbRect, thumbRect, afterThumbRect);
            if (part == BackTrackPart)
                result = beforeThumbRect;
            else if (part == ForwardTrackPart)
                result = afterThumbRect;
            else
                result = thumbRect;
        }
    }
    result.move(-x(), -y());
    invalidateRect(result);
}

int PlatformScrollbar::width() const
{
    return Widget::width();
}

int PlatformScrollbar::height() const
{
    return Widget::height();
}

void PlatformScrollbar::setRect(const IntRect& rect)
{
    // Get our window resizer rect and see if we overlap.  Adjust to avoid the overlap
    // if necessary.
    IntRect adjustedRect(rect);
    if (parent() && parent()->isFrameView()) {
        bool overlapsResizer = false;
        FrameView* view = static_cast<FrameView*>(parent());
        IntRect resizerRect = view->windowResizerRect();
        resizerRect.setLocation(view->convertFromContainingWindow(resizerRect.location()));
        if (rect.intersects(resizerRect)) {
            if (orientation() == HorizontalScrollbar) {
                int overlap = rect.right() - resizerRect.x();
                if (overlap > 0 && resizerRect.right() >= rect.right()) {
                    adjustedRect.setWidth(rect.width() - overlap);
                    overlapsResizer = true;
                }
            } else {
                int overlap = rect.bottom() - resizerRect.y();
                if (overlap > 0 && resizerRect.bottom() >= rect.bottom()) {
                    adjustedRect.setHeight(rect.height() - overlap);
                    overlapsResizer = true;
                }
            }
        }

        if (overlapsResizer != m_overlapsResizer) {
            m_overlapsResizer = overlapsResizer;
            view->adjustOverlappingScrollbarCount(m_overlapsResizer ? 1 : -1);
        }
    }

    setFrameGeometry(adjustedRect);
}

void PlatformScrollbar::setParent(ScrollView* parentView)
{
    if (!parentView && m_overlapsResizer && parent() && parent()->isFrameView())
        static_cast<FrameView*>(parent())->adjustOverlappingScrollbarCount(-1);
    Widget::setParent(parentView);
}

void PlatformScrollbar::setEnabled(bool enabled)
{
    if (enabled != isEnabled()) {
        Widget::setEnabled(enabled);
        invalidate();
    }
}

void PlatformScrollbar::paint(GraphicsContext* graphicsContext, const IntRect& damageRect)
{
    if (graphicsContext->paintingDisabled())
        return;

    // Don't paint anything if the scrollbar doesn't intersect the damage rect.
    if (!frameGeometry().intersects(damageRect))
        return;

    IntRect track = trackRect();
    paintTrack(graphicsContext, track, true, damageRect);

	paintButton(graphicsContext, backButtonRect(), true, damageRect);
	paintButton(graphicsContext, forwardButtonRect(), false, damageRect);

    if (damageRect.intersects(track) && isEnabled()) {
        IntRect startTrackRect, thumbRect, endTrackRect;
        splitTrack(track, startTrackRect, thumbRect, endTrackRect);
        paintThumb(graphicsContext, thumbRect, damageRect);
    }
}

IntRect PlatformScrollbar::backButtonRect() const
{
    // Our desired rect is essentially 17x17.
    
    // Our actual rect will shrink to half the available space when
    // we have < 34 pixels left.  This allows the scrollbar
    // to scale down and function even at tiny sizes.
    if (m_orientation == HorizontalScrollbar)
        return IntRect(x(), y(), cButtonLength, cHorizontalHeight);
    return IntRect(x(), y(), cVerticalWidth, cButtonLength);
}

IntRect PlatformScrollbar::forwardButtonRect() const
{
    // Our desired rect is essentially 17x17.
    
    // Our actual rect will shrink to half the available space when
    // we have < 34 pixels left.  This allows the scrollbar
    // to scale down and function even at tiny sizes.

    if (m_orientation == HorizontalScrollbar)
        return IntRect(x() + width() - cButtonLength, y(), cButtonLength, cHorizontalHeight);
    
    return IntRect(x(), y() + height() - cButtonLength, cVerticalWidth, cButtonLength);
}

IntRect PlatformScrollbar::trackRect() const
{
    if (m_orientation == HorizontalScrollbar) {
        if (width() < 2 * cHorizontalWidth)
            return IntRect();
        return IntRect(x() + cButtonLength, y(), width() - 2 * cButtonLength, cHorizontalHeight);
    }

    if (height() < 2 * cVerticalHeight)
        return IntRect();
    return IntRect(x(), y() + cButtonLength, cVerticalWidth, height() - 2 * cButtonLength);
}

IntRect PlatformScrollbar::thumbRect() const
{
    IntRect beforeThumbRect, thumbRect, afterThumbRect;
    splitTrack(trackRect(), beforeThumbRect, thumbRect, afterThumbRect);
    return thumbRect;
}

void PlatformScrollbar::splitTrack(const IntRect& trackRect, IntRect& beforeThumbRect, IntRect& thumbRect, IntRect& afterThumbRect) const
{
    // This function won't even get called unless we're big enough to have some combination of these three rects where at least
    // one of them is non-empty.
    int thumbPos = thumbPosition();
    if (m_orientation == HorizontalScrollbar) {
        thumbRect = IntRect(trackRect.x() + thumbPos, trackRect.y() + (trackRect.height() - cThumbHeight) / 2, thumbLength(), cThumbHeight);
        beforeThumbRect = IntRect(trackRect.x(), trackRect.y(), thumbPos, trackRect.height());
        afterThumbRect = IntRect(thumbRect.x() + thumbRect.width(), trackRect.y(), trackRect.right() - thumbRect.right(), trackRect.height());
    } else {
        thumbRect = IntRect(trackRect.x() + (trackRect.width() - cThumbWidth) / 2, trackRect.y() + thumbPos, cThumbWidth, thumbLength());
        beforeThumbRect = IntRect(trackRect.x(), trackRect.y(), trackRect.width(), thumbPos);
        afterThumbRect = IntRect(trackRect.x(), thumbRect.y() + thumbRect.height(), trackRect.width(), trackRect.bottom() - thumbRect.bottom());
    }
}

int PlatformScrollbar::thumbPosition() const
{
    if (!isEnabled())
        return 0;

     return (int)ceilf(m_currentPos * (trackLength() - thumbLength()) / (m_totalSize - m_visibleSize));
}

int PlatformScrollbar::thumbLength() const
{
    if (!isEnabled())
        return 0;

    float proportion = (float)(m_visibleSize) / m_totalSize;
    int trackLen = trackLength();
    int length = (int)ceilf(proportion * trackLen);
    int minLength = cThumbMinLength;
    length = max(length, minLength);
    if (length > trackLen)
        length = 0; // Once the thumb is below the track length, it just goes away (to make more room for the track).
    return length;
}

int PlatformScrollbar::trackLength() const
{
    return (m_orientation == HorizontalScrollbar) ? trackRect().width() : trackRect().height();
}

void PlatformScrollbar::paintButton(GraphicsContext* context, const IntRect& rect, bool start, const IntRect& damageRect) const
{
    IntRect paintRect = buttonRepaintRect(rect, m_orientation, controlSize(), start);
    
    if (!damageRect.intersects(paintRect))
        return;

    IntRect drawRect = paintRect;

    SCROLLPART part;
    PARTSTATE state = LEAVE;
    bool enable = false;
    if (m_orientation == HorizontalScrollbar)
        part = start ? ScrollLeftArrowPart : ScrollRightArrowPart;
    else
        part = start ? ScrollUpArrowPart : ScrollDownArrowPart;

    if (isEnabled())
        enable = true;
    if ((m_pressedPart == BackButtonPart && start) || (m_pressedPart == ForwardButtonPart && !start))
        state = PRESS;

    paintScrollPart (part, context->platformContext(), drawRect, enable, state); 
}

void PlatformScrollbar::paintTrack(GraphicsContext* context, const IntRect& rect, bool start, const IntRect& damageRect) const
{
    IntRect paintRect = trackRepaintRect(rect, m_orientation, controlSize());
    
    if (!damageRect.intersects(paintRect))
        return;

    IntRect drawRect = paintRect;

    SCROLLPART part = (m_orientation == HorizontalScrollbar) ? HScrollTrackPart : VScrollTrackPart;
    bool enable = false;
    PARTSTATE state = LEAVE;
    if (isEnabled())
        enable = true;

    paintScrollPart (part, context->platformContext(), drawRect, enable, state);
}

void PlatformScrollbar::paintThumb(GraphicsContext* context, const IntRect& rect, const IntRect& damageRect) const
{
    if (!damageRect.intersects(rect))
        return;

    IntRect drawRect = rect;

    SCROLLPART part = (m_orientation == HorizontalScrollbar) ? HScrollThumbPart : VScrollThumbPart;
    bool enable = false;
    PARTSTATE state = LEAVE;
    if (isEnabled())
        enable = true;

    paintScrollPart (part, context->platformContext(), drawRect, enable, state);
}

ScrollbarPart PlatformScrollbar::hitTest(const PlatformMouseEvent& evt)
{
    ScrollbarPart result = NoPart;
    if (!isEnabled())
        return result;

    IntPoint mousePosition = convertFromContainingWindow(evt.pos());
    mousePosition.move(x(), y());
    if (backButtonRect().contains(mousePosition))
        result = BackButtonPart;
    else if (forwardButtonRect().contains(mousePosition))
        result = ForwardButtonPart;
    else {
        IntRect track = trackRect();
        if (track.contains(mousePosition)) {
            IntRect beforeThumbRect, thumbRect, afterThumbRect;
            splitTrack(track, beforeThumbRect, thumbRect, afterThumbRect);
            if (beforeThumbRect.contains(mousePosition))
                result = BackTrackPart;
            else if (thumbRect.contains(mousePosition))
                result = ThumbPart;
            else
                result = ForwardTrackPart;
        }
    }
    return result;
}


bool PlatformScrollbar::handleMouseMoveEvent(const PlatformMouseEvent& evt)
{
    if (m_pressedPart == ThumbPart) {
        // Drag the thumb.
        int thumbPos = thumbPosition();
        int thumbLen = thumbLength();
        int trackLen = trackLength();
        int maxPos = trackLen - thumbLen;
        int delta = 0;
        if (m_orientation == HorizontalScrollbar)
            delta = convertFromContainingWindow(evt.pos()).x() - m_pressedPos;
        else
            delta = convertFromContainingWindow(evt.pos()).y() - m_pressedPos;

        if (delta > 0)
            // The mouse moved down/right.
            delta = min(maxPos - thumbPos, delta);
        else if (delta < 0)
            // The mouse moved up/left.
            delta = max(-thumbPos, delta);

        if (delta != 0) {
            setValue((int)ceilf(thumbPos + delta) * (m_totalSize - m_visibleSize) / (trackLen - thumbLen));
            m_pressedPos += thumbPosition() - thumbPos;
        }
        
        return true;
    }

    if (m_pressedPart != NoPart)
        m_pressedPos = (m_orientation == HorizontalScrollbar ? convertFromContainingWindow(evt.pos()).x() : convertFromContainingWindow(evt.pos()).y());

    ScrollbarPart part = hitTest(evt);    
    if (part != m_hoveredPart) {
        if (m_pressedPart != NoPart) {
            if (part == m_pressedPart) {
                // The mouse is moving back over the pressed part.  We
                // need to start up the timer action again.
                startTimerIfNeeded(cNormalTimerDelay);
                invalidatePart(m_pressedPart);
            } else if (m_hoveredPart == m_pressedPart) {
                // The mouse is leaving the pressed part.  Kill our timer
                // if needed.
                stopTimerIfNeeded();
                invalidatePart(m_pressedPart);
            }
        } else {
            invalidatePart(part);
            invalidatePart(m_hoveredPart);
        }
        m_hoveredPart = part;
    } 

    return true;
}

bool PlatformScrollbar::handleMouseOutEvent(const PlatformMouseEvent& evt)
{
    invalidatePart(m_hoveredPart);
    m_hoveredPart = NoPart;

    return true;
}

bool PlatformScrollbar::handleMousePressEvent(const PlatformMouseEvent& evt)
{
    m_pressedPart = hitTest(evt);
    m_pressedPos = (m_orientation == HorizontalScrollbar ? convertFromContainingWindow(evt.pos()).x() : convertFromContainingWindow(evt.pos()).y());
    invalidatePart(m_pressedPart);
    autoscrollPressedPart(cInitialTimerDelay);
    return true;
}

bool PlatformScrollbar::handleMouseReleaseEvent(const PlatformMouseEvent& evt)
{
    invalidatePart(m_pressedPart);
    m_pressedPart = NoPart;
    m_pressedPos = 0;
    stopTimerIfNeeded();

    if (parent() && parent()->isFrameView())
        static_cast<FrameView*>(parent())->frame()->eventHandler()->setMousePressed(false);

    return true;
}

void PlatformScrollbar::startTimerIfNeeded(double delay)
{
    // Don't do anything for the thumb.
    if (m_pressedPart == ThumbPart)
        return;

    // Handle the track.  We halt track scrolling once the thumb is level
    // with us.
    if ((m_pressedPart == BackTrackPart || m_pressedPart == ForwardTrackPart) && thumbUnderMouse()) {
        invalidatePart(m_pressedPart);
        m_hoveredPart = ThumbPart;
        return;
    }

    // We can't scroll if we've hit the beginning or end.
    ScrollDirection dir = pressedPartScrollDirection();
    if (dir == ScrollUp || dir == ScrollLeft) {
        if (m_currentPos == 0)
            return;
    } else {
        if (m_currentPos == m_totalSize - m_visibleSize)
            return;
    }

    m_scrollTimer.startOneShot(delay);
}

void PlatformScrollbar::stopTimerIfNeeded()
{
    if (m_scrollTimer.isActive())
        m_scrollTimer.stop();
}

void PlatformScrollbar::autoscrollPressedPart(double delay)
{
    // Don't do anything for the thumb or if nothing was pressed.
    if (m_pressedPart == ThumbPart || m_pressedPart == NoPart)
        return;

    // Handle the track.
    if ((m_pressedPart == BackTrackPart || m_pressedPart == ForwardTrackPart) && thumbUnderMouse()) {
        invalidatePart(m_pressedPart);
        m_hoveredPart = ThumbPart;
        return;
    }

    // Handle the arrows and track.
    if (scroll(pressedPartScrollDirection(), pressedPartScrollGranularity()))
        startTimerIfNeeded(delay);
}

void PlatformScrollbar::autoscrollTimerFired(Timer<PlatformScrollbar>*)
{
    autoscrollPressedPart(cNormalTimerDelay);
}

ScrollDirection PlatformScrollbar::pressedPartScrollDirection()
{
    if (m_orientation == HorizontalScrollbar) {
        if (m_pressedPart == BackButtonPart || m_pressedPart == BackTrackPart)
            return ScrollLeft;
        return ScrollRight;
    } else {
        if (m_pressedPart == BackButtonPart || m_pressedPart == BackTrackPart)
            return ScrollUp;
        return ScrollDown;
    }
}

ScrollGranularity PlatformScrollbar::pressedPartScrollGranularity()
{
    if (m_pressedPart == BackButtonPart || m_pressedPart == ForwardButtonPart)
        return ScrollByLine;
    return ScrollByPage;
}

bool PlatformScrollbar::thumbUnderMouse()
{
    // Construct a rect.
    IntRect thumb = thumbRect();
    thumb.move(-x(), -y());
    int begin = (m_orientation == HorizontalScrollbar) ? thumb.x() : thumb.y();
    int end = (m_orientation == HorizontalScrollbar) ? thumb.right() : thumb.bottom();
    return (begin <= m_pressedPos && m_pressedPos < end);
}

int PlatformScrollbar::horizontalScrollbarHeight(ScrollbarControlSize controlSize)
{
    return cHorizontalWidth;
}

int PlatformScrollbar::verticalScrollbarWidth(ScrollbarControlSize controlSize)
{
    return cVerticalHeight;
}

IntRect PlatformScrollbar::windowClipRect() const
{
    IntRect clipRect(0, 0, width(), height());
    clipRect = convertToContainingWindow(clipRect);
    if (m_client)
        clipRect.intersect(m_client->windowClipRect());
    return clipRect;
}

Color PlatformScrollbar::cornerColor (void)
{
    return PlatformScrollbar::g_cornerColor;
}

void PlatformScrollbar::paintGripper(GraphicsContext*, const IntRect& rect) const
{
    notImplemented();
}

IntRect PlatformScrollbar::gripperRect(const IntRect& thumbRect) const
{
    notImplemented();
    return IntRect();
}

void PlatformScrollbar::themeChanged()
{
    notImplemented();
}

} // namespace WebCore
