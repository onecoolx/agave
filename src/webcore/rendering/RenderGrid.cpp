/*
 * This file is part of the render object implementation for Agave.
 *
 * Copyright (C) 2026 Zhang Ji Peng
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

#include "config.h"
#include "RenderGrid.h"

#include "RenderLayer.h"
#include "RenderView.h"

using namespace std;

namespace WebCore {

RenderGrid::RenderGrid(Node* node)
    : RenderBlock(node)
{
    setChildrenInline(false); // Grid items are block-level.
    m_flexingChildren = false;
}

RenderGrid::~RenderGrid()
{
}

const char* RenderGrid::renderName() const
{
    if (isFloating())
        return "RenderGrid (floating)";
    if (isPositioned())
        return "RenderGrid (positioned)";
    if (isAnonymous())
        return "RenderGrid (anonymous)";
    if (isRelPositioned())
        return "RenderGrid (relative positioned)";
    return "RenderGrid";
}

void RenderGrid::layoutBlock(bool relayoutChildren)
{
    ASSERT(needsLayout());

#if ENABLE(MODERN_GRID)
    if (!relayoutChildren && layoutOnlyPositionedObjects())
        return;

    IntRect oldBounds;
    IntRect oldOutlineBox;
    bool checkForRepaint = checkForRepaintDuringLayout();
    if (checkForRepaint) {
        oldBounds = absoluteClippedOverflowRect();
        oldOutlineBox = absoluteOutlineBox();
    }

    view()->pushLayoutState(this, IntSize(m_x, m_y));

    int previousWidth = m_width;
    int previousHeight = m_height;

    calcWidth();
    calcHeight();
    m_overflowWidth = m_width;

    if (previousWidth != m_width || previousHeight != m_height)
        relayoutChildren = true;

    m_height = 0;
    m_overflowHeight = 0;
    m_flexingChildren = false;

    initMaxMarginValues();

    if (scrollsOverflow()) {
        if (style()->overflowX() == OSCROLL)
            m_layer->setHasHorizontalScrollbar(true);
        if (style()->overflowY() == OSCROLL)
            m_layer->setHasVerticalScrollbar(true);
    }

    layoutGrid(relayoutChildren);

    int oldHeight = m_height;
    calcHeight();
    if (oldHeight != m_height) {
        if (m_overflowHeight > m_height)
            m_overflowHeight -= (borderBottom() + paddingBottom() + horizontalScrollbarHeight());
        if (m_overflowHeight < m_height)
            m_overflowHeight = m_height;
    }
    if (previousHeight != m_height)
        relayoutChildren = true;

    layoutPositionedObjects(relayoutChildren || isRoot());

    if (m_overflowWidth < m_width)
        m_overflowWidth = m_width;

    view()->popLayoutState();

    if (hasOverflowClip())
        m_layer->updateScrollInfoAfterLayout();

    if (checkForRepaint)
        repaintAfterLayoutIfNeeded(oldBounds, oldOutlineBox);

    setNeedsLayout(false);
#else
    RenderBlock::layoutBlock(relayoutChildren);
#endif
}

#if ENABLE(MODERN_GRID)
// 2a-0 PROBE: hard-coded 2x2 equal-track grid.
// Splits the content box into a 2x2 grid of equal cells and places up to four
// flow children (in DOM order) into cells [0,0],[0,1],[1,0],[1,1]. Each item is
// sized to fill its cell via setOverrideSize (width) + a temporary fixed height
// (mirroring the flexbox stretch path). Purpose: validate two-dimensional cell
// positioning and size negotiation on the existing RenderBlock framework.
void RenderGrid::layoutGrid(bool relayoutChildren)
{
    const int contentLeft = borderLeft() + paddingLeft();
    const int contentTop = borderTop() + paddingTop();
    const int contentWidth = m_width - borderLeft() - paddingLeft() - borderRight() - paddingRight();

    // Resolve a definite container height from style (m_height was reset to 0).
    int contentHeight = 0;
    if (style()->height().isFixed())
        contentHeight = style()->height().value();
    else if (style()->height().isPercent() && containingBlock())
        contentHeight = (int)(style()->height().percent() * containingBlock()->contentHeight() / 100.0);

    const int cols = 2;
    const int rows = 2;
    const int cellWidth = contentWidth / cols;
    const int cellHeight = contentHeight > 0 ? (contentHeight / rows) : 0;

    m_flexingChildren = true;

    int index = 0;
    for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
        if (child->isPositioned()) {
            child->containingBlock()->insertPositionedObject(child);
            if (child->hasStaticX())
                child->setStaticX(contentLeft);
            if (child->hasStaticY())
                child->setStaticY(contentTop);
            continue;
        }

        if (index >= cols * rows) {
            // Probe only handles the first 2x2 cells.
            child->setChildNeedsLayout(true, false);
            child->layoutIfNeeded();
            continue;
        }

        int col = index % cols;
        int row = index / cols;

        if (relayoutChildren)
            child->setChildNeedsLayout(true, false);

        // Size the item to its cell.
        child->setOverrideSize(cellWidth);
        bool restoreHeight = false;
        Length savedHeight = child->style()->height();
        if (cellHeight > 0 && child->style()->height().isAuto()) {
            int innerH = cellHeight - child->borderTop() - child->paddingTop()
                - child->borderBottom() - child->paddingBottom()
                - child->marginTop() - child->marginBottom();
            if (innerH < 0)
                innerH = 0;
            child->style()->setHeight(Length(innerH, Fixed));
            restoreHeight = true;
        }
        child->setNeedsLayout(true, false);
        child->layoutIfNeeded();
        if (restoreHeight)
            child->style()->setHeight(savedHeight);

        int x = contentLeft + col * cellWidth + child->marginLeft();
        int y = contentTop + row * cellHeight + child->marginTop();
        child->setPos(x, y);

        index++;
    }

    m_flexingChildren = false;

    if (style()->height().isAuto())
        m_height = contentTop + (contentHeight > 0 ? contentHeight : 0) + borderBottom() + paddingBottom();
    else
        m_height = contentTop + contentHeight + borderBottom() + paddingBottom();

    m_overflowHeight = max(m_overflowHeight, m_height);
    m_overflowWidth = max(m_overflowWidth, m_width);
}
#endif // ENABLE(MODERN_GRID)

} // namespace WebCore
