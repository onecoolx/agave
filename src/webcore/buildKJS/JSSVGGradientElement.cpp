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
#include "JSSVGGradientElement.h"

#include <wtf/GetPtr.h>

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSSVGAnimatedBoolean.h"
#include "JSSVGAnimatedEnumeration.h"
#include "JSSVGAnimatedString.h"
#include "JSSVGAnimatedTransformList.h"
#include "SVGGradientElement.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGGradientElementTableEntries[] =
{
    { "href", JSSVGGradientElement::HrefAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "externalResourcesRequired", JSSVGGradientElement::ExternalResourcesRequiredAttrNum, DontDelete|ReadOnly, 0, &JSSVGGradientElementTableEntries[8] },
    { "className", JSSVGGradientElement::ClassNameAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "style", JSSVGGradientElement::StyleAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "spreadMethod", JSSVGGradientElement::SpreadMethodAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "gradientUnits", JSSVGGradientElement::GradientUnitsAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "gradientTransform", JSSVGGradientElement::GradientTransformAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "constructor", JSSVGGradientElement::ConstructorAttrNum, DontDelete|DontEnum|ReadOnly, 0, 0 }
};

static const HashTable JSSVGGradientElementTable = 
{
    2, 9, JSSVGGradientElementTableEntries, 8
};

/* Hash table for constructor */

static const HashEntry JSSVGGradientElementConstructorTableEntries[] =
{
    { "SVG_SPREADMETHOD_UNKNOWN", SVGGradientElement::SVG_SPREADMETHOD_UNKNOWN, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "SVG_SPREADMETHOD_REPEAT", SVGGradientElement::SVG_SPREADMETHOD_REPEAT, DontDelete|ReadOnly, 0, 0 },
    { "SVG_SPREADMETHOD_PAD", SVGGradientElement::SVG_SPREADMETHOD_PAD, DontDelete|ReadOnly, 0, &JSSVGGradientElementConstructorTableEntries[4] },
    { "SVG_SPREADMETHOD_REFLECT", SVGGradientElement::SVG_SPREADMETHOD_REFLECT, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSSVGGradientElementConstructorTable = 
{
    2, 5, JSSVGGradientElementConstructorTableEntries, 4
};

class JSSVGGradientElementConstructor : public DOMObject {
public:
    JSSVGGradientElementConstructor(ExecState* exec)
    {
        setPrototype(exec->lexicalInterpreter()->builtinObjectPrototype());
        putDirect(exec->propertyNames().prototype, JSSVGGradientElementPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    JSValue* getValueProperty(ExecState*, int token) const;
    virtual const ClassInfo* classInfo() const { return &info; }
    static const ClassInfo info;

    virtual bool implementsHasInstance() const { return true; }
};

const ClassInfo JSSVGGradientElementConstructor::info = { "SVGGradientElementConstructor", 0, &JSSVGGradientElementConstructorTable, 0 };

bool JSSVGGradientElementConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGGradientElementConstructor, DOMObject>(exec, &JSSVGGradientElementConstructorTable, this, propertyName, slot);
}

JSValue* JSSVGGradientElementConstructor::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

/* Hash table for prototype */

static const HashEntry JSSVGGradientElementPrototypeTableEntries[] =
{
    { "SVG_SPREADMETHOD_REFLECT", SVGGradientElement::SVG_SPREADMETHOD_REFLECT, DontDelete|ReadOnly, 0, 0 },
    { "SVG_SPREADMETHOD_UNKNOWN", SVGGradientElement::SVG_SPREADMETHOD_UNKNOWN, DontDelete|ReadOnly, 0, &JSSVGGradientElementPrototypeTableEntries[5] },
    { 0, 0, 0, 0, 0 },
    { "SVG_SPREADMETHOD_REPEAT", SVGGradientElement::SVG_SPREADMETHOD_REPEAT, DontDelete|ReadOnly, 0, 0 },
    { "SVG_SPREADMETHOD_PAD", SVGGradientElement::SVG_SPREADMETHOD_PAD, DontDelete|ReadOnly, 0, 0 },
    { "getPresentationAttribute", JSSVGGradientElement::GetPresentationAttributeFuncNum, DontDelete|Function, 1, 0 }
};

static const HashTable JSSVGGradientElementPrototypeTable = 
{
    2, 6, JSSVGGradientElementPrototypeTableEntries, 5
};

const ClassInfo JSSVGGradientElementPrototype::info = { "SVGGradientElementPrototype", 0, &JSSVGGradientElementPrototypeTable, 0 };

JSObject* JSSVGGradientElementPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGGradientElementPrototype>(exec, "[[JSSVGGradientElement.prototype]]");
}

bool JSSVGGradientElementPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticPropertySlot<JSSVGGradientElementPrototypeFunction, JSSVGGradientElementPrototype, JSObject>(exec, &JSSVGGradientElementPrototypeTable, this, propertyName, slot);
}

JSValue* JSSVGGradientElementPrototype::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

const ClassInfo JSSVGGradientElement::info = { "SVGGradientElement", &JSSVGElement::info, &JSSVGGradientElementTable, 0 };

JSSVGGradientElement::JSSVGGradientElement(ExecState* exec, SVGGradientElement* impl)
    : JSSVGElement(exec, impl)
{
    setPrototype(JSSVGGradientElementPrototype::self(exec));
}

bool JSSVGGradientElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGGradientElement, JSSVGElement>(exec, &JSSVGGradientElementTable, this, propertyName, slot);
}

JSValue* JSSVGGradientElement::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case GradientUnitsAttrNum: {
        SVGGradientElement* imp = static_cast<SVGGradientElement*>(impl());

        RefPtr<SVGAnimatedEnumeration> obj = imp->gradientUnitsAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case GradientTransformAttrNum: {
        SVGGradientElement* imp = static_cast<SVGGradientElement*>(impl());

        RefPtr<SVGAnimatedTransformList> obj = imp->gradientTransformAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case SpreadMethodAttrNum: {
        SVGGradientElement* imp = static_cast<SVGGradientElement*>(impl());

        RefPtr<SVGAnimatedEnumeration> obj = imp->spreadMethodAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case HrefAttrNum: {
        SVGGradientElement* imp = static_cast<SVGGradientElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ExternalResourcesRequiredAttrNum: {
        SVGGradientElement* imp = static_cast<SVGGradientElement*>(impl());

        RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case ClassNameAttrNum: {
        SVGGradientElement* imp = static_cast<SVGGradientElement*>(impl());

        RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
        return toJS(exec, obj.get(), imp);
    }
    case StyleAttrNum: {
        SVGGradientElement* imp = static_cast<SVGGradientElement*>(impl());

        return toJS(exec, WTF::getPtr(imp->style()));
    }
    case ConstructorAttrNum:
        return getConstructor(exec);
    }
    return 0;
}

JSValue* JSSVGGradientElement::getConstructor(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGGradientElementConstructor>(exec, "[[SVGGradientElement.constructor]]");
}
JSValue* JSSVGGradientElementPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSSVGGradientElement::info))
      return throwError(exec, TypeError);

    JSSVGGradientElement* castedThisObj = static_cast<JSSVGGradientElement*>(thisObj);
    SVGGradientElement* imp = static_cast<SVGGradientElement*>(castedThisObj->impl());

    switch (id) {
    case JSSVGGradientElement::GetPresentationAttributeFuncNum: {
        String name = args[0]->toString(exec);


        KJS::JSValue* result = toJS(exec, WTF::getPtr(imp->getPresentationAttribute(name)));
        return result;
    }
    }
    return 0;
}

}

#endif // ENABLE(SVG)
