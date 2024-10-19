#
# Copyright (C) 2024 Zhang Ji Peng. <onecoolx@gmail.com>.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# aint with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.

package CodeGeneratorQJS;

use File::stat;

my $module = "";
my $outputDir = "";

my @headerContent = ();
my @implContentHeader = ();
my @implContent = ();
my %implIncludes = ();

# Default .h template
my $headerTemplate = << "EOF";
/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx\@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
EOF

# Default constructor
sub new
{
    my $object = shift;
    my $reference = { };

    $codeGenerator = shift;
    $outputDir = shift;

    bless($reference, $object);
    return $reference;
}

sub finish
{
    my $object = shift;

    # Commit changes!
    $object->WriteData();
}

sub leftShift($$) {
    my ($value, $distance) = @_;
    return (($value << $distance) & 0xFFFFFFFF);
}

# Uppercase the first letter, while respecting WebKit style guidelines. 
# E.g., xmlEncoding becomes XMLEncoding, but xmlllang becomes Xmllang.
sub WK_ucfirst
{
    my $param = shift;
    my $ret = ucfirst($param);
    $ret =~ s/Xml/XML/ if $ret =~ /^Xml[^a-z]/;
    return $ret;
}

# Params: 'domClass' struct
sub GenerateInterface
{
    my $object = shift;
    my $dataNode = shift;
    my $defines = shift;

    # Start actual generation
    $object->GenerateHeader($dataNode);
    $object->GenerateImplementation($dataNode);

    my $name = $dataNode->name;

    # Open files for writing
    my $headerFileName = "$outputDir/QJS$name.h";
    my $implFileName = "$outputDir/QJS$name.cpp";

    open($IMPL, ">$implFileName") || die "Couldn't open file $implFileName";
    open($HEADER, ">$headerFileName") || die "Couldn't open file $headerFileName";
}

# Params: 'idlDocument' struct
sub GenerateModule
{
    my $object = shift;
    my $dataNode = shift;

    $module = $dataNode->module;
}

sub GetParentClassName
{
    my $dataNode = shift;

    return $dataNode->extendedAttributes->{"LegacyParent"} if $dataNode->extendedAttributes->{"LegacyParent"};
    return "QJS::DOMObject" if @{$dataNode->parents} eq 0;
    return "JS" . $codeGenerator->StripModule($dataNode->parents(0));
}

sub GetLegacyHeaderIncludes
{
    my $legacyParent = shift;

    return "#include \"QJSHTMLInputElementBase.h\"\n\n" if $legacyParent eq "JSHTMLInputElementBase";
    return "#include \"qjs_window.h\"\n\n" if $legacyParent eq "Window"; #<Debug> QJS::Window #
    return "#include \"qjs_events.h\"\n\n" if $module eq "events";
    return "#include \"qjs_css.h\"\n\n" if $module eq "css";
    return "#include \"qjs_html.h\"\n\n" if $module eq "html";

    die "Don't know what headers to include for module $module";
}

sub AvoidInclusionOfType
{
    my $type = shift;

    # Special case: SVGRect.h / SVGPoint.h / SVGNumber.h / SVGMatrix.h do not exist.
    return 1 if $type eq "SVGRect" or $type eq "SVGPoint" or $type eq "SVGNumber" or $type eq "SVGMatrix";
    return 0;
}

sub UsesManualToJSImplementation
{
    my $type = shift;

    return 1 if $type eq "Node" or $type eq "Document" or $type eq "HTMLCollection" or $type eq "SVGPathSeg" or $type eq "StyleSheet" or $type eq "CSSRule" or $type eq "CSSValue" or $type eq "Event";
    return 0;
}

sub IndexGetterReturnsStrings
{
    my $type = shift;

    return 1 if $type eq "CSSStyleDeclaration" or $type eq "MediaList";
    return 0;
}

sub CreateSVGContextInterfaceName
{
    my $type = shift;

    return $type if $codeGenerator->IsSVGAnimatedType($type);
    return "SVGPathSeg" if $type =~ /^SVGPathSeg/ and $type ne "SVGPathSegList";

    return "";
}

sub AddIncludesForType
{
    my $type = $codeGenerator->StripModule(shift);

    # When we're finished with the one-file-per-class
    # reorganization, we won't need these special cases.
    if ($codeGenerator->IsPrimitiveType($type) or AvoidInclusionOfType($type)
        or $type eq "DOMString" or $type eq "DOMObject" or $type eq "RGBColor") {
    } elsif ($type =~ /SVGPathSeg/) {
        $joinedName = $type;
        $joinedName =~ s/Abs|Rel//;
        $implIncludes{"${joinedName}.h"} = 1;
    } elsif ($type eq "XPathNSResolver") {
        $implIncludes{"QJSXPathNSResolver.h"} = 1;
        $implIncludes{"QJSCustomXPathNSResolver.h"} = 1;
    } else {
        # default, include the same named file
        $implIncludes{"${type}.h"} = 1;
    }

    # additional includes (things needed to compile the bindings but not the header)

    if ($type eq "CanvasRenderingContext2D") {
        $implIncludes{"CanvasGradient.h"} = 1;
        $implIncludes{"CanvasPattern.h"} = 1;
        $implIncludes{"CanvasStyle.h"} = 1;
    }

    if ($type eq "CanvasGradient" or $type eq "XPathNSResolver") {
        $implIncludes{"text/String.h"} = 1;
    }
}

sub AddIncludesForSVGAnimatedType
{
    my $type = shift;
    $type =~ s/SVGAnimated//;

    if ($type eq "Point" or $type eq "Rect") {
        $implIncludes{"Float$type.h"} = 1;
    } elsif ($type eq "String") {
        $implIncludes{"text/String.h"} = 1;
    }
}

sub AddClassForwardIfNeeded
{
    my $implClassName = shift;

    # SVGAnimatedLength/Number/etc.. are typedefs to SVGAnimtatedTemplate, so don't use class forwards for them!
    push(@headerContent, "class $implClassName;\n\n") unless $codeGenerator->IsSVGAnimatedType($implClassName);
}

sub IsSVGTypeNeedingContextParameter
{
    my $implClassName = shift;

    if ($implClassName =~ /SVG/ and not $implClassName =~ /Element/) {
        return 1 unless $implClassName =~ /SVGPaint/ or $implClassName =~ /SVGColor/ or $implClassName =~ /SVGDocument/;
    }

    return 0;
}

sub HashValueForClassAndName
{
    my $class = shift;
    my $name = shift;

    # SVG Filter enums live in WebCore namespace (platform/graphics/)
    if ($class =~ /^SVGFE*/ or $class =~ /^SVGComponentTransferFunctionElement$/) {
        return "WebCore::$name";
    }

    return "${class}::$name";
}

sub GenerateHeader
{
    my $object = shift;
    my $dataNode = shift;

    my $interfaceName = $dataNode->name;
    my $className = "JS$interfaceName";
    my $implClassName = $interfaceName;

    # We only support multiple parents with SVG (for now).
    if (@{$dataNode->parents} > 1) {
        die "A class can't have more than one parent" unless $interfaceName =~ /SVG/;
        $codeGenerator->AddMethodsConstantsAndAttributesFromParentClasses($dataNode);
    }

    my $hasLegacyParent = $dataNode->extendedAttributes->{"LegacyParent"};
    my $hasRealParent = @{$dataNode->parents} > 0;
    my $hasParent = $hasLegacyParent || $hasRealParent;
    my $parentClassName = GetParentClassName($dataNode);
    my $conditional = $dataNode->extendedAttributes->{"Conditional"};

    # - Add default header template
    @headerContent = split("\r", $headerTemplate);

    # - Add header protection
    push(@headerContent, "\n#ifndef _$className" . "_H_");
    push(@headerContent, "\n#define _$className" . "_H_\n\n");

    my $conditionalString;
    if ($conditional) {
        $conditionalString = "ENABLE(" . join(") && ENABLE(", split(/&/, $conditional)) . ")";
        push(@headerContent, "\n#if ${conditionalString}\n\n");
    }

    if (exists $dataNode->extendedAttributes->{"LegacyParent"}) {
        push(@headerContent, GetLegacyHeaderIncludes($dataNode->extendedAttributes->{"LegacyParent"}));
    } else {
        if ($hasParent) {
            push(@headerContent, "#include \"Q$parentClassName.h\"\n");
        } else {
            push(@headerContent, "#include \"qjs_binding.h\"\n");
        }
    }

    # Get correct pass/store types respecting PODType flag
    my $podType = $dataNode->extendedAttributes->{"PODType"};
    my $passType = $podType ? "JSSVGPODTypeWrapper<$podType>*" : "$implClassName*";
    push(@headerContent, "#include \"$podType.h\"\n") if $podType and $podType ne "float";

    push(@headerContent, "#include \"JSSVGPODTypeWrapper.h\"\n") if $podType;

    my $numConstants = @{$dataNode->constants};
    my $numAttributes = @{$dataNode->attributes};
    my $numFunctions = @{$dataNode->functions};

    push(@headerContent, "\nnamespace WebCore {\n\n");

    # Implementation class forward declaration
    AddClassForwardIfNeeded($implClassName) unless $podType;

    # Class declaration
    #<Debug>#push(@headerContent, "class $className : public $parentClassName {\n");
    push(@headerContent, "class $className {\n");
    push(@headerContent, "private:\n");
    push(@headerContent, "      $className();\n");
    push(@headerContent, "public:\n");

    push(@headerContent, "    static void init(JSContext*);\n");
    # Constructor
    if ($dataNode->extendedAttributes->{"DoNotCache"}) {
        push(@headerContent, "    static JSValue create(JSContext*, JSValue, $passType);\n");
    } else {
        if (IsSVGTypeNeedingContextParameter($implClassName)) {
            push(@headerContent, "    static JSValue create(JSContext*, $passType, SVGElement* context);\n");
        } else {
            push(@headerContent, "    static JSValue create(JSContext*, $passType);\n");
        }
    }

    # Destructor
    if (!$hasParent or $interfaceName eq "Document") {
        #<Debug>#push(@headerContent, "    virtual ~$className();\n");
    }

    push(@headerContent, "    static void finalizer(JSRuntime *rt, JSValue val);\n");
    push(@headerContent, "\n");

    # Getters
    if ($numAttributes > 0 || $dataNode->extendedAttributes->{"GenerateConstructor"}) {
        push(@headerContent, "    static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);\n");
        if ($dataNode->extendedAttributes->{"CustomGetOwnPropertySlot"}) {
            #<Debug> #push(@headerContent, "    bool customGetOwnPropertySlot(KJS::ExecState*, const KJS::Identifier&, KJS::PropertySlot&);\n");
        }
    }

    # Check if we have any writable properties
    my $hasReadWriteProperties = 0;
    foreach (@{$dataNode->attributes}) {
        if ($_->type !~ /^readonly\ attribute$/) {
            $hasReadWriteProperties = 1;
        }
    }

    if ($hasReadWriteProperties) {
        push(@headerContent, "    static JSValue putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue val, int token);\n");
        if ($dataNode->extendedAttributes->{"CustomPutFunction"}) {
            #<Debug>#push(@headerContent, "    bool customPut(KJS::ExecState*, const KJS::Identifier&, KJS::JSValue*, int attr);\n");
        }
    }

    # Class info
    push(@headerContent, "\n");
    push(@headerContent, "    static JSClassID js_class_id;\n\n");

    push(@headerContent, "    static void mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func);\n\n");

    # Custom pushEventHandlerScope function
    if ($dataNode->extendedAttributes->{"CustomPushEventHandlerScope"}) {
        #<Debug>#push(@headerContent, "    virtual void pushEventHandlerScope(KJS::ExecState*, KJS::ScopeChain&) const;\n\n");
    }

    # Custom call functions
    if ($dataNode->extendedAttributes->{"CustomCall"}) {
        push(@headerContent, "    virtual JSValue callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv);\n");
    }

    # Constructor object getter
    if ($dataNode->extendedAttributes->{"GenerateConstructor"} || $dataNode->extendedAttributes->{"GenerateExtendConstructor"}) {
        push(@headerContent, "    static JSValue getConstructor(JSContext *ctx);\n");
        push(@headerContent, "\n");
    }

    my $numCustomFunctions = 0;
    my $numCustomAttributes = 0;

    # Attribute and function enums
    my $hasAttrFunctionEnum = ($numAttributes + $numFunctions > 0) || $dataNode->extendedAttributes->{"GenerateConstructor"};
    push(@headerContent, "    enum {\n") if ($hasAttrFunctionEnum);

    if ($numAttributes > 0) {
        push(@headerContent, "        // Attributes\n        ");

        my $i = -1;
        foreach (@{$dataNode->attributes}) {
            my $attribute = $_;

            $numCustomAttributes++ if $attribute->signature->extendedAttributes->{"Custom"};
            $numCustomAttributes++ if $attribute->signature->extendedAttributes->{"CustomGetter"};
            $numCustomAttributes++ if $attribute->signature->extendedAttributes->{"CustomSetter"};

            $i++;
            if ((($i % 4) eq 0) and ($i ne 0)) {
                push(@headerContent, "\n        ");
            }

            my $value = $attribute->signature->type =~ /Constructor$/
                      ? $attribute->signature->name . "ConstructorAttrNum"
                      : WK_ucfirst($attribute->signature->name) . "AttrNum";
            $value .= ", " if (($i < $numAttributes - 1) or (($i eq $numAttributes - 1) and (($numFunctions ne 0) or $dataNode->extendedAttributes->{"GenerateConstructor"})));
            push(@headerContent, $value);
        }
    }

    if ($dataNode->extendedAttributes->{"GenerateConstructor"}) {
        push(@headerContent, "\n\n") if $numAttributes > 0;
        push(@headerContent, "        // The Constructor Attribute\n");
        push(@headerContent, "        ConstructorAttrNum" . ($numFunctions ? ", " : ""));
    }

    if ($numFunctions > 0) {
        push(@headerContent, "\n\n") if $numAttributes > 0 || $dataNode->extendedAttributes->{"GenerateConstructor"};
        push(@headerContent,"        // Functions\n        ");

        $i = -1;
        foreach my $function (@{$dataNode->functions}) {
            $i++;

            push(@headerContent, "\n        ") if ((($i % 4) eq 0) and ($i ne 0));

            $numCustomFunctions++ if $function->signature->extendedAttributes->{"Custom"};

            my $value = WK_ucfirst($function->signature->name) . "FuncNum";
            $value .= ", " if ($i < $numFunctions - 1);
            push(@headerContent, $value);
        }
    }


    push(@headerContent, "\n    };\n") if ($hasAttrFunctionEnum);

    if ($numCustomAttributes > 0) {
        push(@headerContent, "\n    // Custom attributes\n");

        foreach my $attribute (@{$dataNode->attributes}) {
            if ($attribute->signature->extendedAttributes->{"Custom"}) {
                push(@headerContent, "    static JSValue " . $attribute->signature->name . "(JSContext *ctx, $implClassName *impl);\n");
                if ($attribute->type !~ /^readonly/) {
                    push(@headerContent, "    static void set" . WK_ucfirst($attribute->signature->name) . "(JSContext *ctx, JSValue, $implClassName *impl);\n");
                }
            } elsif ($attribute->signature->extendedAttributes->{"CustomGetter"}) {
                push(@headerContent, "    static JSValue " . $attribute->signature->name . "(JSContext *ctx, $implClassName *impl);\n");
            } elsif ($attribute->signature->extendedAttributes->{"CustomSetter"}) {
                if ($attribute->type !~ /^readonly/) {
                    push(@headerContent, "    static void set" . WK_ucfirst($attribute->signature->name) . "(JSContext *ctx, JSValue, $implClassName *impl);\n");
                }
            }
        }
    }

    if ($numCustomFunctions > 0) {
        push(@headerContent, "\n    // Custom functions\n");
        foreach my $function (@{$dataNode->functions}) {
            if ($function->signature->extendedAttributes->{"Custom"}) {
                push(@headerContent, "    static JSValue " . $function->signature->name . "(JSContext *ctx, int argc, JSValueConst *argv, $implClassName *impl);\n");
            }
        }
    }

    # Index setter
    if ($dataNode->extendedAttributes->{"HasCustomIndexSetter"}) {
        push(@headerContent, "    void indexSetter(JSContext* ctx, unsigned index, JSValue value, int attr);\n");
    }

    if (!$hasParent) {
        if ($podType) {
            push(@headerContent, "    JSSVGPODTypeWrapper<$podType>* impl() const { return m_impl.get(); }\n");
            push(@headerContent, "    SVGElement* context() const { return m_context.get(); }\n\n");
            push(@headerContent, "private:\n");
            push(@headerContent, "    RefPtr<SVGElement> m_context;\n");
            push(@headerContent, "    RefPtr<JSSVGPODTypeWrapper<$podType> > m_impl;\n");
        } elsif (IsSVGTypeNeedingContextParameter($implClassName)) {
            push(@headerContent, "    $implClassName* impl() const { return m_impl.get(); }\n");
            push(@headerContent, "    SVGElement* context() const { return m_context.get(); }\n\n");
            push(@headerContent, "private:\n");
            push(@headerContent, "    RefPtr<SVGElement> m_context;\n");
            push(@headerContent, "    RefPtr<$implClassName > m_impl;\n");
        } else {
            #<Debug>#push(@headerContent, "    $implClassName* impl() const { return m_impl.get(); }\n\n");
            #<Debug>#push(@headerContent, "private:\n");
            #<Debug>#push(@headerContent, "    RefPtr<$implClassName> m_impl;\n");
        }
    } elsif ($dataNode->extendedAttributes->{"GenerateNativeConverter"}) {
        #<Debug>#push(@headerContent, "    $implClassName* impl() const;\n");
    }

    # Index getter
    if ($dataNode->extendedAttributes->{"HasIndexGetter"}) {
        push(@headerContent, "private:\n");
        push(@headerContent, "    static JSValue indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx);\n");
    }
    # Name getter
    if ($dataNode->extendedAttributes->{"HasNameGetter"} || $dataNode->extendedAttributes->{"HasOverridingNameGetter"}) {
        push(@headerContent, "private:\n");
        #<Debug>#push(@headerContent, "    static bool canGetItemsForName(KJS::ExecState*, $implClassName*, const KJS::Identifier&);\n");
        push(@headerContent, "    static JSValue nameGetter(JSContext *ctx, JSValueConst this_obj, const char* prop);\n");
    }

    push(@headerContent, "};\n\n");

    if (!$hasParent || $dataNode->extendedAttributes->{"GenerateToJS"}) {
        if ($podType) {
            push(@headerContent, "JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<$podType>*, SVGElement* context);\n");
        } elsif (IsSVGTypeNeedingContextParameter($implClassName)) {
            push(@headerContent, "JSValue toJS(JSContext *ctx, $passType, SVGElement* context);\n");
        } elsif ($interfaceName eq "Node") {
            push(@headerContent, "JSValue toJS(JSContext *ctx, PassRefPtr<Node>);\n");
        } else {
            push(@headerContent, "JSValue toJS(JSContext *ctx, $passType);\n");
        }
    }
    if (!$hasParent || $dataNode->extendedAttributes->{"GenerateNativeConverter"}) {
        if ($podType) {
            push(@headerContent, "$podType to${interfaceName}(JSValue);\n");
        } else {
            push(@headerContent, "$implClassName* to${interfaceName}(JSValue);\n");
        }
    }
    push(@headerContent, "\n");

    # Add prototype declaration -- code adopted from the KJS_DEFINE_PROTOTYPE and KJS_DEFINE_PROTOTYPE_WITH_PROTOTYPE macros
    push(@headerContent, "class ${className}Prototype {\n");
    push(@headerContent, "public:\n");
    push(@headerContent, "    static JSValue self(JSContext * ctx);\n");
    push(@headerContent, "    static void initPrototype(JSContext * ctx, JSValue this_obj);\n");
    if ($numFunctions > 0 || $numConstants > 0) {
        #<Debug>#push(@headerContent, "    bool getOwnPropertySlot(KJS::ExecState*, const KJS::Identifier&, KJS::PropertySlot&);\n");
    }
    if ($numConstants ne 0) {
        push(@headerContent, "    static JSValue getValueProperty(JSContext *ctx, JSValueConst this_val, int token);\n");
    }
    push(@headerContent, "};\n\n");

    if ($numFunctions > 0) {
        push(@headerContent, prototypeFunctionFor($className));
    }

    push(@headerContent, "} // namespace WebCore\n\n");
    push(@headerContent, "#endif // ${conditionalString}\n\n") if $conditional;
    push(@headerContent, "#endif\n");
}

sub GenerateImplementation
{
    my ($object, $dataNode) = @_;

    my $interfaceName = $dataNode->name;
    my $className = "JS$interfaceName";
    my $implClassName = $interfaceName;

    my $hasLegacyParent = $dataNode->extendedAttributes->{"LegacyParent"};
    my $hasRealParent = @{$dataNode->parents} > 0;
    my $hasParent = $hasLegacyParent || $hasRealParent;
    my $parentClassName = GetParentClassName($dataNode);
    my $conditional = $dataNode->extendedAttributes->{"Conditional"};

    # - Add default header template
    @implContentHeader = split("\r", $headerTemplate);
    push(@implContentHeader, "\n#include \"config.h\"\n\n");
    my $conditionalString;
    if ($conditional) {
        $conditionalString = "ENABLE(" . join(") && ENABLE(", split(/&/, $conditional)) . ")";
        push(@implContentHeader, "\n#if ${conditionalString}\n\n");
    }

    if ($className =~ /^JSSVG/) {
        push(@implContentHeader, "#include \"Document.h\"\n");
        push(@implContentHeader, "#include \"Frame.h\"\n");
        push(@implContentHeader, "#include \"SVGDocumentExtensions.h\"\n");
        push(@implContentHeader, "#include \"SVGElement.h\"\n");
        push(@implContentHeader, "#include \"SVGAnimatedTemplate.h\"\n");

        if ($className =~ /^JSSVGAnimated/) {
            AddIncludesForSVGAnimatedType($interfaceName);
        }
    }

    push(@implContentHeader, "#include \"Q$className.h\"\n\n");
    #<Debug>#push(@implContentHeader, "#include <wtf/GetPtr.h>\n\n");

    AddIncludesForType($interfaceName);

    @implContent = ();

    push(@implContent, "\nusing namespace QJS;\n\n");
    push(@implContent, "\nnamespace WebCore {\n\n");

    push(@implContent, "#define countof(x) (sizeof(x) / sizeof((x)[0]))\n");
    # - Add all attributes in a hashtable definition
    my $numAttributes = @{$dataNode->attributes};
    $numAttributes++ if $dataNode->extendedAttributes->{"GenerateConstructor"};

    if ($numAttributes > 0) {
        my $hashSize = $numAttributes;
        my $hashName = $className;
        my $hashType = "Attributes";

        my @hashKeys = ();      # ie. 'insertBefore'
        my @hashValues = ();    # ie. 'JSNode::InsertBefore'
        my @hashSpecials = ();    # ie. 'DontDelete|Function'
        my @hashParameters = ();  # ie. '2'
        my @hashReadonly = ();

        foreach my $attribute (@{$dataNode->attributes}) {
            my $name = $attribute->signature->name;
            push(@hashKeys, $name);

            my $value = $className . "::" . ($attribute->signature->type =~ /Constructor$/
                                       ? $attribute->signature->name . "ConstructorAttrNum"
                                       : WK_ucfirst($attribute->signature->name) . "AttrNum");
            push(@hashValues, $value);

            if ($attribute->type =~ /readonly/) {
                push(@hashReadonly, "1");
            } else {
                push(@hashReadonly, "0");
            }
        }

        if ($dataNode->extendedAttributes->{"GenerateConstructor"}) {
            push(@hashKeys, "constructor");
            push(@hashValues, $className . "::ConstructorAttrNum");
            push(@hashReadonly, "1"); 
        }

        $object->GenerateAttributesTable($hashName, $hashSize, $hashType,
                                   \@hashKeys, \@hashValues,
                                   \@hashReadonly);
    }

    my $numConstants = @{$dataNode->constants};
    my $numFunctions = @{$dataNode->functions};

    # - Add all constants
    if ($dataNode->extendedAttributes->{"GenerateConstructor"}) {
        my $protoClassName;
        $protoClassName = "${className}Prototype";

        push(@implContent, constructorFor($className, $protoClassName, $interfaceName, $dataNode->extendedAttributes->{"CanBeConstructed"}));

        if ($numConstants > 0) {
            # create constructor table

            $hashSize = $numConstants;
            $hashName = $className . "Constructor";

            @hashKeys = ();
            @hashValues = ();
            @hashParameters = ();
            @hashReadonly = ();

            foreach my $constant (@{$dataNode->constants}) {
                my $name = $constant->name;
                push(@hashKeys, $name);

                my $value = HashValueForClassAndName($implClassName, $name);
                push(@hashValues, $value);

                my $numParameters = 0;
                push(@hashParameters, $numParameters);
                push(@hashReadonly, "1");
            }

            $object->GenerateHashTable($hashName, $hashSize,
                                       \@hashKeys, \@hashValues,
                                       \@hashParameters, \@hashReadonly);
        }

        push(@implContent, "JSValue ${className}Constructor::self(JSContext * ctx)\n{\n");
        push(@implContent, "    JSValue globalObj = JS_GetGlobalObject(ctx);\n");
        push(@implContent, "    JSValue obj = JS_GetPropertyStr(ctx, globalObj, \"[[${interfaceName}.constructor]]\");\n");
        push(@implContent, "    if (JS_IsException(obj)) {\n");
        if ($dataNode->extendedAttributes->{"CanBeConstructed"}) {
            push(@implContent, "        obj = JS_NewCFunction2(ctx, ${className}Constructor::construct, \"${interfaceName}\", 0, JS_CFUNC_constructor, 0);\n");
        } else {
            push(@implContent, "        obj = JS_NewObject(ctx);\n");
        }
        push(@implContent, "        ${className}Constructor::initConstructor(ctx, obj);\n");
        push(@implContent, "        JS_SetPropertyStr(ctx, globalObj, \"[[${interfaceName}.constructor]]\", obj);\n");
        push(@implContent, "        obj = JS_DupValue(ctx, obj);\n");
        push(@implContent, "    }\n");
        push(@implContent, "    JS_FreeValue(ctx, globalObj);\n");
        push(@implContent, "    JS_FreeValue(ctx, obj);\n");
        push(@implContent, "    return obj;\n");
        push(@implContent, "}\n\n");

        push(@implContent, "void ${className}Constructor::initConstructor(JSContext * ctx, JSValue this_obj)\n{\n");
        if ($numConstants > 0) {
            push(@implContent, "    JS_SetPropertyFunctionList(ctx, this_obj, ${className}ConstructorFunctions, countof(${className}ConstructorFunctions));\n");
        }
        push(@implContent, "}\n\n");

        if ($dataNode->extendedAttributes->{"CanBeConstructed"}) {
            push(@implContent, "JSValue ${className}Constructor::construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv)\n{\n");
            push(@implContent, "    return toJS(ctx, adoptRef(new $interfaceName).get());\n");
            push(@implContent, "}\n\n");
        }
    }

    if ($numConstants > 0) {
        # - Add constants
        $hashSize = $numConstants;
        $hashName = $className . "Prototype";
        $hashType = "Constants";

        @hashKeys = ();
        @hashValues = ();
        @hashReadonly = ();

        foreach my $constant (@{$dataNode->constants}) {
            my $name = $constant->name;
            push(@hashKeys, $name);

            my $value = HashValueForClassAndName($implClassName, $name);
            push(@hashValues, $value);

            push(@hashReadonly, "1");
        }

        $object->GenerateAttributesTable($hashName, $hashSize, $hashType,
                                   \@hashKeys, \@hashValues,
                                   \@hashReadonly);
    }

    if ($numFunctions > 0) {
        # - Add functions 
        $hashSize = $numFunctions;
        $hashName = $className . "Prototype";

        @hashKeys = ();
        @hashValues = ();
        @hashParameters = ();

        foreach my $function (@{$dataNode->functions}) {
            my $name = $function->signature->name;
            push(@hashKeys, $name);

            my $value = $className . "::" . WK_ucfirst($name) . "FuncNum";
            push(@hashValues, $value);

            my $numParameters = @{$function->parameters};
            push(@hashParameters, $numParameters);
        }

        $object->GeneratePrototypeFuncTable($hashName, $hashSize,
                                   \@hashKeys, \@hashValues,
                                   \@hashParameters);
    }

    if ($dataNode->extendedAttributes->{"DoNotCache"}) {
        push(@implContent, "JSValue ${className}Prototype::self(JSContext * ctx)\n");
        push(@implContent, "{\n");
        push(@implContent, "    JSValue obj = JS_NewObject(ctx);\n");
        push(@implContent, "    ${className}Prototype::initPrototype(ctx, obj);\n");
        push(@implContent, "    return obj;\n");
        push(@implContent, "}\n\n");
    } else {
        push(@implContent, "JSValue ${className}Prototype::self(JSContext * ctx)\n");
        push(@implContent, "{\n");
        push(@implContent, "    JSValue globalObj = JS_GetGlobalObject(ctx);\n");
        push(@implContent, "    JSValue obj = JS_GetPropertyStr(ctx, globalObj, \"[[${className}.prototype]]\");\n");
        push(@implContent, "    if (JS_IsException(obj)) {\n");
        if ($hasParent) {
            push(@implContent, "        obj = JS_NewObjectProto(ctx, ${parentClassName}Prototype::self(ctx));\n");
        } else {
            push(@implContent, "        obj = JS_NewObject(ctx);\n");
        }
        push(@implContent, "        ${className}Prototype::initPrototype(ctx, obj);\n");
        push(@implContent, "        JS_SetPropertyStr(ctx, globalObj, \"[[${className}.prototype]]\", obj);\n");
        push(@implContent, "        obj = JS_DupValue(ctx, obj);\n");
        push(@implContent, "    }\n");
        push(@implContent, "    JS_FreeValue(ctx, globalObj);\n");
        push(@implContent, "    JS_FreeValue(ctx, obj);\n");
        push(@implContent, "    return obj;\n");
        push(@implContent, "}\n\n");
    }
    if ($numConstants > 0 || $numFunctions > 0) {
        #<Debug>#push(@implContent, "bool ${className}Prototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)\n");
        #push(@implContent, "{\n");
        #if ($numConstants eq 0) {
        #    push(@implContent, "    return getStaticFunctionSlot<${className}PrototypeFunction, JSObject>(exec, &${className}PrototypeTable, this, propertyName, slot);\n");
        #} elsif ($numFunctions eq 0) {
         #   push(@implContent, "    return getStaticValueSlot<${className}Prototype, JSObject>(exec, &${className}PrototypeTable, this, propertyName, slot);\n");
        #} else {
        #    push(@implContent, "    return getStaticPropertySlot<${className}PrototypeFunction, ${className}Prototype, JSObject>(exec, &${className}PrototypeTable, this, propertyName, slot);\n");
        #}
        #push(@implContent, "}\n\n");
    }
    push(@implContent, "void ${className}Prototype::initPrototype(JSContext * ctx, JSValue this_obj)\n{\n");
    if ($numAttributes > 0) {
        push(@implContent, "    JS_SetPropertyFunctionList(ctx, this_obj, ${className}AttributesFunctions, countof(${className}AttributesFunctions));\n");
    }
    if ($numConstants > 0) {
        push(@implContent, "    JS_SetPropertyFunctionList(ctx, this_obj, ${className}PrototypeConstantsFunctions, countof(${className}PrototypeConstantsFunctions));\n");
    }
    if ($numFunctions > 0) {
        push(@implContent, "    JS_SetPropertyFunctionList(ctx, this_obj, ${className}PrototypeFunctions, countof(${className}PrototypeFunctions));\n");
    }
    push(@implContent, "}\n\n");
    if ($numConstants ne 0) {
        push(@implContent, "JSValue ${className}Prototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)\n{\n");
        push(@implContent, "    // The token is the numeric value of its associated constant\n");
        push(@implContent, "    return JS_NewInt32(ctx, token);\n}\n\n");
    }

    # - Initialize static ClassInfo object
    #<Debug>#push(@implContent, "const ClassInfo $className" . "::info = { \"$interfaceName\", ");
    #if ($hasParent) {
    #    push(@implContent, "&" .$parentClassName . "::info, ");
    #} else {
    #    push(@implContent, "0, ");
    #}

    #if ($numAttributes > 0) {
    #    push(@implContent, "&${className}Table, ");
    #} else {
    #    push(@implContent, "0, ")
    #}
    #push(@implContent, "0 };\n\n");


    push(@implContent, "static JSClassDef ${className}ClassDefine = \n\{\n");
    push(@implContent, "    \"${interfaceName}\",\n");
    push(@implContent, "    .finalizer = ${className}::finalizer,\n");
    push(@implContent, "    .gc_mark = ${className}::mark,\n");
    push(@implContent, "};\n\n");

    push(@implContent, "JSClassID ${className}::js_class_id = 0;\n\n");

    push(@implContent, "void ${className}::init(JSContext* ctx)\n{\n");
    push(@implContent, "    JS_NewClassID(&${className}::js_class_id);\n");
    push(@implContent, "    JS_NewClass(JS_GetRuntime(ctx), ${className}::js_class_id, &${className}ClassDefine);\n");
    if (!$dataNode->extendedAttributes->{"DoNotCache"}) {
        if ($dataNode->extendedAttributes->{"GenerateConstructor"}) {
            push(@implContent, "    JS_SetConstructor(ctx, ${className}Constructor::self(ctx), ${className}Prototype::self(ctx));\n");
        }
        push(@implContent, "    JS_SetClassProto(ctx, ${className}::js_class_id, ${className}Prototype::self(ctx));\n");
    }
    push(@implContent, "}\n\n");

    # Get correct pass/store types respecting PODType flag
    my $podType = $dataNode->extendedAttributes->{"PODType"};
    my $passType = $podType ? "JSSVGPODTypeWrapper<$podType>*" : "$implClassName*";

    # Constructor
    if ($dataNode->extendedAttributes->{"DoNotCache"}) {
        push(@implContent, "JSValue ${className}::create(JSContext* ctx, JSValue obj, $passType impl)\n");
        push(@implContent, "{\n");
        push(@implContent, "    JS_SetPrototype(ctx, obj, ${className}Prototype::self(ctx));\n");
        push(@implContent, "    JS_SetOpaque(obj, impl);\n");
        push(@implContent, "    impl->ref();\n");
        push(@implContent, "    return obj;\n");
        push(@implContent, "}\n\n");

    } else {
        my $needsSVGContext = IsSVGTypeNeedingContextParameter($implClassName);
        if ($needsSVGContext) {
            push(@implContent, "JSValue ${className}::create(JSContext* ctx, $passType impl, SVGElement* context)\n");
        } else {
            push(@implContent, "JSValue ${className}::create(JSContext* ctx, $passType impl)\n");
        }

        push(@implContent, "{\n");
        push(@implContent, "    JSValue obj = JS_NewObjectProtoClass(ctx, ${className}Prototype::self(ctx), ${className}::js_class_id);\n");
        push(@implContent, "    if (JS_IsException(obj)) {\n");
        push(@implContent, "        return JS_EXCEPTION;\n");
        push(@implContent, "    }\n");
        push(@implContent, "    JS_SetOpaque(obj, impl);\n");
        push(@implContent, "    impl->ref();\n");
        push(@implContent, "    return obj;\n");
        push(@implContent, "}\n\n");
    }

    push(@implContent, "void ${className}::finalizer(JSRuntime* rt, JSValue val)\n");
    push(@implContent, "{\n");
    push(@implContent, "    ${implClassName}* impl = (${implClassName}*)JS_GetOpaque(val, ${className}::js_class_id);\n");

    if ($interfaceName eq "Node") {
        push(@implContent, "    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);\n");
    } else {
        if ($podType) {
            my $animatedType = $implClassName;
            $animatedType =~ s/SVG/SVGAnimated/;

            # Special case for JSSVGNumber
            if ($codeGenerator->IsSVGAnimatedType($animatedType) and $podType ne "float") {
                push(@implContent, "    JSSVGPODTypeWrapperCache<$podType, $animatedType>::forgetWrapper(impl);\n");
            }
        }
        push(@implContent, "    ScriptInterpreter::forgetDOMObject(impl);\n");
    }

    push(@implContent, "    impl->deref();\n");
    push(@implContent, "}\n\n");

    # Document needs a special destructor because it's a special case for caching. It needs
    # its own special handling rather than relying on the caching that Node normally does.

    if (!$dataNode->extendedAttributes->{"CustomMarkFunction"}) {
        push(@implContent, "void ${className}::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)\n{\n");
        if ($hasRealParent) {
            push(@implContent, "    ${parentClassName}::mark(rt, val, mark_func);\n");
        } else {
            push(@implContent, "    JS_MarkValue(rt, val, mark_func);\n");
        }
        push(@implContent, "}\n\n");
    }
    

    # Attributes
    if ($numAttributes ne 0) {
#<Debug>#
        #push(@implContent, "bool ${className}::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)\n");
        #push(@implContent, "{\n");

       #if ($interfaceName eq "NamedNodeMap" or $interfaceName eq "HTMLCollection") {
       #     push(@implContent, "    JSValue* proto = prototype();\n");
       #     push(@implContent, "    if (proto->isObject() && static_cast<JSObject*>(proto)->hasProperty(exec, propertyName))\n");
       #     push(@implContent, "        return false;\n\n");
       # }

       #my $hasNameGetterGeneration = sub {
       #     push(@implContent, "    if (canGetItemsForName(exec, static_cast<$implClassName*>(impl()), propertyName)) {\n");
       #     push(@implContent, "        slot.setCustom(this, nameGetter);\n");
       #     push(@implContent, "        return true;\n");
       #     push(@implContent, "    }\n");
       #     $implIncludes{"AtomicString.h"} = 1;
       # };

       # if ($dataNode->extendedAttributes->{"HasOverridingNameGetter"}) {
       #     &$hasNameGetterGeneration();
       # }

       # my $requiresManualLookup = $dataNode->extendedAttributes->{"HasIndexGetter"} || $dataNode->extendedAttributes->{"HasNameGetter"};
       # if ($requiresManualLookup) {
       #     push(@implContent, "    const HashEntry* entry = Lookup::findEntry(&${className}Table, propertyName);\n");
       #     push(@implContent, "    if (entry) {\n");
       #     push(@implContent, "        slot.setStaticEntry(this, entry, staticValueGetter<$className>);\n");
       #     push(@implContent, "        return true;\n");
       #     push(@implContent, "    }\n");
       # }

       # if ($dataNode->extendedAttributes->{"HasIndexGetter"}) {
       #     push(@implContent, "    bool ok;\n");
       #     push(@implContent, "    unsigned index = propertyName.toUInt32(&ok, false);\n");
       #     push(@implContent, "    if (ok && index < static_cast<$implClassName*>(impl())->length()) {\n");
       #     push(@implContent, "        slot.setCustomIndex(this, index, indexGetter);\n");
       #     push(@implContent, "        return true;\n");
       #     push(@implContent, "    }\n");
       # }

       # if ($dataNode->extendedAttributes->{"HasNameGetter"}) {
       #     &$hasNameGetterGeneration();
       # }

       # if ($dataNode->extendedAttributes->{"CustomGetOwnPropertySlot"}) {
       #         push(@implContent, "    if (customGetOwnPropertySlot(exec, propertyName, slot))\n");
       #         push(@implContent, "        return true;\n");
       # }

       # if ($requiresManualLookup) {
       #     push(@implContent, "    return ${parentClassName}::getOwnPropertySlot(exec, propertyName, slot);\n");
       # } else {
       #     push(@implContent, "    return getStaticValueSlot<$className, $parentClassName>(exec, &${className}Table, this, propertyName, slot);\n");
       # }
       # push(@implContent, "}\n\n");




        push(@implContent, "JSValue ${className}::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)\n{\n");

        push(@implContent, "    switch (token) {\n");

        foreach my $attribute (@{$dataNode->attributes}) {
            my $name = $attribute->signature->name;

            my $implClassNameForValueConversion = "";
            if (!$podType and ($codeGenerator->IsSVGAnimatedType($implClassName) or $attribute->type !~ /^readonly/)) {
                $implClassNameForValueConversion = $implClassName;
            }

            if ($attribute->signature->type =~ /Constructor$/) {
                push(@implContent, "        case " . $name . "ConstructorAttrNum: {\n");
            } else {
                push(@implContent, "        case " . WK_ucfirst($name) . "AttrNum: {\n");
            }

            if ($dataNode->extendedAttributes->{"CheckDomainSecurity"} && !$attribute->signature->extendedAttributes->{"DoNotCheckDomainSecurity"}) {
                push(@implContent, "            if (!isSafeScript(ctx, this_val))\n");
                push(@implContent, "                return JS_UNDEFINED;\n");
            }

            if ($attribute->signature->extendedAttributes->{"Custom"} || $attribute->signature->extendedAttributes->{"CustomGetter"}) {
                push(@implContent, "            $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_val, ${className}::js_class_id);\n");
                push(@implContent, "            return ${className}::$name(ctx, imp);\n");
            } elsif ($attribute->signature->extendedAttributes->{"CheckNodeSecurity"}) {
                $implIncludes{"qjs_dom.h"} = 1;
                push(@implContent, "            $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_val, ${className}::js_class_id);\n");
                push(@implContent, "            return checkNodeSecurity(ctx, imp->$name()) ? " . NativeToJSValue($attribute->signature, 0, $implClassName, $implClassNameForValueConversion, "imp->$name()") . " : JS_UNDEFINED;\n");
            } elsif ($attribute->signature->extendedAttributes->{"CheckFrameSecurity"}) {
                $implIncludes{"Document.h"} = 1;
                $implIncludes{"qjs_dom.h"} = 1;
                push(@implContent, "            $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_val, ${className}::js_class_id);\n");
                push(@implContent, "            return checkNodeSecurity(ctx, imp->contentDocument()) ? " . NativeToJSValue($attribute->signature,  0, $implClassName, $implClassNameForValueConversion, "imp->$name()") . " : JS_UNDEFINED;\n");
            } elsif ($attribute->signature->type =~ /Constructor$/) {
                my $constructorType = $codeGenerator->StripModule($attribute->signature->type);
                $constructorType =~ s/Constructor$//;
                push(@implContent, "            return JS" . $constructorType . "::getConstructor(ctx);\n");
            } elsif (!@{$attribute->getterExceptions}) {
                if ($podType) {
                    push(@implContent, "            $podType imp(*impl());\n\n");
                    if ($podType eq "float") { # Special case for JSSVGNumber
                        push(@implContent, "            return " . NativeToJSValue($attribute->signature, 0, $implClassName, "", "imp") . ";\n");
                    } else {
                        push(@implContent, "            return " . NativeToJSValue($attribute->signature, 0, $implClassName, "", "imp.$name()") . ";\n");
                    }
                } else {
                    push(@implContent, "            $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_val, ${className}::js_class_id);\n");
                    my $type = $codeGenerator->StripModule($attribute->signature->type);
                    my $jsType = NativeToJSValue($attribute->signature, 0, $implClassName, $implClassNameForValueConversion, "imp->$name()");

                    if ($codeGenerator->IsSVGAnimatedType($type)) {
                        push(@implContent, "            RefPtr<$type> obj = $jsType;\n");
                        push(@implContent, "            return toJS(ctx, obj.get(), imp);\n");
                    } else {
                        push(@implContent, "            return $jsType;\n");
                    }
                }
            } else {
                push(@implContent, "            ExceptionCode ec = 0;\n");

                if ($podType) {
                    push(@implContent, "            $podType imp(*impl());\n\n");
                    push(@implContent, "            JSValue result = " . NativeToJSValue($attribute->signature, 0, $implClassName, "", "imp.$name(ec)") . ";\n");
                } else {
                    push(@implContent, "            $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_val, ${className}::js_class_id);\n");
                    push(@implContent, "            JSValue result = " . NativeToJSValue($attribute->signature, 0, $implClassName, $implClassNameForValueConversion, "imp->$name(ec)") . ";\n");
                }

                push(@implContent, "            setDOMException(ctx, ec);\n");
                push(@implContent, "            return result;\n");
            }
            push(@implContent, "        }\n");
        }

        if ($dataNode->extendedAttributes->{"GenerateConstructor"}) {
            push(@implContent, "        case ConstructorAttrNum:\n");
            push(@implContent, "            return getConstructor(ctx);\n");
        }

        push(@implContent, "    }\n");
        push(@implContent, "    return JS_NULL;\n}\n\n");

        # Check if we have any writable attributes
        my $hasReadWriteProperties = 0;
        foreach my $attribute (@{$dataNode->attributes}) {
            $hasReadWriteProperties = 1 if $attribute->type !~ /^readonly/;
        }
        if ($hasReadWriteProperties) {
            #push(@implContent, "void ${className}::put(ExecState* exec, const Identifier& propertyName, JSValue* value, int attr)\n");
            #push(@implContent, "{\n");
            #if ($dataNode->extendedAttributes->{"HasCustomIndexSetter"}) {
            #    push(@implContent, "    bool ok;\n");
            #    push(@implContent, "    unsigned index = propertyName.toUInt32(&ok, false);\n");
            #    push(@implContent, "    if (ok) {\n");
            #    push(@implContent, "        indexSetter(exec, index, value, attr);\n");
            #    push(@implContent, "        return;\n");
            #    push(@implContent, "    }\n");
            #}
            #if ($dataNode->extendedAttributes->{"CustomPutFunction"}) {
            #    push(@implContent, "    if (customPut(exec, propertyName, value, attr))\n");
            #    push(@implContent, "        return;\n");
            #}

            #push(@implContent, "    lookupPut<$className, $parentClassName>(exec, propertyName, value, attr, &${className}Table, this);\n");
            #push(@implContent, "}\n\n");

            push(@implContent, "JSValue ${className}::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)\n");
            push(@implContent, "{\n");

            push(@implContent, "    switch (token) {\n");

            foreach my $attribute (@{$dataNode->attributes}) {
                if ($attribute->type !~ /^readonly/) {
                    my $name = $attribute->signature->name;

                    if ($attribute->signature->type =~ /Constructor$/) {
                        push(@implContent, "        case " . $name ."ConstructorAttrNum: {\n");
                    } else {
                        push(@implContent, "        case " . WK_ucfirst($name) . "AttrNum: {\n");
                    }

                    if ($dataNode->extendedAttributes->{"CheckDomainSecurity"} && !$attribute->signature->extendedAttributes->{"DoNotCheckDomainSecurity"}) {
                        push(@implContent, "            if (!isSafeScript(ctx, this_val))\n");
                        push(@implContent, "                return JS_UNDEFINED;\n");
                    }

                    if ($attribute->signature->extendedAttributes->{"Custom"} || $attribute->signature->extendedAttributes->{"CustomSetter"}) {
                        push(@implContent, "            $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_val, ${className}::js_class_id);\n");
                        push(@implContent, "            ${className}::set" . WK_ucfirst($name) . "(ctx, value, imp);\n");
                    } elsif ($attribute->signature->type =~ /Constructor$/) {
                        my $constructorType = $attribute->signature->type;
                        $constructorType =~ s/Constructor$//;
                        $implIncludes{"QJS" . $constructorType . ".h"} = 1;
                        push(@implContent, "            // Shadowing a built-in constructor\n");
                        push(@implContent, "            JS_SetPropertyStr(ctx, this_val, \"$name\", value);\n");
                    } else {
                        if ($podType) {
                            push(@implContent, "            $podType imp(*impl());\n\n");
                            if ($podType eq "float") { # Special case for JSSVGNumber
                                push(@implContent, "            imp = " . JSValueToNative($attribute->signature, "value") . ";\n");
                            } else {
                                push(@implContent, "            imp.set" . WK_ucfirst($name) . "(" . JSValueToNative($attribute->signature, "value") . ");\n");
                            }
                            push(@implContent, "            m_impl->commitChange(ctx, imp);\n");
                        } else {
                            push(@implContent, "            $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_val, ${className}::js_class_id);\n");
                            push(@implContent, "            ExceptionCode ec = 0;\n") if @{$attribute->setterExceptions};
                            push(@implContent, "            imp->set" . WK_ucfirst($name) . "(" . JSValueToNative($attribute->signature, "value"));
                            push(@implContent, ", ec") if @{$attribute->setterExceptions};
                            push(@implContent, ");\n");
                            push(@implContent, "            setDOMException(ctx, ec);\n") if @{$attribute->setterExceptions};
                        }
                    }
                    push(@implContent, "            break;\n");
                    push(@implContent, "        }\n");
                }
            }
            push(@implContent, "    }\n"); # end switch
            push(@implContent, "    return JS_UNDEFINED;\n");

            if (IsSVGTypeNeedingContextParameter($implClassName)) {
                push(@implContent, "    if (context())\n");
                push(@implContent, "        context()->notifyAttributeChange();\n");
            }

            push(@implContent, "}\n\n"); # end function
        }
    }

    if ($dataNode->extendedAttributes->{"GenerateConstructor"}) {
        push(@implContent, "JSValue ${className}::getConstructor(JSContext *ctx)\n{\n");
        push(@implContent, "    return ${className}Constructor::self(ctx);\n");
        push(@implContent, "}\n\n");
    } elsif ($dataNode->extendedAttributes->{"GenerateExtendConstructor"}) {
        push(@implContent, "JSValue ${className}::getConstructor(JSContext *ctx)\n{\n");
        push(@implContent, "    return ${parentClassName}::getConstructor(ctx);\n");
        push(@implContent, "}\n\n");
    }

    # Functions
    if ($numFunctions ne 0) {
        push(@implContent, "JSValue ${className}PrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)\n{\n");
        push(@implContent, "    $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_val, ${className}::js_class_id);\n");
        push(@implContent, "    if (!imp)\n");
        push(@implContent, "        return JS_ThrowTypeError(ctx, \"Type error\"); \n\n");
        if ($podType) {
            push(@implContent, "    JSSVGPODTypeWrapper<$podType>* wrapper = castedThisObj->impl();\n");
            push(@implContent, "    $podType imp(*wrapper);\n\n");
        } else {
        }

        push(@implContent, "    switch (token) {\n");

        my $hasCustomFunctionsOnly = 1;

        foreach my $function (@{$dataNode->functions}) {
            push(@implContent, "        case ${className}::" . WK_ucfirst($function->signature->name) . "FuncNum: {\n");

            if ($function->signature->extendedAttributes->{"Custom"}) {
                push(@implContent, "            return ${className}::" . $function->signature->name . "(ctx, argc, argv, imp);\n        }\n");
                next;
            }

            $hasCustomFunctionsOnly = 0;
            AddIncludesForType($function->signature->type);

            if (@{$function->raisesExceptions}) {
                push(@implContent, "            ExceptionCode ec = 0;\n");
            }

            if ($function->signature->extendedAttributes->{"SVGCheckSecurityDocument"}) {
                push(@implContent, "            if (!checkNodeSecurity(ctx, imp->getSVGDocument(" . (@{$function->raisesExceptions} ? "ec" : "") .")))\n");
                push(@implContent, "                return JS_UNDEFINED;\n");
                $implIncludes{"qjs_dom.h"} = 1;
            }

            my $paramIndex = 0;
            my $functionString = "imp" . ($podType ? "." : "->") . $function->signature->name . "(";

            my $numParameters = @{$function->parameters};
            my $hasOptionalArguments = 0;

            foreach my $parameter (@{$function->parameters}) {
                if (!$hasOptionalArguments && $parameter->extendedAttributes->{"Optional"}) {
                    $hasOptionalArguments = 1;
                }

                if ($hasOptionalArguments) {
                    push(@implContent, "            if (argc < " . ($paramIndex + 1) . ") {\n");
                    GenerateImplementationFunctionCall($function, $functionString, $paramIndex, "      " x 3, $podType, $implClassName);
                    push(@implContent, "            }\n\n");
                }

                my $name = $parameter->name;
                
                if ($parameter->type eq "XPathNSResolver") {
                    push(@implContent, "            RefPtr<XPathNSResolver> customResolver;\n");
                    push(@implContent, "            XPathNSResolver* resolver = toXPathNSResolver(argv[$paramIndex]);\n");
                    push(@implContent, "            if (!resolver) {\n");
                    push(@implContent, "                customResolver = JSCustomXPathNSResolver::create(ctx, argv[$paramIndex]);\n");
                    push(@implContent, "                if (JS_HasException(ctx))\n");
                    push(@implContent, "                    return JS_UNDEFINED;\n");
                    push(@implContent, "                resolver = customResolver.get();\n");
                    push(@implContent, "            }\n");
                } else {
                    push(@implContent, "            bool ${name}Ok;\n") if TypeCanFailConversion($parameter);
                    push(@implContent, "            " . GetNativeTypeFromSignature($parameter) . " $name = " . JSValueToNative($parameter, "argv[$paramIndex]", TypeCanFailConversion($parameter) ? "${name}Ok" : undef) . ";\n");
                    if (TypeCanFailConversion($parameter)) {
                        push(@implContent, "            if (!${name}Ok) {\n");
                        push(@implContent, "                setDOMException(ctx, TYPE_MISMATCH_ERR);\n");
                        push(@implContent, "                return JS_UNDEFINED;\n            }\n");
                    }

                    # If a parameter is "an index", it should throw an INDEX_SIZE_ERR
                    # exception
                    if ($parameter->extendedAttributes->{"IsIndex"}) {
                        $implIncludes{"ExceptionCode.h"} = 1;
                        push(@implContent, "            if ($name < 0) {\n");
                        push(@implContent, "                setDOMException(ctx, INDEX_SIZE_ERR);\n");
                        push(@implContent, "                return JS_UNDEFINED;\n            }\n");
                    }
                }

                $functionString .= ", " if $paramIndex;
                $functionString .= $name;

                $paramIndex++;
            }

            GenerateImplementationFunctionCall($function, $functionString, $paramIndex, "      " x 2, $podType, $implClassName);

            push(@implContent, "        }\n"); # end case
        }
        push(@implContent, "    }\n"); # end switch
        push(@implContent, "    (void)imp;\n") if $hasCustomFunctionsOnly;
        push(@implContent, "    return JS_NULL;\n");
        push(@implContent, "}\n\n");
    }
    if ($dataNode->extendedAttributes->{"HasIndexGetter"}) {
        push(@implContent, "JSValue ${className}::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)\n");
        push(@implContent, "{\n");
        push(@implContent, "    $implClassName* imp = ($implClassName*)JS_GetOpaque2(ctx, this_obj, ${className}::js_class_id);\n");
        if (IndexGetterReturnsStrings($implClassName)) {
            $implIncludes{"text/String.h"} = 1;
            push(@implContent, "    return jsStringOrNull(ctx, imp->item(idx));\n");
        } else {
            push(@implContent, "    return toJS(ctx, imp->item(idx));\n");
        }
        push(@implContent, "}\n\n");
        if ($interfaceName eq "HTMLCollection") {
            $implIncludes{"QJSNode.h"} = 1;
            $implIncludes{"Node.h"} = 1;
        }
    }

    if ((!$hasParent or $dataNode->extendedAttributes->{"GenerateToJS"}) and !UsesManualToJSImplementation($implClassName)) {
        if ($podType) {
            push(@implContent, "JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<$podType>* obj, SVGElement* context)\n");
        } elsif (IsSVGTypeNeedingContextParameter($implClassName)) {
            push(@implContent, "JSValue toJS(JSContext *ctx, $passType obj, SVGElement* context)\n");
        } else {
            push(@implContent, "JSValue toJS(JSContext *ctx, $passType obj)\n");
        }

        push(@implContent, "{\n");
        if ($podType) {
            push(@implContent, "    return QJS::cacheSVGDOMObject<JSSVGPODTypeWrapper<$podType>, $className>(ctx, obj, context);\n");
        } elsif (IsSVGTypeNeedingContextParameter($implClassName)) {
            push(@implContent, "    return QJS::cacheSVGDOMObject<$implClassName, $className>(ctx, obj, context);\n");
        } else {
            push(@implContent, "    return QJS::cacheDOMObject<$implClassName, $className>(ctx, obj);\n");
        }
        push(@implContent, "}\n");
    }

    if ((!$hasParent or $dataNode->extendedAttributes->{"GenerateNativeConverter"}) and !$dataNode->extendedAttributes->{"CustomNativeConverter"}) {
        push(@implContent, "\n");
        if ($podType) {
            push(@implContent, "$podType to${interfaceName}(JSValue val)\n");
        } else {
            push(@implContent, "$implClassName* to${interfaceName}(JSValue val)\n");
        }

        push(@implContent, "{\n");

        push(@implContent, "    if (JS_IsObject(val)) {\n");
        push(@implContent, "        ${implClassName}* impl = (${implClassName}*)JS_GetOpaque(val, ${className}::js_class_id);\n");
        push(@implContent, "        return " .($podType ? "($podType) *" : "") ."impl;\n");
        push(@implContent, "    } else {\n");
        push(@implContent, "        return ");
        if ($podType and $podType ne "float") {
            push(@implContent, "$podType();\n");
        } else {
            push(@implContent, "0;\n");
            push(@implContent, "    }\n");
        }
        push(@implContent, "}\n");
    }

    if ($dataNode->extendedAttributes->{"GenerateNativeConverter"} && $hasParent) {
        #<Debug>#push(@implContent, "\n$implClassName* ${className}::impl() const\n");
        #<Debug>#push(@implContent, "{\n");
        #<Debug>#push(@implContent, "    return static_cast<$implClassName*>(${parentClassName}::impl());\n");
        #<Debug>#push(@implContent, "}\n");
    }

    push(@implContent, "\n}\n");

    push(@implContent, "\n#endif // ${conditionalString}\n") if $conditional;
}

sub GenerateImplementationFunctionCall()
{
    my $function = shift;
    my $functionString = shift;
    my $paramIndex = shift;
    my $indent = shift;
    my $podType = shift;
    my $implClassName = shift;

    if (@{$function->raisesExceptions}) {
        $functionString .= ", " if $paramIndex;
        $functionString .= "ec";
    }
    $functionString .= ")";

    if ($function->signature->type eq "void") {
        push(@implContent, $indent . "$functionString;\n");
        push(@implContent, $indent . "setDOMException(ctx, ec);\n") if @{$function->raisesExceptions};

        if ($podType) {
            push(@implContent, $indent . "wrapper->commitChange(exec, imp);\n\n");
            push(@implContent, $indent . "if (castedThisObj->context())\n");
            push(@implContent, $indent . "    castedThisObj->context()->notifyAttributeChange();\n");
        }

        push(@implContent, $indent . "return JS_UNDEFINED;\n");
    } else {
        push(@implContent, "\n" . $indent . "JSValue result = " . NativeToJSValue($function->signature, 1, $implClassName, "", $functionString) . ";\n");
        push(@implContent, $indent . "setDOMException(ctx, ec);\n") if @{$function->raisesExceptions};

        if ($podType) {
            push(@implContent, $indent . "wrapper->commitChange(exec, imp);\n\n");
            push(@implContent, $indent . "if (castedThisObj->context())\n");
            push(@implContent, $indent . "    castedThisObj->context()->notifyAttributeChange();\n");
        }

        push(@implContent, $indent . "return result;\n");
    }
}

sub GetNativeTypeFromSignature
{
    my $signature = shift;
    my $type = $codeGenerator->StripModule($signature->type);

    if ($type eq "unsigned long" and $signature->extendedAttributes->{"IsIndex"}) {
        # Special-case index arguments because we need to check that they aren't < 0.
        return "int";
    }

    return GetNativeType($type);
}

sub GetNativeType
{
    my $type = shift;

    return "unsigned" if $type eq "unsigned long";
    return $type if $type eq "unsigned short" or $type eq "float" or $type eq "double" or $type eq "AtomicString";
    return "bool" if $type eq "boolean";
    return "int" if $type eq "long";
    return "String" if $type eq "DOMString";
    return "Range::CompareHow" if $type eq "CompareHow";
    return "EventTargetNode*" if $type eq "EventTarget";
    return "FloatRect" if $type eq "SVGRect";
    return "FloatPoint" if $type eq "SVGPoint";
    return "AffineTransform" if $type eq "SVGMatrix";
    return "SVGTransform" if $type eq "SVGTransform";
    return "SVGLength" if $type eq "SVGLength";
    return "float" if $type eq "SVGNumber";
    return "SVGPaint::SVGPaintType" if $type eq "SVGPaintType";

    # Default, assume native type is a pointer with same type name as idl type
    return "${type}*";
}

sub TypeCanFailConversion
{
    my $signature = shift;

    my $type = $codeGenerator->StripModule($signature->type);

    # FIXME: convert to use a hash

    return 0 if $type eq "boolean" or
                $type eq "float" or
                $type eq "double" or
                $type eq "AtomicString" or
                $type eq "DOMString" or
                $type eq "Node" or
                $type eq "Element" or
                $type eq "DocumentType" or
                $type eq "Event" or
                $type eq "EventListener" or
                $type eq "EventTarget" or
                $type eq "Range" or
                $type eq "NodeFilter" or
                $type eq "DOMWindow" or
                $type eq "XPathEvaluator" or
                $type eq "XPathNSResolver" or
                $type eq "XPathResult" or
                $type eq "SVGAngle" or
                $type eq "SVGLength" or
                $type eq "SVGNumber" or
                $type eq "SVGPoint" or
                $type eq "SVGTransform" or
                $type eq "SVGPathSeg" or
                $type eq "SVGMatrix" or
                $type eq "SVGRect" or
                $type eq "SVGElement" or
                $type eq "HTMLElement" or
                $type eq "HTMLOptionElement" or
                $type eq "unsigned short" or # or can it?
                $type eq "CompareHow" or # or can it?
                $type eq "SVGPaintType"; # or can it?

    if ($type eq "unsigned long" or $type eq "long" or $type eq "Attr") {
        $implIncludes{"ExceptionCode.h"} = 1;
        return 1;
    }

    die "Don't know whether a JS value can fail conversion to type $type."
}

sub JSValueToNative
{
    my $signature = shift;
    my $value = shift;
    my $okParam = shift;
    my $maybeOkParam = $okParam ? ", ${okParam}" : "";

    my $type = $codeGenerator->StripModule($signature->type);

    return "valueToBoolean(ctx, $value)" if $type eq "boolean";
    return "valueToDouble(ctx, $value)" if $type eq "double";
    return "valueToFloat(ctx, $value)" if $type eq "float" or $type eq "SVGNumber";
    return "valueToInt32(ctx, $value${maybeOkParam})" if $type eq "unsigned long" or $type eq "long" or $type eq "unsigned short";

    return "static_cast<Range::CompareHow>(valueToInt32(ctx, $value))" if $type eq "CompareHow";
    return "static_cast<SVGPaint::SVGPaintType>(valueToInt32(ctx, $value))" if $type eq "SVGPaintType";

    return "valueToString(ctx, $value)" if $type eq "AtomicString";
    if ($type eq "DOMString") {
        return "valueToStringWithNullCheck(ctx, $value)" if $signature->extendedAttributes->{"ConvertNullToNullString"};
        return "valueToStringWithUndefinedOrNullCheck(ctx, $value)" if $signature->extendedAttributes->{"ConvertUndefinedOrNullToNullString"};
        return "valueToString(ctx, $value)";
    }

    if ($type eq "EventTarget") {
        $implIncludes{"QJSEventTargetNode.h"} = 1;
        return "toEventTargetNode($value)";
    }

    if ($type eq "Attr") {
        $implIncludes{"qjs_dom.h"} = 1;
        return "toAttr($value${maybeOkParam})";
    }

    if ($type eq "SVGRect") {
        $implIncludes{"FloatRect.h"} = 1;
    }

    if ($type eq "SVGPoint") {
        $implIncludes{"FloatPoint.h"} = 1;
    }

    # Default, assume autogenerated type conversion routines
    $implIncludes{"QJS$type.h"} = 1;
    return "to$type($value)";
}

sub NativeToJSValue
{
    my $signature = shift;
    my $inFunctionCall = shift;
    my $implClassName = shift;
    my $implClassNameForValueConversion = shift;
    my $value = shift;

    my $type = $codeGenerator->StripModule($signature->type);

    return "JS_NewBool(ctx, $value ? 1 : 0)" if $type eq "boolean";
    return "JS_NewBigUint64(ctx, $value)" if $codeGenerator->IsPrimitiveType($type) or $type eq "SVGPaintType" or $type eq "DOMTimeStamp";

    if ($codeGenerator->IsStringType($type)) {
        $implIncludes{"text/String.h"} = 1;
        my $conv = $signature->extendedAttributes->{"ConvertNullStringTo"};
        if (defined $conv) {
            return "jsStringOrNull(ctx, $value)" if $conv eq "Null";
            return "jsStringOrUndefined(ctx, $value)" if $conv eq "Undefined";
            return "jsStringOrFalse(ctx, $value)" if $conv eq "False";

            die "Unknown value for ConvertNullStringTo extended attribute";
        }
        return "JS_NewString(ctx, ((const String&)${value}).utf8().data())";
    }

    if ($type eq "RGBColor") {
        $implIncludes{"qjs_css.h"} = 1;
        return "getJSRGBColor(ctx, $value)";
    }

    if ($codeGenerator->IsPodType($type)) {
        $implIncludes{"QJS$type.h"} = 1;

        my $nativeType = GetNativeType($type);

        my $getter = $value;
        $getter =~ s/imp->//;
        $getter =~ s/\(\)//;

        my $setter = "set" . WK_ucfirst($getter);

        if ($implClassNameForValueConversion eq "") {
            if (IsSVGTypeNeedingContextParameter($implClassName)) {
                return "toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<$nativeType>($value), castedThisObj->context())" if $inFunctionCall eq 1;

                # Special case: SVGZoomEvent - it doesn't have a context, but it's no problem, as there are no readwrite props
                return "toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<$nativeType>($value), 0)" if $implClassName eq "SVGZoomEvent";
                return "toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<$nativeType>($value), context())";
            } else {
                return "toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<$nativeType>($value), imp)";
            }
        } else { # These classes, always have a m_context pointer!
            return "toJS(ctx, JSSVGPODTypeWrapperCache<$nativeType, $implClassNameForValueConversion>::lookupOrCreateWrapper(imp, &${implClassNameForValueConversion}::$getter, &${implClassNameForValueConversion}::$setter), context())";
        }
    }

    if ($codeGenerator->IsSVGAnimatedType($type)) {
        $value =~ s/\(\)//;
        $value .= "Animated()";
    }

    if ($type eq "CSSStyleDeclaration") {
        $implIncludes{"CSSMutableStyleDeclaration.h"} = 1;
    }

    if ($type eq "NamedNodeMap") {
        $implIncludes{"NamedAttrMap.h"} = 1;
    }

    if ($type eq "NodeList") {
        $implIncludes{"NameNodeList.h"} = 1;
    }

    if ($type eq "EventTarget") {
        $implIncludes{"EventTargetNode.h"} = 1;
        $implIncludes{"QJSEventTargetNode.h"} = 1;
        $implIncludes{"qjs_dom.h"} = 1;
    } elsif ($type eq "DOMWindow") {
        $implIncludes{"qjs_window.h"} = 1;
    } elsif ($type eq "DOMObject") {
        $implIncludes{"QJSCanvasRenderingContext2D.h"} = 1;
    } elsif ($type eq "Clipboard") {
        $implIncludes{"qjs_events.h"} = 1;
        $implIncludes{"Clipboard.h"} = 1;
    } elsif ($type =~ /SVGPathSeg/) {
        $implIncludes{"QJS$type.h"} = 1;
        $joinedName = $type;
        $joinedName =~ s/Abs|Rel//;
        $implIncludes{"$joinedName.h"} = 1;
    } else {
        # Default, include header with same name.
        $implIncludes{"QJS$type.h"} = 1;
        $implIncludes{"$type.h"} = 1;
    }

    return $value if $codeGenerator->IsSVGAnimatedType($type);

    if (IsSVGTypeNeedingContextParameter($type)) {
        if (IsSVGTypeNeedingContextParameter($implClassName)) {
            if ($inFunctionCall eq 1) {
                return "toJS(ctx, QJS::getPtr($value), castedThisObj->context())";
            } else {
                return "toJS(ctx, QJS::getPtr($value), context())";
            }
        } else {
            return "toJS(ctx, QJS::getPtr($value), imp)";
        }
    }

    return "toJS(ctx, QJS::getPtr($value))";
}

# Internal Helper
sub GenerateAttributesTable
{
    my $object = shift;

    my $name = shift;
    my $size = shift;
    my $type = shift;
    my $keys = shift;
    my $values = shift;
    my $readonly = shift;

    # Helpers
    my @table = ();
    my @links = ();

    my $maxDepth = 0;
    my $collisions = 0;
    my $numEntries = $size;

    # Collect hashtable information
    my $i = 0;
    foreach (@{$keys}) {
        my $depth = 0;
        my $h = $object->GenerateHashValue($_) % $numEntries;

        while (defined($table[$h])) {
            if (defined($links[$h])) {
                $h = $links[$h];
                $depth++;
            } else {
                $collisions++;
                $links[$h] = $size;
                $h = $size;
                $size++;
            }
        }

        $table[$h] = $i;

        $i++;
        $maxDepth = $depth if ($depth > $maxDepth);
    }

    # Ensure table is big enough (in case of undef entries at the end)
    if ($#table + 1 < $size) {
        $#table = $size - 1;
    }

    # Start outputing the hashtables
    my $nameEntries = "". ${name}.${type}."Functions";
    $nameEntries =~ s/:/_/g;

    push(@implContent, "/* Functions table */\n");

    # Dump the hash table
    push(@implContent, "\nstatic const JSCFunctionListEntry $nameEntries\[\] =\n\{\n");

    $i = 0;
    foreach $entry (@table) {
        if (defined($entry)) {
            my $key = @$keys[$entry];

            push(@implContent, "    JS_CGETSET_MAGIC_DEF(\"" . $key . "\"");
            push(@implContent, ", " . $name . "::" . "getValueProperty");
            if (@$readonly[$entry] eq "1") {
                push(@implContent, ", NULL");
            } else {
                push(@implContent, ", " . $name . "::" . "putValueProperty");
            }
            push(@implContent, ", " . @$values[$entry]);
            push(@implContent, ")");

            push(@implContent, ",") unless($i eq $size - 1);
            push(@implContent, "\n");
        } 
        $i++;
    }
    push(@implContent, "};\n\n");
}

# proto functions table
sub GeneratePrototypeFuncTable
{
    my $object = shift;

    my $name = shift;
    my $size = shift;
    my $keys = shift;
    my $values = shift;
    my $parameters = shift;

    # Helpers
    my @table = ();
    my @links = ();

    my $maxDepth = 0;
    my $collisions = 0;
    my $numEntries = $size;

    # Collect hashtable information
    my $i = 0;
    foreach (@{$keys}) {
        my $depth = 0;
        my $h = $object->GenerateHashValue($_) % $numEntries;

        while (defined($table[$h])) {
            if (defined($links[$h])) {
                $h = $links[$h];
                $depth++;
            } else {
                $collisions++;
                $links[$h] = $size;
                $h = $size;
                $size++;
            }
        }

        $table[$h] = $i;

        $i++;
        $maxDepth = $depth if ($depth > $maxDepth);
    }

    # Ensure table is big enough (in case of undef entries at the end)
    if ($#table + 1 < $size) {
        $#table = $size - 1;
    }

    # Start outputing the hashtables
    my $nameEntries = "${name}Functions";
    $nameEntries =~ s/:/_/g;

    push(@implContent, "/* Prototype functions table */\n");

    # Dump the hash table
    push(@implContent, "\nstatic const JSCFunctionListEntry $nameEntries\[\] =\n\{\n");

    $i = 0;
    foreach $entry (@table) {
        if (defined($entry)) {
            my $key = @$keys[$entry];

            push(@implContent, "    JS_CFUNC_MAGIC_DEF(\"" . $key . "\"");
            push(@implContent, ", " . @$parameters[$entry]);
            push(@implContent, ", " . $name . "Function::" . "callAsFunction");
            push(@implContent, ", " . @$values[$entry]);
            push(@implContent, ")");
            push(@implContent, ",") unless($i eq $size - 1);
            push(@implContent, "\n");
        } 
        $i++;
    }
    push(@implContent, "};\n\n");
}


sub GenerateHashTable
{
    my $object = shift;

    my $name = shift;
    my $size = shift;
    my $keys = shift;
    my $values = shift;
    my $parameters = shift;
    my $readonly = shift;

    # Helpers
    my @table = ();
    my @links = ();

    my $maxDepth = 0;
    my $collisions = 0;
    my $numEntries = $size;

    # Collect hashtable information
    my $i = 0;
    foreach (@{$keys}) {
        my $depth = 0;
        my $h = $object->GenerateHashValue($_) % $numEntries;

        while (defined($table[$h])) {
            if (defined($links[$h])) {
                $h = $links[$h];
                $depth++;
            } else {
                $collisions++;
                $links[$h] = $size;
                $h = $size;
                $size++;
            }
        }

        $table[$h] = $i;

        $i++;
        $maxDepth = $depth if ($depth > $maxDepth);
    }

    # Ensure table is big enough (in case of undef entries at the end)
    if ($#table + 1 < $size) {
        $#table = $size - 1;
    }

    # Start outputing the hashtables
    my $nameEntries = "${name}Functions";
    $nameEntries =~ s/:/_/g;

    # first, build the string table
    my %soffset = ();
    if (($name =~ /Prototype/) or ($name =~ /Constructor/)) {
        my $type = $name;
        my $implClass;

        if ($name =~ /Prototype/) {
            $type =~ s/Prototype.*//;
            $implClass = $type; $implClass =~ s/Wrapper$//;
            push(@implContent, "/* Functions table for prototype */\n");
        } else {
            $type =~ s/Constructor.*//;
            $implClass = $type; $implClass =~ s/Constructor$//;
            push(@implContent, "/* Functions table for constructor */\n");
        }
    } else {
        push(@implContent, "/* Functions table */\n");
    }

    # Dump the hash table
    push(@implContent, "\nstatic const JSCFunctionListEntry $nameEntries\[\] =\n\{\n");

    $i = 0;
    foreach $entry (@table) {
        if (defined($entry)) {
            my $key = @$keys[$entry];

            push(@implContent, "    JS_CGETSET_MAGIC_DEF(\"" . $key . "\"");
            push(@implContent, ", " . $name . "::" . "getValueProperty");
            if (@$readonly[$entry] eq "1") {
                push(@implContent, ", NULL");
            } else {
                push(@implContent, ", " . $name . "::" . "putValueProperty");
            }
            push(@implContent, ", " . @$values[$entry]);
            push(@implContent, ")");

            push(@implContent, ",") unless($i eq $size - 1);
            push(@implContent, "\n");
        } 
        $i++;
    }
    push(@implContent, "};\n\n");
}

# Internal helper
sub GenerateHashValue
{
    my $object = shift;

    @chars = split(/ */, $_[0]);

    # This hash is designed to work on 16-bit chunks at a time. But since the normal case
    # (above) is to hash UTF-16 characters, we just treat the 8-bit chars as if they
    # were 16-bit chunks, which should give matching results

    my $EXP2_32 = 4294967296;

    my $hash = 0x9e3779b9;
    my $l    = scalar @chars; #I wish this was in Ruby --- Maks
    my $rem  = $l & 1;
    $l = $l >> 1;

    my $s = 0;

    # Main loop
    for (; $l > 0; $l--) {
        $hash   += ord($chars[$s]);
        my $tmp = leftShift(ord($chars[$s+1]), 11) ^ $hash;
        $hash   = (leftShift($hash, 16)% $EXP2_32) ^ $tmp;
        $s += 2;
        $hash += $hash >> 11;
        $hash %= $EXP2_32;
    }

    # Handle end case
    if ($rem != 0) {
        $hash += ord($chars[$s]);
        $hash ^= (leftShift($hash, 11)% $EXP2_32);
        $hash += $hash >> 17;
    }

    # Force "avalanching" of final 127 bits
    $hash ^= leftShift($hash, 3);
    $hash += ($hash >> 5);
    $hash = ($hash% $EXP2_32);
    $hash ^= (leftShift($hash, 2)% $EXP2_32);
    $hash += ($hash >> 15);
    $hash = $hash% $EXP2_32;
    $hash ^= (leftShift($hash, 10)% $EXP2_32);

    # this avoids ever returning a hash code of 0, since that is used to
    # signal "hash not computed yet", using a value that is likely to be
    # effectively the same as 0 when the low bits are masked
    $hash = 0x80000000 if ($hash == 0);

    return $hash;
}

# Internal helper
sub WriteData
{
    if (defined($IMPL)) {
        # Write content to file.
        print $IMPL @implContentHeader;

        foreach my $implInclude (sort keys(%implIncludes)) {
            my $checkType = $implInclude;
            $checkType =~ s/\.h//;

            print $IMPL "#include \"$implInclude\"\n" unless $codeGenerator->IsSVGAnimatedType($checkType);
        }

        print $IMPL @implContent;
        close($IMPL);
        undef($IMPL);

        @implHeaderContent = ();
        @implContent = ();
        %implIncludes = ();
    }

    if (defined($HEADER)) {
        # Write content to file.
        print $HEADER @headerContent;
        close($HEADER);
        undef($HEADER);

        @headerContent = ();
    }
}

sub constructorFor
{
    my $className = shift;
    my $protoClassName = shift;
    my $interfaceName = shift;
    my $canConstruct = shift;

my $implContent = << "EOF";
class ${className}Constructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
EOF

    if ($canConstruct) {
$implContent .= << "EOF";
    static JSValue construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv);
EOF
    }

$implContent .= << "EOF";
};

JSValue ${className}Constructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

EOF

    return $implContent;
}

sub prototypeFunctionFor
{
    my $className = shift;

my $implContent = << "EOF";
class ${className}PrototypeFunction {
public:
    static JSValue callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token);
};

EOF

    return $implContent;
}

1;
