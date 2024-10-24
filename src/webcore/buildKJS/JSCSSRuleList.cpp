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

#include "JSCSSRuleList.h"

#include <wtf/GetPtr.h>

#include "CSSRule.h"
#include "CSSRuleList.h"
#include "ExceptionCode.h"
#include "JSCSSRule.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSCSSRuleListTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "length", JSCSSRuleList::LengthAttrNum, DontDelete|ReadOnly, 0, &JSCSSRuleListTableEntries[2] },
    { "constructor", JSCSSRuleList::ConstructorAttrNum, DontDelete|DontEnum|ReadOnly, 0, 0 }
};

static const HashTable JSCSSRuleListTable = 
{
    2, 3, JSCSSRuleListTableEntries, 2
};

/* Hash table for constructor */

static const HashEntry JSCSSRuleListConstructorTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSCSSRuleListConstructorTable = 
{
    2, 1, JSCSSRuleListConstructorTableEntries, 1
};

class JSCSSRuleListConstructor : public DOMObject {
public:
    JSCSSRuleListConstructor(ExecState* exec)
    {
        setPrototype(exec->lexicalInterpreter()->builtinObjectPrototype());
        putDirect(exec->propertyNames().prototype, JSCSSRuleListPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    JSValue* getValueProperty(ExecState*, int token) const;
    virtual const ClassInfo* classInfo() const { return &info; }
    static const ClassInfo info;

    virtual bool implementsHasInstance() const { return true; }
};

const ClassInfo JSCSSRuleListConstructor::info = { "CSSRuleListConstructor", 0, &JSCSSRuleListConstructorTable, 0 };

bool JSCSSRuleListConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSCSSRuleListConstructor, DOMObject>(exec, &JSCSSRuleListConstructorTable, this, propertyName, slot);
}

JSValue* JSCSSRuleListConstructor::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

/* Hash table for prototype */

static const HashEntry JSCSSRuleListPrototypeTableEntries[] =
{
    { "item", JSCSSRuleList::ItemFuncNum, DontDelete|Function, 1, 0 }
};

static const HashTable JSCSSRuleListPrototypeTable = 
{
    2, 1, JSCSSRuleListPrototypeTableEntries, 1
};

const ClassInfo JSCSSRuleListPrototype::info = { "CSSRuleListPrototype", 0, &JSCSSRuleListPrototypeTable, 0 };

JSObject* JSCSSRuleListPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSCSSRuleListPrototype>(exec, "[[JSCSSRuleList.prototype]]");
}

bool JSCSSRuleListPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSCSSRuleListPrototypeFunction, JSObject>(exec, &JSCSSRuleListPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSCSSRuleList::info = { "CSSRuleList", 0, &JSCSSRuleListTable, 0 };

JSCSSRuleList::JSCSSRuleList(ExecState* exec, CSSRuleList* impl)
    : m_impl(impl)
{
    setPrototype(JSCSSRuleListPrototype::self(exec));
}

JSCSSRuleList::~JSCSSRuleList()
{
    ScriptInterpreter::forgetDOMObject(m_impl.get());

}

bool JSCSSRuleList::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    const HashEntry* entry = Lookup::findEntry(&JSCSSRuleListTable, propertyName);
    if (entry) {
        slot.setStaticEntry(this, entry, staticValueGetter<JSCSSRuleList>);
        return true;
    }
    bool ok;
    unsigned index = propertyName.toUInt32(&ok, false);
    if (ok && index < static_cast<CSSRuleList*>(impl())->length()) {
        slot.setCustomIndex(this, index, indexGetter);
        return true;
    }
    return KJS::DOMObject::getOwnPropertySlot(exec, propertyName, slot);
}

JSValue* JSCSSRuleList::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case LengthAttrNum: {
        CSSRuleList* imp = static_cast<CSSRuleList*>(impl());

        return jsNumber(imp->length());
    }
    case ConstructorAttrNum:
        return getConstructor(exec);
    }
    return 0;
}

JSValue* JSCSSRuleList::getConstructor(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSCSSRuleListConstructor>(exec, "[[CSSRuleList.constructor]]");
}
JSValue* JSCSSRuleListPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSCSSRuleList::info))
      return throwError(exec, TypeError);

    JSCSSRuleList* castedThisObj = static_cast<JSCSSRuleList*>(thisObj);
    CSSRuleList* imp = static_cast<CSSRuleList*>(castedThisObj->impl());

    switch (id) {
    case JSCSSRuleList::ItemFuncNum: {
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

JSValue* JSCSSRuleList::indexGetter(ExecState* exec, JSObject* originalObject, const Identifier& propertyName, const PropertySlot& slot)
{
    JSCSSRuleList* thisObj = static_cast<JSCSSRuleList*>(slot.slotBase());
    return toJS(exec, static_cast<CSSRuleList*>(thisObj->impl())->item(slot.index()));
}
KJS::JSValue* toJS(KJS::ExecState* exec, CSSRuleList* obj)
{
    return KJS::cacheDOMObject<CSSRuleList, JSCSSRuleList>(exec, obj);
}
CSSRuleList* toCSSRuleList(KJS::JSValue* val)
{
    return val->isObject(&JSCSSRuleList::info) ? static_cast<JSCSSRuleList*>(val)->impl() : 0;
}

}
