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
// Resolves track sizes given available space and per-track content minimums.
// fixed/percent tracks take their computed size; fr tracks share the leftover
// space in proportion to their fraction; auto tracks take their content size.
void RenderGrid::resolveTrackSizes(const Vector<GridTrackSize>& templates, int availableSpace,
                                   const Vector<int>& contentSizes, Vector<int>& outSizes)
{
    size_t n = templates.size();
    outSizes.clear();
    outSizes.resize(n);

    int usedSpace = 0;
    float totalFr = 0.0f;

    // Pass 1: resolve non-flexible tracks; accumulate fr weights.
    for (size_t i = 0; i < n; i++) {
        const GridTrackSize& t = templates[i];
        int size = 0;
        switch (t.kind) {
            case GridTrackSize::FixedTrack:
                size = t.length;
                break;
            case GridTrackSize::PercentTrack:
                size = availableSpace > 0 ? (int)(t.length * availableSpace / 100.0) : 0;
                break;
            case GridTrackSize::AutoTrack:
                size = (i < contentSizes.size()) ? contentSizes[i] : 0;
                break;
            case GridTrackSize::FrTrack:
                totalFr += t.fr;
                size = 0; // resolved in pass 2
                break;
        }
        outSizes[i] = size;
        if (t.kind != GridTrackSize::FrTrack)
            usedSpace += size;
    }

    // Pass 2: distribute remaining space across fr tracks.
    int freeSpace = availableSpace - usedSpace;
    if (freeSpace < 0)
        freeSpace = 0;
    if (totalFr > 0.0f) {
        for (size_t i = 0; i < n; i++) {
            if (templates[i].kind == GridTrackSize::FrTrack)
                outSizes[i] = (int)(freeSpace * (templates[i].fr / totalFr));
        }
    }
}

// 2a-4 track sizing: resolves explicit column/row tracks (fixed/percent/fr/auto)
// and places flow items into cells in DOM order (auto-flow: row). Full item
// placement (explicit grid-column/row, span) lands in 2a-5.
void RenderGrid::layoutGrid(bool relayoutChildren)
{
    const int contentLeft = borderLeft() + paddingLeft();
    const int contentTop = borderTop() + paddingTop();
    const int contentWidth = m_width - borderLeft() - paddingLeft() - borderRight() - paddingRight();

    int contentHeight = 0;
    if (style()->height().isFixed())
        contentHeight = style()->height().value();
    else if (style()->height().isPercent() && containingBlock())
        contentHeight = (int)(style()->height().percent() * containingBlock()->contentHeight() / 100.0);

    const int colGap = style()->gridColumnGap();
    const int rowGap = style()->gridRowGap();

    const Vector<GridTrackSize>& colTemplate = style()->gridTemplateColumns();
    const Vector<GridTrackSize>& rowTemplate = style()->gridTemplateRows();

    // A grid with no explicit columns behaves like a single auto column.
    int numCols = colTemplate.size() > 0 ? (int)colTemplate.size() : 1;

    // Count flow items to derive the implicit row count.
    int itemCount = 0;
    for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
        if (!child->isPositioned())
            itemCount++;
    }
    int explicitRows = rowTemplate.size() > 0 ? (int)rowTemplate.size() : 0;
    int neededRows = (itemCount + numCols - 1) / numCols;
    int numRows = max(explicitRows, max(neededRows, 1));

    m_flexingChildren = true;

    // --- Measure content sizes for auto tracks (natural item sizes) ---
    Vector<int> colContent(numCols, 0);
    Vector<int> rowContent(numRows, 0);
    {
        int index = 0;
        for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
            if (child->isPositioned())
                continue;
            child->setOverrideSize(-1);
            child->setChildNeedsLayout(true, false);
            child->calcWidth();
            child->layoutIfNeeded();
            int col = index % numCols;
            int row = index / numCols;
            int w = child->width() + child->marginLeft() + child->marginRight();
            int h = child->height() + child->marginTop() + child->marginBottom();
            if (w > colContent[col]) colContent[col] = w;
            if (row < numRows && h > rowContent[row]) rowContent[row] = h;
            index++;
        }
    }

    // --- Resolve column and row track sizes ---
    int colGapTotal = numCols > 1 ? (numCols - 1) * colGap : 0;
    int rowGapTotal = numRows > 1 ? (numRows - 1) * rowGap : 0;

    Vector<int> colSizes;
    if (colTemplate.size() > 0) {
        resolveTrackSizes(colTemplate, contentWidth - colGapTotal, colContent, colSizes);
    } else {
        colSizes.append(contentWidth);
    }

    Vector<int> rowSizes;
    if (rowTemplate.size() > 0) {
        int availRowSpace = (contentHeight > 0 ? contentHeight : 0) - rowGapTotal;
        resolveTrackSizes(rowTemplate, availRowSpace, rowContent, rowSizes);
        // Extra implicit rows (beyond template) take their content height.
        for (int r = (int)rowTemplate.size(); r < numRows; r++)
            rowSizes.append(r < (int)rowContent.size() ? rowContent[r] : 0);
    } else {
        for (int r = 0; r < numRows; r++)
            rowSizes.append(r < (int)rowContent.size() ? rowContent[r] : 0);
    }

    // --- Compute track offsets (cumulative positions including gaps) ---
    Vector<int> colPos(numCols, 0);
    int x = contentLeft;
    for (int c = 0; c < numCols; c++) {
        colPos[c] = x;
        x += (c < (int)colSizes.size() ? colSizes[c] : 0) + colGap;
    }
    Vector<int> rowPos(numRows, 0);
    int y = contentTop;
    for (int r = 0; r < numRows; r++) {
        rowPos[r] = y;
        y += (r < (int)rowSizes.size() ? rowSizes[r] : 0) + rowGap;
    }

    // --- Place items into cells (auto-flow: row) ---
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

        int col = index % numCols;
        int row = index / numCols;
        int cellW = col < (int)colSizes.size() ? colSizes[col] : 0;
        int cellH = row < (int)rowSizes.size() ? rowSizes[row] : 0;

        child->setOverrideSize(cellW - child->marginLeft() - child->marginRight());
        bool restoreHeight = false;
        Length savedHeight = child->style()->height();
        if (cellH > 0 && child->style()->height().isAuto()) {
            int innerH = cellH - child->borderTop() - child->paddingTop()
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

        child->setPos(colPos[col] + child->marginLeft(), rowPos[row] + child->marginTop());
        index++;
    }

    m_flexingChildren = false;

    // --- Container height ---
    if (style()->height().isAuto()) {
        int totalRows = 0;
        for (int r = 0; r < numRows; r++)
            totalRows += (r < (int)rowSizes.size() ? rowSizes[r] : 0);
        totalRows += rowGapTotal;
        m_height = contentTop + totalRows + borderBottom() + paddingBottom();
    } else {
        m_height = contentTop + contentHeight + borderBottom() + paddingBottom();
    }

    m_overflowHeight = max(m_overflowHeight, m_height);
    m_overflowWidth = max(m_overflowWidth, m_width);
}
#endif // ENABLE(MODERN_GRID)

} // namespace WebCore
