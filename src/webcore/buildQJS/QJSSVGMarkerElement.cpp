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
#include "QJSSVGMarkerElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAngle.h"
#include "QJSSVGAnimatedAngle.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedRect.h"
#include "QJSSVGAnimatedString.h"
#include "SVGMarkerElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGMarkerElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("refX", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::RefXAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGMarkerElement::getValueProperty, JSSVGMarkerElement::putValueProperty, JSSVGMarkerElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("orientAngle", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::OrientAngleAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("refY", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::RefYAttrNum),
    JS_CGETSET_MAGIC_DEF("markerUnits", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::MarkerUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("markerWidth", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::MarkerWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("markerHeight", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::MarkerHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("orientType", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::OrientTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGMarkerElement::getValueProperty, JSSVGMarkerElement::putValueProperty, JSSVGMarkerElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("viewBox", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::ViewBoxAttrNum),
    JS_CGETSET_MAGIC_DEF("preserveAspectRatio", JSSVGMarkerElement::getValueProperty, NULL, JSSVGMarkerElement::PreserveAspectRatioAttrNum)
};

class JSSVGMarkerElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGMarkerElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGMarkerElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_MARKERUNITS_UNKNOWN", JSSVGMarkerElementConstructor::getValueProperty, NULL, SVGMarkerElement::SVG_MARKERUNITS_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_MARKERUNITS_USERSPACEONUSE", JSSVGMarkerElementConstructor::getValueProperty, NULL, SVGMarkerElement::SVG_MARKERUNITS_USERSPACEONUSE),
    JS_CGETSET_MAGIC_DEF("SVG_MARKER_ORIENT_UNKNOWN", JSSVGMarkerElementConstructor::getValueProperty, NULL, SVGMarkerElement::SVG_MARKER_ORIENT_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_MARKERUNITS_STROKEWIDTH", JSSVGMarkerElementConstructor::getValueProperty, NULL, SVGMarkerElement::SVG_MARKERUNITS_STROKEWIDTH),
    JS_CGETSET_MAGIC_DEF("SVG_MARKER_ORIENT_ANGLE", JSSVGMarkerElementConstructor::getValueProperty, NULL, SVGMarkerElement::SVG_MARKER_ORIENT_ANGLE),
    JS_CGETSET_MAGIC_DEF("SVG_MARKER_ORIENT_AUTO", JSSVGMarkerElementConstructor::getValueProperty, NULL, SVGMarkerElement::SVG_MARKER_ORIENT_AUTO)
};

JSValue JSSVGMarkerElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGMarkerElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGMarkerElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGMarkerElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGMarkerElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMarkerElementConstructorFunctions, countof(JSSVGMarkerElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGMarkerElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_MARKERUNITS_UNKNOWN", JSSVGMarkerElementPrototype::getValueProperty, NULL, SVGMarkerElement::SVG_MARKERUNITS_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_MARKERUNITS_USERSPACEONUSE", JSSVGMarkerElementPrototype::getValueProperty, NULL, SVGMarkerElement::SVG_MARKERUNITS_USERSPACEONUSE),
    JS_CGETSET_MAGIC_DEF("SVG_MARKER_ORIENT_UNKNOWN", JSSVGMarkerElementPrototype::getValueProperty, NULL, SVGMarkerElement::SVG_MARKER_ORIENT_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_MARKERUNITS_STROKEWIDTH", JSSVGMarkerElementPrototype::getValueProperty, NULL, SVGMarkerElement::SVG_MARKERUNITS_STROKEWIDTH),
    JS_CGETSET_MAGIC_DEF("SVG_MARKER_ORIENT_ANGLE", JSSVGMarkerElementPrototype::getValueProperty, NULL, SVGMarkerElement::SVG_MARKER_ORIENT_ANGLE),
    JS_CGETSET_MAGIC_DEF("SVG_MARKER_ORIENT_AUTO", JSSVGMarkerElementPrototype::getValueProperty, NULL, SVGMarkerElement::SVG_MARKER_ORIENT_AUTO)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGMarkerElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGMarkerElementPrototypeFunction::callAsFunction, JSSVGMarkerElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("setOrientToAuto", 0, JSSVGMarkerElementPrototypeFunction::callAsFunction, JSSVGMarkerElement::SetOrientToAutoFuncNum),
    JS_CFUNC_MAGIC_DEF("setOrientToAngle", 1, JSSVGMarkerElementPrototypeFunction::callAsFunction, JSSVGMarkerElement::SetOrientToAngleFuncNum)
};

JSValue JSSVGMarkerElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGMarkerElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGMarkerElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGMarkerElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGMarkerElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMarkerElementAttributesFunctions, countof(JSSVGMarkerElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMarkerElementPrototypeConstantsFunctions, countof(JSSVGMarkerElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMarkerElementPrototypeFunctions, countof(JSSVGMarkerElementPrototypeFunctions));
}

JSValue JSSVGMarkerElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGMarkerElementClassDefine = 
{
    "SVGMarkerElement",
    .finalizer = JSSVGMarkerElement::finalizer,
    .gc_mark = JSSVGMarkerElement::mark,
};

JSClassID JSSVGMarkerElement::js_class_id = 0;

void JSSVGMarkerElement::init(JSContext* ctx)
{
    if (JSSVGMarkerElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGMarkerElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGMarkerElement::js_class_id, &JSSVGMarkerElementClassDefine);
        JS_SetConstructor(ctx, JSSVGMarkerElementConstructor::self(ctx), JSSVGMarkerElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGMarkerElement::js_class_id, JSSVGMarkerElementPrototype::self(ctx));
    }
}

JSValue JSSVGMarkerElement::create(JSContext* ctx, SVGMarkerElement* impl)
{
    JSSVGMarkerElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGMarkerElementPrototype::self(ctx), JSSVGMarkerElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGMarkerElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGMarkerElement* impl = (SVGMarkerElement*)JS_GetOpaque(val, JSSVGMarkerElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGMarkerElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGMarkerElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RefXAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->refXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RefYAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->refYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MarkerUnitsAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->markerUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MarkerWidthAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->markerWidthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MarkerHeightAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->markerHeightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case OrientTypeAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->orientTypeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case OrientAngleAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedAngle> obj = imp->orientAngleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XmllangAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ViewBoxAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGMarkerElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGMarkerElement::getConstructor(JSContext *ctx)
{
    return JSSVGMarkerElementConstructor::self(ctx);
}

JSValue JSSVGMarkerElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque2(ctx, this_val, JSSVGMarkerElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGMarkerElement::SetOrientToAutoFuncNum: {
            imp->setOrientToAuto();
            return JS_UNDEFINED;
        }
        case JSSVGMarkerElement::SetOrientToAngleFuncNum: {
            SVGAngle* angle = toSVGAngle(argv[0]);
            imp->setOrientToAngle(angle);
            return JS_UNDEFINED;
        }
        case JSSVGMarkerElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
