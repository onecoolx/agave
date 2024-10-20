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
#include "JSSVGAnimatedLength.h"

#include <wtf/GetPtr.h>

#include "JSSVGLength.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGAnimatedLengthTableEntries[] =
{
    { "baseVal", JSSVGAnimatedLength::BaseValAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "animVal", JSSVGAnimatedLength::AnimValAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSSVGAnimatedLengthTable = 
{
    2, 2, JSSVGAnimatedLengthTableEntries, 2
};

/* Hash table for prototype */

static const HashEntry JSSVGAnimatedLengthPrototypeTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSSVGAnimatedLengthPrototypeTable = 
{
    2, 1, JSSVGAnimatedLengthPrototypeTableEntries, 1
};

const ClassInfo JSSVGAnimatedLengthPrototype::info = { "SVGAnimatedLengthPrototype", 0, &JSSVGAnimatedLengthPrototypeTable, 0 };

JSObject* JSSVGAnimatedLengthPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGAnimatedLengthPrototype>(exec, "[[JSSVGAnimatedLength.prototype]]");
}

const ClassInfo JSSVGAnimatedLength::info = { "SVGAnimatedLength", 0, &JSSVGAnimatedLengthTable, 0 };

JSSVGAnimatedLength::JSSVGAnimatedLength(ExecState* exec, SVGAnimatedLength* impl, SVGElement* context)
    : m_context(context)
    , m_impl(impl)
{
    setPrototype(JSSVGAnimatedLengthPrototype::self(exec));
}

JSSVGAnimatedLength::~JSSVGAnimatedLength()
{
    ScriptInterpreter::forgetDOMObject(m_impl.get());

}

bool JSSVGAnimatedLength::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGAnimatedLength, KJS::DOMObject>(exec, &JSSVGAnimatedLengthTable, this, propertyName, slot);
}

JSValue* JSSVGAnimatedLength::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case BaseValAttrNum: {
        SVGAnimatedLength* imp = static_cast<SVGAnimatedLength*>(impl());

        return toJS(exec, JSSVGPODTypeWrapperCache<SVGLength, SVGAnimatedLength>::lookupOrCreateWrapper(imp, &SVGAnimatedLength::baseVal, &SVGAnimatedLength::setBaseVal), context());
    }
    case AnimValAttrNum: {
        SVGAnimatedLength* imp = static_cast<SVGAnimatedLength*>(impl());

        return toJS(exec, JSSVGPODTypeWrapperCache<SVGLength, SVGAnimatedLength>::lookupOrCreateWrapper(imp, &SVGAnimatedLength::animVal, &SVGAnimatedLength::setAnimVal), context());
    }
    }
    return 0;
}

KJS::JSValue* toJS(KJS::ExecState* exec, SVGAnimatedLength* obj, SVGElement* context)
{
    return KJS::cacheSVGDOMObject<SVGAnimatedLength, JSSVGAnimatedLength>(exec, obj, context);
}
SVGAnimatedLength* toSVGAnimatedLength(KJS::JSValue* val)
{
    return val->isObject(&JSSVGAnimatedLength::info) ? static_cast<JSSVGAnimatedLength*>(val)->impl() : 0;
}

}

#endif // ENABLE(SVG)
