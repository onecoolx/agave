/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#include "QJSHTMLFormElement.h"

#include "HTMLCollection.h"
#include "HTMLFormElement.h"
#include "QJSHTMLCollection.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLFormElementAttributesFunctions[10];
static bool JSHTMLFormElementAttributesFunctions_initialized = false;

static void init_JSHTMLFormElementAttributesFunctions()
{
    if (JSHTMLFormElementAttributesFunctions_initialized) return;
    JSHTMLFormElementAttributesFunctions_initialized = true;
    memset(JSHTMLFormElementAttributesFunctions, 0, sizeof(JSHTMLFormElementAttributesFunctions));
    JSHTMLFormElementAttributesFunctions[0].name = "elements";
    JSHTMLFormElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[0].magic = JSHTMLFormElement::ElementsAttrNum;
    JSHTMLFormElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLFormElementAttributesFunctions[1].name = "length";
    JSHTMLFormElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[1].magic = JSHTMLFormElement::LengthAttrNum;
    JSHTMLFormElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSHTMLFormElementAttributesFunctions[2].name = "name";
    JSHTMLFormElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[2].magic = JSHTMLFormElement::NameAttrNum;
    JSHTMLFormElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLFormElement::putValueProperty;
    JSHTMLFormElementAttributesFunctions[3].name = "acceptCharset";
    JSHTMLFormElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[3].magic = JSHTMLFormElement::AcceptCharsetAttrNum;
    JSHTMLFormElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLFormElement::putValueProperty;
    JSHTMLFormElementAttributesFunctions[4].name = "action";
    JSHTMLFormElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[4].magic = JSHTMLFormElement::ActionAttrNum;
    JSHTMLFormElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLFormElement::putValueProperty;
    JSHTMLFormElementAttributesFunctions[5].name = "encoding";
    JSHTMLFormElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[5].magic = JSHTMLFormElement::EncodingAttrNum;
    JSHTMLFormElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLFormElement::putValueProperty;
    JSHTMLFormElementAttributesFunctions[6].name = "enctype";
    JSHTMLFormElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[6].magic = JSHTMLFormElement::EnctypeAttrNum;
    JSHTMLFormElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLFormElement::putValueProperty;
    JSHTMLFormElementAttributesFunctions[7].name = "method";
    JSHTMLFormElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[7].magic = JSHTMLFormElement::MethodAttrNum;
    JSHTMLFormElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLFormElement::putValueProperty;
    JSHTMLFormElementAttributesFunctions[8].name = "target";
    JSHTMLFormElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[8].magic = JSHTMLFormElement::TargetAttrNum;
    JSHTMLFormElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLFormElement::putValueProperty;
    JSHTMLFormElementAttributesFunctions[9].name = "constructor";
    JSHTMLFormElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFormElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFormElementAttributesFunctions[9].magic = JSHTMLFormElement::ConstructorAttrNum;
    JSHTMLFormElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLFormElement::getValueProperty;
    JSHTMLFormElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
}

class JSHTMLFormElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLFormElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLFormElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLFormElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLFormElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLFormElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLFormElementPrototypeFunctions[2];
static bool JSHTMLFormElementPrototypeFunctions_initialized = false;

static void init_JSHTMLFormElementPrototypeFunctions()
{
    if (JSHTMLFormElementPrototypeFunctions_initialized) return;
    JSHTMLFormElementPrototypeFunctions_initialized = true;
    memset(JSHTMLFormElementPrototypeFunctions, 0, sizeof(JSHTMLFormElementPrototypeFunctions));
    JSHTMLFormElementPrototypeFunctions[0].name = "submit";
    JSHTMLFormElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLFormElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLFormElementPrototypeFunctions[0].magic = JSHTMLFormElement::SubmitFuncNum;
    JSHTMLFormElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLFormElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLFormElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLFormElementPrototypeFunction::callAsFunction;
    JSHTMLFormElementPrototypeFunctions[1].name = "reset";
    JSHTMLFormElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLFormElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLFormElementPrototypeFunctions[1].magic = JSHTMLFormElement::ResetFuncNum;
    JSHTMLFormElementPrototypeFunctions[1].u.func.length = 0;
    JSHTMLFormElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLFormElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLFormElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLFormElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLFormElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLFormElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLFormElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLFormElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLFormElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFormElementAttributesFunctions, countof(JSHTMLFormElementAttributesFunctions));
    init_JSHTMLFormElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFormElementPrototypeFunctions, countof(JSHTMLFormElementPrototypeFunctions));
}

JSClassID JSHTMLFormElement::js_class_id = 0;

void JSHTMLFormElement::init(JSContext* ctx)
{
    if (JSHTMLFormElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLFormElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLFormElement::create(JSContext* ctx, HTMLFormElement* impl)
{
    JSHTMLFormElement::init(ctx);
    JSValue _proto = JSHTMLFormElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLFormElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLFormElement* impl = (HTMLFormElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->elements()));
        }
        case LengthAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
        case NameAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case AcceptCharsetAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->acceptCharset()).utf8().data());
        }
        case ActionAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->action()).utf8().data());
        }
        case EncodingAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->encoding()).utf8().data());
        }
        case EnctypeAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->enctype()).utf8().data());
        }
        case MethodAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->method()).utf8().data());
        }
        case TargetAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AcceptCharsetAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAcceptCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ActionAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAction(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case EncodingAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setEncoding(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case EnctypeAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setEnctype(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MethodAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMethod(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    HTMLFormElement* imp = (HTMLFormElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
