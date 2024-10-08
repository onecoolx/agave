/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#include "JSUIEvent.h"

#include <wtf/GetPtr.h>

#include "ExceptionCode.h"
#include "JSDOMWindow.h"
#include "UIEvent.h"
#include "kjs_window.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSUIEventTableEntries[] =
{
    { "layerX", JSUIEvent::LayerXAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "view", JSUIEvent::ViewAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "pageX", JSUIEvent::PageXAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "which", JSUIEvent::WhichAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "detail", JSUIEvent::DetailAttrNum, DontDelete|ReadOnly, 0, &JSUIEventTableEntries[9] },
    { "keyCode", JSUIEvent::KeyCodeAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "charCode", JSUIEvent::CharCodeAttrNum, DontDelete|ReadOnly, 0, &JSUIEventTableEntries[10] },
    { "layerY", JSUIEvent::LayerYAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "pageY", JSUIEvent::PageYAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSUIEventTable = 
{
    2, 11, JSUIEventTableEntries, 9
};

/* Hash table for prototype */

static const HashEntry JSUIEventPrototypeTableEntries[] =
{
    { "initUIEvent", JSUIEvent::InitUIEventFuncNum, DontDelete|Function, 5, 0 }
};

static const HashTable JSUIEventPrototypeTable = 
{
    2, 1, JSUIEventPrototypeTableEntries, 1
};

const ClassInfo JSUIEventPrototype::info = { "UIEventPrototype", 0, &JSUIEventPrototypeTable, 0 };

JSObject* JSUIEventPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSUIEventPrototype>(exec, "[[JSUIEvent.prototype]]");
}

bool JSUIEventPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSUIEventPrototypeFunction, JSObject>(exec, &JSUIEventPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSUIEvent::info = { "UIEvent", &JSEvent::info, &JSUIEventTable, 0 };

JSUIEvent::JSUIEvent(ExecState* exec, UIEvent* impl)
    : JSEvent(exec, impl)
{
    setPrototype(JSUIEventPrototype::self(exec));
}

bool JSUIEvent::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSUIEvent, JSEvent>(exec, &JSUIEventTable, this, propertyName, slot);
}

JSValue* JSUIEvent::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case ViewAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return toJS(exec, WTF::getPtr(imp->view()));
    }
    case DetailAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return jsNumber(imp->detail());
    }
    case KeyCodeAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return jsNumber(imp->keyCode());
    }
    case CharCodeAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return jsNumber(imp->charCode());
    }
    case LayerXAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return jsNumber(imp->layerX());
    }
    case LayerYAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return jsNumber(imp->layerY());
    }
    case PageXAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return jsNumber(imp->pageX());
    }
    case PageYAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return jsNumber(imp->pageY());
    }
    case WhichAttrNum: {
        UIEvent* imp = static_cast<UIEvent*>(impl());

        return jsNumber(imp->which());
    }
    }
    return 0;
}

JSValue* JSUIEventPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSUIEvent::info))
      return throwError(exec, TypeError);

    JSUIEvent* castedThisObj = static_cast<JSUIEvent*>(thisObj);
    UIEvent* imp = static_cast<UIEvent*>(castedThisObj->impl());

    switch (id) {
    case JSUIEvent::InitUIEventFuncNum: {
        AtomicString type = args[0]->toString(exec);
        bool canBubble = args[1]->toBoolean(exec);
        bool cancelable = args[2]->toBoolean(exec);
        DOMWindow* view = toDOMWindow(args[3]);
        bool detailOk;
        int detail = args[4]->toInt32(exec, detailOk);
        if (!detailOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }

        imp->initUIEvent(type, canBubble, cancelable, view, detail);
        return jsUndefined();
    }
    }
    return 0;
}

}
