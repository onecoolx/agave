/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2006 Rob Buis <buis@kde.org>

    This file is part of the KDE project

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
#include "SVGFEMergeNodeElement.h"

namespace WebCore {

SVGFEMergeNodeElement::SVGFEMergeNodeElement(const QualifiedName& tagName, Document* doc)
    : SVGElement(tagName, doc)
{
}

SVGFEMergeNodeElement::~SVGFEMergeNodeElement()
{
}

ANIMATED_PROPERTY_DEFINITIONS(SVGFEMergeNodeElement, String, String, string, In1, in1, SVGNames::inAttr.localName(), m_in1)

void SVGFEMergeNodeElement::parseMappedAttribute(MappedAttribute* attr)
{
    const String& value = attr->value();
    if (attr->name() == SVGNames::inAttr)
        setIn1BaseValue(value);
    else
        SVGElement::parseMappedAttribute(attr);
}

}

#endif // ENABLE(SVG)

// vim:ts=4:noet
