/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
 * All rights reserved.
 */

#include "config.h"

#if ENABLE(QJS)

#include "QJSCSSStyleDeclaration.h"
#include "QJSNodeList.h"
#include "QJSNamedNodeMap.h"
#include "QJSStyleSheetList.h"
#include "QJSEvent.h"
#include "QJSHTMLFormElement.h"

#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "CSSPrimitiveValue.h"
#include "Event.h"
#include "HTMLFormElement.h"
#include "HTMLStyleElement.h"
#include "NamedNodeMap.h"
#include "Node.h"
#include "NodeList.h"
#include "StyleSheet.h"
#include "StyleSheetList.h"

#include "qjs_binding.h"

using namespace QJS;

namespace WebCore {

// --- JSCSSStyleDeclaration ---

static String cssPropertyName(const char* name)
{
    if (!name || !name[0])
        return String();

    Vector<char> buf;
    for (int i = 0; name[i]; ++i) {
        char c = name[i];
        if (c >= 'A' && c <= 'Z') {
            buf.append('-');
            buf.append(c - 'A' + 'a');
        } else {
            buf.append(c);
        }
    }
    buf.append('\0');

    String prop(buf.data());
    if (prop.startsWith("webkit-") || prop.startsWith("khtml-"))
        prop = "-" + prop;
    return prop;
}

JSValue JSCSSStyleDeclaration::nameGetter(JSContext* ctx, JSValueConst this_obj, const char* prop)
{
    CSSStyleDeclaration* impl = (CSSStyleDeclaration*)JS_GetOpaque(this_obj, JSCSSStyleDeclaration::js_class_id);
    if (!impl)
        return JS_UNDEFINED;

    String cssProp = cssPropertyName(prop);
    String value = impl->getPropertyValue(cssProp);
    if (value.isNull())
        return JS_NewString(ctx, "");
    return jsString(ctx, value);
}

// --- JSNodeList ---

JSValue JSNodeList::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    NodeList* impl = (NodeList*)JS_GetOpaque(this_val, JSNodeList::js_class_id);
    if (!impl || argc < 1)
        return JS_NULL;
    unsigned index = valueToInt32(ctx, argv[0]);
    Node* node = impl->item(index);
    if (!node)
        return JS_NULL;
    return toJS(ctx, node);
}

JSValue JSNodeList::nameGetter(JSContext* ctx, JSValueConst this_obj, const char* prop)
{
    NodeList* impl = (NodeList*)JS_GetOpaque(this_obj, JSNodeList::js_class_id);
    if (!impl)
        return JS_NULL;
    return toJS(ctx, impl->itemWithName(AtomicString(prop)));
}

// --- JSNamedNodeMap ---

JSValue JSNamedNodeMap::nameGetter(JSContext* ctx, JSValueConst this_obj, const char* prop)
{
    NamedNodeMap* impl = (NamedNodeMap*)JS_GetOpaque(this_obj, JSNamedNodeMap::js_class_id);
    if (!impl)
        return JS_NULL;
    return toJS(ctx, impl->getNamedItem(String(prop)).get());
}

// --- JSStyleSheetList ---

JSValue JSStyleSheetList::nameGetter(JSContext* ctx, JSValueConst this_obj, const char* prop)
{
    StyleSheetList* impl = (StyleSheetList*)JS_GetOpaque(this_obj, JSStyleSheetList::js_class_id);
    if (!impl)
        return JS_NULL;
    HTMLStyleElement* elem = impl->getNamedItem(String(prop));
    if (!elem)
        return JS_NULL;
    return toJS(ctx, PassRefPtr<Node>(elem));
}

// --- JSHTMLFormElement ---

JSValue JSHTMLFormElement::nameGetter(JSContext* ctx, JSValueConst this_obj, const char* prop)
{
    HTMLFormElement* impl = (HTMLFormElement*)JS_GetOpaque(this_obj, JSHTMLFormElement::js_class_id);
    if (!impl)
        return JS_NULL;

    Vector<RefPtr<Node> > nodes;
    impl->getNamedElements(AtomicString(prop), nodes);
    if (nodes.isEmpty())
        return JS_UNDEFINED;
    if (nodes.size() == 1)
        return toJS(ctx, nodes[0].get());
    return toJS(ctx, nodes[0].get());
}

} // namespace WebCore

#endif
