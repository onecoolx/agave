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

#if ENABLE(MUTATION_OBSERVERS)

#include "qjs_mutation_observer.h"

#include "MutationObserver.h"
#include "MutationObserverRegistry.h"
#include "MutationRecord.h"
#include "Node.h"
#include "NodeList.h"
#include "QJSNode.h"
#include "QJSNodeList.h"
#include "qjs_binding.h"

namespace WebCore {

using namespace QJS;

static JSClassID gObserverClassId = 0;

// Builds a plain JS object mirroring a MutationRecord (inline, like fetch's
// Response object, avoiding a separate generated wrapper class).
static JSValue buildRecordObject(JSContext* ctx, MutationRecord* record)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "type", jsString(ctx, record->type()));
    JS_SetPropertyStr(ctx, obj, "target", record->target() ? toJS(ctx, record->target()) : JS_NULL);
    JS_SetPropertyStr(ctx, obj, "addedNodes",
        record->addedNodes() ? toJS(ctx, record->addedNodes()) : JS_NULL);
    JS_SetPropertyStr(ctx, obj, "removedNodes",
        record->removedNodes() ? toJS(ctx, record->removedNodes()) : JS_NULL);
    JS_SetPropertyStr(ctx, obj, "previousSibling",
        record->previousSibling() ? toJS(ctx, record->previousSibling()) : JS_NULL);
    JS_SetPropertyStr(ctx, obj, "nextSibling",
        record->nextSibling() ? toJS(ctx, record->nextSibling()) : JS_NULL);
    JS_SetPropertyStr(ctx, obj, "attributeName",
        record->attributeName().isEmpty() ? JS_NULL : jsString(ctx, record->attributeName()));
    JS_SetPropertyStr(ctx, obj, "oldValue",
        record->hasOldValue() ? jsString(ctx, record->oldValue()) : JS_NULL);
    return obj;
}

// Bridges the C++ MutationObserver to its JS callback. Holds the JS callback
// function and the JS wrapper object (both GC-protected via JS_DupValue).
class QJSMutationCallback : public MutationCallback {
public:
    QJSMutationCallback(JSContext* ctx, JSValue callback)
        : m_ctx(ctx), m_callback(JS_DupValue(ctx, callback)), m_jsObject(JS_UNDEFINED) { }

    virtual ~QJSMutationCallback()
    {
        JS_FreeValue(m_ctx, m_callback);
    }

    void setJSObject(JSValue obj) { m_jsObject = obj; } // not duplicated; owned by runtime

    virtual void deliver(const Vector<RefPtr<MutationRecord> >& records, MutationObserver*)
    {
        if (!JS_IsFunction(m_ctx, m_callback))
            return;
        JSValue arr = JS_NewArray(m_ctx);
        for (size_t i = 0; i < records.size(); ++i)
            JS_SetPropertyUint32(m_ctx, arr, i, buildRecordObject(m_ctx, records[i].get()));
        JSValue args[2];
        args[0] = arr;
        args[1] = JS_DupValue(m_ctx, m_jsObject);
        JSValue r = JS_Call(m_ctx, m_callback, m_jsObject, 2, args);
        JS_FreeValue(m_ctx, r);
        JS_FreeValue(m_ctx, args[0]);
        JS_FreeValue(m_ctx, args[1]);
    }

    virtual void observerDestroyed() { delete this; }

private:
    JSContext* m_ctx;
    JSValue m_callback;
    JSValue m_jsObject;
};

// Opaque attached to each MutationObserver JS object.
struct ObserverBinding {
    RefPtr<MutationObserver> observer;
    QJSMutationCallback* callback; // owned by the observer (observerDestroyed deletes)
};

static ObserverBinding* bindingFrom(JSValueConst val)
{
    return (ObserverBinding*)JS_GetOpaque(val, gObserverClassId);
}

static void observerFinalizer(JSRuntime*, JSValue val)
{
    ObserverBinding* b = (ObserverBinding*)JS_GetOpaque(val, gObserverClassId);
    if (b) {
        // Dropping the RefPtr destroys the observer when last ref goes, which
        // unregisters it and deletes the callback via observerDestroyed().
        delete b;
    }
}

// Parses an options object into the MutationObserver option bitmask.
static unsigned parseOptions(JSContext* ctx, JSValueConst opts)
{
    unsigned flags = 0;
    if (!JS_IsObject(opts))
        return flags;
    struct { const char* name; unsigned flag; } map[] = {
        { "childList", MutationObserveChildList },
        { "attributes", MutationObserveAttributes },
        { "characterData", MutationObserveCharacterData },
        { "subtree", MutationObserveSubtree },
        { "attributeOldValue", MutationAttributeOldValue },
        { "characterDataOldValue", MutationCharacterDataOldValue },
    };
    for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); ++i) {
        JSValue v = JS_GetPropertyStr(ctx, opts, map[i].name);
        if (JS_ToBool(ctx, v))
            flags |= map[i].flag;
        JS_FreeValue(ctx, v);
    }
    // attributeOldValue/characterDataOldValue imply their base type.
    if (flags & MutationAttributeOldValue)
        flags |= MutationObserveAttributes;
    if (flags & MutationCharacterDataOldValue)
        flags |= MutationObserveCharacterData;
    return flags;
}

static JSValue observerObserve(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    ObserverBinding* b = bindingFrom(this_val);
    if (!b || argc < 1)
        return JS_ThrowTypeError(ctx, "MutationObserver.observe requires a target");
    Node* target = toNode(argv[0]);
    if (!target)
        return JS_ThrowTypeError(ctx, "MutationObserver.observe target must be a Node");
    unsigned options = (argc >= 2) ? parseOptions(ctx, argv[1]) : 0;
    if (!(options & (MutationObserveChildList | MutationObserveAttributes | MutationObserveCharacterData)))
        return JS_ThrowTypeError(ctx, "MutationObserver.observe requires childList, attributes, or characterData");
    b->observer->observe(target, options);
    return JS_UNDEFINED;
}

static JSValue observerDisconnect(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    ObserverBinding* b = bindingFrom(this_val);
    if (b)
        b->observer->disconnect();
    return JS_UNDEFINED;
}

static JSValue observerTakeRecords(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    ObserverBinding* b = bindingFrom(this_val);
    JSValue arr = JS_NewArray(ctx);
    if (b) {
        Vector<RefPtr<MutationRecord> > records;
        b->observer->takeRecords(records);
        for (size_t i = 0; i < records.size(); ++i)
            JS_SetPropertyUint32(ctx, arr, i, buildRecordObject(ctx, records[i].get()));
    }
    return arr;
}

static JSValue observerConstructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
    if (argc < 1 || !JS_IsFunction(ctx, argv[0]))
        return JS_ThrowTypeError(ctx, "MutationObserver requires a callback function");

    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, gObserverClassId);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        return obj;

    QJSMutationCallback* callback = new QJSMutationCallback(ctx, argv[0]);
    callback->setJSObject(obj);
    ObserverBinding* binding = new ObserverBinding();
    binding->callback = callback;
    binding->observer = MutationObserver::create(callback);
    JS_SetOpaque(obj, binding);
    return obj;
}

// Delivery scheduler: enqueued as a QuickJS job (microtask) so records are
// delivered after the current script finishes, matching the spec checkpoint.
static JSValue deliverJob(JSContext* ctx, int argc, JSValueConst* argv)
{
    MutationObserverRegistry::deliverAllPending();
    return JS_UNDEFINED;
}

static JSContext* gSchedulerContext = 0;

static void scheduleDeliveryHook()
{
    if (gSchedulerContext)
        JS_EnqueueJob(gSchedulerContext, deliverJob, 0, 0);
}

void registerMutationObserver(JSContext* ctx, JSValue global)
{
    if (gObserverClassId == 0) {
        JS_NewClassID(&gObserverClassId);
        JSClassDef def;
        memset(&def, 0, sizeof(def));
        def.class_name = "MutationObserver";
        def.finalizer = observerFinalizer;
        JS_NewClass(JS_GetRuntime(ctx), gObserverClassId, &def);
    }

    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, proto, "observe", JS_NewCFunction(ctx, observerObserve, "observe", 2));
    JS_SetPropertyStr(ctx, proto, "disconnect", JS_NewCFunction(ctx, observerDisconnect, "disconnect", 0));
    JS_SetPropertyStr(ctx, proto, "takeRecords", JS_NewCFunction(ctx, observerTakeRecords, "takeRecords", 0));
    JS_SetClassProto(ctx, gObserverClassId, proto);

    JSValue ctor = JS_NewCFunction2(ctx, observerConstructor, "MutationObserver", 1, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, ctor, proto);
    JS_SetPropertyStr(ctx, global, "MutationObserver", ctor);

    gSchedulerContext = ctx;
    MutationObserverRegistry::setDeliveryScheduler(scheduleDeliveryHook);
}

} // namespace WebCore

#endif // ENABLE(MUTATION_OBSERVERS)
