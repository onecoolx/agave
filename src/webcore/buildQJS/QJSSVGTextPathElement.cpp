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
#include "QJSSVGTextPathElement.h"

#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGTextPathElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGTextPathElementAttributesFunctions[5];
static bool JSSVGTextPathElementAttributesFunctions_initialized = false;

static void init_JSSVGTextPathElementAttributesFunctions()
{
    if (JSSVGTextPathElementAttributesFunctions_initialized) return;
    JSSVGTextPathElementAttributesFunctions_initialized = true;
    memset(JSSVGTextPathElementAttributesFunctions, 0, sizeof(JSSVGTextPathElementAttributesFunctions));
    JSSVGTextPathElementAttributesFunctions[0].name = "startOffset";
    JSSVGTextPathElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPathElementAttributesFunctions[0].magic = JSSVGTextPathElement::StartOffsetAttrNum;
    JSSVGTextPathElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGTextPathElement::getValueProperty;
    JSSVGTextPathElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGTextPathElementAttributesFunctions[1].name = "method";
    JSSVGTextPathElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPathElementAttributesFunctions[1].magic = JSSVGTextPathElement::MethodAttrNum;
    JSSVGTextPathElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGTextPathElement::getValueProperty;
    JSSVGTextPathElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGTextPathElementAttributesFunctions[2].name = "spacing";
    JSSVGTextPathElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPathElementAttributesFunctions[2].magic = JSSVGTextPathElement::SpacingAttrNum;
    JSSVGTextPathElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGTextPathElement::getValueProperty;
    JSSVGTextPathElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGTextPathElementAttributesFunctions[3].name = "href";
    JSSVGTextPathElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPathElementAttributesFunctions[3].magic = JSSVGTextPathElement::HrefAttrNum;
    JSSVGTextPathElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGTextPathElement::getValueProperty;
    JSSVGTextPathElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGTextPathElementAttributesFunctions[4].name = "constructor";
    JSSVGTextPathElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextPathElementAttributesFunctions[4].magic = JSSVGTextPathElement::ConstructorAttrNum;
    JSSVGTextPathElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGTextPathElement::getValueProperty;
    JSSVGTextPathElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

class JSSVGTextPathElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGTextPathElementConstructorFunctions[6];
static bool JSSVGTextPathElementConstructorFunctions_initialized = false;

static void init_JSSVGTextPathElementConstructorFunctions()
{
    if (JSSVGTextPathElementConstructorFunctions_initialized) return;
    JSSVGTextPathElementConstructorFunctions_initialized = true;
    memset(JSSVGTextPathElementConstructorFunctions, 0, sizeof(JSSVGTextPathElementConstructorFunctions));
    JSSVGTextPathElementConstructorFunctions[0].name = "TEXTPATH_METHODTYPE_UNKNOWN";
    JSSVGTextPathElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementConstructorFunctions[0].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_METHODTYPE_UNKNOWN;
    JSSVGTextPathElementConstructorFunctions[1].name = "TEXTPATH_METHODTYPE_ALIGN";
    JSSVGTextPathElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementConstructorFunctions[1].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_METHODTYPE_ALIGN;
    JSSVGTextPathElementConstructorFunctions[2].name = "TEXTPATH_METHODTYPE_STRETCH";
    JSSVGTextPathElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementConstructorFunctions[2].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_METHODTYPE_STRETCH;
    JSSVGTextPathElementConstructorFunctions[3].name = "TEXTPATH_SPACINGTYPE_UNKNOWN";
    JSSVGTextPathElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementConstructorFunctions[3].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_SPACINGTYPE_UNKNOWN;
    JSSVGTextPathElementConstructorFunctions[4].name = "TEXTPATH_SPACINGTYPE_AUTO";
    JSSVGTextPathElementConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementConstructorFunctions[4].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_SPACINGTYPE_AUTO;
    JSSVGTextPathElementConstructorFunctions[5].name = "TEXTPATH_SPACINGTYPE_EXACT";
    JSSVGTextPathElementConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementConstructorFunctions[5].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_SPACINGTYPE_EXACT;
}

JSValue JSSVGTextPathElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGTextPathElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTextPathElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGTextPathElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTextPathElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTextPathElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextPathElementConstructorFunctions, countof(JSSVGTextPathElementConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGTextPathElementPrototypeConstantsFunctions[6];
static bool JSSVGTextPathElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGTextPathElementPrototypeConstantsFunctions()
{
    if (JSSVGTextPathElementPrototypeConstantsFunctions_initialized) return;
    JSSVGTextPathElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGTextPathElementPrototypeConstantsFunctions, 0, sizeof(JSSVGTextPathElementPrototypeConstantsFunctions));
    JSSVGTextPathElementPrototypeConstantsFunctions[0].name = "TEXTPATH_METHODTYPE_UNKNOWN";
    JSSVGTextPathElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_METHODTYPE_UNKNOWN;
    JSSVGTextPathElementPrototypeConstantsFunctions[1].name = "TEXTPATH_METHODTYPE_ALIGN";
    JSSVGTextPathElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_METHODTYPE_ALIGN;
    JSSVGTextPathElementPrototypeConstantsFunctions[2].name = "TEXTPATH_METHODTYPE_STRETCH";
    JSSVGTextPathElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_METHODTYPE_STRETCH;
    JSSVGTextPathElementPrototypeConstantsFunctions[3].name = "TEXTPATH_SPACINGTYPE_UNKNOWN";
    JSSVGTextPathElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementPrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_SPACINGTYPE_UNKNOWN;
    JSSVGTextPathElementPrototypeConstantsFunctions[4].name = "TEXTPATH_SPACINGTYPE_AUTO";
    JSSVGTextPathElementPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementPrototypeConstantsFunctions[4].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_SPACINGTYPE_AUTO;
    JSSVGTextPathElementPrototypeConstantsFunctions[5].name = "TEXTPATH_SPACINGTYPE_EXACT";
    JSSVGTextPathElementPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextPathElementPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGTextPathElementPrototypeConstantsFunctions[5].u.i32 = (int32_t)SVGTextPathElement::TEXTPATH_SPACINGTYPE_EXACT;
}

JSValue JSSVGTextPathElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTextPathElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGTextContentElementPrototype::self(ctx));
        JSSVGTextPathElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTextPathElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTextPathElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTextPathElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextPathElementAttributesFunctions, countof(JSSVGTextPathElementAttributesFunctions));
    init_JSSVGTextPathElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextPathElementPrototypeConstantsFunctions, countof(JSSVGTextPathElementPrototypeConstantsFunctions));
}

static JSClassDef JSSVGTextPathElementClassDefine;
static bool JSSVGTextPathElementClassDefine_initialized = false;

static void init_JSSVGTextPathElementClassDefine()
{
    if (JSSVGTextPathElementClassDefine_initialized) return;
    JSSVGTextPathElementClassDefine_initialized = true;
    memset(&JSSVGTextPathElementClassDefine, 0, sizeof(JSSVGTextPathElementClassDefine));
    JSSVGTextPathElementClassDefine.class_name = "SVGTextPathElement";
    JSSVGTextPathElementClassDefine.finalizer = JSSVGTextPathElement::finalizer;
    JSSVGTextPathElementClassDefine.gc_mark = JSSVGTextPathElement::mark;
}

JSClassID JSSVGTextPathElement::js_class_id = 0;

void JSSVGTextPathElement::init(JSContext* ctx)
{
    if (JSSVGTextPathElement::js_class_id == 0) {
        init_JSSVGTextPathElementClassDefine();
        JS_NewClassID(&JSSVGTextPathElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTextPathElement::js_class_id, &JSSVGTextPathElementClassDefine);
        JS_SetConstructor(ctx, JSSVGTextPathElementConstructor::self(ctx), JSSVGTextPathElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGTextPathElement::js_class_id, JSSVGTextPathElementPrototype::self(ctx));
    }
}

JSValue JSSVGTextPathElement::create(JSContext* ctx, SVGTextPathElement* impl)
{
    JSSVGTextPathElement::init(ctx);
    JSValue _proto = JSSVGTextPathElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGTextPathElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTextPathElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTextPathElement* impl = (SVGTextPathElement*)JS_GetOpaque(val, JSSVGTextPathElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTextPathElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGTextContentElement::mark(rt, val, mark_func);
}

JSValue JSSVGTextPathElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case StartOffsetAttrNum: {
            SVGTextPathElement* imp = (SVGTextPathElement*)JS_GetOpaque(this_val, JSSVGTextPathElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->startOffsetAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MethodAttrNum: {
            SVGTextPathElement* imp = (SVGTextPathElement*)JS_GetOpaque(this_val, JSSVGTextPathElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->methodAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpacingAttrNum: {
            SVGTextPathElement* imp = (SVGTextPathElement*)JS_GetOpaque(this_val, JSSVGTextPathElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->spacingAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGTextPathElement* imp = (SVGTextPathElement*)JS_GetOpaque(this_val, JSSVGTextPathElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGTextPathElement::getConstructor(JSContext *ctx)
{
    return JSSVGTextPathElementConstructor::self(ctx);
}


}

#endif // ENABLE(SVG)
