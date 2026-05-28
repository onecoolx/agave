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
#include "QJSSVGFESpotLightElement.h"

#include "QJSSVGAnimatedNumber.h"
#include "SVGFESpotLightElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFESpotLightElementAttributesFunctions[8];
static bool JSSVGFESpotLightElementAttributesFunctions_initialized = false;

static void init_JSSVGFESpotLightElementAttributesFunctions()
{
    if (JSSVGFESpotLightElementAttributesFunctions_initialized) return;
    JSSVGFESpotLightElementAttributesFunctions_initialized = true;
    memset(JSSVGFESpotLightElementAttributesFunctions, 0, sizeof(JSSVGFESpotLightElementAttributesFunctions));
    JSSVGFESpotLightElementAttributesFunctions[0].name = "x";
    JSSVGFESpotLightElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpotLightElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpotLightElementAttributesFunctions[0].magic = JSSVGFESpotLightElement::XAttrNum;
    JSSVGFESpotLightElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFESpotLightElement::getValueProperty;
    JSSVGFESpotLightElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFESpotLightElementAttributesFunctions[1].name = "y";
    JSSVGFESpotLightElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpotLightElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpotLightElementAttributesFunctions[1].magic = JSSVGFESpotLightElement::YAttrNum;
    JSSVGFESpotLightElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFESpotLightElement::getValueProperty;
    JSSVGFESpotLightElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFESpotLightElementAttributesFunctions[2].name = "z";
    JSSVGFESpotLightElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpotLightElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpotLightElementAttributesFunctions[2].magic = JSSVGFESpotLightElement::ZAttrNum;
    JSSVGFESpotLightElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFESpotLightElement::getValueProperty;
    JSSVGFESpotLightElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFESpotLightElementAttributesFunctions[3].name = "pointsAtX";
    JSSVGFESpotLightElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpotLightElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpotLightElementAttributesFunctions[3].magic = JSSVGFESpotLightElement::PointsAtXAttrNum;
    JSSVGFESpotLightElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFESpotLightElement::getValueProperty;
    JSSVGFESpotLightElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFESpotLightElementAttributesFunctions[4].name = "pointsAtY";
    JSSVGFESpotLightElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpotLightElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpotLightElementAttributesFunctions[4].magic = JSSVGFESpotLightElement::PointsAtYAttrNum;
    JSSVGFESpotLightElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFESpotLightElement::getValueProperty;
    JSSVGFESpotLightElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFESpotLightElementAttributesFunctions[5].name = "pointsAtZ";
    JSSVGFESpotLightElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpotLightElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpotLightElementAttributesFunctions[5].magic = JSSVGFESpotLightElement::PointsAtZAttrNum;
    JSSVGFESpotLightElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFESpotLightElement::getValueProperty;
    JSSVGFESpotLightElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFESpotLightElementAttributesFunctions[6].name = "specularExponent";
    JSSVGFESpotLightElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpotLightElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpotLightElementAttributesFunctions[6].magic = JSSVGFESpotLightElement::SpecularExponentAttrNum;
    JSSVGFESpotLightElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFESpotLightElement::getValueProperty;
    JSSVGFESpotLightElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFESpotLightElementAttributesFunctions[7].name = "limitingConeAngle";
    JSSVGFESpotLightElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFESpotLightElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFESpotLightElementAttributesFunctions[7].magic = JSSVGFESpotLightElement::LimitingConeAngleAttrNum;
    JSSVGFESpotLightElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFESpotLightElement::getValueProperty;
    JSSVGFESpotLightElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGFESpotLightElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFESpotLightElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFESpotLightElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFESpotLightElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFESpotLightElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFESpotLightElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFESpotLightElementAttributesFunctions, countof(JSSVGFESpotLightElementAttributesFunctions));
}

static JSClassDef JSSVGFESpotLightElementClassDefine;
static bool JSSVGFESpotLightElementClassDefine_initialized = false;

static void init_JSSVGFESpotLightElementClassDefine()
{
    if (JSSVGFESpotLightElementClassDefine_initialized) return;
    JSSVGFESpotLightElementClassDefine_initialized = true;
    memset(&JSSVGFESpotLightElementClassDefine, 0, sizeof(JSSVGFESpotLightElementClassDefine));
    JSSVGFESpotLightElementClassDefine.class_name = "SVGFESpotLightElement";
    JSSVGFESpotLightElementClassDefine.finalizer = JSSVGFESpotLightElement::finalizer;
    JSSVGFESpotLightElementClassDefine.gc_mark = JSSVGFESpotLightElement::mark;
}

JSClassID JSSVGFESpotLightElement::js_class_id = 0;

void JSSVGFESpotLightElement::init(JSContext* ctx)
{
    if (JSSVGFESpotLightElement::js_class_id == 0) {
        init_JSSVGFESpotLightElementClassDefine();
        JS_NewClassID(&JSSVGFESpotLightElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFESpotLightElement::js_class_id, &JSSVGFESpotLightElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFESpotLightElement::js_class_id, JSSVGFESpotLightElementPrototype::self(ctx));
    }
}

JSValue JSSVGFESpotLightElement::create(JSContext* ctx, SVGFESpotLightElement* impl)
{
    JSSVGFESpotLightElement::init(ctx);
    JSValue _proto = JSSVGFESpotLightElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFESpotLightElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFESpotLightElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFESpotLightElement* impl = (SVGFESpotLightElement*)JS_GetOpaque(val, JSSVGFESpotLightElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFESpotLightElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFESpotLightElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque(this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque(this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ZAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque(this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->zAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PointsAtXAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque(this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->pointsAtXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PointsAtYAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque(this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->pointsAtYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PointsAtZAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque(this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->pointsAtZAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpecularExponentAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque(this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->specularExponentAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case LimitingConeAngleAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque(this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->limitingConeAngleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
