// -*- c-basic-offset: 4 -*-
/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.
 *  Copyright (C) 2007 Samuel Weinig <sam@webkit.org>
 *  Copyright (C) 2024 Zhang Ji Peng <onecoolx@gmail.com>
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

#include <Shared.h>
#include <text/String.h>

#include <quickjs.h>

#include "global.h"

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
     * We inherit from Interpreter, to save a pointer to the HTML part
     * that the interpreter runs for.
     */
    class ScriptInterpreter : public WebCore::Shared<ScriptInterpreter> {
    public:
        ScriptInterpreter(JSValue global, WebCore::Frame*);
        virtual ~ScriptInterpreter() { }

        static JSValue getDOMObject(void* objectHandle);
        static void putDOMObject(void* objectHandle, JSValue);
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
        void setProcessingTimerCallback(bool timerCallback) { m_timerCallback = timerCallback; }


        JSValue evaluate(const WebCore::String& sourceURL, int startingLineNumber, const UChar* code, int codeLength, JSValue thisV);
        JSValue evaluate(const WebCore::String& sourceURL, int startingLineNumber, const WebCore::String& code, JSValue thisV);
        /**
         * "Smart" window.open policy
         */
        bool wasRunByUserGesture() const;

        WebCore::Event* getCurrentEvent() const { return m_currentEvent; }

        bool shouldInterruptScript() const;

    private:
        JSValue m_globalObject;
        WebCore::Frame* m_frame;
        WebCore::Event* m_currentEvent;
        bool m_timerCallback;
    };

    /**
     * Retrieve from cache, or create, a QJS object around a DOM object
     */
    template<class DOMObj, class QJSDOMObj> inline JSValue cacheDOMObject(JSContext* ctx, DOMObj* domObj)
    {
        if (!domObj)
            return JS_NULL;

        JSValue ret = ScriptInterpreter::getDOMObject(domObj);
        if (!JS_IsNull(ret)) {
            return ret;
        }
        ret = QJSDOMObj::create(ctx, domObj);
        ScriptInterpreter::putDOMObject(domObj, ret);
        return ret;
    }

#if ENABLE(SVG)
    /**
     * Retrieve from cache, or create, a QJS object around a SVG DOM object
     */
    template<class DOMObj, class QJSDOMObj> inline JSValue cacheSVGDOMObject(JSContext* ctx, DOMObj* domObj, WebCore::SVGElement* context)
    {
        if (!domObj)
            return JS_NULL;

        JSValue ret = ScriptInterpreter::getDOMObject(domObj);
        if (!JS_IsNull(ret)) {
            return ret;
        }
        ret = new QJSDOMObj(ctx, domObj, context);
        ScriptInterpreter::putDOMObject(domObj, ret);
        return ret;
    }
#endif

    // Convert a DOM implementation exception code into a JavaScript exception in the execution state.
    void setDOMException(JSContext*, WebCore::ExceptionCode);

    // Helper class to call setDOMException on exit without adding lots of separate calls to that function.
    class DOMExceptionTranslator : Noncopyable {
    public:
        explicit DOMExceptionTranslator(JSContext* ctx) : m_ctx(ctx), m_code(0) { }
        ~DOMExceptionTranslator() { setDOMException(m_ctx, m_code); }
        operator WebCore::ExceptionCode&() { return m_code; }
    private:
        JSContext* m_ctx;
        WebCore::ExceptionCode m_code;
    };


    JSValue jsStringOrNull(JSContext*, const WebCore::String&); // null if the string is null
    JSValue jsStringOrUndefined(JSContext*, const WebCore::String&); // undefined if the string is null
    JSValue jsStringOrFalse(JSContext*, const WebCore::String&); // boolean false if the string is null

    // by another object, so that collecting the JSString wrapper is unlikely to save memory.

    WebCore::String valueToStringWithNullCheck(JSContext*, JSValue); // null String if the value is null
    WebCore::String valueToStringWithUndefinedOrNullCheck(JSContext*, JSValue); // null String if the value is null or undefined
    WebCore::String valueToString(JSContext*, JSValue);
    int32_t valueToInt32(JSContext*, JSValue, bool& ok);
    int32_t valueToInt32(JSContext*, JSValue);
    float valueToFloat(JSContext*, JSValue);
    double valueToDouble(JSContext*, JSValue);
    bool valueToBoolean(JSContext*, JSValue);

    template <typename T> inline JSValue toJS(JSContext* ctx, PassRefPtr<T> ptr) { return toJS(ctx, ptr.get()); }


    static inline void gcProtect(JSContext* ctx, JSValue val) 
    { 
        val = JS_DupValue(ctx, val); 
    }

    static inline void gcUnprotect(JSContext* ctx, JSValue val)
    { 
        JS_FreeValue(ctx, val);
    }

    static inline void gcProtectNullTolerant(JSContext* ctx, JSValue val) 
    {
        if (!JS_IsNull(val)) 
            gcProtect(ctx, val);
    }

    static inline void gcUnprotectNullTolerant(JSContext* ctx, JSValue val) 
    {
        if (!JS_IsNull(val)) 
            gcUnprotect(ctx, val);
    }

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
