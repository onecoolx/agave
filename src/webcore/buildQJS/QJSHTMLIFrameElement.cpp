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

#include "QJSHTMLIFrameElement.h"

#include "Document.h"
#include "HTMLIFrameElement.h"
#include "QJSDocument.h"
#include "qjs_dom.h"
#include "qjs_window.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLIFrameElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("height", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("longDesc", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::LongDescAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLIFrameElement::getValueProperty, NULL, JSHTMLIFrameElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("marginWidth", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::MarginWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("contentDocument", JSHTMLIFrameElement::getValueProperty, NULL, JSHTMLIFrameElement::ContentDocumentAttrNum),
    JS_CGETSET_MAGIC_DEF("frameBorder", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::FrameBorderAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("contentWindow", JSHTMLIFrameElement::getValueProperty, NULL, JSHTMLIFrameElement::ContentWindowAttrNum),
    JS_CGETSET_MAGIC_DEF("marginHeight", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::MarginHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("scrolling", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::ScrollingAttrNum),
    JS_CGETSET_MAGIC_DEF("src", JSHTMLIFrameElement::getValueProperty, JSHTMLIFrameElement::putValueProperty, JSHTMLIFrameElement::SrcAttrNum)
};

class JSHTMLIFrameElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLIFrameElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLIFrameElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLIFrameElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLIFrameElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLIFrameElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLIFrameElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLIFrameElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLIFrameElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLIFrameElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLIFrameElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLIFrameElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLIFrameElementAttributesFunctions, countof(JSHTMLIFrameElementAttributesFunctions));
}

static JSClassDef JSHTMLIFrameElementClassDefine = 
{
    "HTMLIFrameElement",
    .finalizer = JSHTMLIFrameElement::finalizer,
    .gc_mark = JSHTMLIFrameElement::mark,
};

JSClassID JSHTMLIFrameElement::js_class_id = 0;

void JSHTMLIFrameElement::init(JSContext* ctx)
{
    if (JSHTMLIFrameElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLIFrameElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLIFrameElement::js_class_id, &JSHTMLIFrameElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLIFrameElementConstructor::self(ctx), JSHTMLIFrameElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLIFrameElement::js_class_id, JSHTMLIFrameElementPrototype::self(ctx));
    }
}

JSValue JSHTMLIFrameElement::create(JSContext* ctx, HTMLIFrameElement* impl)
{
    JSHTMLIFrameElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLIFrameElementPrototype::self(ctx), JSHTMLIFrameElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLIFrameElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLIFrameElement* impl = (HTMLIFrameElement*)JS_GetOpaque(val, JSHTMLIFrameElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLIFrameElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLIFrameElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case FrameBorderAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->frameBorder()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case LongDescAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->longDesc()).utf8().data());
        }
        case MarginHeightAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->marginHeight()).utf8().data());
        }
        case MarginWidthAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->marginWidth()).utf8().data());
        }
        case NameAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ScrollingAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->scrolling()).utf8().data());
        }
        case SrcAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ContentDocumentAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return checkNodeSecurity(ctx, imp->contentDocument()) ? toJS(ctx, QJS::getPtr(imp->contentDocument())) : JS_UNDEFINED;
        }
        case ContentWindowAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->contentWindow()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLIFrameElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FrameBorderAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setFrameBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LongDescAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setLongDesc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MarginHeightAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setMarginHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MarginWidthAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setMarginWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ScrollingAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setScrolling(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            JSHTMLIFrameElement::setSrc(ctx, value, imp);
            break;
        }
        case WidthAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIFrameElement::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLIFrameElement::getConstructor(JSContext *ctx)
{
    return JSHTMLIFrameElementConstructor::self(ctx);
}


}
