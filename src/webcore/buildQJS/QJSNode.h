/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#ifndef _JSNode_H_
#define _JSNode_H_

#include "qjs_binding.h"

namespace WebCore {

class Node;
class Element;
class Attr;
class EventTargetNode;

class JSNode {
public:
    static void init(JSContext*);
    static JSValue create(JSContext*, Node*);
    static void finalizer(JSRuntime *rt, JSValue val);

    static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
    static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);

    static JSClassID js_class_id;

    static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

    static JSValue getConstructor(JSContext *ctx);

    enum {
        // Attributes
        NodeNameAttrNum, NodeValueAttrNum, NodeTypeAttrNum, ParentNodeAttrNum, 
        ChildNodesAttrNum, FirstChildAttrNum, LastChildAttrNum, PreviousSiblingAttrNum, 
        NextSiblingAttrNum, AttributesAttrNum, OwnerDocumentAttrNum, NamespaceURIAttrNum, 
        PrefixAttrNum, LocalNameAttrNum, BaseURIAttrNum, TextContentAttrNum, 
        ParentElementAttrNum, 

        // The Constructor Attribute
        ConstructorAttrNum, 

        // Functions
        InsertBeforeFuncNum, ReplaceChildFuncNum, RemoveChildFuncNum, AppendChildFuncNum, 
        HasChildNodesFuncNum, CloneNodeFuncNum, NormalizeFuncNum, IsSupportedFuncNum, 
        HasAttributesFuncNum, IsSameNodeFuncNum, IsEqualNodeFuncNum, LookupPrefixFuncNum, 
        IsDefaultNamespaceFuncNum, LookupNamespaceURIFuncNum
    };

    // Custom functions
    static JSValue insertBefore(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, Node *impl);
    static JSValue replaceChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, Node *impl);
    static JSValue removeChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, Node *impl);
    static JSValue appendChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, Node *impl);
};

JSValue toJS(JSContext *ctx, Node* ptr);
inline JSValue toJS(JSContext *ctx, PassRefPtr<Node> ptr) { return toJS(ctx, ptr.get()); }
inline JSValue toJS(JSContext *ctx, Element* ptr) { return toJS(ctx, static_cast<Node*>(ptr)); }
inline JSValue toJS(JSContext *ctx, Attr* ptr) { return toJS(ctx, static_cast<Node*>(ptr)); }
inline JSValue toJS(JSContext *ctx, EventTargetNode* ptr) { return toJS(ctx, static_cast<Node*>(ptr)); }
Node* toNode(JSValue);

class JSNodePrototype {
public:
    static JSValue self(JSContext * ctx);
    static void initPrototype(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext *ctx, JSValueConst this_val, int token);
};

class JSNodePrototypeFunction {
public:
    static JSValue callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token);
};

} // namespace WebCore

#endif
