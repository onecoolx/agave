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


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGStyleElement.h"

#include "SVGStyleElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGStyleElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGStyleElement::getValueProperty, JSSVGStyleElement::putValueProperty, JSSVGStyleElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("media", JSSVGStyleElement::getValueProperty, JSSVGStyleElement::putValueProperty, JSSVGStyleElement::MediaAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSSVGStyleElement::getValueProperty, JSSVGStyleElement::putValueProperty, JSSVGStyleElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("title", JSSVGStyleElement::getValueProperty, JSSVGStyleElement::putValueProperty, JSSVGStyleElement::TitleAttrNum)
};

JSValue JSSVGStyleElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGStyleElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGStyleElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGStyleElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGStyleElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStyleElementAttributesFunctions, countof(JSSVGStyleElementAttributesFunctions));
}

static JSClassDef JSSVGStyleElementClassDefine = 
{
    "SVGStyleElement",
    .finalizer = JSSVGStyleElement::finalizer,
    .gc_mark = JSSVGStyleElement::mark,
};

JSClassID JSSVGStyleElement::js_class_id = 0;

void JSSVGStyleElement::init(JSContext* ctx)
{
    if (JSSVGStyleElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGStyleElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGStyleElement::js_class_id, &JSSVGStyleElementClassDefine);
        JS_SetClassProto(ctx, JSSVGStyleElement::js_class_id, JSSVGStyleElementPrototype::self(ctx));
    }
}

JSValue JSSVGStyleElement::create(JSContext* ctx, SVGStyleElement* impl)
{
    JSSVGStyleElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGStyleElementPrototype::self(ctx), JSSVGStyleElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGStyleElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGStyleElement* impl = (SVGStyleElement*)JS_GetOpaque(val, JSSVGStyleElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGStyleElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGStyleElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XmlspaceAttrNum: {
            SVGStyleElement* imp = (SVGStyleElement*)JS_GetOpaque2(ctx, this_val, JSSVGStyleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case TypeAttrNum: {
            SVGStyleElement* imp = (SVGStyleElement*)JS_GetOpaque2(ctx, this_val, JSSVGStyleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case MediaAttrNum: {
            SVGStyleElement* imp = (SVGStyleElement*)JS_GetOpaque2(ctx, this_val, JSSVGStyleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->media()).utf8().data());
        }
        case TitleAttrNum: {
            SVGStyleElement* imp = (SVGStyleElement*)JS_GetOpaque2(ctx, this_val, JSSVGStyleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->title()).utf8().data());
        }
    }
    return JS_NULL;
}

JSValue JSSVGStyleElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmlspaceAttrNum: {
            SVGStyleElement* imp = (SVGStyleElement*)JS_GetOpaque2(ctx, this_val, JSSVGStyleElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setXmlspace(valueToString(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case TypeAttrNum: {
            SVGStyleElement* imp = (SVGStyleElement*)JS_GetOpaque2(ctx, this_val, JSSVGStyleElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setType(valueToString(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case MediaAttrNum: {
            SVGStyleElement* imp = (SVGStyleElement*)JS_GetOpaque2(ctx, this_val, JSSVGStyleElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setMedia(valueToString(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case TitleAttrNum: {
            SVGStyleElement* imp = (SVGStyleElement*)JS_GetOpaque2(ctx, this_val, JSSVGStyleElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setTitle(valueToString(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}


}

#endif // ENABLE(SVG)
