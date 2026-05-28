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
#include "QJSSVGFilterElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedInteger.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFilterElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFilterElementAttributesFunctions[14];
static bool JSSVGFilterElementAttributesFunctions_initialized = false;

static void init_JSSVGFilterElementAttributesFunctions()
{
    if (JSSVGFilterElementAttributesFunctions_initialized) return;
    JSSVGFilterElementAttributesFunctions_initialized = true;
    memset(JSSVGFilterElementAttributesFunctions, 0, sizeof(JSSVGFilterElementAttributesFunctions));
    JSSVGFilterElementAttributesFunctions[0].name = "filterUnits";
    JSSVGFilterElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[0].magic = JSSVGFilterElement::FilterUnitsAttrNum;
    JSSVGFilterElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[1].name = "primitiveUnits";
    JSSVGFilterElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[1].magic = JSSVGFilterElement::PrimitiveUnitsAttrNum;
    JSSVGFilterElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[2].name = "x";
    JSSVGFilterElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[2].magic = JSSVGFilterElement::XAttrNum;
    JSSVGFilterElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[3].name = "y";
    JSSVGFilterElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[3].magic = JSSVGFilterElement::YAttrNum;
    JSSVGFilterElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[4].name = "width";
    JSSVGFilterElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[4].magic = JSSVGFilterElement::WidthAttrNum;
    JSSVGFilterElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[5].name = "height";
    JSSVGFilterElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[5].magic = JSSVGFilterElement::HeightAttrNum;
    JSSVGFilterElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[6].name = "filterResX";
    JSSVGFilterElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[6].magic = JSSVGFilterElement::FilterResXAttrNum;
    JSSVGFilterElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[7].name = "filterResY";
    JSSVGFilterElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[7].magic = JSSVGFilterElement::FilterResYAttrNum;
    JSSVGFilterElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[8].name = "href";
    JSSVGFilterElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[8].magic = JSSVGFilterElement::HrefAttrNum;
    JSSVGFilterElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[9].name = "xmllang";
    JSSVGFilterElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[9].magic = JSSVGFilterElement::XmllangAttrNum;
    JSSVGFilterElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[9].u.getset.set.setter_magic = JSSVGFilterElement::putValueProperty;
    JSSVGFilterElementAttributesFunctions[10].name = "xmlspace";
    JSSVGFilterElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[10].magic = JSSVGFilterElement::XmlspaceAttrNum;
    JSSVGFilterElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[10].u.getset.set.setter_magic = JSSVGFilterElement::putValueProperty;
    JSSVGFilterElementAttributesFunctions[11].name = "externalResourcesRequired";
    JSSVGFilterElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[11].magic = JSSVGFilterElement::ExternalResourcesRequiredAttrNum;
    JSSVGFilterElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[12].name = "className";
    JSSVGFilterElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[12].magic = JSSVGFilterElement::ClassNameAttrNum;
    JSSVGFilterElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGFilterElementAttributesFunctions[13].name = "style";
    JSSVGFilterElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFilterElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFilterElementAttributesFunctions[13].magic = JSSVGFilterElement::StyleAttrNum;
    JSSVGFilterElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGFilterElement::getValueProperty;
    JSSVGFilterElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFilterElementPrototypeFunctions[2];
static bool JSSVGFilterElementPrototypeFunctions_initialized = false;

static void init_JSSVGFilterElementPrototypeFunctions()
{
    if (JSSVGFilterElementPrototypeFunctions_initialized) return;
    JSSVGFilterElementPrototypeFunctions_initialized = true;
    memset(JSSVGFilterElementPrototypeFunctions, 0, sizeof(JSSVGFilterElementPrototypeFunctions));
    JSSVGFilterElementPrototypeFunctions[0].name = "setFilterRes";
    JSSVGFilterElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFilterElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFilterElementPrototypeFunctions[0].magic = JSSVGFilterElement::SetFilterResFuncNum;
    JSSVGFilterElementPrototypeFunctions[0].u.func.length = 2;
    JSSVGFilterElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFilterElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFilterElementPrototypeFunction::callAsFunction;
    JSSVGFilterElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGFilterElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFilterElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGFilterElementPrototypeFunctions[1].magic = JSSVGFilterElement::GetPresentationAttributeFuncNum;
    JSSVGFilterElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGFilterElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFilterElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGFilterElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFilterElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFilterElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFilterElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFilterElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFilterElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFilterElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFilterElementAttributesFunctions, countof(JSSVGFilterElementAttributesFunctions));
    init_JSSVGFilterElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFilterElementPrototypeFunctions, countof(JSSVGFilterElementPrototypeFunctions));
}

static JSClassDef JSSVGFilterElementClassDefine;
static bool JSSVGFilterElementClassDefine_initialized = false;

static void init_JSSVGFilterElementClassDefine()
{
    if (JSSVGFilterElementClassDefine_initialized) return;
    JSSVGFilterElementClassDefine_initialized = true;
    memset(&JSSVGFilterElementClassDefine, 0, sizeof(JSSVGFilterElementClassDefine));
    JSSVGFilterElementClassDefine.class_name = "SVGFilterElement";
    JSSVGFilterElementClassDefine.finalizer = JSSVGFilterElement::finalizer;
    JSSVGFilterElementClassDefine.gc_mark = JSSVGFilterElement::mark;
}

JSClassID JSSVGFilterElement::js_class_id = 0;

void JSSVGFilterElement::init(JSContext* ctx)
{
    if (JSSVGFilterElement::js_class_id == 0) {
        init_JSSVGFilterElementClassDefine();
        JS_NewClassID(&JSSVGFilterElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFilterElement::js_class_id, &JSSVGFilterElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFilterElement::js_class_id, JSSVGFilterElementPrototype::self(ctx));
    }
}

JSValue JSSVGFilterElement::create(JSContext* ctx, SVGFilterElement* impl)
{
    JSSVGFilterElement::init(ctx);
    JSValue _proto = JSSVGFilterElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFilterElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFilterElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFilterElement* impl = (SVGFilterElement*)JS_GetOpaque(val, JSSVGFilterElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFilterElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFilterElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FilterUnitsAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->filterUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PrimitiveUnitsAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->primitiveUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case FilterResXAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedInteger> obj = imp->filterResXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case FilterResYAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedInteger> obj = imp->filterResYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XmllangAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFilterElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGFilterElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFilterElement* imp = (SVGFilterElement*)JS_GetOpaque(this_val, JSSVGFilterElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFilterElement::SetFilterResFuncNum: {
            bool filterResXOk;
            unsigned filterResX = valueToInt32(ctx, argv[0], filterResXOk);
            if (!filterResXOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool filterResYOk;
            unsigned filterResY = valueToInt32(ctx, argv[1], filterResYOk);
            if (!filterResYOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setFilterRes(filterResX, filterResY);
            return JS_UNDEFINED;
        }
        case JSSVGFilterElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
