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
#include "QJSSVGViewElement.h"

#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedRect.h"
#include "QJSSVGStringList.h"
#include "SVGStringList.h"
#include "SVGViewElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGViewElementAttributesFunctions[5];
static bool JSSVGViewElementAttributesFunctions_initialized = false;

static void init_JSSVGViewElementAttributesFunctions()
{
    if (JSSVGViewElementAttributesFunctions_initialized) return;
    JSSVGViewElementAttributesFunctions_initialized = true;
    memset(JSSVGViewElementAttributesFunctions, 0, sizeof(JSSVGViewElementAttributesFunctions));
    JSSVGViewElementAttributesFunctions[0].name = "viewTarget";
    JSSVGViewElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGViewElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGViewElementAttributesFunctions[0].magic = JSSVGViewElement::ViewTargetAttrNum;
    JSSVGViewElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGViewElement::getValueProperty;
    JSSVGViewElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGViewElementAttributesFunctions[1].name = "externalResourcesRequired";
    JSSVGViewElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGViewElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGViewElementAttributesFunctions[1].magic = JSSVGViewElement::ExternalResourcesRequiredAttrNum;
    JSSVGViewElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGViewElement::getValueProperty;
    JSSVGViewElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGViewElementAttributesFunctions[2].name = "viewBox";
    JSSVGViewElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGViewElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGViewElementAttributesFunctions[2].magic = JSSVGViewElement::ViewBoxAttrNum;
    JSSVGViewElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGViewElement::getValueProperty;
    JSSVGViewElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGViewElementAttributesFunctions[3].name = "preserveAspectRatio";
    JSSVGViewElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGViewElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGViewElementAttributesFunctions[3].magic = JSSVGViewElement::PreserveAspectRatioAttrNum;
    JSSVGViewElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGViewElement::getValueProperty;
    JSSVGViewElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGViewElementAttributesFunctions[4].name = "zoomAndPan";
    JSSVGViewElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGViewElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGViewElementAttributesFunctions[4].magic = JSSVGViewElement::ZoomAndPanAttrNum;
    JSSVGViewElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGViewElement::getValueProperty;
    JSSVGViewElementAttributesFunctions[4].u.getset.set.setter_magic = JSSVGViewElement::putValueProperty;
}

/* Constants table */

static JSCFunctionListEntry JSSVGViewElementPrototypeConstantsFunctions[3];
static bool JSSVGViewElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGViewElementPrototypeConstantsFunctions()
{
    if (JSSVGViewElementPrototypeConstantsFunctions_initialized) return;
    JSSVGViewElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGViewElementPrototypeConstantsFunctions, 0, sizeof(JSSVGViewElementPrototypeConstantsFunctions));
    JSSVGViewElementPrototypeConstantsFunctions[0].name = "SVG_ZOOMANDPAN_UNKNOWN";
    JSSVGViewElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGViewElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGViewElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGViewElement::SVG_ZOOMANDPAN_UNKNOWN;
    JSSVGViewElementPrototypeConstantsFunctions[1].name = "SVG_ZOOMANDPAN_DISABLE";
    JSSVGViewElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGViewElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGViewElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGViewElement::SVG_ZOOMANDPAN_DISABLE;
    JSSVGViewElementPrototypeConstantsFunctions[2].name = "SVG_ZOOMANDPAN_MAGNIFY";
    JSSVGViewElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGViewElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGViewElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGViewElement::SVG_ZOOMANDPAN_MAGNIFY;
}

JSValue JSSVGViewElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGViewElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGViewElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGViewElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGViewElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGViewElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGViewElementAttributesFunctions, countof(JSSVGViewElementAttributesFunctions));
    init_JSSVGViewElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGViewElementPrototypeConstantsFunctions, countof(JSSVGViewElementPrototypeConstantsFunctions));
}

static JSClassDef JSSVGViewElementClassDefine;
static bool JSSVGViewElementClassDefine_initialized = false;

static void init_JSSVGViewElementClassDefine()
{
    if (JSSVGViewElementClassDefine_initialized) return;
    JSSVGViewElementClassDefine_initialized = true;
    memset(&JSSVGViewElementClassDefine, 0, sizeof(JSSVGViewElementClassDefine));
    JSSVGViewElementClassDefine.class_name = "SVGViewElement";
    JSSVGViewElementClassDefine.finalizer = JSSVGViewElement::finalizer;
    JSSVGViewElementClassDefine.gc_mark = JSSVGViewElement::mark;
}

JSClassID JSSVGViewElement::js_class_id = 0;

void JSSVGViewElement::init(JSContext* ctx)
{
    if (JSSVGViewElement::js_class_id == 0) {
        init_JSSVGViewElementClassDefine();
        JS_NewClassID(&JSSVGViewElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGViewElement::js_class_id, &JSSVGViewElementClassDefine);
        JS_SetClassProto(ctx, JSSVGViewElement::js_class_id, JSSVGViewElementPrototype::self(ctx));
    }
}

JSValue JSSVGViewElement::create(JSContext* ctx, SVGViewElement* impl)
{
    JSSVGViewElement::init(ctx);
    JSValue _proto = JSSVGViewElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGViewElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGViewElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGViewElement* impl = (SVGViewElement*)JS_GetOpaque(val, JSSVGViewElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGViewElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGViewElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ViewTargetAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque(this_val, JSSVGViewElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->viewTarget()), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque(this_val, JSSVGViewElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ViewBoxAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque(this_val, JSSVGViewElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque(this_val, JSSVGViewElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ZoomAndPanAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque(this_val, JSSVGViewElement::js_class_id);
            return JS_NewInt32(ctx, imp->zoomAndPan());
        }
    }
    return JS_NULL;
}

JSValue JSSVGViewElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ZoomAndPanAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque(this_val, JSSVGViewElement::js_class_id);
            imp->setZoomAndPan(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}


}

#endif // ENABLE(SVG)
