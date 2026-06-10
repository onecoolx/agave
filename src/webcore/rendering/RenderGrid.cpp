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

// Resolves a single implicit track (grid-auto-rows/columns) to a pixel size.
// fr/auto/content fall back to the measured content size; fixed/percent use
// their declared value. minmax uses its min component as the base.
static int resolveImplicitTrack(const GridTrackSize& t, int availableSpace, int content)
{
    GridTrackSize::Kind kind = t.kind;
    int length = t.length;
    switch (kind) {
        case GridTrackSize::FixedTrack:   return length;
        case GridTrackSize::PercentTrack: return availableSpace > 0 ? (int)(length * availableSpace / 100.0) : 0;
        case GridTrackSize::MinContentTrack:
        case GridTrackSize::MaxContentTrack:
        case GridTrackSize::AutoTrack:    return content;
        case GridTrackSize::FrTrack:      return content; // no free space context for implicit
    }
    return content;
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

    // Pass 1: resolve base sizes; accumulate fr weights for flexible tracks.
    // A track is flexible if it is an fr track, or a minmax() whose max is fr.
    for (size_t i = 0; i < n; i++) {
        const GridTrackSize& t = templates[i];
        int content = (i < contentSizes.size()) ? contentSizes[i] : 0;
        int size = 0;
        bool flexible = false;

        if (t.isMinMax) {
            // Base size from the min component.
            switch (t.kind) {
                case GridTrackSize::FixedTrack:   size = t.length; break;
                case GridTrackSize::PercentTrack: size = availableSpace > 0 ? (int)(t.length * availableSpace / 100.0) : 0; break;
                case GridTrackSize::MaxContentTrack:
                case GridTrackSize::MinContentTrack:
                case GridTrackSize::AutoTrack:    size = content; break;
                case GridTrackSize::FrTrack:      size = 0; break; // fr as min is invalid; treat as 0
            }
            if (t.maxKind == GridTrackSize::FrTrack) {
                flexible = true;
                totalFr += t.maxFr;
            } else {
                // Clamp base up to a definite max where applicable.
                int maxSize = size;
                switch (t.maxKind) {
                    case GridTrackSize::FixedTrack:   maxSize = t.maxLength; break;
                    case GridTrackSize::PercentTrack: maxSize = availableSpace > 0 ? (int)(t.maxLength * availableSpace / 100.0) : size; break;
                    case GridTrackSize::MaxContentTrack:
                    case GridTrackSize::MinContentTrack:
                    case GridTrackSize::AutoTrack:    maxSize = content; break;
                    case GridTrackSize::FrTrack:      break;
                }
                if (maxSize > size)
                    size = maxSize;
            }
        } else {
            switch (t.kind) {
                case GridTrackSize::FixedTrack:   size = t.length; break;
                case GridTrackSize::PercentTrack: size = availableSpace > 0 ? (int)(t.length * availableSpace / 100.0) : 0; break;
                case GridTrackSize::MaxContentTrack:
                case GridTrackSize::MinContentTrack:
                case GridTrackSize::AutoTrack:    size = content; break;
                case GridTrackSize::FrTrack:      flexible = true; totalFr += t.fr; size = 0; break;
            }
        }

        outSizes[i] = size;
        if (!flexible)
            usedSpace += size;
    }

    // Pass 2: distribute remaining space across flexible (fr) tracks.
    int freeSpace = availableSpace - usedSpace;
    if (freeSpace < 0)
        freeSpace = 0;
    if (totalFr > 0.0f) {
        for (size_t i = 0; i < n; i++) {
            const GridTrackSize& t = templates[i];
            bool isFr = (!t.isMinMax && t.kind == GridTrackSize::FrTrack);
            bool isMinMaxFr = (t.isMinMax && t.maxKind == GridTrackSize::FrTrack);
            if (isFr) {
                outSizes[i] = (int)(freeSpace * (t.fr / totalFr));
            } else if (isMinMaxFr) {
                // Flexible length grows from its base (min) by its fr share.
                int grow = (int)(freeSpace * (t.maxFr / totalFr));
                if (grow > outSizes[i])
                    outSizes[i] = grow;
            }
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

// Computes content-distribution offset (before first track) and extra gap
// (between tracks) for justify-content / align-content.
static void gridContentDistribution(EGridContent mode, int freeSpace, int trackCount,
                                    int& outOffset, int& outExtraGap)
{
    outOffset = 0;
    outExtraGap = 0;
    if (freeSpace <= 0 || trackCount <= 0)
        return;
    switch (mode) {
        case GridContentStart:
        case GridContentStretch: // stretch handled at track sizing; no offset here
            break;
        case GridContentEnd:
            outOffset = freeSpace;
            break;
        case GridContentCenter:
            outOffset = freeSpace / 2;
            break;
        case GridContentSpaceBetween:
            if (trackCount > 1)
                outExtraGap = freeSpace / (trackCount - 1);
            break;
        case GridContentSpaceAround:
            outExtraGap = freeSpace / trackCount;
            outOffset = outExtraGap / 2;
            break;
    }
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

    int numCols = colTemplate.size() > 0 ? (int)colTemplate.size() : 1;

    // For column auto-flow with a fixed row count, columns may need to grow to
    // fit all items. Estimate the required column count up front.
    if (style()->gridAutoFlow() == GridAutoFlowColumn && rowTemplate.size() > 0) {
        int rows = (int)rowTemplate.size();
        int flowItems = 0;
        for (RenderObject* c = firstChild(); c; c = c->nextSibling())
            if (!c->isPositioned())
                flowItems++;
        int neededCols = (flowItems + rows - 1) / rows;
        if (neededCols > numCols)
            numCols = neededCols;
    }

    m_flexingChildren = true;

    // --- Per-item placement record ---
    struct ItemPlacement {
        RenderObject* child;
        int col, colSpan;
        int row, rowSpan;
        int naturalW, naturalH; // outer size at natural (content) sizing
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
        p.naturalW = 0;
        p.naturalH = 0;
        resolveGridSpan(child->style()->gridColumnStart(), child->style()->gridColumnEnd(), p.col, p.colSpan);
        resolveGridSpan(child->style()->gridRowStart(), child->style()->gridRowEnd(), p.row, p.rowSpan);

        // grid-area naming a template area overrides line-based placement.
        const String& areaName = child->style()->gridArea();
        if (!areaName.isEmpty()) {
            const Vector<GridNamedArea>& areas = style()->gridTemplateAreas();
            for (size_t a = 0; a < areas.size(); a++) {
                if (areas[a].name == areaName) {
                    p.col = areas[a].colStart;
                    p.colSpan = max(1, areas[a].colEnd - areas[a].colStart);
                    p.row = areas[a].rowStart;
                    p.rowSpan = max(1, areas[a].rowEnd - areas[a].rowStart);
                    break;
                }
            }
        }

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

    // Flow auto-placed items. Direction (row/column) and dense packing come
    // from grid-auto-flow. For column flow the row count is fixed (explicit
    // template rows) and items fill down each column before moving right.
    const bool columnFlow = (style()->gridAutoFlow() == GridAutoFlowColumn);
    const bool dense = style()->gridAutoFlowDense();
    int flowRows = (explicitRows > 0) ? explicitRows : max(usedRows, 1);

    int cursorRow = 0, cursorCol = 0;
    for (size_t i = 0; i < items.size(); i++) {
        ItemPlacement& p = items[i];
        if (p.row >= 0 && p.col >= 0)
            continue; // already placed

        int cspan = p.colSpan;

        if (p.col >= 0) {
            // Fixed column, auto row: scan this column downward for a free run.
            int fixedCol = p.col;
            if (fixedCol + cspan > numCols)
                cspan = max(1, numCols - fixedCol);
            int r = 0;
            while (r < maxRows - 1) {
                bool free = true;
                for (int c = fixedCol; c < fixedCol + cspan; c++)
                    if (occupied[r * numCols + c]) { free = false; break; }
                if (free)
                    break;
                r++;
            }
            p.row = r;
        } else if (columnFlow) {
            // Column-major flow: iterate column by column, rows within each column.
            int startRow = dense ? 0 : cursorRow;
            int startCol = dense ? 0 : cursorCol;
            int rspan = p.rowSpan;
            int placeR = startRow, placeC = startCol;
            bool placed = false;
            for (int c = startCol; c < numCols && !placed; c++) {
                int rBegin = (c == startCol) ? startRow : 0;
                for (int r = rBegin; r + rspan <= flowRows; r++) {
                    bool free = true;
                    for (int cc = c; cc < c + cspan && cc < numCols; cc++)
                        for (int rr = r; rr < r + rspan; rr++)
                            if (occupied[rr * numCols + cc]) { free = false; break; }
                    if (free) { placeR = r; placeC = c; placed = true; break; }
                }
            }
            p.row = placeR;
            p.col = placeC;
            if (!dense) { cursorCol = placeC; cursorRow = placeR + rspan; if (cursorRow + rspan > flowRows) { cursorRow = 0; cursorCol++; } }
        } else {
            // Row-major flow (default).
            int r = dense ? 0 : cursorRow;
            int cStart = dense ? 0 : cursorCol;
            int placeR = r, placeC = cStart;
            bool placed = false;
            while (!placed && r < maxRows - 1) {
                for (int c = (r == (dense ? 0 : cursorRow) ? cStart : 0); c + cspan <= numCols; c++) {
                    bool free = true;
                    for (int cc = c; cc < c + cspan; cc++)
                        if (occupied[r * numCols + cc]) { free = false; break; }
                    if (free) { placeR = r; placeC = c; placed = true; break; }
                }
                if (!placed) r++;
            }
            p.row = placeR;
            p.col = placeC;
            if (!dense) { cursorRow = placeR; cursorCol = placeC + cspan; }
        }

        for (int r = p.row; r < p.row + p.rowSpan && r < maxRows; r++)
            for (int c = p.col; c < p.col + cspan && c < numCols; c++)
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
        p.naturalW = w;
        p.naturalH = h;
        if (p.colSpan == 1 && p.col >= 0 && p.col < numCols && w > colContent[p.col])
            colContent[p.col] = w;
        if (p.rowSpan == 1 && p.row >= 0 && p.row < numRows && h > rowContent[p.row])
            rowContent[p.row] = h;
    }

    // --- Phase 4: resolve track sizes ---
    int colGapTotal = numCols > 1 ? (numCols - 1) * colGap : 0;
    int rowGapTotal = numRows > 1 ? (numRows - 1) * rowGap : 0;

    const GridTrackSize& autoRow = style()->gridAutoRows();
    const GridTrackSize& autoCol = style()->gridAutoColumns();

    Vector<int> colSizes;
    if (colTemplate.size() > 0) {
        resolveTrackSizes(colTemplate, contentWidth - colGapTotal, colContent, colSizes);
        // Implicit columns (beyond the explicit template) use grid-auto-columns.
        for (int c = (int)colTemplate.size(); c < numCols; c++) {
            int content = c < (int)colContent.size() ? colContent[c] : 0;
            colSizes.append(resolveImplicitTrack(autoCol, contentWidth, content));
        }
    } else {
        colSizes.append(contentWidth);
    }

    Vector<int> rowSizes;
    if (rowTemplate.size() > 0) {
        int availRowSpace = (contentHeight > 0 ? contentHeight : 0) - rowGapTotal;
        resolveTrackSizes(rowTemplate, availRowSpace, rowContent, rowSizes);
        // Implicit rows use grid-auto-rows (falling back to content).
        for (int r = (int)rowTemplate.size(); r < numRows; r++) {
            int content = r < (int)rowContent.size() ? rowContent[r] : 0;
            rowSizes.append(resolveImplicitTrack(autoRow, contentHeight, content));
        }
    } else {
        for (int r = 0; r < numRows; r++) {
            int content = r < (int)rowContent.size() ? rowContent[r] : 0;
            rowSizes.append(resolveImplicitTrack(autoRow, contentHeight, content));
        }
    }

    // --- Phase 5: track offsets ---
    // --- Phase 5: track offsets, with content alignment distribution ---
    // Compute leftover space along each axis for justify/align-content.
    int colTracksTotal = 0;
    for (int c = 0; c < numCols; c++)
        colTracksTotal += (c < (int)colSizes.size() ? colSizes[c] : 0);
    int rowTracksTotal = 0;
    for (int r = 0; r < numRows; r++)
        rowTracksTotal += (r < (int)rowSizes.size() ? rowSizes[r] : 0);

    int colFree = contentWidth - colTracksTotal - colGapTotal;
    if (colFree < 0) colFree = 0;
    int rowFree = (contentHeight > 0 ? contentHeight : rowTracksTotal) - rowTracksTotal - rowGapTotal;
    if (rowFree < 0) rowFree = 0;

    // Distribution offset (before first track) and extra gap (between tracks).
    int colOffset = 0, colExtraGap = 0;
    gridContentDistribution(style()->gridJustifyContent(), colFree, numCols, colOffset, colExtraGap);
    int rowOffset = 0, rowExtraGap = 0;
    gridContentDistribution(style()->gridAlignContent(), rowFree, numRows, rowOffset, rowExtraGap);

    Vector<int> colPos(numCols, 0);
    int x = contentLeft + colOffset;
    for (int c = 0; c < numCols; c++) {
        colPos[c] = x;
        x += (c < (int)colSizes.size() ? colSizes[c] : 0) + colGap + colExtraGap;
    }
    Vector<int> rowPos(numRows, 0);
    int yy = contentTop + rowOffset;
    for (int r = 0; r < numRows; r++) {
        rowPos[r] = yy;
        yy += (r < (int)rowSizes.size() ? rowSizes[r] : 0) + rowGap + rowExtraGap;
    }

    // --- Phase 6: size and position each item across its span ---
    const EGridAlign containerJustify = style()->gridJustifyItems();
    const EGridAlign containerAlign = style()->gridAlignItems();

    for (size_t i = 0; i < items.size(); i++) {
        ItemPlacement& p = items[i];
        RenderObject* child = p.child;

        // Span area = sum of spanned tracks + interior gaps.
        int cellW = 0;
        for (int c = p.col; c < p.col + p.colSpan && c < numCols; c++)
            cellW += (c < (int)colSizes.size() ? colSizes[c] : 0);
        cellW += (p.colSpan - 1) * colGap;

        int cellH = 0;
        for (int r = p.row; r < p.row + p.rowSpan && r < numRows; r++)
            cellH += (r < (int)rowSizes.size() ? rowSizes[r] : 0);
        cellH += (p.rowSpan - 1) * rowGap;

        // Resolve effective item alignment (self overrides container; -1 = auto).
        int js = child->style()->gridJustifySelf();
        EGridAlign justify = (js >= 0) ? (EGridAlign)js : containerJustify;
        int as = child->style()->gridAlignSelf();
        EGridAlign align = (as >= 0) ? (EGridAlign)as : containerAlign;

        int marginW = child->marginLeft() + child->marginRight();
        int marginH = child->marginTop() + child->marginBottom();

        // Row axis (justify): stretch fills the cell, else use natural width.
        bool restoreHeight = false;
        Length savedHeight = child->style()->height();
        if (justify == GridAlignStretch)
            child->setOverrideSize(cellW - marginW);
        else
            child->setOverrideSize(-1);

        // Block axis (align): stretch fills the cell height.
        if (align == GridAlignStretch && cellH > 0 && child->style()->height().isAuto()) {
            int innerH = cellH - child->borderTop() - child->paddingTop()
                - child->borderBottom() - child->paddingBottom() - marginH;
            if (innerH < 0)
                innerH = 0;
            child->style()->setHeight(Length(innerH, Fixed));
            restoreHeight = true;
        }
        child->setNeedsLayout(true, false);
        child->layoutIfNeeded();
        if (restoreHeight)
            child->style()->setHeight(savedHeight);

        int cellX = (p.col < (int)colPos.size() ? colPos[p.col] : contentLeft);
        int cellY = (p.row < (int)rowPos.size() ? rowPos[p.row] : contentTop);

        // Position within the cell area according to alignment. For non-stretch
        // axes use the item's natural outer size measured in Phase 3.
        int itemOuterW = (justify == GridAlignStretch) ? (child->width() + marginW) : p.naturalW;
        int itemOuterH = (align == GridAlignStretch) ? (child->height() + marginH) : p.naturalH;
        int px = cellX + child->marginLeft();
        int py = cellY + child->marginTop();
        if (justify == GridAlignEnd)
            px = cellX + cellW - itemOuterW + child->marginLeft();
        else if (justify == GridAlignCenter)
            px = cellX + (cellW - itemOuterW) / 2 + child->marginLeft();
        if (align == GridAlignEnd)
            py = cellY + cellH - itemOuterH + child->marginTop();
        else if (align == GridAlignCenter)
            py = cellY + (cellH - itemOuterH) / 2 + child->marginTop();

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
