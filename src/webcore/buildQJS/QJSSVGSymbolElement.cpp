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
#include "QJSSVGSymbolElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedRect.h"
#include "QJSSVGAnimatedString.h"
#include "SVGSymbolElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGSymbolElementAttributesFunctions[7];
static bool JSSVGSymbolElementAttributesFunctions_initialized = false;

static void init_JSSVGSymbolElementAttributesFunctions()
{
    if (JSSVGSymbolElementAttributesFunctions_initialized) return;
    JSSVGSymbolElementAttributesFunctions_initialized = true;
    memset(JSSVGSymbolElementAttributesFunctions, 0, sizeof(JSSVGSymbolElementAttributesFunctions));
    JSSVGSymbolElementAttributesFunctions[0].name = "xmlspace";
    JSSVGSymbolElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSymbolElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSymbolElementAttributesFunctions[0].magic = JSSVGSymbolElement::XmlspaceAttrNum;
    JSSVGSymbolElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGSymbolElement::getValueProperty;
    JSSVGSymbolElementAttributesFunctions[0].u.getset.set.setter_magic = JSSVGSymbolElement::putValueProperty;
    JSSVGSymbolElementAttributesFunctions[1].name = "className";
    JSSVGSymbolElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSymbolElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSymbolElementAttributesFunctions[1].magic = JSSVGSymbolElement::ClassNameAttrNum;
    JSSVGSymbolElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGSymbolElement::getValueProperty;
    JSSVGSymbolElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGSymbolElementAttributesFunctions[2].name = "externalResourcesRequired";
    JSSVGSymbolElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSymbolElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSymbolElementAttributesFunctions[2].magic = JSSVGSymbolElement::ExternalResourcesRequiredAttrNum;
    JSSVGSymbolElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGSymbolElement::getValueProperty;
    JSSVGSymbolElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGSymbolElementAttributesFunctions[3].name = "preserveAspectRatio";
    JSSVGSymbolElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSymbolElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSymbolElementAttributesFunctions[3].magic = JSSVGSymbolElement::PreserveAspectRatioAttrNum;
    JSSVGSymbolElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGSymbolElement::getValueProperty;
    JSSVGSymbolElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGSymbolElementAttributesFunctions[4].name = "xmllang";
    JSSVGSymbolElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSymbolElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSymbolElementAttributesFunctions[4].magic = JSSVGSymbolElement::XmllangAttrNum;
    JSSVGSymbolElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGSymbolElement::getValueProperty;
    JSSVGSymbolElementAttributesFunctions[4].u.getset.set.setter_magic = JSSVGSymbolElement::putValueProperty;
    JSSVGSymbolElementAttributesFunctions[5].name = "style";
    JSSVGSymbolElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSymbolElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSymbolElementAttributesFunctions[5].magic = JSSVGSymbolElement::StyleAttrNum;
    JSSVGSymbolElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGSymbolElement::getValueProperty;
    JSSVGSymbolElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGSymbolElementAttributesFunctions[6].name = "viewBox";
    JSSVGSymbolElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSymbolElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSymbolElementAttributesFunctions[6].magic = JSSVGSymbolElement::ViewBoxAttrNum;
    JSSVGSymbolElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGSymbolElement::getValueProperty;
    JSSVGSymbolElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGSymbolElementPrototypeFunctions[1];
static bool JSSVGSymbolElementPrototypeFunctions_initialized = false;

static void init_JSSVGSymbolElementPrototypeFunctions()
{
    if (JSSVGSymbolElementPrototypeFunctions_initialized) return;
    JSSVGSymbolElementPrototypeFunctions_initialized = true;
    memset(JSSVGSymbolElementPrototypeFunctions, 0, sizeof(JSSVGSymbolElementPrototypeFunctions));
    JSSVGSymbolElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGSymbolElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSymbolElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGSymbolElementPrototypeFunctions[0].magic = JSSVGSymbolElement::GetPresentationAttributeFuncNum;
    JSSVGSymbolElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGSymbolElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSymbolElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGSymbolElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGSymbolElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGSymbolElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGSymbolElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGSymbolElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGSymbolElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGSymbolElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSymbolElementAttributesFunctions, countof(JSSVGSymbolElementAttributesFunctions));
    init_JSSVGSymbolElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSymbolElementPrototypeFunctions, countof(JSSVGSymbolElementPrototypeFunctions));
}

static JSClassDef JSSVGSymbolElementClassDefine;
static bool JSSVGSymbolElementClassDefine_initialized = false;

static void init_JSSVGSymbolElementClassDefine()
{
    if (JSSVGSymbolElementClassDefine_initialized) return;
    JSSVGSymbolElementClassDefine_initialized = true;
    memset(&JSSVGSymbolElementClassDefine, 0, sizeof(JSSVGSymbolElementClassDefine));
    JSSVGSymbolElementClassDefine.class_name = "SVGSymbolElement";
    JSSVGSymbolElementClassDefine.finalizer = JSSVGSymbolElement::finalizer;
    JSSVGSymbolElementClassDefine.gc_mark = JSSVGSymbolElement::mark;
}

JSClassID JSSVGSymbolElement::js_class_id = 0;

void JSSVGSymbolElement::init(JSContext* ctx)
{
    if (JSSVGSymbolElement::js_class_id == 0) {
        init_JSSVGSymbolElementClassDefine();
        JS_NewClassID(&JSSVGSymbolElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGSymbolElement::js_class_id, &JSSVGSymbolElementClassDefine);
        JS_SetClassProto(ctx, JSSVGSymbolElement::js_class_id, JSSVGSymbolElementPrototype::self(ctx));
    }
}

JSValue JSSVGSymbolElement::create(JSContext* ctx, SVGSymbolElement* impl)
{
    JSSVGSymbolElement::init(ctx);
    JSValue _proto = JSSVGSymbolElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGSymbolElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGSymbolElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGSymbolElement* impl = (SVGSymbolElement*)JS_GetOpaque(val, JSSVGSymbolElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGSymbolElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGSymbolElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ViewBoxAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGSymbolElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGSymbolElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGSymbolElement* imp = (SVGSymbolElement*)JS_GetOpaque(this_val, JSSVGSymbolElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGSymbolElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
