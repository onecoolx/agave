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
#include "QJSSVGElementInstance.h"

#include "QJSSVGElement.h"
#include "QJSSVGElementInstance.h"
#include "QJSSVGElementInstanceList.h"
#include "QJSSVGUseElement.h"
#include "SVGElement.h"
#include "SVGElementInstance.h"
#include "SVGElementInstanceList.h"
#include "SVGUseElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGElementInstanceAttributesFunctions[8];
static bool JSSVGElementInstanceAttributesFunctions_initialized = false;

static void init_JSSVGElementInstanceAttributesFunctions()
{
    if (JSSVGElementInstanceAttributesFunctions_initialized) return;
    JSSVGElementInstanceAttributesFunctions_initialized = true;
    memset(JSSVGElementInstanceAttributesFunctions, 0, sizeof(JSSVGElementInstanceAttributesFunctions));
    JSSVGElementInstanceAttributesFunctions[0].name = "correspondingElement";
    JSSVGElementInstanceAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementInstanceAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementInstanceAttributesFunctions[0].magic = JSSVGElementInstance::CorrespondingElementAttrNum;
    JSSVGElementInstanceAttributesFunctions[0].u.getset.get.getter_magic = JSSVGElementInstance::getValueProperty;
    JSSVGElementInstanceAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGElementInstanceAttributesFunctions[1].name = "correspondingUseElement";
    JSSVGElementInstanceAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementInstanceAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementInstanceAttributesFunctions[1].magic = JSSVGElementInstance::CorrespondingUseElementAttrNum;
    JSSVGElementInstanceAttributesFunctions[1].u.getset.get.getter_magic = JSSVGElementInstance::getValueProperty;
    JSSVGElementInstanceAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGElementInstanceAttributesFunctions[2].name = "parentNode";
    JSSVGElementInstanceAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementInstanceAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementInstanceAttributesFunctions[2].magic = JSSVGElementInstance::ParentNodeAttrNum;
    JSSVGElementInstanceAttributesFunctions[2].u.getset.get.getter_magic = JSSVGElementInstance::getValueProperty;
    JSSVGElementInstanceAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGElementInstanceAttributesFunctions[3].name = "childNodes";
    JSSVGElementInstanceAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementInstanceAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementInstanceAttributesFunctions[3].magic = JSSVGElementInstance::ChildNodesAttrNum;
    JSSVGElementInstanceAttributesFunctions[3].u.getset.get.getter_magic = JSSVGElementInstance::getValueProperty;
    JSSVGElementInstanceAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGElementInstanceAttributesFunctions[4].name = "firstChild";
    JSSVGElementInstanceAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementInstanceAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementInstanceAttributesFunctions[4].magic = JSSVGElementInstance::FirstChildAttrNum;
    JSSVGElementInstanceAttributesFunctions[4].u.getset.get.getter_magic = JSSVGElementInstance::getValueProperty;
    JSSVGElementInstanceAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGElementInstanceAttributesFunctions[5].name = "lastChild";
    JSSVGElementInstanceAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementInstanceAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementInstanceAttributesFunctions[5].magic = JSSVGElementInstance::LastChildAttrNum;
    JSSVGElementInstanceAttributesFunctions[5].u.getset.get.getter_magic = JSSVGElementInstance::getValueProperty;
    JSSVGElementInstanceAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGElementInstanceAttributesFunctions[6].name = "previousSibling";
    JSSVGElementInstanceAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementInstanceAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementInstanceAttributesFunctions[6].magic = JSSVGElementInstance::PreviousSiblingAttrNum;
    JSSVGElementInstanceAttributesFunctions[6].u.getset.get.getter_magic = JSSVGElementInstance::getValueProperty;
    JSSVGElementInstanceAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGElementInstanceAttributesFunctions[7].name = "nextSibling";
    JSSVGElementInstanceAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGElementInstanceAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGElementInstanceAttributesFunctions[7].magic = JSSVGElementInstance::NextSiblingAttrNum;
    JSSVGElementInstanceAttributesFunctions[7].u.getset.get.getter_magic = JSSVGElementInstance::getValueProperty;
    JSSVGElementInstanceAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGElementInstancePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGElementInstance.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGElementInstancePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGElementInstance.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGElementInstancePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGElementInstanceAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGElementInstanceAttributesFunctions, countof(JSSVGElementInstanceAttributesFunctions));
}

static JSClassDef JSSVGElementInstanceClassDefine;
static bool JSSVGElementInstanceClassDefine_initialized = false;

static void init_JSSVGElementInstanceClassDefine()
{
    if (JSSVGElementInstanceClassDefine_initialized) return;
    JSSVGElementInstanceClassDefine_initialized = true;
    memset(&JSSVGElementInstanceClassDefine, 0, sizeof(JSSVGElementInstanceClassDefine));
    JSSVGElementInstanceClassDefine.class_name = "SVGElementInstance";
    JSSVGElementInstanceClassDefine.finalizer = JSSVGElementInstance::finalizer;
    JSSVGElementInstanceClassDefine.gc_mark = JSSVGElementInstance::mark;
}

JSClassID JSSVGElementInstance::js_class_id = 0;

void JSSVGElementInstance::init(JSContext* ctx)
{
    if (JSSVGElementInstance::js_class_id == 0) {
        init_JSSVGElementInstanceClassDefine();
        JS_NewClassID(&JSSVGElementInstance::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGElementInstance::js_class_id, &JSSVGElementInstanceClassDefine);
        JS_SetClassProto(ctx, JSSVGElementInstance::js_class_id, JSSVGElementInstancePrototype::self(ctx));
    }
}

JSValue JSSVGElementInstance::create(JSContext* ctx, SVGElementInstance* impl)
{
    JSSVGElementInstance::init(ctx);
    JSValue _proto = JSSVGElementInstancePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGElementInstance::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGElementInstance::finalizer(JSRuntime* rt, JSValue val)
{
    SVGElementInstance* impl = (SVGElementInstance*)JS_GetOpaque(val, JSSVGElementInstance::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGElementInstance::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGElementInstance::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CorrespondingElementAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque(this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->correspondingElement()));
        }
        case CorrespondingUseElementAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque(this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->correspondingUseElement()));
        }
        case ParentNodeAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque(this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentNode()));
        }
        case ChildNodesAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque(this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->childNodes()));
        }
        case FirstChildAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque(this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->firstChild()));
        }
        case LastChildAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque(this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->lastChild()));
        }
        case PreviousSiblingAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque(this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->previousSibling()));
        }
        case NextSiblingAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque(this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nextSibling()));
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGElementInstance* obj)
{
    return QJS::cacheDOMObject<SVGElementInstance, JSSVGElementInstance>(ctx, obj);
}

SVGElementInstance* toSVGElementInstance(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGElementInstance* impl = (SVGElementInstance*)JS_GetOpaque(val, JSSVGElementInstance::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
