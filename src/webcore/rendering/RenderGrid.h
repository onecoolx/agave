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

#ifndef RenderGrid_h
#define RenderGrid_h

#include "RenderBlock.h"

namespace WebCore {

// Modern CSS Grid layout, isolated behind ENABLE_MODERN_GRID.
// Supports explicit and implicit tracks (fixed/percent/fr/auto/min-content/
// max-content/minmax), repeat(), line- and area-based item placement with
// spanning, auto-flow (row/column/dense), gaps, item alignment
// (justify/align-items/self) and content distribution (justify/align-content).
class RenderGrid : public RenderBlock {
public:
    RenderGrid(Node*);
    virtual ~RenderGrid();

    virtual const char* renderName() const;

    virtual bool isRenderGrid() const { return true; }
    virtual bool avoidsFloats() const { return true; }
    virtual bool isFlexingChildren() const { return m_flexingChildren; }

    virtual void layoutBlock(bool relayoutChildren);

#if ENABLE(MODERN_GRID)
    virtual void calcPrefWidths();
    void layoutGrid(bool relayoutChildren);

private:
    // Resolves a list of track sizes (fixed/percent/fr/auto) into pixel sizes,
    // given the available space and the per-track content-based minimums.
    // contentSizes may be empty (treats auto/content as 0).
    void resolveTrackSizes(const Vector<GridTrackSize>& templates, int availableSpace,
                           const Vector<int>& contentSizes, Vector<int>& outSizes);

    // Resolves a start/end GridPosition pair into a 0-based start index and a
    // span count. explicitCount is the number of explicit tracks in that axis.
    void resolveGridSpan(const GridPosition& start, const GridPosition& end,
                         int& outStart, int& outSpan);
#endif

private:
    bool m_flexingChildren : 1;
};

} // namespace WebCore

#endif // RenderGrid_h
