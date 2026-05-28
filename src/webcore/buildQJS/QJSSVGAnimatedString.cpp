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
#include "QJSSVGAnimatedString.h"

#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGAnimatedStringAttributesFunctions[2];
static bool JSSVGAnimatedStringAttributesFunctions_initialized = false;

static void init_JSSVGAnimatedStringAttributesFunctions()
{
    if (JSSVGAnimatedStringAttributesFunctions_initialized) return;
    JSSVGAnimatedStringAttributesFunctions_initialized = true;
    memset(JSSVGAnimatedStringAttributesFunctions, 0, sizeof(JSSVGAnimatedStringAttributesFunctions));
    JSSVGAnimatedStringAttributesFunctions[0].name = "baseVal";
    JSSVGAnimatedStringAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedStringAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedStringAttributesFunctions[0].magic = JSSVGAnimatedString::BaseValAttrNum;
    JSSVGAnimatedStringAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAnimatedString::getValueProperty;
    JSSVGAnimatedStringAttributesFunctions[0].u.getset.set.setter_magic = JSSVGAnimatedString::putValueProperty;
    JSSVGAnimatedStringAttributesFunctions[1].name = "animVal";
    JSSVGAnimatedStringAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAnimatedStringAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAnimatedStringAttributesFunctions[1].magic = JSSVGAnimatedString::AnimValAttrNum;
    JSSVGAnimatedStringAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAnimatedString::getValueProperty;
    JSSVGAnimatedStringAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGAnimatedStringPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedString.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedStringPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedString.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAnimatedStringPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAnimatedStringAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedStringAttributesFunctions, countof(JSSVGAnimatedStringAttributesFunctions));
}

static JSClassDef JSSVGAnimatedStringClassDefine;
static bool JSSVGAnimatedStringClassDefine_initialized = false;

static void init_JSSVGAnimatedStringClassDefine()
{
    if (JSSVGAnimatedStringClassDefine_initialized) return;
    JSSVGAnimatedStringClassDefine_initialized = true;
    memset(&JSSVGAnimatedStringClassDefine, 0, sizeof(JSSVGAnimatedStringClassDefine));
    JSSVGAnimatedStringClassDefine.class_name = "SVGAnimatedString";
    JSSVGAnimatedStringClassDefine.finalizer = JSSVGAnimatedString::finalizer;
    JSSVGAnimatedStringClassDefine.gc_mark = JSSVGAnimatedString::mark;
}

JSClassID JSSVGAnimatedString::js_class_id = 0;

void JSSVGAnimatedString::init(JSContext* ctx)
{
    if (JSSVGAnimatedString::js_class_id == 0) {
        init_JSSVGAnimatedStringClassDefine();
        JS_NewClassID(&JSSVGAnimatedString::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedString::js_class_id, &JSSVGAnimatedStringClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedString::js_class_id, JSSVGAnimatedStringPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedString::create(JSContext* ctx, SVGAnimatedString* impl, SVGElement* context)
{
    JSSVGAnimatedString::init(ctx);
    JSValue _proto = JSSVGAnimatedStringPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAnimatedString::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedString::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedString* impl = (SVGAnimatedString*)JS_GetOpaque(val, JSSVGAnimatedString::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedString::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedString::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedString* imp = (SVGAnimatedString*)JS_GetOpaque(this_val, JSSVGAnimatedString::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->baseVal()).utf8().data());
        }
        case AnimValAttrNum: {
            SVGAnimatedString* imp = (SVGAnimatedString*)JS_GetOpaque(this_val, JSSVGAnimatedString::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->animVal()).utf8().data());
        }
    }
    return JS_NULL;
}

JSValue JSSVGAnimatedString::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedString* imp = (SVGAnimatedString*)JS_GetOpaque(this_val, JSSVGAnimatedString::js_class_id);
            imp->setBaseVal(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue toJS(JSContext *ctx, SVGAnimatedString* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedString, JSSVGAnimatedString>(ctx, obj, context);
}

SVGAnimatedString* toSVGAnimatedString(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedString* impl = (SVGAnimatedString*)JS_GetOpaque(val, JSSVGAnimatedString::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
