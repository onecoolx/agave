/* Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 *                     1999-2001 Lars Knoll <knoll@kde.org>
 *                     1999-2001 Antti Koivisto <koivisto@kde.org>
 *                     2000-2001 Simon Hausmann <hausmann@kde.org>
 *                     2000-2001 Dirk Mueller <mueller@kde.org>
 *                     2000 Stefan Schimanski <1Stein@gmx.de>
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Trolltech ASA
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
 */

#ifndef FramePrivate_h
#define FramePrivate_h

#include "Editor.h"
#include "EventHandler.h"
#include "FrameTree.h"
#include "Range.h"
#include "SelectionController.h"
#include "StringHash.h"

#if USE(NPOBJECT)
namespace KJS {
    class Interpreter;
        
    namespace Bindings {
        class Instance;
        class RootObject;
    }
}
#endif //USE(NPOBJECT)

namespace WebCore {

    class UserStyleSheetLoader;

#if USE(NPOBJECT)
    typedef HashMap<void*, RefPtr<KJS::Bindings::RootObject> > RootObjectMap;
#endif //USE(NPOBJECT)
    
    class FramePrivate {
    public:
        FramePrivate(Page*, Frame* parent, Frame* thisFrame, HTMLFrameOwnerElement*, FrameLoaderClient*);
        ~FramePrivate();

        Page* m_page;
        FrameTree m_treeNode;
        RefPtr<DOMWindow> m_domWindow;

        HTMLFrameOwnerElement* m_ownerElement;
        RefPtr<FrameView> m_view;
        RefPtr<Document> m_doc;

#if ENABLE(KJS)
        KJSProxy* m_jscript;
#endif

#if ENABLE(QJS)
        ScriptController* m_jscript;
#endif

        String m_kjsStatusBarText;
        String m_kjsDefaultStatusBarText;

        int m_zoomFactor;
		float m_pageFactor;

        TextGranularity m_selectionGranularity;

        SelectionController m_selectionController;
        Selection m_mark;
        Timer<Frame> m_caretBlinkTimer;
        Editor m_editor;
        EventHandler m_eventHandler;

        bool m_caretVisible : 1;
        bool m_caretPaint : 1;
        bool m_isActive : 1;
        bool m_isPainting : 1;

        RefPtr<CSSMutableStyleDeclaration> m_typingStyle;

        Timer<Frame> m_lifeSupportTimer;

        FrameLoader* m_loader;
        
        UserStyleSheetLoader* m_userStyleSheetLoader;
        
        RefPtr<Node> m_elementToDraw;
        PaintRestriction m_paintRestriction;
        
        bool m_highlightTextMatches;
        bool m_windowHasFocus;
        
        bool m_inViewSourceMode;

        unsigned frameCount;

        bool m_prohibitsScrolling;

#if USE(NPOBJECT)
        // The root object used for objects bound outside the context of a plugin.
        RefPtr<KJS::Bindings::RootObject> m_bindingRootObject; 
        RootObjectMap m_rootObjects;
        NPObject* m_windowScriptNPObject;
#endif //USE(NPOBJECT)
    };
}

#endif
