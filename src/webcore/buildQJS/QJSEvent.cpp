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

#include "QJSEvent.h"

#include "Event.h"
#include "EventTargetNode.h"
#include "QJSEventTargetNode.h"
#include "qjs_dom.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSEventAttributesFunctions[12];
static bool JSEventAttributesFunctions_initialized = false;

static void init_JSEventAttributesFunctions()
{
    if (JSEventAttributesFunctions_initialized) return;
    JSEventAttributesFunctions_initialized = true;
    memset(JSEventAttributesFunctions, 0, sizeof(JSEventAttributesFunctions));
    JSEventAttributesFunctions[0].name = "srcElement";
    JSEventAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[0].magic = JSEvent::SrcElementAttrNum;
    JSEventAttributesFunctions[0].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[1].name = "bubbles";
    JSEventAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[1].magic = JSEvent::BubblesAttrNum;
    JSEventAttributesFunctions[1].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[2].name = "currentTarget";
    JSEventAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[2].magic = JSEvent::CurrentTargetAttrNum;
    JSEventAttributesFunctions[2].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[3].name = "type";
    JSEventAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[3].magic = JSEvent::TypeAttrNum;
    JSEventAttributesFunctions[3].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[4].name = "returnValue";
    JSEventAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[4].magic = JSEvent::ReturnValueAttrNum;
    JSEventAttributesFunctions[4].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[4].u.getset.set.setter_magic = JSEvent::putValueProperty;
    JSEventAttributesFunctions[5].name = "constructor";
    JSEventAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[5].magic = JSEvent::ConstructorAttrNum;
    JSEventAttributesFunctions[5].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[6].name = "target";
    JSEventAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[6].magic = JSEvent::TargetAttrNum;
    JSEventAttributesFunctions[6].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[7].name = "eventPhase";
    JSEventAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[7].magic = JSEvent::EventPhaseAttrNum;
    JSEventAttributesFunctions[7].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[8].name = "cancelable";
    JSEventAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[8].magic = JSEvent::CancelableAttrNum;
    JSEventAttributesFunctions[8].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[9].name = "timeStamp";
    JSEventAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[9].magic = JSEvent::TimeStampAttrNum;
    JSEventAttributesFunctions[9].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSEventAttributesFunctions[10].name = "cancelBubble";
    JSEventAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[10].magic = JSEvent::CancelBubbleAttrNum;
    JSEventAttributesFunctions[10].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[10].u.getset.set.setter_magic = JSEvent::putValueProperty;
    JSEventAttributesFunctions[11].name = "clipboardData";
    JSEventAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventAttributesFunctions[11].magic = JSEvent::ClipboardDataAttrNum;
    JSEventAttributesFunctions[11].u.getset.get.getter_magic = JSEvent::getValueProperty;
    JSEventAttributesFunctions[11].u.getset.set.setter_magic = NULL;
}

class JSEventConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSEventConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSEventConstructorFunctions[19];
static bool JSEventConstructorFunctions_initialized = false;

static void init_JSEventConstructorFunctions()
{
    if (JSEventConstructorFunctions_initialized) return;
    JSEventConstructorFunctions_initialized = true;
    memset(JSEventConstructorFunctions, 0, sizeof(JSEventConstructorFunctions));
    JSEventConstructorFunctions[0].name = "MOUSEOUT";
    JSEventConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[0].magic = Event::MOUSEOUT;
    JSEventConstructorFunctions[0].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[1].name = "MOUSEOVER";
    JSEventConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[1].magic = Event::MOUSEOVER;
    JSEventConstructorFunctions[1].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[2].name = "CAPTURING_PHASE";
    JSEventConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[2].magic = Event::CAPTURING_PHASE;
    JSEventConstructorFunctions[2].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[2].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[3].name = "MOUSEDOWN";
    JSEventConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[3].magic = Event::MOUSEDOWN;
    JSEventConstructorFunctions[3].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[3].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[4].name = "AT_TARGET";
    JSEventConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[4].magic = Event::AT_TARGET;
    JSEventConstructorFunctions[4].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[4].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[5].name = "DRAGDROP";
    JSEventConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[5].magic = Event::DRAGDROP;
    JSEventConstructorFunctions[5].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[5].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[6].name = "KEYUP";
    JSEventConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[6].magic = Event::KEYUP;
    JSEventConstructorFunctions[6].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[6].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[7].name = "SELECT";
    JSEventConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[7].magic = Event::SELECT;
    JSEventConstructorFunctions[7].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[7].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[8].name = "BLUR";
    JSEventConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[8].magic = Event::BLUR;
    JSEventConstructorFunctions[8].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[8].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[9].name = "MOUSEMOVE";
    JSEventConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[9].magic = Event::MOUSEMOVE;
    JSEventConstructorFunctions[9].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[9].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[10].name = "CLICK";
    JSEventConstructorFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[10].magic = Event::CLICK;
    JSEventConstructorFunctions[10].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[10].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[11].name = "MOUSEUP";
    JSEventConstructorFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[11].magic = Event::MOUSEUP;
    JSEventConstructorFunctions[11].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[11].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[12].name = "BUBBLING_PHASE";
    JSEventConstructorFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[12].magic = Event::BUBBLING_PHASE;
    JSEventConstructorFunctions[12].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[12].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[13].name = "MOUSEDRAG";
    JSEventConstructorFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[13].magic = Event::MOUSEDRAG;
    JSEventConstructorFunctions[13].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[13].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[14].name = "DBLCLICK";
    JSEventConstructorFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[14].magic = Event::DBLCLICK;
    JSEventConstructorFunctions[14].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[14].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[15].name = "KEYDOWN";
    JSEventConstructorFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[15].magic = Event::KEYDOWN;
    JSEventConstructorFunctions[15].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[15].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[16].name = "KEYPRESS";
    JSEventConstructorFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[16].magic = Event::KEYPRESS;
    JSEventConstructorFunctions[16].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[16].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[17].name = "FOCUS";
    JSEventConstructorFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[17].magic = Event::FOCUS;
    JSEventConstructorFunctions[17].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[17].u.getset.set.setter_magic = NULL;
    JSEventConstructorFunctions[18].name = "CHANGE";
    JSEventConstructorFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventConstructorFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventConstructorFunctions[18].magic = Event::CHANGE;
    JSEventConstructorFunctions[18].u.getset.get.getter_magic = JSEventConstructor::getValueProperty;
    JSEventConstructorFunctions[18].u.getset.set.setter_magic = NULL;
}

JSValue JSEventConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Event.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSEventConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Event.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSEventConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSEventConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSEventConstructorFunctions, countof(JSEventConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSEventPrototypeConstantsFunctions[19];
static bool JSEventPrototypeConstantsFunctions_initialized = false;

static void init_JSEventPrototypeConstantsFunctions()
{
    if (JSEventPrototypeConstantsFunctions_initialized) return;
    JSEventPrototypeConstantsFunctions_initialized = true;
    memset(JSEventPrototypeConstantsFunctions, 0, sizeof(JSEventPrototypeConstantsFunctions));
    JSEventPrototypeConstantsFunctions[0].name = "MOUSEOUT";
    JSEventPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[0].magic = Event::MOUSEOUT;
    JSEventPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[1].name = "MOUSEOVER";
    JSEventPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[1].magic = Event::MOUSEOVER;
    JSEventPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[2].name = "CAPTURING_PHASE";
    JSEventPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[2].magic = Event::CAPTURING_PHASE;
    JSEventPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[3].name = "MOUSEDOWN";
    JSEventPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[3].magic = Event::MOUSEDOWN;
    JSEventPrototypeConstantsFunctions[3].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[3].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[4].name = "AT_TARGET";
    JSEventPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[4].magic = Event::AT_TARGET;
    JSEventPrototypeConstantsFunctions[4].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[4].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[5].name = "DRAGDROP";
    JSEventPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[5].magic = Event::DRAGDROP;
    JSEventPrototypeConstantsFunctions[5].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[5].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[6].name = "KEYUP";
    JSEventPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[6].magic = Event::KEYUP;
    JSEventPrototypeConstantsFunctions[6].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[6].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[7].name = "SELECT";
    JSEventPrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[7].magic = Event::SELECT;
    JSEventPrototypeConstantsFunctions[7].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[7].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[8].name = "BLUR";
    JSEventPrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[8].magic = Event::BLUR;
    JSEventPrototypeConstantsFunctions[8].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[8].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[9].name = "MOUSEMOVE";
    JSEventPrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[9].magic = Event::MOUSEMOVE;
    JSEventPrototypeConstantsFunctions[9].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[9].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[10].name = "CLICK";
    JSEventPrototypeConstantsFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[10].magic = Event::CLICK;
    JSEventPrototypeConstantsFunctions[10].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[10].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[11].name = "MOUSEUP";
    JSEventPrototypeConstantsFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[11].magic = Event::MOUSEUP;
    JSEventPrototypeConstantsFunctions[11].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[11].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[12].name = "BUBBLING_PHASE";
    JSEventPrototypeConstantsFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[12].magic = Event::BUBBLING_PHASE;
    JSEventPrototypeConstantsFunctions[12].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[12].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[13].name = "MOUSEDRAG";
    JSEventPrototypeConstantsFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[13].magic = Event::MOUSEDRAG;
    JSEventPrototypeConstantsFunctions[13].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[13].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[14].name = "DBLCLICK";
    JSEventPrototypeConstantsFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[14].magic = Event::DBLCLICK;
    JSEventPrototypeConstantsFunctions[14].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[14].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[15].name = "KEYDOWN";
    JSEventPrototypeConstantsFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[15].magic = Event::KEYDOWN;
    JSEventPrototypeConstantsFunctions[15].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[15].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[16].name = "KEYPRESS";
    JSEventPrototypeConstantsFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[16].magic = Event::KEYPRESS;
    JSEventPrototypeConstantsFunctions[16].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[16].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[17].name = "FOCUS";
    JSEventPrototypeConstantsFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[17].magic = Event::FOCUS;
    JSEventPrototypeConstantsFunctions[17].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[17].u.getset.set.setter_magic = NULL;
    JSEventPrototypeConstantsFunctions[18].name = "CHANGE";
    JSEventPrototypeConstantsFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSEventPrototypeConstantsFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSEventPrototypeConstantsFunctions[18].magic = Event::CHANGE;
    JSEventPrototypeConstantsFunctions[18].u.getset.get.getter_magic = JSEventPrototype::getValueProperty;
    JSEventPrototypeConstantsFunctions[18].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSEventPrototypeFunctions[3];
static bool JSEventPrototypeFunctions_initialized = false;

static void init_JSEventPrototypeFunctions()
{
    if (JSEventPrototypeFunctions_initialized) return;
    JSEventPrototypeFunctions_initialized = true;
    memset(JSEventPrototypeFunctions, 0, sizeof(JSEventPrototypeFunctions));
    JSEventPrototypeFunctions[0].name = "stopPropagation";
    JSEventPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSEventPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSEventPrototypeFunctions[0].magic = JSEvent::StopPropagationFuncNum;
    JSEventPrototypeFunctions[0].u.func.length = 0;
    JSEventPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSEventPrototypeFunctions[0].u.func.cfunc.generic_magic = JSEventPrototypeFunction::callAsFunction;
    JSEventPrototypeFunctions[1].name = "preventDefault";
    JSEventPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSEventPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSEventPrototypeFunctions[1].magic = JSEvent::PreventDefaultFuncNum;
    JSEventPrototypeFunctions[1].u.func.length = 0;
    JSEventPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSEventPrototypeFunctions[1].u.func.cfunc.generic_magic = JSEventPrototypeFunction::callAsFunction;
    JSEventPrototypeFunctions[2].name = "initEvent";
    JSEventPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSEventPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSEventPrototypeFunctions[2].magic = JSEvent::InitEventFuncNum;
    JSEventPrototypeFunctions[2].u.func.length = 3;
    JSEventPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSEventPrototypeFunctions[2].u.func.cfunc.generic_magic = JSEventPrototypeFunction::callAsFunction;
}

JSValue JSEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSEvent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSEventAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSEventAttributesFunctions, countof(JSEventAttributesFunctions));
    init_JSEventPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSEventPrototypeConstantsFunctions, countof(JSEventPrototypeConstantsFunctions));
    init_JSEventPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSEventPrototypeFunctions, countof(JSEventPrototypeFunctions));
}

JSValue JSEventPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSEventClassDefine;
static bool JSEventClassDefine_initialized = false;

static void init_JSEventClassDefine()
{
    if (JSEventClassDefine_initialized) return;
    JSEventClassDefine_initialized = true;
    memset(&JSEventClassDefine, 0, sizeof(JSEventClassDefine));
    JSEventClassDefine.class_name = "Event";
    JSEventClassDefine.finalizer = JSEvent::finalizer;
    JSEventClassDefine.gc_mark = JSEvent::mark;
}

JSClassID JSEvent::js_class_id = 0;

void JSEvent::init(JSContext* ctx)
{
    if (JSEvent::js_class_id == 0) {
        init_JSEventClassDefine();
        JS_NewClassID(&JSEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSEvent::js_class_id, &JSEventClassDefine);
        JS_SetConstructor(ctx, JSEventConstructor::self(ctx), JSEventPrototype::self(ctx));
        JS_SetClassProto(ctx, JSEvent::js_class_id, JSEventPrototype::self(ctx));
    }
}

JSValue JSEvent::create(JSContext* ctx, Event* impl)
{
    JSEvent::init(ctx);
    JSValue _proto = JSEventPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSEvent::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSEvent::finalizer(JSRuntime* rt, JSValue val)
{
    Event* impl = (Event*)JS_GetOpaque(val, JSEvent::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case TargetAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->target()));
        }
        case CurrentTargetAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->currentTarget()));
        }
        case EventPhaseAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return JS_NewInt32(ctx, imp->eventPhase());
        }
        case BubblesAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return JS_NewBool(ctx, imp->bubbles() ? 1 : 0);
        }
        case CancelableAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return JS_NewBool(ctx, imp->cancelable() ? 1 : 0);
        }
        case TimeStampAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return JS_NewInt32(ctx, imp->timeStamp());
        }
        case SrcElementAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->srcElement()));
        }
        case ReturnValueAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return JS_NewBool(ctx, imp->returnValue() ? 1 : 0);
        }
        case CancelBubbleAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return JS_NewBool(ctx, imp->cancelBubble() ? 1 : 0);
        }
        case ClipboardDataAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            return JSEvent::clipboardData(ctx, this_val, imp);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSEvent::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ReturnValueAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            imp->setReturnValue(valueToBoolean(ctx, value));
            break;
        }
        case CancelBubbleAttrNum: {
            Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
            imp->setCancelBubble(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSEvent::getConstructor(JSContext *ctx)
{
    return JSEventConstructor::self(ctx);
}

JSValue JSEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Event* imp = (Event*)JS_GetOpaque(this_val, JSEvent::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSEvent::StopPropagationFuncNum: {
            imp->stopPropagation();
            return JS_UNDEFINED;
        }
        case JSEvent::PreventDefaultFuncNum: {
            imp->preventDefault();
            return JS_UNDEFINED;
        }
        case JSEvent::InitEventFuncNum: {
            String eventTypeArg = valueToString(ctx, argv[0]);
            bool canBubbleArg = valueToBoolean(ctx, argv[1]);
            bool cancelableArg = valueToBoolean(ctx, argv[2]);
            imp->initEvent(eventTypeArg, canBubbleArg, cancelableArg);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


Event* toEvent(JSValue val)
{
    if (JS_IsObject(val)) {
        Event* impl = (Event*)JS_GetOpaque(val, JSEvent::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
