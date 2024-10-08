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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "JSSVGFEFloodElement.h"

#include <wtf/GetPtr.h>

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSSVGAnimatedLength.h"
#include "JSSVGAnimatedString.h"
#include "SVGFEFloodElement.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGFEFloodElementTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "x", JSSVGFEFloodElement::XAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEFloodElementTableEntries[8] },
    { "className", JSSVGFEFloodElement::ClassNameAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "width", JSSVGFEFloodElement::WidthAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "y", JSSVGFEFloodElement::YAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEFloodElementTableEntries[10] },
    { 0, 0, 0, 0, 0 },
    { "in1", JSSVGFEFloodElement::In1AttrNum, DontDelete|ReadOnly, 0, &JSSVGFEFloodElementTableEntries[9] },
    { 0, 0, 0, 0, 0 },
    { "height", JSSVGFEFloodElement::HeightAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "result", JSSVGFEFloodElement::ResultAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "style", JSSVGFEFloodElement::StyleAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSSVGFEFloodElementTable = 
{
    2, 11, JSSVGFEFloodElementTableEntries, 8
};

/* Hash table for prototype */

static const HashEntry JSSVGFEFloodElementPrototypeTableEntries[] =
{
    { "getPresentationAttribute", JSSVGFEFloodElement::GetPresentationAttributeFuncNum, DontDelete|Function, 1, 0 }
};

static const HashTable JSSVGFEFloodElementPrototypeTable = 
{
    2, 1, JSSVGFEFloodElementPrototypeTableEntries, 1
};

const ClassInfo JSSVGFEFloodElementPrototype::info = { "SVGFEFloodElementPrototype", 0, &JSSVGFEFloodElementPrototypeTable, 0 };

JSObject* JSSVGFEFloodElementPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGFEFloodElementPrototype>(exec, "[[JSSVGFEFloodElement.prototype]]");
}

bool JSSVGFEFloodElementPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSSVGFEFloodElementPrototypeFunction, JSObject>(exec, &JSSVGFEFloodElementPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSSVGFEFloodElement::info = { "SVGFEFloodElement", &JSSVGElement::info, &JSSVGFEFloodElementTable, 0 };

JSSVGFEFloodElement::JSSVGFEFloodElement(ExecState* exec, SVGFEFloodElement* impl)
    : JSSVGElement(exec, impl)
{
    setPrototype(JSSVGFEFloodElementPrototype::self(exec));
}

bool JSSVGFEFloodElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGFEFloodElement, JSSVGElement>(exec, &JSSVGFEFloodElementTable, this, propertyName, slot);
}

JSValue* JSSVGFEFloodElement::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case In1AttrNum: {
        SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->in1Animated();
        return toJS(exec, obj.get(), imp);
    }
    case XAttrNum: {
        SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case YAttrNum: {
        SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case WidthAttrNum: {
        SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case HeightAttrNum: {
        SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ResultAttrNum: {
        SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ClassNameAttrNum: {
        SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case StyleAttrNum: {
        SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(impl());

        return toJS(exec, WTF::getPtr(imp->style()));
    }
    }
    return 0;
}

JSValue* JSSVGFEFloodElementPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSSVGFEFloodElement::info))
      return throwError(exec, TypeError);

    JSSVGFEFloodElement* castedThisObj = static_cast<JSSVGFEFloodElement*>(thisObj);
    SVGFEFloodElement* imp = static_cast<SVGFEFloodElement*>(castedThisObj->impl());

    switch (id) {
    case JSSVGFEFloodElement::GetPresentationAttributeFuncNum: {
        String name = args[0]->toString(exec);


        KJS::JSValue* result = toJS(exec, WTF::getPtr(imp->getPresentationAttribute(name)));
        return result;
    }
    }
    return 0;
}

}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
