/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003 Apple Computer, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef qjs_events_h
#define qjs_events_h

#include "EventListener.h"
#include "qjs_dom.h"
#include "qjs_html.h"

#include <text/String.h>

namespace QJS {
    class Window;
}

namespace WebCore {

    class Clipboard;
    class Event;

    class JSAbstractEventListener : public EventListener {
    public:
        JSAbstractEventListener(bool html = false);

        virtual void handleEvent(Event*, bool isWindowEvent);
        virtual bool isHTMLEventListener() const;
        virtual JSValue listenerObj() const = 0;
        virtual QJS::Window* windowObj() const = 0;

    private:
        bool m_html;
    };

    class JSUnprotectedEventListener : public JSAbstractEventListener {
    public:
        JSUnprotectedEventListener(JSValue listener, QJS::Window*, bool html = false);
        virtual ~JSUnprotectedEventListener();

        virtual JSValue listenerObj() const;
        virtual QJS::Window* windowObj() const;
        void clearWindowObj();
        virtual void mark();
    private:
        JSValue m_listener;
        QJS::Window* m_win;
    };

    class JSEventListener : public JSAbstractEventListener {
    public:
        JSEventListener(JSValue listener, QJS::Window*, bool html = false);
        virtual ~JSEventListener();

        virtual JSValue listenerObj() const;
        virtual QJS::Window* windowObj() const;
        void clearWindowObj();

    protected:
        mutable JSValue m_listener;

    private:
        QJS::Window* m_win;
    };

    class JSLazyEventListener : public JSEventListener {
    public:
        JSLazyEventListener(const String& functionName, const String& code, QJS::Window*, Node*, int lineNumber = 0);
        virtual JSValue listenerObj() const;

    private:
        virtual JSValue eventParameterName() const;
        void parseCode() const;

        mutable String m_functionName;
        mutable String m_code;
        mutable bool m_parsed;
        int m_lineNumber;
        Node* m_originalNode;
    };

    JSValue getNodeEventListener(Node*, const AtomicString& eventType);

    class JSClipboard {
    public:
        static void init(JSContext*);
        static JSValue create(JSContext*, Clipboard*);
        static void finalizer(JSRuntime *rt, JSValue val);

        static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
        static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);

        static JSClassID js_class_id;

        static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

        enum { ClearData, GetData, SetData, Types, SetDragImage, DropEffect, EffectAllowed };

    };

    JSValue toJS(JSContext*, Clipboard*);
    Clipboard* toClipboard(JSValue);

} // namespace WebCore

#endif // kjs_events_h
