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
#include "QJSSVGUnitTypes.h"

#include "SVGUnitTypes.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGUnitTypesAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGUnitTypes::getValueProperty, NULL, JSSVGUnitTypes::ConstructorAttrNum)
};

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

static const JSCFunctionListEntry JSSVGUnitTypesConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_UNIT_TYPE_UNKNOWN", JSSVGUnitTypesConstructor::getValueProperty, NULL, SVGUnitTypes::SVG_UNIT_TYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_UNIT_TYPE_USERSPACEONUSE", JSSVGUnitTypesConstructor::getValueProperty, NULL, SVGUnitTypes::SVG_UNIT_TYPE_USERSPACEONUSE),
    JS_CGETSET_MAGIC_DEF("SVG_UNIT_TYPE_OBJECTBOUNDINGBOX", JSSVGUnitTypesConstructor::getValueProperty, NULL, SVGUnitTypes::SVG_UNIT_TYPE_OBJECTBOUNDINGBOX)
};

JSValue JSSVGUnitTypesConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGUnitTypes.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGUnitTypesConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGUnitTypes.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGUnitTypesConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUnitTypesConstructorFunctions, countof(JSSVGUnitTypesConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGUnitTypesPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_UNIT_TYPE_UNKNOWN", JSSVGUnitTypesPrototype::getValueProperty, NULL, SVGUnitTypes::SVG_UNIT_TYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_UNIT_TYPE_USERSPACEONUSE", JSSVGUnitTypesPrototype::getValueProperty, NULL, SVGUnitTypes::SVG_UNIT_TYPE_USERSPACEONUSE),
    JS_CGETSET_MAGIC_DEF("SVG_UNIT_TYPE_OBJECTBOUNDINGBOX", JSSVGUnitTypesPrototype::getValueProperty, NULL, SVGUnitTypes::SVG_UNIT_TYPE_OBJECTBOUNDINGBOX)
};

JSValue JSSVGUnitTypesPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGUnitTypes.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGUnitTypesPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGUnitTypes.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGUnitTypesPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUnitTypesAttributesFunctions, countof(JSSVGUnitTypesAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUnitTypesPrototypeConstantsFunctions, countof(JSSVGUnitTypesPrototypeConstantsFunctions));
}

JSValue JSSVGUnitTypesPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGUnitTypesClassDefine = 
{
    "SVGUnitTypes",
    .finalizer = JSSVGUnitTypes::finalizer,
    .gc_mark = JSSVGUnitTypes::mark,
};

JSClassID JSSVGUnitTypes::js_class_id = 0;

void JSSVGUnitTypes::init(JSContext* ctx)
{
    if (JSSVGUnitTypes::js_class_id == 0) {
        JS_NewClassID(&JSSVGUnitTypes::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGUnitTypes::js_class_id, &JSSVGUnitTypesClassDefine);
        JS_SetConstructor(ctx, JSSVGUnitTypesConstructor::self(ctx), JSSVGUnitTypesPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGUnitTypes::js_class_id, JSSVGUnitTypesPrototype::self(ctx));
    }
}

JSValue JSSVGUnitTypes::create(JSContext* ctx, SVGUnitTypes* impl, SVGElement* context)
{
    JSSVGUnitTypes::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGUnitTypesPrototype::self(ctx), JSSVGUnitTypes::js_class_id);
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
