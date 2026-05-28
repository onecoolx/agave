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

static JSCFunctionListEntry JSHTMLFrameElementAttributesFunctions[14];
static bool JSHTMLFrameElementAttributesFunctions_initialized = false;

static void init_JSHTMLFrameElementAttributesFunctions()
{
    if (JSHTMLFrameElementAttributesFunctions_initialized) return;
    JSHTMLFrameElementAttributesFunctions_initialized = true;
    memset(JSHTMLFrameElementAttributesFunctions, 0, sizeof(JSHTMLFrameElementAttributesFunctions));
    JSHTMLFrameElementAttributesFunctions[0].name = "frameBorder";
    JSHTMLFrameElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[0].magic = JSHTMLFrameElement::FrameBorderAttrNum;
    JSHTMLFrameElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[1].name = "longDesc";
    JSHTMLFrameElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[1].magic = JSHTMLFrameElement::LongDescAttrNum;
    JSHTMLFrameElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[2].name = "marginHeight";
    JSHTMLFrameElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[2].magic = JSHTMLFrameElement::MarginHeightAttrNum;
    JSHTMLFrameElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[3].name = "marginWidth";
    JSHTMLFrameElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[3].magic = JSHTMLFrameElement::MarginWidthAttrNum;
    JSHTMLFrameElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[4].name = "name";
    JSHTMLFrameElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[4].magic = JSHTMLFrameElement::NameAttrNum;
    JSHTMLFrameElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[5].name = "noResize";
    JSHTMLFrameElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[5].magic = JSHTMLFrameElement::NoResizeAttrNum;
    JSHTMLFrameElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[6].name = "scrolling";
    JSHTMLFrameElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[6].magic = JSHTMLFrameElement::ScrollingAttrNum;
    JSHTMLFrameElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[7].name = "src";
    JSHTMLFrameElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[7].magic = JSHTMLFrameElement::SrcAttrNum;
    JSHTMLFrameElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[8].name = "contentDocument";
    JSHTMLFrameElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[8].magic = JSHTMLFrameElement::ContentDocumentAttrNum;
    JSHTMLFrameElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSHTMLFrameElementAttributesFunctions[9].name = "contentWindow";
    JSHTMLFrameElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[9].magic = JSHTMLFrameElement::ContentWindowAttrNum;
    JSHTMLFrameElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSHTMLFrameElementAttributesFunctions[10].name = "location";
    JSHTMLFrameElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[10].magic = JSHTMLFrameElement::LocationAttrNum;
    JSHTMLFrameElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLFrameElement::putValueProperty;
    JSHTMLFrameElementAttributesFunctions[11].name = "width";
    JSHTMLFrameElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[11].magic = JSHTMLFrameElement::WidthAttrNum;
    JSHTMLFrameElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSHTMLFrameElementAttributesFunctions[12].name = "height";
    JSHTMLFrameElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[12].magic = JSHTMLFrameElement::HeightAttrNum;
    JSHTMLFrameElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSHTMLFrameElementAttributesFunctions[13].name = "constructor";
    JSHTMLFrameElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFrameElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFrameElementAttributesFunctions[13].magic = JSHTMLFrameElement::ConstructorAttrNum;
    JSHTMLFrameElementAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLFrameElement::getValueProperty;
    JSHTMLFrameElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
}

class JSHTMLFrameElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLFrameElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLFrameElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLFrameElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLFrameElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLFrameElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLFrameElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLFrameElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLFrameElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLFrameElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLFrameElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLFrameElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFrameElementAttributesFunctions, countof(JSHTMLFrameElementAttributesFunctions));
}

JSClassID JSHTMLFrameElement::js_class_id = 0;

void JSHTMLFrameElement::init(JSContext* ctx)
{
    if (JSHTMLFrameElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLFrameElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLFrameElement::create(JSContext* ctx, HTMLFrameElement* impl)
{
    JSHTMLFrameElement::init(ctx);
    JSValue _proto = JSHTMLFrameElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLFrameElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLFrameElement* impl = (HTMLFrameElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->frameBorder()).utf8().data());
        }
        case LongDescAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->longDesc()).utf8().data());
        }
        case MarginHeightAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->marginHeight()).utf8().data());
        }
        case MarginWidthAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->marginWidth()).utf8().data());
        }
        case NameAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case NoResizeAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->noResize() ? 1 : 0);
        }
        case ScrollingAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->scrolling()).utf8().data());
        }
        case SrcAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case ContentDocumentAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return checkNodeSecurity(ctx, imp->contentDocument()) ? toJS(ctx, QJS::getPtr(imp->contentDocument())) : JS_UNDEFINED;
        }
        case ContentWindowAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->contentWindow()));
        }
        case LocationAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->location()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->width());
        }
        case HeightAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->height());
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
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setFrameBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LongDescAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setLongDesc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MarginHeightAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMarginHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MarginWidthAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMarginWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NoResizeAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setNoResize(valueToBoolean(ctx, value));
            break;
        }
        case ScrollingAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setScrolling(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            JSHTMLFrameElement::setSrc(ctx, value, imp);
            break;
        }
        case LocationAttrNum: {
            HTMLFrameElement* imp = (HTMLFrameElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
