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
            { PausedTimeout *a = m_array; m_array = 0; return a; }

    private:
        PausedTimeout *m_array;
        size_t m_length;

        PausedTimeouts(const PausedTimeouts&);
        PausedTimeouts& operator=(const PausedTimeouts&);
    };

    class DOMWindowTimer;

  struct WindowPrivate;

  class Window : public DOMObject {
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
    static JSValue* retrieve(WebCore::Frame*);
    /**
     * Returns the Window object for a given HTML frame
     */
    static Window* retrieveWindow(WebCore::Frame*);
    /**
     * returns a pointer to the Window object this javascript interpreting instance
     * was called from.
     */
    static Window* retrieveActive(JSContext*);

#if 0
    virtual void mark();
    JSValue *getValueProperty(ExecState *exec, int token) const;
    virtual void put(ExecState *exec, const Identifier &propertyName, JSValue *value, int attr = None);

    int installTimeout(const UString& handler, int t, bool singleShot);
    int installTimeout(JSValue* function, const List& args, int t, bool singleShot);
#endif
    void clearTimeout(int timerId, bool delAction = true);
    PausedTimeouts* pauseTimeouts();
    void resumeTimeouts(PausedTimeouts*);

    void timerFired(DOMWindowTimer*);
    
#if 0
    KJS::ScriptInterpreter *interpreter() const;
#endif
        
    static bool isSafeScript(const ScriptInterpreter *origin, const ScriptInterpreter *target);

    Location* location() const;

    // Finds a wrapper of a JS EventListener, returns 0 if no existing one.
    WebCore::JSEventListener* findJSEventListener(JSValue*, bool html = false);

    // Finds or creates a wrapper of a JS EventListener. JS EventListener object is GC-protected.
    WebCore::JSEventListener *findOrCreateJSEventListener(JSValue*, bool html = false);

    // Finds a wrapper of a GC-unprotected JS EventListener, returns 0 if no existing one.
    WebCore::JSUnprotectedEventListener* findJSUnprotectedEventListener(JSValue*, bool html = false);

    // Finds or creates a wrapper of a JS EventListener. JS EventListener object is *NOT* GC-protected.
    WebCore::JSUnprotectedEventListener *findOrCreateJSUnprotectedEventListener(JSValue*, bool html = false);

    void clear();

    // Set the current "event" object
    void setCurrentEvent(WebCore::Event*);

#if 0
    // Set a place to put a dialog return value when the window is cleared.
    void setReturnValueSlot(JSValue** slot);

    typedef HashMap<JSObject*, WebCore::JSEventListener*> ListenersMap;
    typedef HashMap<JSObject*, WebCore::JSUnprotectedEventListener*> UnprotectedListenersMap;
    
    ListenersMap& jsEventListeners();
    ListenersMap& jsHTMLEventListeners();
    UnprotectedListenersMap& jsUnprotectedEventListeners();
    UnprotectedListenersMap& jsUnprotectedHTMLEventListeners();
    
    //virtual const ClassInfo* classInfo() const { return &info; }
    //static const ClassInfo info;
#endif

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
#if 0
    JSValue* getListener(ExecState*, const WebCore::AtomicString& eventType) const;
    void setListener(ExecState*, const WebCore::AtomicString& eventType, JSValue* func);

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

#if 0
  KJS_IMPLEMENT_PROTOTYPE_FUNCTION(WindowFunc)
#endif

  /**
   * An action (either function or string) to be executed after a specified
   * time interval, either once or repeatedly. Used for window.setTimeout()
   * and window.setInterval()
   */
    class ScheduledAction {
    public:
#if 0
        ScheduledAction(JSValue *func, const List& args)
            : m_func(func), m_args(args) { }
#endif
        ScheduledAction(const WebCore::String& code)
            : m_code(code) { }
        void execute(Window *);

    private:
#if 0
        ScheduledAction(JSValue *func, const List& args)
        ProtectedPtr<JSValue> m_func;
        List m_args;
#endif
        WebCore::String m_code;
    };

  class Location {
  public:
    static void init(JSContext*);
    static JSValue create(JSContext*, WebCore::Frame*);
    static void finalizer(JSRuntime *rt, JSValue val);

    static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
    static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);

    static JSClassID js_class_id;

    static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

    enum { Hash, Href, Hostname, Host, Pathname, Port, Protocol, Search, 
           Replace, Reload, ToString, Assign };

#if 0
    static WebCore::Frame* frame(JSContext *ctx, JSValue);
#endif
  };

} // namespace

namespace WebCore {

    bool isSafeScript(JSContext*, JSValue);

    JSValue toJS(JSContext*, DOMWindow*);
} // namespace WebCore

#endif
