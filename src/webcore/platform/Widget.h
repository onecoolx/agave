/*
 * Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.  All rights reserved.
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

#ifndef Widget_h
#define Widget_h

#include <wtf/Platform.h>

#include "macross.h"

namespace WebCore {

    class Cursor;
    class Event;
    class Font;
    class GraphicsContext;
    class IntPoint;
    class IntRect;
    class IntSize;
    class PlatformMouseEvent;
    class ScrollView;
    class WidgetClient;
    class WidgetPrivate;

    class Widget {
    public:
        Widget();
        virtual ~Widget();

        virtual void setEnabled(bool);
        virtual bool isEnabled() const;
        int x() const;
        int y() const;
        int width() const;
        int height() const;
        IntSize size() const;
        void resize(int, int);
        void resize(const IntSize&);
        IntPoint pos() const;
        void move(int, int);
        void move(const IntPoint&);

        virtual void paint(GraphicsContext*, const IntRect&);
        virtual void invalidate();
        virtual void invalidateRect(const IntRect&);

        virtual void setFrameGeometry(const IntRect&);
        virtual IntRect frameGeometry() const;

        virtual void setFocus();

        void setCursor(const Cursor&);
        Cursor cursor();

        virtual void show();
        virtual void hide();

        void setIsSelected(bool);

        void setClient(WidgetClient*);
        WidgetClient* client() const;

        virtual bool isFrameView() const;

        virtual void removeFromParent();

        // This method is used by plugins on all platforms to obtain a clip rect that includes clips set by WebCore,
        // e.g., in overflow:auto sections.  The clip rects coordinates are in the containing window's coordinate space.
        // This clip includes any clips that the widget itself sets up for its children.
        virtual IntRect windowClipRect() const;

        virtual void handleEvent(Event*) {}

        virtual void setParent(ScrollView*);
        virtual ScrollView* parent() const;

        virtual void geometryChanged() const {};
        
        IntRect convertToContainingWindow(const IntRect&) const;
        IntPoint convertToContainingWindow(const IntPoint&) const;
        IntPoint convertFromContainingWindow(const IntPoint&) const;

        virtual IntPoint convertChildToSelf(const Widget*, const IntPoint&) const;
        virtual IntPoint convertSelfToChild(const Widget*, const IntPoint&) const;

        bool suppressInvalidation() const;
        void setSuppressInvalidation(bool);


        void setContainingView(_mc_view*);
        _mc_view* containingView() const;
    private:
        WidgetPrivate* data;
    };

} // namespace WebCore

#endif // Widget_h
