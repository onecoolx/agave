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
#include "QJSSVGPreserveAspectRatio.h"

#include "SVGPreserveAspectRatio.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPreserveAspectRatioAttributesFunctions[3];
static bool JSSVGPreserveAspectRatioAttributesFunctions_initialized = false;

static void init_JSSVGPreserveAspectRatioAttributesFunctions()
{
    if (JSSVGPreserveAspectRatioAttributesFunctions_initialized) return;
    JSSVGPreserveAspectRatioAttributesFunctions_initialized = true;
    memset(JSSVGPreserveAspectRatioAttributesFunctions, 0, sizeof(JSSVGPreserveAspectRatioAttributesFunctions));
    JSSVGPreserveAspectRatioAttributesFunctions[0].name = "meetOrSlice";
    JSSVGPreserveAspectRatioAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPreserveAspectRatioAttributesFunctions[0].magic = JSSVGPreserveAspectRatio::MeetOrSliceAttrNum;
    JSSVGPreserveAspectRatioAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPreserveAspectRatio::getValueProperty;
    JSSVGPreserveAspectRatioAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPreserveAspectRatio::putValueProperty;
    JSSVGPreserveAspectRatioAttributesFunctions[1].name = "align";
    JSSVGPreserveAspectRatioAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPreserveAspectRatioAttributesFunctions[1].magic = JSSVGPreserveAspectRatio::AlignAttrNum;
    JSSVGPreserveAspectRatioAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPreserveAspectRatio::getValueProperty;
    JSSVGPreserveAspectRatioAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPreserveAspectRatio::putValueProperty;
    JSSVGPreserveAspectRatioAttributesFunctions[2].name = "constructor";
    JSSVGPreserveAspectRatioAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPreserveAspectRatioAttributesFunctions[2].magic = JSSVGPreserveAspectRatio::ConstructorAttrNum;
    JSSVGPreserveAspectRatioAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPreserveAspectRatio::getValueProperty;
    JSSVGPreserveAspectRatioAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSSVGPreserveAspectRatioConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGPreserveAspectRatioConstructorFunctions[14];
static bool JSSVGPreserveAspectRatioConstructorFunctions_initialized = false;

static void init_JSSVGPreserveAspectRatioConstructorFunctions()
{
    if (JSSVGPreserveAspectRatioConstructorFunctions_initialized) return;
    JSSVGPreserveAspectRatioConstructorFunctions_initialized = true;
    memset(JSSVGPreserveAspectRatioConstructorFunctions, 0, sizeof(JSSVGPreserveAspectRatioConstructorFunctions));
    JSSVGPreserveAspectRatioConstructorFunctions[0].name = "SVG_PRESERVEASPECTRATIO_UNKNOWN";
    JSSVGPreserveAspectRatioConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[0].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_UNKNOWN;
    JSSVGPreserveAspectRatioConstructorFunctions[1].name = "SVG_PRESERVEASPECTRATIO_NONE";
    JSSVGPreserveAspectRatioConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[1].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_NONE;
    JSSVGPreserveAspectRatioConstructorFunctions[2].name = "SVG_PRESERVEASPECTRATIO_XMINYMIN";
    JSSVGPreserveAspectRatioConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[2].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMIN;
    JSSVGPreserveAspectRatioConstructorFunctions[3].name = "SVG_PRESERVEASPECTRATIO_XMIDYMIN";
    JSSVGPreserveAspectRatioConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[3].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMIN;
    JSSVGPreserveAspectRatioConstructorFunctions[4].name = "SVG_PRESERVEASPECTRATIO_XMAXYMIN";
    JSSVGPreserveAspectRatioConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[4].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMIN;
    JSSVGPreserveAspectRatioConstructorFunctions[5].name = "SVG_PRESERVEASPECTRATIO_XMINYMID";
    JSSVGPreserveAspectRatioConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[5].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMID;
    JSSVGPreserveAspectRatioConstructorFunctions[6].name = "SVG_PRESERVEASPECTRATIO_XMIDYMID";
    JSSVGPreserveAspectRatioConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[6].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMID;
    JSSVGPreserveAspectRatioConstructorFunctions[7].name = "SVG_PRESERVEASPECTRATIO_XMAXYMID";
    JSSVGPreserveAspectRatioConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[7].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMID;
    JSSVGPreserveAspectRatioConstructorFunctions[8].name = "SVG_PRESERVEASPECTRATIO_XMINYMAX";
    JSSVGPreserveAspectRatioConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[8].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMAX;
    JSSVGPreserveAspectRatioConstructorFunctions[9].name = "SVG_PRESERVEASPECTRATIO_XMIDYMAX";
    JSSVGPreserveAspectRatioConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[9].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMAX;
    JSSVGPreserveAspectRatioConstructorFunctions[10].name = "SVG_PRESERVEASPECTRATIO_XMAXYMAX";
    JSSVGPreserveAspectRatioConstructorFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[10].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[10].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMAX;
    JSSVGPreserveAspectRatioConstructorFunctions[11].name = "SVG_MEETORSLICE_UNKNOWN";
    JSSVGPreserveAspectRatioConstructorFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[11].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[11].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_MEETORSLICE_UNKNOWN;
    JSSVGPreserveAspectRatioConstructorFunctions[12].name = "SVG_MEETORSLICE_MEET";
    JSSVGPreserveAspectRatioConstructorFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[12].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[12].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_MEETORSLICE_MEET;
    JSSVGPreserveAspectRatioConstructorFunctions[13].name = "SVG_MEETORSLICE_SLICE";
    JSSVGPreserveAspectRatioConstructorFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioConstructorFunctions[13].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioConstructorFunctions[13].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_MEETORSLICE_SLICE;
}

JSValue JSSVGPreserveAspectRatioConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGPreserveAspectRatio.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPreserveAspectRatioConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGPreserveAspectRatio.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPreserveAspectRatioConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPreserveAspectRatioConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPreserveAspectRatioConstructorFunctions, countof(JSSVGPreserveAspectRatioConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGPreserveAspectRatioPrototypeConstantsFunctions[14];
static bool JSSVGPreserveAspectRatioPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGPreserveAspectRatioPrototypeConstantsFunctions()
{
    if (JSSVGPreserveAspectRatioPrototypeConstantsFunctions_initialized) return;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGPreserveAspectRatioPrototypeConstantsFunctions, 0, sizeof(JSSVGPreserveAspectRatioPrototypeConstantsFunctions));
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[0].name = "SVG_PRESERVEASPECTRATIO_UNKNOWN";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_UNKNOWN;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[1].name = "SVG_PRESERVEASPECTRATIO_NONE";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_NONE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[2].name = "SVG_PRESERVEASPECTRATIO_XMINYMIN";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMIN;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[3].name = "SVG_PRESERVEASPECTRATIO_XMIDYMIN";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMIN;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[4].name = "SVG_PRESERVEASPECTRATIO_XMAXYMIN";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[4].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMIN;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[5].name = "SVG_PRESERVEASPECTRATIO_XMINYMID";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[5].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMID;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[6].name = "SVG_PRESERVEASPECTRATIO_XMIDYMID";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[6].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMID;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[7].name = "SVG_PRESERVEASPECTRATIO_XMAXYMID";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[7].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMID;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[8].name = "SVG_PRESERVEASPECTRATIO_XMINYMAX";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[8].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMINYMAX;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[9].name = "SVG_PRESERVEASPECTRATIO_XMIDYMAX";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[9].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMIDYMAX;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[10].name = "SVG_PRESERVEASPECTRATIO_XMAXYMAX";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[10].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[10].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_XMAXYMAX;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[11].name = "SVG_MEETORSLICE_UNKNOWN";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[11].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[11].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_MEETORSLICE_UNKNOWN;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[12].name = "SVG_MEETORSLICE_MEET";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[12].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[12].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_MEETORSLICE_MEET;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[13].name = "SVG_MEETORSLICE_SLICE";
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[13].def_type = JS_DEF_PROP_INT32;
    JSSVGPreserveAspectRatioPrototypeConstantsFunctions[13].u.i32 = (int32_t)SVGPreserveAspectRatio::SVG_MEETORSLICE_SLICE;
}

JSValue JSSVGPreserveAspectRatioPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPreserveAspectRatio.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPreserveAspectRatioPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPreserveAspectRatio.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPreserveAspectRatioPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPreserveAspectRatioAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPreserveAspectRatioAttributesFunctions, countof(JSSVGPreserveAspectRatioAttributesFunctions));
    init_JSSVGPreserveAspectRatioPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPreserveAspectRatioPrototypeConstantsFunctions, countof(JSSVGPreserveAspectRatioPrototypeConstantsFunctions));
}

static JSClassDef JSSVGPreserveAspectRatioClassDefine;
static bool JSSVGPreserveAspectRatioClassDefine_initialized = false;

static void init_JSSVGPreserveAspectRatioClassDefine()
{
    if (JSSVGPreserveAspectRatioClassDefine_initialized) return;
    JSSVGPreserveAspectRatioClassDefine_initialized = true;
    memset(&JSSVGPreserveAspectRatioClassDefine, 0, sizeof(JSSVGPreserveAspectRatioClassDefine));
    JSSVGPreserveAspectRatioClassDefine.class_name = "SVGPreserveAspectRatio";
    JSSVGPreserveAspectRatioClassDefine.finalizer = JSSVGPreserveAspectRatio::finalizer;
    JSSVGPreserveAspectRatioClassDefine.gc_mark = JSSVGPreserveAspectRatio::mark;
}

JSClassID JSSVGPreserveAspectRatio::js_class_id = 0;

void JSSVGPreserveAspectRatio::init(JSContext* ctx)
{
    if (JSSVGPreserveAspectRatio::js_class_id == 0) {
        init_JSSVGPreserveAspectRatioClassDefine();
        JS_NewClassID(&JSSVGPreserveAspectRatio::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPreserveAspectRatio::js_class_id, &JSSVGPreserveAspectRatioClassDefine);
        JS_SetConstructor(ctx, JSSVGPreserveAspectRatioConstructor::self(ctx), JSSVGPreserveAspectRatioPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGPreserveAspectRatio::js_class_id, JSSVGPreserveAspectRatioPrototype::self(ctx));
    }
}

JSValue JSSVGPreserveAspectRatio::create(JSContext* ctx, SVGPreserveAspectRatio* impl, SVGElement* context)
{
    JSSVGPreserveAspectRatio::init(ctx);
    JSValue _proto = JSSVGPreserveAspectRatioPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPreserveAspectRatio::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGPreserveAspectRatio* imp = (SVGPreserveAspectRatio*)JS_GetOpaque(this_val, JSSVGPreserveAspectRatio::js_class_id);
            return JS_NewInt32(ctx, imp->align());
        }
        case MeetOrSliceAttrNum: {
            SVGPreserveAspectRatio* imp = (SVGPreserveAspectRatio*)JS_GetOpaque(this_val, JSSVGPreserveAspectRatio::js_class_id);
            return JS_NewInt32(ctx, imp->meetOrSlice());
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
            SVGPreserveAspectRatio* imp = (SVGPreserveAspectRatio*)JS_GetOpaque(this_val, JSSVGPreserveAspectRatio::js_class_id);
            imp->setAlign(valueToInt32(ctx, value));
            break;
        }
        case MeetOrSliceAttrNum: {
            SVGPreserveAspectRatio* imp = (SVGPreserveAspectRatio*)JS_GetOpaque(this_val, JSSVGPreserveAspectRatio::js_class_id);
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
