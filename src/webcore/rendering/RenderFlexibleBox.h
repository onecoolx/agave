/*
 * This file is part of the render object implementation for KHTML.
 *
 * Copyright (C) 2003 Apple Computer, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef RenderFlexibleBox_h
#define RenderFlexibleBox_h

#include "RenderBlock.h"

namespace WebCore {

class RenderFlexibleBox : public RenderBlock {
public:
    RenderFlexibleBox(Node*);
    virtual ~RenderFlexibleBox();

    virtual const char* renderName() const;

    virtual void calcPrefWidths();
    void calcHorizontalPrefWidths();
    void calcVerticalPrefWidths();

    virtual void layoutBlock(bool relayoutChildren);
    void layoutHorizontalBox(bool relayoutChildren);
    void layoutVerticalBox(bool relayoutChildren);

#if ENABLE(MODERN_FLEXBOX)
    // Modern CSS Flexbox layout: single-line flex with direction, basis,
    // grow/shrink, justify-content, align-items. Used for display:flex/inline-flex.
    void layoutModernFlexbox(bool relayoutChildren);

    // Sets a flex item's override main-axis size, relays it out, and refreshes
    // the cached outer main/cross sizes. Shared by grow/shrink/clamp passes.
    void resolveFlexItemMainSize(RenderObject* child, bool isRow, int newMainContent,
                                 int mainMargin, int crossMargin, int& mainSize, int& crossSize);
#endif

    virtual bool avoidsFloats() const { return true; }

    virtual bool isFlexibleBox() const { return true; }
    virtual bool isFlexingChildren() const { return m_flexingChildren; }
    virtual bool isStretchingChildren() const { return m_stretchingChildren; }

    void placeChild(RenderObject* child, int x, int y);

protected:
    int allowedChildFlex(RenderObject* child, bool expanding, unsigned group);

    bool hasMultipleLines() const { return style()->boxLines() == MULTIPLE; }
    bool isVertical() const { return style()->boxOrient() == VERTICAL; }
    bool isHorizontal() const { return style()->boxOrient() == HORIZONTAL; }

    bool m_flexingChildren : 1;
    bool m_stretchingChildren : 1;
};

} // namespace WebCore

#endif // RenderFlexibleBox_h
