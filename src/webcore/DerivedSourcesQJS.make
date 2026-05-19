# Copyright (C) 2024 Zhang Ji Peng. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
# 3.  Neither the name of the copyright holder nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# QuickJS Binding Code Generator
# Usage: make -C buildQJS -f ../DerivedSourcesQJS.make WebCore=..

VPATH = \
    $(WebCore) \
    $(WebCore)/bindings/qjs \
    $(WebCore)/css \
    $(WebCore)/dom \
    $(WebCore)/html \
    $(WebCore)/page \
    $(WebCore)/xml \
    $(WebCore)/ksvg2/svg \
    $(WebCore)/ksvg2/events \
#

FEATURE_DEFINES = ENABLE_XPATH ENABLE_XSLT

.PHONY : all

all : \
    QJSAttr.h \
    QJSBarInfo.h \
    QJSCDATASection.h \
    QJSCSSCharsetRule.h \
    QJSCSSFontFaceRule.h \
    QJSCSSImportRule.h \
    QJSCSSMediaRule.h \
    QJSCSSPageRule.h \
    QJSCSSPrimitiveValue.h \
    QJSCSSRule.h \
    QJSCSSRuleList.h \
    QJSCSSStyleRule.h \
    QJSCSSStyleSheet.h \
    QJSCSSValue.h \
    QJSCSSValueList.h \
    QJSCanvasGradient.h \
    QJSCanvasPattern.h \
    QJSCanvasRenderingContext2D.h \
    QJSCharacterData.h \
    QJSComment.h \
    QJSCounter.h \
    QJSCSSStyleDeclaration.h \
    QJSDOMImplementation.h \
    QJSDOMParser.h \
    QJSDOMSelection.h \
    QJSDOMWindow.h \
    QJSDocument.h \
    QJSDocumentFragment.h \
    QJSDocumentType.h \
    QJSElement.h \
    QJSEntity.h \
    QJSEntityReference.h \
    QJSEvent.h \
    QJSHTMLAppletElement.h \
    QJSHTMLAnchorElement.h \
    QJSHTMLAreaElement.h \
    QJSHTMLBaseElement.h \
    QJSHTMLBaseFontElement.h \
    QJSHTMLBlockquoteElement.h \
    QJSHTMLBodyElement.h \
    QJSHTMLBRElement.h \
    QJSHTMLButtonElement.h \
    QJSHTMLCanvasElement.h \
    QJSHTMLCollection.h \
    QJSHTMLDListElement.h \
    QJSHTMLDirectoryElement.h \
    QJSHTMLDivElement.h \
    QJSHTMLDocument.h \
    QJSHTMLElement.h \
    QJSHTMLEmbedElement.h \
    QJSHTMLFieldSetElement.h \
    QJSHTMLFontElement.h \
    QJSHTMLFormElement.h \
    QJSHTMLFrameElement.h \
    QJSHTMLFrameSetElement.h \
    QJSHTMLHRElement.h \
    QJSHTMLHeadElement.h \
    QJSHTMLHeadingElement.h \
    QJSHTMLHtmlElement.h \
    QJSHTMLIFrameElement.h \
    QJSHTMLImageElement.h \
    QJSHTMLInputElement.h \
    QJSHTMLIsIndexElement.h \
    QJSHTMLLIElement.h \
    QJSHTMLLabelElement.h \
    QJSHTMLLegendElement.h \
    QJSHTMLLinkElement.h \
    QJSHTMLMapElement.h \
    QJSHTMLMarqueeElement.h \
    QJSHTMLMenuElement.h \
    QJSHTMLMetaElement.h \
    QJSHTMLModElement.h \
    QJSHTMLOListElement.h \
    QJSHTMLOptGroupElement.h \
    QJSHTMLObjectElement.h \
    QJSHTMLOptionElement.h \
    QJSHTMLOptionsCollection.h \
    QJSHTMLParagraphElement.h \
    QJSHTMLParamElement.h \
    QJSHTMLPreElement.h \
    QJSHTMLQuoteElement.h \
    QJSHTMLScriptElement.h \
    QJSHTMLSelectElement.h \
    QJSHTMLStyleElement.h \
    QJSHTMLTableCaptionElement.h \
    QJSHTMLTableCellElement.h \
    QJSHTMLTableColElement.h \
    QJSHTMLTableElement.h \
    QJSHTMLTableRowElement.h \
    QJSHTMLTableSectionElement.h \
    QJSHTMLTextAreaElement.h \
    QJSHTMLTitleElement.h \
    QJSHTMLUListElement.h \
    QJSHistory.h \
    QJSKeyboardEvent.h \
    QJSMediaList.h \
    QJSMouseEvent.h \
    QJSMutationEvent.h \
    QJSNamedNodeMap.h \
    QJSNode.h \
    QJSNodeFilter.h \
    QJSNodeIterator.h \
    QJSNodeList.h \
    QJSNotation.h \
    QJSOverflowEvent.h \
    QJSProcessingInstruction.h \
    QJSRange.h \
    QJSRangeException.h \
    QJSRect.h \
    QJSSVGAElement.h \
    QJSSVGAngle.h \
    QJSSVGAnimatedAngle.h \
    QJSSVGAnimateColorElement.h \
    QJSSVGAnimateElement.h \
    QJSSVGAnimateTransformElement.h \
    QJSSVGAnimatedBoolean.h \
    QJSSVGAnimatedEnumeration.h \
    QJSSVGAnimatedInteger.h \
    QJSSVGAnimatedLength.h \
    QJSSVGAnimatedLengthList.h \
    QJSSVGAnimatedNumber.h \
    QJSSVGAnimatedNumberList.h \
    QJSSVGAnimatedPoints.h \
    QJSSVGAnimatedPreserveAspectRatio.h \
    QJSSVGAnimatedRect.h \
    QJSSVGAnimatedString.h \
    QJSSVGAnimatedTransformList.h \
    QJSSVGAnimationElement.h \
    QJSSVGColor.h \
    QJSSVGCircleElement.h \
    QJSSVGClipPathElement.h \
    QJSSVGComponentTransferFunctionElement.h \
    QJSSVGCursorElement.h \
    QJSSVGDefsElement.h \
    QJSSVGDescElement.h \
    QJSSVGDocument.h \
    QJSSVGException.h \
    QJSSVGLength.h \
    QJSSVGMatrix.h \
    QJSSVGMetadataElement.h \
    QJSSVGPathElement.h \
    QJSSVGPathSeg.h \
    QJSSVGPathSegArcAbs.h \
    QJSSVGPathSegArcRel.h \
    QJSSVGPathSegClosePath.h \
    QJSSVGPathSegCurvetoCubicAbs.h \
    QJSSVGPathSegCurvetoCubicRel.h \
    QJSSVGPathSegCurvetoCubicSmoothAbs.h \
    QJSSVGPathSegCurvetoCubicSmoothRel.h \
    QJSSVGPathSegCurvetoQuadraticAbs.h \
    QJSSVGPathSegCurvetoQuadraticRel.h \
    QJSSVGPathSegCurvetoQuadraticSmoothAbs.h \
    QJSSVGPathSegCurvetoQuadraticSmoothRel.h \
    QJSSVGPathSegLinetoAbs.h \
    QJSSVGPathSegLinetoHorizontalAbs.h \
    QJSSVGPathSegLinetoHorizontalRel.h \
    QJSSVGPathSegLinetoRel.h \
    QJSSVGPathSegLinetoVerticalAbs.h \
    QJSSVGPathSegLinetoVerticalRel.h \
    QJSSVGPathSegMovetoAbs.h \
    QJSSVGPathSegMovetoRel.h \
    QJSSVGNumber.h \
    QJSSVGNumberList.h \
    QJSSVGPaint.h \
    QJSSVGPathSegList.h \
    QJSSVGPatternElement.h \
    QJSSVGPoint.h \
    QJSSVGPointList.h \
    QJSSVGPolygonElement.h \
    QJSSVGPolylineElement.h \
    QJSSVGRadialGradientElement.h \
    QJSSVGRect.h \
    QJSSVGRectElement.h \
    QJSSVGRenderingIntent.h \
    QJSSVGSetElement.h \
    QJSSVGScriptElement.h \
    QJSSVGStyleElement.h \
    QJSSVGSwitchElement.h \
    QJSSVGStopElement.h \
    QJSSVGStringList.h \
    QJSSVGSymbolElement.h \
    QJSSVGTRefElement.h \
    QJSSVGTSpanElement.h \
    QJSSVGTextElement.h \
    QJSSVGTextContentElement.h \
    QJSSVGTextPathElement.h \
    QJSSVGTextPositioningElement.h \
    QJSSVGTitleElement.h \
    QJSSVGTransform.h \
    QJSSVGTransformList.h \
    QJSSVGUnitTypes.h \
    QJSSVGUseElement.h \
    QJSSVGViewElement.h \
    QJSSVGPreserveAspectRatio.h \
    QJSSVGElement.h \
    QJSSVGElementInstance.h \
    QJSSVGElementInstanceList.h \
    QJSSVGSVGElement.h \
    QJSSVGEllipseElement.h \
    QJSSVGFEBlendElement.h \
    QJSSVGFEColorMatrixElement.h \
    QJSSVGFEComponentTransferElement.h \
    QJSSVGFECompositeElement.h \
    QJSSVGFEDiffuseLightingElement.h \
    QJSSVGFEDisplacementMapElement.h \
    QJSSVGFEDistantLightElement.h \
    QJSSVGFEFloodElement.h \
    QJSSVGFEFuncAElement.h \
    QJSSVGFEFuncBElement.h \
    QJSSVGFEFuncGElement.h \
    QJSSVGFEFuncRElement.h \
    QJSSVGFEGaussianBlurElement.h \
    QJSSVGFEImageElement.h \
    QJSSVGFEMergeElement.h \
    QJSSVGFEMergeNodeElement.h \
    QJSSVGFEOffsetElement.h \
    QJSSVGFEPointLightElement.h \
    QJSSVGFESpecularLightingElement.h \
    QJSSVGFESpotLightElement.h \
    QJSSVGFETileElement.h \
    QJSSVGFETurbulenceElement.h \
    QJSSVGFilterElement.h \
    QJSSVGForeignObjectElement.h \
    QJSSVGGElement.h \
    QJSSVGGradientElement.h \
    QJSSVGImageElement.h \
    QJSSVGLength.h \
    QJSSVGLengthList.h \
    QJSSVGLineElement.h \
    QJSSVGLinearGradientElement.h \
    QJSSVGMaskElement.h \
    QJSSVGMarkerElement.h \
    QJSSVGTransform.h \
    QJSSVGZoomEvent.h \
    QJSScreen.h \
    QJSStyleSheet.h \
    QJSStyleSheetList.h \
    QJSText.h \
    QJSTextEvent.h \
    QJSTreeWalker.h \
    QJSUIEvent.h \
    QJSXPathEvaluator.h \
    QJSXPathExpression.h \
    QJSXPathNSResolver.h \
    QJSXPathResult.h \
    QJSWheelEvent.h \
    QJSXMLSerializer.h \
#

# QuickJS binding code generator scripts

QJS_BINDINGS_SCRIPTS = \
    bindings/scripts/CodeGenerator.pm \
    bindings/scripts/CodeGeneratorQJS.pm \
    bindings/scripts/IDLParser.pm \
    bindings/scripts/IDLStructure.pm \
    bindings/scripts/generate-bindings.pl \
#

QJS%.h : %.idl $(QJS_BINDINGS_SCRIPTS)
	perl -I $(WebCore)/bindings/scripts $(WebCore)/bindings/scripts/generate-bindings.pl --defines "$(FEATURE_DEFINES) LANGUAGE_JAVASCRIPT" --generator QJS --include $(WebCore)/dom --include $(WebCore)/html --include $(WebCore)/css --include $(WebCore)/page --include $(WebCore)/xml --include $(WebCore)/ksvg2/svg --include $(WebCore)/ksvg2/events --outputdir . $<
