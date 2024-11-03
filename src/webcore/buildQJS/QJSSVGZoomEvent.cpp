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
#include "QJSSVGZoomEvent.h"

#include "QJSSVGPoint.h"
#include "QJSSVGRect.h"
#include "SVGZoomEvent.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGZoomEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("previousScale", JSSVGZoomEvent::getValueProperty, NULL, JSSVGZoomEvent::PreviousScaleAttrNum),
    JS_CGETSET_MAGIC_DEF("zoomRectScreen", JSSVGZoomEvent::getValueProperty, NULL, JSSVGZoomEvent::ZoomRectScreenAttrNum),
    JS_CGETSET_MAGIC_DEF("newTranslate", JSSVGZoomEvent::getValueProperty, NULL, JSSVGZoomEvent::NewTranslateAttrNum),
    JS_CGETSET_MAGIC_DEF("previousTranslate", JSSVGZoomEvent::getValueProperty, NULL, JSSVGZoomEvent::PreviousTranslateAttrNum),
    JS_CGETSET_MAGIC_DEF("newScale", JSSVGZoomEvent::getValueProperty, NULL, JSSVGZoomEvent::NewScaleAttrNum)
};

JSValue JSSVGZoomEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGZoomEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSSVGZoomEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGZoomEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGZoomEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGZoomEventAttributesFunctions, countof(JSSVGZoomEventAttributesFunctions));
}

static JSClassDef JSSVGZoomEventClassDefine = 
{
    "SVGZoomEvent",
    .finalizer = JSSVGZoomEvent::finalizer,
    .gc_mark = JSSVGZoomEvent::mark,
};

JSClassID JSSVGZoomEvent::js_class_id = 0;

void JSSVGZoomEvent::init(JSContext* ctx)
{
    if (JSSVGZoomEvent::js_class_id == 0) {
        JS_NewClassID(&JSSVGZoomEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGZoomEvent::js_class_id, &JSSVGZoomEventClassDefine);
        JS_SetClassProto(ctx, JSSVGZoomEvent::js_class_id, JSSVGZoomEventPrototype::self(ctx));
    }
}

JSValue JSSVGZoomEvent::create(JSContext* ctx, SVGZoomEvent* impl, SVGElement* context)
{
    JSSVGZoomEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGZoomEventPrototype::self(ctx), JSSVGZoomEvent::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGZoomEvent::finalizer(JSRuntime* rt, JSValue val)
{
    SVGZoomEvent* impl = (SVGZoomEvent*)JS_GetOpaque(val, JSSVGZoomEvent::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGZoomEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSUIEvent::mark(rt, val, mark_func);
}

JSValue JSSVGZoomEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ZoomRectScreenAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque2(ctx, this_val, JSSVGZoomEvent::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->zoomRectScreen()), 0);
        }
        case PreviousScaleAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque2(ctx, this_val, JSSVGZoomEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->previousScale());
        }
        case PreviousTranslateAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque2(ctx, this_val, JSSVGZoomEvent::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->previousTranslate()), 0);
        }
        case NewScaleAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque2(ctx, this_val, JSSVGZoomEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->newScale());
        }
        case NewTranslateAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque2(ctx, this_val, JSSVGZoomEvent::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->newTranslate()), 0);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
