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
#include "QJSSVGZoomEvent.h"

#include "QJSSVGPoint.h"
#include "QJSSVGRect.h"
#include "SVGZoomEvent.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGZoomEventAttributesFunctions[5];
static bool JSSVGZoomEventAttributesFunctions_initialized = false;

static void init_JSSVGZoomEventAttributesFunctions()
{
    if (JSSVGZoomEventAttributesFunctions_initialized) return;
    JSSVGZoomEventAttributesFunctions_initialized = true;
    memset(JSSVGZoomEventAttributesFunctions, 0, sizeof(JSSVGZoomEventAttributesFunctions));
    JSSVGZoomEventAttributesFunctions[0].name = "zoomRectScreen";
    JSSVGZoomEventAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGZoomEventAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGZoomEventAttributesFunctions[0].magic = JSSVGZoomEvent::ZoomRectScreenAttrNum;
    JSSVGZoomEventAttributesFunctions[0].u.getset.get.getter_magic = JSSVGZoomEvent::getValueProperty;
    JSSVGZoomEventAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGZoomEventAttributesFunctions[1].name = "previousScale";
    JSSVGZoomEventAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGZoomEventAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGZoomEventAttributesFunctions[1].magic = JSSVGZoomEvent::PreviousScaleAttrNum;
    JSSVGZoomEventAttributesFunctions[1].u.getset.get.getter_magic = JSSVGZoomEvent::getValueProperty;
    JSSVGZoomEventAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGZoomEventAttributesFunctions[2].name = "previousTranslate";
    JSSVGZoomEventAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGZoomEventAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGZoomEventAttributesFunctions[2].magic = JSSVGZoomEvent::PreviousTranslateAttrNum;
    JSSVGZoomEventAttributesFunctions[2].u.getset.get.getter_magic = JSSVGZoomEvent::getValueProperty;
    JSSVGZoomEventAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGZoomEventAttributesFunctions[3].name = "newScale";
    JSSVGZoomEventAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGZoomEventAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGZoomEventAttributesFunctions[3].magic = JSSVGZoomEvent::NewScaleAttrNum;
    JSSVGZoomEventAttributesFunctions[3].u.getset.get.getter_magic = JSSVGZoomEvent::getValueProperty;
    JSSVGZoomEventAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGZoomEventAttributesFunctions[4].name = "newTranslate";
    JSSVGZoomEventAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGZoomEventAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGZoomEventAttributesFunctions[4].magic = JSSVGZoomEvent::NewTranslateAttrNum;
    JSSVGZoomEventAttributesFunctions[4].u.getset.get.getter_magic = JSSVGZoomEvent::getValueProperty;
    JSSVGZoomEventAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGZoomEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGZoomEvent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSSVGZoomEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGZoomEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGZoomEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGZoomEventAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGZoomEventAttributesFunctions, countof(JSSVGZoomEventAttributesFunctions));
}

static JSClassDef JSSVGZoomEventClassDefine;
static bool JSSVGZoomEventClassDefine_initialized = false;

static void init_JSSVGZoomEventClassDefine()
{
    if (JSSVGZoomEventClassDefine_initialized) return;
    JSSVGZoomEventClassDefine_initialized = true;
    memset(&JSSVGZoomEventClassDefine, 0, sizeof(JSSVGZoomEventClassDefine));
    JSSVGZoomEventClassDefine.class_name = "SVGZoomEvent";
    JSSVGZoomEventClassDefine.finalizer = JSSVGZoomEvent::finalizer;
    JSSVGZoomEventClassDefine.gc_mark = JSSVGZoomEvent::mark;
}

JSClassID JSSVGZoomEvent::js_class_id = 0;

void JSSVGZoomEvent::init(JSContext* ctx)
{
    if (JSSVGZoomEvent::js_class_id == 0) {
        init_JSSVGZoomEventClassDefine();
        JS_NewClassID(&JSSVGZoomEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGZoomEvent::js_class_id, &JSSVGZoomEventClassDefine);
        JS_SetClassProto(ctx, JSSVGZoomEvent::js_class_id, JSSVGZoomEventPrototype::self(ctx));
    }
}

JSValue JSSVGZoomEvent::create(JSContext* ctx, SVGZoomEvent* impl, SVGElement* context)
{
    JSSVGZoomEvent::init(ctx);
    JSValue _proto = JSSVGZoomEventPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGZoomEvent::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque(this_val, JSSVGZoomEvent::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->zoomRectScreen()), 0);
        }
        case PreviousScaleAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque(this_val, JSSVGZoomEvent::js_class_id);
            return JS_NewInt32(ctx, imp->previousScale());
        }
        case PreviousTranslateAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque(this_val, JSSVGZoomEvent::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->previousTranslate()), 0);
        }
        case NewScaleAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque(this_val, JSSVGZoomEvent::js_class_id);
            return JS_NewInt32(ctx, imp->newScale());
        }
        case NewTranslateAttrNum: {
            SVGZoomEvent* imp = (SVGZoomEvent*)JS_GetOpaque(this_val, JSSVGZoomEvent::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->newTranslate()), 0);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
