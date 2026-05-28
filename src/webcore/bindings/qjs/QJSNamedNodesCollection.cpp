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
#include <string.h>
#include "QJSNamedNodesCollection.h"

#if ENABLE(QJS)

#include "AtomicString.h"
#include "NamedAttrMap.h"
#include "Node.h"
#include "qjs_dom.h"

namespace WebCore {

using namespace QJS;

#define countof(x) (sizeof(x) / sizeof((x)[0]))

/* Functions table */

static JSCFunctionListEntry JSNamedNodesCollectionAttributesFunctions[1];
static bool JSNamedNodesCollectionAttributesFunctions_inited = false;

static void init_JSNamedNodesCollectionAttributesFunctions()
{
    if (JSNamedNodesCollectionAttributesFunctions_inited) return;
    JSNamedNodesCollectionAttributesFunctions_inited = true;
    memset(JSNamedNodesCollectionAttributesFunctions, 0, sizeof(JSNamedNodesCollectionAttributesFunctions));
    JSNamedNodesCollectionAttributesFunctions[0].name = "length";
    JSNamedNodesCollectionAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNamedNodesCollectionAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSNamedNodesCollectionAttributesFunctions[0].magic = JSNamedNodesCollection::LengthAttrNum;
    JSNamedNodesCollectionAttributesFunctions[0].u.getset.get.getter_magic = JSNamedNodesCollection::getValueProperty;
    JSNamedNodesCollectionAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

static JSClassDef JSNamedNodesCollectionClassDefine;
static bool JSNamedNodesCollectionClassDefine_inited = false;

static void init_JSNamedNodesCollectionClassDefine()
{
    if (JSNamedNodesCollectionClassDefine_inited) return;
    JSNamedNodesCollectionClassDefine_inited = true;
    memset(&JSNamedNodesCollectionClassDefine, 0, sizeof(JSNamedNodesCollectionClassDefine));
    JSNamedNodesCollectionClassDefine.class_name = "NamedNodesCollection";
    JSNamedNodesCollectionClassDefine.finalizer = JSNamedNodesCollection::finalizer;
}

JSClassID JSNamedNodesCollection::js_class_id = 0;

void JSNamedNodesCollection::init(JSContext* ctx)
{
    if (JSNamedNodesCollection::js_class_id == 0) {
        init_JSNamedNodesCollectionClassDefine();
        JS_NewClassID(&JSNamedNodesCollection::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNamedNodesCollection::js_class_id, &JSNamedNodesCollectionClassDefine);

        init_JSNamedNodesCollectionAttributesFunctions();
        JSValue proto = JS_NewObject(ctx);
        JS_SetPropertyFunctionList(ctx, proto, JSNamedNodesCollectionAttributesFunctions, countof(JSNamedNodesCollectionAttributesFunctions));

        JS_SetClassProto(ctx, JSNamedNodesCollection::js_class_id, proto);
    }
}

JSValue JSNamedNodesCollection::create(JSContext* ctx, const Vector<RefPtr<Node> >& nodes)
{
    JSNamedNodesCollection::init(ctx);

    JSValue obj = JS_NewObjectClass(ctx, JSNamedNodesCollection::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    
    JSNamedNodesCollection * impl = new JSNamedNodesCollection(nodes);
    JS_SetOpaque(obj, impl);
    return obj;
}

void JSNamedNodesCollection::finalizer(JSRuntime *rt, JSValue val)
{
    JSNamedNodesCollection* imp = (JSNamedNodesCollection*)JS_GetOpaque(val, JSNamedNodesCollection::js_class_id);
    delete imp;
}

JSValue JSNamedNodesCollection::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            JSNamedNodesCollection* imp = (JSNamedNodesCollection*)JS_GetOpaque2(ctx, this_val, JSNamedNodesCollection::js_class_id);
            return JS_NewBigUint64(ctx, imp->m_nodes.size());
        }
    }
    return JS_NULL;
}

// Such a collection is usually very short-lived, it only exists
// for constructs like document.forms.<name>[1],
// so it shouldn't be a problem that it's storing all the nodes (with the same name). (David)
JSNamedNodesCollection::JSNamedNodesCollection(const Vector<RefPtr<Node> >& nodes)
    : m_nodes(nodes)
{
}

#if 0
JSValue* JSNamedNodesCollection::indexGetter(ExecState* exec, JSObject* originalObject, const Identifier& propertyName, const PropertySlot& slot)
{
    JSNamedNodesCollection *thisObj = static_cast<JSNamedNodesCollection*>(slot.slotBase());
    return toJS(exec, thisObj->m_nodes[slot.index()].get());
}
#endif

} // namespace WebCore
#endif
