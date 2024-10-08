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
#include "JSSVGFEComponentTransferElement.h"

#include <wtf/GetPtr.h>

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSSVGAnimatedLength.h"
#include "JSSVGAnimatedString.h"
#include "SVGFEComponentTransferElement.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGFEComponentTransferElementTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "x", JSSVGFEComponentTransferElement::XAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEComponentTransferElementTableEntries[8] },
    { "className", JSSVGFEComponentTransferElement::ClassNameAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "width", JSSVGFEComponentTransferElement::WidthAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "y", JSSVGFEComponentTransferElement::YAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEComponentTransferElementTableEntries[10] },
    { 0, 0, 0, 0, 0 },
    { "in1", JSSVGFEComponentTransferElement::In1AttrNum, DontDelete|ReadOnly, 0, &JSSVGFEComponentTransferElementTableEntries[9] },
    { 0, 0, 0, 0, 0 },
    { "height", JSSVGFEComponentTransferElement::HeightAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "result", JSSVGFEComponentTransferElement::ResultAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "style", JSSVGFEComponentTransferElement::StyleAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSSVGFEComponentTransferElementTable = 
{
    2, 11, JSSVGFEComponentTransferElementTableEntries, 8
};

/* Hash table for prototype */

static const HashEntry JSSVGFEComponentTransferElementPrototypeTableEntries[] =
{
    { "getPresentationAttribute", JSSVGFEComponentTransferElement::GetPresentationAttributeFuncNum, DontDelete|Function, 1, 0 }
};

static const HashTable JSSVGFEComponentTransferElementPrototypeTable = 
{
    2, 1, JSSVGFEComponentTransferElementPrototypeTableEntries, 1
};

const ClassInfo JSSVGFEComponentTransferElementPrototype::info = { "SVGFEComponentTransferElementPrototype", 0, &JSSVGFEComponentTransferElementPrototypeTable, 0 };

JSObject* JSSVGFEComponentTransferElementPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGFEComponentTransferElementPrototype>(exec, "[[JSSVGFEComponentTransferElement.prototype]]");
}

bool JSSVGFEComponentTransferElementPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSSVGFEComponentTransferElementPrototypeFunction, JSObject>(exec, &JSSVGFEComponentTransferElementPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSSVGFEComponentTransferElement::info = { "SVGFEComponentTransferElement", &JSSVGElement::info, &JSSVGFEComponentTransferElementTable, 0 };

JSSVGFEComponentTransferElement::JSSVGFEComponentTransferElement(ExecState* exec, SVGFEComponentTransferElement* impl)
    : JSSVGElement(exec, impl)
{
    setPrototype(JSSVGFEComponentTransferElementPrototype::self(exec));
}

bool JSSVGFEComponentTransferElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGFEComponentTransferElement, JSSVGElement>(exec, &JSSVGFEComponentTransferElementTable, this, propertyName, slot);
}

JSValue* JSSVGFEComponentTransferElement::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case In1AttrNum: {
        SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->in1Animated();
        return toJS(exec, obj.get(), imp);
    }
    case XAttrNum: {
        SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case YAttrNum: {
        SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case WidthAttrNum: {
        SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case HeightAttrNum: {
        SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(impl());

        RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ResultAttrNum: {
        SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ClassNameAttrNum: {
        SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case StyleAttrNum: {
        SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(impl());

        return toJS(exec, WTF::getPtr(imp->style()));
    }
    }
    return 0;
}

JSValue* JSSVGFEComponentTransferElementPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSSVGFEComponentTransferElement::info))
      return throwError(exec, TypeError);

    JSSVGFEComponentTransferElement* castedThisObj = static_cast<JSSVGFEComponentTransferElement*>(thisObj);
    SVGFEComponentTransferElement* imp = static_cast<SVGFEComponentTransferElement*>(castedThisObj->impl());

    switch (id) {
    case JSSVGFEComponentTransferElement::GetPresentationAttributeFuncNum: {
        String name = args[0]->toString(exec);


        KJS::JSValue* result = toJS(exec, WTF::getPtr(imp->getPresentationAttribute(name)));
        return result;
    }
    }
    return 0;
}

}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
