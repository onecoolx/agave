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
#include "QJSSVGLinearGradientElement.h"

#include "QJSSVGAnimatedLength.h"
#include "SVGLinearGradientElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGLinearGradientElementAttributesFunctions[4];
static bool JSSVGLinearGradientElementAttributesFunctions_initialized = false;

static void init_JSSVGLinearGradientElementAttributesFunctions()
{
    if (JSSVGLinearGradientElementAttributesFunctions_initialized) return;
    JSSVGLinearGradientElementAttributesFunctions_initialized = true;
    memset(JSSVGLinearGradientElementAttributesFunctions, 0, sizeof(JSSVGLinearGradientElementAttributesFunctions));
    JSSVGLinearGradientElementAttributesFunctions[0].name = "x1";
    JSSVGLinearGradientElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLinearGradientElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLinearGradientElementAttributesFunctions[0].magic = JSSVGLinearGradientElement::X1AttrNum;
    JSSVGLinearGradientElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGLinearGradientElement::getValueProperty;
    JSSVGLinearGradientElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGLinearGradientElementAttributesFunctions[1].name = "y1";
    JSSVGLinearGradientElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLinearGradientElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLinearGradientElementAttributesFunctions[1].magic = JSSVGLinearGradientElement::Y1AttrNum;
    JSSVGLinearGradientElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGLinearGradientElement::getValueProperty;
    JSSVGLinearGradientElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGLinearGradientElementAttributesFunctions[2].name = "x2";
    JSSVGLinearGradientElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLinearGradientElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLinearGradientElementAttributesFunctions[2].magic = JSSVGLinearGradientElement::X2AttrNum;
    JSSVGLinearGradientElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGLinearGradientElement::getValueProperty;
    JSSVGLinearGradientElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGLinearGradientElementAttributesFunctions[3].name = "y2";
    JSSVGLinearGradientElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLinearGradientElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLinearGradientElementAttributesFunctions[3].magic = JSSVGLinearGradientElement::Y2AttrNum;
    JSSVGLinearGradientElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGLinearGradientElement::getValueProperty;
    JSSVGLinearGradientElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGLinearGradientElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGLinearGradientElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGGradientElementPrototype::self(ctx));
        JSSVGLinearGradientElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGLinearGradientElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGLinearGradientElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGLinearGradientElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLinearGradientElementAttributesFunctions, countof(JSSVGLinearGradientElementAttributesFunctions));
}

static JSClassDef JSSVGLinearGradientElementClassDefine;
static bool JSSVGLinearGradientElementClassDefine_initialized = false;

static void init_JSSVGLinearGradientElementClassDefine()
{
    if (JSSVGLinearGradientElementClassDefine_initialized) return;
    JSSVGLinearGradientElementClassDefine_initialized = true;
    memset(&JSSVGLinearGradientElementClassDefine, 0, sizeof(JSSVGLinearGradientElementClassDefine));
    JSSVGLinearGradientElementClassDefine.class_name = "SVGLinearGradientElement";
    JSSVGLinearGradientElementClassDefine.finalizer = JSSVGLinearGradientElement::finalizer;
    JSSVGLinearGradientElementClassDefine.gc_mark = JSSVGLinearGradientElement::mark;
}

JSClassID JSSVGLinearGradientElement::js_class_id = 0;

void JSSVGLinearGradientElement::init(JSContext* ctx)
{
    if (JSSVGLinearGradientElement::js_class_id == 0) {
        init_JSSVGLinearGradientElementClassDefine();
        JS_NewClassID(&JSSVGLinearGradientElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGLinearGradientElement::js_class_id, &JSSVGLinearGradientElementClassDefine);
        JS_SetClassProto(ctx, JSSVGLinearGradientElement::js_class_id, JSSVGLinearGradientElementPrototype::self(ctx));
    }
}

JSValue JSSVGLinearGradientElement::create(JSContext* ctx, SVGLinearGradientElement* impl)
{
    JSSVGLinearGradientElement::init(ctx);
    JSValue _proto = JSSVGLinearGradientElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGLinearGradientElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGLinearGradientElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGLinearGradientElement* impl = (SVGLinearGradientElement*)JS_GetOpaque(val, JSSVGLinearGradientElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGLinearGradientElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGGradientElement::mark(rt, val, mark_func);
}

JSValue JSSVGLinearGradientElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case X1AttrNum: {
            SVGLinearGradientElement* imp = (SVGLinearGradientElement*)JS_GetOpaque(this_val, JSSVGLinearGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->x1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case Y1AttrNum: {
            SVGLinearGradientElement* imp = (SVGLinearGradientElement*)JS_GetOpaque(this_val, JSSVGLinearGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->y1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case X2AttrNum: {
            SVGLinearGradientElement* imp = (SVGLinearGradientElement*)JS_GetOpaque(this_val, JSSVGLinearGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->x2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case Y2AttrNum: {
            SVGLinearGradientElement* imp = (SVGLinearGradientElement*)JS_GetOpaque(this_val, JSSVGLinearGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->y2Animated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
