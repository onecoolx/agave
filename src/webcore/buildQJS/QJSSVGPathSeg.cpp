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
#include "QJSSVGPathSeg.h"

#include "SVGPathSeg.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGPathSeg::getValueProperty, NULL, JSSVGPathSeg::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("pathSegType", JSSVGPathSeg::getValueProperty, NULL, JSSVGPathSeg::PathSegTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("pathSegTypeAsLetter", JSSVGPathSeg::getValueProperty, NULL, JSSVGPathSeg::PathSegTypeAsLetterAttrNum)
};

class JSSVGPathSegConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGPathSegConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGPathSegConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_CUBIC_SMOOTH_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_MOVETO_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_MOVETO_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_VERTICAL_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_VERTICAL_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CLOSEPATH", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CLOSEPATH),
    JS_CGETSET_MAGIC_DEF("PATHSEG_ARC_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_ARC_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_CUBIC_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_CUBIC_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_CUBIC_SMOOTH_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_CUBIC_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_CUBIC_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_QUADRATIC_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_UNKNOWN", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("PATHSEG_MOVETO_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_MOVETO_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_QUADRATIC_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_ARC_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_ARC_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_HORIZONTAL_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_HORIZONTAL_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_VERTICAL_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_VERTICAL_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL", JSSVGPathSegConstructor::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL)
};

JSValue JSSVGPathSegConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGPathSeg.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPathSegConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGPathSeg.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegConstructorFunctions, countof(JSSVGPathSegConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_CUBIC_SMOOTH_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_MOVETO_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_MOVETO_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_VERTICAL_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_VERTICAL_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CLOSEPATH", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CLOSEPATH),
    JS_CGETSET_MAGIC_DEF("PATHSEG_ARC_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_ARC_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_CUBIC_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_CUBIC_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_CUBIC_SMOOTH_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_CUBIC_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_CUBIC_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_QUADRATIC_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_UNKNOWN", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("PATHSEG_MOVETO_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_MOVETO_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_QUADRATIC_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_ARC_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_ARC_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_HORIZONTAL_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_HORIZONTAL_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_REL),
    JS_CGETSET_MAGIC_DEF("PATHSEG_LINETO_VERTICAL_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_LINETO_VERTICAL_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS),
    JS_CGETSET_MAGIC_DEF("PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL", JSSVGPathSegPrototype::getValueProperty, NULL, SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL)
};

JSValue JSSVGPathSegPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSeg.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPathSegPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSeg.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegAttributesFunctions, countof(JSSVGPathSegAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegPrototypeConstantsFunctions, countof(JSSVGPathSegPrototypeConstantsFunctions));
}

JSValue JSSVGPathSegPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGPathSegClassDefine = 
{
    "SVGPathSeg",
    .finalizer = JSSVGPathSeg::finalizer,
    .gc_mark = JSSVGPathSeg::mark,
};

JSClassID JSSVGPathSeg::js_class_id = 0;

void JSSVGPathSeg::init(JSContext* ctx)
{
    if (JSSVGPathSeg::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSeg::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSeg::js_class_id, &JSSVGPathSegClassDefine);
        JS_SetConstructor(ctx, JSSVGPathSegConstructor::self(ctx), JSSVGPathSegPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGPathSeg::js_class_id, JSSVGPathSegPrototype::self(ctx));
    }
}

JSValue JSSVGPathSeg::create(JSContext* ctx, SVGPathSeg* impl, SVGElement* context)
{
    JSSVGPathSeg::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegPrototype::self(ctx), JSSVGPathSeg::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSeg::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSeg* impl = (SVGPathSeg*)JS_GetOpaque(val, JSSVGPathSeg::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSeg::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGPathSeg::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PathSegTypeAttrNum: {
            SVGPathSeg* imp = (SVGPathSeg*)JS_GetOpaque2(ctx, this_val, JSSVGPathSeg::js_class_id);
            return JS_NewBigUint64(ctx, imp->pathSegType());
        }
        case PathSegTypeAsLetterAttrNum: {
            SVGPathSeg* imp = (SVGPathSeg*)JS_GetOpaque2(ctx, this_val, JSSVGPathSeg::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->pathSegTypeAsLetter()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGPathSeg::getConstructor(JSContext *ctx)
{
    return JSSVGPathSegConstructor::self(ctx);
}


SVGPathSeg* toSVGPathSeg(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGPathSeg* impl = (SVGPathSeg*)JS_GetOpaque(val, JSSVGPathSeg::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
