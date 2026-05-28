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

#include "QJSUIEvent.h"

#include "ExceptionCode.h"
#include "QJSDOMWindow.h"
#include "UIEvent.h"
#include "qjs_window.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSUIEventAttributesFunctions[9];
static bool JSUIEventAttributesFunctions_initialized = false;

static void init_JSUIEventAttributesFunctions()
{
    if (JSUIEventAttributesFunctions_initialized) return;
    JSUIEventAttributesFunctions_initialized = true;
    memset(JSUIEventAttributesFunctions, 0, sizeof(JSUIEventAttributesFunctions));
    JSUIEventAttributesFunctions[0].name = "layerX";
    JSUIEventAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[0].magic = JSUIEvent::LayerXAttrNum;
    JSUIEventAttributesFunctions[0].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSUIEventAttributesFunctions[1].name = "view";
    JSUIEventAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[1].magic = JSUIEvent::ViewAttrNum;
    JSUIEventAttributesFunctions[1].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSUIEventAttributesFunctions[2].name = "pageX";
    JSUIEventAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[2].magic = JSUIEvent::PageXAttrNum;
    JSUIEventAttributesFunctions[2].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSUIEventAttributesFunctions[3].name = "which";
    JSUIEventAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[3].magic = JSUIEvent::WhichAttrNum;
    JSUIEventAttributesFunctions[3].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSUIEventAttributesFunctions[4].name = "detail";
    JSUIEventAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[4].magic = JSUIEvent::DetailAttrNum;
    JSUIEventAttributesFunctions[4].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSUIEventAttributesFunctions[5].name = "keyCode";
    JSUIEventAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[5].magic = JSUIEvent::KeyCodeAttrNum;
    JSUIEventAttributesFunctions[5].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSUIEventAttributesFunctions[6].name = "charCode";
    JSUIEventAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[6].magic = JSUIEvent::CharCodeAttrNum;
    JSUIEventAttributesFunctions[6].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSUIEventAttributesFunctions[7].name = "layerY";
    JSUIEventAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[7].magic = JSUIEvent::LayerYAttrNum;
    JSUIEventAttributesFunctions[7].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSUIEventAttributesFunctions[8].name = "pageY";
    JSUIEventAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSUIEventAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSUIEventAttributesFunctions[8].magic = JSUIEvent::PageYAttrNum;
    JSUIEventAttributesFunctions[8].u.getset.get.getter_magic = JSUIEvent::getValueProperty;
    JSUIEventAttributesFunctions[8].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSUIEventPrototypeFunctions[1];
static bool JSUIEventPrototypeFunctions_initialized = false;

static void init_JSUIEventPrototypeFunctions()
{
    if (JSUIEventPrototypeFunctions_initialized) return;
    JSUIEventPrototypeFunctions_initialized = true;
    memset(JSUIEventPrototypeFunctions, 0, sizeof(JSUIEventPrototypeFunctions));
    JSUIEventPrototypeFunctions[0].name = "initUIEvent";
    JSUIEventPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSUIEventPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSUIEventPrototypeFunctions[0].magic = JSUIEvent::InitUIEventFuncNum;
    JSUIEventPrototypeFunctions[0].u.func.length = 5;
    JSUIEventPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSUIEventPrototypeFunctions[0].u.func.cfunc.generic_magic = JSUIEventPrototypeFunction::callAsFunction;
}

JSValue JSUIEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSUIEvent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventPrototype::self(ctx));
        JSUIEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSUIEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSUIEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSUIEventAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSUIEventAttributesFunctions, countof(JSUIEventAttributesFunctions));
    init_JSUIEventPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSUIEventPrototypeFunctions, countof(JSUIEventPrototypeFunctions));
}

static JSClassDef JSUIEventClassDefine;
static bool JSUIEventClassDefine_initialized = false;

static void init_JSUIEventClassDefine()
{
    if (JSUIEventClassDefine_initialized) return;
    JSUIEventClassDefine_initialized = true;
    memset(&JSUIEventClassDefine, 0, sizeof(JSUIEventClassDefine));
    JSUIEventClassDefine.class_name = "UIEvent";
    JSUIEventClassDefine.finalizer = JSUIEvent::finalizer;
    JSUIEventClassDefine.gc_mark = JSUIEvent::mark;
}

JSClassID JSUIEvent::js_class_id = 0;

void JSUIEvent::init(JSContext* ctx)
{
    if (JSUIEvent::js_class_id == 0) {
        init_JSUIEventClassDefine();
        JS_NewClassID(&JSUIEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSUIEvent::js_class_id, &JSUIEventClassDefine);
        JS_SetClassProto(ctx, JSUIEvent::js_class_id, JSUIEventPrototype::self(ctx));
    }
}

JSValue JSUIEvent::create(JSContext* ctx, UIEvent* impl)
{
    JSUIEvent::init(ctx);
    JSValue _proto = JSUIEventPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSUIEvent::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->view()));
        }
        case DetailAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return JS_NewInt32(ctx, imp->detail());
        }
        case KeyCodeAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return JS_NewInt32(ctx, imp->keyCode());
        }
        case CharCodeAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return JS_NewInt32(ctx, imp->charCode());
        }
        case LayerXAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return JS_NewInt32(ctx, imp->layerX());
        }
        case LayerYAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return JS_NewInt32(ctx, imp->layerY());
        }
        case PageXAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return JS_NewInt32(ctx, imp->pageX());
        }
        case PageYAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return JS_NewInt32(ctx, imp->pageY());
        }
        case WhichAttrNum: {
            UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
            return JS_NewInt32(ctx, imp->which());
        }
    }
    return JS_NULL;
}

JSValue JSUIEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    UIEvent* imp = (UIEvent*)JS_GetOpaque(this_val, JSUIEvent::js_class_id);
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
