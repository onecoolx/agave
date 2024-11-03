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

#include "QJSHTMLFormElement.h"

#include "HTMLCollection.h"
#include "HTMLFormElement.h"
#include "QJSHTMLCollection.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLFormElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("action", JSHTMLFormElement::getValueProperty, JSHTMLFormElement::putValueProperty, JSHTMLFormElement::ActionAttrNum),
    JS_CGETSET_MAGIC_DEF("target", JSHTMLFormElement::getValueProperty, JSHTMLFormElement::putValueProperty, JSHTMLFormElement::TargetAttrNum),
    JS_CGETSET_MAGIC_DEF("method", JSHTMLFormElement::getValueProperty, JSHTMLFormElement::putValueProperty, JSHTMLFormElement::MethodAttrNum),
    JS_CGETSET_MAGIC_DEF("length", JSHTMLFormElement::getValueProperty, NULL, JSHTMLFormElement::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLFormElement::getValueProperty, NULL, JSHTMLFormElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("elements", JSHTMLFormElement::getValueProperty, NULL, JSHTMLFormElement::ElementsAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLFormElement::getValueProperty, JSHTMLFormElement::putValueProperty, JSHTMLFormElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("acceptCharset", JSHTMLFormElement::getValueProperty, JSHTMLFormElement::putValueProperty, JSHTMLFormElement::AcceptCharsetAttrNum),
    JS_CGETSET_MAGIC_DEF("encoding", JSHTMLFormElement::getValueProperty, JSHTMLFormElement::putValueProperty, JSHTMLFormElement::EncodingAttrNum),
    JS_CGETSET_MAGIC_DEF("enctype", JSHTMLFormElement::getValueProperty, JSHTMLFormElement::putValueProperty, JSHTMLFormElement::EnctypeAttrNum)
};

class JSHTMLFormElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLFormElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLFormElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLFormElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLFormElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLFormElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLFormElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLFormElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("submit", 0, JSHTMLFormElementPrototypeFunction::callAsFunction, JSHTMLFormElement::SubmitFuncNum),
    JS_CFUNC_MAGIC_DEF("reset", 0, JSHTMLFormElementPrototypeFunction::callAsFunction, JSHTMLFormElement::ResetFuncNum)
};

JSValue JSHTMLFormElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLFormElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLFormElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLFormElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLFormElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFormElementAttributesFunctions, countof(JSHTMLFormElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFormElementPrototypeFunctions, countof(JSHTMLFormElementPrototypeFunctions));
}

static JSClassDef JSHTMLFormElementClassDefine = 
{
    "HTMLFormElement",
    .finalizer = JSHTMLFormElement::finalizer,
    .gc_mark = JSHTMLFormElement::mark,
};

JSClassID JSHTMLFormElement::js_class_id = 0;

void JSHTMLFormElement::init(JSContext* ctx)
{
    if (JSHTMLFormElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLFormElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLFormElement::js_class_id, &JSHTMLFormElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLFormElementConstructor::self(ctx), JSHTMLFormElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLFormElement::js_class_id, JSHTMLFormElementPrototype::self(ctx));
    }
}

JSValue JSHTMLFormElement::create(JSContext* ctx, HTMLFormElement* impl)
{
    JSHTMLFormElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLFormElementPrototype::self(ctx), JSHTMLFormElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLFormElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLFormElement* impl = (HTMLFormElement*)JS_GetOpaque(val, JSHTMLFormElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLFormElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLFormElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ElementsAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->elements()));
        }
        case LengthAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case NameAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case AcceptCharsetAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->acceptCharset()).utf8().data());
        }
        case ActionAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->action()).utf8().data());
        }
        case EncodingAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->encoding()).utf8().data());
        }
        case EnctypeAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->enctype()).utf8().data());
        }
        case MethodAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->method()).utf8().data());
        }
        case TargetAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLFormElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AcceptCharsetAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            imp->setAcceptCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ActionAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            imp->setAction(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case EncodingAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            imp->setEncoding(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case EnctypeAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            imp->setEnctype(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MethodAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            imp->setMethod(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
            imp->setTarget(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLFormElement::getConstructor(JSContext *ctx)
{
    return JSHTMLFormElementConstructor::self(ctx);
}

JSValue JSHTMLFormElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFormElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLFormElement::SubmitFuncNum: {
            imp->submit();
            return JS_UNDEFINED;
        }
        case JSHTMLFormElement::ResetFuncNum: {
            imp->reset();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue JSHTMLFormElement::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque2(ctx, this_obj, JSHTMLFormElement::js_class_id);
    return toJS(ctx, imp->item(idx));
}


}
