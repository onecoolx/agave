/*
 * This file is part of the render object implementation for KHTML.
 *
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
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

#include "config.h"
#include "RenderFlexibleBox.h"

#include "CharacterNames.h"
#include "RenderLayer.h"
#include "RenderView.h"

using namespace std;

namespace WebCore {

class FlexBoxIterator {
public:
    FlexBoxIterator(RenderFlexibleBox* parent) {
        box = parent;
        if (box->style()->boxOrient() == HORIZONTAL && box->style()->direction() == RTL)
            forward = box->style()->boxDirection() != BNORMAL;
        else
            forward = box->style()->boxDirection() == BNORMAL;
        lastOrdinal = 1; 
        if (!forward) {
            // No choice, since we're going backwards, we have to find out the highest ordinal up front.
            RenderObject* child = box->firstChild();
            while (child) {
                if (child->style()->boxOrdinalGroup() > lastOrdinal)
                    lastOrdinal = child->style()->boxOrdinalGroup();
                child = child->nextSibling();
            }
        }
        
        reset();
    }

    void reset() {
        current = 0;
        currentOrdinal = forward ? 0 : lastOrdinal+1;
    }

    RenderObject* first() {
        reset();
        return next();
    }
    
    RenderObject* next() {

        do { 
            if (!current) {
                if (forward) {
                    currentOrdinal++; 
                    if (currentOrdinal > lastOrdinal)
                        return 0;
                    current = box->firstChild();
                } else {
                    currentOrdinal--;
                    if (currentOrdinal == 0)
                        return 0;
                    current = box->lastChild();
                }
            }
            else
                current = forward ? current->nextSibling() : current->previousSibling();
            if (current && current->style()->boxOrdinalGroup() > lastOrdinal)
                lastOrdinal = current->style()->boxOrdinalGroup();
        } while (!current || current->style()->boxOrdinalGroup() != currentOrdinal ||
                 current->style()->visibility() == COLLAPSE);
        return current;
    }

private:
    RenderFlexibleBox* box;
    RenderObject* current;
    bool forward;
    unsigned int currentOrdinal;
    unsigned int lastOrdinal;
};
    
RenderFlexibleBox::RenderFlexibleBox(Node* node)
:RenderBlock(node)
{
    setChildrenInline(false); // All of our children must be block-level
    m_flexingChildren = m_stretchingChildren = false;
}

RenderFlexibleBox::~RenderFlexibleBox()
{
}

void RenderFlexibleBox::calcHorizontalPrefWidths()
{
    RenderObject *child = firstChild();
    while (child) {
        // positioned children don't affect the minmaxwidth
        if (child->isPositioned() || child->style()->visibility() == COLLAPSE) {
            child = child->nextSibling();
            continue;
        }

        // A margin basically has three types: fixed, percentage, and auto (variable).
        // Auto and percentage margins simply become 0 when computing min/max width.
        // Fixed margins can be added in as is.
        Length ml = child->style()->marginLeft();
        Length mr = child->style()->marginRight();
        int margin = 0, marginLeft = 0, marginRight = 0;
        if (ml.isFixed())
            marginLeft += ml.value();
        if (mr.isFixed())
            marginRight += mr.value();
        margin = marginLeft + marginRight;

        m_minPrefWidth += child->minPrefWidth() + margin;
        m_maxPrefWidth += child->maxPrefWidth() + margin;

        child = child->nextSibling();
    }    
}

void RenderFlexibleBox::calcVerticalPrefWidths()
{
    RenderObject *child = firstChild();
    while(child != 0)
    {
        // Positioned children and collapsed children don't affect the min/max width
        if (child->isPositioned() || child->style()->visibility() == COLLAPSE) {
            child = child->nextSibling();
            continue;
        }

        // A margin basically has three types: fixed, percentage, and auto (variable).
        // Auto/percentage margins simply become 0 when computing min/max width.
        // Fixed margins can be added in as is.
        Length ml = child->style()->marginLeft();
        Length mr = child->style()->marginRight();
        int margin = 0;
        if (ml.isFixed())
            margin += ml.value();
        if (mr.isFixed())
            margin += mr.value();
        
        int w = child->minPrefWidth() + margin;
        m_minPrefWidth = max(w, m_minPrefWidth);
        
        w = child->maxPrefWidth() + margin;
        m_maxPrefWidth = max(w, m_maxPrefWidth);

        child = child->nextSibling();
    }    
}

void RenderFlexibleBox::calcPrefWidths()
{
    ASSERT(prefWidthsDirty());

    if (style()->width().isFixed() && style()->width().value() > 0)
        m_minPrefWidth = m_maxPrefWidth = calcContentBoxWidth(style()->width().value());
    else {
        m_minPrefWidth = m_maxPrefWidth = 0;

#if ENABLE(MODERN_FLEXBOX)
        if (style()->display() == FLEX || style()->display() == INLINE_FLEX) {
            EFlexDirection dir = style()->flexDirection();
            if (dir == FlowColumn || dir == FlowColumnReverse)
                calcVerticalPrefWidths();
            else
                calcHorizontalPrefWidths();
        } else
#endif
        if (hasMultipleLines() || isVertical())
            calcVerticalPrefWidths();
        else
            calcHorizontalPrefWidths();

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

void RenderFlexibleBox::layoutBlock(bool relayoutChildren)
{
    ASSERT(needsLayout());

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

    if (previousWidth != m_width || previousHeight != m_height ||
        (parent()->isFlexibleBox() && parent()->style()->boxOrient() == HORIZONTAL &&
         parent()->style()->boxAlign() == BSTRETCH))
        relayoutChildren = true;

    m_height = 0;
    m_overflowHeight = 0;
    m_flexingChildren = m_stretchingChildren = false;

    initMaxMarginValues();

    // For overflow:scroll blocks, ensure we have both scrollbars in place always.
    if (scrollsOverflow()) {
        if (style()->overflowX() == OSCROLL)
            m_layer->setHasHorizontalScrollbar(true);
        if (style()->overflowY() == OSCROLL)
            m_layer->setHasVerticalScrollbar(true);
    }

#if ENABLE(MODERN_FLEXBOX)
    // display:flex/inline-flex → modern flexbox; -webkit-box → legacy path.
    if (style()->display() == FLEX || style()->display() == INLINE_FLEX)
        layoutModernFlexbox(relayoutChildren);
    else if (isHorizontal())
        layoutHorizontalBox(relayoutChildren);
    else
        layoutVerticalBox(relayoutChildren);
#else
    if (isHorizontal())
        layoutHorizontalBox(relayoutChildren);
    else
        layoutVerticalBox(relayoutChildren);
#endif

    int oldHeight = m_height;
    calcHeight();
    if (oldHeight != m_height) {
        // If the block got expanded in size, then increase our overflowheight to match.
        if (m_overflowHeight > m_height)
            m_overflowHeight -= (borderBottom() + paddingBottom() + horizontalScrollbarHeight());
        if (m_overflowHeight < m_height)
            m_overflowHeight = m_height;
    }
    if (previousHeight != m_height)
        relayoutChildren = true;

    layoutPositionedObjects(relayoutChildren || isRoot());

    if (!isFloatingOrPositioned() && m_height == 0) {
        // We are a block with no border and padding and a computed height
        // of 0.  The CSS spec states that zero-height blocks collapse their margins
        // together.
        // When blocks are self-collapsing, we just use the top margin values and set the
        // bottom margin max values to 0.  This way we don't factor in the values
        // twice when we collapse with our previous vertically adjacent and
        // following vertically adjacent blocks.
        int pos = maxTopPosMargin();
        int neg = maxTopNegMargin();
        if (maxBottomPosMargin() > pos)
            pos = maxBottomPosMargin();
        if (maxBottomNegMargin() > neg)
            neg = maxBottomNegMargin();
        setMaxTopMargins(pos, neg);
        setMaxBottomMargins(0, 0);
    }

    // Always ensure our overflow width is at least as large as our width.
    if (m_overflowWidth < m_width)
        m_overflowWidth = m_width;

    if (!hasOverflowClip()) {
        if (ShadowData* boxShadow = style()->boxShadow()) {
            m_overflowLeft = min(m_overflowLeft, boxShadow->x - boxShadow->blur);
            m_overflowWidth = max(m_overflowWidth, m_width + boxShadow->x + boxShadow->blur);
            m_overflowTop = min(m_overflowTop, boxShadow->y - boxShadow->blur);
            m_overflowHeight = max(m_overflowHeight, m_height + boxShadow->y + boxShadow->blur);
        }
    }

    view()->popLayoutState();

    // Update our scrollbars if we're overflow:auto/scroll/hidden now that we know if
    // we overflow or not.
    if (hasOverflowClip())
        m_layer->updateScrollInfoAfterLayout();

    // Repaint with our new bounds if they are different from our old bounds.
    if (checkForRepaint)
        repaintAfterLayoutIfNeeded(oldBounds, oldOutlineBox);
    
    setNeedsLayout(false);
}

void RenderFlexibleBox::layoutHorizontalBox(bool relayoutChildren)
{
    int toAdd = borderBottom() + paddingBottom() + horizontalScrollbarHeight();
    int yPos = borderTop() + paddingTop();
    int xPos = borderLeft() + paddingLeft();
    bool heightSpecified = false;
    int oldHeight = 0;
    
    unsigned int highestFlexGroup = 0;
    unsigned int lowestFlexGroup = 0;
    bool haveFlex = false;
    int remainingSpace = 0;
    m_overflowHeight = m_height;

    // The first walk over our kids is to find out if we have any flexible children.
    FlexBoxIterator iterator(this);
    RenderObject* child = iterator.next();
    while (child) {
        // Check to see if this child flexes.
        if (!child->isPositioned() && child->style()->boxFlex() > 0.0f) {
            // We always have to lay out flexible objects again, since the flex distribution
            // may have changed, and we need to reallocate space.
            child->setOverrideSize(-1);
            if (!relayoutChildren)
                child->setChildNeedsLayout(true, false);
            haveFlex = true;
            unsigned int flexGroup = child->style()->boxFlexGroup();
            if (lowestFlexGroup == 0)
                lowestFlexGroup = flexGroup;
            if (flexGroup < lowestFlexGroup)
                lowestFlexGroup = flexGroup;
            if (flexGroup > highestFlexGroup)
                highestFlexGroup = flexGroup;
        }
        child = iterator.next();
    }
    
    // We do 2 passes.  The first pass is simply to lay everyone out at
    // their preferred widths.  The second pass handles flexing the children.
    do {
        // Reset our height.
        m_height = yPos;
        m_overflowHeight = m_height;
        xPos = borderLeft() + paddingLeft();
                
        // Our first pass is done without flexing.  We simply lay the children
        // out within the box.  We have to do a layout first in order to determine
        // our box's intrinsic height.
        int maxAscent = 0, maxDescent = 0;
        child = iterator.first();
        while (child) {
            // make sure we relayout children if we need it.
            if (relayoutChildren || (child->isReplaced() && (child->style()->width().isPercent() || child->style()->height().isPercent())))
                child->setChildNeedsLayout(true, false);
            
            if (child->isPositioned()) {
                child = iterator.next();
                continue;
            }
    
            // Compute the child's vertical margins.
            child->calcVerticalMargins();
    
            // Now do the layout.
            child->layoutIfNeeded();
    
            // Update our height and overflow height.
            if (style()->boxAlign() == BBASELINE) {
                int ascent = child->marginTop() + child->getBaselineOfFirstLineBox();
                if (ascent == -1)
                    ascent = child->marginTop() + child->height() + child->marginBottom();
                int descent = (child->marginTop() + child->height() + child->marginBottom()) - ascent;
                
                // Update our maximum ascent.
                maxAscent = max(maxAscent, ascent);
                
                // Update our maximum descent.
                maxDescent = max(maxDescent, descent);
                
                // Now update our height.
                m_height = max(yPos + maxAscent + maxDescent, m_height);
            }
            else
                m_height = max(m_height, yPos + child->marginTop() + child->height() + child->marginBottom());

            child = iterator.next();
        }
        
        if (!iterator.first() && hasLineIfEmpty())
            m_height += lineHeight(true, true);
        
        m_height += toAdd;

        // Always make sure our overflowheight is at least our height.
        if (m_overflowHeight < m_height)
            m_overflowHeight = m_height;
        
        oldHeight = m_height;
        calcHeight();

        relayoutChildren = false;
        if (oldHeight != m_height)
            heightSpecified = true;
        
        // Now that our height is actually known, we can place our boxes.
        m_stretchingChildren = (style()->boxAlign() == BSTRETCH);
        child = iterator.first();
        while (child) {
            if (child->isPositioned()) {
                child->containingBlock()->insertPositionedObject(child);
                if (child->hasStaticX()) {
                    if (style()->direction() == LTR)
                        child->setStaticX(xPos);
                    else child->setStaticX(width() - xPos);
                }
                if (child->hasStaticY())
                    child->setStaticY(yPos);
                child = iterator.next();
                continue;
            }
    
            // We need to see if this child's height has changed, since we make block elements
            // fill the height of a containing box by default.
            // Now do a layout.
            int oldChildHeight = child->height();
            static_cast<RenderBox*>(child)->calcHeight();
            if (oldChildHeight != child->height())
                child->setChildNeedsLayout(true, false);
            child->layoutIfNeeded();
    
            // We can place the child now, using our value of box-align.
            xPos += child->marginLeft();
            int childY = yPos;
            switch (style()->boxAlign()) {
                case BCENTER:
                    childY += child->marginTop() + max(0, (contentHeight() - (child->height() + child->marginTop() + child->marginBottom()))/2);
                    break;
                case BBASELINE: {
                    int ascent = child->marginTop() + child->getBaselineOfFirstLineBox();
                    if (ascent == -1)
                        ascent = child->marginTop() + child->height() + child->marginBottom();
                    childY += child->marginTop() + (maxAscent - ascent);
                    break;
                }
                case BEND:
                    childY += contentHeight() - child->marginBottom() - child->height();
                    break;
                default: // BSTART
                    childY += child->marginTop();
                    break;
            }

            placeChild(child, xPos, childY);

            if (child->isRenderBlock())
                static_cast<RenderBlock*>(child)->addVisualOverflow(static_cast<RenderBlock*>(child)->floatRect());

            m_overflowHeight = max(m_overflowHeight, childY + child->overflowHeight(false));
            m_overflowTop = min(m_overflowTop, child->yPos() + child->overflowTop(false));
            
            xPos += child->width() + child->marginRight();
    
            child = iterator.next();
        }

        remainingSpace = borderLeft() + paddingLeft() + contentWidth() - xPos;
        
        m_stretchingChildren = false;
        if (m_flexingChildren)
            haveFlex = false; // We're done.
        else if (haveFlex) {
            // We have some flexible objects.  See if we need to grow/shrink them at all.
            if (!remainingSpace)
                break;

            // Allocate the remaining space among the flexible objects.  If we are trying to
            // grow, then we go from the lowest flex group to the highest flex group.  For shrinking,
            // we go from the highest flex group to the lowest group.
            bool expanding = remainingSpace > 0;
            unsigned int start = expanding ? lowestFlexGroup : highestFlexGroup;
            unsigned int end = expanding? highestFlexGroup : lowestFlexGroup;
            for (unsigned int i = start; i <= end && remainingSpace; i++) {
                // Always start off by assuming the group can get all the remaining space.
                int groupRemainingSpace = remainingSpace;
                do {
                    // Flexing consists of multiple passes, since we have to change ratios every time an object hits its max/min-width
                    // For a given pass, we always start off by computing the totalFlex of all objects that can grow/shrink at all, and
                    // computing the allowed growth before an object hits its min/max width (and thus
                    // forces a totalFlex recomputation).
                    int groupRemainingSpaceAtBeginning = groupRemainingSpace;
                    float totalFlex = 0.0f;
                    child = iterator.first();
                    while (child) {
                        if (allowedChildFlex(child, expanding, i))
                            totalFlex += child->style()->boxFlex();
                        child = iterator.next();
                    }
                    child = iterator.first();
                    int spaceAvailableThisPass = groupRemainingSpace;
                    while (child) {
                        int allowedFlex = allowedChildFlex(child, expanding, i);
                        if (allowedFlex) {
                            int projectedFlex = (allowedFlex == INT_MAX) ? allowedFlex : (int)(allowedFlex * (totalFlex / child->style()->boxFlex()));
                            spaceAvailableThisPass = expanding ? min(spaceAvailableThisPass, projectedFlex) : max(spaceAvailableThisPass, projectedFlex);
                        }
                        child = iterator.next();
                    }

                    // The flex groups may not have any flexible objects this time around. 
                    if (!spaceAvailableThisPass || totalFlex == 0.0f) {
                        // If we just couldn't grow/shrink any more, then it's time to transition to the next flex group.
                        groupRemainingSpace = 0;
                        continue;
                    }

                    // Now distribute the space to objects.
                    child = iterator.first();
                    while (child && spaceAvailableThisPass && totalFlex) {
                        if (allowedChildFlex(child, expanding, i)) {
                            int spaceAdd = (int)(spaceAvailableThisPass * (child->style()->boxFlex()/totalFlex));
                            if (spaceAdd) {
                                child->setOverrideSize(child->overrideWidth() + spaceAdd);
                                m_flexingChildren = true;
                                relayoutChildren = true;
                            }

                            spaceAvailableThisPass -= spaceAdd;
                            remainingSpace -= spaceAdd;
                            groupRemainingSpace -= spaceAdd;
                            
                            totalFlex -= child->style()->boxFlex();
                        }
                        child = iterator.next();
                    }
                    if (groupRemainingSpace == groupRemainingSpaceAtBeginning) {
                        // this is not advancing, avoid getting stuck by distributing the remaining pixels
                        child = iterator.first();
                        int spaceAdd = groupRemainingSpace > 0 ? 1 : -1;
                        while (child && groupRemainingSpace) {
                            if (allowedChildFlex(child, expanding, i)) {
                                child->setOverrideSize(child->overrideWidth() + spaceAdd);
                                m_flexingChildren = true;
                                relayoutChildren = true;
                                remainingSpace -= spaceAdd;
                                groupRemainingSpace -= spaceAdd;
                            }
                            child = iterator.next();
                        }
                    }
                } while (groupRemainingSpace);
            }

            // We didn't find any children that could grow.
            if (haveFlex && !m_flexingChildren)
                haveFlex = false;
        }
    } while (haveFlex);

    m_flexingChildren = false;
    
    if (remainingSpace > 0 && ((style()->direction() == LTR && style()->boxPack() != BSTART) ||
                               (style()->direction() == RTL && style()->boxPack() != BEND))) {
        // Children must be repositioned.
        int offset = 0;
        if (style()->boxPack() == BJUSTIFY) {
            // Determine the total number of children.
            int totalChildren = 0;
            child = iterator.first();
            while (child) {
                if (child->isPositioned()) {
                    child = iterator.next();
                    continue;
                }
                totalChildren++;
                child = iterator.next();
            }

            // Iterate over the children and space them out according to the
            // justification level.
            if (totalChildren > 1) {
                totalChildren--;
                bool firstChild = true;
                child = iterator.first();
                while (child) {
                    if (child->isPositioned()) {
                        child = iterator.next();
                        continue;
                    }

                    if (firstChild) {
                        firstChild = false;
                        child = iterator.next();
                        continue;
                    }

                    offset += remainingSpace/totalChildren;
                    remainingSpace -= (remainingSpace/totalChildren);
                    totalChildren--;

                    placeChild(child, child->xPos()+offset, child->yPos());
                    child = iterator.next();
                }
            }
        } else {
            if (style()->boxPack() == BCENTER)
                offset += remainingSpace/2;
            else // END for LTR, START for RTL
                offset += remainingSpace;
            child = iterator.first();
            while (child) {
                if (child->isPositioned()) {
                    child = iterator.next();
                    continue;
                }
                placeChild(child, child->xPos()+offset, child->yPos());
                child = iterator.next();
            }
        }
    }
    
    child = iterator.first();
    while (child && child->isPositioned()) {
        child = iterator.next();
    }
    
    if (child) {
        m_overflowLeft = min(child->xPos() + child->overflowLeft(false), m_overflowLeft);

        RenderObject* lastChild = child;
        while ((child = iterator.next())) {
            if (!child->isPositioned())
                lastChild = child;
        }
        m_overflowWidth = max(lastChild->xPos() + lastChild->overflowWidth(false), m_overflowWidth);
    }
    
    // So that the calcHeight in layoutBlock() knows to relayout positioned objects because of
    // a height change, we revert our height back to the intrinsic height before returning.
    if (heightSpecified)
        m_height = oldHeight;
}

void RenderFlexibleBox::layoutVerticalBox(bool relayoutChildren)
{
    int xPos = borderLeft() + paddingLeft();
    int yPos = borderTop() + paddingTop();
    if( style()->direction() == RTL )
        xPos = m_width - paddingRight() - borderRight();
    int toAdd = borderBottom() + paddingBottom() + horizontalScrollbarHeight();
    bool heightSpecified = false;
    int oldHeight = 0;
    
    unsigned int highestFlexGroup = 0;
    unsigned int lowestFlexGroup = 0;
    bool haveFlex = false;
    int remainingSpace = 0;
    
    // The first walk over our kids is to find out if we have any flexible children.
    FlexBoxIterator iterator(this);
    RenderObject *child = iterator.next();
    while (child) {
        // Check to see if this child flexes.
        if (!child->isPositioned() && child->style()->boxFlex() > 0.0f) {
            // We always have to lay out flexible objects again, since the flex distribution
            // may have changed, and we need to reallocate space.
            child->setOverrideSize(-1);
            if (!relayoutChildren)
                child->setChildNeedsLayout(true, false);
            haveFlex = true;
            unsigned int flexGroup = child->style()->boxFlexGroup();
            if (lowestFlexGroup == 0)
                lowestFlexGroup = flexGroup;
            if (flexGroup < lowestFlexGroup)
                lowestFlexGroup = flexGroup;
            if (flexGroup > highestFlexGroup)
                highestFlexGroup = flexGroup;
        }
        child = iterator.next();
    }

    // We confine the line clamp ugliness to vertical flexible boxes (thus keeping it out of
    // mainstream block layout); this is not really part of the XUL box model.
    bool haveLineClamp = style()->lineClamp() >= 0 && style()->lineClamp() <= 100;
    if (haveLineClamp) {
        int maxLineCount = 0;
        child = iterator.first();
        while (child) {
            if (!child->isPositioned()) {
                if (relayoutChildren || (child->isReplaced() && (child->style()->width().isPercent() || child->style()->height().isPercent())) ||
                    (child->style()->height().isAuto() && child->isBlockFlow() && !child->needsLayout())) {
                    child->setChildNeedsLayout(true, false);
                    
                    // Dirty all the positioned objects.
                    if (child->isRenderBlock()) {
                        static_cast<RenderBlock*>(child)->markPositionedObjectsForLayout();
                        static_cast<RenderBlock*>(child)->clearTruncation();
                    }
                }
                child->layoutIfNeeded();
                if (child->style()->height().isAuto() && child->isBlockFlow())
                    maxLineCount = max(maxLineCount, static_cast<RenderBlock*>(child)->lineCount());
            }
            child = iterator.next();
        }
        
        // Get the # of lines and then alter all block flow children with auto height to use the
        // specified height. We always try to leave room for at least one line.
        int numVisibleLines = max(1, static_cast<int>((maxLineCount + 1) * style()->lineClamp() / 100.0));
        if (numVisibleLines < maxLineCount) {
            for (child = iterator.first(); child; child = iterator.next()) {
                if (child->isPositioned() || !child->style()->height().isAuto() || !child->isBlockFlow())
                    continue;
                
                RenderBlock* blockChild = static_cast<RenderBlock*>(child);
                int lineCount = blockChild->lineCount();
                if (lineCount <= numVisibleLines)
                    continue;
                
                int newHeight = blockChild->heightForLineCount(numVisibleLines);
                if (newHeight == child->height())
                    continue;
                
                child->setChildNeedsLayout(true, false);
                child->setOverrideSize(newHeight);
                m_flexingChildren = true;
                child->layoutIfNeeded();
                m_flexingChildren = false;
                child->setOverrideSize(-1);
                
                // FIXME: For now don't support RTL.
                if (style()->direction() != LTR)
                    continue;
                
                // Get the last line
                RootInlineBox* lastLine = blockChild->lineAtIndex(lineCount-1);
                if (!lastLine)
                    continue;
                
                // See if the last item is an anchor
                InlineBox* anchorBox = lastLine->lastChild();
                if (!anchorBox)
                    continue;
                if (!anchorBox->object()->element())
                    continue;
                if (!anchorBox->object()->element()->isLink())
                    continue;
                
                RootInlineBox* lastVisibleLine = blockChild->lineAtIndex(numVisibleLines-1);
                if (!lastVisibleLine)
                    continue;

                const UChar ellipsisAndSpace[2] = { horizontalEllipsis, ' ' };
                static AtomicString ellipsisAndSpaceStr(ellipsisAndSpace, 2);

                const Font& font = style(numVisibleLines == 1)->font();
                int ellipsisAndSpaceWidth = font.width(TextRun(ellipsisAndSpace, 2));

                // Get ellipsis width + " " + anchor width
                int totalWidth = ellipsisAndSpaceWidth + anchorBox->width();
                
                // See if this width can be accommodated on the last visible line
                RenderBlock* destBlock = static_cast<RenderBlock*>(lastVisibleLine->object());
                RenderBlock* srcBlock = static_cast<RenderBlock*>(lastLine->object());
                
                // FIXME: Directions of src/destBlock could be different from our direction and from one another.
                if (srcBlock->style()->direction() != LTR)
                    continue;
                if (destBlock->style()->direction() != LTR)
                    continue;

                int blockEdge = destBlock->rightOffset(lastVisibleLine->yPos());
                if (!lastVisibleLine->canAccommodateEllipsis(true, blockEdge, 
                                                             lastVisibleLine->xPos() + lastVisibleLine->width(),
                                                             totalWidth))
                    continue;

                // Let the truncation code kick in.
                lastVisibleLine->placeEllipsis(ellipsisAndSpaceStr, true, blockEdge, totalWidth, anchorBox);
                destBlock->setHasMarkupTruncation(true);
            }
        }
    }

    // We do 2 passes.  The first pass is simply to lay everyone out at
    // their preferred widths.  The second pass handles flexing the children.
    // Our first pass is done without flexing.  We simply lay the children
    // out within the box.
    do {
        m_height = borderTop() + paddingTop();
        int minHeight = m_height + toAdd;
        m_overflowHeight = m_height;

        child = iterator.first();
        while (child) {
            // make sure we relayout children if we need it.
            if (!haveLineClamp && (relayoutChildren || (child->isReplaced() && (child->style()->width().isPercent() || child->style()->height().isPercent()))))
                child->setChildNeedsLayout(true, false);
    
            if (child->isPositioned())
            {
                child->containingBlock()->insertPositionedObject(child);
                if (child->hasStaticX()) {
                    if (style()->direction() == LTR)
                        child->setStaticX(borderLeft()+paddingLeft());
                    else
                        child->setStaticX(borderRight()+paddingRight());
                }
                if (child->hasStaticY())
                    child->setStaticY(m_height);
                child = iterator.next();
                continue;
            } 
    
            // Compute the child's vertical margins.
            child->calcVerticalMargins();
    
            // Add in the child's marginTop to our height.
            m_height += child->marginTop();
    
            // Now do a layout.
            child->layoutIfNeeded();
    
            // We can place the child now, using our value of box-align.
            int childX = borderLeft() + paddingLeft();
            switch (style()->boxAlign()) {
                case BCENTER:
                case BBASELINE: // Baseline just maps to center for vertical boxes
                    childX += child->marginLeft() + max(0, (contentWidth() - (child->width() + child->marginLeft() + child->marginRight()))/2);
                    break;
                case BEND:
                    if (style()->direction() == RTL)
                        childX += child->marginLeft();
                    else
                        childX += contentWidth() - child->marginRight() - child->width();
                    break;
                default: // BSTART/BSTRETCH
                    if (style()->direction() == LTR)
                        childX += child->marginLeft();
                    else
                        childX += contentWidth() - child->marginRight() - child->width();
                    break;
            }
    
            // Place the child.
            placeChild(child, childX, m_height);
            m_height += child->height() + child->marginBottom();
    
            if (child->isRenderBlock())
                static_cast<RenderBlock*>(child)->addVisualOverflow(static_cast<RenderBlock*>(child)->floatRect());

            // See if this child has made our overflow need to grow.
            m_overflowWidth = max(child->xPos() + child->overflowWidth(false), m_overflowWidth);
            m_overflowLeft = min(child->xPos() + child->overflowLeft(false), m_overflowLeft);
            
            child = iterator.next();
        }

        yPos = m_height;
        
        if (!iterator.first() && hasLineIfEmpty())
            m_height += lineHeight(true, true);
    
        m_height += toAdd;

        // Negative margins can cause our height to shrink below our minimal height (border/padding).
        // If this happens, ensure that the computed height is increased to the minimal height.
        if (m_height < minHeight)
            m_height = minHeight;

        // Always make sure our overflowheight is at least our height.
        if (m_overflowHeight < m_height)
            m_overflowHeight = m_height;

        // Now we have to calc our height, so we know how much space we have remaining.
        oldHeight = m_height;
        calcHeight();
        if (oldHeight != m_height)
            heightSpecified = true;

        remainingSpace = borderTop() + paddingTop() + contentHeight() - yPos;
        
        if (m_flexingChildren)
            haveFlex = false; // We're done.
        else if (haveFlex) {
            // We have some flexible objects.  See if we need to grow/shrink them at all.
            if (!remainingSpace)
                break;

            // Allocate the remaining space among the flexible objects.  If we are trying to
            // grow, then we go from the lowest flex group to the highest flex group.  For shrinking,
            // we go from the highest flex group to the lowest group.
            bool expanding = remainingSpace > 0;
            unsigned int start = expanding ? lowestFlexGroup : highestFlexGroup;
            unsigned int end = expanding? highestFlexGroup : lowestFlexGroup;
            for (unsigned int i = start; i <= end && remainingSpace; i++) {
                // Always start off by assuming the group can get all the remaining space.
                int groupRemainingSpace = remainingSpace;
                do {
                    // Flexing consists of multiple passes, since we have to change ratios every time an object hits its max/min-width
                    // For a given pass, we always start off by computing the totalFlex of all objects that can grow/shrink at all, and
                    // computing the allowed growth before an object hits its min/max width (and thus
                    // forces a totalFlex recomputation).
                    int groupRemainingSpaceAtBeginning = groupRemainingSpace;
                    float totalFlex = 0.0f;
                    child = iterator.first();
                    while (child) {
                        if (allowedChildFlex(child, expanding, i))
                            totalFlex += child->style()->boxFlex();
                        child = iterator.next();
                    }
                    child = iterator.first();
                    int spaceAvailableThisPass = groupRemainingSpace;
                    while (child) {
                        int allowedFlex = allowedChildFlex(child, expanding, i);
                        if (allowedFlex) {
                            int projectedFlex = (allowedFlex == INT_MAX) ? allowedFlex : (int)(allowedFlex * (totalFlex / child->style()->boxFlex()));
                            spaceAvailableThisPass = expanding ? min(spaceAvailableThisPass, projectedFlex) : max(spaceAvailableThisPass, projectedFlex);
                        }
                        child = iterator.next();
                    }

                    // The flex groups may not have any flexible objects this time around. 
                    if (!spaceAvailableThisPass || totalFlex == 0.0f) {
                        // If we just couldn't grow/shrink any more, then it's time to transition to the next flex group.
                        groupRemainingSpace = 0;
                        continue;
                    }
            
                    // Now distribute the space to objects.
                    child = iterator.first();
                    while (child && spaceAvailableThisPass && totalFlex) {
                        if (allowedChildFlex(child, expanding, i)) {
                            int spaceAdd = (int)(spaceAvailableThisPass * (child->style()->boxFlex()/totalFlex));
                            if (spaceAdd) {
                                child->setOverrideSize(child->overrideHeight() + spaceAdd);
                                m_flexingChildren = true;
                                relayoutChildren = true;
                            }

                            spaceAvailableThisPass -= spaceAdd;
                            remainingSpace -= spaceAdd;
                            groupRemainingSpace -= spaceAdd;
                            
                            totalFlex -= child->style()->boxFlex();
                        }
                        child = iterator.next();
                    }
                    if (groupRemainingSpace == groupRemainingSpaceAtBeginning) {
                        // this is not advancing, avoid getting stuck by distributing the remaining pixels
                        child = iterator.first();
                        int spaceAdd = groupRemainingSpace > 0 ? 1 : -1;
                        while (child && groupRemainingSpace) {
                            if (allowedChildFlex(child, expanding, i)) {
                                child->setOverrideSize(child->overrideHeight() + spaceAdd);
                                m_flexingChildren = true;
                                relayoutChildren = true;
                                remainingSpace -= spaceAdd;
                                groupRemainingSpace -= spaceAdd;
                            }
                            child = iterator.next();
                        }
                    }
                } while (groupRemainingSpace);
            }

            // We didn't find any children that could grow.
            if (haveFlex && !m_flexingChildren)
                haveFlex = false;
        }        
    } while (haveFlex);

    if (style()->boxPack() != BSTART && remainingSpace > 0) {
        // Children must be repositioned.
        int offset = 0;
        if (style()->boxPack() == BJUSTIFY) {
            // Determine the total number of children.
            int totalChildren = 0;
            child = iterator.first();
            while (child) {
                if (child->isPositioned()) {
                    child = iterator.next();
                    continue;
                }
                totalChildren++;
                child = iterator.next();
            }
            
            // Iterate over the children and space them out according to the
            // justification level.
            if (totalChildren > 1) {
                totalChildren--;
                bool firstChild = true;
                child = iterator.first();
                while (child) {
                    if (child->isPositioned()) {
                        child = iterator.next();
                        continue;
                    }
                    
                    if (firstChild) {
                        firstChild = false;
                        child = iterator.next();
                        continue;
                    }

                    offset += remainingSpace/totalChildren;
                    remainingSpace -= (remainingSpace/totalChildren);
                    totalChildren--;
                    placeChild(child, child->xPos(), child->yPos()+offset);
                    child = iterator.next();
                }
            }
        } else {
            if (style()->boxPack() == BCENTER)
                offset += remainingSpace/2;
            else // END
                offset += remainingSpace;
            child = iterator.first();
            while (child) {
                if (child->isPositioned()) {
                    child = iterator.next();
                    continue;
                }
                placeChild(child, child->xPos(), child->yPos()+offset);
                child = iterator.next();
            }
        }
    }
    
    child = iterator.first();
    while (child && child->isPositioned()) {
        child = iterator.next();
    }
    
    if (child) {
        m_overflowTop = min(child->yPos() + child->overflowTop(false), m_overflowTop);

        RenderObject* lastChild = child;
        while ((child = iterator.next())) {
            if (!child->isPositioned())
                lastChild = child;
        }
        m_overflowHeight = max(lastChild->yPos() + lastChild->overflowHeight(false), m_overflowHeight);
    }

    // So that the calcHeight in layoutBlock() knows to relayout positioned objects because of
    // a height change, we revert our height back to the intrinsic height before returning.
    if (heightSpecified)
        m_height = oldHeight; 
}

void RenderFlexibleBox::placeChild(RenderObject* child, int x, int y)
{
    IntRect oldRect(child->xPos(), child->yPos() , child->width(), child->height());

    // Place the child.
    child->setPos(x, y);

    // If the child moved, we have to repaint it as well as any floating/positioned
    // descendants.  An exception is if we need a layout.  In this case, we know we're going to
    // repaint ourselves (and the child) anyway.
    if (!selfNeedsLayout() && child->checkForRepaintDuringLayout())
        child->repaintDuringLayoutIfMoved(oldRect);
}

#if ENABLE(MODERN_FLEXBOX)
void RenderFlexibleBox::resolveFlexItemMainSize(RenderObject* child, bool isRow, int newMainContent,
                                                int mainMargin, int crossMargin, int& mainSize, int& crossSize)
{
    if (newMainContent < 0)
        newMainContent = 0;
    child->setOverrideSize(newMainContent);
    child->setNeedsLayout(true, false);
    child->layoutIfNeeded();
    mainSize = (isRow ? child->width() : child->height()) + mainMargin;
    crossSize = (isRow ? child->height() : child->width()) + crossMargin;
}

void RenderFlexibleBox::layoutModernFlexbox(bool relayoutChildren)
{
    const EFlexDirection dir = style()->flexDirection();
    const bool isRow = (dir == FlowRow || dir == FlowRowReverse);
    const bool isReverse = (dir == FlowRowReverse || dir == FlowColumnReverse);
    const EFlexWrap wrapMode = style()->flexWrap();
    const bool doWrap = (wrapMode != FlexNoWrap);
    const EFlexJustify justify = style()->justifyContent();
    const EFlexAlign align = style()->alignItems();

    const int mainStart = isRow ? (borderLeft() + paddingLeft()) : (borderTop() + paddingTop());
    const int crossStart = isRow ? (borderTop() + paddingTop()) : (borderLeft() + paddingLeft());

    int mainAvail = 0;
    if (isRow) {
        mainAvail = m_width - borderLeft() - paddingLeft() - borderRight() - paddingRight();
    } else {
        if (style()->height().isFixed())
            mainAvail = style()->height().value();
        else if (style()->height().isPercent() && containingBlock())
            mainAvail = (int)(style()->height().percent() * containingBlock()->contentHeight() / 100.0);
    }

    int crossContainerSize = 0;
    if (isRow) {
        if (style()->height().isFixed())
            crossContainerSize = style()->height().value();
        else if (style()->height().isPercent() && containingBlock())
            crossContainerSize = (int)(style()->height().percent() * containingBlock()->contentHeight() / 100.0);
    } else {
        crossContainerSize = m_width - borderLeft() - paddingLeft() - borderRight() - paddingRight();
    }

    m_overflowHeight = m_height;
    m_overflowWidth = m_width;

    EBoxOrient savedOrient = style()->boxOrient();
    style()->setBoxOrient(isRow ? HORIZONTAL : VERTICAL);
    m_flexingChildren = true;

    struct FlexItem {
        RenderObject* child;
        int baseSize, mainMargin, crossMargin;
        float grow, shrink;
        int mainSize, crossSize;
        int order;
        EFlexAlignSelf alignSelf;
        bool autoMarginMainStart, autoMarginMainEnd;
    };

    Vector<FlexItem> allItems;
    FlexBoxIterator iterator(this);
    for (RenderObject* child = iterator.first(); child; child = iterator.next()) {
        if (child->isPositioned()) {
            // Positioned children are laid out later by layoutPositionedObjects;
            // give them a static position at the current main-axis start.
            child->containingBlock()->insertPositionedObject(child);
            if (child->hasStaticX())
                child->setStaticX(style()->direction() == LTR ? mainStart : width() - mainStart);
            if (child->hasStaticY())
                child->setStaticY(crossStart);
            continue;
        }
        if (relayoutChildren)
            child->setChildNeedsLayout(true, false);

        child->setOverrideSize(-1);
        if (isRow)
            child->calcWidth();
        child->layoutIfNeeded();

        Length basis = child->style()->flexBasis();
        int basisPx;
        if (basis.isAuto()) {
            basisPx = isRow ? child->width() : child->height();
        } else {
            basisPx = basis.isPercent() ? (int)(basis.percent() * mainAvail / 100.0) : basis.value();
            basisPx += isRow ? (child->borderLeft() + child->paddingLeft() + child->borderRight() + child->paddingRight())
                             : (child->borderTop() + child->paddingTop() + child->borderBottom() + child->paddingBottom());
            // Apply the basis size now so items without grow/shrink still honor it.
            child->setOverrideSize(basisPx);
            child->setNeedsLayout(true, false);
            child->layoutIfNeeded();
        }

        bool autoStart = isRow ? child->style()->marginLeft().isAuto() : child->style()->marginTop().isAuto();
        bool autoEnd = isRow ? child->style()->marginRight().isAuto() : child->style()->marginBottom().isAuto();

        int mainMarginStart = isRow ? child->marginLeft() : child->marginTop();
        int mainMarginEnd = isRow ? child->marginRight() : child->marginBottom();
        int mm = (autoStart ? 0 : mainMarginStart) + (autoEnd ? 0 : mainMarginEnd);
        int cm = isRow ? (child->marginTop() + child->marginBottom()) : (child->marginLeft() + child->marginRight());

        FlexItem item = { child, basisPx + mm, mm, cm,
                          child->style()->flexGrow(), child->style()->flexShrink(),
                          basisPx + mm,
                          (isRow ? child->height() : child->width()) + cm,
                          child->style()->flexOrder(), child->style()->alignSelf(),
                          autoStart, autoEnd };
        allItems.append(item);
    }

    // Stable sort by order (CSS keeps document order for equal order values)
    for (size_t i = 1; i < allItems.size(); i++) {
        FlexItem key = allItems[i];
        size_t j = i;
        while (j > 0 && allItems[j - 1].order > key.order) {
            allItems[j] = allItems[j - 1];
            j--;
        }
        allItems[j] = key;
    }

    // Break into lines
    struct FlexLine { size_t start, end; int maxCrossSize; };
    Vector<FlexLine> lines;
    size_t lineStart = 0;
    while (lineStart < allItems.size()) {
        size_t i = lineStart;
        int lineUsed = 0;
        while (i < allItems.size()) {
            if (doWrap && i > lineStart && mainAvail > 0 && lineUsed + allItems[i].baseSize > mainAvail)
                break;
            lineUsed += allItems[i].baseSize;
            i++;
        }
        FlexLine line = { lineStart, i, 0 };
        lines.append(line);
        lineStart = i;
    }

    // Process each line
    for (size_t li = 0; li < lines.size(); li++) {
        FlexLine& line = lines[li];
        int lineUsed = 0;
        float totalGrow = 0, totalShrinkScaled = 0;
        for (size_t i = line.start; i < line.end; i++) {
            lineUsed += allItems[i].baseSize;
            totalGrow += allItems[i].grow;
            int bp = allItems[i].baseSize - allItems[i].mainMargin;
            totalShrinkScaled += allItems[i].shrink * max(0, bp);
        }

        int freeSpace = mainAvail - lineUsed;

        if (freeSpace > 0 && totalGrow > 0.0f) {
            for (size_t i = line.start; i < line.end; i++) {
                if (allItems[i].grow > 0.0f)
                    resolveFlexItemMainSize(allItems[i].child, isRow,
                        allItems[i].baseSize - allItems[i].mainMargin + (int)(freeSpace * (allItems[i].grow / totalGrow)),
                        allItems[i].mainMargin, allItems[i].crossMargin, allItems[i].mainSize, allItems[i].crossSize);
            }
        } else if (freeSpace < 0 && totalShrinkScaled > 0.0f) {
            int deficit = -freeSpace;
            for (size_t i = line.start; i < line.end; i++) {
                if (allItems[i].shrink > 0.0f) {
                    int bp = allItems[i].baseSize - allItems[i].mainMargin;
                    resolveFlexItemMainSize(allItems[i].child, isRow,
                        bp - (int)(deficit * (allItems[i].shrink * bp) / totalShrinkScaled),
                        allItems[i].mainMargin, allItems[i].crossMargin, allItems[i].mainSize, allItems[i].crossSize);
                }
            }
        }

        // Clamp min/max and freeze clamped items
        bool needsRedist = false;
        int frozenSpace = 0;
        float unfrozenGrow = 0;
        for (size_t i = line.start; i < line.end; i++) {
            RenderObject* child = allItems[i].child;
            int cur = allItems[i].mainSize - allItems[i].mainMargin;
            int clamped = cur;
            if (isRow) {
                int bp = child->borderLeft() + child->paddingLeft() + child->borderRight() + child->paddingRight();
                if (child->style()->minWidth().isFixed() && child->style()->minWidth().value() > 0)
                    clamped = max(clamped, child->style()->minWidth().value() + bp);
                if (child->style()->maxWidth().isFixed() && !child->style()->maxWidth().isUndefined())
                    clamped = min(clamped, child->style()->maxWidth().value() + bp);
            } else {
                int bp = child->borderTop() + child->paddingTop() + child->borderBottom() + child->paddingBottom();
                if (child->style()->minHeight().isFixed() && child->style()->minHeight().value() > 0)
                    clamped = max(clamped, child->style()->minHeight().value() + bp);
                if (child->style()->maxHeight().isFixed() && !child->style()->maxHeight().isUndefined())
                    clamped = min(clamped, child->style()->maxHeight().value() + bp);
            }
            if (clamped != cur) {
                resolveFlexItemMainSize(child, isRow, clamped,
                    allItems[i].mainMargin, allItems[i].crossMargin, allItems[i].mainSize, allItems[i].crossSize);
                allItems[i].grow = 0;
                needsRedist = true;
                frozenSpace += allItems[i].mainSize;
            } else {
                unfrozenGrow += allItems[i].grow;
            }
        }
        if (needsRedist && unfrozenGrow > 0.0f) {
            int avail = mainAvail - frozenSpace;
            for (size_t i = line.start; i < line.end; i++) {
                if (allItems[i].grow > 0.0f)
                    resolveFlexItemMainSize(allItems[i].child, isRow,
                        (int)(avail * (allItems[i].grow / unfrozenGrow)) - allItems[i].mainMargin,
                        allItems[i].mainMargin, allItems[i].crossMargin, allItems[i].mainSize, allItems[i].crossSize);
            }
        }

        // Line cross size
        int lineCross = 0;
        for (size_t i = line.start; i < line.end; i++)
            if (allItems[i].crossSize > lineCross)
                lineCross = allItems[i].crossSize;
        line.maxCrossSize = lineCross;
    }

    // Position lines and items
    int totalCrossUsed = 0;
    for (size_t li = 0; li < lines.size(); li++)
        totalCrossUsed += lines[li].maxCrossSize;

    int crossPos = crossStart;

    // align-content: distribute cross-axis free space among lines
    const EFlexAlignContent ac = style()->alignContent();
    int crossFree = (crossContainerSize > 0 ? crossContainerSize : totalCrossUsed) - totalCrossUsed;
    if (crossFree < 0) crossFree = 0;
    int lineOffset = 0, lineGap = 0;
    int numLines = (int)lines.size();

    if (numLines > 0) {
        switch (ac) {
            case ContentFlexStart:  break;
            case ContentFlexEnd:    lineOffset = crossFree; break;
            case ContentCenter:     lineOffset = crossFree / 2; break;
            case ContentSpaceBetween:
                if (numLines > 1) lineGap = crossFree / (numLines - 1);
                break;
            case ContentSpaceAround:
                lineGap = crossFree / numLines;
                lineOffset = lineGap / 2;
                break;
            case ContentStretch:
                // Distribute extra space equally to each line
                if (numLines > 0) {
                    int extra = crossFree / numLines;
                    for (size_t li = 0; li < lines.size(); li++)
                        lines[li].maxCrossSize += extra;
                }
                break;
        }
    }
    crossPos += lineOffset;

    for (size_t lineIdx = 0; lineIdx < lines.size(); lineIdx++) {
        size_t li = (wrapMode == FlexWrapReverse) ? (lines.size() - 1 - lineIdx) : lineIdx;
        FlexLine& line = lines[li];
        int lineCross = line.maxCrossSize;
        // Single line with definite cross container: use full cross size for alignment
        if (lines.size() == 1 && crossContainerSize > 0)
            lineCross = crossContainerSize;

        int lineMainUsed = 0;
        for (size_t i = line.start; i < line.end; i++)
            lineMainUsed += allItems[i].mainSize;
        int freeSpace = mainAvail - lineMainUsed;
        if (freeSpace < 0) freeSpace = 0;

        // Count auto margins on the main axis; they absorb free space before justify.
        int autoMarginCount = 0;
        for (size_t i = line.start; i < line.end; i++) {
            if (allItems[i].autoMarginMainStart) autoMarginCount++;
            if (allItems[i].autoMarginMainEnd) autoMarginCount++;
        }
        int autoMarginSize = (autoMarginCount > 0 && freeSpace > 0) ? (freeSpace / autoMarginCount) : 0;

        int mainOffset = 0, mainGap = 0;
        int n = (int)(line.end - line.start);
        if (autoMarginCount > 0) {
            // Auto margins consume all free space; justify-content has no effect.
            freeSpace = 0;
        } else {
            switch (justify) {
                case JustifyFlexStart:  break;
                case JustifyFlexEnd:    mainOffset = freeSpace; break;
                case JustifyCenter:     mainOffset = freeSpace / 2; break;
                case JustifySpaceBetween:
                    if (n > 1) mainGap = freeSpace / (n - 1);
                    break;
                case JustifySpaceAround:
                    if (n > 0) { mainGap = freeSpace / n; mainOffset = mainGap / 2; }
                    break;
            }
        }

        int mainPos = isReverse ? (mainStart + mainAvail) : (mainStart + mainOffset);

        for (size_t i = line.start; i < line.end; i++) {
            FlexItem& item = allItems[i];
            RenderObject* child = item.child;
            int childMain = item.mainSize - item.mainMargin;
            int childCross = item.crossSize - item.crossMargin;
            int crossMarginBefore = isRow ? child->marginTop() : child->marginLeft();
            int crossMarginAfter = isRow ? child->marginBottom() : child->marginRight();

            int itemCrossPos = crossPos;
            // Resolve align-self (auto falls back to container align-items).
            EFlexAlign effAlign = align;
            switch (item.alignSelf) {
                case AlignSelfAuto: effAlign = align; break;
                case AlignSelfFlexStart: effAlign = AlignFlexStart; break;
                case AlignSelfFlexEnd: effAlign = AlignFlexEnd; break;
                case AlignSelfCenter: effAlign = AlignCenter; break;
                case AlignSelfBaseline: effAlign = AlignBaseline; break;
                case AlignSelfStretch: effAlign = AlignStretch; break;
            }
            switch (effAlign) {
                case AlignFlexStart:
                case AlignBaseline:
                    itemCrossPos += crossMarginBefore;
                    break;
                case AlignFlexEnd:
                    itemCrossPos += lineCross - childCross - crossMarginAfter;
                    break;
                case AlignCenter:
                    itemCrossPos += (lineCross - childCross - crossMarginBefore - crossMarginAfter) / 2 + crossMarginBefore;
                    break;
                case AlignStretch:
                    itemCrossPos += crossMarginBefore;
                    if (isRow && child->style()->height().isAuto()) {
                        int h = lineCross - crossMarginBefore - crossMarginAfter
                            - child->borderTop() - child->paddingTop() - child->borderBottom() - child->paddingBottom();
                        if (h > 0 && h != child->contentHeight()) {
                            child->style()->setHeight(Length(h, Fixed));
                            child->setChildNeedsLayout(true, false);
                            child->layoutIfNeeded();
                            child->style()->setHeight(Length(Auto));
                        }
                    } else if (!isRow && child->style()->width().isAuto()) {
                        int w = lineCross - crossMarginBefore - crossMarginAfter
                            - child->borderLeft() - child->paddingLeft() - child->borderRight() - child->paddingRight();
                        if (w > 0 && w != child->contentWidth()) {
                            child->setOverrideSize(w + child->borderLeft() + child->paddingLeft()
                                + child->borderRight() + child->paddingRight());
                            child->setChildNeedsLayout(true, false);
                            child->layoutIfNeeded();
                        }
                    }
                    break;
            }

            int x, y;
            if (isRow) {
                if (isReverse) {
                    mainPos -= child->marginRight() + childMain;
                    x = mainPos;
                    mainPos -= child->marginLeft() + mainGap;
                } else {
                    if (item.autoMarginMainStart) mainPos += autoMarginSize;
                    x = mainPos + (item.autoMarginMainStart ? 0 : child->marginLeft());
                    mainPos += item.mainSize + mainGap;
                    if (item.autoMarginMainEnd) mainPos += autoMarginSize;
                }
                y = itemCrossPos;
            } else {
                if (isReverse) {
                    mainPos -= child->marginBottom() + childMain;
                    y = mainPos;
                    mainPos -= child->marginTop() + mainGap;
                } else {
                    if (item.autoMarginMainStart) mainPos += autoMarginSize;
                    y = mainPos + (item.autoMarginMainStart ? 0 : child->marginTop());
                    mainPos += item.mainSize + mainGap;
                    if (item.autoMarginMainEnd) mainPos += autoMarginSize;
                }
                x = itemCrossPos;
            }
            placeChild(child, x, y);
        }
        crossPos += lineCross + lineGap;
    }

    m_flexingChildren = false;
    style()->setBoxOrient(savedOrient);

    if (isRow) {
        if (style()->height().isAuto())
            m_height = crossStart + totalCrossUsed + borderBottom() + paddingBottom();
    } else {
        if (style()->height().isAuto()) {
            int usedMain = 0;
            for (size_t i = 0; i < allItems.size(); i++)
                usedMain += allItems[i].mainSize;
            m_height = mainStart + usedMain + borderBottom() + paddingBottom();
        }
    }
    m_overflowHeight = max(m_overflowHeight, m_height);
    m_overflowWidth = max(m_overflowWidth, m_width);
}
#endif // ENABLE(MODERN_FLEXBOX)

int RenderFlexibleBox::allowedChildFlex(RenderObject* child, bool expanding, unsigned int group)
{
    if (child->isPositioned() || child->style()->boxFlex() == 0.0f || child->style()->boxFlexGroup() != group)
        return 0;
                        
    if (expanding) {
        if (isHorizontal()) {
            // FIXME: For now just handle fixed values.
            int maxW = INT_MAX;
            int w = child->overrideWidth() - (child->borderLeft() + child->borderRight() + child->paddingLeft() + child->paddingRight());
            if (!child->style()->maxWidth().isUndefined() &&
                child->style()->maxWidth().isFixed())
                maxW = child->style()->maxWidth().value();
            else if (child->style()->maxWidth().type() == Intrinsic)
                maxW = child->maxPrefWidth();
            else if (child->style()->maxWidth().type() == MinIntrinsic)
                maxW = child->minPrefWidth();
            if (maxW == INT_MAX)
                return maxW;
            return max(0, maxW - w);
        } else {
            // FIXME: For now just handle fixed values.
            int maxH = INT_MAX;
            int h = child->overrideHeight() - (child->borderTop() + child->borderBottom() + child->paddingTop() + child->paddingBottom());
            if (!child->style()->maxHeight().isUndefined() &&
                child->style()->maxHeight().isFixed())
                maxH = child->style()->maxHeight().value();
            if (maxH == INT_MAX)
                return maxH;
            return max(0, maxH - h);
        }
    }

    // FIXME: For now just handle fixed values.
    if (isHorizontal()) {
        int minW = child->minPrefWidth();
        int w = child->overrideWidth() - (child->borderLeft() + child->borderRight() + child->paddingLeft() + child->paddingRight());
        if (child->style()->minWidth().isFixed())
            minW = child->style()->minWidth().value();
        else if (child->style()->minWidth().type() == Intrinsic)
            minW = child->maxPrefWidth();
        else if (child->style()->minWidth().type() == MinIntrinsic)
            minW = child->minPrefWidth();
            
        int allowedShrinkage = min(0, minW - w);
        return allowedShrinkage;
    } else {
        if (child->style()->minHeight().isFixed()) {
            int minH = child->style()->minHeight().value();
            int h = child->overrideHeight() - (child->borderLeft() + child->borderRight() + child->paddingLeft() + child->paddingRight());
            int allowedShrinkage = min(0, minH - h);
            return allowedShrinkage;
        }
    }
    
    return 0;
}

const char *RenderFlexibleBox::renderName() const
{
    if (isFloating())
        return "RenderFlexibleBox (floating)";
    if (isPositioned())
        return "RenderFlexibleBox (positioned)";
    if (isRelPositioned())
        return "RenderFlexibleBox (relative positioned)";
    return "RenderFlexibleBox";
}

} // namespace WebCore
