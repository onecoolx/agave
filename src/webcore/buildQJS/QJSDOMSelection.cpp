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

static const JSCFunctionListEntry JSDOMSelectionAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("anchorOffset", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::AnchorOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("baseOffset", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::BaseOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("extentOffset", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::ExtentOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("focusNode", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::FocusNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("anchorNode", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::AnchorNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("focusOffset", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::FocusOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("baseNode", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::BaseNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("extentNode", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::ExtentNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("isCollapsed", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::IsCollapsedAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("rangeCount", JSDOMSelection::getValueProperty, NULL, JSDOMSelection::RangeCountAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSDOMSelectionPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("toString", 0, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::ToStringFuncNum),
    JS_CFUNC_MAGIC_DEF("modify", 3, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::ModifyFuncNum),
    JS_CFUNC_MAGIC_DEF("collapse", 2, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::CollapseFuncNum),
    JS_CFUNC_MAGIC_DEF("removeAllRanges", 0, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::RemoveAllRangesFuncNum),
    JS_CFUNC_MAGIC_DEF("addRange", 1, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::AddRangeFuncNum),
    JS_CFUNC_MAGIC_DEF("setBaseAndExtent", 4, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::SetBaseAndExtentFuncNum),
    JS_CFUNC_MAGIC_DEF("empty", 0, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::EmptyFuncNum),
    JS_CFUNC_MAGIC_DEF("setPosition", 2, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::SetPositionFuncNum),
    JS_CFUNC_MAGIC_DEF("collapseToEnd", 0, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::CollapseToEndFuncNum),
    JS_CFUNC_MAGIC_DEF("collapseToStart", 0, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::CollapseToStartFuncNum),
    JS_CFUNC_MAGIC_DEF("getRangeAt", 1, JSDOMSelectionPrototypeFunction::callAsFunction, JSDOMSelection::GetRangeAtFuncNum)
};

JSValue JSDOMSelectionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDOMSelection.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSDOMSelectionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDOMSelection.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDOMSelectionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMSelectionAttributesFunctions, countof(JSDOMSelectionAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMSelectionPrototypeFunctions, countof(JSDOMSelectionPrototypeFunctions));
}

static JSClassDef JSDOMSelectionClassDefine = 
{
    "DOMSelection",
    .finalizer = JSDOMSelection::finalizer,
    .gc_mark = JSDOMSelection::mark,
};

JSClassID JSDOMSelection::js_class_id = 0;

void JSDOMSelection::init(JSContext* ctx)
{
    if (JSDOMSelection::js_class_id == 0) {
        JS_NewClassID(&JSDOMSelection::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDOMSelection::js_class_id, &JSDOMSelectionClassDefine);
        JS_SetClassProto(ctx, JSDOMSelection::js_class_id, JSDOMSelectionPrototype::self(ctx));
    }
}

JSValue JSDOMSelection::create(JSContext* ctx, DOMSelection* impl)
{
    JSDOMSelection::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSDOMSelectionPrototype::self(ctx), JSDOMSelection::js_class_id);
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
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->anchorNode()));
        }
        case AnchorOffsetAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return JS_NewBigUint64(ctx, imp->anchorOffset());
        }
        case FocusNodeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->focusNode()));
        }
        case FocusOffsetAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return JS_NewBigUint64(ctx, imp->focusOffset());
        }
        case BaseNodeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->baseNode()));
        }
        case BaseOffsetAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return JS_NewBigUint64(ctx, imp->baseOffset());
        }
        case ExtentNodeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->extentNode()));
        }
        case ExtentOffsetAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return JS_NewBigUint64(ctx, imp->extentOffset());
        }
        case IsCollapsedAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return JS_NewBool(ctx, imp->isCollapsed() ? 1 : 0);
        }
        case TypeAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case RangeCountAttrNum: {
            DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
            return JS_NewBigUint64(ctx, imp->rangeCount());
        }
    }
    return JS_NULL;
}

JSValue JSDOMSelectionPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    DOMSelection* imp = (DOMSelection*)JS_GetOpaque2(ctx, this_val, JSDOMSelection::js_class_id);
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
