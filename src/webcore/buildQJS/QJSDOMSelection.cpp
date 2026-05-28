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

#include "QJSDOMSelection.h"

#include "DOMSelection.h"
#include "ExceptionCode.h"
#include "Node.h"
#include "QJSNode.h"
#include "QJSRange.h"
#include "Range.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSDOMSelectionAttributesFunctions[11];
static bool JSDOMSelectionAttributesFunctions_initialized = false;

static void init_JSDOMSelectionAttributesFunctions()
{
    if (JSDOMSelectionAttributesFunctions_initialized) return;
    JSDOMSelectionAttributesFunctions_initialized = true;
    memset(JSDOMSelectionAttributesFunctions, 0, sizeof(JSDOMSelectionAttributesFunctions));
    JSDOMSelectionAttributesFunctions[0].name = "anchorNode";
    JSDOMSelectionAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[0].magic = JSDOMSelection::AnchorNodeAttrNum;
    JSDOMSelectionAttributesFunctions[0].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[1].name = "anchorOffset";
    JSDOMSelectionAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[1].magic = JSDOMSelection::AnchorOffsetAttrNum;
    JSDOMSelectionAttributesFunctions[1].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[2].name = "focusNode";
    JSDOMSelectionAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[2].magic = JSDOMSelection::FocusNodeAttrNum;
    JSDOMSelectionAttributesFunctions[2].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[3].name = "focusOffset";
    JSDOMSelectionAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[3].magic = JSDOMSelection::FocusOffsetAttrNum;
    JSDOMSelectionAttributesFunctions[3].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[4].name = "baseNode";
    JSDOMSelectionAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[4].magic = JSDOMSelection::BaseNodeAttrNum;
    JSDOMSelectionAttributesFunctions[4].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[5].name = "baseOffset";
    JSDOMSelectionAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[5].magic = JSDOMSelection::BaseOffsetAttrNum;
    JSDOMSelectionAttributesFunctions[5].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[6].name = "extentNode";
    JSDOMSelectionAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[6].magic = JSDOMSelection::ExtentNodeAttrNum;
    JSDOMSelectionAttributesFunctions[6].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[7].name = "extentOffset";
    JSDOMSelectionAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[7].magic = JSDOMSelection::ExtentOffsetAttrNum;
    JSDOMSelectionAttributesFunctions[7].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[8].name = "isCollapsed";
    JSDOMSelectionAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[8].magic = JSDOMSelection::IsCollapsedAttrNum;
    JSDOMSelectionAttributesFunctions[8].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[9].name = "type";
    JSDOMSelectionAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[9].magic = JSDOMSelection::TypeAttrNum;
    JSDOMSelectionAttributesFunctions[9].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSDOMSelectionAttributesFunctions[10].name = "rangeCount";
    JSDOMSelectionAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMSelectionAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMSelectionAttributesFunctions[10].magic = JSDOMSelection::RangeCountAttrNum;
    JSDOMSelectionAttributesFunctions[10].u.getset.get.getter_magic = JSDOMSelection::getValueProperty;
    JSDOMSelectionAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSDOMSelectionPrototypeFunctions[11];
static bool JSDOMSelectionPrototypeFunctions_initialized = false;

static void init_JSDOMSelectionPrototypeFunctions()
{
    if (JSDOMSelectionPrototypeFunctions_initialized) return;
    JSDOMSelectionPrototypeFunctions_initialized = true;
    memset(JSDOMSelectionPrototypeFunctions, 0, sizeof(JSDOMSelectionPrototypeFunctions));
    JSDOMSelectionPrototypeFunctions[0].name = "collapse";
    JSDOMSelectionPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[0].magic = JSDOMSelection::CollapseFuncNum;
    JSDOMSelectionPrototypeFunctions[0].u.func.length = 2;
    JSDOMSelectionPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[0].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[1].name = "collapseToEnd";
    JSDOMSelectionPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[1].magic = JSDOMSelection::CollapseToEndFuncNum;
    JSDOMSelectionPrototypeFunctions[1].u.func.length = 0;
    JSDOMSelectionPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[1].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[2].name = "collapseToStart";
    JSDOMSelectionPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[2].magic = JSDOMSelection::CollapseToStartFuncNum;
    JSDOMSelectionPrototypeFunctions[2].u.func.length = 0;
    JSDOMSelectionPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[2].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[3].name = "empty";
    JSDOMSelectionPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[3].magic = JSDOMSelection::EmptyFuncNum;
    JSDOMSelectionPrototypeFunctions[3].u.func.length = 0;
    JSDOMSelectionPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[3].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[4].name = "setBaseAndExtent";
    JSDOMSelectionPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[4].magic = JSDOMSelection::SetBaseAndExtentFuncNum;
    JSDOMSelectionPrototypeFunctions[4].u.func.length = 4;
    JSDOMSelectionPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[4].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[5].name = "setPosition";
    JSDOMSelectionPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[5].magic = JSDOMSelection::SetPositionFuncNum;
    JSDOMSelectionPrototypeFunctions[5].u.func.length = 2;
    JSDOMSelectionPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[5].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[6].name = "modify";
    JSDOMSelectionPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[6].magic = JSDOMSelection::ModifyFuncNum;
    JSDOMSelectionPrototypeFunctions[6].u.func.length = 3;
    JSDOMSelectionPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[6].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[7].name = "getRangeAt";
    JSDOMSelectionPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[7].magic = JSDOMSelection::GetRangeAtFuncNum;
    JSDOMSelectionPrototypeFunctions[7].u.func.length = 1;
    JSDOMSelectionPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[7].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[8].name = "removeAllRanges";
    JSDOMSelectionPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[8].magic = JSDOMSelection::RemoveAllRangesFuncNum;
    JSDOMSelectionPrototypeFunctions[8].u.func.length = 0;
    JSDOMSelectionPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[8].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[9].name = "addRange";
    JSDOMSelectionPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[9].magic = JSDOMSelection::AddRangeFuncNum;
    JSDOMSelectionPrototypeFunctions[9].u.func.length = 1;
    JSDOMSelectionPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[9].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
    JSDOMSelectionPrototypeFunctions[10].name = "toString";
    JSDOMSelectionPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMSelectionPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSDOMSelectionPrototypeFunctions[10].magic = JSDOMSelection::ToStringFuncNum;
    JSDOMSelectionPrototypeFunctions[10].u.func.length = 0;
    JSDOMSelectionPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMSelectionPrototypeFunctions[10].u.func.cfunc.generic_magic = JSDOMSelectionPrototypeFunction::callAsFunction;
}

JSValue JSDOMSelectionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDOMSelection.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSDOMSelectionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDOMSelection.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDOMSelectionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSDOMSelectionAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMSelectionAttributesFunctions, countof(JSDOMSelectionAttributesFunctions));
    init_JSDOMSelectionPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMSelectionPrototypeFunctions, countof(JSDOMSelectionPrototypeFunctions));
}

static JSClassDef JSDOMSelectionClassDefine;
static bool JSDOMSelectionClassDefine_initialized = false;

static void init_JSDOMSelectionClassDefine()
{
    if (JSDOMSelectionClassDefine_initialized) return;
    JSDOMSelectionClassDefine_initialized = true;
    memset(&JSDOMSelectionClassDefine, 0, sizeof(JSDOMSelectionClassDefine));
    JSDOMSelectionClassDefine.class_name = "DOMSelection";
    JSDOMSelectionClassDefine.finalizer = JSDOMSelection::finalizer;
    JSDOMSelectionClassDefine.gc_mark = JSDOMSelection::mark;
}

JSClassID JSDOMSelection::js_class_id = 0;

void JSDOMSelection::init(JSContext* ctx)
{
    if (JSDOMSelection::js_class_id == 0) {
        init_JSDOMSelectionClassDefine();
        JS_NewClassID(&JSDOMSelection::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDOMSelection::js_class_id, &JSDOMSelectionClassDefine);
        JS_SetClassProto(ctx, JSDOMSelection::js_class_id, JSDOMSelectionPrototype::self(ctx));
    }
}

JSValue JSDOMSelection::create(JSContext* ctx, DOMSelection* impl)
{
    JSDOMSelection::init(ctx);
    JSValue _proto = JSDOMSelectionPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSDOMSelection::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDOMSelection::finalizer(JSRuntime* rt, JSValue val)
{
    DOMSelection* impl = (DOMSelection*)JS_GetOpaque(val, JSDOMSelection::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSDOMSelection::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSDOMSelection::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AnchorNodeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->anchorNode()));
        }
        case AnchorOffsetAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return JS_NewInt32(ctx, imp->anchorOffset());
        }
        case FocusNodeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->focusNode()));
        }
        case FocusOffsetAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return JS_NewInt32(ctx, imp->focusOffset());
        }
        case BaseNodeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->baseNode()));
        }
        case BaseOffsetAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return JS_NewInt32(ctx, imp->baseOffset());
        }
        case ExtentNodeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->extentNode()));
        }
        case ExtentOffsetAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return JS_NewInt32(ctx, imp->extentOffset());
        }
        case IsCollapsedAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return JS_NewBool(ctx, imp->isCollapsed() ? 1 : 0);
        }
        case TypeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case RangeCountAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
            return JS_NewInt32(ctx, imp->rangeCount());
        }
    }
    return JS_NULL;
}

JSValue JSDOMSelectionPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    DOMSelection* imp = (DOMSelection*)JS_GetOpaque(this_val, JSDOMSelection::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSDOMSelection::CollapseFuncNum: {
            ExceptionCode ec = 0;
            Node* node = toNode(argv[0]);
            bool indexOk;
            int index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->collapse(node, index, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSDOMSelection::CollapseToEndFuncNum: {
            imp->collapseToEnd();
            return JS_UNDEFINED;
        }
        case JSDOMSelection::CollapseToStartFuncNum: {
            imp->collapseToStart();
            return JS_UNDEFINED;
        }
        case JSDOMSelection::EmptyFuncNum: {
            imp->empty();
            return JS_UNDEFINED;
        }
        case JSDOMSelection::SetBaseAndExtentFuncNum: {
            ExceptionCode ec = 0;
            Node* baseNode = toNode(argv[0]);
            bool baseOffsetOk;
            int baseOffset = valueToInt32(ctx, argv[1], baseOffsetOk);
            if (!baseOffsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            Node* extentNode = toNode(argv[2]);
            bool extentOffsetOk;
            int extentOffset = valueToInt32(ctx, argv[3], extentOffsetOk);
            if (!extentOffsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setBaseAndExtent(baseNode, baseOffset, extentNode, extentOffset, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSDOMSelection::SetPositionFuncNum: {
            ExceptionCode ec = 0;
            Node* node = toNode(argv[0]);
            if (argc < 2) {
                  imp->setPosition(node, ec);
                  setDOMException(ctx, ec);
                  return JS_UNDEFINED;
            }

            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setPosition(node, offset, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSDOMSelection::ModifyFuncNum: {
            String alter = valueToString(ctx, argv[0]);
            String direction = valueToString(ctx, argv[1]);
            String granularity = valueToString(ctx, argv[2]);
            imp->modify(alter, direction, granularity);
            return JS_UNDEFINED;
        }
        case JSDOMSelection::GetRangeAtFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            int index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->getRangeAt(index, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDOMSelection::RemoveAllRangesFuncNum: {
            imp->removeAllRanges();
            return JS_UNDEFINED;
        }
        case JSDOMSelection::AddRangeFuncNum: {
            Range* range = toRange(argv[0]);
            imp->addRange(range);
            return JS_UNDEFINED;
        }
        case JSDOMSelection::ToStringFuncNum: {

            JSValue result = JS_NewString(ctx, ((const String&)imp->toString()).utf8().data());
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, DOMSelection* obj)
{
    return QJS::cacheDOMObject<DOMSelection, JSDOMSelection>(ctx, obj);
}

DOMSelection* toDOMSelection(JSValue val)
{
    if (JS_IsObject(val)) {
        DOMSelection* impl = (DOMSelection*)JS_GetOpaque(val, JSDOMSelection::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
