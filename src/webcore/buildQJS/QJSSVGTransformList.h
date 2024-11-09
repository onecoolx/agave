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

#ifndef _JSSVGTransformList_H_
#define _JSSVGTransformList_H_


#if ENABLE(SVG)

#include "qjs_binding.h"

namespace WebCore {

class SVGTransformList;

class JSSVGTransformList {
public:
    static void init(JSContext*);
    static JSValue create(JSContext*, SVGTransformList*, SVGElement* context);
    static void finalizer(JSRuntime *rt, JSValue val);

    static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);

    static JSClassID js_class_id;

    static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

    enum {
        // Attributes
        NumberOfItemsAttrNum, 

        // Functions
        ClearFuncNum, InitializeFuncNum, GetItemFuncNum, InsertItemBeforeFuncNum, 
        ReplaceItemFuncNum, RemoveItemFuncNum, AppendItemFuncNum, CreateSVGTransformFromMatrixFuncNum, 
        ConsolidateFuncNum
    };

    // Custom functions
    static JSValue clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, SVGTransformList *impl);
    static JSValue initialize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, SVGTransformList *impl);
    static JSValue getItem(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, SVGTransformList *impl);
    static JSValue insertItemBefore(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, SVGTransformList *impl);
    static JSValue replaceItem(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, SVGTransformList *impl);
    static JSValue removeItem(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, SVGTransformList *impl);
    static JSValue appendItem(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, SVGTransformList *impl);
    SVGTransformList* impl() const { return m_impl.get(); }
    SVGElement* context() const { return m_context.get(); }

private:
    RefPtr<SVGElement> m_context;
    RefPtr<SVGTransformList > m_impl;
};

JSValue toJS(JSContext *ctx, SVGTransformList*, SVGElement* context);
SVGTransformList* toSVGTransformList(JSValue);

class JSSVGTransformListPrototype {
public:
    static JSValue self(JSContext * ctx);
    static void initPrototype(JSContext * ctx, JSValue this_obj);
};

class JSSVGTransformListPrototypeFunction {
public:
    static JSValue callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token);
};

} // namespace WebCore

#endif // ENABLE(SVG)

#endif
