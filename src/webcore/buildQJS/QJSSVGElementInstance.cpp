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

static const JSCFunctionListEntry JSSVGElementInstanceAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("childNodes", JSSVGElementInstance::getValueProperty, NULL, JSSVGElementInstance::ChildNodesAttrNum),
    JS_CGETSET_MAGIC_DEF("parentNode", JSSVGElementInstance::getValueProperty, NULL, JSSVGElementInstance::ParentNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("firstChild", JSSVGElementInstance::getValueProperty, NULL, JSSVGElementInstance::FirstChildAttrNum),
    JS_CGETSET_MAGIC_DEF("correspondingUseElement", JSSVGElementInstance::getValueProperty, NULL, JSSVGElementInstance::CorrespondingUseElementAttrNum),
    JS_CGETSET_MAGIC_DEF("lastChild", JSSVGElementInstance::getValueProperty, NULL, JSSVGElementInstance::LastChildAttrNum),
    JS_CGETSET_MAGIC_DEF("correspondingElement", JSSVGElementInstance::getValueProperty, NULL, JSSVGElementInstance::CorrespondingElementAttrNum),
    JS_CGETSET_MAGIC_DEF("previousSibling", JSSVGElementInstance::getValueProperty, NULL, JSSVGElementInstance::PreviousSiblingAttrNum),
    JS_CGETSET_MAGIC_DEF("nextSibling", JSSVGElementInstance::getValueProperty, NULL, JSSVGElementInstance::NextSiblingAttrNum)
};

JSValue JSSVGElementInstancePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGElementInstance.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGElementInstancePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGElementInstance.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGElementInstancePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGElementInstanceAttributesFunctions, countof(JSSVGElementInstanceAttributesFunctions));
}

static JSClassDef JSSVGElementInstanceClassDefine = 
{
    "SVGElementInstance",
    .finalizer = JSSVGElementInstance::finalizer,
    .gc_mark = JSSVGElementInstance::mark,
};

JSClassID JSSVGElementInstance::js_class_id = 0;

void JSSVGElementInstance::init(JSContext* ctx)
{
    if (JSSVGElementInstance::js_class_id == 0) {
        JS_NewClassID(&JSSVGElementInstance::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGElementInstance::js_class_id, &JSSVGElementInstanceClassDefine);
        JS_SetClassProto(ctx, JSSVGElementInstance::js_class_id, JSSVGElementInstancePrototype::self(ctx));
    }
}

JSValue JSSVGElementInstance::create(JSContext* ctx, SVGElementInstance* impl)
{
    JSSVGElementInstance::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGElementInstancePrototype::self(ctx), JSSVGElementInstance::js_class_id);
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
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque2(ctx, this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->correspondingElement()));
        }
        case CorrespondingUseElementAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque2(ctx, this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->correspondingUseElement()));
        }
        case ParentNodeAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque2(ctx, this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentNode()));
        }
        case ChildNodesAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque2(ctx, this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->childNodes()));
        }
        case FirstChildAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque2(ctx, this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->firstChild()));
        }
        case LastChildAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque2(ctx, this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->lastChild()));
        }
        case PreviousSiblingAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque2(ctx, this_val, JSSVGElementInstance::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->previousSibling()));
        }
        case NextSiblingAttrNum: {
            SVGElementInstance* imp = (SVGElementInstance*)JS_GetOpaque2(ctx, this_val, JSSVGElementInstance::js_class_id);
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
