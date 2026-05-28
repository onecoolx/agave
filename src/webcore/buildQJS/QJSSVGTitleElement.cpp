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
#include "QJSSVGTitleElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedString.h"
#include "SVGTitleElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGTitleElementAttributesFunctions[4];
static bool JSSVGTitleElementAttributesFunctions_initialized = false;

static void init_JSSVGTitleElementAttributesFunctions()
{
    if (JSSVGTitleElementAttributesFunctions_initialized) return;
    JSSVGTitleElementAttributesFunctions_initialized = true;
    memset(JSSVGTitleElementAttributesFunctions, 0, sizeof(JSSVGTitleElementAttributesFunctions));
    JSSVGTitleElementAttributesFunctions[0].name = "xmllang";
    JSSVGTitleElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTitleElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTitleElementAttributesFunctions[0].magic = JSSVGTitleElement::XmllangAttrNum;
    JSSVGTitleElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGTitleElement::getValueProperty;
    JSSVGTitleElementAttributesFunctions[0].u.getset.set.setter_magic = JSSVGTitleElement::putValueProperty;
    JSSVGTitleElementAttributesFunctions[1].name = "className";
    JSSVGTitleElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTitleElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTitleElementAttributesFunctions[1].magic = JSSVGTitleElement::ClassNameAttrNum;
    JSSVGTitleElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGTitleElement::getValueProperty;
    JSSVGTitleElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGTitleElementAttributesFunctions[2].name = "xmlspace";
    JSSVGTitleElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTitleElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTitleElementAttributesFunctions[2].magic = JSSVGTitleElement::XmlspaceAttrNum;
    JSSVGTitleElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGTitleElement::getValueProperty;
    JSSVGTitleElementAttributesFunctions[2].u.getset.set.setter_magic = JSSVGTitleElement::putValueProperty;
    JSSVGTitleElementAttributesFunctions[3].name = "style";
    JSSVGTitleElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTitleElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTitleElementAttributesFunctions[3].magic = JSSVGTitleElement::StyleAttrNum;
    JSSVGTitleElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGTitleElement::getValueProperty;
    JSSVGTitleElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGTitleElementPrototypeFunctions[1];
static bool JSSVGTitleElementPrototypeFunctions_initialized = false;

static void init_JSSVGTitleElementPrototypeFunctions()
{
    if (JSSVGTitleElementPrototypeFunctions_initialized) return;
    JSSVGTitleElementPrototypeFunctions_initialized = true;
    memset(JSSVGTitleElementPrototypeFunctions, 0, sizeof(JSSVGTitleElementPrototypeFunctions));
    JSSVGTitleElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGTitleElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTitleElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGTitleElementPrototypeFunctions[0].magic = JSSVGTitleElement::GetPresentationAttributeFuncNum;
    JSSVGTitleElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGTitleElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTitleElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGTitleElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGTitleElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTitleElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGTitleElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTitleElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTitleElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTitleElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTitleElementAttributesFunctions, countof(JSSVGTitleElementAttributesFunctions));
    init_JSSVGTitleElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTitleElementPrototypeFunctions, countof(JSSVGTitleElementPrototypeFunctions));
}

static JSClassDef JSSVGTitleElementClassDefine;
static bool JSSVGTitleElementClassDefine_initialized = false;

static void init_JSSVGTitleElementClassDefine()
{
    if (JSSVGTitleElementClassDefine_initialized) return;
    JSSVGTitleElementClassDefine_initialized = true;
    memset(&JSSVGTitleElementClassDefine, 0, sizeof(JSSVGTitleElementClassDefine));
    JSSVGTitleElementClassDefine.class_name = "SVGTitleElement";
    JSSVGTitleElementClassDefine.finalizer = JSSVGTitleElement::finalizer;
    JSSVGTitleElementClassDefine.gc_mark = JSSVGTitleElement::mark;
}

JSClassID JSSVGTitleElement::js_class_id = 0;

void JSSVGTitleElement::init(JSContext* ctx)
{
    if (JSSVGTitleElement::js_class_id == 0) {
        init_JSSVGTitleElementClassDefine();
        JS_NewClassID(&JSSVGTitleElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTitleElement::js_class_id, &JSSVGTitleElementClassDefine);
        JS_SetClassProto(ctx, JSSVGTitleElement::js_class_id, JSSVGTitleElementPrototype::self(ctx));
    }
}

JSValue JSSVGTitleElement::create(JSContext* ctx, SVGTitleElement* impl)
{
    JSSVGTitleElement::init(ctx);
    JSValue _proto = JSSVGTitleElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGTitleElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTitleElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTitleElement* impl = (SVGTitleElement*)JS_GetOpaque(val, JSSVGTitleElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTitleElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGTitleElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGTitleElement* imp = (SVGTitleElement*)JS_GetOpaque(this_val, JSSVGTitleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGTitleElement* imp = (SVGTitleElement*)JS_GetOpaque(this_val, JSSVGTitleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ClassNameAttrNum: {
            SVGTitleElement* imp = (SVGTitleElement*)JS_GetOpaque(this_val, JSSVGTitleElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGTitleElement* imp = (SVGTitleElement*)JS_GetOpaque(this_val, JSSVGTitleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGTitleElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGTitleElement* imp = (SVGTitleElement*)JS_GetOpaque(this_val, JSSVGTitleElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGTitleElement* imp = (SVGTitleElement*)JS_GetOpaque(this_val, JSSVGTitleElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGTitleElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGTitleElement* imp = (SVGTitleElement*)JS_GetOpaque(this_val, JSSVGTitleElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGTitleElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
