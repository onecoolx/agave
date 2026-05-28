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

static JSCFunctionListEntry JSHTMLDocumentAttributesFunctions[15];
static bool JSHTMLDocumentAttributesFunctions_initialized = false;

static void init_JSHTMLDocumentAttributesFunctions()
{
    if (JSHTMLDocumentAttributesFunctions_initialized) return;
    JSHTMLDocumentAttributesFunctions_initialized = true;
    memset(JSHTMLDocumentAttributesFunctions, 0, sizeof(JSHTMLDocumentAttributesFunctions));
    JSHTMLDocumentAttributesFunctions[0].name = "embeds";
    JSHTMLDocumentAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[0].magic = JSHTMLDocument::EmbedsAttrNum;
    JSHTMLDocumentAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLDocumentAttributesFunctions[1].name = "plugins";
    JSHTMLDocumentAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[1].magic = JSHTMLDocument::PluginsAttrNum;
    JSHTMLDocumentAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSHTMLDocumentAttributesFunctions[2].name = "scripts";
    JSHTMLDocumentAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[2].magic = JSHTMLDocument::ScriptsAttrNum;
    JSHTMLDocumentAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSHTMLDocumentAttributesFunctions[3].name = "all";
    JSHTMLDocumentAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[3].magic = JSHTMLDocument::AllAttrNum;
    JSHTMLDocumentAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLDocument::putValueProperty;
    JSHTMLDocumentAttributesFunctions[4].name = "width";
    JSHTMLDocumentAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[4].magic = JSHTMLDocument::WidthAttrNum;
    JSHTMLDocumentAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSHTMLDocumentAttributesFunctions[5].name = "height";
    JSHTMLDocumentAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[5].magic = JSHTMLDocument::HeightAttrNum;
    JSHTMLDocumentAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSHTMLDocumentAttributesFunctions[6].name = "dir";
    JSHTMLDocumentAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[6].magic = JSHTMLDocument::DirAttrNum;
    JSHTMLDocumentAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLDocument::putValueProperty;
    JSHTMLDocumentAttributesFunctions[7].name = "designMode";
    JSHTMLDocumentAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[7].magic = JSHTMLDocument::DesignModeAttrNum;
    JSHTMLDocumentAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLDocument::putValueProperty;
    JSHTMLDocumentAttributesFunctions[8].name = "compatMode";
    JSHTMLDocumentAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[8].magic = JSHTMLDocument::CompatModeAttrNum;
    JSHTMLDocumentAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSHTMLDocumentAttributesFunctions[9].name = "bgColor";
    JSHTMLDocumentAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[9].magic = JSHTMLDocument::BgColorAttrNum;
    JSHTMLDocumentAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[9].u.getset.set.setter_magic = JSHTMLDocument::putValueProperty;
    JSHTMLDocumentAttributesFunctions[10].name = "fgColor";
    JSHTMLDocumentAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[10].magic = JSHTMLDocument::FgColorAttrNum;
    JSHTMLDocumentAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLDocument::putValueProperty;
    JSHTMLDocumentAttributesFunctions[11].name = "alinkColor";
    JSHTMLDocumentAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[11].magic = JSHTMLDocument::AlinkColorAttrNum;
    JSHTMLDocumentAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[11].u.getset.set.setter_magic = JSHTMLDocument::putValueProperty;
    JSHTMLDocumentAttributesFunctions[12].name = "linkColor";
    JSHTMLDocumentAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[12].magic = JSHTMLDocument::LinkColorAttrNum;
    JSHTMLDocumentAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[12].u.getset.set.setter_magic = JSHTMLDocument::putValueProperty;
    JSHTMLDocumentAttributesFunctions[13].name = "vlinkColor";
    JSHTMLDocumentAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[13].magic = JSHTMLDocument::VlinkColorAttrNum;
    JSHTMLDocumentAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[13].u.getset.set.setter_magic = JSHTMLDocument::putValueProperty;
    JSHTMLDocumentAttributesFunctions[14].name = "constructor";
    JSHTMLDocumentAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDocumentAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDocumentAttributesFunctions[14].magic = JSHTMLDocument::ConstructorAttrNum;
    JSHTMLDocumentAttributesFunctions[14].u.getset.get.getter_magic = JSHTMLDocument::getValueProperty;
    JSHTMLDocumentAttributesFunctions[14].u.getset.set.setter_magic = NULL;
}

class JSHTMLDocumentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLDocumentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLDocument.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLDocumentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLDocument.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLDocumentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLDocumentPrototypeFunctions[8];
static bool JSHTMLDocumentPrototypeFunctions_initialized = false;

static void init_JSHTMLDocumentPrototypeFunctions()
{
    if (JSHTMLDocumentPrototypeFunctions_initialized) return;
    JSHTMLDocumentPrototypeFunctions_initialized = true;
    memset(JSHTMLDocumentPrototypeFunctions, 0, sizeof(JSHTMLDocumentPrototypeFunctions));
    JSHTMLDocumentPrototypeFunctions[0].name = "open";
    JSHTMLDocumentPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLDocumentPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLDocumentPrototypeFunctions[0].magic = JSHTMLDocument::OpenFuncNum;
    JSHTMLDocumentPrototypeFunctions[0].u.func.length = 0;
    JSHTMLDocumentPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLDocumentPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLDocumentPrototypeFunction::callAsFunction;
    JSHTMLDocumentPrototypeFunctions[1].name = "close";
    JSHTMLDocumentPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLDocumentPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLDocumentPrototypeFunctions[1].magic = JSHTMLDocument::CloseFuncNum;
    JSHTMLDocumentPrototypeFunctions[1].u.func.length = 0;
    JSHTMLDocumentPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLDocumentPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLDocumentPrototypeFunction::callAsFunction;
    JSHTMLDocumentPrototypeFunctions[2].name = "write";
    JSHTMLDocumentPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLDocumentPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSHTMLDocumentPrototypeFunctions[2].magic = JSHTMLDocument::WriteFuncNum;
    JSHTMLDocumentPrototypeFunctions[2].u.func.length = 1;
    JSHTMLDocumentPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLDocumentPrototypeFunctions[2].u.func.cfunc.generic_magic = JSHTMLDocumentPrototypeFunction::callAsFunction;
    JSHTMLDocumentPrototypeFunctions[3].name = "writeln";
    JSHTMLDocumentPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLDocumentPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSHTMLDocumentPrototypeFunctions[3].magic = JSHTMLDocument::WritelnFuncNum;
    JSHTMLDocumentPrototypeFunctions[3].u.func.length = 1;
    JSHTMLDocumentPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLDocumentPrototypeFunctions[3].u.func.cfunc.generic_magic = JSHTMLDocumentPrototypeFunction::callAsFunction;
    JSHTMLDocumentPrototypeFunctions[4].name = "getElementById";
    JSHTMLDocumentPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLDocumentPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSHTMLDocumentPrototypeFunctions[4].magic = JSHTMLDocument::GetElementByIdFuncNum;
    JSHTMLDocumentPrototypeFunctions[4].u.func.length = 1;
    JSHTMLDocumentPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLDocumentPrototypeFunctions[4].u.func.cfunc.generic_magic = JSHTMLDocumentPrototypeFunction::callAsFunction;
    JSHTMLDocumentPrototypeFunctions[5].name = "clear";
    JSHTMLDocumentPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLDocumentPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSHTMLDocumentPrototypeFunctions[5].magic = JSHTMLDocument::ClearFuncNum;
    JSHTMLDocumentPrototypeFunctions[5].u.func.length = 0;
    JSHTMLDocumentPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLDocumentPrototypeFunctions[5].u.func.cfunc.generic_magic = JSHTMLDocumentPrototypeFunction::callAsFunction;
    JSHTMLDocumentPrototypeFunctions[6].name = "captureEvents";
    JSHTMLDocumentPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLDocumentPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSHTMLDocumentPrototypeFunctions[6].magic = JSHTMLDocument::CaptureEventsFuncNum;
    JSHTMLDocumentPrototypeFunctions[6].u.func.length = 0;
    JSHTMLDocumentPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLDocumentPrototypeFunctions[6].u.func.cfunc.generic_magic = JSHTMLDocumentPrototypeFunction::callAsFunction;
    JSHTMLDocumentPrototypeFunctions[7].name = "releaseEvents";
    JSHTMLDocumentPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLDocumentPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSHTMLDocumentPrototypeFunctions[7].magic = JSHTMLDocument::ReleaseEventsFuncNum;
    JSHTMLDocumentPrototypeFunctions[7].u.func.length = 0;
    JSHTMLDocumentPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLDocumentPrototypeFunctions[7].u.func.cfunc.generic_magic = JSHTMLDocumentPrototypeFunction::callAsFunction;
}

JSValue JSHTMLDocumentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLDocument.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSDocumentPrototype::self(ctx));
        JSHTMLDocumentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLDocument.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLDocumentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLDocumentAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDocumentAttributesFunctions, countof(JSHTMLDocumentAttributesFunctions));
    init_JSHTMLDocumentPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDocumentPrototypeFunctions, countof(JSHTMLDocumentPrototypeFunctions));
}

JSClassID JSHTMLDocument::js_class_id = 0;

void JSHTMLDocument::init(JSContext* ctx)
{
    if (JSHTMLDocument::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLDocument::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLDocument::create(JSContext* ctx, HTMLDocument* impl)
{
    JSHTMLDocument::init(ctx);
    JSValue _proto = JSHTMLDocumentPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLDocument::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLDocument* impl = (HTMLDocument*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->embeds()));
        }
        case PluginsAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->plugins()));
        }
        case ScriptsAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->scripts()));
        }
        case AllAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JSHTMLDocument::all(ctx, this_val, imp);
        }
        case WidthAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->width());
        }
        case HeightAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->height());
        }
        case DirAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->dir()).utf8().data());
        }
        case DesignModeAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->designMode()).utf8().data());
        }
        case CompatModeAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->compatMode()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case FgColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->fgColor()).utf8().data());
        }
        case AlinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alinkColor()).utf8().data());
        }
        case LinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->linkColor()).utf8().data());
        }
        case VlinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            JSHTMLDocument::setAll(ctx, this_val, value, imp);
            break;
        }
        case DirAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDir(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DesignModeAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDesignMode(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FgColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setFgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlinkColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setLinkColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VlinkColorAttrNum: {
            HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    HTMLDocument* imp = (HTMLDocument*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLDocument::OpenFuncNum: {
            return JSHTMLDocument::open(ctx, this_val, argc, argv, imp);
        }
        case JSHTMLDocument::CloseFuncNum: {
            imp->close();
            return JS_UNDEFINED;
        }
        case JSHTMLDocument::WriteFuncNum: {
            return JSHTMLDocument::write(ctx, this_val, argc, argv, imp);
        }
        case JSHTMLDocument::WritelnFuncNum: {
            return JSHTMLDocument::writeln(ctx, this_val, argc, argv, imp);
        }
        case JSHTMLDocument::GetElementByIdFuncNum: {
            String elementId = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getElementById(elementId)));
            return result;
        }
        case JSHTMLDocument::ClearFuncNum: {
            return JSHTMLDocument::clear(ctx, this_val, argc, argv, imp);
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
