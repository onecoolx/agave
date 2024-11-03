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

#include "QJSHTMLFrameElement.h"

#include "Document.h"
#include "HTMLFrameElement.h"
#include "QJSDocument.h"
#include "qjs_dom.h"
#include "qjs_window.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLFrameElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("scrolling", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::ScrollingAttrNum),
    JS_CGETSET_MAGIC_DEF("longDesc", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::LongDescAttrNum),
    JS_CGETSET_MAGIC_DEF("location", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::LocationAttrNum),
    JS_CGETSET_MAGIC_DEF("marginWidth", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::MarginWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("marginHeight", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::MarginHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("frameBorder", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::FrameBorderAttrNum),
    JS_CGETSET_MAGIC_DEF("noResize", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::NoResizeAttrNum),
    JS_CGETSET_MAGIC_DEF("src", JSHTMLFrameElement::getValueProperty, JSHTMLFrameElement::putValueProperty, JSHTMLFrameElement::SrcAttrNum),
    JS_CGETSET_MAGIC_DEF("contentDocument", JSHTMLFrameElement::getValueProperty, NULL, JSHTMLFrameElement::ContentDocumentAttrNum),
    JS_CGETSET_MAGIC_DEF("contentWindow", JSHTMLFrameElement::getValueProperty, NULL, JSHTMLFrameElement::ContentWindowAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLFrameElement::getValueProperty, NULL, JSHTMLFrameElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSHTMLFrameElement::getValueProperty, NULL, JSHTMLFrameElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLFrameElement::getValueProperty, NULL, JSHTMLFrameElement::ConstructorAttrNum)
};

class JSHTMLFrameElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLFrameElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLFrameElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLFrameElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLFrameElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLFrameElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLFrameElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLFrameElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLFrameElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLFrameElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLFrameElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLFrameElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFrameElementAttributesFunctions, countof(JSHTMLFrameElementAttributesFunctions));
}

static JSClassDef JSHTMLFrameElementClassDefine = 
{
    "HTMLFrameElement",
    .finalizer = JSHTMLFrameElement::finalizer,
    .gc_mark = JSHTMLFrameElement::mark,
};

JSClassID JSHTMLFrameElement::js_class_id = 0;

void JSHTMLFrameElement::init(JSContext* ctx)
{
    if (JSHTMLFrameElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLFrameElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLFrameElement::js_class_id, &JSHTMLFrameElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLFrameElementConstructor::self(ctx), JSHTMLFrameElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLFrameElement::js_class_id, JSHTMLFrameElementPrototype::self(ctx));
    }
}

JSValue JSHTMLFrameElement::create(JSContext* ctx, HTMLFrameElement* impl)
{
    JSHTMLFrameElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLFrameElementPrototype::self(ctx), JSHTMLFrameElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLFrameElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLFrameElement* impl = (HTMLFrameElement*)JS_GetOpaque(val, JSHTMLFrameElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLFrameElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLFrameElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FrameBorderAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->frameBorder()).utf8().data());
        }
        case LongDescAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->longDesc()).utf8().data());
        }
        case MarginHeightAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->marginHeight()).utf8().data());
        }
        case MarginWidthAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->marginWidth()).utf8().data());
        }
        case NameAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case NoResizeAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewBool(ctx, imp->noResize() ? 1 : 0);
        }
        case ScrollingAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->scrolling()).utf8().data());
        }
        case SrcAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case ContentDocumentAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return checkNodeSecurity(ctx, imp->contentDocument()) ? toJS(ctx, QJS::getPtr(imp->contentDocument())) : JS_UNDEFINED;
        }
        case ContentWindowAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->contentWindow()));
        }
        case LocationAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->location()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->width());
        }
        case HeightAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->height());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLFrameElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case FrameBorderAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            imp->setFrameBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LongDescAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            imp->setLongDesc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MarginHeightAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            imp->setMarginHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MarginWidthAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            imp->setMarginWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NoResizeAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            imp->setNoResize(valueToBoolean(ctx, value));
            break;
        }
        case ScrollingAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            imp->setScrolling(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            JSHTMLFrameElement::setSrc(ctx, value, imp);
            break;
        }
        case LocationAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameElement::js_class_id);
            JSHTMLFrameElement::setLocation(ctx, value, imp);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLFrameElement::getConstructor(JSContext *ctx)
{
    return JSHTMLFrameElementConstructor::self(ctx);
}


}
