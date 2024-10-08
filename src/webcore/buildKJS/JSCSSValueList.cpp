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

#include "JSCSSValueList.h"

#include <wtf/GetPtr.h>

#include "CSSValue.h"
#include "CSSValueList.h"
#include "ExceptionCode.h"
#include "JSCSSValue.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSCSSValueListTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "length", JSCSSValueList::LengthAttrNum, DontDelete|ReadOnly, 0, &JSCSSValueListTableEntries[2] },
    { "constructor", JSCSSValueList::ConstructorAttrNum, DontDelete|DontEnum|ReadOnly, 0, 0 }
};

static const HashTable JSCSSValueListTable = 
{
    2, 3, JSCSSValueListTableEntries, 2
};

/* Hash table for constructor */

static const HashEntry JSCSSValueListConstructorTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSCSSValueListConstructorTable = 
{
    2, 1, JSCSSValueListConstructorTableEntries, 1
};

class JSCSSValueListConstructor : public DOMObject {
public:
    JSCSSValueListConstructor(ExecState* exec)
    {
        setPrototype(exec->lexicalInterpreter()->builtinObjectPrototype());
        putDirect(exec->propertyNames().prototype, JSCSSValueListPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    JSValue* getValueProperty(ExecState*, int token) const;
    virtual const ClassInfo* classInfo() const { return &info; }
    static const ClassInfo info;

    virtual bool implementsHasInstance() const { return true; }
};

const ClassInfo JSCSSValueListConstructor::info = { "CSSValueListConstructor", 0, &JSCSSValueListConstructorTable, 0 };

bool JSCSSValueListConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSCSSValueListConstructor, DOMObject>(exec, &JSCSSValueListConstructorTable, this, propertyName, slot);
}

JSValue* JSCSSValueListConstructor::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

/* Hash table for prototype */

static const HashEntry JSCSSValueListPrototypeTableEntries[] =
{
    { "item", JSCSSValueList::ItemFuncNum, DontDelete|Function, 1, 0 }
};

static const HashTable JSCSSValueListPrototypeTable = 
{
    2, 1, JSCSSValueListPrototypeTableEntries, 1
};

const ClassInfo JSCSSValueListPrototype::info = { "CSSValueListPrototype", 0, &JSCSSValueListPrototypeTable, 0 };

JSObject* JSCSSValueListPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSCSSValueListPrototype>(exec, "[[JSCSSValueList.prototype]]");
}

bool JSCSSValueListPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSCSSValueListPrototypeFunction, JSObject>(exec, &JSCSSValueListPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSCSSValueList::info = { "CSSValueList", &JSCSSValue::info, &JSCSSValueListTable, 0 };

JSCSSValueList::JSCSSValueList(ExecState* exec, CSSValueList* impl)
    : JSCSSValue(exec, impl)
{
    setPrototype(JSCSSValueListPrototype::self(exec));
}

bool JSCSSValueList::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    const HashEntry* entry = Lookup::findEntry(&JSCSSValueListTable, propertyName);
    if (entry) {
        slot.setStaticEntry(this, entry, staticValueGetter<JSCSSValueList>);
        return true;
    }
    bool ok;
    unsigned index = propertyName.toUInt32(&ok, false);
    if (ok && index < static_cast<CSSValueList*>(impl())->length()) {
        slot.setCustomIndex(this, index, indexGetter);
        return true;
    }
    return JSCSSValue::getOwnPropertySlot(exec, propertyName, slot);
}

JSValue* JSCSSValueList::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case LengthAttrNum: {
        CSSValueList* imp = static_cast<CSSValueList*>(impl());

        return jsNumber(imp->length());
    }
    case ConstructorAttrNum:
        return getConstructor(exec);
    }
    return 0;
}

JSValue* JSCSSValueList::getConstructor(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSCSSValueListConstructor>(exec, "[[CSSValueList.constructor]]");
}
JSValue* JSCSSValueListPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSCSSValueList::info))
      return throwError(exec, TypeError);

    JSCSSValueList* castedThisObj = static_cast<JSCSSValueList*>(thisObj);
    CSSValueList* imp = static_cast<CSSValueList*>(castedThisObj->impl());

    switch (id) {
    case JSCSSValueList::ItemFuncNum: {
        bool indexOk;
        unsigned index = args[0]->toInt32(exec, indexOk);
        if (!indexOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }


        KJS::JSValue* result = toJS(exec, WTF::getPtr(imp->item(index)));
        return result;
    }
    }
    return 0;
}

JSValue* JSCSSValueList::indexGetter(ExecState* exec, JSObject* originalObject, const Identifier& propertyName, const PropertySlot& slot)
{
    JSCSSValueList* thisObj = static_cast<JSCSSValueList*>(slot.slotBase());
    return toJS(exec, static_cast<CSSValueList*>(thisObj->impl())->item(slot.index()));
}

}
