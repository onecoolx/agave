/*
    Copyright (C) 2004, 2005, 2006, 2007 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2006, 2007 Rob Buis <buis@kde.org>

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

#if ENABLE(SVG)
#include "SVGPatternElement.h"

#include "AffineTransform.h"
#include "Document.h"
#include "FloatConversion.h"
#include "GraphicsContext.h"
#include "ImageBuffer.h"
#include "PatternAttributes.h"
#include "RenderSVGContainer.h"
#include "SVGLength.h"
#include "SVGNames.h"
#include "SVGPaintServerPattern.h"
#include "SVGRenderSupport.h"
#include "SVGStyledTransformableElement.h"
#include "SVGSVGElement.h"
#include "SVGTransformList.h"
#include "SVGTransformable.h"
#include "SVGUnitTypes.h"

#include <math.h>
#include <wtf/OwnPtr.h>
#include <wtf/MathExtras.h>

using namespace std;

namespace WebCore {

SVGPatternElement::SVGPatternElement(const QualifiedName& tagName, Document* doc)
    : SVGStyledElement(tagName, doc)
    , SVGURIReference()
    , SVGTests()
    , SVGLangSpace()
    , SVGExternalResourcesRequired()
    , SVGFitToViewBox()
    , m_x(this, LengthModeWidth)
    , m_y(this, LengthModeHeight)
    , m_width(this, LengthModeWidth)
    , m_height(this, LengthModeHeight)
    , m_patternUnits(SVGUnitTypes::SVG_UNIT_TYPE_OBJECTBOUNDINGBOX)
    , m_patternContentUnits(SVGUnitTypes::SVG_UNIT_TYPE_USERSPACEONUSE)
    , m_patternTransform(new SVGTransformList())
{
}

SVGPatternElement::~SVGPatternElement()
{
}

ANIMATED_PROPERTY_DEFINITIONS(SVGPatternElement, int, Enumeration, enumeration, PatternUnits, patternUnits, SVGNames::patternUnitsAttr.localName(), m_patternUnits)
ANIMATED_PROPERTY_DEFINITIONS(SVGPatternElement, int, Enumeration, enumeration, PatternContentUnits, patternContentUnits, SVGNames::patternContentUnitsAttr.localName(), m_patternContentUnits)
ANIMATED_PROPERTY_DEFINITIONS(SVGPatternElement, SVGLength, Length, length, X, x, SVGNames::xAttr.localName(), m_x)
ANIMATED_PROPERTY_DEFINITIONS(SVGPatternElement, SVGLength, Length, length, Y, y, SVGNames::yAttr.localName(), m_y)
ANIMATED_PROPERTY_DEFINITIONS(SVGPatternElement, SVGLength, Length, length, Width, width, SVGNames::widthAttr.localName(), m_width)
ANIMATED_PROPERTY_DEFINITIONS(SVGPatternElement, SVGLength, Length, length, Height, height, SVGNames::heightAttr.localName(), m_height)
ANIMATED_PROPERTY_DEFINITIONS(SVGPatternElement, SVGTransformList*, TransformList, transformList, PatternTransform, patternTransform, SVGNames::patternTransformAttr.localName(), m_patternTransform.get())

void SVGPatternElement::parseMappedAttribute(MappedAttribute* attr)
{
    if (attr->name() == SVGNames::patternUnitsAttr) {
        if (attr->value() == "userSpaceOnUse")
            setPatternUnitsBaseValue(SVGUnitTypes::SVG_UNIT_TYPE_USERSPACEONUSE);
        else if (attr->value() == "objectBoundingBox")
            setPatternUnitsBaseValue(SVGUnitTypes::SVG_UNIT_TYPE_OBJECTBOUNDINGBOX);
    } else if (attr->name() == SVGNames::patternContentUnitsAttr) {
        if (attr->value() == "userSpaceOnUse")
            setPatternContentUnitsBaseValue(SVGUnitTypes::SVG_UNIT_TYPE_USERSPACEONUSE);
        else if (attr->value() == "objectBoundingBox")
            setPatternContentUnitsBaseValue(SVGUnitTypes::SVG_UNIT_TYPE_OBJECTBOUNDINGBOX);
    } else if (attr->name() == SVGNames::patternTransformAttr) {
        SVGTransformList* patternTransforms = patternTransformBaseValue();
        if (!SVGTransformable::parseTransformAttribute(patternTransforms, attr->value())) {
            ExceptionCode ec = 0;
            patternTransforms->clear(ec);
        }
    } else if (attr->name() == SVGNames::xAttr)
        setXBaseValue(SVGLength(this, LengthModeWidth, attr->value()));
    else if (attr->name() == SVGNames::yAttr)
        setYBaseValue(SVGLength(this, LengthModeHeight, attr->value()));
    else if (attr->name() == SVGNames::widthAttr) {
        setWidthBaseValue(SVGLength(this, LengthModeWidth, attr->value()));
        if (width().value() < 0.0)
            document()->accessSVGExtensions()->reportError("A negative value for pattern attribute <width> is not allowed");
    } else if (attr->name() == SVGNames::heightAttr) {
        setHeightBaseValue(SVGLength(this, LengthModeHeight, attr->value()));
        if (width().value() < 0.0)
            document()->accessSVGExtensions()->reportError("A negative value for pattern attribute <height> is not allowed");
    } else {
        if (SVGURIReference::parseMappedAttribute(attr))
            return;
        if (SVGTests::parseMappedAttribute(attr))
            return;
        if (SVGLangSpace::parseMappedAttribute(attr))
            return;
        if (SVGExternalResourcesRequired::parseMappedAttribute(attr))
            return;
        if (SVGFitToViewBox::parseMappedAttribute(attr))
            return;

        SVGStyledElement::parseMappedAttribute(attr);
    }
}

void SVGPatternElement::buildPattern(const FloatRect& targetRect) const
{
    PatternAttributes attributes = collectPatternProperties();

    // If we didn't find any pattern content, ignore the request.
    if (!attributes.patternContentElement() || !renderer() || !renderer()->style())
        return;

    FloatRect patternBoundaries; 
    FloatRect patternContentBoundaries;

    // Determine specified pattern size
    if (attributes.boundingBoxMode())
        patternBoundaries = FloatRect(attributes.x().valueAsPercentage() * targetRect.width(),
                                      attributes.y().valueAsPercentage() * targetRect.height(),
                                      attributes.width().valueAsPercentage() * targetRect.width(),
                                      attributes.height().valueAsPercentage() * targetRect.height());
    else
        patternBoundaries = FloatRect(attributes.x().value(),
                                      attributes.y().value(),
                                      attributes.width().value(),
                                      attributes.height().value());

    // Clip pattern boundaries to target boundaries
    if (patternBoundaries.width() > targetRect.width())
        patternBoundaries.setWidth(targetRect.width());

    if (patternBoundaries.height() > targetRect.height())
        patternBoundaries.setHeight(targetRect.height());

    // Eventually calculate the pattern content boundaries (only needed with overflow="visible").
    RenderStyle* style = renderer()->style();
    if (style->overflowX() == OVISIBLE && style->overflowY() == OVISIBLE) {
        for (Node* n = attributes.patternContentElement()->firstChild(); n; n = n->nextSibling()) {
            SVGElement* elem = svg_dynamic_cast(n);
            if (!elem || !elem->isStyledTransformable())
                continue;

            SVGStyledElement* e = static_cast<SVGStyledElement*>(elem);
            RenderObject* item = e->renderer();
            if (!item)
                continue;

            patternContentBoundaries.unite(item->relativeBBox(true));
        }
    }

    AffineTransform viewBoxCTM = viewBoxToViewTransform(patternBoundaries.width(), patternBoundaries.height()); 
    FloatRect patternBoundariesIncludingOverflow = patternBoundaries;

    // Apply objectBoundingBoxMode fixup for patternContentUnits, if viewBox is not set.
    if (!patternContentBoundaries.isEmpty()) {
        if (!viewBoxCTM.isIdentity())
            patternContentBoundaries = viewBoxCTM.mapRect(patternContentBoundaries);
        else if (attributes.boundingBoxModeContent())
            patternContentBoundaries = FloatRect(patternContentBoundaries.x() * targetRect.width(),
                                                 patternContentBoundaries.y() * targetRect.height(),
                                                 patternContentBoundaries.width() * targetRect.width(),
                                                 patternContentBoundaries.height() * targetRect.height());

        patternBoundariesIncludingOverflow.unite(patternContentBoundaries);
    }

    auto_ptr<ImageBuffer> patternImage = ImageBuffer::create(IntSize(lroundf(patternBoundariesIncludingOverflow.width()),
                                                                     lroundf(patternBoundariesIncludingOverflow.height())), false);

    if (!patternImage.get())
        return;

    GraphicsContext* context = patternImage->context();
    ASSERT(context);

    context->save();

    // Move to pattern start origin
    if (patternBoundariesIncludingOverflow.location() != patternBoundaries.location()) {
        context->translate(patternBoundaries.x() - patternBoundariesIncludingOverflow.x(),
                           patternBoundaries.y() - patternBoundariesIncludingOverflow.y());

        patternBoundaries.setLocation(patternBoundariesIncludingOverflow.location());
    }

    // Process viewBox or boundingBoxModeContent correction
    if (!viewBoxCTM.isIdentity())
        context->concatCTM(viewBoxCTM);
    else if (attributes.boundingBoxModeContent()) {
        context->translate(targetRect.x(), targetRect.y());
        context->scale(FloatSize(targetRect.width(), targetRect.height()));
    }

    // Render subtree into ImageBuffer
    for (Node* n = attributes.patternContentElement()->firstChild(); n; n = n->nextSibling()) {
        SVGElement* elem = svg_dynamic_cast(n);
        if (!elem || !elem->isStyled())
            continue;

        SVGStyledElement* e = static_cast<SVGStyledElement*>(elem);
        RenderObject* item = e->renderer();
        if (!item)
            continue;

        renderSubtreeToImage(patternImage.get(), item);
    }

    context->restore();

    m_resource->setPatternTransform(attributes.patternTransform());
    m_resource->setPatternBoundaries(patternBoundaries); 
    m_resource->setTile(patternImage);
}

void SVGPatternElement::notifyAttributeChange() const
{
    if (!m_resource || !attached() || document()->parsing())
        return;

    m_resource->invalidate();
    m_resource->repaintClients();
}

RenderObject* SVGPatternElement::createRenderer(RenderArena* arena, RenderStyle*)
{
    RenderSVGContainer* patternContainer = new (arena) RenderSVGContainer(this);
    patternContainer->setDrawsContents(false);
    return patternContainer;
}

SVGResource* SVGPatternElement::canvasResource()
{
    if (!m_resource)
        m_resource = new SVGPaintServerPattern(this);

    return m_resource.get();
}

PatternAttributes SVGPatternElement::collectPatternProperties() const
{
    PatternAttributes attributes;
    HashSet<const SVGPatternElement*> processedPatterns;

    const SVGPatternElement* current = this;
    while (current) {
        if (!attributes.hasX() && current->hasAttribute(SVGNames::xAttr))
            attributes.setX(current->x());

        if (!attributes.hasY() && current->hasAttribute(SVGNames::yAttr))
            attributes.setY(current->y());

        if (!attributes.hasWidth() && current->hasAttribute(SVGNames::widthAttr))
            attributes.setWidth(current->width());

        if (!attributes.hasHeight() && current->hasAttribute(SVGNames::heightAttr))
            attributes.setHeight(current->height());

        if (!attributes.hasBoundingBoxMode() && current->hasAttribute(SVGNames::patternUnitsAttr))
            attributes.setBoundingBoxMode(current->getAttribute(SVGNames::patternUnitsAttr) == "objectBoundingBox");

        if (!attributes.hasBoundingBoxModeContent() && current->hasAttribute(SVGNames::patternContentUnitsAttr))
            attributes.setBoundingBoxModeContent(current->getAttribute(SVGNames::patternContentUnitsAttr) == "objectBoundingBox");

        if (!attributes.hasPatternTransform() && current->hasAttribute(SVGNames::patternTransformAttr))
            attributes.setPatternTransform(current->patternTransform()->consolidate().matrix());

        if (!attributes.hasPatternContentElement() && current->hasChildNodes())
            attributes.setPatternContentElement(current);

        processedPatterns.add(current);

        // Respect xlink:href, take attributes from referenced element
        Node* refNode = ownerDocument()->getElementById(SVGURIReference::getTarget(current->href()));
        if (refNode && refNode->hasTagName(SVGNames::patternTag)) {
            current = static_cast<const SVGPatternElement*>(const_cast<const Node*>(refNode));

            // Cycle detection
            if (processedPatterns.contains(current))
                return PatternAttributes();
        } else
            current = 0;
    }

    return attributes;
}

}

#endif // ENABLE(SVG)

// vim:ts=4:noet
