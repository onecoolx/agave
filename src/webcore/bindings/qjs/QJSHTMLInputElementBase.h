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

#ifndef _JSHTMLInputElementBase_H_
#define _JSHTMLInputElementBase_H_

#include "QJSHTMLElement.h"
#include "qjs_binding.h"
#include "qjs_html.h"

namespace WebCore {

    class HTMLInputElement;

    class JSHTMLInputElementBase {
    public:
        static void init(JSContext* ctx);
        static JSValue create(JSContext* ctx, HTMLInputElement* n);
        static void finalizer(JSRuntime *rt, JSValue val);

        static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);
        static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);

        static JSClassID js_class_id;

        static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);

        enum { SetSelectionRange, SelectionStart, SelectionEnd };
    };

    //KJS_DEFINE_PROTOTYPE_WITH_PROTOTYPE(JSHTMLInputElementBasePrototype, JSHTMLElementPrototype)
    class JSHTMLInputElementBasePrototype {
    public:
        static JSValue self(JSContext * ctx);
        static void initPrototype(JSContext * ctx, JSValue this_obj);
    };
} // namespace WebCore

#endif // _JSHTMLInputElementBase_H_
