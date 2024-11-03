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
#include "QJSSVGElement.h"

#include "QJSSVGElement.h"
#include "QJSSVGSVGElement.h"
#include "SVGElement.h"
#include "SVGSVGElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("id", JSSVGElement::getValueProperty, JSSVGElement::putValueProperty, JSSVGElement::IdAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlbase", JSSVGElement::getValueProperty, JSSVGElement::putValueProperty, JSSVGElement::XmlbaseAttrNum),
    JS_CGETSET_MAGIC_DEF("ownerSVGElement", JSSVGElement::getValueProperty, NULL, JSSVGElement::OwnerSVGElementAttrNum),
    JS_CGETSET_MAGIC_DEF("viewportElement", JSSVGElement::getValueProperty, NULL, JSSVGElement::ViewportElementAttrNum)
};

JSValue JSSVGElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSElementPrototype::self(ctx));
        JSSVGElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGElementAttributesFunctions, countof(JSSVGElementAttributesFunctions));
}

static JSClassDef JSSVGElementClassDefine = 
{
    "SVGElement",
    .finalizer = JSSVGElement::finalizer,
    .gc_mark = JSSVGElement::mark,
};

JSClassID JSSVGElement::js_class_id = 0;

void JSSVGElement::init(JSContext* ctx)
{
    if (JSSVGElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGElement::js_class_id, &JSSVGElementClassDefine);
        JS_SetClassProto(ctx, JSSVGElement::js_class_id, JSSVGElementPrototype::self(ctx));
    }
}

JSValue JSSVGElement::create(JSContext* ctx, SVGElement* impl)
{
    JSSVGElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGElementPrototype::self(ctx), JSSVGElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGElement* impl = (SVGElement*)JS_GetOpaque(val, JSSVGElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSElement::mark(rt, val, mark_func);
}

JSValue JSSVGElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case IdAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->id()).utf8().data());
        }
        case XmlbaseAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlbase()).utf8().data());
        }
        case OwnerSVGElementAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerSVGElement()));
        }
        case ViewportElementAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->viewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case IdAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setId(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case XmlbaseAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setXmlbase(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}


SVGElement* toSVGElement(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGElement* impl = (SVGElement*)JS_GetOpaque(val, JSSVGElement::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
