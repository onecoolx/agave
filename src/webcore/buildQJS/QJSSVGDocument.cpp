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
#include "QJSSVGDocument.h"

#include "Event.h"
#include "QJSEvent.h"
#include "QJSSVGSVGElement.h"
#include "SVGDocument.h"
#include "SVGSVGElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGDocumentAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("rootElement", JSSVGDocument::getValueProperty, NULL, JSSVGDocument::RootElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGDocumentPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("createEvent", 1, JSSVGDocumentPrototypeFunction::callAsFunction, JSSVGDocument::CreateEventFuncNum)
};

JSValue JSSVGDocumentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGDocument.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSDocumentPrototype::self(ctx));
        JSSVGDocumentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGDocument.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGDocumentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGDocumentAttributesFunctions, countof(JSSVGDocumentAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGDocumentPrototypeFunctions, countof(JSSVGDocumentPrototypeFunctions));
}

static JSClassDef JSSVGDocumentClassDefine = 
{
    "SVGDocument",
    .finalizer = JSSVGDocument::finalizer,
    .gc_mark = JSSVGDocument::mark,
};

JSClassID JSSVGDocument::js_class_id = 0;

void JSSVGDocument::init(JSContext* ctx)
{
    if (JSSVGDocument::js_class_id == 0) {
        JS_NewClassID(&JSSVGDocument::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGDocument::js_class_id, &JSSVGDocumentClassDefine);
        JS_SetClassProto(ctx, JSSVGDocument::js_class_id, JSSVGDocumentPrototype::self(ctx));
    }
}

JSValue JSSVGDocument::create(JSContext* ctx, SVGDocument* impl)
{
    JSSVGDocument::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGDocumentPrototype::self(ctx), JSSVGDocument::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGDocument::finalizer(JSRuntime* rt, JSValue val)
{
    SVGDocument* impl = (SVGDocument*)JS_GetOpaque(val, JSSVGDocument::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGDocument::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSDocument::mark(rt, val, mark_func);
}

JSValue JSSVGDocument::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RootElementAttrNum: {
            SVGDocument* imp = (SVGDocument*)JS_GetOpaque2(ctx, this_val, JSSVGDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->rootElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGDocumentPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGDocument* imp = (SVGDocument*)JS_GetOpaque2(ctx, this_val, JSSVGDocument::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGDocument::CreateEventFuncNum: {
            ExceptionCode ec = 0;
            String eventType = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createEvent(eventType, ec)));
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
