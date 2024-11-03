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

#ifndef _JSSVGPoint_H_
#define _JSSVGPoint_H_


#if ENABLE(SVG)

#include "qjs_binding.h"
#include "FloatPoint.h"
#include "JSSVGPODTypeWrapper.h"

namespace WebCore {

class JSSVGPoint {
public:
    static void init(JSContext*);
    static JSValue create(JSContext*, JSSVGPODTypeWrapper<FloatPoint>*, SVGElement* context);
    static void finalizer(JSRuntime *rt, JSValue val);

    static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
    static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);

    static JSClassID js_class_id;

    static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

    enum {
        // Attributes
        XAttrNum, YAttrNum, 

        // Functions
        MatrixTransformFuncNum
    };
    JSSVGPODTypeWrapper<FloatPoint>* impl() const { return m_impl.get(); }
    SVGElement* context() const { return m_context.get(); }

private:
    RefPtr<SVGElement> m_context;
    RefPtr<JSSVGPODTypeWrapper<FloatPoint> > m_impl;
};

JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<FloatPoint>*, SVGElement* context);
FloatPoint toSVGPoint(JSValue);

class JSSVGPointPrototype {
public:
    static JSValue self(JSContext * ctx);
    static void initPrototype(JSContext * ctx, JSValue this_obj);
};

class JSSVGPointPrototypeFunction {
public:
    static JSValue callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token);
};

} // namespace WebCore

#endif // ENABLE(SVG)

#endif
