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

static const JSCFunctionListEntry JSSVGViewElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGViewElement::getValueProperty, NULL, JSSVGViewElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("zoomAndPan", JSSVGViewElement::getValueProperty, JSSVGViewElement::putValueProperty, JSSVGViewElement::ZoomAndPanAttrNum),
    JS_CGETSET_MAGIC_DEF("viewTarget", JSSVGViewElement::getValueProperty, NULL, JSSVGViewElement::ViewTargetAttrNum),
    JS_CGETSET_MAGIC_DEF("viewBox", JSSVGViewElement::getValueProperty, NULL, JSSVGViewElement::ViewBoxAttrNum),
    JS_CGETSET_MAGIC_DEF("preserveAspectRatio", JSSVGViewElement::getValueProperty, NULL, JSSVGViewElement::PreserveAspectRatioAttrNum)
};

/* Functions table */

static const JSCFunctionListEntry JSSVGViewElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_ZOOMANDPAN_UNKNOWN", JSSVGViewElementPrototype::getValueProperty, NULL, SVGViewElement::SVG_ZOOMANDPAN_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_ZOOMANDPAN_DISABLE", JSSVGViewElementPrototype::getValueProperty, NULL, SVGViewElement::SVG_ZOOMANDPAN_DISABLE),
    JS_CGETSET_MAGIC_DEF("SVG_ZOOMANDPAN_MAGNIFY", JSSVGViewElementPrototype::getValueProperty, NULL, SVGViewElement::SVG_ZOOMANDPAN_MAGNIFY)
};

JSValue JSSVGViewElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGViewElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGViewElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGViewElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGViewElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGViewElementAttributesFunctions, countof(JSSVGViewElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGViewElementPrototypeConstantsFunctions, countof(JSSVGViewElementPrototypeConstantsFunctions));
}

JSValue JSSVGViewElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGViewElementClassDefine = 
{
    "SVGViewElement",
    .finalizer = JSSVGViewElement::finalizer,
    .gc_mark = JSSVGViewElement::mark,
};

JSClassID JSSVGViewElement::js_class_id = 0;

void JSSVGViewElement::init(JSContext* ctx)
{
    if (JSSVGViewElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGViewElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGViewElement::js_class_id, &JSSVGViewElementClassDefine);
        JS_SetClassProto(ctx, JSSVGViewElement::js_class_id, JSSVGViewElementPrototype::self(ctx));
    }
}

JSValue JSSVGViewElement::create(JSContext* ctx, SVGViewElement* impl)
{
    JSSVGViewElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGViewElementPrototype::self(ctx), JSSVGViewElement::js_class_id);
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
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque2(ctx, this_val, JSSVGViewElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->viewTarget()), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque2(ctx, this_val, JSSVGViewElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ViewBoxAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque2(ctx, this_val, JSSVGViewElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque2(ctx, this_val, JSSVGViewElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ZoomAndPanAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque2(ctx, this_val, JSSVGViewElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->zoomAndPan());
        }
    }
    return JS_NULL;
}

JSValue JSSVGViewElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ZoomAndPanAttrNum: {
            SVGViewElement* imp = (SVGViewElement*)JS_GetOpaque2(ctx, this_val, JSSVGViewElement::js_class_id);
            imp->setZoomAndPan(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}


}

#endif // ENABLE(SVG)
