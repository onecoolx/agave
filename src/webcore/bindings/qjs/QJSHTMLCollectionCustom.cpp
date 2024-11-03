/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
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

#if ENABLE(QJS)
#include "QJSHTMLCollection.h"

#include "AtomicString.h"
#include "HTMLCollection.h"
#include "HTMLOptionsCollection.h"
#include "QJSHTMLAllCollection.h"
#include "QJSHTMLOptionsCollection.h"
#include "QJSNamedNodesCollection.h"
#include "QJSNode.h"
#include "Node.h"
#include "qjs_binding.h"
#include "qjs_html.h"
#include <wtf/Vector.h>

using namespace QJS;

namespace WebCore {

static JSValue getNamedItems(JSContext* ctx, HTMLCollection* impl, const String& propertyName)
{
    Vector<RefPtr<Node> > namedItems;
    impl->namedItems(propertyName, namedItems);

    if (namedItems.isEmpty())
        return JS_UNDEFINED;

    if (namedItems.size() == 1)
        return toJS(ctx, namedItems[0].get());

    return JSNamedNodesCollection::create(ctx, namedItems);
}

// HTMLCollections are strange objects, they support both get and call,
// so that document.forms.item(0) and document.forms(0) both work.
JSValue JSHTMLCollection::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if (argc < 1)
        return JS_UNDEFINED;

    // Do not use thisObj here. It can be the JSHTMLDocument, in the document.forms(i) case.
    HTMLCollection* collection = (HTMLCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);

    // Also, do we need the TypeError test here ?

    if (argc == 1) {
        // Support for document.all(<index>) etc.
        bool ok;
        String string = valueToString(ctx, argv[0]);
        unsigned index = (unsigned)string.toDouble(&ok);
        if (ok)
            return toJS(ctx, collection->item(index));

        // Support for document.images('<name>') etc.
        return getNamedItems(ctx, collection, string);
    }

    // The second arg, if set, is the index of the item we want
    bool ok;
    String string = valueToString(ctx, argv[0]);
    unsigned index = (unsigned)valueToString(ctx, argv[1]).toDouble(&ok);
    if (ok) {
        String pstr = string;
        Node* node = collection->namedItem(pstr);
        while (node) {
            if (!index)
                return toJS(ctx, node);
            node = collection->nextNamedItem(pstr);
            --index;
        }
    }

    return JS_UNDEFINED;
}

JSValue JSHTMLCollection::nameGetter(JSContext *ctx, JSValueConst this_val, const char* propertyName)
{
    HTMLCollection* impl = (HTMLCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);
    return getNamedItems(ctx, impl, propertyName);
}

JSValue JSHTMLCollection::item(JSContext *ctx, int argc, JSValueConst *argv, HTMLCollection *impl)
{
    bool ok;
    String string = valueToString(ctx, argv[0]);
    unsigned index = (unsigned)string.toDouble(&ok);
    if (ok)
        return toJS(ctx, impl->item(index));
    return getNamedItems(ctx, impl, string);
}

JSValue JSHTMLCollection::namedItem(JSContext* ctx, int argc, JSValueConst *argv, HTMLCollection *impl)
{
    return getNamedItems(ctx, impl, valueToString(ctx, argv[0]));
}

JSValue toJS(JSContext* ctx, HTMLCollection* collection)
{
    if (!collection)
        return JS_NULL;

    QJS::ScriptInterpreter* interp = static_cast<QJS::ScriptInterpreter*>(JS_GetContextOpaque(ctx));
    JSValue ret = QJS::ScriptInterpreter::getDOMObject(interp, collection);

    if (!JS_IsNull(ret)) {
        return ret;
    }

    switch (collection->collectionType()) {
        case HTMLCollection::SelectOptions:
            ret = JSHTMLOptionsCollection::create(ctx, static_cast<HTMLOptionsCollection*>(collection));
            break;
        case HTMLCollection::DocAll:
            ret = JSHTMLAllCollection::create(ctx, static_cast<HTMLCollection*>(collection));
            break;
        default:
            ret = JSHTMLCollection::create(ctx, static_cast<HTMLCollection*>(collection));
            break;
    }

    QJS::ScriptInterpreter::putDOMObject(interp, collection, ret);
    return ret;
}

} // namespace WebCore
#endif
