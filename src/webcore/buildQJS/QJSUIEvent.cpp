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

#include "QJSUIEvent.h"

#include "ExceptionCode.h"
#include "QJSDOMWindow.h"
#include "UIEvent.h"
#include "qjs_window.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSUIEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("layerX", JSUIEvent::getValueProperty, NULL, JSUIEvent::LayerXAttrNum),
    JS_CGETSET_MAGIC_DEF("view", JSUIEvent::getValueProperty, NULL, JSUIEvent::ViewAttrNum),
    JS_CGETSET_MAGIC_DEF("pageX", JSUIEvent::getValueProperty, NULL, JSUIEvent::PageXAttrNum),
    JS_CGETSET_MAGIC_DEF("which", JSUIEvent::getValueProperty, NULL, JSUIEvent::WhichAttrNum),
    JS_CGETSET_MAGIC_DEF("detail", JSUIEvent::getValueProperty, NULL, JSUIEvent::DetailAttrNum),
    JS_CGETSET_MAGIC_DEF("keyCode", JSUIEvent::getValueProperty, NULL, JSUIEvent::KeyCodeAttrNum),
    JS_CGETSET_MAGIC_DEF("charCode", JSUIEvent::getValueProperty, NULL, JSUIEvent::CharCodeAttrNum),
    JS_CGETSET_MAGIC_DEF("layerY", JSUIEvent::getValueProperty, NULL, JSUIEvent::LayerYAttrNum),
    JS_CGETSET_MAGIC_DEF("pageY", JSUIEvent::getValueProperty, NULL, JSUIEvent::PageYAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSUIEventPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("initUIEvent", 5, JSUIEventPrototypeFunction::callAsFunction, JSUIEvent::InitUIEventFuncNum)
};

JSValue JSUIEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSUIEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventPrototype::self(ctx));
        JSUIEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSUIEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSUIEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSUIEventAttributesFunctions, countof(JSUIEventAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSUIEventPrototypeFunctions, countof(JSUIEventPrototypeFunctions));
}

static JSClassDef JSUIEventClassDefine = 
{
    "UIEvent",
    .finalizer = JSUIEvent::finalizer,
    .gc_mark = JSUIEvent::mark,
};

JSClassID JSUIEvent::js_class_id = 0;

void JSUIEvent::init(JSContext* ctx)
{
    if (JSUIEvent::js_class_id == 0) {
        JS_NewClassID(&JSUIEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSUIEvent::js_class_id, &JSUIEventClassDefine);
        JS_SetClassProto(ctx, JSUIEvent::js_class_id, JSUIEventPrototype::self(ctx));
    }
}

JSValue JSUIEvent::create(JSContext* ctx, UIEvent* impl)
{
    JSUIEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSUIEventPrototype::self(ctx), JSUIEvent::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSUIEvent::finalizer(JSRuntime* rt, JSValue val)
{
    UIEvent* impl = (UIEvent*)JS_GetOpaque(val, JSUIEvent::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSUIEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSEvent::mark(rt, val, mark_func);
}

JSValue JSUIEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ViewAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->view()));
        }
        case DetailAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->detail());
        }
        case KeyCodeAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->keyCode());
        }
        case CharCodeAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->charCode());
        }
        case LayerXAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->layerX());
        }
        case LayerYAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->layerY());
        }
        case PageXAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->pageX());
        }
        case PageYAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->pageY());
        }
        case WhichAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->which());
        }
    }
    return JS_NULL;
}

JSValue JSUIEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    UIEvent* imp = (UIEvent*)JS_GetOpaque2(ctx, this_val, JSUIEvent::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSUIEvent::InitUIEventFuncNum: {
            AtomicString type = valueToString(ctx, argv[0]);
            bool canBubble = valueToBoolean(ctx, argv[1]);
            bool cancelable = valueToBoolean(ctx, argv[2]);
            DOMWindow* view = toDOMWindow(argv[3]);
            bool detailOk;
            int detail = valueToInt32(ctx, argv[4], detailOk);
            if (!detailOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->initUIEvent(type, canBubble, cancelable, view, detail);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
