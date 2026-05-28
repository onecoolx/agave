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

#include "QJSHTMLLinkElement.h"

#include "HTMLLinkElement.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLLinkElementAttributesFunctions[11];
static bool JSHTMLLinkElementAttributesFunctions_initialized = false;

static void init_JSHTMLLinkElementAttributesFunctions()
{
    if (JSHTMLLinkElementAttributesFunctions_initialized) return;
    JSHTMLLinkElementAttributesFunctions_initialized = true;
    memset(JSHTMLLinkElementAttributesFunctions, 0, sizeof(JSHTMLLinkElementAttributesFunctions));
    JSHTMLLinkElementAttributesFunctions[0].name = "disabled";
    JSHTMLLinkElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[0].magic = JSHTMLLinkElement::DisabledAttrNum;
    JSHTMLLinkElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[1].name = "charset";
    JSHTMLLinkElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[1].magic = JSHTMLLinkElement::CharsetAttrNum;
    JSHTMLLinkElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[2].name = "href";
    JSHTMLLinkElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[2].magic = JSHTMLLinkElement::HrefAttrNum;
    JSHTMLLinkElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[3].name = "hreflang";
    JSHTMLLinkElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[3].magic = JSHTMLLinkElement::HreflangAttrNum;
    JSHTMLLinkElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[4].name = "media";
    JSHTMLLinkElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[4].magic = JSHTMLLinkElement::MediaAttrNum;
    JSHTMLLinkElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[5].name = "rel";
    JSHTMLLinkElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[5].magic = JSHTMLLinkElement::RelAttrNum;
    JSHTMLLinkElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[6].name = "rev";
    JSHTMLLinkElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[6].magic = JSHTMLLinkElement::RevAttrNum;
    JSHTMLLinkElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[7].name = "target";
    JSHTMLLinkElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[7].magic = JSHTMLLinkElement::TargetAttrNum;
    JSHTMLLinkElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[8].name = "type";
    JSHTMLLinkElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[8].magic = JSHTMLLinkElement::TypeAttrNum;
    JSHTMLLinkElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLLinkElement::putValueProperty;
    JSHTMLLinkElementAttributesFunctions[9].name = "sheet";
    JSHTMLLinkElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[9].magic = JSHTMLLinkElement::SheetAttrNum;
    JSHTMLLinkElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSHTMLLinkElementAttributesFunctions[10].name = "constructor";
    JSHTMLLinkElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLinkElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLinkElementAttributesFunctions[10].magic = JSHTMLLinkElement::ConstructorAttrNum;
    JSHTMLLinkElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLLinkElement::getValueProperty;
    JSHTMLLinkElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

class JSHTMLLinkElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLLinkElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLLinkElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLLinkElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLLinkElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLLinkElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLLinkElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLLinkElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLLinkElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLLinkElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLLinkElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLLinkElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLinkElementAttributesFunctions, countof(JSHTMLLinkElementAttributesFunctions));
}

static JSClassDef JSHTMLLinkElementClassDefine;
static bool JSHTMLLinkElementClassDefine_initialized = false;

static void init_JSHTMLLinkElementClassDefine()
{
    if (JSHTMLLinkElementClassDefine_initialized) return;
    JSHTMLLinkElementClassDefine_initialized = true;
    memset(&JSHTMLLinkElementClassDefine, 0, sizeof(JSHTMLLinkElementClassDefine));
    JSHTMLLinkElementClassDefine.class_name = "HTMLLinkElement";
    JSHTMLLinkElementClassDefine.finalizer = JSHTMLLinkElement::finalizer;
    JSHTMLLinkElementClassDefine.gc_mark = JSHTMLLinkElement::mark;
}

JSClassID JSHTMLLinkElement::js_class_id = 0;

void JSHTMLLinkElement::init(JSContext* ctx)
{
    if (JSHTMLLinkElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLLinkElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLLinkElement::create(JSContext* ctx, HTMLLinkElement* impl)
{
    JSHTMLLinkElement::init(ctx);
    JSValue _proto = JSHTMLLinkElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLLinkElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLLinkElement* impl = (HTMLLinkElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLLinkElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLLinkElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case CharsetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->charset()).utf8().data());
        }
        case HrefAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->href()).utf8().data());
        }
        case HreflangAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hreflang()).utf8().data());
        }
        case MediaAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->media()).utf8().data());
        }
        case RelAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rel()).utf8().data());
        }
        case RevAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rev()).utf8().data());
        }
        case TargetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case SheetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->sheet()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLLinkElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case CharsetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HrefAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHref(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HreflangAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHreflang(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MediaAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMedia(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RelAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setRel(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RevAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setRev(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTarget(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLLinkElement::getConstructor(JSContext *ctx)
{
    return JSHTMLLinkElementConstructor::self(ctx);
}


}
