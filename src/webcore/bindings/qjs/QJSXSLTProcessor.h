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

#ifndef _JSXSLTProcessor_H_
#define _JSXSLTProcessor_H_

#if ENABLE(XSLT)

#include "qjs_binding.h"

namespace WebCore {
    class XSLTProcessor;
}

// Eventually we should implement XSLTException:
// http://lxr.mozilla.org/seamonkey/source/content/xsl/public/nsIXSLTException.idl
// http://bugs.webkit.org/show_bug.cgi?id=5446

namespace QJS {

    class JSXSLTProcessor {
    public:
        static void init(JSContext* ctx);
        static JSValue create(JSContext* ctx);
        static void finalizer(JSRuntime *rt, JSValue val);

        static JSClassID js_class_id;

        enum {
            ImportStylesheet,
            TransformToFragment,
            TransformToDocument,
            SetParameter,
            GetParameter,
            RemoveParameter,
            ClearParameters,
            Reset
        };

        static WebCore::XSLTProcessor *impl(JSContext* ctx, JSValue val);
    };

    class JSXSLTProcessorConstructor {
    public:
        static JSValue self(JSContext* ctx);
        static JSValue construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv);
    };

    class JSXSLTProcessorPrototypeFunction {
    public:
        static JSValue callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token);
    };

} // namespace QJS

#endif // ENABLE(XSLT)

#endif // _JSXSLTProcessor_H_
