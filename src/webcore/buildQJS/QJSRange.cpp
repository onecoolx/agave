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

#include "QJSRange.h"

#include "DocumentFragment.h"
#include "ExceptionCode.h"
#include "Node.h"
#include "QJSDocumentFragment.h"
#include "QJSNode.h"
#include "QJSRange.h"
#include "Range.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSRangeAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("endOffset", JSRange::getValueProperty, NULL, JSRange::EndOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("startOffset", JSRange::getValueProperty, NULL, JSRange::StartOffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("endContainer", JSRange::getValueProperty, NULL, JSRange::EndContainerAttrNum),
    JS_CGETSET_MAGIC_DEF("startContainer", JSRange::getValueProperty, NULL, JSRange::StartContainerAttrNum),
    JS_CGETSET_MAGIC_DEF("collapsed", JSRange::getValueProperty, NULL, JSRange::CollapsedAttrNum),
    JS_CGETSET_MAGIC_DEF("commonAncestorContainer", JSRange::getValueProperty, NULL, JSRange::CommonAncestorContainerAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSRange::getValueProperty, NULL, JSRange::ConstructorAttrNum)
};

class JSRangeConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSRangeConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSRangeConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("START_TO_START", JSRangeConstructor::getValueProperty, NULL, Range::START_TO_START),
    JS_CGETSET_MAGIC_DEF("NODE_AFTER", JSRangeConstructor::getValueProperty, NULL, Range::NODE_AFTER),
    JS_CGETSET_MAGIC_DEF("NODE_BEFORE", JSRangeConstructor::getValueProperty, NULL, Range::NODE_BEFORE),
    JS_CGETSET_MAGIC_DEF("START_TO_END", JSRangeConstructor::getValueProperty, NULL, Range::START_TO_END),
    JS_CGETSET_MAGIC_DEF("END_TO_END", JSRangeConstructor::getValueProperty, NULL, Range::END_TO_END),
    JS_CGETSET_MAGIC_DEF("END_TO_START", JSRangeConstructor::getValueProperty, NULL, Range::END_TO_START),
    JS_CGETSET_MAGIC_DEF("NODE_BEFORE_AND_AFTER", JSRangeConstructor::getValueProperty, NULL, Range::NODE_BEFORE_AND_AFTER),
    JS_CGETSET_MAGIC_DEF("NODE_INSIDE", JSRangeConstructor::getValueProperty, NULL, Range::NODE_INSIDE)
};

JSValue JSRangeConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Range.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSRangeConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Range.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSRangeConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangeConstructorFunctions, countof(JSRangeConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSRangePrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("START_TO_START", JSRangePrototype::getValueProperty, NULL, Range::START_TO_START),
    JS_CGETSET_MAGIC_DEF("NODE_AFTER", JSRangePrototype::getValueProperty, NULL, Range::NODE_AFTER),
    JS_CGETSET_MAGIC_DEF("NODE_BEFORE", JSRangePrototype::getValueProperty, NULL, Range::NODE_BEFORE),
    JS_CGETSET_MAGIC_DEF("START_TO_END", JSRangePrototype::getValueProperty, NULL, Range::START_TO_END),
    JS_CGETSET_MAGIC_DEF("END_TO_END", JSRangePrototype::getValueProperty, NULL, Range::END_TO_END),
    JS_CGETSET_MAGIC_DEF("END_TO_START", JSRangePrototype::getValueProperty, NULL, Range::END_TO_START),
    JS_CGETSET_MAGIC_DEF("NODE_BEFORE_AND_AFTER", JSRangePrototype::getValueProperty, NULL, Range::NODE_BEFORE_AND_AFTER),
    JS_CGETSET_MAGIC_DEF("NODE_INSIDE", JSRangePrototype::getValueProperty, NULL, Range::NODE_INSIDE)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSRangePrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("toString", 0, JSRangePrototypeFunction::callAsFunction, JSRange::ToStringFuncNum),
    JS_CFUNC_MAGIC_DEF("setEnd", 2, JSRangePrototypeFunction::callAsFunction, JSRange::SetEndFuncNum),
    JS_CFUNC_MAGIC_DEF("setEndBefore", 1, JSRangePrototypeFunction::callAsFunction, JSRange::SetEndBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("setStart", 2, JSRangePrototypeFunction::callAsFunction, JSRange::SetStartFuncNum),
    JS_CFUNC_MAGIC_DEF("intersectsNode", 1, JSRangePrototypeFunction::callAsFunction, JSRange::IntersectsNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("surroundContents", 1, JSRangePrototypeFunction::callAsFunction, JSRange::SurroundContentsFuncNum),
    JS_CFUNC_MAGIC_DEF("selectNode", 1, JSRangePrototypeFunction::callAsFunction, JSRange::SelectNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("setEndAfter", 1, JSRangePrototypeFunction::callAsFunction, JSRange::SetEndAfterFuncNum),
    JS_CFUNC_MAGIC_DEF("collapse", 1, JSRangePrototypeFunction::callAsFunction, JSRange::CollapseFuncNum),
    JS_CFUNC_MAGIC_DEF("setStartBefore", 1, JSRangePrototypeFunction::callAsFunction, JSRange::SetStartBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("insertNode", 1, JSRangePrototypeFunction::callAsFunction, JSRange::InsertNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("setStartAfter", 1, JSRangePrototypeFunction::callAsFunction, JSRange::SetStartAfterFuncNum),
    JS_CFUNC_MAGIC_DEF("compareBoundaryPoints", 2, JSRangePrototypeFunction::callAsFunction, JSRange::CompareBoundaryPointsFuncNum),
    JS_CFUNC_MAGIC_DEF("selectNodeContents", 1, JSRangePrototypeFunction::callAsFunction, JSRange::SelectNodeContentsFuncNum),
    JS_CFUNC_MAGIC_DEF("deleteContents", 0, JSRangePrototypeFunction::callAsFunction, JSRange::DeleteContentsFuncNum),
    JS_CFUNC_MAGIC_DEF("extractContents", 0, JSRangePrototypeFunction::callAsFunction, JSRange::ExtractContentsFuncNum),
    JS_CFUNC_MAGIC_DEF("cloneContents", 0, JSRangePrototypeFunction::callAsFunction, JSRange::CloneContentsFuncNum),
    JS_CFUNC_MAGIC_DEF("cloneRange", 0, JSRangePrototypeFunction::callAsFunction, JSRange::CloneRangeFuncNum),
    JS_CFUNC_MAGIC_DEF("detach", 0, JSRangePrototypeFunction::callAsFunction, JSRange::DetachFuncNum),
    JS_CFUNC_MAGIC_DEF("createContextualFragment", 1, JSRangePrototypeFunction::callAsFunction, JSRange::CreateContextualFragmentFuncNum),
    JS_CFUNC_MAGIC_DEF("compareNode", 1, JSRangePrototypeFunction::callAsFunction, JSRange::CompareNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("comparePoint", 2, JSRangePrototypeFunction::callAsFunction, JSRange::ComparePointFuncNum),
    JS_CFUNC_MAGIC_DEF("isPointInRange", 2, JSRangePrototypeFunction::callAsFunction, JSRange::IsPointInRangeFuncNum)
};

JSValue JSRangePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSRange.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSRangePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSRange.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSRangePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangeAttributesFunctions, countof(JSRangeAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangePrototypeConstantsFunctions, countof(JSRangePrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangePrototypeFunctions, countof(JSRangePrototypeFunctions));
}

JSValue JSRangePrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSRangeClassDefine = 
{
    "Range",
    .finalizer = JSRange::finalizer,
    .gc_mark = JSRange::mark,
};

JSClassID JSRange::js_class_id = 0;

void JSRange::init(JSContext* ctx)
{
    if (JSRange::js_class_id == 0) {
        JS_NewClassID(&JSRange::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSRange::js_class_id, &JSRangeClassDefine);
        JS_SetConstructor(ctx, JSRangeConstructor::self(ctx), JSRangePrototype::self(ctx));
        JS_SetClassProto(ctx, JSRange::js_class_id, JSRangePrototype::self(ctx));
    }
}

JSValue JSRange::create(JSContext* ctx, Range* impl)
{
    JSRange::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSRangePrototype::self(ctx), JSRange::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSRange::finalizer(JSRuntime* rt, JSValue val)
{
    Range* impl = (Range*)JS_GetOpaque(val, JSRange::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSRange::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSRange::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case StartContainerAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque2(ctx, this_val, JSRange::js_class_id);
            JSValue result = toJS(ctx, QJS::getPtr(imp->startContainer(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case StartOffsetAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque2(ctx, this_val, JSRange::js_class_id);
            JSValue result = JS_NewBigUint64(ctx, imp->startOffset(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case EndContainerAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque2(ctx, this_val, JSRange::js_class_id);
            JSValue result = toJS(ctx, QJS::getPtr(imp->endContainer(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case EndOffsetAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque2(ctx, this_val, JSRange::js_class_id);
            JSValue result = JS_NewBigUint64(ctx, imp->endOffset(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case CollapsedAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque2(ctx, this_val, JSRange::js_class_id);
            JSValue result = JS_NewBool(ctx, imp->collapsed(ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
        case CommonAncestorContainerAttrNum: {
            ExceptionCode ec = 0;
            Range* imp = (Range*)JS_GetOpaque2(ctx, this_val, JSRange::js_class_id);
            JSValue result = toJS(ctx, QJS::getPtr(imp->commonAncestorContainer(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSRange::getConstructor(JSContext *ctx)
{
    return JSRangeConstructor::self(ctx);
}

JSValue JSRangePrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Range* imp = (Range*)JS_GetOpaque2(ctx, this_val, JSRange::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSRange::SetStartFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setStart(refNode, offset, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetEndFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setEnd(refNode, offset, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetStartBeforeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->setStartBefore(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetStartAfterFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->setStartAfter(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetEndBeforeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->setEndBefore(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SetEndAfterFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->setEndAfter(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::CollapseFuncNum: {
            ExceptionCode ec = 0;
            bool toStart = valueToBoolean(ctx, argv[0]);
            imp->collapse(toStart, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SelectNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->selectNode(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SelectNodeContentsFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            imp->selectNodeContents(refNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::CompareBoundaryPointsFuncNum: {
            ExceptionCode ec = 0;
            Range::CompareHow how = static_cast<Range::CompareHow>(valueToInt32(ctx, argv[0]));
            Range* sourceRange = toRange(argv[1]);

            JSValue result = JS_NewBigUint64(ctx, imp->compareBoundaryPoints(how, sourceRange, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::DeleteContentsFuncNum: {
            ExceptionCode ec = 0;
            imp->deleteContents(ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::ExtractContentsFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->extractContents(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::CloneContentsFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->cloneContents(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::InsertNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* newNode = toNode(argv[0]);
            imp->insertNode(newNode, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::SurroundContentsFuncNum: {
            ExceptionCode ec = 0;
            Node* newParent = toNode(argv[0]);
            imp->surroundContents(newParent, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::CloneRangeFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->cloneRange(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::ToStringFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = JS_NewString(ctx, ((const String&)imp->toString(ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::DetachFuncNum: {
            ExceptionCode ec = 0;
            imp->detach(ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSRange::CreateContextualFragmentFuncNum: {
            ExceptionCode ec = 0;
            String html = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createContextualFragment(html, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::IntersectsNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);

            JSValue result = JS_NewBool(ctx, imp->intersectsNode(refNode, ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::CompareNodeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);

            JSValue result = JS_NewBigUint64(ctx, imp->compareNode(refNode, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::ComparePointFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewBigUint64(ctx, imp->comparePoint(refNode, offset, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSRange::IsPointInRangeFuncNum: {
            ExceptionCode ec = 0;
            Node* refNode = toNode(argv[0]);
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[1], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewBool(ctx, imp->isPointInRange(refNode, offset, ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, Range* obj)
{
    return QJS::cacheDOMObject<Range, JSRange>(ctx, obj);
}

Range* toRange(JSValue val)
{
    if (JS_IsObject(val)) {
        Range* impl = (Range*)JS_GetOpaque(val, JSRange::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
