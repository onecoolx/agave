/*
 *  Copyright (C) 2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007 Apple Inc. All rights reseved.
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

#ifndef qjs_window_h
#define qjs_window_h

#include "qjs_binding.h"
#include <text/String.h>
#include <wtf/HashMap.h>
#include <wtf/OwnPtr.h>

namespace WebCore {
    class AtomicString;
    class DOMWindow;
    class Frame;
    class FrameView;
    class JSDOMWindow;
    class JSEventListener;
    class JSUnprotectedEventListener;
    class Node;
}

namespace QJS {

    class Location;
    class PausedTimeout;
    class ScheduledAction;
    class Window;
    class WindowFunc;

    class PausedTimeouts {
        public:
            PausedTimeouts(PausedTimeout *a, size_t length) : m_array(a), m_length(length) { }
            ~PausedTimeouts();

            size_t numTimeouts() const { return m_length; }
            PausedTimeout *takeTimeouts()
            {
                PausedTimeout *a = m_array;
                m_array = 0;
                return a; 
            }

        private:
            PausedTimeout *m_array;
            size_t m_length;

            PausedTimeouts(const PausedTimeouts&);
            PausedTimeouts& operator=(const PausedTimeouts&);
    };

    class DOMWindowTimer;

    struct WindowPrivate;

    class Window {
        friend class Location;
        friend class WindowFunc;
        friend class ScheduledAction;
    protected:
        Window(WebCore::DOMWindow*);
    public:
        ~Window();
        WebCore::DOMWindow* impl() const { return m_impl.get(); }
        void disconnectFrame();
        /**
         * Returns and registers a window object. In case there's already a Window
         * for the specified frame p this will be returned in order to have unique
         * bindings.
         */
        static JSValue retrieve(WebCore::Frame*);
        /**
         * Returns the Window object for a given HTML frame
         */
        static Window* retrieveWindow(WebCore::Frame*);
        /**
         * returns a pointer to the Window object this javascript interpreting instance
         * was called from.
         */
        static Window* retrieveActive(JSContext*);

        static void init(JSContext*);
        static JSValue create(JSContext*, WebCore::DOMWindow*);
        static void finalizer(JSRuntime *rt, JSValue val);

        static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
        static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);
        static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

        static JSClassID js_class_id;


        int installTimeout(const WebCore::String& handler, int t, bool singleShot);
        int installTimeout(JSValue* function, const WTF::Vector<JSValue>& args, int t, bool singleShot);

        void clearTimeout(int timerId, bool delAction = true);
        PausedTimeouts* pauseTimeouts();
        void resumeTimeouts(PausedTimeouts*);

        void timerFired(DOMWindowTimer*);

        ScriptInterpreter *interpreter() const;

        bool isSafeScript(JSContext*) const;
        //static bool isSafeScript(const ScriptInterpreter *origin, const ScriptInterpreter *target);

        JSValue location(JSContext*) const;

        // Finds a wrapper of a JS EventListener, returns 0 if no existing one.
        WebCore::JSEventListener* findJSEventListener(JSValue, bool html = false);

        // Finds or creates a wrapper of a JS EventListener. JS EventListener object is GC-protected.
        WebCore::JSEventListener *findOrCreateJSEventListener(JSValue, bool html = false);

        // Finds a wrapper of a GC-unprotected JS EventListener, returns 0 if no existing one.
        WebCore::JSUnprotectedEventListener* findJSUnprotectedEventListener(JSValue, bool html = false);

        // Finds or creates a wrapper of a JS EventListener. JS EventListener object is *NOT* GC-protected.
        WebCore::JSUnprotectedEventListener *findOrCreateJSUnprotectedEventListener(JSValue, bool html = false);

        void clear();

        // Set the current "event" object
        void setCurrentEvent(WebCore::Event*);

        // Set a place to put a dialog return value when the window is cleared.
        void setReturnValueSlot(JSValue** slot);

        typedef HashMap<JSObject*, WebCore::JSEventListener*> ListenersMap;
        typedef HashMap<JSObject*, WebCore::JSUnprotectedEventListener*> UnprotectedListenersMap;

        ListenersMap& jsEventListeners();
        ListenersMap& jsHTMLEventListeners();
        UnprotectedListenersMap& jsUnprotectedEventListeners();
        UnprotectedListenersMap& jsUnprotectedHTMLEventListeners();

        enum {
            // Functions
            AToB, BToA, Open, SetTimeout,
            ClearTimeout, SetInterval, ClearInterval, CaptureEvents, 
            ReleaseEvents, AddEventListener, RemoveEventListener, Scroll,
            ScrollBy, ScrollTo, MoveBy, MoveTo,
            ResizeBy, ResizeTo, ShowModalDialog,

            // Attributes
            Crypto, Event_, Location_, Navigator_,
            ClientInformation,

            // Event Listeners
            Onabort, Onblur, Onchange, Onclick,
            Ondblclick, Onerror, Onfocus, Onkeydown,
            Onkeypress, Onkeyup, Onload, Onmousedown,
            Onmousemove, Onmouseout, Onmouseover, Onmouseup,
            OnWindowMouseWheel, Onreset, Onresize, Onscroll,
            Onsearch, Onselect, Onsubmit, Onunload,
            Onbeforeunload,

            // Constructors
            DOMException, Image, Option, XMLHttpRequest,
            XSLTProcessor_
        };

    private:
        JSValue getListener(JSContext*, const WebCore::AtomicString& eventType) const;
        void setListener(JSContext*, const WebCore::AtomicString& eventType, JSValue func);

#if 0
        static JSValue *childFrameGetter(ExecState *exec, JSObject *, const Identifier&, const PropertySlot& slot);
        static JSValue *namedFrameGetter(ExecState *exec, JSObject *, const Identifier&, const PropertySlot& slot);
        static JSValue *indexGetter(ExecState *exec, JSObject *, const Identifier&, const PropertySlot& slot);
        static JSValue *namedItemGetter(ExecState *exec, JSObject *, const Identifier&, const PropertySlot& slot);
#endif

        void updateLayout() const;

        void clearHelperObjectProperties();
        void clearAllTimeouts();
        int installTimeout(ScheduledAction*, int interval, bool singleShot);

        RefPtr<WebCore::DOMWindow> m_impl;
        OwnPtr<WindowPrivate> d;
    };

    class WindowPrototype {
    public:
        static JSValue self(JSContext * ctx);
        static void initPrototype(JSContext * ctx, JSValue this_obj);
    };

    class WindowFunction {
    public:
        static JSValue callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token);
    };

  /**
   * An action (either function or string) to be executed after a specified
   * time interval, either once or repeatedly. Used for window.setTimeout()
   * and window.setInterval()
   */

    class ProtectedJSValue {
    public:
        ProtectedJSValue() : m_ctx(0), m_val(JS_NULL) { }
        ProtectedJSValue(JSContext* ctx, JSValue val)
            : m_ctx(ctx)
        {
            m_val = JS_DupValue(m_ctx, val);
        }

        ~ProtectedJSValue()
        {
            if (!JS_IsNull(m_val)) {
                JS_FreeValue(m_ctx, m_val);
            }
        }

    private:
        JSContext* m_ctx;
        JSValue m_val;
    };

    class JSValueList {
    public:
        JSValueList() : m_ctx(0) { }
        JSValueList(JSContext* ctx) : m_ctx(ctx) { }
        ~JSValueList() { }
        
        void append(const JSValueConst& val)
        {
            JSValue v = JS_DupValue(m_ctx, val); 
            m_list.append(v);
        }

        void release(void)
        {
            for (size_t i = 0; i < m_list.size(); i++) {
                JS_FreeValue(m_ctx, m_list[i]);
            }
            m_list.clear();
        }

    private:
        JSContext* m_ctx;
        WTF::Vector<JSValue> m_list;
    };

    class ScheduledAction {
    public:
        ScheduledAction(JSContext* ctx, JSValue func, const JSValueList& args)
            : m_func(ctx, func), m_args(args) { }

        ScheduledAction(const WebCore::String& code)
            : m_code(code) { }

        ~ScheduledAction() { m_args.release(); }
        void execute(Window *);

    private:
        ProtectedJSValue m_func;
        JSValueList m_args;
        WebCore::String m_code;
    };

    class Location {
    public:
        static void init(JSContext*);
        static JSValue create(JSContext*, WebCore::Frame*);

        static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
        static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);

        static JSClassID js_class_id;

        enum { Hash, Href, Hostname, Host, Pathname, Port, Protocol, Search, 
            Replace, Reload, ToString, Assign };

        static WebCore::Frame* frame(JSValue);
    };

} // namespace

namespace WebCore {

    bool isSafeScript(JSContext*, JSValue);

    JSValue toJS(JSContext*, DOMWindow*);
} // namespace WebCore

#endif
