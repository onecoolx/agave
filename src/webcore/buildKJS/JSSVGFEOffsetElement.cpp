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
#include "JSSVGFEOffsetElement.h"

#include <wtf/GetPtr.h>

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSSVGAnimatedLength.h"
#include "JSSVGAnimatedNumber.h"
#include "JSSVGAnimatedString.h"
#include "SVGFEOffsetElement.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGFEOffsetElementTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "dy", JSSVGFEOffsetElement::DyAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEOffsetElementTableEntries[11] },
    { 0, 0, 0, 0, 0 },
    { "x", JSSVGFEOffsetElement::XAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "dx", JSSVGFEOffsetElement::DxAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "in1", JSSVGFEOffsetElement::In1AttrNum, DontDelete|ReadOnly, 0, &JSSVGFEOffsetElementTableEntries[10] },
    { "height", JSSVGFEOffsetElement::HeightAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "result", JSSVGFEOffsetElement::ResultAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "y", JSSVGFEOffsetElement::YAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEOffsetElementTableEntries[12] },
    { "width", JSSVGFEOffsetElement::WidthAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "className", JSSVGFEOffsetElement::ClassNameAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEOffsetElementTableEntries[13] },
    { "style", JSSVGFEOffsetElement::StyleAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSSVGFEOffsetElementTable = 
{
    2, 14, JSSVGFEOffsetElementTableEntries, 10
};

/* Hash table for prototype */

static const HashEntry JSSVGFEOffsetElementPrototypeTableEntries[] =
{
    { "getPresentationAttribute", JSSVGFEOffsetElement::GetPresentationAttributeFuncNum, DontDelete|Function, 1, 0 }
};

static const HashTable JSSVGFEOffsetElementPrototypeTable = 
{
    2, 1, JSSVGFEOffsetElementPrototypeTableEntries, 1
};

const ClassInfo JSSVGFEOffsetElementPrototype::info = { "SVGFEOffsetElementPrototype", 0, &JSSVGFEOffsetElementPrototypeTable, 0 };

JSObject* JSSVGFEOffsetElementPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGFEOffsetElementPrototype>(exec, "[[JSSVGFEOffsetElement.prototype]]");
}

bool JSSVGFEOffsetElementPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSSVGFEOffsetElementPrototypeFunction, JSObject>(exec, &JSSVGFEOffsetElementPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSSVGFEOffsetElement::info = { "SVGFEOffsetElement", &JSSVGElement::info, &JSSVGFEOffsetElementTable, 0 };

JSSVGFEOffsetElement::JSSVGFEOffsetElement(ExecState* exec, SVGFEOffsetElement* impl)
    : JSSVGElement(exec, impl)
{
    setPrototype(JSSVGFEOffsetElementPrototype::self(exec));
}

bool JSSVGFEOffsetElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGFEOffsetElement, JSSVGElement>(exec, &JSSVGFEOffsetElementTable, this, propertyName, slot);
}

JSValue* JSSVGFEOffsetElement::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case In1AttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->in1Animated();
        return toJS(exec, obj.get(), imp);
    }
    case DxAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedNumber> obj = imp->dxAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case DyAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedNumber> obj = imp->dyAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case XAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case YAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case WidthAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case HeightAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ResultAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ClassNameAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case StyleAttrNum: {
        SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(impl());

        return toJS(exec, WTF::getPtr(imp->style()));
    }
    }
    return 0;
}

JSValue* JSSVGFEOffsetElementPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSSVGFEOffsetElement::info))
      return throwError(exec, TypeError);

    JSSVGFEOffsetElement* castedThisObj = static_cast<JSSVGFEOffsetElement*>(thisObj);
    SVGFEOffsetElement* imp = static_cast<SVGFEOffsetElement*>(castedThisObj->impl());

    switch (id) {
    case JSSVGFEOffsetElement::GetPresentationAttributeFuncNum: {
        String name = args[0]->toString(exec);


        KJS::JSValue* result = toJS(exec, WTF::getPtr(imp->getPresentationAttribute(name)));
        return result;
    }
    }
    return 0;
}

}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
