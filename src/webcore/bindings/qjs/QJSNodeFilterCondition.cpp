/*
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2007 Apple Inc. All rights reserved.
 *  Copyright (C) 2024 Zhang Ji Peng All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "QJSNodeFilterCondition.h"

#include "Document.h"
#include "Frame.h"
#include "QJSNode.h"
#include "QJSNodeFilter.h"
#include "NodeFilter.h"

#include "qjs_script.h"

namespace WebCore {

JSNodeFilterCondition::JSNodeFilterCondition(JSContext* ctx, JSValue filter)
    : m_context(ctx)
    , m_filter(JS_DupValue(ctx, filter))
{
}

void JSNodeFilterCondition::mark()
{
    JS_FreeValue(m_context, m_filter);
}

short JSNodeFilterCondition::acceptNode(Node* filterNode) const
{
    Node* node = filterNode;
    Frame* frame = node->document()->frame();
    ScriptController* controller = frame->script();
    if (controller) {
        JSContext* ctx = controller->context(); 
        JSValue argv = toJS(ctx, node);

        JSValue protoType = JS_GetPrototype(ctx, m_filter);
        JSValue method = JS_GetPropertyStr(ctx, protoType, "acceptNode");

        JSValue result = JS_Call(ctx, method, m_filter, 1, &argv);
        JS_FreeValue(ctx, protoType);
        JS_FreeValue(ctx, method);
        return QJS::valueToInt32(ctx, result);
    }

    return NodeFilter::FILTER_REJECT;
}

} // namespace WebCore
