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
#include "QJSSVGDescElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedString.h"
#include "SVGDescElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGDescElementAttributesFunctions[4];
static bool JSSVGDescElementAttributesFunctions_initialized = false;

static void init_JSSVGDescElementAttributesFunctions()
{
    if (JSSVGDescElementAttributesFunctions_initialized) return;
    JSSVGDescElementAttributesFunctions_initialized = true;
    memset(JSSVGDescElementAttributesFunctions, 0, sizeof(JSSVGDescElementAttributesFunctions));
    JSSVGDescElementAttributesFunctions[0].name = "xmllang";
    JSSVGDescElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDescElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDescElementAttributesFunctions[0].magic = JSSVGDescElement::XmllangAttrNum;
    JSSVGDescElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGDescElement::getValueProperty;
    JSSVGDescElementAttributesFunctions[0].u.getset.set.setter_magic = JSSVGDescElement::putValueProperty;
    JSSVGDescElementAttributesFunctions[1].name = "xmlspace";
    JSSVGDescElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDescElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDescElementAttributesFunctions[1].magic = JSSVGDescElement::XmlspaceAttrNum;
    JSSVGDescElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGDescElement::getValueProperty;
    JSSVGDescElementAttributesFunctions[1].u.getset.set.setter_magic = JSSVGDescElement::putValueProperty;
    JSSVGDescElementAttributesFunctions[2].name = "className";
    JSSVGDescElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDescElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDescElementAttributesFunctions[2].magic = JSSVGDescElement::ClassNameAttrNum;
    JSSVGDescElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGDescElement::getValueProperty;
    JSSVGDescElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGDescElementAttributesFunctions[3].name = "style";
    JSSVGDescElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDescElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDescElementAttributesFunctions[3].magic = JSSVGDescElement::StyleAttrNum;
    JSSVGDescElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGDescElement::getValueProperty;
    JSSVGDescElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGDescElementPrototypeFunctions[1];
static bool JSSVGDescElementPrototypeFunctions_initialized = false;

static void init_JSSVGDescElementPrototypeFunctions()
{
    if (JSSVGDescElementPrototypeFunctions_initialized) return;
    JSSVGDescElementPrototypeFunctions_initialized = true;
    memset(JSSVGDescElementPrototypeFunctions, 0, sizeof(JSSVGDescElementPrototypeFunctions));
    JSSVGDescElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGDescElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGDescElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGDescElementPrototypeFunctions[0].magic = JSSVGDescElement::GetPresentationAttributeFuncNum;
    JSSVGDescElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGDescElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGDescElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGDescElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGDescElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGDescElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGDescElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGDescElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGDescElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGDescElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGDescElementAttributesFunctions, countof(JSSVGDescElementAttributesFunctions));
    init_JSSVGDescElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGDescElementPrototypeFunctions, countof(JSSVGDescElementPrototypeFunctions));
}

static JSClassDef JSSVGDescElementClassDefine;
static bool JSSVGDescElementClassDefine_initialized = false;

static void init_JSSVGDescElementClassDefine()
{
    if (JSSVGDescElementClassDefine_initialized) return;
    JSSVGDescElementClassDefine_initialized = true;
    memset(&JSSVGDescElementClassDefine, 0, sizeof(JSSVGDescElementClassDefine));
    JSSVGDescElementClassDefine.class_name = "SVGDescElement";
    JSSVGDescElementClassDefine.finalizer = JSSVGDescElement::finalizer;
    JSSVGDescElementClassDefine.gc_mark = JSSVGDescElement::mark;
}

JSClassID JSSVGDescElement::js_class_id = 0;

void JSSVGDescElement::init(JSContext* ctx)
{
    if (JSSVGDescElement::js_class_id == 0) {
        init_JSSVGDescElementClassDefine();
        JS_NewClassID(&JSSVGDescElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGDescElement::js_class_id, &JSSVGDescElementClassDefine);
        JS_SetClassProto(ctx, JSSVGDescElement::js_class_id, JSSVGDescElementPrototype::self(ctx));
    }
}

JSValue JSSVGDescElement::create(JSContext* ctx, SVGDescElement* impl)
{
    JSSVGDescElement::init(ctx);
    JSValue _proto = JSSVGDescElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGDescElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGDescElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGDescElement* impl = (SVGDescElement*)JS_GetOpaque(val, JSSVGDescElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGDescElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGDescElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGDescElement* imp = (SVGDescElement*)JS_GetOpaque(this_val, JSSVGDescElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGDescElement* imp = (SVGDescElement*)JS_GetOpaque(this_val, JSSVGDescElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ClassNameAttrNum: {
            SVGDescElement* imp = (SVGDescElement*)JS_GetOpaque(this_val, JSSVGDescElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGDescElement* imp = (SVGDescElement*)JS_GetOpaque(this_val, JSSVGDescElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGDescElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGDescElement* imp = (SVGDescElement*)JS_GetOpaque(this_val, JSSVGDescElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGDescElement* imp = (SVGDescElement*)JS_GetOpaque(this_val, JSSVGDescElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGDescElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGDescElement* imp = (SVGDescElement*)JS_GetOpaque(this_val, JSSVGDescElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGDescElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
