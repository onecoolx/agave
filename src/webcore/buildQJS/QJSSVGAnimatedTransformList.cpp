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
#include "QJSSVGAnimatedTransformList.h"

#include "QJSSVGTransformList.h"
#include "SVGTransformList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGAnimatedTransformListAttributesFunctions[2];
static bool JSSVGAnimatedTransformListAttributesFunctions_initialized = false;

static void init_JSSVGAnimatedTransformListAttributesFunctions()
{
    if (JSSVGAnimatedTransformListAttributesFunctions_initialized) return;
    JSSVGAnimatedTransformListAttributesFunctions_initialized = true;
    memset(JSSVGAnimatedTransformListAttributesFunctions, 0, sizeof(JSSVGAnimatedTransformListAttributesFunctions));
    JSSVGAnimatedTransformListAttributesFunctions[0].name = "baseVal";
    JSSVGAnimatedTransformListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedTransformListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedTransformListAttributesFunctions[0].magic = JSSVGAnimatedTransformList::BaseValAttrNum;
    JSSVGAnimatedTransformListAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAnimatedTransformList::getValueProperty;
    JSSVGAnimatedTransformListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGAnimatedTransformListAttributesFunctions[1].name = "animVal";
    JSSVGAnimatedTransformListAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedTransformListAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedTransformListAttributesFunctions[1].magic = JSSVGAnimatedTransformList::AnimValAttrNum;
    JSSVGAnimatedTransformListAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAnimatedTransformList::getValueProperty;
    JSSVGAnimatedTransformListAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGAnimatedTransformListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedTransformList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedTransformListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedTransformList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnimatedTransformListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAnimatedTransformListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedTransformListAttributesFunctions, countof(JSSVGAnimatedTransformListAttributesFunctions));
}

static JSClassDef JSSVGAnimatedTransformListClassDefine;
static bool JSSVGAnimatedTransformListClassDefine_initialized = false;

static void init_JSSVGAnimatedTransformListClassDefine()
{
    if (JSSVGAnimatedTransformListClassDefine_initialized) return;
    JSSVGAnimatedTransformListClassDefine_initialized = true;
    memset(&JSSVGAnimatedTransformListClassDefine, 0, sizeof(JSSVGAnimatedTransformListClassDefine));
    JSSVGAnimatedTransformListClassDefine.class_name = "SVGAnimatedTransformList";
    JSSVGAnimatedTransformListClassDefine.finalizer = JSSVGAnimatedTransformList::finalizer;
    JSSVGAnimatedTransformListClassDefine.gc_mark = JSSVGAnimatedTransformList::mark;
}

JSClassID JSSVGAnimatedTransformList::js_class_id = 0;

void JSSVGAnimatedTransformList::init(JSContext* ctx)
{
    if (JSSVGAnimatedTransformList::js_class_id == 0) {
        init_JSSVGAnimatedTransformListClassDefine();
        JS_NewClassID(&JSSVGAnimatedTransformList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedTransformList::js_class_id, &JSSVGAnimatedTransformListClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedTransformList::js_class_id, JSSVGAnimatedTransformListPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedTransformList::create(JSContext* ctx, SVGAnimatedTransformList* impl, SVGElement* context)
{
    JSSVGAnimatedTransformList::init(ctx);
    JSValue _proto = JSSVGAnimatedTransformListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAnimatedTransformList::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedTransformList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedTransformList* impl = (SVGAnimatedTransformList*)JS_GetOpaque(val, JSSVGAnimatedTransformList::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedTransformList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedTransformList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedTransformList* imp = (SVGAnimatedTransformList*)JS_GetOpaque(this_val, JSSVGAnimatedTransformList::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->baseVal()), context());
        }
        case AnimValAttrNum: {
            SVGAnimatedTransformList* imp = (SVGAnimatedTransformList*)JS_GetOpaque(this_val, JSSVGAnimatedTransformList::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animVal()), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedTransformList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedTransformList, JSSVGAnimatedTransformList>(ctx, obj, context);
}

SVGAnimatedTransformList* toSVGAnimatedTransformList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedTransformList* impl = (SVGAnimatedTransformList*)JS_GetOpaque(val, JSSVGAnimatedTransformList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
