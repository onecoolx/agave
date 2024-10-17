/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
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

#ifndef JSEventTargetNode_h
#define JSEventTargetNode_h

#include "QJSNode.h"

namespace WebCore {

    class EventTargetNode;
    class Node;

    class JSEventTargetNode {
    public:
        static void init(JSContext* ctx);
        static JSValue create(JSContext* ctx, Node* n);
        static void finalizer(JSRuntime *rt, JSValue val);

        //void setListener(KJS::ExecState*, const AtomicString& eventType, KJS::JSValue* func) const;
        //KJS::JSValue* getListener(const AtomicString& eventType) const;
        //virtual void pushEventHandlerScope(KJS::ExecState*, KJS::ScopeChain&) const;

        static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
        static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);

        static JSClassID js_class_id;

        static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

        enum {  
            AddEventListener, RemoveEventListener, DispatchEvent,
            OnAbort, OnBlur, OnChange, OnClick, OnContextMenu, OnDblClick, OnError,
            OnDragEnter, OnDragOver, OnDragLeave, OnDrop, OnDragStart, OnDrag, OnDragEnd,
            OnBeforeCut, OnCut, OnBeforeCopy, OnCopy, OnBeforePaste, OnPaste, OnSelectStart,
            OnFocus, OnInput, OnKeyDown, OnKeyPress, OnKeyUp, OnLoad, OnMouseDown,
            OnMouseMove, OnMouseOut, OnMouseOver, OnMouseUp, OnMouseWheel, OnReset,
            OnResize, OnScroll, OnSearch, OnSelect, OnSubmit, OnUnload
        };
    };

    EventTargetNode* toEventTargetNode(JSValue val);

    class JSEventTargetNodePrototype {
    public:
        static JSValue self(JSContext * ctx);
        static void initPrototype(JSContext * ctx, JSValue this_obj);
    };

} // namespace WebCore

#endif // JSEventTargetNode_h
