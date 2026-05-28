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
#include "QJSSVGUnitTypes.h"

#include "SVGUnitTypes.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGUnitTypesAttributesFunctions[1];
static bool JSSVGUnitTypesAttributesFunctions_initialized = false;

static void init_JSSVGUnitTypesAttributesFunctions()
{
    if (JSSVGUnitTypesAttributesFunctions_initialized) return;
    JSSVGUnitTypesAttributesFunctions_initialized = true;
    memset(JSSVGUnitTypesAttributesFunctions, 0, sizeof(JSSVGUnitTypesAttributesFunctions));
    JSSVGUnitTypesAttributesFunctions[0].name = "constructor";
    JSSVGUnitTypesAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUnitTypesAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUnitTypesAttributesFunctions[0].magic = JSSVGUnitTypes::ConstructorAttrNum;
    JSSVGUnitTypesAttributesFunctions[0].u.getset.get.getter_magic = JSSVGUnitTypes::getValueProperty;
    JSSVGUnitTypesAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSSVGUnitTypesConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGUnitTypesConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGUnitTypesConstructorFunctions[3];
static bool JSSVGUnitTypesConstructorFunctions_initialized = false;

static void init_JSSVGUnitTypesConstructorFunctions()
{
    if (JSSVGUnitTypesConstructorFunctions_initialized) return;
    JSSVGUnitTypesConstructorFunctions_initialized = true;
    memset(JSSVGUnitTypesConstructorFunctions, 0, sizeof(JSSVGUnitTypesConstructorFunctions));
    JSSVGUnitTypesConstructorFunctions[0].name = "SVG_UNIT_TYPE_UNKNOWN";
    JSSVGUnitTypesConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUnitTypesConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUnitTypesConstructorFunctions[0].magic = SVGUnitTypes::SVG_UNIT_TYPE_UNKNOWN;
    JSSVGUnitTypesConstructorFunctions[0].u.getset.get.getter_magic = JSSVGUnitTypesConstructor::getValueProperty;
    JSSVGUnitTypesConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGUnitTypesConstructorFunctions[1].name = "SVG_UNIT_TYPE_USERSPACEONUSE";
    JSSVGUnitTypesConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUnitTypesConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUnitTypesConstructorFunctions[1].magic = SVGUnitTypes::SVG_UNIT_TYPE_USERSPACEONUSE;
    JSSVGUnitTypesConstructorFunctions[1].u.getset.get.getter_magic = JSSVGUnitTypesConstructor::getValueProperty;
    JSSVGUnitTypesConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGUnitTypesConstructorFunctions[2].name = "SVG_UNIT_TYPE_OBJECTBOUNDINGBOX";
    JSSVGUnitTypesConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUnitTypesConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUnitTypesConstructorFunctions[2].magic = SVGUnitTypes::SVG_UNIT_TYPE_OBJECTBOUNDINGBOX;
    JSSVGUnitTypesConstructorFunctions[2].u.getset.get.getter_magic = JSSVGUnitTypesConstructor::getValueProperty;
    JSSVGUnitTypesConstructorFunctions[2].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGUnitTypesConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGUnitTypes.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGUnitTypesConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGUnitTypes.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGUnitTypesConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGUnitTypesConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUnitTypesConstructorFunctions, countof(JSSVGUnitTypesConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSSVGUnitTypesPrototypeConstantsFunctions[3];
static bool JSSVGUnitTypesPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGUnitTypesPrototypeConstantsFunctions()
{
    if (JSSVGUnitTypesPrototypeConstantsFunctions_initialized) return;
    JSSVGUnitTypesPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGUnitTypesPrototypeConstantsFunctions, 0, sizeof(JSSVGUnitTypesPrototypeConstantsFunctions));
    JSSVGUnitTypesPrototypeConstantsFunctions[0].name = "SVG_UNIT_TYPE_UNKNOWN";
    JSSVGUnitTypesPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUnitTypesPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUnitTypesPrototypeConstantsFunctions[0].magic = SVGUnitTypes::SVG_UNIT_TYPE_UNKNOWN;
    JSSVGUnitTypesPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSSVGUnitTypesPrototype::getValueProperty;
    JSSVGUnitTypesPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGUnitTypesPrototypeConstantsFunctions[1].name = "SVG_UNIT_TYPE_USERSPACEONUSE";
    JSSVGUnitTypesPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUnitTypesPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUnitTypesPrototypeConstantsFunctions[1].magic = SVGUnitTypes::SVG_UNIT_TYPE_USERSPACEONUSE;
    JSSVGUnitTypesPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSSVGUnitTypesPrototype::getValueProperty;
    JSSVGUnitTypesPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGUnitTypesPrototypeConstantsFunctions[2].name = "SVG_UNIT_TYPE_OBJECTBOUNDINGBOX";
    JSSVGUnitTypesPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUnitTypesPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUnitTypesPrototypeConstantsFunctions[2].magic = SVGUnitTypes::SVG_UNIT_TYPE_OBJECTBOUNDINGBOX;
    JSSVGUnitTypesPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSSVGUnitTypesPrototype::getValueProperty;
    JSSVGUnitTypesPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGUnitTypesPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGUnitTypes.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGUnitTypesPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGUnitTypes.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGUnitTypesPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGUnitTypesAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUnitTypesAttributesFunctions, countof(JSSVGUnitTypesAttributesFunctions));
    init_JSSVGUnitTypesPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUnitTypesPrototypeConstantsFunctions, countof(JSSVGUnitTypesPrototypeConstantsFunctions));
}

JSValue JSSVGUnitTypesPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGUnitTypesClassDefine;
static bool JSSVGUnitTypesClassDefine_initialized = false;

static void init_JSSVGUnitTypesClassDefine()
{
    if (JSSVGUnitTypesClassDefine_initialized) return;
    JSSVGUnitTypesClassDefine_initialized = true;
    memset(&JSSVGUnitTypesClassDefine, 0, sizeof(JSSVGUnitTypesClassDefine));
    JSSVGUnitTypesClassDefine.class_name = "SVGUnitTypes";
    JSSVGUnitTypesClassDefine.finalizer = JSSVGUnitTypes::finalizer;
    JSSVGUnitTypesClassDefine.gc_mark = JSSVGUnitTypes::mark;
}

JSClassID JSSVGUnitTypes::js_class_id = 0;

void JSSVGUnitTypes::init(JSContext* ctx)
{
    if (JSSVGUnitTypes::js_class_id == 0) {
        init_JSSVGUnitTypesClassDefine();
        JS_NewClassID(&JSSVGUnitTypes::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGUnitTypes::js_class_id, &JSSVGUnitTypesClassDefine);
        JS_SetConstructor(ctx, JSSVGUnitTypesConstructor::self(ctx), JSSVGUnitTypesPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGUnitTypes::js_class_id, JSSVGUnitTypesPrototype::self(ctx));
    }
}

JSValue JSSVGUnitTypes::create(JSContext* ctx, SVGUnitTypes* impl, SVGElement* context)
{
    JSSVGUnitTypes::init(ctx);
    JSValue _proto = JSSVGUnitTypesPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGUnitTypes::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGUnitTypes::finalizer(JSRuntime* rt, JSValue val)
{
    SVGUnitTypes* impl = (SVGUnitTypes*)JS_GetOpaque(val, JSSVGUnitTypes::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGUnitTypes::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGUnitTypes::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGUnitTypes::getConstructor(JSContext *ctx)
{
    return JSSVGUnitTypesConstructor::self(ctx);
}

JSValue toJS(JSContext *ctx, SVGUnitTypes* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGUnitTypes, JSSVGUnitTypes>(ctx, obj, context);
}

SVGUnitTypes* toSVGUnitTypes(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGUnitTypes* impl = (SVGUnitTypes*)JS_GetOpaque(val, JSSVGUnitTypes::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
