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


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "JSSVGStopElement.h"

#include <wtf/GetPtr.h>

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSSVGAnimatedNumber.h"
#include "JSSVGAnimatedString.h"
#include "SVGStopElement.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGStopElementTableEntries[] =
{
    { "className", JSSVGStopElement::ClassNameAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "offset", JSSVGStopElement::OffsetAttrNum, DontDelete|ReadOnly, 0, &JSSVGStopElementTableEntries[3] },
    { "style", JSSVGStopElement::StyleAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSSVGStopElementTable = 
{
    2, 4, JSSVGStopElementTableEntries, 3
};

/* Hash table for prototype */

static const HashEntry JSSVGStopElementPrototypeTableEntries[] =
{
    { "getPresentationAttribute", JSSVGStopElement::GetPresentationAttributeFuncNum, DontDelete|Function, 1, 0 }
};

static const HashTable JSSVGStopElementPrototypeTable = 
{
    2, 1, JSSVGStopElementPrototypeTableEntries, 1
};

const ClassInfo JSSVGStopElementPrototype::info = { "SVGStopElementPrototype", 0, &JSSVGStopElementPrototypeTable, 0 };

JSObject* JSSVGStopElementPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGStopElementPrototype>(exec, "[[JSSVGStopElement.prototype]]");
}

bool JSSVGStopElementPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSSVGStopElementPrototypeFunction, JSObject>(exec, &JSSVGStopElementPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSSVGStopElement::info = { "SVGStopElement", &JSSVGElement::info, &JSSVGStopElementTable, 0 };

JSSVGStopElement::JSSVGStopElement(ExecState* exec, SVGStopElement* impl)
    : JSSVGElement(exec, impl)
{
    setPrototype(JSSVGStopElementPrototype::self(exec));
}

bool JSSVGStopElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGStopElement, JSSVGElement>(exec, &JSSVGStopElementTable, this, propertyName, slot);
}

JSValue* JSSVGStopElement::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case OffsetAttrNum: {
        SVGStopElement* imp = static_cast<SVGStopElement*>(impl());

        RefPtr<SVGAnimatedNumber> obj = imp->offsetAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ClassNameAttrNum: {
        SVGStopElement* imp = static_cast<SVGStopElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case StyleAttrNum: {
        SVGStopElement* imp = static_cast<SVGStopElement*>(impl());

        return toJS(exec, WTF::getPtr(imp->style()));
    }
    }
    return 0;
}

JSValue* JSSVGStopElementPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSSVGStopElement::info))
      return throwError(exec, TypeError);

    JSSVGStopElement* castedThisObj = static_cast<JSSVGStopElement*>(thisObj);
    SVGStopElement* imp = static_cast<SVGStopElement*>(castedThisObj->impl());

    switch (id) {
    case JSSVGStopElement::GetPresentationAttributeFuncNum: {
        String name = args[0]->toString(exec);


        KJS::JSValue* result = toJS(exec, WTF::getPtr(imp->getPresentationAttribute(name)));
        return result;
    }
    }
    return 0;
}

}

#endif // ENABLE(SVG)
