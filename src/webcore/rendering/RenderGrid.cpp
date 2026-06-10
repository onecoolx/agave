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
// Computes the grid container's intrinsic (min/max-content) width as the sum
// of column track widths plus column gaps. Fixed/percent tracks use their
// declared size; auto/fr tracks use the widest item assigned to that column.
// 2a subset: auto-flow:row column assignment, single-column-span items.
void RenderGrid::calcPrefWidths()
{
    ASSERT(prefWidthsDirty());

    if (style()->width().isFixed() && style()->width().value() > 0) {
        m_minPrefWidth = m_maxPrefWidth = calcContentBoxWidth(style()->width().value());
    } else {
        const Vector<GridTrackSize>& colTemplate = style()->gridTemplateColumns();
        int numCols = colTemplate.size() > 0 ? (int)colTemplate.size() : 1;
        int colGap = style()->gridColumnGap();

        // Per-column content min/max widths (auto-flow:row assignment).
        Vector<int> colMin(numCols, 0);
        Vector<int> colMax(numCols, 0);
        int index = 0;
        for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
            if (child->isPositioned())
                continue;
            int col = index % numCols;
            int childMin = child->minPrefWidth();
            int childMax = child->maxPrefWidth();
            int ml = child->style()->marginLeft().isFixed() ? child->style()->marginLeft().value() : 0;
            int mr = child->style()->marginRight().isFixed() ? child->style()->marginRight().value() : 0;
            childMin += ml + mr;
            childMax += ml + mr;
            if (childMin > colMin[col]) colMin[col] = childMin;
            if (childMax > colMax[col]) colMax[col] = childMax;
            index++;
        }

        int minTotal = 0;
        int maxTotal = 0;
        for (int c = 0; c < numCols; c++) {
            if (c < (int)colTemplate.size()) {
                const GridTrackSize& t = colTemplate[c];
                if (t.kind == GridTrackSize::FixedTrack) {
                    minTotal += t.length;
                    maxTotal += t.length;
                    continue;
                }
                // percent/fr/auto: fall back to content widths.
            }
            minTotal += colMin[c];
            maxTotal += colMax[c];
        }

        int gapTotal = numCols > 1 ? (numCols - 1) * colGap : 0;
        m_minPrefWidth = minTotal + gapTotal;
        m_maxPrefWidth = maxTotal + gapTotal;
        m_maxPrefWidth = max(m_minPrefWidth, m_maxPrefWidth);
    }

    if (style()->minWidth().isFixed() && style()->minWidth().value() > 0) {
        m_maxPrefWidth = max(m_maxPrefWidth, calcContentBoxWidth(style()->minWidth().value()));
        m_minPrefWidth = max(m_minPrefWidth, calcContentBoxWidth(style()->minWidth().value()));
    }
    if (style()->maxWidth().isFixed() && style()->maxWidth().value() != undefinedLength) {
        m_maxPrefWidth = min(m_maxPrefWidth, calcContentBoxWidth(style()->maxWidth().value()));
        m_minPrefWidth = min(m_minPrefWidth, calcContentBoxWidth(style()->maxWidth().value()));
    }

    int toAdd = borderLeft() + borderRight() + paddingLeft() + paddingRight();
    m_minPrefWidth += toAdd;
    m_maxPrefWidth += toAdd;

    setPrefWidthsDirty(false);
}

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

// Resolves a start/end GridPosition pair into a 0-based start index and span.
// outStart == -1 means the start is auto (to be auto-placed by the caller).
// 2a subset: line numbers are 1-based; span N means span N tracks.
void RenderGrid::resolveGridSpan(const GridPosition& start, const GridPosition& end,
                                 int& outStart, int& outSpan)
{
    outStart = -1;
    outSpan = 1;

    int startLine = (!start.isAuto && !start.isSpan) ? start.line : 0; // 1-based or 0
    int endLine = (!end.isAuto && !end.isSpan) ? end.line : 0;

    if (startLine > 0 && endLine > 0) {
        int a = min(startLine, endLine);
        int b = max(startLine, endLine);
        outStart = a - 1;
        outSpan = max(1, b - a);
        return;
    }
    if (startLine > 0) {
        outStart = startLine - 1;
        if (end.isSpan && end.line > 0)
            outSpan = end.line;
        return;
    }
    if (endLine > 0) {
        // Only end line given: place so the item ends at endLine.
        int span = (start.isSpan && start.line > 0) ? start.line : 1;
        outStart = max(0, endLine - 1 - span);
        outSpan = span;
        return;
    }
    // No definite line: auto start. Span may still be set via either side.
    if (start.isSpan && start.line > 0)
        outSpan = start.line;
    else if (end.isSpan && end.line > 0)
        outSpan = end.line;
}

// Explicit CSS Grid layout (2a). Six phases: resolve explicit positions,
// auto-flow placement (row), measure content, resolve track sizes, compute
// track offsets, then size and position each item across its span.
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

    const int numCols = colTemplate.size() > 0 ? (int)colTemplate.size() : 1;

    m_flexingChildren = true;

    // --- Per-item placement record ---
    struct ItemPlacement {
        RenderObject* child;
        int col, colSpan;
        int row, rowSpan;
    };
    Vector<ItemPlacement> items;

    // --- Phase 1: resolve explicit positions; collect auto-placed items ---
    for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
        if (child->isPositioned()) {
            child->containingBlock()->insertPositionedObject(child);
            if (child->hasStaticX())
                child->setStaticX(contentLeft);
            if (child->hasStaticY())
                child->setStaticY(contentTop);
            continue;
        }
        ItemPlacement p;
        p.child = child;
        resolveGridSpan(child->style()->gridColumnStart(), child->style()->gridColumnEnd(), p.col, p.colSpan);
        resolveGridSpan(child->style()->gridRowStart(), child->style()->gridRowEnd(), p.row, p.rowSpan);
        // Clamp column span to the number of columns.
        if (p.colSpan > numCols) p.colSpan = numCols;
        if (p.col >= 0 && p.col + p.colSpan > numCols)
            p.col = max(0, numCols - p.colSpan);
        items.append(p);
    }

    // --- Phase 2: auto-placement (auto-flow: row) ---
    // Upper bound on rows: explicit rows, any row reached by definite placement,
    // plus one row per auto item (worst case one item per row). Pre-allocate the
    // occupancy grid once so resize never leaves bool cells uninitialized.
    int explicitRows = rowTemplate.size() > 0 ? (int)rowTemplate.size() : 0;
    int maxDefiniteRow = 0;
    int autoCount = 0;
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].row >= 0)
            maxDefiniteRow = max(maxDefiniteRow, items[i].row + items[i].rowSpan);
        else
            autoCount++;
    }
    int maxRows = max(explicitRows, maxDefiniteRow) + autoCount + 1;
    if (maxRows < 1)
        maxRows = 1;

    Vector<bool> occupied(maxRows * numCols, false);
    for (int k = 0; k < maxRows * numCols; k++)
        occupied[k] = false;

    int usedRows = max(explicitRows, max(maxDefiniteRow, 1));

    // Mark cells of items with a definite row+col.
    for (size_t i = 0; i < items.size(); i++) {
        ItemPlacement& p = items[i];
        if (p.row >= 0 && p.col >= 0) {
            for (int r = p.row; r < p.row + p.rowSpan && r < maxRows; r++) {
                for (int c = p.col; c < p.col + p.colSpan && c < numCols; c++)
                    occupied[r * numCols + c] = true;
            }
        }
    }

    // Flow auto-placed items into the next free cell (row-major).
    int cursorRow = 0, cursorCol = 0;
    for (size_t i = 0; i < items.size(); i++) {
        ItemPlacement& p = items[i];
        if (p.row >= 0 && p.col >= 0)
            continue; // already placed

        int span = p.colSpan;

        if (p.col >= 0) {
            // Fixed column, auto row: scan this column downward for a free run.
            int fixedCol = p.col;
            if (fixedCol + span > numCols)
                span = max(1, numCols - fixedCol);
            int r = 0;
            while (r < maxRows - 1) {
                bool free = true;
                for (int c = fixedCol; c < fixedCol + span; c++)
                    if (occupied[r * numCols + c]) { free = false; break; }
                if (free)
                    break;
                r++;
            }
            p.row = r;
        } else {
            // Fully auto: advance the row-major cursor to the next free run.
            while (cursorRow < maxRows - 1) {
                if (cursorCol + span > numCols) {
                    cursorCol = 0;
                    cursorRow++;
                    continue;
                }
                bool free = true;
                for (int c = cursorCol; c < cursorCol + span; c++)
                    if (occupied[cursorRow * numCols + c]) { free = false; break; }
                if (free)
                    break;
                cursorCol++;
            }
            p.col = cursorCol;
            p.row = cursorRow;
            cursorCol += span;
        }

        for (int r = p.row; r < p.row + p.rowSpan && r < maxRows; r++)
            for (int c = p.col; c < p.col + span && c < numCols; c++)
                occupied[r * numCols + c] = true;

        if (p.row + p.rowSpan > usedRows)
            usedRows = p.row + p.rowSpan;
    }

    int numRows = max(usedRows, 1);

    // --- Phase 3: measure content sizes (single-track items feed auto tracks) ---
    Vector<int> colContent(numCols, 0);
    Vector<int> rowContent(numRows, 0);
    for (size_t i = 0; i < items.size(); i++) {
        ItemPlacement& p = items[i];
        RenderObject* child = p.child;
        child->setOverrideSize(-1);
        child->setChildNeedsLayout(true, false);
        child->calcWidth();
        child->layoutIfNeeded();
        int w = child->width() + child->marginLeft() + child->marginRight();
        int h = child->height() + child->marginTop() + child->marginBottom();
        if (p.colSpan == 1 && p.col >= 0 && p.col < numCols && w > colContent[p.col])
            colContent[p.col] = w;
        if (p.rowSpan == 1 && p.row >= 0 && p.row < numRows && h > rowContent[p.row])
            rowContent[p.row] = h;
    }

    // --- Phase 4: resolve track sizes ---
    int colGapTotal = numCols > 1 ? (numCols - 1) * colGap : 0;
    int rowGapTotal = numRows > 1 ? (numRows - 1) * rowGap : 0;

    Vector<int> colSizes;
    if (colTemplate.size() > 0)
        resolveTrackSizes(colTemplate, contentWidth - colGapTotal, colContent, colSizes);
    else
        colSizes.append(contentWidth);

    Vector<int> rowSizes;
    if (rowTemplate.size() > 0) {
        int availRowSpace = (contentHeight > 0 ? contentHeight : 0) - rowGapTotal;
        resolveTrackSizes(rowTemplate, availRowSpace, rowContent, rowSizes);
        for (int r = (int)rowTemplate.size(); r < numRows; r++)
            rowSizes.append(r < (int)rowContent.size() ? rowContent[r] : 0);
    } else {
        for (int r = 0; r < numRows; r++)
            rowSizes.append(r < (int)rowContent.size() ? rowContent[r] : 0);
    }

    // --- Phase 5: track offsets ---
    Vector<int> colPos(numCols, 0);
    int x = contentLeft;
    for (int c = 0; c < numCols; c++) {
        colPos[c] = x;
        x += (c < (int)colSizes.size() ? colSizes[c] : 0) + colGap;
    }
    Vector<int> rowPos(numRows, 0);
    int yy = contentTop;
    for (int r = 0; r < numRows; r++) {
        rowPos[r] = yy;
        yy += (r < (int)rowSizes.size() ? rowSizes[r] : 0) + rowGap;
    }

    // --- Phase 6: size and position each item across its span ---
    for (size_t i = 0; i < items.size(); i++) {
        ItemPlacement& p = items[i];
        RenderObject* child = p.child;

        // Span size = sum of spanned tracks + interior gaps.
        int cellW = 0;
        for (int c = p.col; c < p.col + p.colSpan && c < numCols; c++)
            cellW += (c < (int)colSizes.size() ? colSizes[c] : 0);
        cellW += (p.colSpan - 1) * colGap;

        int cellH = 0;
        for (int r = p.row; r < p.row + p.rowSpan && r < numRows; r++)
            cellH += (r < (int)rowSizes.size() ? rowSizes[r] : 0);
        cellH += (p.rowSpan - 1) * rowGap;

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

        int px = (p.col < (int)colPos.size() ? colPos[p.col] : contentLeft) + child->marginLeft();
        int py = (p.row < (int)rowPos.size() ? rowPos[p.row] : contentTop) + child->marginTop();
        child->setPos(px, py);
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
