/*
 * Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Alexey Proskuryakov (ap@webkit.org)
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
#include "EventHandler.h"

#include "CachedImage.h"
#include "ChromeClient.h"
#include "Cursor.h"
#include "Document.h"
#include "DragController.h"
#include "Editor.h"
#include "EventNames.h"
#include "FloatPoint.h"
#include "FocusController.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "HitTestRequest.h"
#include "HitTestResult.h"
#include "HTMLFrameSetElement.h"
#include "HTMLFrameElementBase.h"
#include "HTMLInputElement.h"
#include "HTMLNames.h"
#include "Image.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "MouseEventWithHitTestResults.h"
#include "Page.h"
#include "PlatformKeyboardEvent.h"
#include "PlatformScrollBar.h"
#include "PlatformWheelEvent.h"
#include "RenderWidget.h"
#include "SelectionController.h"
#include "Settings.h"
#include "TextEvent.h"

#if ENABLE(SVG)
#include "SVGCursorElement.h"
#include "SVGDocument.h"
#include "SVGLength.h"
#include "SVGNames.h"
#endif

namespace WebCore {

using namespace EventNames;
using namespace HTMLNames;

// The link drag hysteresis is much larger than the others because there
// needs to be enough space to cancel the link press without starting a link drag,
// and because dragging links is rare.
const int LinkDragHysteresis = 40;
const int ImageDragHysteresis = 5;
const int TextDragHysteresis = 3;
const int GeneralDragHysteresis = 3;
const double TextDragDelay = 0.15;

// Match key code of composition keydown event on windows.
// IE sends VK_PROCESSKEY which has value 229;
const int CompositionEventKeyCode = 229;

#if ENABLE(SVG)
using namespace SVGNames;
#endif

const double autoscrollInterval = 0.1;

static Frame* subframeForTargetNode(Node* node);

EventHandler::EventHandler(Frame* frame)
    : m_frame(frame)
    , m_mousePressed(false)
    , m_mouseDownMayStartSelect(false)
    , m_mouseDownMayStartDrag(false)
    , m_mouseDownWasSingleClickInSelection(false)
    , m_beganSelectingText(false)
    , m_hoverTimer(this, &EventHandler::hoverTimerFired)
    , m_autoscrollTimer(this, &EventHandler::autoscrollTimerFired)
    , m_autoscrollRenderer(0)
    , m_mouseDownMayStartAutoscroll(false)
    , m_mouseDownWasInSubframe(false)
#if ENABLE(SVG)
    , m_svgPan(false)
#endif
    , m_resizeLayer(0)
    , m_capturingMouseEventsNode(0)
    , m_clickCount(0)
    , m_mouseDownTimestamp(0)
{
}

EventHandler::~EventHandler()
{
}
    
EventHandler::EventHandlerDragState& EventHandler::dragState()
{
    static EventHandlerDragState state;
    return state;
}
    
void EventHandler::clear()
{
    m_hoverTimer.stop();
    m_resizeLayer = 0;
    m_nodeUnderMouse = 0;
    m_lastNodeUnderMouse = 0;
    m_lastMouseMoveEventSubframe = 0;
    m_lastScrollbarUnderMouse = 0;
    m_clickCount = 0;
    m_clickNode = 0;
    m_frameSetBeingResized = 0;
    m_dragTarget = 0;
    m_currentMousePosition = IntPoint();
    m_mousePressNode = 0;
    m_mousePressed = false;
    m_capturingMouseEventsNode = 0;
}

void EventHandler::selectClosestWordFromMouseEvent(const MouseEventWithHitTestResults& result)
{
    Node* innerNode = result.targetNode();
    Selection newSelection;

    if (innerNode && innerNode->renderer() && m_mouseDownMayStartSelect) {
        VisiblePosition pos(innerNode->renderer()->positionForPoint(result.localPoint()));
        if (pos.isNotNull()) {
            newSelection = Selection(pos);
            newSelection.expandUsingGranularity(WordGranularity);
        }
    
        if (newSelection.isRange()) {
            m_frame->setSelectionGranularity(WordGranularity);
            m_beganSelectingText = true;
        }
        
        if (m_frame->shouldChangeSelection(newSelection))
            m_frame->selectionController()->setSelection(newSelection);
    }
}

bool EventHandler::handleMousePressEventDoubleClick(const MouseEventWithHitTestResults& event)
{
    if (event.event().button() != LeftButton)
        return false;

    if (m_frame->selectionController()->isRange())
        // A double-click when range is already selected
        // should not change the selection.  So, do not call
        // selectClosestWordFromMouseEvent, but do set
        // m_beganSelectingText to prevent handleMouseReleaseEvent
        // from setting caret selection.
        m_beganSelectingText = true;
    else
        selectClosestWordFromMouseEvent(event);

    return true;
}

bool EventHandler::handleMousePressEventTripleClick(const MouseEventWithHitTestResults& event)
{
    if (event.event().button() != LeftButton)
        return false;
    
    Node* innerNode = event.targetNode();
    if (!(innerNode && innerNode->renderer() && m_mouseDownMayStartSelect))
        return false;

    Selection newSelection;
    VisiblePosition pos(innerNode->renderer()->positionForPoint(event.localPoint()));
    if (pos.isNotNull()) {
        newSelection = Selection(pos);
        newSelection.expandUsingGranularity(ParagraphGranularity);
    }
    if (newSelection.isRange()) {
        m_frame->setSelectionGranularity(ParagraphGranularity);
        m_beganSelectingText = true;
    }
    
    if (m_frame->shouldChangeSelection(newSelection))
        m_frame->selectionController()->setSelection(newSelection);

    return true;
}

bool EventHandler::handleMousePressEventSingleClick(const MouseEventWithHitTestResults& event)
{
    if (event.event().button() != LeftButton)
        return false;
    
    Node* innerNode = event.targetNode();
    if (!(innerNode && innerNode->renderer() && m_mouseDownMayStartSelect))
        return false;

    // Extend the selection if the Shift key is down, unless the click is in a link.
    bool extendSelection = event.event().shiftKey() && !event.isOverLink();

    // Don't restart the selection when the mouse is pressed on an
    // existing selection so we can allow for text dragging.
    IntPoint vPoint = m_frame->view()->windowToContents(event.event().pos());
    if (!extendSelection && m_frame->selectionController()->contains(vPoint)) {
        m_mouseDownWasSingleClickInSelection = true;
        return false;
    }

    VisiblePosition visiblePos(innerNode->renderer()->positionForPoint(event.localPoint()));
    if (visiblePos.isNull())
        visiblePos = VisiblePosition(innerNode, innerNode->caretMinOffset(), DOWNSTREAM);
    Position pos = visiblePos.deepEquivalent();
    
    Selection newSelection = m_frame->selectionController()->selection();
    if (extendSelection && newSelection.isCaretOrRange()) {
        m_frame->selectionController()->setLastChangeWasHorizontalExtension(false);
        
        // See <rdar://problem/3668157> REGRESSION (Mail): shift-click deselects when selection 
        // was created right-to-left
        Position start = newSelection.start();
        Position end = newSelection.end();
        short before = Range::compareBoundaryPoints(pos.node(), pos.offset(), start.node(), start.offset());
        if (before <= 0)
            newSelection = Selection(pos, end);
        else
            newSelection = Selection(start, pos);

        if (m_frame->selectionGranularity() != CharacterGranularity)
            newSelection.expandUsingGranularity(m_frame->selectionGranularity());
        m_beganSelectingText = true;
    } else {
        newSelection = Selection(visiblePos);
        m_frame->setSelectionGranularity(CharacterGranularity);
    }
    
    if (m_frame->shouldChangeSelection(newSelection))
        m_frame->selectionController()->setSelection(newSelection);

    return true;
}

bool EventHandler::handleMousePressEvent(const MouseEventWithHitTestResults& event)
{
    bool singleClick = event.event().clickCount() <= 1;

    // If we got the event back, that must mean it wasn't prevented,
    // so it's allowed to start a drag or selection.
    m_mouseDownMayStartSelect = canMouseDownStartSelect(event.targetNode());
    
    // Careful that the drag starting logic stays in sync with eventMayStartDrag()
    m_mouseDownMayStartDrag = singleClick;

    m_mouseDownWasSingleClickInSelection = false;

    if (passWidgetMouseDownEventToWidget(event))
        return true;

#if ENABLE(SVG)
    if (m_frame->document()->isSVGDocument() &&
       static_cast<SVGDocument*>(m_frame->document())->zoomAndPanEnabled()) {
        if (event.event().shiftKey() && singleClick) {
            m_svgPan = true;
            static_cast<SVGDocument*>(m_frame->document())->startPan(event.event().pos());
            return true;
        }
    }
#endif

    // We don't do this at the start of mouse down handling,
    // because we don't want to do it until we know we didn't hit a widget.
    if (singleClick)
        focusDocumentView();

    Node* innerNode = event.targetNode();

    m_mousePressNode = innerNode;
    m_dragStartPos = event.event().pos();

    bool swallowEvent = false;
    if (event.event().button() == LeftButton || event.event().button() == MiddleButton) {
        m_frame->selectionController()->setCaretBlinkingSuspended(true);
        m_mousePressed = true;
        m_beganSelectingText = false;

        if (event.event().clickCount() == 2)
            swallowEvent = handleMousePressEventDoubleClick(event);
        else if (event.event().clickCount() >= 3)
            swallowEvent = handleMousePressEventTripleClick(event);
        else
            swallowEvent = handleMousePressEventSingleClick(event);
    }
    
   m_mouseDownMayStartAutoscroll = m_mouseDownMayStartSelect || 
        (m_mousePressNode && m_mousePressNode->renderer() && m_mousePressNode->renderer()->shouldAutoscroll());

   return swallowEvent;
}

bool EventHandler::handleMouseDraggedEvent(const MouseEventWithHitTestResults& event)
{
    if (handleDrag(event))
        return true;

    if (!m_mousePressed)
        return false;

    Node* targetNode = event.targetNode();
    if (event.event().button() != LeftButton || !targetNode || !targetNode->renderer())
        return false;

    m_mouseDownMayStartDrag = false;

    if (m_mouseDownMayStartAutoscroll) {            
        // If the selection is contained in a layer that can scroll, that layer should handle the autoscroll
        // Otherwise, let the bridge handle it so the view can scroll itself.
        RenderObject* renderer = targetNode->renderer();
        while (renderer && !renderer->shouldAutoscroll())
            renderer = renderer->parent();
        if (renderer)
            handleAutoscroll(renderer);
    }
    
    updateSelectionForMouseDrag(targetNode, event.localPoint());
    return true;
}
    
bool EventHandler::eventMayStartDrag(const PlatformMouseEvent& event) const
{
    // This is a pre-flight check of whether the event might lead to a drag being started.  Be careful
    // that its logic needs to stay in sync with handleMouseMoveEvent() and the way we setMouseDownMayStartDrag
    // in handleMousePressEvent
    
    if (!m_frame->renderer() || !m_frame->renderer()->hasLayer()
        || event.button() != LeftButton || event.clickCount() != 1)
        return false;
    
    bool DHTMLFlag;
    bool UAFlag;
    allowDHTMLDrag(DHTMLFlag, UAFlag);
    if (!DHTMLFlag && !UAFlag)
        return false;
    
    HitTestRequest request(true, false);
    HitTestResult result(m_frame->view()->windowToContents(event.pos()));
    m_frame->renderer()->layer()->hitTest(request, result);
    bool srcIsDHTML;
    return result.innerNode() && result.innerNode()->renderer()->draggableNode(DHTMLFlag, UAFlag, result.point().x(), result.point().y(), srcIsDHTML);
}

void EventHandler::updateSelectionForMouseDrag()
{
    FrameView* view = m_frame->view();
    if (!view)
        return;
    RenderObject* renderer = m_frame->renderer();
    if (!renderer)
        return;
    RenderLayer* layer = renderer->layer();
    if (!layer)
        return;

    HitTestResult result(view->windowToContents(m_currentMousePosition));
    layer->hitTest(HitTestRequest(true, true, true), result);
    updateSelectionForMouseDrag(result.innerNode(), result.localPoint());
}

void EventHandler::updateSelectionForMouseDrag(Node* targetNode, const IntPoint& localPoint)
{
    if (!m_mouseDownMayStartSelect)
        return;

    if (!targetNode)
        return;

    RenderObject* targetRenderer = targetNode->renderer();
    if (!targetRenderer)
        return;
        
    if (!canMouseDragExtendSelect(targetNode))
        return;

    VisiblePosition targetPosition(targetRenderer->positionForPoint(localPoint));

    // Don't modify the selection if we're not on a node.
    if (targetPosition.isNull())
        return;

    // Restart the selection if this is the first mouse move. This work is usually
    // done in handleMousePressEvent, but not if the mouse press was on an existing selection.
    Selection newSelection = m_frame->selectionController()->selection();

#if ENABLE(SVG)
    // Special case to limit selection to the containing block for SVG text.
    // FIXME: Isn't there a better non-SVG-specific way to do this?
    if (Node* selectionBaseNode = newSelection.base().node())
        if (RenderObject* selectionBaseRenderer = selectionBaseNode->renderer())
            if (selectionBaseRenderer->isSVGText())
                if (targetNode->renderer()->containingBlock() != selectionBaseRenderer->containingBlock())
                    return;
#endif

    if (!m_beganSelectingText) {
        m_beganSelectingText = true;
        newSelection = Selection(targetPosition);
    }

    newSelection.setExtent(targetPosition);
    if (m_frame->selectionGranularity() != CharacterGranularity)
        newSelection.expandUsingGranularity(m_frame->selectionGranularity());

    if (m_frame->shouldChangeSelection(newSelection)) {
        m_frame->selectionController()->setLastChangeWasHorizontalExtension(false);
        m_frame->selectionController()->setSelection(newSelection);
    }
}
    
bool EventHandler::handleMouseUp(const MouseEventWithHitTestResults& event)
{
    if (eventLoopHandleMouseUp(event))
        return true;
    
    // If this was the first click in the window, we don't even want to clear the selection.
    // This case occurs when the user clicks on a draggable element, since we have to process
    // the mouse down and drag events to see if we might start a drag.  For other first clicks
    // in a window, we just don't acceptFirstMouse, and the whole down-drag-up sequence gets
    // ignored upstream of this layer.
    return eventActivatedView(event.event());
}    

bool EventHandler::handleMouseReleaseEvent(const MouseEventWithHitTestResults& event)
{
    stopAutoscrollTimer();

    if (handleMouseUp(event))
        return true;

    // Used to prevent mouseMoveEvent from initiating a drag before
    // the mouse is pressed again.
    m_frame->selectionController()->setCaretBlinkingSuspended(false);
    m_mousePressed = false;
    m_mouseDownMayStartDrag = false;
    m_mouseDownMayStartSelect = false;
    m_mouseDownMayStartAutoscroll = false;
    m_mouseDownWasInSubframe = false;
  
    bool handled = false;

    // Clear the selection if the mouse didn't move after the last mouse press.
    // We do this so when clicking on the selection, the selection goes away.
    // However, if we are editing, place the caret.
    if (m_mouseDownWasSingleClickInSelection && !m_beganSelectingText
            && m_dragStartPos == event.event().pos()
            && m_frame->selectionController()->isRange()) {
        Selection newSelection;
        Node *node = event.targetNode();
        if (node && node->isContentEditable() && node->renderer()) {
            VisiblePosition pos = node->renderer()->positionForPoint(event.localPoint());
            newSelection = Selection(pos);
        }
        if (m_frame->shouldChangeSelection(newSelection))
            m_frame->selectionController()->setSelection(newSelection);

        handled = true;
    }

    m_frame->notifyRendererOfSelectionChange(true);

    m_frame->selectionController()->selectFrameElementInParentIfFullySelected();

    return handled;
}

void EventHandler::handleAutoscroll(RenderObject* renderer)
{
    if (m_autoscrollTimer.isActive())
        return;
    setAutoscrollRenderer(renderer);
    startAutoscrollTimer();
}

void EventHandler::autoscrollTimerFired(Timer<EventHandler>*)
{
    if (!m_mousePressed) {
        stopAutoscrollTimer();
        return;
    }
    if (RenderObject* r = autoscrollRenderer())
        r->autoscroll();
}

RenderObject* EventHandler::autoscrollRenderer() const
{
    return m_autoscrollRenderer;
}

void EventHandler::setAutoscrollRenderer(RenderObject* renderer)
{
    m_autoscrollRenderer = renderer;
}

void EventHandler::allowDHTMLDrag(bool& flagDHTML, bool& flagUA) const
{
    if (!m_frame || !m_frame->document()) {
        flagDHTML = false;
        flagUA = false;
    }
    
    unsigned mask = m_frame->page()->dragController()->delegateDragSourceAction(m_frame->view()->contentsToWindow(m_mouseDownPos));
    flagDHTML = (mask & DragSourceActionDHTML) != DragSourceActionNone;
    flagUA = ((mask & DragSourceActionImage) || (mask & DragSourceActionLink) || (mask & DragSourceActionSelection));
}
    
HitTestResult EventHandler::hitTestResultAtPoint(const IntPoint& point, bool allowShadowContent)
{
    HitTestResult result(point);
    if (!m_frame->renderer())
        return result;
    m_frame->renderer()->layer()->hitTest(HitTestRequest(true, true), result);

    IntPoint widgetPoint(point);
    while (true) {
        Node* n = result.innerNode();
        if (!n || !n->renderer() || !n->renderer()->isWidget())
            break;
        Widget* widget = static_cast<RenderWidget*>(n->renderer())->widget();
        if (!widget || !widget->isFrameView())
            break;
        Frame* frame = static_cast<HTMLFrameElementBase*>(n)->contentFrame();
        if (!frame || !frame->renderer())
            break;
        int absX, absY;
        n->renderer()->absolutePosition(absX, absY, true);
        FrameView* view = static_cast<FrameView*>(widget);
        widgetPoint.move(view->contentsX() - absX, view->contentsY() - absY);
        HitTestResult widgetHitTestResult(widgetPoint);
        frame->renderer()->layer()->hitTest(HitTestRequest(true, true), widgetHitTestResult);
        result = widgetHitTestResult;
    }

    if (!allowShadowContent)
        result.setToNonShadowAncestor();

    return result;
}


void EventHandler::startAutoscrollTimer()
{
    m_autoscrollTimer.startRepeating(autoscrollInterval);
}

void EventHandler::stopAutoscrollTimer(bool rendererIsBeingDestroyed)
{
    if (m_mouseDownWasInSubframe) {
        if (Frame* subframe = subframeForTargetNode(m_mousePressNode.get()))
            subframe->eventHandler()->stopAutoscrollTimer(rendererIsBeingDestroyed);
        return;
    }

    if (!rendererIsBeingDestroyed && autoscrollRenderer())
        autoscrollRenderer()->stopAutoscroll();
    setAutoscrollRenderer(0);
    m_autoscrollTimer.stop();
}

Node* EventHandler::mousePressNode() const
{
    return m_mousePressNode.get();
}

void EventHandler::setMousePressNode(PassRefPtr<Node> node)
{
    m_mousePressNode = node;
}

bool EventHandler::scrollOverflow(ScrollDirection direction, ScrollGranularity granularity)
{
    if (!m_frame->document())
        return false;
    
    Node* node = m_frame->document()->focusedNode();
    if (!node)
        node = m_mousePressNode.get();
    
    if (node) {
        RenderObject *r = node->renderer();
        if (r && !r->isListBox())
            return r->scroll(direction, granularity);
    }

    return false;
}

IntPoint EventHandler::currentMousePosition() const
{
    return m_currentMousePosition;
}

Frame* subframeForTargetNode(Node* node)
{
    if (!node)
        return 0;

    RenderObject* renderer = node->renderer();
    if (!renderer || !renderer->isWidget())
        return 0;

    Widget* widget = static_cast<RenderWidget*>(renderer)->widget();
    if (!widget || !widget->isFrameView())
        return 0;

    return static_cast<FrameView*>(widget)->frame();
}

static bool isSubmitImage(Node* node)
{
    return node && node->hasTagName(inputTag)
        && static_cast<HTMLInputElement*>(node)->inputType() == HTMLInputElement::IMAGE;
}

// Returns true if the node's editable block is not current focused for editing
static bool nodeIsNotBeingEdited(Node* node, Frame* frame)
{
    return frame->selectionController()->rootEditableElement() != node->rootEditableElement();
}

Cursor EventHandler::selectCursor(const MouseEventWithHitTestResults& event, PlatformScrollbar* scrollbar)
{
    // During selection, use an I-beam no matter what we're over.
    // If you're capturing mouse events for a particular node, don't treat this as a selection.
    if (m_mousePressed && m_mouseDownMayStartSelect && m_frame->selectionController()->isCaretOrRange() && !m_capturingMouseEventsNode)
        return iBeamCursor();

    Node* node = event.targetNode();
    RenderObject* renderer = node ? node->renderer() : 0;
    RenderStyle* style = renderer ? renderer->style() : 0;

    if (style && style->cursors()) {
        const CursorList* cursors = style->cursors();
        for (unsigned i = 0; i < cursors->size(); ++i) {
            CachedImage* cimage = (*cursors)[i].cursorImage;
            IntPoint hotSpot = (*cursors)[i].hotSpot;
#if ENABLE(SVG)
            if (!cimage) {
                Element* e = node->document()->getElementById((*cursors)[i].cursorFragmentId);
                if (e && e->hasTagName(cursorTag)) {
                    hotSpot.setX(int(static_cast<SVGCursorElement*>(e)->x().value()));
                    hotSpot.setY(int(static_cast<SVGCursorElement*>(e)->y().value()));
                    cimage = static_cast<SVGCursorElement*>(e)->cachedImage();
                }
            }
#endif
            if (!cimage)
                continue;
            if (cimage->image()->isNull())
                break;
            if (!cimage->errorOccurred())
                return Cursor(cimage->image(), hotSpot);
        }
    }

    switch (style ? style->cursor() : CURSOR_AUTO) {
        case CURSOR_AUTO: {
            bool editable = (node && node->isContentEditable());
            bool editableLinkEnabled = false;

            // If the link is editable, then we need to check the settings to see whether or not the link should be followed
            if (editable) {
                ASSERT(m_frame->settings());
                switch(m_frame->settings()->editableLinkBehavior()) {
                    default:
                    case EditableLinkDefaultBehavior:
                    case EditableLinkAlwaysLive:
                        editableLinkEnabled = true;
                        break;

                    case EditableLinkNeverLive:
                        editableLinkEnabled = false;
                        break;

                    case EditableLinkLiveWhenNotFocused:
                        editableLinkEnabled = nodeIsNotBeingEdited(node, m_frame) || event.event().shiftKey();
                        break;
                    
                    case EditableLinkOnlyLiveWithShiftKey:
                        editableLinkEnabled = event.event().shiftKey();
                        break;
                }
            }
            
            if ((event.isOverLink() || isSubmitImage(node)) && (!editable || editableLinkEnabled))
                return handCursor();
            RenderLayer* layer = renderer ? renderer->enclosingLayer() : 0;
            bool inResizer = false;
            if (m_frame->view() && layer && layer->isPointInResizeControl(m_frame->view()->windowToContents(event.event().pos())))
                inResizer = true;
            if ((editable || (renderer && renderer->isText() && node->canStartSelection())) && !inResizer && !scrollbar)
                return iBeamCursor();
            return pointerCursor();
        }
        case CURSOR_CROSS:
            return crossCursor();
        case CURSOR_POINTER:
            return handCursor();
        case CURSOR_MOVE:
            return moveCursor();
        case CURSOR_ALL_SCROLL:
            return moveCursor();
        case CURSOR_E_RESIZE:
            return eastResizeCursor();
        case CURSOR_W_RESIZE:
            return westResizeCursor();
        case CURSOR_N_RESIZE:
            return northResizeCursor();
        case CURSOR_S_RESIZE:
            return southResizeCursor();
        case CURSOR_NE_RESIZE:
            return northEastResizeCursor();
        case CURSOR_SW_RESIZE:
            return southWestResizeCursor();
        case CURSOR_NW_RESIZE:
            return northWestResizeCursor();
        case CURSOR_SE_RESIZE:
            return southEastResizeCursor();
        case CURSOR_NS_RESIZE:
            return northSouthResizeCursor();
        case CURSOR_EW_RESIZE:
            return eastWestResizeCursor();
        case CURSOR_NESW_RESIZE:
            return northEastSouthWestResizeCursor();
        case CURSOR_NWSE_RESIZE:
            return northWestSouthEastResizeCursor();
        case CURSOR_COL_RESIZE:
            return columnResizeCursor();
        case CURSOR_ROW_RESIZE:
            return rowResizeCursor();
        case CURSOR_TEXT:
            return iBeamCursor();
        case CURSOR_WAIT:
            return waitCursor();
        case CURSOR_HELP:
            return helpCursor();
        case CURSOR_VERTICAL_TEXT:
            return verticalTextCursor();
        case CURSOR_CELL:
            return cellCursor();
        case CURSOR_CONTEXT_MENU:
            return contextMenuCursor();
        case CURSOR_PROGRESS:
            return progressCursor();
        case CURSOR_NO_DROP:
            return noDropCursor();
        case CURSOR_ALIAS:
            return aliasCursor();
        case CURSOR_COPY:
            return copyCursor();
        case CURSOR_NONE:
            return noneCursor();
        case CURSOR_NOT_ALLOWED:
            return notAllowedCursor();
        case CURSOR_DEFAULT:
            return pointerCursor();
        case CURSOR_WEBKIT_ZOOM_IN:
            return zoomInCursor();
        case CURSOR_WEBKIT_ZOOM_OUT:
            return zoomOutCursor();
    }
    return pointerCursor();
}

bool EventHandler::handleMousePressEvent(const PlatformMouseEvent& mouseEvent)
{
    if (!m_frame->document())
        return false;

    RefPtr<FrameView> protector(m_frame->view());

    m_mousePressed = true;
    m_currentMousePosition = mouseEvent.pos();
    m_mouseDownTimestamp = mouseEvent.timestamp();
    m_mouseDownMayStartDrag = false;
    m_mouseDownMayStartSelect = false;
    m_mouseDownMayStartAutoscroll = false;
    m_mouseDownPos = m_frame->view()->windowToContents(mouseEvent.pos());
    m_mouseDownWasInSubframe = false;
    
    MouseEventWithHitTestResults mev = prepareMouseEvent(HitTestRequest(false, true), mouseEvent);

    if (!mev.targetNode()) {
        invalidateClick();
        return false;
    }

    m_mousePressNode = mev.targetNode();

    Frame* subframe = subframeForTargetNode(mev.targetNode());
    if (subframe && passMousePressEventToSubframe(mev, subframe)) {
        // Start capturing future events for this frame.  We only do this if we didn't clear
        // the m_mousePressed flag, which may happen if an AppKit widget entered a modal event loop.
        if (m_mousePressed)
            m_capturingMouseEventsNode = mev.targetNode();
        invalidateClick();
        return true;
    }

    m_clickCount = mouseEvent.clickCount();
    m_clickNode = mev.targetNode();
    
    RenderLayer* layer = m_clickNode->renderer() ? m_clickNode->renderer()->enclosingLayer() : 0;
    IntPoint p = m_frame->view()->windowToContents(mouseEvent.pos());
    if (layer && layer->isPointInResizeControl(p)) {
        layer->setInResizeMode(true);
        m_resizeLayer = layer;
        m_offsetFromResizeCorner = layer->offsetFromResizeCorner(p);
        invalidateClick();
        return true;
    }

    bool swallowEvent = dispatchMouseEvent(mousedownEvent, mev.targetNode(), true, m_clickCount, mouseEvent, true);

    // If the hit testing originally determined the event was in a scrollbar, refetch the MouseEventWithHitTestResults
    // in case the scrollbar widget was destroyed when the mouse event was handled.
    if (mev.scrollbar())
        mev = prepareMouseEvent(HitTestRequest(true, true), mouseEvent);

    if (swallowEvent) {
        // scrollbars should get events anyway, even disabled controls might be scrollable
        if (mev.scrollbar()) {
            passMousePressEventToScrollbar(mev, mev.scrollbar());
        }
        else {
            PlatformScrollbar* scrollbar = m_frame->view()->scrollbarUnderMouse(mouseEvent);
            if (scrollbar)
                passMousePressEventToScrollbar(mev, scrollbar);
        }
    } else {
        // Refetch the event target node if it currently is the shadow node inside an <input> element.
        // If a mouse event handler changes the input element type to one that has a widget associated,
        // we'd like to EventHandler::handleMousePressEvent to pass the event to the widget and thus the
        // event target node can't still be the shadow node.
        if (mev.targetNode()->isShadowNode() && mev.targetNode()->shadowParentNode()->hasTagName(inputTag))
            mev = prepareMouseEvent(HitTestRequest(true, true), mouseEvent);

        PlatformScrollbar* scrollbar = m_frame->view()->scrollbarUnderMouse(mouseEvent);
        if (!scrollbar)
            scrollbar = mev.scrollbar();
        if (scrollbar && passMousePressEventToScrollbar(mev, scrollbar))
            swallowEvent = true;
        else
            swallowEvent = handleMousePressEvent(mev);
    }

    return swallowEvent;
}

// This method only exists for platforms that don't know how to deliver 
bool EventHandler::handleMouseDoubleClickEvent(const PlatformMouseEvent& mouseEvent)
{
    if (!m_frame->document())
        return false;

    RefPtr<FrameView> protector(m_frame->view());

    // We get this instead of a second mouse-up 
    m_mousePressed = false;
    m_currentMousePosition = mouseEvent.pos();

    MouseEventWithHitTestResults mev = prepareMouseEvent(HitTestRequest(false, true), mouseEvent);
    Frame* subframe = subframeForTargetNode(mev.targetNode());
    if (subframe && passMousePressEventToSubframe(mev, subframe)) {
        m_capturingMouseEventsNode = 0;
        return true;
    }

    m_clickCount = mouseEvent.clickCount();
    bool swallowMouseUpEvent = dispatchMouseEvent(mouseupEvent, mev.targetNode(), true, m_clickCount, mouseEvent, false);

    bool swallowClickEvent = false;
    // Don't ever dispatch click events for right clicks
    if (mouseEvent.button() != RightButton && mev.targetNode() == m_clickNode)
        swallowClickEvent = dispatchMouseEvent(clickEvent, mev.targetNode(), true, m_clickCount, mouseEvent, true);

    bool swallowMouseReleaseEvent = false;
    if (!swallowMouseUpEvent)
        swallowMouseReleaseEvent = handleMouseReleaseEvent(mev);

    invalidateClick();

    return swallowMouseUpEvent || swallowClickEvent || swallowMouseReleaseEvent;
}

bool EventHandler::handleMouseMoveEvent(const PlatformMouseEvent& event)
{
    HitTestResult hoveredNode = HitTestResult(event.pos());
    bool result = handleMouseMoveEvent(event, &hoveredNode);

    Page* page = m_frame->page();
    if (!page)
        return result;

    hoveredNode.setToNonShadowAncestor();
    page->chrome()->mouseDidMoveOverElement(hoveredNode, event.modifierFlags());
    page->chrome()->setToolTip(hoveredNode, event.pos());
    return result;
}

bool EventHandler::handleMouseMoveEvent(const PlatformMouseEvent& mouseEvent, HitTestResult* hoveredNode)
{
    // in Radar 3703768 we saw frequent crashes apparently due to the
    // part being null here, which seems impossible, so check for nil
    // but also assert so that we can try to figure this out in debug
    // builds, if it happens.
    ASSERT(m_frame);
    if (!m_frame || !m_frame->document())
        return false;

    RefPtr<FrameView> protector(m_frame->view());
    m_currentMousePosition = mouseEvent.pos();

    if (m_hoverTimer.isActive())
        m_hoverTimer.stop();

#if ENABLE(SVG)
    if (m_svgPan) {
        static_cast<SVGDocument*>(m_frame->document())->updatePan(m_currentMousePosition);
        return true;
    }
#endif

    if (m_frameSetBeingResized)
        return dispatchMouseEvent(mousemoveEvent, m_frameSetBeingResized.get(), false, 0, mouseEvent, false);

    // Send events right to a scrollbar if the mouse is pressed.
    if (m_lastScrollbarUnderMouse && m_mousePressed)
        return m_lastScrollbarUnderMouse->handleMouseMoveEvent(mouseEvent);

    // Treat mouse move events while the mouse is pressed as "read-only" in prepareMouseEvent
    // if we are allowed to select.
    // This means that :hover and :active freeze in the state they were in when the mouse
    // was pressed, rather than updating for nodes the mouse moves over as you hold the mouse down.
    HitTestRequest request(m_mousePressed && m_mouseDownMayStartSelect, m_mousePressed, true);
    MouseEventWithHitTestResults mev = prepareMouseEvent(request, mouseEvent);
    if (hoveredNode)
        *hoveredNode = mev.hitTestResult();

    PlatformScrollbar* scrollbar = 0;

    if (m_resizeLayer && m_resizeLayer->inResizeMode())
        m_resizeLayer->resize(mouseEvent, m_offsetFromResizeCorner);
    else {
        if (m_frame->view())
            scrollbar = m_frame->view()->scrollbarUnderMouse(mouseEvent);

        if (!scrollbar)
            scrollbar = mev.scrollbar();

        if (m_lastScrollbarUnderMouse != scrollbar) {
            // Send mouse exited to the old scrollbar.
            if (m_lastScrollbarUnderMouse)
                m_lastScrollbarUnderMouse->handleMouseOutEvent(mouseEvent);
            m_lastScrollbarUnderMouse = scrollbar;
        }
    }

    bool swallowEvent = false;
    Node* targetNode = m_capturingMouseEventsNode ? m_capturingMouseEventsNode.get() : mev.targetNode();
    RefPtr<Frame> newSubframe = subframeForTargetNode(targetNode);
    
    // We want mouseouts to happen first, from the inside out.  First send a move event to the last subframe so that it will fire mouseouts.
    if (m_lastMouseMoveEventSubframe && m_lastMouseMoveEventSubframe->tree()->isDescendantOf(m_frame) && m_lastMouseMoveEventSubframe != newSubframe)
        passMouseMoveEventToSubframe(mev, m_lastMouseMoveEventSubframe.get());

    if (newSubframe) {
        // Update over/out state before passing the event to the subframe.
        updateMouseEventTargetNode(mev.targetNode(), mouseEvent, true);
        swallowEvent |= passMouseMoveEventToSubframe(mev, newSubframe.get(), hoveredNode);
    } else {
        if (scrollbar && !m_mousePressed)
            scrollbar->handleMouseMoveEvent(mouseEvent); // Handle hover effects on platforms that support visual feedback on scrollbar hovering.
        if ((!m_resizeLayer || !m_resizeLayer->inResizeMode()) && m_frame->view())
            m_frame->view()->setCursor(selectCursor(mev, scrollbar));
    }
    
    m_lastMouseMoveEventSubframe = newSubframe;

    if (swallowEvent)
        return true;
    
    swallowEvent = dispatchMouseEvent(mousemoveEvent, mev.targetNode(), false, 0, mouseEvent, true);
    if (!swallowEvent)
        swallowEvent = handleMouseDraggedEvent(mev);

    return swallowEvent;
}

void EventHandler::invalidateClick()
{
    m_clickCount = 0;
    m_clickNode = 0;
}

bool EventHandler::handleMouseReleaseEvent(const PlatformMouseEvent& mouseEvent)
{
    if (!m_frame->document())
        return false;

    RefPtr<FrameView> protector(m_frame->view());

    m_mousePressed = false;
    m_currentMousePosition = mouseEvent.pos();

#if ENABLE(SVG)
    if (m_svgPan) {
        m_svgPan = false;
        static_cast<SVGDocument*>(m_frame->document())->updatePan(m_currentMousePosition);
        return true;
    }
#endif

    if (m_frameSetBeingResized)
        return dispatchMouseEvent(mouseupEvent, m_frameSetBeingResized.get(), true, m_clickCount, mouseEvent, false);

    if (m_lastScrollbarUnderMouse) {
        invalidateClick();
        return m_lastScrollbarUnderMouse->handleMouseReleaseEvent(mouseEvent);
    }

    MouseEventWithHitTestResults mev = prepareMouseEvent(HitTestRequest(false, false, false, true), mouseEvent);
    Node* targetNode = m_capturingMouseEventsNode.get() ? m_capturingMouseEventsNode.get() : mev.targetNode();
    Frame* subframe = subframeForTargetNode(targetNode);
    if (subframe && passMouseReleaseEventToSubframe(mev, subframe)) {
        m_capturingMouseEventsNode = 0;
        return true;
    }

    bool swallowMouseUpEvent = dispatchMouseEvent(mouseupEvent, mev.targetNode(), true, m_clickCount, mouseEvent, false);

    // Don't ever dispatch click events for right clicks
    bool swallowClickEvent = false;
    if (m_clickCount > 0 && mouseEvent.button() != RightButton && mev.targetNode() == m_clickNode)
        swallowClickEvent = dispatchMouseEvent(clickEvent, mev.targetNode(), true, m_clickCount, mouseEvent, true);

    if (m_resizeLayer) {
        m_resizeLayer->setInResizeMode(false);
        m_resizeLayer = 0;
    }

    bool swallowMouseReleaseEvent = false;
    if (!swallowMouseUpEvent)
        swallowMouseReleaseEvent = handleMouseReleaseEvent(mev);

    invalidateClick();

    return swallowMouseUpEvent || swallowClickEvent || swallowMouseReleaseEvent;
}

bool EventHandler::dispatchDragEvent(const AtomicString& eventType, Node* dragTarget, const PlatformMouseEvent& event, Clipboard* clipboard)
{
    IntPoint contentsPos = m_frame->view()->windowToContents(event.pos());
    
    RefPtr<MouseEvent> me = new MouseEvent(eventType,
        true, true, m_frame->document()->defaultView(),
        0, event.globalX(), event.globalY(), contentsPos.x(), contentsPos.y(),
        event.ctrlKey(), event.altKey(), event.shiftKey(), event.metaKey(),
        0, 0, clipboard);

    ExceptionCode ec = 0;
    EventTargetNodeCast(dragTarget)->dispatchEvent(me.get(), ec, true);
    return me->defaultPrevented();
}

bool EventHandler::updateDragAndDrop(const PlatformMouseEvent& event, Clipboard* clipboard)
{
    bool accept = false;

    if (!m_frame->document())
        return false;

    if (!m_frame->view())
        return false;
    
    MouseEventWithHitTestResults mev = prepareMouseEvent(HitTestRequest(true, false), event);

    // Drag events should never go to text nodes (following IE, and proper mouseover/out dispatch)
    Node* newTarget = mev.targetNode();
    if (newTarget && newTarget->isTextNode())
        newTarget = newTarget->parentNode();
    if (newTarget)
        newTarget = newTarget->shadowAncestorNode();

    if (m_dragTarget != newTarget) {
        // FIXME: this ordering was explicitly chosen to match WinIE. However,
        // it is sometimes incorrect when dragging within subframes, as seen with
        // LayoutTests/fast/events/drag-in-frames.html.
        if (newTarget) {
            if (newTarget->hasTagName(frameTag) || newTarget->hasTagName(iframeTag))
                accept = static_cast<HTMLFrameElementBase*>(newTarget)->contentFrame()->eventHandler()->updateDragAndDrop(event, clipboard);
            else
                accept = dispatchDragEvent(dragenterEvent, newTarget, event, clipboard);
		}
        
        if (m_dragTarget) {
            Frame* frame = (m_dragTarget->hasTagName(frameTag) || m_dragTarget->hasTagName(iframeTag)) 
                            ? static_cast<HTMLFrameElementBase*>(m_dragTarget.get())->contentFrame() : 0;
            if (frame)
                accept = frame->eventHandler()->updateDragAndDrop(event, clipboard);
            else
                dispatchDragEvent(dragleaveEvent, m_dragTarget.get(), event, clipboard);
        }
    } else {
        if (newTarget) {
            if (newTarget->hasTagName(frameTag) || newTarget->hasTagName(iframeTag))
                accept = static_cast<HTMLFrameElementBase*>(newTarget)->contentFrame()->eventHandler()->updateDragAndDrop(event, clipboard);
            else
                accept = dispatchDragEvent(dragoverEvent, newTarget, event, clipboard);
		}
    }
    m_dragTarget = newTarget;

    return accept;
}

void EventHandler::cancelDragAndDrop(const PlatformMouseEvent& event, Clipboard* clipboard)
{
    if (m_dragTarget) {
        Frame* frame = (m_dragTarget->hasTagName(frameTag) || m_dragTarget->hasTagName(iframeTag)) 
                        ? static_cast<HTMLFrameElementBase*>(m_dragTarget.get())->contentFrame() : 0;
        if (frame)
            frame->eventHandler()->cancelDragAndDrop(event, clipboard);
        else
            dispatchDragEvent(dragleaveEvent, m_dragTarget.get(), event, clipboard);
    }
    m_dragTarget = 0;
}

bool EventHandler::performDragAndDrop(const PlatformMouseEvent& event, Clipboard* clipboard)
{
    bool accept = false;
    if (m_dragTarget) {
        Frame* frame = (m_dragTarget->hasTagName(frameTag) || m_dragTarget->hasTagName(iframeTag)) 
                        ? static_cast<HTMLFrameElementBase*>(m_dragTarget.get())->contentFrame() : 0;
        if (frame)
            accept = frame->eventHandler()->performDragAndDrop(event, clipboard);
        else
            accept = dispatchDragEvent(dropEvent, m_dragTarget.get(), event, clipboard);
    }
    m_dragTarget = 0;
    return accept;
}

Node* EventHandler::nodeUnderMouse() const
{
    return m_nodeUnderMouse.get();
}

void EventHandler::setCapturingMouseEventsNode(PassRefPtr<Node> n)
{
    m_capturingMouseEventsNode = n;
}

MouseEventWithHitTestResults EventHandler::prepareMouseEvent(const HitTestRequest& request, const PlatformMouseEvent& mev)
{
    ASSERT(m_frame);
    ASSERT(m_frame->document());
    
    IntPoint documentPoint = m_frame->view()->windowToContents(mev.pos());
    return m_frame->document()->prepareMouseEvent(request, documentPoint, mev);
}

void EventHandler::updateMouseEventTargetNode(Node* targetNode, const PlatformMouseEvent& mouseEvent, bool fireMouseOverOut)
{
    Node* result = targetNode;
    
    // If we're capturing, we always go right to that node.
    if (m_capturingMouseEventsNode)
        result = m_capturingMouseEventsNode.get();
    
    // If the target node is a text node, dispatch on the parent node - rdar://4196646
    if (result && result->isTextNode())
        result = result->parentNode();
    if (result)
        result = result->shadowAncestorNode();
        
    m_nodeUnderMouse = result;
    
    // Fire mouseout/mouseover if the mouse has shifted to a different node.
    if (fireMouseOverOut) {
        if (m_lastNodeUnderMouse && m_lastNodeUnderMouse->document() != m_frame->document()) {
            m_lastNodeUnderMouse = 0;
            m_lastScrollbarUnderMouse = 0;
        }

        if (m_lastNodeUnderMouse != m_nodeUnderMouse) {
            // send mouseout event to the old node
            if (m_lastNodeUnderMouse)
                EventTargetNodeCast(m_lastNodeUnderMouse.get())->dispatchMouseEvent(mouseEvent, mouseoutEvent, 0, m_nodeUnderMouse.get());
            // send mouseover event to the new node
            if (m_nodeUnderMouse)
                EventTargetNodeCast(m_nodeUnderMouse.get())->dispatchMouseEvent(mouseEvent, mouseoverEvent, 0, m_lastNodeUnderMouse.get());
        }
        m_lastNodeUnderMouse = m_nodeUnderMouse;
    }
}

bool EventHandler::dispatchMouseEvent(const AtomicString& eventType, Node* targetNode, bool cancelable, int clickCount, const PlatformMouseEvent& mouseEvent, bool setUnder)
{
    updateMouseEventTargetNode(targetNode, mouseEvent, setUnder);

    bool swallowEvent = false;

    if (m_nodeUnderMouse)
        swallowEvent = EventTargetNodeCast(m_nodeUnderMouse.get())->dispatchMouseEvent(mouseEvent, eventType, clickCount);
    
    if (!swallowEvent && eventType == mousedownEvent) {
        // Blur current focus node when a link/button is clicked; this
        // is expected by some sites that rely on onChange handlers running
        // from form fields before the button click is processed.
        Node* node = m_nodeUnderMouse.get();
        RenderObject* renderer = node ? node->renderer() : 0;
                
        // Walk up the render tree to search for a node to focus.
        // Walking up the DOM tree wouldn't work for shadow trees, like those behind the engine-based text fields.
        while (renderer) {
            node = renderer->element();
            if (node && node->isFocusable()) {
                // To fix <rdar://problem/4895428> Can't drag selected ToDo, we don't focus a 
                // node on mouse down if it's selected and inside a focused node. It will be 
                // focused if the user does a mouseup over it, however, because the mouseup
                // will set a selection inside it, which will call setFocuseNodeIfNeeded.
                ExceptionCode ec = 0;
                Node* n = node->isShadowNode() ? node->shadowParentNode() : node;
                if (m_frame->selectionController()->isRange() && 
                    m_frame->selectionController()->toRange()->compareNode(n, ec) == Range::NODE_INSIDE &&
                    n->isDescendantOf(m_frame->document()->focusedNode()))
                    return false;
                    
                break;
            }
            
            renderer = renderer->parent();
        }
        // If focus shift is blocked, we eat the event.  Note we should never clear swallowEvent
        // if the page already set it (e.g., by canceling default behavior).
        if (node && node->isMouseFocusable()) {
            if (!m_frame->page()->focusController()->setFocusedNode(node, m_frame))
                swallowEvent = true;
        } else if (!node || !node->focused()) {
            if (!m_frame->page()->focusController()->setFocusedNode(0, m_frame))
                swallowEvent = true;
        }
    }

    return swallowEvent;
}

bool EventHandler::handleWheelEvent(PlatformWheelEvent& e)
{
    Document* doc = m_frame->document();
    if (!doc)
        return false;

    RenderObject* docRenderer = doc->renderer();
    if (!docRenderer)
        return false;

    IntPoint vPoint = m_frame->view()->windowToContents(e.pos());

    HitTestRequest request(true, false);
    HitTestResult result(vPoint);
    doc->renderer()->layer()->hitTest(request, result);
    Node* node = result.innerNode();
    
    if (node) {
        // Figure out which view to send the event to.
        RenderObject* target = node->renderer();
        
        if (target && target->isWidget()) {
            Widget* widget = static_cast<RenderWidget*>(target)->widget();

            if (widget && passWheelEventToWidget(e, widget)) {
                e.accept();
                return true;
            }
        }

        node = node->shadowAncestorNode();
        EventTargetNodeCast(node)->dispatchWheelEvent(e);
        if (e.isAccepted())
            return true;
            
        if (node->renderer()) {
            // Just break up into two scrolls if we need to.  Diagonal movement on 
            // a MacBook pro is an example of a 2-dimensional mouse wheel event (where both deltaX and deltaY can be set).
            float deltaX = e.isContinuous() ? e.continuousDeltaX() : e.deltaX();
            float deltaY = e.isContinuous() ? e.continuousDeltaY() : e.deltaY();
            if (deltaX && node->renderer()->scroll(deltaX < 0 ? ScrollRight : ScrollLeft, e.isContinuous() ? ScrollByPixel : ScrollByLine,
                                                       deltaX < 0 ? -deltaX : deltaX))
                e.accept();
            if (deltaY && node->renderer()->scroll(deltaY < 0 ? ScrollDown : ScrollUp, e.isContinuous() ? ScrollByPixel : ScrollByLine,
                                                       deltaY < 0 ? -deltaY : deltaY))
                e.accept();
        }
    }

    if (!e.isAccepted())
        m_frame->view()->wheelEvent(e);
    
    return e.isAccepted();
}

bool EventHandler::sendContextMenuEvent(const PlatformMouseEvent& event)
{
    Document* doc = m_frame->document();
    FrameView* v = m_frame->view();
    if (!doc || !v)
        return false;
    
    bool swallowEvent;
    IntPoint viewportPos = v->windowToContents(event.pos());
    MouseEventWithHitTestResults mev = doc->prepareMouseEvent(HitTestRequest(false, true), viewportPos, event);

#if 0
    if (!m_frame->selectionController()->contains(viewportPos) && 
        // FIXME: In the editable case, word selection sometimes selects content that isn't underneath the mouse.
        // If the selection is non-editable, we do word selection to make it easier to use the contextual menu items
        // available for text selections.  But only if we're above text.
        (m_frame->selectionController()->isContentEditable() || mev.targetNode() && mev.targetNode()->isTextNode())) {
        m_mouseDownMayStartSelect = true; // context menu events are always allowed to perform a selection
        selectClosestWordFromMouseEvent(mev);
    }
#endif

    swallowEvent = dispatchMouseEvent(contextmenuEvent, mev.targetNode(), true, 0, event, true);
    
    return swallowEvent;
}

void EventHandler::scheduleHoverStateUpdate()
{
    if (!m_hoverTimer.isActive())
        m_hoverTimer.startOneShot(0);
}

// Whether or not a mouse down can begin the creation of a selection.  Fires the selectStart event.
bool EventHandler::canMouseDownStartSelect(Node* node)
{
    if (!node || !node->renderer())
        return true;
    
    // Some controls and images can't start a select on a mouse down.
    if (!node->canStartSelection())
        return false;
            
    for (RenderObject* curr = node->renderer(); curr; curr = curr->parent())    
        if (Node* node = curr->element())
            return EventTargetNodeCast(node)->dispatchHTMLEvent(selectstartEvent, true, true);
    
    return true;
}

bool EventHandler::canMouseDragExtendSelect(Node* node)
{
    if (!node || !node->renderer())
        return true;
            
    for (RenderObject* curr = node->renderer(); curr; curr = curr->parent())    
        if (Node* node = curr->element())
            return EventTargetNodeCast(node)->dispatchHTMLEvent(selectstartEvent, true, true);
    
    return true;
}

void EventHandler::setResizingFrameSet(HTMLFrameSetElement* frameSet)
{
    m_frameSetBeingResized = frameSet;
}

void EventHandler::hoverTimerFired(Timer<EventHandler>*)
{
    m_hoverTimer.stop();

    ASSERT(m_frame);
    ASSERT(m_frame->document());

    if (RenderObject* renderer = m_frame->renderer()) {
        HitTestResult result(m_frame->view()->windowToContents(m_currentMousePosition));
        renderer->layer()->hitTest(HitTestRequest(false, false, true), result);
        m_frame->document()->updateRendering();
    }
}

static EventTargetNode* eventTargetNodeForDocument(Document* doc)
{
    if (!doc)
        return 0;
    Node* node = doc->focusedNode();
    if (!node) {
        if (doc->isHTMLDocument())
            node = doc->body();
        else
            node = doc->documentElement();
        if (!node)
            return 0;
    }
    return EventTargetNodeCast(node);
}


bool EventHandler::keyEvent(const PlatformKeyboardEvent& initialKeyEvent)
{
    // Check for cases where we are too early for events -- possible unmatched key up
    // from pressing return in the location bar.
    EventTargetNode* node = eventTargetNodeForDocument(m_frame->document());
    if (!node)
        return false;
        
    if (initialKeyEvent.isKeyUp())
        return !node->dispatchKeyEvent(initialKeyEvent);
        
    m_frame->loader()->resetMultipleFormSubmissionProtection();
    
    // Prepare the keyPress in advance of the keyDown so we can fire the input method
    // in advance of keyDown
    PlatformKeyboardEvent keyPressEvent = initialKeyEvent;    
    keyPressEvent.setIsAutoRepeat(true);
    RefPtr<KeyboardEvent> keypress = new KeyboardEvent(keyPressEvent, m_frame->document()->defaultView());
    keypress->setTarget(node);
    
    // Run input method in advance of DOM event handling.  This may result in the IM
    // modifying the page prior the keydown event, however this behaviour is necessary
    // in order to match IE
    m_frame->editor()->handleInputMethodKeypress(keypress.get());
    
    bool handledByInputMethod = keypress->defaultHandled();
    
    PlatformKeyboardEvent keyDownEvent = initialKeyEvent; 
    
    if (handledByInputMethod) 
        keyDownEvent.setWindowsKeyCode(CompositionEventKeyCode);
        
    // We always send keyDown and keyPress for all events, including autorepeat keys
    keyDownEvent.setIsAutoRepeat(false);
    
    bool result = !node->dispatchKeyEvent(keyDownEvent);
    
    // Focus may have change during the keyDown handling, so refetch node
    node = eventTargetNodeForDocument(m_frame->document());
    if (!node)
        return result;
    
    if (keypress->defaultHandled())
        return true;
    
    if (handledByInputMethod)
        return result;
    
    // If the default handling has been prevented on the keydown, we prevent it on
    // the keypress as well
    if (result)
        keypress->setDefaultHandled();
    
    ExceptionCode ec;
    node->dispatchEvent(keypress, ec, true);
    
    return result || keypress->defaultHandled() || keypress->defaultPrevented();
}

void EventHandler::defaultKeyboardEventHandler(KeyboardEvent* event)
{
   if (event->type() == keypressEvent) {
        m_frame->editor()->handleKeypress(event);
        if (event->defaultHandled())
            return;
        if ((event->keyIdentifier() == "Tab") && event->keyEvent()->shiftKey())
            defaultTabEventHandler(event, true);
        else if (event->keyIdentifier() == "Tab")
            defaultTabEventHandler(event, false);
    }
}

bool EventHandler::dragHysteresisExceeded(const FloatPoint& floatDragViewportLocation) const
{
    IntPoint dragViewportLocation((int)floatDragViewportLocation.x(), (int)floatDragViewportLocation.y());
    return dragHysteresisExceeded(dragViewportLocation);
}
    
bool EventHandler::dragHysteresisExceeded(const IntPoint& dragViewportLocation) const
{
    IntPoint dragLocation = m_frame->view()->windowToContents(dragViewportLocation);
    IntSize delta = dragLocation - m_mouseDownPos;
    
    int threshold = GeneralDragHysteresis;
    if (dragState().m_dragSrcIsImage)
        threshold = ImageDragHysteresis;
    else if (dragState().m_dragSrcIsLink)
        threshold = LinkDragHysteresis;
    else if (dragState().m_dragSrcInSelection)
        threshold = TextDragHysteresis;
    
    return abs(delta.width()) >= threshold || abs(delta.height()) >= threshold;
}
    
void EventHandler::freeClipboard()
{
    if (dragState().m_dragClipboard)
        dragState().m_dragClipboard->setAccessPolicy(ClipboardNumb);
}

bool EventHandler::shouldDragAutoNode(Node* node, const IntPoint& point) const
{
    ASSERT(node);
    if (node->hasChildNodes() || !m_frame->view())
        return false;
    return m_frame->page() && m_frame->page()->dragController()->mayStartDragAtEventLocation(m_frame, point);
}
    
void EventHandler::dragSourceMovedTo(const PlatformMouseEvent& event)
{
    if (dragState().m_dragSrc && dragState().m_dragSrcMayBeDHTML)
        // for now we don't care if event handler cancels default behavior, since there is none
        dispatchDragSrcEvent(dragEvent, event);
}
    
void EventHandler::dragSourceEndedAt(const PlatformMouseEvent& event, DragOperation operation)
{
    if (dragState().m_dragSrc && dragState().m_dragSrcMayBeDHTML) {
        dragState().m_dragClipboard->setDestinationOperation(operation);
        // for now we don't care if event handler cancels default behavior, since there is none
        dispatchDragSrcEvent(dragendEvent, event);
    }
    freeClipboard();
    dragState().m_dragSrc = 0;
}
    
// returns if we should continue "default processing", i.e., whether eventhandler canceled
bool EventHandler::dispatchDragSrcEvent(const AtomicString& eventType, const PlatformMouseEvent& event)
{
    return !dispatchDragEvent(eventType, dragState().m_dragSrc.get(), event, dragState().m_dragClipboard.get());
}
    
bool EventHandler::handleDrag(const MouseEventWithHitTestResults& event)
{
    if (event.event().button() != LeftButton || event.event().eventType() != MouseEventMoved) {
        // If we allowed the other side of the bridge to handle a drag
        // last time, then m_mousePressed might still be set. So we
        // clear it now to make sure the next move after a drag
        // doesn't look like a drag.
        m_mousePressed = false;
        return false;
    }
    
    if (eventLoopHandleMouseDragged(event))
        return true;
    
    // Careful that the drag starting logic stays in sync with eventMayStartDrag()
    
    if (m_mouseDownMayStartDrag && !dragState().m_dragSrc) {
        allowDHTMLDrag(dragState().m_dragSrcMayBeDHTML, dragState().m_dragSrcMayBeUA);
        if (!dragState().m_dragSrcMayBeDHTML && !dragState().m_dragSrcMayBeUA)
            m_mouseDownMayStartDrag = false;     // no element is draggable
    }

    if (m_mouseDownMayStartDrag && !dragState().m_dragSrc) {
        // try to find an element that wants to be dragged
        HitTestRequest request(true, false);
        HitTestResult result(m_mouseDownPos);
        m_frame->renderer()->layer()->hitTest(request, result);
        Node* node = result.innerNode();
        if (node && node->renderer())
            dragState().m_dragSrc = node->renderer()->draggableNode(dragState().m_dragSrcMayBeDHTML, dragState().m_dragSrcMayBeUA,
                                                                    m_mouseDownPos.x(), m_mouseDownPos.y(), dragState().m_dragSrcIsDHTML);
        else
            dragState().m_dragSrc = 0;
        
        if (!dragState().m_dragSrc)
            m_mouseDownMayStartDrag = false;     // no element is draggable
        else {
            // remember some facts about this source, while we have a HitTestResult handy
            node = result.URLElement();
            dragState().m_dragSrcIsLink = node && node->isLink();
            
            node = result.innerNonSharedNode();
            dragState().m_dragSrcIsImage = node && node->renderer() && node->renderer()->isImage();
            
            dragState().m_dragSrcInSelection = m_frame->selectionController()->contains(m_mouseDownPos);
        }                
    }
    
    // For drags starting in the selection, the user must wait between the mousedown and mousedrag,
    // or else we bail on the dragging stuff and allow selection to occur
    if (m_mouseDownMayStartDrag && !dragState().m_dragSrcIsImage && dragState().m_dragSrcInSelection && event.event().timestamp() - m_mouseDownTimestamp < TextDragDelay) {
        m_mouseDownMayStartDrag = false;
        dragState().m_dragSrc = 0;
        // ...but if this was the first click in the window, we don't even want to start selection
        if (eventActivatedView(event.event()))
            m_mouseDownMayStartSelect = false;
    }
    
    if (!m_mouseDownMayStartDrag)
        return !mouseDownMayStartSelect() && !m_mouseDownMayStartAutoscroll;
    
    // We are starting a text/image/url drag, so the cursor should be an arrow
    m_frame->view()->setCursor(pointerCursor());
    
    if (!dragHysteresisExceeded(event.event().pos())) 
        return true;
    
    // Once we're past the hysteresis point, we don't want to treat this gesture as a click
    invalidateClick();
    
    DragOperation srcOp = DragOperationNone;      
    
    freeClipboard();    // would only happen if we missed a dragEnd.  Do it anyway, just
                        // to make sure it gets numbified
    dragState().m_dragClipboard = createDraggingClipboard();  
    
    if (dragState().m_dragSrcMayBeDHTML) {
        // Check to see if the is a DOM based drag, if it is get the DOM specified drag 
        // image and offset
        if (dragState().m_dragSrcIsDHTML) {
            int srcX, srcY;
            dragState().m_dragSrc->renderer()->absolutePosition(srcX, srcY);
            IntSize delta = m_mouseDownPos - IntPoint(srcX, srcY);
            dragState().m_dragClipboard->setDragImageElement(dragState().m_dragSrc.get(), IntPoint() + delta);
        } 
        
        m_mouseDownMayStartDrag = dispatchDragSrcEvent(dragstartEvent, m_mouseDown)
            && !m_frame->selectionController()->isInPasswordField();
        
        // Invalidate clipboard here against anymore pasteboard writing for security.  The drag
        // image can still be changed as we drag, but not the pasteboard data.
        dragState().m_dragClipboard->setAccessPolicy(ClipboardImageWritable);
        
        if (m_mouseDownMayStartDrag) {
            // gather values from DHTML element, if it set any
            dragState().m_dragClipboard->sourceOperation(srcOp);
            
            // Yuck, dragSourceMovedTo() can be called as a result of kicking off the drag with
            // dragImage!  Because of that dumb reentrancy, we may think we've not started the
            // drag when that happens.  So we have to assume it's started before we kick it off.
            dragState().m_dragClipboard->setDragHasStarted();
        }
    }
    
    if (m_mouseDownMayStartDrag) {
        DragController* dragController = m_frame->page() ? m_frame->page()->dragController() : 0;
        bool startedDrag = dragController && dragController->startDrag(m_frame, dragState().m_dragClipboard.get(), srcOp, event.event(), m_mouseDownPos, dragState().m_dragSrcIsDHTML);
        if (!startedDrag && dragState().m_dragSrcMayBeDHTML) {
            // Drag was canned at the last minute - we owe m_dragSrc a DRAGEND event
            dispatchDragSrcEvent(dragendEvent, event.event());
            m_mouseDownMayStartDrag = false;
        }
    } 
    
    if (!m_mouseDownMayStartDrag) {
        // something failed to start the drag, cleanup
        freeClipboard();
        dragState().m_dragSrc = 0;
    }
    
    // No more default handling (like selection), whether we're past the hysteresis bounds or not
    return true;
}
  
bool EventHandler::handleTextInputEvent(const String& text, Event* underlyingEvent,
                                        bool isLineBreak, bool isBackTab)
{
    if (!m_frame)
        return false;
    EventTarget* target;
    if (underlyingEvent)
        target = underlyingEvent->target();
    else
        target = eventTargetNodeForDocument(m_frame->document());
    if (!target)
        return false;
    RefPtr<TextEvent> event = new TextEvent(m_frame->domWindow(), text);
    event->setUnderlyingEvent(underlyingEvent);
    event->setIsLineBreak(isLineBreak);
    event->setIsBackTab(isBackTab);
    ExceptionCode ec;
    return target->dispatchEvent(event.release(), ec, true);
}
    
    
bool EventHandler::invertSenseOfTabsToLinks(KeyboardEvent*) const
{
    return false;
}

bool EventHandler::tabsToLinks(KeyboardEvent* event) const
{
    Page* page = m_frame->page();
    if (!page)
        return false;

    if (page->chrome()->client()->tabsToLinks())
        return !invertSenseOfTabsToLinks(event);

    return invertSenseOfTabsToLinks(event);
}

void EventHandler::defaultTextInputEventHandler(TextEvent* event)
{
    String data = event->data();
    if (data == "\t") {
        defaultTabEventHandler(event, event->isBackTab());
        if (event->defaultHandled())
            return;
    }
    if (data == "\n") {
        if (event->isLineBreak()) {
            if (m_frame->editor()->insertLineBreak())
                event->setDefaultHandled();
        } else {
            if (m_frame->editor()->insertParagraphSeparator())
                event->setDefaultHandled();
        }
    } else {
        if (m_frame->editor()->insertTextWithoutSendingTextEvent(data, false, event))
            event->setDefaultHandled();
    }
}

void EventHandler::defaultTabEventHandler(Event* event, bool isBackTab)
{
    Page* page = m_frame->page();
    // Tabs can be used in design mode editing. You can still move out with back tab.
    if (!page || !page->tabKeyCyclesThroughElements() || (m_frame->document()->inDesignMode() && !isBackTab))
        return;
    FocusController* focus = page->focusController();
    KeyboardEvent* keyboardEvent = findKeyboardEvent(event);
    bool handled;
    if (isBackTab)
        handled = focus->advanceFocus(FocusDirectionBackward, keyboardEvent);
    else
        handled = focus->advanceFocus(keyboardEvent); // get direction from keyboard event

    if (handled) {
#if 1
		Node* n = m_frame->document()->focusedNode();
		if (n && n->isElementNode()) {
			Element* elem = static_cast<Element*>(n);
			m_frame->notifyFocusChanged(elem);
		}
#endif
        event->setDefaultHandled();
	}
}

}
