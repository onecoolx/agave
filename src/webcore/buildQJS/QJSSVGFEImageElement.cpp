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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGFEImageElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEImageElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFEImageElementAttributesFunctions[11];
static bool JSSVGFEImageElementAttributesFunctions_initialized = false;

static void init_JSSVGFEImageElementAttributesFunctions()
{
    if (JSSVGFEImageElementAttributesFunctions_initialized) return;
    JSSVGFEImageElementAttributesFunctions_initialized = true;
    memset(JSSVGFEImageElementAttributesFunctions, 0, sizeof(JSSVGFEImageElementAttributesFunctions));
    JSSVGFEImageElementAttributesFunctions[0].name = "href";
    JSSVGFEImageElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[0].magic = JSSVGFEImageElement::HrefAttrNum;
    JSSVGFEImageElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEImageElementAttributesFunctions[1].name = "xmllang";
    JSSVGFEImageElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[1].magic = JSSVGFEImageElement::XmllangAttrNum;
    JSSVGFEImageElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[1].u.getset.set.setter_magic = JSSVGFEImageElement::putValueProperty;
    JSSVGFEImageElementAttributesFunctions[2].name = "xmlspace";
    JSSVGFEImageElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[2].magic = JSSVGFEImageElement::XmlspaceAttrNum;
    JSSVGFEImageElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[2].u.getset.set.setter_magic = JSSVGFEImageElement::putValueProperty;
    JSSVGFEImageElementAttributesFunctions[3].name = "externalResourcesRequired";
    JSSVGFEImageElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[3].magic = JSSVGFEImageElement::ExternalResourcesRequiredAttrNum;
    JSSVGFEImageElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEImageElementAttributesFunctions[4].name = "x";
    JSSVGFEImageElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[4].magic = JSSVGFEImageElement::XAttrNum;
    JSSVGFEImageElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEImageElementAttributesFunctions[5].name = "y";
    JSSVGFEImageElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[5].magic = JSSVGFEImageElement::YAttrNum;
    JSSVGFEImageElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEImageElementAttributesFunctions[6].name = "width";
    JSSVGFEImageElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[6].magic = JSSVGFEImageElement::WidthAttrNum;
    JSSVGFEImageElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEImageElementAttributesFunctions[7].name = "height";
    JSSVGFEImageElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[7].magic = JSSVGFEImageElement::HeightAttrNum;
    JSSVGFEImageElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFEImageElementAttributesFunctions[8].name = "result";
    JSSVGFEImageElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[8].magic = JSSVGFEImageElement::ResultAttrNum;
    JSSVGFEImageElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFEImageElementAttributesFunctions[9].name = "className";
    JSSVGFEImageElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[9].magic = JSSVGFEImageElement::ClassNameAttrNum;
    JSSVGFEImageElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGFEImageElementAttributesFunctions[10].name = "style";
    JSSVGFEImageElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEImageElementAttributesFunctions[10].magic = JSSVGFEImageElement::StyleAttrNum;
    JSSVGFEImageElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFEImageElement::getValueProperty;
    JSSVGFEImageElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEImageElementPrototypeFunctions[1];
static bool JSSVGFEImageElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEImageElementPrototypeFunctions()
{
    if (JSSVGFEImageElementPrototypeFunctions_initialized) return;
    JSSVGFEImageElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEImageElementPrototypeFunctions, 0, sizeof(JSSVGFEImageElementPrototypeFunctions));
    JSSVGFEImageElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEImageElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEImageElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEImageElementPrototypeFunctions[0].magic = JSSVGFEImageElement::GetPresentationAttributeFuncNum;
    JSSVGFEImageElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEImageElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEImageElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEImageElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEImageElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEImageElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEImageElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEImageElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEImageElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEImageElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEImageElementAttributesFunctions, countof(JSSVGFEImageElementAttributesFunctions));
    init_JSSVGFEImageElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEImageElementPrototypeFunctions, countof(JSSVGFEImageElementPrototypeFunctions));
}

static JSClassDef JSSVGFEImageElementClassDefine;
static bool JSSVGFEImageElementClassDefine_initialized = false;

static void init_JSSVGFEImageElementClassDefine()
{
    if (JSSVGFEImageElementClassDefine_initialized) return;
    JSSVGFEImageElementClassDefine_initialized = true;
    memset(&JSSVGFEImageElementClassDefine, 0, sizeof(JSSVGFEImageElementClassDefine));
    JSSVGFEImageElementClassDefine.class_name = "SVGFEImageElement";
    JSSVGFEImageElementClassDefine.finalizer = JSSVGFEImageElement::finalizer;
    JSSVGFEImageElementClassDefine.gc_mark = JSSVGFEImageElement::mark;
}

JSClassID JSSVGFEImageElement::js_class_id = 0;

void JSSVGFEImageElement::init(JSContext* ctx)
{
    if (JSSVGFEImageElement::js_class_id == 0) {
        init_JSSVGFEImageElementClassDefine();
        JS_NewClassID(&JSSVGFEImageElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEImageElement::js_class_id, &JSSVGFEImageElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEImageElement::js_class_id, JSSVGFEImageElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEImageElement::create(JSContext* ctx, SVGFEImageElement* impl)
{
    JSSVGFEImageElement::init(ctx);
    JSValue _proto = JSSVGFEImageElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEImageElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEImageElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEImageElement* impl = (SVGFEImageElement*)JS_GetOpaque(val, JSSVGFEImageElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEImageElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEImageElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case HrefAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XmllangAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEImageElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGFEImageElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEImageElement* imp = (SVGFEImageElement*)JS_GetOpaque(this_val, JSSVGFEImageElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEImageElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
