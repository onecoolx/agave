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
#include "QJSSVGPreserveAspectRatio.h"

#include "SVGPreserveAspectRatio.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPreserveAspectRatioAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("meetOrSlice", JSSVGPreserveAspectRatio::getValueProperty, JSSVGPreserveAspectRatio::putValueProperty, JSSVGPreserveAspectRatio::MeetOrSliceAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSSVGPreserveAspectRatio::getValueProperty, JSSVGPreserveAspectRatio::putValueProperty, JSSVGPreserveAspectRatio::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGPreserveAspectRatio::getValueProperty, NULL, JSSVGPreserveAspectRatio::ConstructorAttrNum)
};

class JSSVGPreserveAspectRatioConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGPreserveAspectRatioConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGPreserveAspectRatioConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMAXYMAX", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMAX),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMINYMAX", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMAX),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_NONE", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_NONE),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_UNKNOWN", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMIDYMIN", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMIN),
    JS_CGETSET_MAGIC_DEF("SVG_MEETORSLICE_UNKNOWN", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_MEETORSLICE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_MEETORSLICE_SLICE", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_MEETORSLICE_SLICE),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMINYMIN", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMIN),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMAXYMID", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMID),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMAXYMIN", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMIN),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMINYMID", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMID),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMIDYMID", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMID),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMIDYMAX", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMAX),
    JS_CGETSET_MAGIC_DEF("SVG_MEETORSLICE_MEET", JSSVGPreserveAspectRatioConstructor::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_MEETORSLICE_MEET)
};

JSValue JSSVGPreserveAspectRatioConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGPreserveAspectRatio.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPreserveAspectRatioConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGPreserveAspectRatio.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPreserveAspectRatioConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPreserveAspectRatioConstructorFunctions, countof(JSSVGPreserveAspectRatioConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGPreserveAspectRatioPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMAXYMAX", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMAX),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMINYMAX", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMAX),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_NONE", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_NONE),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_UNKNOWN", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMIDYMIN", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMIN),
    JS_CGETSET_MAGIC_DEF("SVG_MEETORSLICE_UNKNOWN", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_MEETORSLICE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_MEETORSLICE_SLICE", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_MEETORSLICE_SLICE),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMINYMIN", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMIN),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMAXYMID", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMID),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMAXYMIN", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMIN),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMINYMID", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMID),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMIDYMID", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMID),
    JS_CGETSET_MAGIC_DEF("SVG_PRESERVEASPECTRATIO_XMIDYMAX", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMAX),
    JS_CGETSET_MAGIC_DEF("SVG_MEETORSLICE_MEET", JSSVGPreserveAspectRatioPrototype::getValueProperty, NULL, SVGPreserveAspectRatio::SVG_MEETORSLICE_MEET)
};

JSValue JSSVGPreserveAspectRatioPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPreserveAspectRatio.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPreserveAspectRatioPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPreserveAspectRatio.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPreserveAspectRatioPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPreserveAspectRatioAttributesFunctions, countof(JSSVGPreserveAspectRatioAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPreserveAspectRatioPrototypeConstantsFunctions, countof(JSSVGPreserveAspectRatioPrototypeConstantsFunctions));
}

JSValue JSSVGPreserveAspectRatioPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGPreserveAspectRatioClassDefine = 
{
    "SVGPreserveAspectRatio",
    .finalizer = JSSVGPreserveAspectRatio::finalizer,
    .gc_mark = JSSVGPreserveAspectRatio::mark,
};

JSClassID JSSVGPreserveAspectRatio::js_class_id = 0;

void JSSVGPreserveAspectRatio::init(JSContext* ctx)
{
    if (JSSVGPreserveAspectRatio::js_class_id == 0) {
        JS_NewClassID(&JSSVGPreserveAspectRatio::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPreserveAspectRatio::js_class_id, &JSSVGPreserveAspectRatioClassDefine);
        JS_SetConstructor(ctx, JSSVGPreserveAspectRatioConstructor::self(ctx), JSSVGPreserveAspectRatioPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGPreserveAspectRatio::js_class_id, JSSVGPreserveAspectRatioPrototype::self(ctx));
    }
}

JSValue JSSVGPreserveAspectRatio::create(JSContext* ctx, SVGPreserveAspectRatio* impl, SVGElement* context)
{
    JSSVGPreserveAspectRatio::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPreserveAspectRatioPrototype::self(ctx), JSSVGPreserveAspectRatio::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPreserveAspectRatio::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPreserveAspectRatio* impl = (SVGPreserveAspectRatio*)JS_GetOpaque(val, JSSVGPreserveAspectRatio::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPreserveAspectRatio::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGPreserveAspectRatio::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            SVGPreserveAspectRatio* imp = (SVGPreserveAspectRatio*)JS_GetOpaque2(ctx, this_val, JSSVGPreserveAspectRatio::js_class_id);
            return JS_NewBigUint64(ctx, imp->align());
        }
        case MeetOrSliceAttrNum: {
            SVGPreserveAspectRatio* imp = (SVGPreserveAspectRatio*)JS_GetOpaque2(ctx, this_val, JSSVGPreserveAspectRatio::js_class_id);
            return JS_NewBigUint64(ctx, imp->meetOrSlice());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGPreserveAspectRatio::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            SVGPreserveAspectRatio* imp = (SVGPreserveAspectRatio*)JS_GetOpaque2(ctx, this_val, JSSVGPreserveAspectRatio::js_class_id);
            imp->setAlign(valueToInt32(ctx, value));
            break;
        }
        case MeetOrSliceAttrNum: {
            SVGPreserveAspectRatio* imp = (SVGPreserveAspectRatio*)JS_GetOpaque2(ctx, this_val, JSSVGPreserveAspectRatio::js_class_id);
            imp->setMeetOrSlice(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue JSSVGPreserveAspectRatio::getConstructor(JSContext *ctx)
{
    return JSSVGPreserveAspectRatioConstructor::self(ctx);
}

JSValue toJS(JSContext *ctx, SVGPreserveAspectRatio* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGPreserveAspectRatio, JSSVGPreserveAspectRatio>(ctx, obj, context);
}

SVGPreserveAspectRatio* toSVGPreserveAspectRatio(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGPreserveAspectRatio* impl = (SVGPreserveAspectRatio*)JS_GetOpaque(val, JSSVGPreserveAspectRatio::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
