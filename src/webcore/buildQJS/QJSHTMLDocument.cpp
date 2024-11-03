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

#include "QJSHTMLDocument.h"

#include "Element.h"
#include "HTMLCollection.h"
#include "HTMLDocument.h"
#include "QJSElement.h"
#include "QJSHTMLCollection.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLDocumentAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("bgColor", JSHTMLDocument::getValueProperty, JSHTMLDocument::putValueProperty, JSHTMLDocument::BgColorAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLDocument::getValueProperty, NULL, JSHTMLDocument::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSHTMLDocument::getValueProperty, NULL, JSHTMLDocument::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("plugins", JSHTMLDocument::getValueProperty, NULL, JSHTMLDocument::PluginsAttrNum),
    JS_CGETSET_MAGIC_DEF("dir", JSHTMLDocument::getValueProperty, JSHTMLDocument::putValueProperty, JSHTMLDocument::DirAttrNum),
    JS_CGETSET_MAGIC_DEF("embeds", JSHTMLDocument::getValueProperty, NULL, JSHTMLDocument::EmbedsAttrNum),
    JS_CGETSET_MAGIC_DEF("vlinkColor", JSHTMLDocument::getValueProperty, JSHTMLDocument::putValueProperty, JSHTMLDocument::VlinkColorAttrNum),
    JS_CGETSET_MAGIC_DEF("alinkColor", JSHTMLDocument::getValueProperty, JSHTMLDocument::putValueProperty, JSHTMLDocument::AlinkColorAttrNum),
    JS_CGETSET_MAGIC_DEF("compatMode", JSHTMLDocument::getValueProperty, NULL, JSHTMLDocument::CompatModeAttrNum),
    JS_CGETSET_MAGIC_DEF("scripts", JSHTMLDocument::getValueProperty, NULL, JSHTMLDocument::ScriptsAttrNum),
    JS_CGETSET_MAGIC_DEF("all", JSHTMLDocument::getValueProperty, JSHTMLDocument::putValueProperty, JSHTMLDocument::AllAttrNum),
    JS_CGETSET_MAGIC_DEF("designMode", JSHTMLDocument::getValueProperty, JSHTMLDocument::putValueProperty, JSHTMLDocument::DesignModeAttrNum),
    JS_CGETSET_MAGIC_DEF("fgColor", JSHTMLDocument::getValueProperty, JSHTMLDocument::putValueProperty, JSHTMLDocument::FgColorAttrNum),
    JS_CGETSET_MAGIC_DEF("linkColor", JSHTMLDocument::getValueProperty, JSHTMLDocument::putValueProperty, JSHTMLDocument::LinkColorAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLDocument::getValueProperty, NULL, JSHTMLDocument::ConstructorAttrNum)
};

class JSHTMLDocumentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLDocumentConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLDocumentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLDocument.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLDocumentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLDocument.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLDocumentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLDocumentPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getElementById", 1, JSHTMLDocumentPrototypeFunction::callAsFunction, JSHTMLDocument::GetElementByIdFuncNum),
    JS_CFUNC_MAGIC_DEF("close", 0, JSHTMLDocumentPrototypeFunction::callAsFunction, JSHTMLDocument::CloseFuncNum),
    JS_CFUNC_MAGIC_DEF("clear", 0, JSHTMLDocumentPrototypeFunction::callAsFunction, JSHTMLDocument::ClearFuncNum),
    JS_CFUNC_MAGIC_DEF("releaseEvents", 0, JSHTMLDocumentPrototypeFunction::callAsFunction, JSHTMLDocument::ReleaseEventsFuncNum),
    JS_CFUNC_MAGIC_DEF("open", 0, JSHTMLDocumentPrototypeFunction::callAsFunction, JSHTMLDocument::OpenFuncNum),
    JS_CFUNC_MAGIC_DEF("write", 1, JSHTMLDocumentPrototypeFunction::callAsFunction, JSHTMLDocument::WriteFuncNum),
    JS_CFUNC_MAGIC_DEF("writeln", 1, JSHTMLDocumentPrototypeFunction::callAsFunction, JSHTMLDocument::WritelnFuncNum),
    JS_CFUNC_MAGIC_DEF("captureEvents", 0, JSHTMLDocumentPrototypeFunction::callAsFunction, JSHTMLDocument::CaptureEventsFuncNum)
};

JSValue JSHTMLDocumentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLDocument.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSDocumentPrototype::self(ctx));
        JSHTMLDocumentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLDocument.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLDocumentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDocumentAttributesFunctions, countof(JSHTMLDocumentAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDocumentPrototypeFunctions, countof(JSHTMLDocumentPrototypeFunctions));
}

static JSClassDef JSHTMLDocumentClassDefine = 
{
    "HTMLDocument",
    .finalizer = JSHTMLDocument::finalizer,
    .gc_mark = JSHTMLDocument::mark,
};

JSClassID JSHTMLDocument::js_class_id = 0;

void JSHTMLDocument::init(JSContext* ctx)
{
    if (JSHTMLDocument::js_class_id == 0) {
        JS_NewClassID(&JSHTMLDocument::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLDocument::js_class_id, &JSHTMLDocumentClassDefine);
        JS_SetConstructor(ctx, JSHTMLDocumentConstructor::self(ctx), JSHTMLDocumentPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLDocument::js_class_id, JSHTMLDocumentPrototype::self(ctx));
    }
}

JSValue JSHTMLDocument::create(JSContext* ctx, HTMLDocument* impl)
{
    JSHTMLDocument::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLDocumentPrototype::self(ctx), JSHTMLDocument::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLDocument::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLDocument* impl = (HTMLDocument*)JS_GetOpaque(val, JSHTMLDocument::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLDocument::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSDocument::mark(rt, val, mark_func);
}

JSValue JSHTMLDocument::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case EmbedsAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->embeds()));
        }
        case PluginsAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->plugins()));
        }
        case ScriptsAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->scripts()));
        }
        case AllAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JSHTMLDocument::all(ctx, imp);
        }
        case WidthAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewBigUint64(ctx, imp->width());
        }
        case HeightAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewBigUint64(ctx, imp->height());
        }
        case DirAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->dir()).utf8().data());
        }
        case DesignModeAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->designMode()).utf8().data());
        }
        case CompatModeAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->compatMode()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case FgColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->fgColor()).utf8().data());
        }
        case AlinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alinkColor()).utf8().data());
        }
        case LinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->linkColor()).utf8().data());
        }
        case VlinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vlinkColor()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLDocument::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AllAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            JSHTMLDocument::setAll(ctx, value, imp);
            break;
        }
        case DirAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            imp->setDir(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DesignModeAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            imp->setDesignMode(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FgColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            imp->setFgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            imp->setAlinkColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            imp->setLinkColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VlinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
            imp->setVlinkColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLDocument::getConstructor(JSContext *ctx)
{
    return JSHTMLDocumentConstructor::self(ctx);
}

JSValue JSHTMLDocumentPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque2(ctx, this_val, JSHTMLDocument::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLDocument::OpenFuncNum: {
            return JSHTMLDocument::open(ctx, argc, argv, imp);
        }
        case JSHTMLDocument::CloseFuncNum: {
            imp->close();
            return JS_UNDEFINED;
        }
        case JSHTMLDocument::WriteFuncNum: {
            return JSHTMLDocument::write(ctx, argc, argv, imp);
        }
        case JSHTMLDocument::WritelnFuncNum: {
            return JSHTMLDocument::writeln(ctx, argc, argv, imp);
        }
        case JSHTMLDocument::GetElementByIdFuncNum: {
            String elementId = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getElementById(elementId)));
            return result;
        }
        case JSHTMLDocument::ClearFuncNum: {
            return JSHTMLDocument::clear(ctx, argc, argv, imp);
        }
        case JSHTMLDocument::CaptureEventsFuncNum: {
            imp->captureEvents();
            return JS_UNDEFINED;
        }
        case JSHTMLDocument::ReleaseEventsFuncNum: {
            imp->releaseEvents();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
