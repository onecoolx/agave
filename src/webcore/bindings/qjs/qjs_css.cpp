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
#include <string.h>

#if ENABLE(QJS)
#include "qjs_css.h"

#include "CSSPrimitiveValue.h"
#include "QJSCSSPrimitiveValue.h"
#include "qjs_dom.h"

namespace WebCore {

using namespace QJS;

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSCFunctionListEntry JSRGBColorAttributesFunctions[3];
static bool JSRGBColorAttributesFunctions_inited = false;

static void init_JSRGBColorAttributesFunctions()
{
    if (JSRGBColorAttributesFunctions_inited) return;
    JSRGBColorAttributesFunctions_inited = true;
    memset(JSRGBColorAttributesFunctions, 0, sizeof(JSRGBColorAttributesFunctions));
    JSRGBColorAttributesFunctions[0].name = "blue";
    JSRGBColorAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSRGBColorAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSRGBColorAttributesFunctions[0].magic = JSRGBColor::Blue;
    JSRGBColorAttributesFunctions[0].u.getset.get.getter_magic = JSRGBColor::getValueProperty;
    JSRGBColorAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSRGBColorAttributesFunctions[1].name = "red";
    JSRGBColorAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSRGBColorAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSRGBColorAttributesFunctions[1].magic = JSRGBColor::Red;
    JSRGBColorAttributesFunctions[1].u.getset.get.getter_magic = JSRGBColor::getValueProperty;
    JSRGBColorAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSRGBColorAttributesFunctions[2].name = "green";
    JSRGBColorAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSRGBColorAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSRGBColorAttributesFunctions[2].magic = JSRGBColor::Green;
    JSRGBColorAttributesFunctions[2].u.getset.get.getter_magic = JSRGBColor::getValueProperty;
    JSRGBColorAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

static JSClassDef JSRGBColorClassDefine;
static bool JSRGBColorClassDefine_inited = false;

static void init_JSRGBColorClassDefine()
{
    if (JSRGBColorClassDefine_inited) return;
    JSRGBColorClassDefine_inited = true;
    memset(&JSRGBColorClassDefine, 0, sizeof(JSRGBColorClassDefine));
    JSRGBColorClassDefine.class_name = "RGBColor";
}

JSClassID JSRGBColor::js_class_id = 0;

void JSRGBColor::init(JSContext* ctx)
{
    if (JSRGBColor::js_class_id == 0) {
        init_JSRGBColorClassDefine();
        JS_NewClassID(&JSRGBColor::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSRGBColor::js_class_id, &JSRGBColorClassDefine);

        init_JSRGBColorAttributesFunctions();
        JSValue proto = JS_NewObject(ctx);
        JS_SetPropertyFunctionList(ctx, proto, JSRGBColorAttributesFunctions, countof(JSRGBColorAttributesFunctions));

        JS_SetClassProto(ctx, JSRGBColor::js_class_id, proto);
    }
}

JSValue JSRGBColor::create(JSContext* ctx, uint32_t color)
{
    JSRGBColor::init(ctx);
    JSValue obj = JS_NewObjectClass(ctx, JSRGBColor::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, reinterpret_cast<void*>(color));
    return obj;
}

JSValue JSRGBColor::getValueProperty(JSContext* ctx, JSValueConst this_val, int token)
{
    uint32_t color = (uintptr_t)JS_GetOpaque2(ctx, this_val, JSRGBColor::js_class_id);
    switch (token) {
        case Red:
            color >>= 8;
            // fall through
        case Green:
            color >>= 8;
            // fall through
        case Blue:
            return toJS(ctx, new CSSPrimitiveValue(color & 0xFF, CSSPrimitiveValue::CSS_NUMBER));
        default:
            return JS_NULL;
    }
}

JSValue getJSRGBColor(JSContext* ctx, uint32_t color)
{
    return JSRGBColor::create(ctx, color);
}

} // namespace WebCore
#endif
