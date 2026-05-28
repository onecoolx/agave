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

static JSCFunctionListEntry JSHTMLIFrameElementAttributesFunctions[13];
static bool JSHTMLIFrameElementAttributesFunctions_initialized = false;

static void init_JSHTMLIFrameElementAttributesFunctions()
{
    if (JSHTMLIFrameElementAttributesFunctions_initialized) return;
    JSHTMLIFrameElementAttributesFunctions_initialized = true;
    memset(JSHTMLIFrameElementAttributesFunctions, 0, sizeof(JSHTMLIFrameElementAttributesFunctions));
    JSHTMLIFrameElementAttributesFunctions[0].name = "height";
    JSHTMLIFrameElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[0].magic = JSHTMLIFrameElement::HeightAttrNum;
    JSHTMLIFrameElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[1].name = "longDesc";
    JSHTMLIFrameElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[1].magic = JSHTMLIFrameElement::LongDescAttrNum;
    JSHTMLIFrameElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[2].name = "constructor";
    JSHTMLIFrameElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[2].magic = JSHTMLIFrameElement::ConstructorAttrNum;
    JSHTMLIFrameElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSHTMLIFrameElementAttributesFunctions[3].name = "name";
    JSHTMLIFrameElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[3].magic = JSHTMLIFrameElement::NameAttrNum;
    JSHTMLIFrameElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[4].name = "width";
    JSHTMLIFrameElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[4].magic = JSHTMLIFrameElement::WidthAttrNum;
    JSHTMLIFrameElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[5].name = "marginWidth";
    JSHTMLIFrameElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[5].magic = JSHTMLIFrameElement::MarginWidthAttrNum;
    JSHTMLIFrameElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[6].name = "contentDocument";
    JSHTMLIFrameElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[6].magic = JSHTMLIFrameElement::ContentDocumentAttrNum;
    JSHTMLIFrameElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSHTMLIFrameElementAttributesFunctions[7].name = "frameBorder";
    JSHTMLIFrameElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[7].magic = JSHTMLIFrameElement::FrameBorderAttrNum;
    JSHTMLIFrameElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[8].name = "align";
    JSHTMLIFrameElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[8].magic = JSHTMLIFrameElement::AlignAttrNum;
    JSHTMLIFrameElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[9].name = "contentWindow";
    JSHTMLIFrameElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[9].magic = JSHTMLIFrameElement::ContentWindowAttrNum;
    JSHTMLIFrameElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSHTMLIFrameElementAttributesFunctions[10].name = "marginHeight";
    JSHTMLIFrameElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[10].magic = JSHTMLIFrameElement::MarginHeightAttrNum;
    JSHTMLIFrameElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[11].name = "scrolling";
    JSHTMLIFrameElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[11].magic = JSHTMLIFrameElement::ScrollingAttrNum;
    JSHTMLIFrameElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[11].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
    JSHTMLIFrameElementAttributesFunctions[12].name = "src";
    JSHTMLIFrameElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIFrameElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIFrameElementAttributesFunctions[12].magic = JSHTMLIFrameElement::SrcAttrNum;
    JSHTMLIFrameElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLIFrameElement::getValueProperty;
    JSHTMLIFrameElementAttributesFunctions[12].u.getset.set.setter_magic = JSHTMLIFrameElement::putValueProperty;
}

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
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLIFrameElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLIFrameElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLIFrameElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLIFrameElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLIFrameElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLIFrameElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLIFrameElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLIFrameElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLIFrameElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLIFrameElementAttributesFunctions, countof(JSHTMLIFrameElementAttributesFunctions));
}

static JSClassDef JSHTMLIFrameElementClassDefine;
static bool JSHTMLIFrameElementClassDefine_initialized = false;

static void init_JSHTMLIFrameElementClassDefine()
{
    if (JSHTMLIFrameElementClassDefine_initialized) return;
    JSHTMLIFrameElementClassDefine_initialized = true;
    memset(&JSHTMLIFrameElementClassDefine, 0, sizeof(JSHTMLIFrameElementClassDefine));
    JSHTMLIFrameElementClassDefine.class_name = "HTMLIFrameElement";
    JSHTMLIFrameElementClassDefine.finalizer = JSHTMLIFrameElement::finalizer;
    JSHTMLIFrameElementClassDefine.gc_mark = JSHTMLIFrameElement::mark;
}

JSClassID JSHTMLIFrameElement::js_class_id = 0;

void JSHTMLIFrameElement::init(JSContext* ctx)
{
    if (JSHTMLIFrameElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLIFrameElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLIFrameElement::create(JSContext* ctx, HTMLIFrameElement* impl)
{
    JSHTMLIFrameElement::init(ctx);
    JSValue _proto = JSHTMLIFrameElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLIFrameElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLIFrameElement* impl = (HTMLIFrameElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case FrameBorderAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->frameBorder()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case LongDescAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->longDesc()).utf8().data());
        }
        case MarginHeightAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->marginHeight()).utf8().data());
        }
        case MarginWidthAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->marginWidth()).utf8().data());
        }
        case NameAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ScrollingAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->scrolling()).utf8().data());
        }
        case SrcAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ContentDocumentAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return checkNodeSecurity(ctx, imp->contentDocument()) ? toJS(ctx, QJS::getPtr(imp->contentDocument())) : JS_UNDEFINED;
        }
        case ContentWindowAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FrameBorderAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setFrameBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LongDescAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setLongDesc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MarginHeightAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMarginHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MarginWidthAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMarginWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ScrollingAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setScrolling(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            JSHTMLIFrameElement::setSrc(ctx, value, imp);
            break;
        }
        case WidthAttrNum: {
            HTMLIFrameElement* imp = (HTMLIFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
