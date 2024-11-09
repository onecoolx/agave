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

#include "config.h"

#if ENABLE(QJS)
#include "QJSHTMLDocument.h"

#include "Frame.h"
#include "HTMLBodyElement.h"
#include "HTMLCollection.h"
#include "HTMLDocument.h"
#include "HTMLElement.h"
#include "HTMLIFrameElement.h"
#include "HTMLNames.h"
#include "QJSHTMLCollection.h"
#include "qjs_html.h"
#include "qjs_window.h"

namespace WebCore {

using namespace QJS;
using namespace HTMLNames;

JSValue JSHTMLDocument::nameGetter(JSContext *ctx, JSValueConst this_obj, const char* prop)
{
    HTMLDocument* doc = (HTMLDocument*)JS_GetOpaque(this_obj, JSHTMLDocument::js_class_id);

    String name(prop);
    RefPtr<HTMLCollection> collection = doc->documentNamedItems(name);

    unsigned length = collection->length();
    if (!length)
        return JS_UNDEFINED;

    if (length == 1) {
        Node* node = collection->firstItem();

        Frame* frame;
        if (node->hasTagName(iframeTag) && (frame = static_cast<HTMLIFrameElement*>(node)->contentFrame()))
            return Window::retrieve(frame);

        return toJS(ctx, node);
    } 

    return toJS(ctx, collection.get());
}

// Custom attributes

JSValue JSHTMLDocument::all(JSContext *ctx, JSValueConst this_val, HTMLDocument *impl)
{
    // If "all" has been overwritten, return the overwritten value
    JSValue v = JS_GetPropertyStr(ctx, this_val, "all");
    if (!JS_IsException(v)) {
        JS_FreeValue(ctx, v);
        return v;
    }

    return toJS(ctx, impl->all().get());
}

void JSHTMLDocument::setAll(JSContext *ctx, JSValueConst this_val, JSValue value, HTMLDocument *impl)
{
    JS_SetPropertyStr(ctx, this_val, "all", value);
}

// Custom functions
JSValue JSHTMLDocument::open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, HTMLDocument *impl)
{
    // For compatibility with other browsers, pass open calls with more than 2 parameters to the window.
    if (argc > 2) {
        Frame* frame = impl->frame();
        if (frame) {
            JSValue windowObj = Window::retrieve(frame);
            if (!JS_IsUndefined(windowObj)) {
                JSValue protoType = JS_GetPrototype(ctx, windowObj);
                JSValue method = JS_GetPropertyStr(ctx, protoType, "open");

                if (!JS_IsFunction(ctx, method)) {
                    JS_FreeValue(ctx, protoType);
                    JS_FreeValue(ctx, method);
                    return JS_ThrowTypeError(ctx, "Type error"); 
                }

                JSValue result = JS_Call(ctx, method, windowObj, argc, argv);
                JS_FreeValue(ctx, protoType);
                JS_FreeValue(ctx, method);
                return result;
            }
        }
        /* Fix a bug about the return value of document.open method. The original return value is jsUndefined().*/
        return this_val;
    }

    // In the case of two parameters or fewer, do a normal document open.
    impl->open();
    /* Fix a bug about the return value of document.open method. The original return value is jsUndefined().*/
    return this_val;
}

static String writeHelper(JSContext *ctx, int argc, JSValueConst *argv)
{
    // DOM only specifies single string argument, but NS & IE allow multiple
    // or no arguments.
    String str = "";
    for (int i = 0; i < argc; ++i)
        str += valueToString(ctx, argv[i]);
    return str;
}

JSValue JSHTMLDocument::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, HTMLDocument *impl)
{
    impl->write(writeHelper(ctx, argc, argv));
    return JS_UNDEFINED;
}

JSValue JSHTMLDocument::writeln(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, HTMLDocument *impl)
{
    impl->write(writeHelper(ctx, argc, argv) + "\n");
    return JS_UNDEFINED;
}

JSValue JSHTMLDocument::clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, HTMLDocument *impl)
{
    // even IE doesn't support this one...
    // static_cast<HTMLDocument*>(impl())->clear();
    return JS_UNDEFINED;
}

} // namespace WebCore
#endif
