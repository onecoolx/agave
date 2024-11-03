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
#include "QJSSVGTextPathElement.h"

#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGTextPathElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGTextPathElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("href", JSSVGTextPathElement::getValueProperty, NULL, JSSVGTextPathElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("spacing", JSSVGTextPathElement::getValueProperty, NULL, JSSVGTextPathElement::SpacingAttrNum),
    JS_CGETSET_MAGIC_DEF("startOffset", JSSVGTextPathElement::getValueProperty, NULL, JSSVGTextPathElement::StartOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("method", JSSVGTextPathElement::getValueProperty, NULL, JSSVGTextPathElement::MethodAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGTextPathElement::getValueProperty, NULL, JSSVGTextPathElement::ConstructorAttrNum)
};

class JSSVGTextPathElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGTextPathElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGTextPathElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("TEXTPATH_METHODTYPE_UNKNOWN", JSSVGTextPathElementConstructor::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_METHODTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_SPACINGTYPE_UNKNOWN", JSSVGTextPathElementConstructor::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_SPACINGTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_METHODTYPE_ALIGN", JSSVGTextPathElementConstructor::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_METHODTYPE_ALIGN),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_METHODTYPE_STRETCH", JSSVGTextPathElementConstructor::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_METHODTYPE_STRETCH),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_SPACINGTYPE_EXACT", JSSVGTextPathElementConstructor::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_SPACINGTYPE_EXACT),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_SPACINGTYPE_AUTO", JSSVGTextPathElementConstructor::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_SPACINGTYPE_AUTO)
};

JSValue JSSVGTextPathElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGTextPathElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTextPathElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGTextPathElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTextPathElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextPathElementConstructorFunctions, countof(JSSVGTextPathElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGTextPathElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("TEXTPATH_METHODTYPE_UNKNOWN", JSSVGTextPathElementPrototype::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_METHODTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_SPACINGTYPE_UNKNOWN", JSSVGTextPathElementPrototype::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_SPACINGTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_METHODTYPE_ALIGN", JSSVGTextPathElementPrototype::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_METHODTYPE_ALIGN),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_METHODTYPE_STRETCH", JSSVGTextPathElementPrototype::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_METHODTYPE_STRETCH),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_SPACINGTYPE_EXACT", JSSVGTextPathElementPrototype::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_SPACINGTYPE_EXACT),
    JS_CGETSET_MAGIC_DEF("TEXTPATH_SPACINGTYPE_AUTO", JSSVGTextPathElementPrototype::getValueProperty, NULL, SVGTextPathElement::TEXTPATH_SPACINGTYPE_AUTO)
};

JSValue JSSVGTextPathElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTextPathElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGTextContentElementPrototype::self(ctx));
        JSSVGTextPathElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTextPathElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTextPathElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextPathElementAttributesFunctions, countof(JSSVGTextPathElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextPathElementPrototypeConstantsFunctions, countof(JSSVGTextPathElementPrototypeConstantsFunctions));
}

JSValue JSSVGTextPathElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGTextPathElementClassDefine = 
{
    "SVGTextPathElement",
    .finalizer = JSSVGTextPathElement::finalizer,
    .gc_mark = JSSVGTextPathElement::mark,
};

JSClassID JSSVGTextPathElement::js_class_id = 0;

void JSSVGTextPathElement::init(JSContext* ctx)
{
    if (JSSVGTextPathElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGTextPathElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTextPathElement::js_class_id, &JSSVGTextPathElementClassDefine);
        JS_SetConstructor(ctx, JSSVGTextPathElementConstructor::self(ctx), JSSVGTextPathElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGTextPathElement::js_class_id, JSSVGTextPathElementPrototype::self(ctx));
    }
}

JSValue JSSVGTextPathElement::create(JSContext* ctx, SVGTextPathElement* impl)
{
    JSSVGTextPathElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGTextPathElementPrototype::self(ctx), JSSVGTextPathElement::js_class_id);
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
            SVGTextPathElement* imp = (SVGTextPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPathElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->startOffsetAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MethodAttrNum: {
            SVGTextPathElement* imp = (SVGTextPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPathElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->methodAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpacingAttrNum: {
            SVGTextPathElement* imp = (SVGTextPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPathElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->spacingAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGTextPathElement* imp = (SVGTextPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextPathElement::js_class_id);
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
