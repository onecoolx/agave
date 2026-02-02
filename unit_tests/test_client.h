/*
 * Agave - A lightweight web browser engine
 *
 * Copyright (c) 2026, Zhang Ji Peng
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _AGAVE_TEST_CLIENTS_H_
#define _AGAVE_TEST_CLIENTS_H_

#include "ChromeClient.h"
#include "ContextMenuClient.h"
#include "DragClient.h"
#include "FrameLoaderClient.h"
#include "EditorClient.h"

#include "Frame.h"
#include "HTTPParsers.h"
#include "MIMETypeRegistry.h"
#include "ProgressTracker.h"

using namespace WebCore;
using namespace WTF;

class ChromeClientTest : public ChromeClient
{
public:
    ChromeClientTest(int32_t, int32_t) {}
    virtual ~ChromeClientTest() {}
    virtual void chromeDestroyed() { delete this; }
    virtual void setWindowRect(const FloatRect&) {}
    virtual FloatRect windowRect() { return FloatRect(); }
    virtual float scaleFactor() { return 1.0f; }
    virtual void focus() {}
    virtual void unfocus() {}
    virtual bool canTakeFocus(FocusDirection) { return false; }
    virtual void takeFocus(FocusDirection) {}
    virtual void setWindowFeature(const WindowFeatures&) {}
    virtual Page* createWindow(Frame*, const FrameLoadRequest&, const WindowFeatures&) { return nullptr; }
    virtual bool canRunModal() { return false; }
    virtual void runModal() {}
    virtual bool toolbarVisible() { return false; }
    virtual bool menubarVisible() { return false; }
    virtual bool scrollbarsVisible() { return false; }
    virtual bool statusbarVisible() { return false; }
    virtual bool locationbarVisible() { return false; }
    virtual bool personalbarVisible() {return false; }
    virtual void addMessageToConsole(const String& message, unsigned int lineNumber, const String& sourceID) {}
    virtual bool canRunBeforeUnloadConfirmPanel() { return false; }
    virtual bool runBeforeUnloadConfirmPanel(const String& message, Frame* frame) { return false; }
    virtual void closeWindowSoon() {}
    virtual void runJavaScriptAlert(Frame*, const String&) {}
    virtual bool runJavaScriptConfirm(Frame*, const String&) { return false; }
    virtual bool runJavaScriptPrompt(Frame*, const String& message, const String& defaultValue, String& result) { return false; }
    virtual void setStatusbarText(const String&) {}
    virtual bool shouldInterruptJavaScript() { return false; }
    virtual bool tabsToLinks() const { return false; }
    virtual IntRect windowResizerRect() const { return IntRect(); }
    virtual void addToDirtyRegion(const IntRect&) {}
    virtual void scrollBackingStore(int dx, int dy, const IntRect& scrollViewRect, const IntRect& clipRect) {}
    virtual void updateBackingStore() {}
    virtual void mouseDidMoveOverElement(const HitTestResult&, unsigned modifierFlags) {}
    virtual void setToolTip(const String&, const IntPoint&) {}
    virtual void print(Frame*) {}
    virtual void focusChangedRect(const IntRect&) {}
};

class ContextMenuClientTest : public ContextMenuClient
{
public:
    ContextMenuClientTest() {}
    virtual ~ContextMenuClientTest() {}
    virtual void contextMenuDestroyed() { delete this; }
    virtual void downloadURL(const KURL& url) {}
    virtual void lookUpInDictionary(Frame*) {}
    virtual void searchWithGoogle(const Frame*) {}
    virtual void speak(const String&) {}
    virtual void stopSpeaking() {}
};

class DragClientTest : public DragClient
{
public:
    DragClientTest() {}
    virtual ~DragClientTest() {}

    virtual void dragControllerDestroyed() {delete this;}

    virtual void willPerformDragDestinationAction(DragDestinationAction, DragData*) {}
    virtual void willPerformDragSourceAction(DragSourceAction, const IntPoint&, Clipboard*) {}
    virtual DragDestinationAction actionMaskForDrag(DragData*) { return DragDestinationActionNone; }
    virtual DragSourceAction dragSourceActionMaskForPoint(const IntPoint& windowPoint) { return DragSourceActionNone; }

    virtual void startDrag(DragImageRef dragImage, const IntPoint& dragImageOrigin,
                           const IntPoint& eventPos, Clipboard*, Frame*, bool linkDrag = false) {}
    virtual DragImageRef createDragImageForLink(KURL&, const String& label, Frame*) { return 0; }
};

class EditorClientTest : public EditorClient
{
public:
    EditorClientTest() {}
    virtual ~EditorClientTest() {}
    virtual void pageDestroyed() { delete this; }

    virtual bool shouldDeleteRange(Range*) { return false; }
    virtual bool shouldShowDeleteInterface(HTMLElement*) { return false; }
    virtual bool smartInsertDeleteEnabled() { return false; }
    virtual bool isContinuousSpellCheckingEnabled() { return false; }
    virtual void toggleContinuousSpellChecking() {}
    virtual bool isGrammarCheckingEnabled() { return false; }
    virtual void toggleGrammarChecking() {}
    virtual int spellCheckerDocumentTag() { return 0; }

    virtual bool isEditable() { return false; }

    virtual bool shouldBeginEditing(Range*) { return false; }
    virtual bool shouldEndEditing(Range*) { return false; }
    virtual bool shouldInsertNode(Node*, Range*, EditorInsertAction) { return false; }
    virtual bool shouldInsertText(String, Range*, EditorInsertAction) { return false; }
    virtual bool shouldChangeSelectedRange(Range* fromRange, Range* toRange, EAffinity, bool stillSelecting) { return false; }

    virtual bool shouldApplyStyle(CSSStyleDeclaration*, Range*) { return false; }
    virtual bool shouldMoveRangeAfterDelete(Range*, Range*) { return false; }

    virtual void didBeginEditing() {}
    virtual void respondToChangedContents() {}
    virtual void respondToChangedSelection() {}
    virtual void didEndEditing() {}
    virtual void didWriteSelectionToPasteboard() {}
    virtual void didSetSelectionTypesForPasteboard() {}
    virtual void registerCommandForUndo(PassRefPtr<WebCore::EditCommand>) {}
    virtual void registerCommandForRedo(PassRefPtr<WebCore::EditCommand>) {}
    virtual void clearUndoRedoOperations() {}

    virtual bool canUndo() const { return false; }
    virtual bool canRedo() const { return false; }
    virtual void undo() {}
    virtual void redo() {}

    virtual void handleKeypress(KeyboardEvent*) {}
    virtual void handleInputMethodKeypress(KeyboardEvent*) {}

    virtual void textFieldDidBeginEditing(Element*) {}
    virtual void textFieldDidEndEditing(Element*) {}
    virtual void textDidChangeInTextField(Element*) {}
    virtual bool doTextFieldCommandFromEvent(Element*, KeyboardEvent*) { return false; }
    virtual void textWillBeDeletedInTextField(Element*) {}
    virtual void textDidChangeInTextArea(Element*) {}

    virtual void ignoreWordInSpellDocument(const String&) {}
    virtual void learnWord(const String&) {}
    virtual void checkSpellingOfString(const UChar*, int length, int* misspellingLocation, int* misspellingLength) {}
    virtual void checkGrammarOfString(const UChar*, int length, Vector<GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength) {}
    virtual void updateSpellingUIWithGrammarString(const String&, const GrammarDetail& detail) {}
    virtual void updateSpellingUIWithMisspelledWord(const String&) {}
    virtual void showSpellingUI(bool show) {}
    virtual bool spellingUIIsShowing() { return false; }
    virtual void getGuessesForWord(const String&, Vector<String>& guesses) {}
    virtual void setInputMethodState(bool enabled) {}
};

class FrameLoaderClientTest : public FrameLoaderClient
{
public:
    FrameLoaderClientTest() : m_frame(0), m_firstData(false), m_loadStart(false), m_progress(0) {}
    virtual ~FrameLoaderClientTest() {}

    virtual void frameLoaderDestroyed() { delete this; }

    virtual bool hasWebView() const { return true; }
    virtual bool hasFrameView() const { return true; }

    virtual bool privateBrowsingEnabled() const { return false; }

    virtual void makeDocumentView() {}
    virtual void makeRepresentation(DocumentLoader*) {}
    virtual void setDocumentViewFromCachedPage(CachedPage*) {}
    virtual void forceLayout()
    {
        m_frame->view()->setNeedsLayout();
        m_frame->view()->layout();
    }
    virtual void forceLayoutForNonHTML() {}

    virtual void setCopiesOnScroll() {}

    virtual void detachedFromParent2() {}
    virtual void detachedFromParent3() {}
    virtual void detachedFromParent4() {}

    virtual void dispatchDidReceiveResponse(DocumentLoader* loader, unsigned long identifier, const ResourceResponse& response)
    {
        m_response = response;
        m_firstData = true;
        if ((!loader->isLoadingSubresources()) && (loader->frame() == m_frame->page()->mainFrame())) {
            if (equalIgnoringCase(loader->request().httpMethod(), "POST")) {
                loader->request().setHTTPMethod("GET");
            }
        }
    }

    virtual void assignIdentifierToInitialRequest(unsigned long identifier, DocumentLoader*, const ResourceRequest&) {}
    virtual void dispatchWillSendRequest(DocumentLoader*, unsigned long identifier, ResourceRequest&, const ResourceResponse& redirectResponse) {}
    virtual void dispatchDidReceiveAuthenticationChallenge(DocumentLoader*, unsigned long identifier, const AuthenticationChallenge&) {}
    virtual void dispatchDidCancelAuthenticationChallenge(DocumentLoader*, unsigned long identifier, const AuthenticationChallenge&) {}
    virtual void dispatchDidReceiveContentLength(DocumentLoader*, unsigned long identifier, int lengthReceived) {}
    virtual void dispatchDidFinishLoading(DocumentLoader*, unsigned long identifier) {}
    virtual void dispatchDidFailLoading(DocumentLoader*, unsigned long identifier, const ResourceError&) {}
    virtual bool dispatchDidLoadResourceFromMemoryCache(DocumentLoader*, const ResourceRequest&, const ResourceResponse&, int length) { return false; }

    virtual void dispatchDidHandleOnloadEvents() {}
    virtual void dispatchDidReceiveServerRedirectForProvisionalLoad() {}
    virtual void dispatchDidCancelClientRedirect() {}
    virtual void dispatchWillPerformClientRedirect(const KURL&,
                                                   double, double) {}
    virtual void dispatchDidChangeLocationWithinPage() {}
    virtual void dispatchWillClose() {}
    virtual void dispatchDidReceiveIcon() {}
    virtual void dispatchDidStartProvisionalLoad() {}
    virtual void dispatchDidReceiveTitle(const String&) {}
    virtual void dispatchDidSetLocation(const KURL&) {}
    virtual void dispatchDidCommitLoad() {}
    virtual void dispatchDidFailProvisionalLoad(const ResourceError&) {}
    virtual void dispatchDidFailLoad(const ResourceError&) {}
    virtual void dispatchDidFinishDocumentLoad() {}
    virtual void dispatchDidFinishLoad() {}
    virtual void dispatchDidFirstLayout() {}
    virtual void dispatchWillFirstLayout() {}

    virtual Frame* dispatchCreatePage() { return 0; }
    virtual void dispatchShow() {}

    virtual void dispatchDecidePolicyForMIMEType(FramePolicyFunction policyFunction, const String& MIMEType, const ResourceRequest&)
    {
        if (!policyFunction || !m_frame) {
            return;
        }

        switch (m_response.httpStatusCode()) {
            case 205:
            // FIXME: a 205 response requires that the requester reset the document view.
            // Fallthrough
            case 204:
                (m_frame->loader()->*policyFunction)(PolicyIgnore);
                return;
        }

        if (WebCore::contentDispositionType(m_response.httpHeaderField("Content-Disposition")) == WebCore::ContentDispositionAttachment) {
            (m_frame->loader()->*policyFunction)(PolicyDownload);
        } else if (canShowMIMEType(m_response.mimeType())) {
            (m_frame->loader()->*policyFunction)(PolicyUse);
        } else {
            (m_frame->loader()->*policyFunction)(PolicyDownload);
        }
    }

    virtual void dispatchDecidePolicyForNewWindowAction(FramePolicyFunction policyFunction, const NavigationAction&, const ResourceRequest&, const String& frameName)
    {
        if (!policyFunction || !m_frame) {
            return;
        }
        (m_frame->loader()->*policyFunction)(PolicyUse);
    }

    virtual void dispatchDecidePolicyForNavigationAction(FramePolicyFunction policyFunction, const NavigationAction&, const ResourceRequest&)
    {
        if (!policyFunction || !m_frame) {
            return;
        }
        (m_frame->loader()->*policyFunction)(PolicyUse);
    }

    virtual void cancelPolicyCheck() {}

    virtual void dispatchUnableToImplementPolicy(const ResourceError&) {}

    virtual void dispatchWillSubmitForm(FramePolicyFunction policyFunction,
                                        PassRefPtr<FormState> form)
    {
        if (!policyFunction || !m_frame) {
            return;
        }

        (m_frame->loader()->*policyFunction)(PolicyUse);
    }

    virtual void dispatchDidLoadMainResource(DocumentLoader*) {}
    virtual void revertToProvisionalState(DocumentLoader*) {}
    virtual void setMainDocumentError(DocumentLoader*, const ResourceError&) {}
    virtual void clearUnarchivingState(DocumentLoader*) {}

    virtual void setMainFrameDocumentReady(bool) {}

    virtual void postProgressStartedNotification()
    {
        m_loadStart = true;
        m_progress = 0;
    }

    virtual void postProgressEstimateChangedNotification()
    {
        m_progress = (uint32_t)(m_frame->page()->progress()->estimatedProgress() * 100);
    }

    virtual void postProgressFinishedNotification()
    {
    }

    virtual void startDownload(const ResourceRequest&) {}

    virtual void willChangeTitle(DocumentLoader*) {}
    virtual void didChangeTitle(DocumentLoader*) {}

    virtual void committedLoad(DocumentLoader* loader, const char* data, int length)
    {
        FrameLoader* fl = loader->frameLoader();
        if (m_firstData) {
            String encoding = loader->overrideEncoding();
            bool userChosen = !encoding.isNull();
            if (encoding.isEmpty()) {
                encoding = loader->response().textEncodingName();
            }
            fl->setEncoding(encoding, userChosen);
            m_firstData = false;
        }

        fl->addData(data, length);
    }

    virtual void finishedLoading(DocumentLoader*) {}
    virtual void finalSetupForReplace(DocumentLoader*) {}

    virtual void updateGlobalHistoryForStandardLoad(const KURL&) {}
    virtual void updateGlobalHistoryForReload(const KURL&) {}
    virtual bool shouldGoToHistoryItem(HistoryItem*) const { return false; }

    virtual ResourceError cancelledError(const ResourceRequest&) { return ResourceError(); }
    virtual ResourceError blockedError(const ResourceRequest&) { return ResourceError(); }
    virtual ResourceError cannotShowURLError(const ResourceRequest&) { return ResourceError(); }
    virtual ResourceError interruptForPolicyChangeError(const ResourceRequest&) { return ResourceError(); }

    virtual ResourceError cannotShowMIMETypeError(const ResourceResponse&) { return ResourceError(); }
    virtual ResourceError fileDoesNotExistError(const ResourceResponse&) { return ResourceError(); }

    virtual bool shouldFallBack(const ResourceError&) { return false; }

    virtual void setDefersLoading(bool) {}

    virtual bool willUseArchive(ResourceLoader*, const ResourceRequest&, const KURL& originalURL) const { return false; }
    virtual bool isArchiveLoadPending(ResourceLoader*) const { return false; }
    virtual void cancelPendingArchiveLoad(ResourceLoader*) {}
    virtual void clearArchivedResources() {}

    virtual bool canHandleRequest(const ResourceRequest&) const { return true; }
    virtual bool canShowMIMEType(const String& MIMEType) const
    {
        if (MIMETypeRegistry::isSupportedImageMIMEType(MIMEType)) {
            return true;
        }

        if (MIMETypeRegistry::isSupportedNonImageMIMEType(MIMEType)) {
            return true;
        }

        return false;
    }
    virtual bool representationExistsForURLScheme(const String&) const { return false; }
    virtual String generatedMIMETypeForURLScheme(const String&) const { return String(); }

    virtual void frameLoadCompleted() {}
    virtual void saveViewStateToItem(HistoryItem*) {}
    virtual void restoreViewState() {}
    virtual void provisionalLoadStarted() {}
    virtual void didFinishLoad() {}
    virtual void prepareForDataSourceReplacement() {}

    virtual PassRefPtr<DocumentLoader> createDocumentLoader(const ResourceRequest& request, const SubstituteData& substituteData)
    {
        RefPtr<DocumentLoader> loader = new DocumentLoader(request, substituteData);
        return loader.release();
    }

    virtual void setTitle(const String& title, const KURL&) {}

    virtual String userAgent(const KURL&) { return String("Mozilla/5.0 (Linux) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/5.0.2 Safari/537.36"); }

    virtual void saveDocumentViewToCachedPage(CachedPage*) {}
    virtual bool canCachePage() const { return false; }
    virtual void download(ResourceHandle*, const ResourceRequest&, const ResourceRequest&, const ResourceResponse&) {}

    virtual PassRefPtr<Frame> createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
                                          const String& referrer, ScrollbarMode modeScrolling, int marginWidth, int marginHeight)
    {
        FrameLoaderClientTest* client = new FrameLoaderClientTest;
        Frame* frame = new Frame(m_frame->page(), ownerElement, client);
        client->setFrame(frame);
        FrameView* frameView = new FrameView(frame);
        frame->setView(frameView);
        frameView->setContainingView(m_frame->view()->containingView());
        frameView->deref();
        frame->init();

        RefPtr<Frame> childFrame(adoptRef(frame));

        childFrame->view()->setScrollbarsMode(modeScrolling);
        if (marginWidth != -1) {
            childFrame->view()->setMarginWidth(marginWidth);
        }
        if (marginHeight != -1) {
            childFrame->view()->setMarginHeight(marginHeight);
        }

        m_frame->tree()->appendChild(childFrame);
        childFrame->tree()->setName(name);
        FrameLoadType childLoadType = FrameLoadTypeRedirectWithLockedHistory;
        childFrame->loader()->load(url, referrer, childLoadType, String(), 0, 0);

        if (!childFrame->tree()->parent()) {
            return 0;
        }

        return childFrame.release();
    }

    virtual Widget* createPlugin(const IntSize&, Element*, const KURL&, const Vector<String>&, const Vector<String>&, const String&, bool loadManually) { return 0; }
    virtual void redirectDataToPlugin(Widget* pluginWidget) {}
    virtual Widget* createJavaAppletWidget(const IntSize&, Element*, const KURL& baseURL, const Vector<String>& paramNames, const Vector<String>& paramValues) { return 0; }

    virtual ObjectContentType objectContentType(const KURL& url, const String& mimeType) { return ObjectContentFrame; }
    virtual String overrideMediaType() const { return String(); }

    virtual void windowObjectCleared() const {}

    virtual void didPerformFirstNavigation() const {}

    virtual void registerForIconNotification(bool listen = true) {}
public:
    void setFrame (Frame* frame) { m_frame = frame; }
    bool loadStart(void) { return m_loadStart; }
    uint32_t progress(void) { return m_progress; }
private:
    Frame* m_frame;
    bool m_firstData;
    ResourceResponse m_response;
    bool m_loadStart;
    uint32_t m_progress;
};

#endif
