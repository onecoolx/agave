// -*- c-basic-offset: 4 -*-
/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.
 *  Copyright (C) 2007 Samuel Weinig <sam@webkit.org>
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

#ifndef qjs_binding_h
#define qjs_binding_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

#include <quickjs.h>

namespace WebCore {
    class AtomicString;
    class Document;
    class Event;
    class Frame;
    class Node;
    class String;
    class JSNode;

    typedef int ExceptionCode;

#if ENABLE(SVG)
    class SVGElement;
#endif
}

namespace QJS {

    /**
     * Base class for all objects in this binding.
     */
    class DOMObject {
    protected:
        DOMObject()
        {
        }
#ifndef NDEBUG
        virtual ~DOMObject();
#endif
    };

    /**
     * We inherit from Interpreter, to save a pointer to the HTML part
     * that the interpreter runs for.
     * The interpreter also stores the DOM object -> KJS::DOMObject cache.
     */
    class ScriptInterpreter {
    public:
        ScriptInterpreter(JSValue global, WebCore::Frame*);

        static JSValue getDOMObject(ScriptInterpreter*, void* objectHandle);
        static void putDOMObject(ScriptInterpreter*, void* objectHandle, JSValue);
        static void forgetDOMObject(void* objectHandle);

        static JSValue getDOMNodeForDocument(WebCore::Document*, WebCore::Node*);
        static void putDOMNodeForDocument(WebCore::Document*, WebCore::Node*, JSValue);
        static void forgetDOMNodeForDocument(WebCore::Document*, WebCore::Node*);
        static void forgetAllDOMNodesForDocument(WebCore::Document*);
        static void updateDOMNodeDocument(WebCore::Node*, WebCore::Document* oldDoc, WebCore::Document* newDoc);
        static void markDOMNodesForDocument(WebCore::Document*);

        WebCore::Frame* frame() const { return m_frame; }

        JSValue globalObject() const;

        void* globalObjectData() const;
        /**
         * Set the event that is triggering the execution of a script, if any
         */
        void setCurrentEvent(WebCore::Event* event) { m_currentEvent = event; }
        void setInlineCode(bool inlineCode) { m_inlineCode = inlineCode; }
        void setProcessingTimerCallback(bool timerCallback) { m_timerCallback = timerCallback; }

        /**
         * "Smart" window.open policy
         */
        bool wasRunByUserGesture() const;

        //virtual ExecState* globalExec();

        WebCore::Event* getCurrentEvent() const { return m_currentEvent; }

        virtual bool shouldInterruptScript() const;

    private:
        virtual ~ScriptInterpreter() { } // only deref on the base class should delete us
    
        WebCore::Frame* m_frame;
        WebCore::Event* m_currentEvent;
        bool m_inlineCode;
        bool m_timerCallback;
    };

    /**
     * Retrieve from cache, or create, a QJS object around a DOM object
     */
    template<class DOMObj, class QJSDOMObj> inline JSValue cacheDOMObject(JSContext* ctx, DOMObj* domObj)
    {
        if (!domObj)
            return JS_NULL;

        ScriptInterpreter* interp = static_cast<ScriptInterpreter*>(JS_GetContextOpaque(ctx));
        JSValue ret = ScriptInterpreter::getDOMObject(interp, domObj);
        if (!JS_IsNull(ret)) {
            return ret;
        }
        ret = QJSDOMObj::create(ctx, domObj);
        ScriptInterpreter::putDOMObject(interp, domObj, ret);
        return ret;
    }

#if ENABLE(SVG)
    /**
     * Retrieve from cache, or create, a KJS object around a SVG DOM object
     */
    template<class DOMObj, class KJSDOMObj> inline JSValue* cacheSVGDOMObject(ExecState* exec, DOMObj* domObj, WebCore::SVGElement* context)
    {
        if (!domObj)
            return jsNull();
        ScriptInterpreter* interp = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter());
        if (DOMObject* ret = interp->getDOMObject(domObj))
            return ret;
        DOMObject* ret = new KJSDOMObj(exec, domObj, context);
        interp->putDOMObject(domObj, ret);
        return ret;
    }
#endif

    // Convert a DOM implementation exception code into a JavaScript exception in the execution state.
    void setDOMException(JSContext*, WebCore::ExceptionCode);
#if 0

    // Helper class to call setDOMException on exit without adding lots of separate calls to that function.
    class DOMExceptionTranslator : Noncopyable {
    public:
        explicit DOMExceptionTranslator(ExecState* exec) : m_exec(exec), m_code(0) { }
        ~DOMExceptionTranslator() { setDOMException(m_exec, m_code); }
        operator WebCore::ExceptionCode&() { return m_code; }
    private:
        ExecState* m_exec;
        WebCore::ExceptionCode m_code;
    };
#endif

    JSValue jsStringOrNull(JSContext*, const WebCore::String&); // null if the string is null
    JSValue jsStringOrUndefined(JSContext*, const WebCore::String&); // undefined if the string is null
    JSValue jsStringOrFalse(JSContext*, const WebCore::String&); // boolean false if the string is null

    // see JavaScriptCore for explanation should be used for UString that is already owned
    // by another object, so that collecting the JSString wrapper is unlikely to save memory.

    //JSValue* jsOwnedStringOrNull(const KJS::UString&); 

    WebCore::String valueToStringWithNullCheck(JSContext*, JSValue); // null String if the value is null
    WebCore::String valueToStringWithUndefinedOrNullCheck(JSContext*, JSValue); // null String if the value is null or undefined
    WebCore::String valueToString(JSContext*, JSValue);
    int32_t valueToInt32(JSContext*, JSValue, bool& ok);
    int32_t valueToInt32(JSContext*, JSValue);
    float valueToFloat(JSContext*, JSValue);
    double valueToDouble(JSContext*, JSValue);
    bool valueToBoolean(JSContext*, JSValue);

    template <typename T> inline JSValue toJS(JSContext* ctx, PassRefPtr<T> ptr) { return toJS(ctx, ptr.get()); }


    template <typename T> inline T* getPtr(const PassRefPtr<T>& p)
    {
        return p.get();
    }

    template <typename T> inline T* getPtr(T* p)
    {
        return p;
    }
} // namespace
#endif
