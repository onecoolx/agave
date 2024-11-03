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
#include "QJSSVGScriptElement.h"

#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedString.h"
#include "SVGScriptElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGScriptElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("type", JSSVGScriptElement::getValueProperty, JSSVGScriptElement::putValueProperty, JSSVGScriptElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGScriptElement::getValueProperty, NULL, JSSVGScriptElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSSVGScriptElement::getValueProperty, NULL, JSSVGScriptElement::HrefAttrNum)
};

JSValue JSSVGScriptElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGScriptElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGScriptElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGScriptElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGScriptElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGScriptElementAttributesFunctions, countof(JSSVGScriptElementAttributesFunctions));
}

static JSClassDef JSSVGScriptElementClassDefine = 
{
    "SVGScriptElement",
    .finalizer = JSSVGScriptElement::finalizer,
    .gc_mark = JSSVGScriptElement::mark,
};

JSClassID JSSVGScriptElement::js_class_id = 0;

void JSSVGScriptElement::init(JSContext* ctx)
{
    if (JSSVGScriptElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGScriptElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGScriptElement::js_class_id, &JSSVGScriptElementClassDefine);
        JS_SetClassProto(ctx, JSSVGScriptElement::js_class_id, JSSVGScriptElementPrototype::self(ctx));
    }
}

JSValue JSSVGScriptElement::create(JSContext* ctx, SVGScriptElement* impl)
{
    JSSVGScriptElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGScriptElementPrototype::self(ctx), JSSVGScriptElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGScriptElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGScriptElement* impl = (SVGScriptElement*)JS_GetOpaque(val, JSSVGScriptElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGScriptElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGScriptElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            SVGScriptElement* imp = (SVGScriptElement*)JS_GetOpaque2(ctx, this_val, JSSVGScriptElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case HrefAttrNum: {
            SVGScriptElement* imp = (SVGScriptElement*)JS_GetOpaque2(ctx, this_val, JSSVGScriptElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGScriptElement* imp = (SVGScriptElement*)JS_GetOpaque2(ctx, this_val, JSSVGScriptElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGScriptElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case TypeAttrNum: {
            SVGScriptElement* imp = (SVGScriptElement*)JS_GetOpaque2(ctx, this_val, JSSVGScriptElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}


}

#endif // ENABLE(SVG)
