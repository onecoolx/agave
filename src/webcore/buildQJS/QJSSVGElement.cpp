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

static JSCFunctionListEntry JSSVGElementAttributesFunctions[4];
static bool JSSVGElementAttributesFunctions_initialized = false;

static void init_JSSVGElementAttributesFunctions()
{
    if (JSSVGElementAttributesFunctions_initialized) return;
    JSSVGElementAttributesFunctions_initialized = true;
    memset(JSSVGElementAttributesFunctions, 0, sizeof(JSSVGElementAttributesFunctions));
    JSSVGElementAttributesFunctions[0].name = "id";
    JSSVGElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementAttributesFunctions[0].magic = JSSVGElement::IdAttrNum;
    JSSVGElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGElement::getValueProperty;
    JSSVGElementAttributesFunctions[0].u.getset.set.setter_magic = JSSVGElement::putValueProperty;
    JSSVGElementAttributesFunctions[1].name = "xmlbase";
    JSSVGElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementAttributesFunctions[1].magic = JSSVGElement::XmlbaseAttrNum;
    JSSVGElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGElement::getValueProperty;
    JSSVGElementAttributesFunctions[1].u.getset.set.setter_magic = JSSVGElement::putValueProperty;
    JSSVGElementAttributesFunctions[2].name = "ownerSVGElement";
    JSSVGElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementAttributesFunctions[2].magic = JSSVGElement::OwnerSVGElementAttrNum;
    JSSVGElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGElement::getValueProperty;
    JSSVGElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGElementAttributesFunctions[3].name = "viewportElement";
    JSSVGElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementAttributesFunctions[3].magic = JSSVGElement::ViewportElementAttrNum;
    JSSVGElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGElement::getValueProperty;
    JSSVGElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSElementPrototype::self(ctx));
        JSSVGElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGElementAttributesFunctions, countof(JSSVGElementAttributesFunctions));
}

static JSClassDef JSSVGElementClassDefine;
static bool JSSVGElementClassDefine_initialized = false;

static void init_JSSVGElementClassDefine()
{
    if (JSSVGElementClassDefine_initialized) return;
    JSSVGElementClassDefine_initialized = true;
    memset(&JSSVGElementClassDefine, 0, sizeof(JSSVGElementClassDefine));
    JSSVGElementClassDefine.class_name = "SVGElement";
    JSSVGElementClassDefine.finalizer = JSSVGElement::finalizer;
    JSSVGElementClassDefine.gc_mark = JSSVGElement::mark;
}

JSClassID JSSVGElement::js_class_id = 0;

void JSSVGElement::init(JSContext* ctx)
{
    if (JSSVGElement::js_class_id == 0) {
        init_JSSVGElementClassDefine();
        JS_NewClassID(&JSSVGElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGElement::js_class_id, &JSSVGElementClassDefine);
        JS_SetClassProto(ctx, JSSVGElement::js_class_id, JSSVGElementPrototype::self(ctx));
    }
}

JSValue JSSVGElement::create(JSContext* ctx, SVGElement* impl)
{
    JSSVGElement::init(ctx);
    JSValue _proto = JSSVGElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGElement* imp = (SVGElement*)JS_GetOpaque(this_val, JSSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->id()).utf8().data());
        }
        case XmlbaseAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque(this_val, JSSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlbase()).utf8().data());
        }
        case OwnerSVGElementAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque(this_val, JSSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerSVGElement()));
        }
        case ViewportElementAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque(this_val, JSSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->viewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case IdAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque(this_val, JSSVGElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setId(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case XmlbaseAttrNum: {
            SVGElement* imp = (SVGElement*)JS_GetOpaque(this_val, JSSVGElement::js_class_id);
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
