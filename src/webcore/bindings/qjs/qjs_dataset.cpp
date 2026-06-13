/*
 * Agave - A lightweight web browser engine
 *
 * Copyright (c) 2026, Zhang Ji Peng
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

#include "config.h"

#include "qjs_dataset.h"

#include "Attribute.h"
#include "Element.h"
#include "ExceptionCode.h"
#include "NamedAttrMap.h"
#include "PlatformString.h"
#include "qjs_binding.h"
#include <wtf/Vector.h>

namespace WebCore {

using namespace QJS;

static JSClassID gDatasetClassId = 0;

// data-* attribute prefix.
static const char* const kDataPrefix = "data-";
static const unsigned kDataPrefixLen = 5;

// Converts a dataset property name (e.g. "fooBar") to its attribute name
// ("data-foo-bar"): each uppercase letter becomes "-" + lowercase. Returns an
// empty string if the name is invalid (an uppercase letter immediately after a
// hyphen has no valid attribute mapping, per the HTML spec).
static String datasetNameToAttr(const String& prop)
{
    Vector<UChar> out;
    out.append('d'); out.append('a'); out.append('t'); out.append('a'); out.append('-');
    const UChar* s = prop.characters();
    unsigned len = prop.length();
    for (unsigned i = 0; i < len; ++i) {
        UChar c = s[i];
        if (c == '-' && i + 1 < len && s[i + 1] >= 'a' && s[i + 1] <= 'z')
            return String(); // "-" followed by lowercase is not representable
        if (c >= 'A' && c <= 'Z') {
            out.append('-');
            out.append(c - 'A' + 'a');
        } else {
            out.append(c);
        }
    }
    return String(out.data(), out.size());
}

// Converts a data-* attribute name ("data-foo-bar") to its dataset property
// name ("fooBar"): "-" + lowercase letter becomes the uppercase letter.
static String attrNameToDataset(const String& attr)
{
    if (attr.length() < kDataPrefixLen)
        return String();
    Vector<UChar> out;
    const UChar* s = attr.characters();
    unsigned len = attr.length();
    for (unsigned i = kDataPrefixLen; i < len; ++i) {
        UChar c = s[i];
        if (c == '-' && i + 1 < len && s[i + 1] >= 'a' && s[i + 1] <= 'z') {
            out.append(s[i + 1] - 'a' + 'A');
            ++i;
        } else {
            out.append(c);
        }
    }
    return String(out.data(), out.size());
}

static Element* datasetElement(JSValueConst obj)
{
    return (Element*)JS_GetOpaque(obj, gDatasetClassId);
}

static void datasetFinalizer(JSRuntime*, JSValue val)
{
    Element* e = (Element*)JS_GetOpaque(val, gDatasetClassId);
    if (e)
        e->deref();
}

// Exotic get: reads element.getAttribute("data-...").
static int datasetGetOwnProperty(JSContext* ctx, JSPropertyDescriptor* desc,
                                 JSValueConst obj, JSAtom prop)
{
    Element* e = datasetElement(obj);
    if (!e)
        return 0;
    const char* cname = JS_AtomToCString(ctx, prop);
    if (!cname)
        return 0;
    String attrName = datasetNameToAttr(String::fromUTF8(cname));
    JS_FreeCString(ctx, cname);
    if (attrName.isEmpty() || !e->hasAttribute(attrName))
        return 0; // property does not exist
    if (desc) {
        desc->flags = JS_PROP_ENUMERABLE | JS_PROP_CONFIGURABLE | JS_PROP_WRITABLE;
        desc->value = jsString(ctx, e->getAttribute(attrName));
        desc->getter = JS_UNDEFINED;
        desc->setter = JS_UNDEFINED;
    }
    return 1; // property exists
}

// Exotic set: writes/removes the data-* attribute.
static int datasetSetProperty(JSContext* ctx, JSValueConst obj, JSAtom prop,
                              JSValueConst value, JSValueConst, int flags)
{
    Element* e = datasetElement(obj);
    if (!e)
        return -1;
    const char* cname = JS_AtomToCString(ctx, prop);
    if (!cname)
        return -1;
    String attrName = datasetNameToAttr(String::fromUTF8(cname));
    JS_FreeCString(ctx, cname);
    if (attrName.isEmpty())
        return JS_ThrowTypeError(ctx, "Invalid dataset name"), -1;
    String v = valueToString(ctx, value);
    ExceptionCode ec = 0;
    e->setAttribute(attrName, v, ec);
    return 1;
}

// Exotic enumerate: lists each data-* attribute as a camelCase name.
static int datasetGetOwnPropertyNames(JSContext* ctx, JSPropertyEnum** ptab,
                                      uint32_t* plen, JSValueConst obj)
{
    Element* e = datasetElement(obj);
    NamedAttrMap* attrs = e ? e->attributes(true) : 0;
    Vector<String> names;
    if (attrs) {
        for (unsigned i = 0; i < attrs->length(); ++i) {
            Attribute* a = attrs->attributeItem(i);
            String local = a->name().localName();
            if (local.startsWith(kDataPrefix))
                names.append(attrNameToDataset(local));
        }
    }

    JSPropertyEnum* tab = (JSPropertyEnum*)js_malloc(ctx, sizeof(JSPropertyEnum) * (names.size() ? names.size() : 1));
    if (!tab)
        return -1;
    for (unsigned i = 0; i < names.size(); ++i) {
        CString utf8 = names[i].utf8();
        tab[i].atom = JS_NewAtom(ctx, utf8.data());
        tab[i].is_enumerable = 1;
    }
    *ptab = tab;
    *plen = names.size();
    return 0;
}

static JSClassExoticMethods gDatasetExotic = {
    0, // get_own_property is set below (can't use designated init in C++98)
};

void initDatasetClass(JSContext* ctx)
{
    if (gDatasetClassId)
        return;
    JS_NewClassID(&gDatasetClassId);

    gDatasetExotic.get_own_property = datasetGetOwnProperty;
    gDatasetExotic.get_own_property_names = datasetGetOwnPropertyNames;
    gDatasetExotic.set_property = datasetSetProperty;

    JSClassDef def;
    memset(&def, 0, sizeof(def));
    def.class_name = "DOMStringMap";
    def.finalizer = datasetFinalizer;
    def.exotic = &gDatasetExotic;
    JS_NewClass(JS_GetRuntime(ctx), gDatasetClassId, &def);
}

JSValue createDatasetObject(JSContext* ctx, Element* element)
{
    if (!element)
        return JS_NULL;
    if (!gDatasetClassId)
        initDatasetClass(ctx);
    JSValue obj = JS_NewObjectClass(ctx, gDatasetClassId);
    if (JS_IsException(obj))
        return obj;
    element->ref();
    JS_SetOpaque(obj, element);
    return obj;
}

} // namespace WebCore
