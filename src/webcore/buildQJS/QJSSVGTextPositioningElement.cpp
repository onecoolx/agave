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
#include "QJSSVGTextPositioningElement.h"

#include "QJSSVGAnimatedLengthList.h"
#include "QJSSVGAnimatedNumberList.h"
#include "SVGTextPositioningElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGTextPositioningElementAttributesFunctions[5];
static bool JSSVGTextPositioningElementAttributesFunctions_initialized = false;

static void init_JSSVGTextPositioningElementAttributesFunctions()
{
    if (JSSVGTextPositioningElementAttributesFunctions_initialized) return;
    JSSVGTextPositioningElementAttributesFunctions_initialized = true;
    memset(JSSVGTextPositioningElementAttributesFunctions, 0, sizeof(JSSVGTextPositioningElementAttributesFunctions));
    JSSVGTextPositioningElementAttributesFunctions[0].name = "y";
    JSSVGTextPositioningElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPositioningElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPositioningElementAttributesFunctions[0].magic = JSSVGTextPositioningElement::YAttrNum;
    JSSVGTextPositioningElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGTextPositioningElement::getValueProperty;
    JSSVGTextPositioningElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGTextPositioningElementAttributesFunctions[1].name = "x";
    JSSVGTextPositioningElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPositioningElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPositioningElementAttributesFunctions[1].magic = JSSVGTextPositioningElement::XAttrNum;
    JSSVGTextPositioningElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGTextPositioningElement::getValueProperty;
    JSSVGTextPositioningElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGTextPositioningElementAttributesFunctions[2].name = "dx";
    JSSVGTextPositioningElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPositioningElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPositioningElementAttributesFunctions[2].magic = JSSVGTextPositioningElement::DxAttrNum;
    JSSVGTextPositioningElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGTextPositioningElement::getValueProperty;
    JSSVGTextPositioningElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGTextPositioningElementAttributesFunctions[3].name = "dy";
    JSSVGTextPositioningElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPositioningElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPositioningElementAttributesFunctions[3].magic = JSSVGTextPositioningElement::DyAttrNum;
    JSSVGTextPositioningElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGTextPositioningElement::getValueProperty;
    JSSVGTextPositioningElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGTextPositioningElementAttributesFunctions[4].name = "rotate";
    JSSVGTextPositioningElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPositioningElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPositioningElementAttributesFunctions[4].magic = JSSVGTextPositioningElement::RotateAttrNum;
    JSSVGTextPositioningElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGTextPositioningElement::getValueProperty;
    JSSVGTextPositioningElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGTextPositioningElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTextPositioningElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGTextContentElementPrototype::self(ctx));
        JSSVGTextPositioningElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTextPositioningElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTextPositioningElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTextPositioningElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextPositioningElementAttributesFunctions, countof(JSSVGTextPositioningElementAttributesFunctions));
}

static JSClassDef JSSVGTextPositioningElementClassDefine;
static bool JSSVGTextPositioningElementClassDefine_initialized = false;

static void init_JSSVGTextPositioningElementClassDefine()
{
    if (JSSVGTextPositioningElementClassDefine_initialized) return;
    JSSVGTextPositioningElementClassDefine_initialized = true;
    memset(&JSSVGTextPositioningElementClassDefine, 0, sizeof(JSSVGTextPositioningElementClassDefine));
    JSSVGTextPositioningElementClassDefine.class_name = "SVGTextPositioningElement";
    JSSVGTextPositioningElementClassDefine.finalizer = JSSVGTextPositioningElement::finalizer;
    JSSVGTextPositioningElementClassDefine.gc_mark = JSSVGTextPositioningElement::mark;
}

JSClassID JSSVGTextPositioningElement::js_class_id = 0;

void JSSVGTextPositioningElement::init(JSContext* ctx)
{
    if (JSSVGTextPositioningElement::js_class_id == 0) {
        init_JSSVGTextPositioningElementClassDefine();
        JS_NewClassID(&JSSVGTextPositioningElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTextPositioningElement::js_class_id, &JSSVGTextPositioningElementClassDefine);
        JS_SetClassProto(ctx, JSSVGTextPositioningElement::js_class_id, JSSVGTextPositioningElementPrototype::self(ctx));
    }
}

JSValue JSSVGTextPositioningElement::create(JSContext* ctx, SVGTextPositioningElement* impl)
{
    JSSVGTextPositioningElement::init(ctx);
    JSValue _proto = JSSVGTextPositioningElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGTextPositioningElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTextPositioningElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTextPositioningElement* impl = (SVGTextPositioningElement*)JS_GetOpaque(val, JSSVGTextPositioningElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTextPositioningElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGTextContentElement::mark(rt, val, mark_func);
}

JSValue JSSVGTextPositioningElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque(this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedLengthList> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque(this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedLengthList> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case DxAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque(this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedLengthList> obj = imp->dxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case DyAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque(this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedLengthList> obj = imp->dyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RotateAttrNum: {
            SVGTextPositioningElement* imp = (SVGTextPositioningElement*)JS_GetOpaque(this_val, JSSVGTextPositioningElement::js_class_id);
            RefPtr<SVGAnimatedNumberList> obj = imp->rotateAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
