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

#include "QJSHTMLImageElement.h"

#include "HTMLImageElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLImageElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("hspace", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::HspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("src", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::SrcAttrNum),
    JS_CGETSET_MAGIC_DEF("useMap", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::UseMapAttrNum),
    JS_CGETSET_MAGIC_DEF("complete", JSHTMLImageElement::getValueProperty, NULL, JSHTMLImageElement::CompleteAttrNum),
    JS_CGETSET_MAGIC_DEF("lowsrc", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::LowsrcAttrNum),
    JS_CGETSET_MAGIC_DEF("alt", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::AltAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSHTMLImageElement::getValueProperty, NULL, JSHTMLImageElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("isMap", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::IsMapAttrNum),
    JS_CGETSET_MAGIC_DEF("vspace", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::VspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("border", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::BorderAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("longDesc", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::LongDescAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLImageElement::getValueProperty, JSHTMLImageElement::putValueProperty, JSHTMLImageElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("naturalHeight", JSHTMLImageElement::getValueProperty, NULL, JSHTMLImageElement::NaturalHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("naturalWidth", JSHTMLImageElement::getValueProperty, NULL, JSHTMLImageElement::NaturalWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSHTMLImageElement::getValueProperty, NULL, JSHTMLImageElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLImageElement::getValueProperty, NULL, JSHTMLImageElement::ConstructorAttrNum)
};

class JSHTMLImageElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLImageElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLImageElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLImageElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLImageElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLImageElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLImageElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLImageElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLImageElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLImageElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLImageElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLImageElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLImageElementAttributesFunctions, countof(JSHTMLImageElementAttributesFunctions));
}

static JSClassDef JSHTMLImageElementClassDefine = 
{
    "HTMLImageElement",
    .finalizer = JSHTMLImageElement::finalizer,
    .gc_mark = JSHTMLImageElement::mark,
};

JSClassID JSHTMLImageElement::js_class_id = 0;

void JSHTMLImageElement::init(JSContext* ctx)
{
    if (JSHTMLImageElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLImageElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLImageElement::js_class_id, &JSHTMLImageElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLImageElementConstructor::self(ctx), JSHTMLImageElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLImageElement::js_class_id, JSHTMLImageElementPrototype::self(ctx));
    }
}

JSValue JSHTMLImageElement::create(JSContext* ctx, HTMLImageElement* impl)
{
    JSHTMLImageElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLImageElementPrototype::self(ctx), JSHTMLImageElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLImageElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLImageElement* impl = (HTMLImageElement*)JS_GetOpaque(val, JSHTMLImageElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLImageElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLImageElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case AltAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alt()).utf8().data());
        }
        case BorderAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->border()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->height());
        }
        case HspaceAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->hspace());
        }
        case IsMapAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBool(ctx, imp->isMap() ? 1 : 0);
        }
        case LongDescAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->longDesc()).utf8().data());
        }
        case SrcAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case UseMapAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->useMap()).utf8().data());
        }
        case VspaceAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->vspace());
        }
        case WidthAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->width());
        }
        case CompleteAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBool(ctx, imp->complete() ? 1 : 0);
        }
        case LowsrcAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->lowsrc()).utf8().data());
        }
        case NaturalHeightAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->naturalHeight());
        }
        case NaturalWidthAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->naturalWidth());
        }
        case XAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLImageElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AltAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setAlt(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BorderAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setHeight(valueToInt32(ctx, value));
            break;
        }
        case HspaceAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setHspace(valueToInt32(ctx, value));
            break;
        }
        case IsMapAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setIsMap(valueToBoolean(ctx, value));
            break;
        }
        case LongDescAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setLongDesc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setSrc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case UseMapAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setUseMap(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VspaceAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setVspace(valueToInt32(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setWidth(valueToInt32(ctx, value));
            break;
        }
        case LowsrcAttrNum: {
            HTMLImageElement* imp = (HTMLImageElement*)JS_GetOpaque2(ctx, this_val, JSHTMLImageElement::js_class_id);
            imp->setLowsrc(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLImageElement::getConstructor(JSContext *ctx)
{
    return JSHTMLImageElementConstructor::self(ctx);
}


}
