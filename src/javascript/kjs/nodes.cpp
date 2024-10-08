/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 1999-2002 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Cameron Zwarich (cwzwarich@uwaterloo.ca)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "nodes.h"

#include <math.h>
#ifdef KJS_DEBUG_MEM
#include <stdio.h>
#include <typeinfo>
#endif

#include "context.h"
#include "debugger.h"
#include "function_object.h"
#include "lexer.h"
#include "operations.h"
#include "PropertyNameArray.h"
#include <wtf/Assertions.h>
#include <wtf/HashSet.h>
#include <wtf/HashCountedSet.h>
#include <wtf/MathExtras.h>

using namespace KJS;

#define KJS_BREAKPOINT \
  if (Debugger::debuggersPresent > 0 && !hitStatement(exec)) \
    return Completion(Normal);

#define KJS_ABORTPOINT \
  if (Debugger::debuggersPresent > 0 && \
      exec->dynamicInterpreter()->imp()->debugger() && \
      exec->dynamicInterpreter()->imp()->debugger()->imp()->aborted()) \
    return Completion(Normal);

#define KJS_CHECKEXCEPTION \
  if (exec->hadException()) { \
    JSValue *ex = exec->exception(); \
    exec->clearException(); \
    handleException(exec, ex); \
    return Completion(Throw, ex); \
  } \
  if (Collector::isOutOfMemory()) \
    return Completion(Throw, Error::create(exec, GeneralError, "Out of memory"));

#define KJS_CHECKEXCEPTIONVALUE \
  if (exec->hadException()) { \
    handleException(exec); \
    return jsUndefined(); \
  } \
  if (Collector::isOutOfMemory()) \
    return jsUndefined(); // will be picked up by KJS_CHECKEXCEPTION

#define KJS_CHECKEXCEPTIONLIST \
  if (exec->hadException()) { \
    handleException(exec); \
    return List(); \
  } \
  if (Collector::isOutOfMemory()) \
    return List(); // will be picked up by KJS_CHECKEXCEPTION

// ------------------------------ Node -----------------------------------------

#ifndef NDEBUG
#ifndef LOG_CHANNEL_PREFIX
#define LOG_CHANNEL_PREFIX Log
#endif
static WTFLogChannel LogKJSNodeLeaks = { 0x00000000, "", WTFLogChannelOn };

struct NodeCounter { 
    static unsigned count; 
    ~NodeCounter() 
    { 
        if (count) 
            LOG(KJSNodeLeaks, "LEAK: %u KJS::Node\n", count); 
    }
};
unsigned NodeCounter::count = 0;
static NodeCounter nodeCounter;
#endif

static HashSet<Node*>* newNodes;
static HashCountedSet<Node*>* nodeExtraRefCounts;

Node::Node()
    : m_mayHaveDeclarations(false)
{
#ifndef NDEBUG
    ++NodeCounter::count;
#endif
  m_line = Lexer::curr()->lineNo();
  if (!newNodes)
      newNodes = new HashSet<Node*>;
  newNodes->add(this);
}

Node::~Node()
{
#ifndef NDEBUG
    --NodeCounter::count;
#endif
}

void Node::ref()
{
    // bumping from 0 to 1 is just removing from the new nodes set
    if (newNodes) {
        HashSet<Node*>::iterator it = newNodes->find(this);
        if (it != newNodes->end()) {
            newNodes->remove(it);
            ASSERT(!nodeExtraRefCounts || !nodeExtraRefCounts->contains(this));
            return;
        }
    }   

    ASSERT(!newNodes || !newNodes->contains(this));
    
    if (!nodeExtraRefCounts)
        nodeExtraRefCounts = new HashCountedSet<Node*>;
    nodeExtraRefCounts->add(this);
}

void Node::deref()
{
    ASSERT(!newNodes || !newNodes->contains(this));
    
    if (!nodeExtraRefCounts) {
        delete this;
        return;
    }

    HashCountedSet<Node*>::iterator it = nodeExtraRefCounts->find(this);
    if (it == nodeExtraRefCounts->end())
        delete this;
    else
        nodeExtraRefCounts->remove(it);
}

unsigned Node::refcount()
{
    if (newNodes && newNodes->contains(this)) {
        ASSERT(!nodeExtraRefCounts || !nodeExtraRefCounts->contains(this));
        return 0;
    }
 
    ASSERT(!newNodes || !newNodes->contains(this));

    if (!nodeExtraRefCounts)
        return 1;

    return 1 + nodeExtraRefCounts->count(this);
}

void Node::clearNewNodes()
{
    if (!newNodes)
        return;

#ifndef NDEBUG
    HashSet<Node*>::iterator end = newNodes->end();
    for (HashSet<Node*>::iterator it = newNodes->begin(); it != end; ++it)
        ASSERT(!nodeExtraRefCounts || !nodeExtraRefCounts->contains(*it));
#endif
    deleteAllValues(*newNodes);
    delete newNodes;
    newNodes = 0;
}

static void substitute(UString &string, const UString &substring) KJS_FAST_CALL;
static void substitute(UString &string, const UString &substring)
{
    int position = string.find("%s");
    ASSERT(position != -1);
    string = string.substr(0, position) + substring + string.substr(position + 2);
}

static inline int currentSourceId(ExecState* exec) KJS_FAST_CALL;
static inline int currentSourceId(ExecState* exec)
{
    return exec->context()->currentBody()->sourceId();
}

static inline const UString& currentSourceURL(ExecState* exec) KJS_FAST_CALL;
static inline const UString& currentSourceURL(ExecState* exec)
{
    return exec->context()->currentBody()->sourceURL();
}

Completion Node::createErrorCompletion(ExecState* exec, ErrorType e, const char *msg)
{
    return Completion(Throw, Error::create(exec, e, msg, lineNo(), currentSourceId(exec), currentSourceURL(exec)));
}

Completion Node::createErrorCompletion(ExecState *exec, ErrorType e, const char *msg, const Identifier &ident)
{
    UString message = msg;
    substitute(message, ident.ustring());
    return Completion(Throw, Error::create(exec, e, message, lineNo(), currentSourceId(exec), currentSourceURL(exec)));
}

JSValue *Node::throwError(ExecState* exec, ErrorType e, const char *msg)
{
    return KJS::throwError(exec, e, msg, lineNo(), currentSourceId(exec), currentSourceURL(exec));
}

JSValue *Node::throwError(ExecState* exec, ErrorType e, const char* msg, const char* string)
{
    UString message = msg;
    substitute(message, string);
    return KJS::throwError(exec, e, message, lineNo(), currentSourceId(exec), currentSourceURL(exec));
}

JSValue *Node::throwError(ExecState *exec, ErrorType e, const char *msg, JSValue *v, Node *expr)
{
    UString message = msg;
    substitute(message, v->toString(exec));
    substitute(message, expr->toString());
    return KJS::throwError(exec, e, message, lineNo(), currentSourceId(exec), currentSourceURL(exec));
}


JSValue *Node::throwError(ExecState *exec, ErrorType e, const char *msg, const Identifier &label)
{
    UString message = msg;
    substitute(message, label.ustring());
    return KJS::throwError(exec, e, message, lineNo(), currentSourceId(exec), currentSourceURL(exec));
}

JSValue *Node::throwError(ExecState *exec, ErrorType e, const char *msg, JSValue *v, Node *e1, Node *e2)
{
    UString message = msg;
    substitute(message, v->toString(exec));
    substitute(message, e1->toString());
    substitute(message, e2->toString());
    return KJS::throwError(exec, e, message, lineNo(), currentSourceId(exec), currentSourceURL(exec));
}

JSValue *Node::throwError(ExecState *exec, ErrorType e, const char *msg, JSValue *v, Node *expr, const Identifier &label)
{
    UString message = msg;
    substitute(message, v->toString(exec));
    substitute(message, expr->toString());
    substitute(message, label.ustring());
    return KJS::throwError(exec, e, message, lineNo(), currentSourceId(exec), currentSourceURL(exec));
}

JSValue *Node::throwError(ExecState *exec, ErrorType e, const char *msg, JSValue *v, const Identifier &label)
{
    UString message = msg;
    substitute(message, v->toString(exec));
    substitute(message, label.ustring());
    return KJS::throwError(exec, e, message, lineNo(), currentSourceId(exec), currentSourceURL(exec));
}

JSValue *Node::throwUndefinedVariableError(ExecState *exec, const Identifier &ident)
{
    return throwError(exec, ReferenceError, "Can't find variable: %s", ident);
}

void Node::handleException(ExecState* exec)
{
    handleException(exec, exec->exception());
}

void Node::handleException(ExecState* exec, JSValue* exceptionValue)
{
    if (exceptionValue->isObject()) {
        JSObject* exception = static_cast<JSObject*>(exceptionValue);
        if (!exception->hasProperty(exec, "line") && !exception->hasProperty(exec, "sourceURL")) {
            exception->put(exec, "line", jsNumber(m_line));
            exception->put(exec, "sourceURL", jsString(currentSourceURL(exec)));
        }
    }
    Debugger* dbg = exec->dynamicInterpreter()->debugger();
    if (dbg && !dbg->hasHandledException(exec, exceptionValue)) {
        bool cont = dbg->exception(exec, currentSourceId(exec), m_line, exceptionValue);
        if (!cont)
            dbg->imp()->abort();
    }
}

Node *Node::nodeInsideAllParens()
{
    return this;
}

// ------------------------------ StatementNode --------------------------------

StatementNode::StatementNode() 
    : m_lastLine(-1)
{
    m_line = -1;
}

void StatementNode::setLoc(int firstLine, int lastLine)
{
    m_line = firstLine;
    m_lastLine = lastLine;
}

// return true if the debugger wants us to stop at this point
bool StatementNode::hitStatement(ExecState* exec)
{
  Debugger *dbg = exec->dynamicInterpreter()->debugger();
  if (dbg)
    return dbg->atStatement(exec, currentSourceId(exec), firstLine(), lastLine());
  else
    return true; // continue
}

// ------------------------------ NullNode -------------------------------------

JSValue *NullNode::evaluate(ExecState *)
{
  return jsNull();
}

// ------------------------------ BooleanNode ----------------------------------

JSValue *BooleanNode::evaluate(ExecState *)
{
  return jsBoolean(value);
}

// ------------------------------ NumberNode -----------------------------------

JSValue *NumberNode::evaluate(ExecState *)
{
  return jsNumber(val);
}

// ------------------------------ StringNode -----------------------------------

JSValue *StringNode::evaluate(ExecState *)
{
  return jsOwnedString(value);
}

// ------------------------------ RegExpNode -----------------------------------

JSValue *RegExpNode::evaluate(ExecState *exec)
{
  List list;
  list.append(jsOwnedString(pattern));
  list.append(jsOwnedString(flags));

  JSObject *reg = exec->lexicalInterpreter()->builtinRegExp();
  return reg->construct(exec,list);
}

// ------------------------------ ThisNode -------------------------------------

// ECMA 11.1.1
JSValue *ThisNode::evaluate(ExecState *exec)
{
  return exec->context()->thisValue();
}

// ------------------------------ ResolveNode ----------------------------------

// ECMA 11.1.2 & 10.1.4
JSValue *ResolveNode::evaluate(ExecState *exec)
{
  const ScopeChain& chain = exec->context()->scopeChain();
  ScopeChainIterator iter = chain.begin();
  ScopeChainIterator end = chain.end();
  
  // we must always have something in the scope chain
  ASSERT(iter != end);

  PropertySlot slot;
  do { 
    JSObject *o = *iter;

    if (o->getPropertySlot(exec, ident, slot))
      return slot.getValue(exec, o, ident);
    
    ++iter;
  } while (iter != end);

  return throwUndefinedVariableError(exec, ident);
}

// ------------------------------ GroupNode ------------------------------------

// ECMA 11.1.6
JSValue *GroupNode::evaluate(ExecState *exec)
{
  return group->evaluate(exec);
}

Node *GroupNode::nodeInsideAllParens()
{
    Node *n = this;
    do
        n = static_cast<GroupNode *>(n)->group.get();
    while (n->isGroupNode());
    return n;
}

// ------------------------------ ElementNode ----------------------------------

// ECMA 11.1.4
JSValue *ElementNode::evaluate(ExecState *exec)
{
  JSObject *array = exec->lexicalInterpreter()->builtinArray()->construct(exec, List::empty());
  int length = 0;
  for (ElementNode *n = this; n; n = n->next.get()) {
    JSValue *val = n->node->evaluate(exec);
    KJS_CHECKEXCEPTIONVALUE
    length += n->elision;
    array->put(exec, length++, val);
  }
  return array;
}

void ElementNode::breakCycle() 
{ 
    next = 0;
}

// ------------------------------ ArrayNode ------------------------------------

// ECMA 11.1.4
JSValue *ArrayNode::evaluate(ExecState *exec)
{
  JSObject *array;
  int length;

  if (element) {
    array = static_cast<JSObject*>(element->evaluate(exec));
    KJS_CHECKEXCEPTIONVALUE
    length = opt ? array->get(exec, exec->propertyNames().length)->toInt32(exec) : 0;
  } else {
    JSValue *newArr = exec->lexicalInterpreter()->builtinArray()->construct(exec,List::empty());
    array = static_cast<JSObject*>(newArr);
    length = 0;
  }

  if (opt)
    array->put(exec, exec->propertyNames().length, jsNumber(elision + length), DontEnum | DontDelete);

  return array;
}

// ------------------------------ ObjectLiteralNode ----------------------------

// ECMA 11.1.5
JSValue *ObjectLiteralNode::evaluate(ExecState *exec)
{
  if (list)
    return list->evaluate(exec);

  return exec->lexicalInterpreter()->builtinObject()->construct(exec,List::empty());
}

// ------------------------------ PropertyListNode -----------------------------

// ECMA 11.1.5
JSValue *PropertyListNode::evaluate(ExecState *exec)
{
  JSObject *obj = exec->lexicalInterpreter()->builtinObject()->construct(exec, List::empty());
  
  for (PropertyListNode *p = this; p; p = p->next.get()) {
    JSValue *n = p->node->name->evaluate(exec);
    KJS_CHECKEXCEPTIONVALUE
    JSValue *v = p->node->assign->evaluate(exec);
    KJS_CHECKEXCEPTIONVALUE
    
    Identifier propertyName = Identifier(n->toString(exec));
    switch (p->node->type) {
      case PropertyNode::Getter:
        ASSERT(v->isObject());
        obj->defineGetter(exec, propertyName, static_cast<JSObject *>(v));
        break;
      case PropertyNode::Setter:
        ASSERT(v->isObject());
        obj->defineSetter(exec, propertyName, static_cast<JSObject *>(v));
        break;
      case PropertyNode::Constant:
        obj->put(exec, propertyName, v);
        break;
    }
  }

  return obj;
}

void PropertyListNode::breakCycle() 
{ 
    next = 0;
}

// ------------------------------ PropertyNode -----------------------------
// ECMA 11.1.5
JSValue *PropertyNode::evaluate(ExecState*)
{
  ASSERT(false);
  return jsNull();
}

// ---------------------------- PropertyNameNode -------------------------------

// ECMA 11.1.5
JSValue *PropertyNameNode::evaluate(ExecState*)
{
  JSValue *s;

  if (str.isNull()) {
    s = jsString(UString::from(numeric));
  } else {
    s = jsOwnedString(str.ustring());
  }

  return s;
}

// ------------------------------ BracketAccessorNode --------------------------------

// ECMA 11.2.1a
JSValue *BracketAccessorNode::evaluate(ExecState *exec)
{
  JSValue *v1 = expr1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *v2 = expr2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSObject *o = v1->toObject(exec);
  uint32_t i;
  if (v2->getUInt32(i))
    return o->get(exec, i);
  return o->get(exec, Identifier(v2->toString(exec)));
}

// ------------------------------ DotAccessorNode --------------------------------

// ECMA 11.2.1b
JSValue *DotAccessorNode::evaluate(ExecState *exec)
{
  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  return v->toObject(exec)->get(exec, ident);

}

// ------------------------------ ArgumentListNode -----------------------------

JSValue *ArgumentListNode::evaluate(ExecState *)
{
  ASSERT(0);
  return 0; // dummy, see evaluateList()
}

// ECMA 11.2.4
List ArgumentListNode::evaluateList(ExecState *exec)
{
  List l;

  for (ArgumentListNode *n = this; n; n = n->next.get()) {
    JSValue *v = n->expr->evaluate(exec);
    KJS_CHECKEXCEPTIONLIST
    l.append(v);
  }

  return l;
}

void ArgumentListNode::breakCycle() 
{ 
    next = 0;
}

// ------------------------------ ArgumentsNode --------------------------------

JSValue *ArgumentsNode::evaluate(ExecState *)
{
  ASSERT(0);
  return 0; // dummy, see evaluateList()
}

// ------------------------------ NewExprNode ----------------------------------

// ECMA 11.2.2

JSValue *NewExprNode::evaluate(ExecState *exec)
{
  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  List argList;
  if (args) {
    argList = args->evaluateList(exec);
    KJS_CHECKEXCEPTIONVALUE
  }

  if (!v->isObject()) {
    return throwError(exec, TypeError, "Value %s (result of expression %s) is not an object. Cannot be used with new.", v, expr.get());
  }

  JSObject *constr = static_cast<JSObject*>(v);
  if (!constr->implementsConstruct()) {
    return throwError(exec, TypeError, "Value %s (result of expression %s) is not a constructor. Cannot be used with new.", v, expr.get());
  }

  return constr->construct(exec, argList);
}

// ECMA 11.2.3
JSValue *FunctionCallValueNode::evaluate(ExecState *exec)
{
  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  if (!v->isObject()) {
    return throwError(exec, TypeError, "Value %s (result of expression %s) is not object.", v, expr.get());
  }
  
  JSObject *func = static_cast<JSObject*>(v);

  if (!func->implementsCall()) {
    return throwError(exec, TypeError, "Object %s (result of expression %s) does not allow calls.", v, expr.get());
  }

  List argList = args->evaluateList(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSObject *thisObj =  exec->dynamicInterpreter()->globalObject();

  return func->call(exec, thisObj, argList);
}

// ECMA 11.2.3
JSValue *FunctionCallResolveNode::evaluate(ExecState *exec)
{
  const ScopeChain& chain = exec->context()->scopeChain();
  ScopeChainIterator iter = chain.begin();
  ScopeChainIterator end = chain.end();
  
  // we must always have something in the scope chain
  ASSERT(iter != end);

  PropertySlot slot;
  JSObject *base;
  do { 
    base = *iter;
    if (base->getPropertySlot(exec, ident, slot)) {
      JSValue *v = slot.getValue(exec, base, ident);
      KJS_CHECKEXCEPTIONVALUE
        
      if (!v->isObject()) {
        return throwError(exec, TypeError, "Value %s (result of expression %s) is not object.", v, ident);
      }
      
      JSObject *func = static_cast<JSObject*>(v);
      
      if (!func->implementsCall()) {
        return throwError(exec, TypeError, "Object %s (result of expression %s) does not allow calls.", v, ident);
      }
      
      List argList = args->evaluateList(exec);
      KJS_CHECKEXCEPTIONVALUE
        
      JSObject *thisObj = base;
      // ECMA 11.2.3 says that in this situation the this value should be null.
      // However, section 10.2.3 says that in the case where the value provided
      // by the caller is null, the global object should be used. It also says
      // that the section does not apply to interal functions, but for simplicity
      // of implementation we use the global object anyway here. This guarantees
      // that in host objects you always get a valid object for this.
      if (thisObj->isActivation())
        thisObj = exec->dynamicInterpreter()->globalObject();

      return func->call(exec, thisObj, argList);
    }
    ++iter;
  } while (iter != end);
  
  return throwUndefinedVariableError(exec, ident);
}

// ECMA 11.2.3
JSValue *FunctionCallBracketNode::evaluate(ExecState *exec)
{
  JSValue *baseVal = base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSValue *subscriptVal = subscript->evaluate(exec);

  JSObject *baseObj = baseVal->toObject(exec);
  uint32_t i;
  PropertySlot slot;

  JSValue *funcVal;
  if (subscriptVal->getUInt32(i)) {
    if (baseObj->getPropertySlot(exec, i, slot))
      funcVal = slot.getValue(exec, baseObj, i);
    else
      funcVal = jsUndefined();
  } else {
    Identifier ident(subscriptVal->toString(exec));
    if (baseObj->getPropertySlot(exec, ident, slot))
      funcVal = baseObj->get(exec, ident);
    else
      funcVal = jsUndefined();
  }

  KJS_CHECKEXCEPTIONVALUE
  
  if (!funcVal->isObject()) {
    return throwError(exec, TypeError, "Value %s (result of expression %s[%s]) is not object.", funcVal, base.get(), subscript.get());
  }
  
  JSObject *func = static_cast<JSObject*>(funcVal);

  if (!func->implementsCall()) {
    return throwError(exec, TypeError, "Object %s (result of expression %s[%s]) does not allow calls.", funcVal, base.get(), subscript.get());
  }

  List argList = args->evaluateList(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSObject *thisObj = baseObj;
  ASSERT(thisObj);
  ASSERT(thisObj->isObject());
  ASSERT(!thisObj->isActivation());

  return func->call(exec, thisObj, argList);
}

static const char *dotExprNotAnObjectString() KJS_FAST_CALL;
static const char *dotExprNotAnObjectString()
{
  return "Value %s (result of expression %s.%s) is not object.";
}

static const char *dotExprDoesNotAllowCallsString() KJS_FAST_CALL;
static const char *dotExprDoesNotAllowCallsString()
{
  return "Object %s (result of expression %s.%s) does not allow calls.";
}

// ECMA 11.2.3
JSValue *FunctionCallDotNode::evaluate(ExecState *exec)
{
  JSValue *baseVal = base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSObject *baseObj = baseVal->toObject(exec);
  PropertySlot slot;
  JSValue *funcVal = baseObj->getPropertySlot(exec, ident, slot) ? slot.getValue(exec, baseObj, ident) : jsUndefined();
  KJS_CHECKEXCEPTIONVALUE

  if (!funcVal->isObject())
    return throwError(exec, TypeError, dotExprNotAnObjectString(), funcVal, base.get(), ident);
  
  JSObject *func = static_cast<JSObject*>(funcVal);

  if (!func->implementsCall())
    return throwError(exec, TypeError, dotExprDoesNotAllowCallsString(), funcVal, base.get(), ident);

  List argList = args->evaluateList(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSObject *thisObj = baseObj;
  ASSERT(thisObj);
  ASSERT(thisObj->isObject());
  ASSERT(!thisObj->isActivation());

  return func->call(exec, thisObj, argList);
}

// ECMA 11.3

// ------------------------------ PostfixResolveNode ----------------------------------

JSValue *PostfixResolveNode::evaluate(ExecState *exec)
{
  const ScopeChain& chain = exec->context()->scopeChain();
  ScopeChainIterator iter = chain.begin();
  ScopeChainIterator end = chain.end();
  
  // we must always have something in the scope chain
  ASSERT(iter != end);

  PropertySlot slot;
  JSObject *base;
  do { 
    base = *iter;
    if (base->getPropertySlot(exec, m_ident, slot)) {
        JSValue *v = slot.getValue(exec, base, m_ident);

        double n = v->toNumber(exec);
        
        double newValue = (m_oper == OpPlusPlus) ? n + 1 : n - 1;
        base->put(exec, m_ident, jsNumber(newValue));
        
        return jsNumber(n);
    }

    ++iter;
  } while (iter != end);

  return throwUndefinedVariableError(exec, m_ident);
}

// ------------------------------ PostfixBracketNode ----------------------------------

JSValue *PostfixBracketNode::evaluate(ExecState *exec)
{
  JSValue *baseValue = m_base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *subscript = m_subscript->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSObject *base = baseValue->toObject(exec);

  uint32_t propertyIndex;
  if (subscript->getUInt32(propertyIndex)) {
    PropertySlot slot;
    JSValue *v = base->getPropertySlot(exec, propertyIndex, slot) ? slot.getValue(exec, base, propertyIndex) : jsUndefined();
    KJS_CHECKEXCEPTIONVALUE

    double n = v->toNumber(exec);

    double newValue = (m_oper == OpPlusPlus) ? n + 1 : n - 1;
    base->put(exec, propertyIndex, jsNumber(newValue));
        
    return jsNumber(n);
  }

  Identifier propertyName(subscript->toString(exec));
  PropertySlot slot;
  JSValue *v = base->getPropertySlot(exec, propertyName, slot) ? slot.getValue(exec, base, propertyName) : jsUndefined();
  KJS_CHECKEXCEPTIONVALUE

  double n = v->toNumber(exec);
  
  double newValue = (m_oper == OpPlusPlus) ? n + 1 : n - 1;
  base->put(exec, propertyName, jsNumber(newValue));
        
  return jsNumber(n);
}

// ------------------------------ PostfixDotNode ----------------------------------

JSValue *PostfixDotNode::evaluate(ExecState *exec)
{
  JSValue *baseValue = m_base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSObject *base = baseValue->toObject(exec);

  PropertySlot slot;
  JSValue *v = base->getPropertySlot(exec, m_ident, slot) ? slot.getValue(exec, base, m_ident) : jsUndefined();
  KJS_CHECKEXCEPTIONVALUE

  double n = v->toNumber(exec);
  
  double newValue = (m_oper == OpPlusPlus) ? n + 1 : n - 1;
  base->put(exec, m_ident, jsNumber(newValue));
        
  return jsNumber(n);
}

// ------------------------------ PostfixErrorNode -----------------------------------

JSValue* PostfixErrorNode::evaluate(ExecState* exec)
{
    throwError(exec, ReferenceError, "Postfix %s operator applied to value that is not a reference.",
        m_oper == OpPlusPlus ? "++" : "--");
    handleException(exec);
    return jsUndefined();
}

// ------------------------------ DeleteResolveNode -----------------------------------

// ECMA 11.4.1

JSValue *DeleteResolveNode::evaluate(ExecState *exec)
{
  const ScopeChain& chain = exec->context()->scopeChain();
  ScopeChainIterator iter = chain.begin();
  ScopeChainIterator end = chain.end();
  
  // we must always have something in the scope chain
  ASSERT(iter != end);

  PropertySlot slot;
  JSObject *base;
  do { 
    base = *iter;
    if (base->getPropertySlot(exec, m_ident, slot)) {
        return jsBoolean(base->deleteProperty(exec, m_ident));
    }

    ++iter;
  } while (iter != end);

  return jsBoolean(true);
}

// ------------------------------ DeleteBracketNode -----------------------------------

JSValue *DeleteBracketNode::evaluate(ExecState *exec)
{
  JSValue *baseValue = m_base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *subscript = m_subscript->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSObject *base = baseValue->toObject(exec);

  uint32_t propertyIndex;
  if (subscript->getUInt32(propertyIndex))
      return jsBoolean(base->deleteProperty(exec, propertyIndex));

  Identifier propertyName(subscript->toString(exec));
  return jsBoolean(base->deleteProperty(exec, propertyName));
}

// ------------------------------ DeleteDotNode -----------------------------------
JSValue *DeleteDotNode::evaluate(ExecState *exec)
{
  JSValue *baseValue = m_base->evaluate(exec);
  JSObject *base = baseValue->toObject(exec);
  KJS_CHECKEXCEPTIONVALUE

  return jsBoolean(base->deleteProperty(exec, m_ident));
}

// ------------------------------ DeleteValueNode -----------------------------------
JSValue *DeleteValueNode::evaluate(ExecState *exec)
{
  m_expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  // delete on a non-location expression ignores the value and returns true
  return jsBoolean(true);
}

// ------------------------------ VoidNode -------------------------------------

// ECMA 11.4.2
JSValue *VoidNode::evaluate(ExecState *exec)
{
  expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return jsUndefined();
}

// ECMA 11.4.3

// ------------------------------ TypeOfValueNode -----------------------------------

static JSValue *typeStringForValue(JSValue *v) KJS_FAST_CALL;
static JSValue *typeStringForValue(JSValue *v)
{
    switch (v->type()) {
    case UndefinedType:
        return jsString("undefined");
    case NullType:
        return jsString("object");
    case BooleanType:
        return jsString("boolean");
    case NumberType:
        return jsString("number");
    case StringType:
        return jsString("string");
    default:
        if (v->isObject()) {
            // Return "undefined" for objects that should be treated
            // as null when doing comparisons.
            if (static_cast<JSObject*>(v)->masqueradeAsUndefined())
                return jsString("undefined");            
            else if (static_cast<JSObject*>(v)->implementsCall())
                return jsString("function");
        }
        
        return jsString("object");
    }
}

JSValue *TypeOfResolveNode::evaluate(ExecState *exec)
{
  const ScopeChain& chain = exec->context()->scopeChain();
  ScopeChainIterator iter = chain.begin();
  ScopeChainIterator end = chain.end();
  
  // we must always have something in the scope chain
  ASSERT(iter != end);

  PropertySlot slot;
  JSObject *base;
  do { 
    base = *iter;
    if (base->getPropertySlot(exec, m_ident, slot)) {
        JSValue *v = slot.getValue(exec, base, m_ident);
        return typeStringForValue(v);
    }

    ++iter;
  } while (iter != end);

  return jsString("undefined");
}

// ------------------------------ TypeOfValueNode -----------------------------------

JSValue *TypeOfValueNode::evaluate(ExecState *exec)
{
  JSValue *v = m_expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return typeStringForValue(v);
}

// ECMA 11.4.4 and 11.4.5

// ------------------------------ PrefixResolveNode ----------------------------------

JSValue *PrefixResolveNode::evaluate(ExecState *exec)
{
  const ScopeChain& chain = exec->context()->scopeChain();
  ScopeChainIterator iter = chain.begin();
  ScopeChainIterator end = chain.end();
  
  // we must always have something in the scope chain
  ASSERT(iter != end);

  PropertySlot slot;
  JSObject *base;
  do { 
    base = *iter;
    if (base->getPropertySlot(exec, m_ident, slot)) {
        JSValue *v = slot.getValue(exec, base, m_ident);

        double n = v->toNumber(exec);
        
        double newValue = (m_oper == OpPlusPlus) ? n + 1 : n - 1;
        JSValue *n2 = jsNumber(newValue);
        base->put(exec, m_ident, n2);

        return n2;
    }

    ++iter;
  } while (iter != end);

  return throwUndefinedVariableError(exec, m_ident);
}

// ------------------------------ PrefixBracketNode ----------------------------------

JSValue *PrefixBracketNode::evaluate(ExecState *exec)
{
  JSValue *baseValue = m_base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *subscript = m_subscript->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSObject *base = baseValue->toObject(exec);

  uint32_t propertyIndex;
  if (subscript->getUInt32(propertyIndex)) {
    PropertySlot slot;
    JSValue *v = base->getPropertySlot(exec, propertyIndex, slot) ? slot.getValue(exec, base, propertyIndex) : jsUndefined();
    KJS_CHECKEXCEPTIONVALUE

    double n = v->toNumber(exec);

    double newValue = (m_oper == OpPlusPlus) ? n + 1 : n - 1;
    JSValue *n2 = jsNumber(newValue);
    base->put(exec, propertyIndex, n2);

    return n2;
  }

  Identifier propertyName(subscript->toString(exec));
  PropertySlot slot;
  JSValue *v = base->getPropertySlot(exec, propertyName, slot) ? slot.getValue(exec, base, propertyName) : jsUndefined();
  KJS_CHECKEXCEPTIONVALUE

  double n = v->toNumber(exec);
  
  double newValue = (m_oper == OpPlusPlus) ? n + 1 : n - 1;
  JSValue *n2 = jsNumber(newValue);
  base->put(exec, propertyName, n2);

  return n2;
}

// ------------------------------ PrefixDotNode ----------------------------------

JSValue *PrefixDotNode::evaluate(ExecState *exec)
{
  JSValue *baseValue = m_base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSObject *base = baseValue->toObject(exec);

  PropertySlot slot;
  JSValue *v = base->getPropertySlot(exec, m_ident, slot) ? slot.getValue(exec, base, m_ident) : jsUndefined();
  KJS_CHECKEXCEPTIONVALUE

  double n = v->toNumber(exec);
  
  double newValue = (m_oper == OpPlusPlus) ? n + 1 : n - 1;
  JSValue *n2 = jsNumber(newValue);
  base->put(exec, m_ident, n2);

  return n2;
}

// ------------------------------ PrefixErrorNode -----------------------------------

JSValue* PrefixErrorNode::evaluate(ExecState* exec)
{
    throwError(exec, ReferenceError, "Prefix %s operator applied to value that is not a reference.",
        m_oper == OpPlusPlus ? "++" : "--");
    handleException(exec);
    return jsUndefined();
}

// ------------------------------ UnaryPlusNode --------------------------------

// ECMA 11.4.6
JSValue *UnaryPlusNode::evaluate(ExecState *exec)
{
  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return jsNumber(v->toNumber(exec));
}

// ------------------------------ NegateNode -----------------------------------

// ECMA 11.4.7
JSValue *NegateNode::evaluate(ExecState *exec)
{
  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  double n = v->toNumber(exec);
  return jsNumber(-n);
}

// ------------------------------ BitwiseNotNode -------------------------------

// ECMA 11.4.8
JSValue *BitwiseNotNode::evaluate(ExecState *exec)
{
  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  return jsNumber(~v->toInt32(exec));
}

// ------------------------------ LogicalNotNode -------------------------------

// ECMA 11.4.9
JSValue *LogicalNotNode::evaluate(ExecState *exec)
{
  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  return jsBoolean(!v->toBoolean(exec));
}

// ------------------------------ MultNode -------------------------------------

// ECMA 11.5
JSValue *MultNode::evaluate(ExecState *exec)
{
  JSValue *v1 = term1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSValue *v2 = term2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return mult(exec, v1, v2, oper);
}

// ------------------------------ AddNode --------------------------------------

// ECMA 11.6
JSValue *AddNode::evaluate(ExecState *exec)
{
  JSValue *v1 = term1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSValue *v2 = term2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return add(exec, v1, v2, oper);
}

// ------------------------------ ShiftNode ------------------------------------

// ECMA 11.7
JSValue *ShiftNode::evaluate(ExecState *exec)
{
  JSValue *v1 = term1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *v2 = term2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  unsigned int i2 = v2->toUInt32(exec);
  i2 &= 0x1f;

  switch (oper) {
  case OpLShift:
    return jsNumber(v1->toInt32(exec) << i2);
  case OpRShift:
    return jsNumber(v1->toInt32(exec) >> i2);
  case OpURShift:
    return jsNumber(v1->toUInt32(exec) >> i2);
  default:
    ASSERT(!"ShiftNode: unhandled switch case");
    return jsUndefined();
  }
}

// ------------------------------ RelationalNode -------------------------------

// ECMA 11.8
JSValue *RelationalNode::evaluate(ExecState *exec)
{
  JSValue *v1 = expr1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *v2 = expr2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  bool b;
  if (oper == OpLess || oper == OpGreaterEq) {
    int r = relation(exec, v1, v2);
    if (r < 0)
      b = false;
    else
      b = (oper == OpLess) ? (r == 1) : (r == 0);
  } else if (oper == OpGreater || oper == OpLessEq) {
    int r = relation(exec, v2, v1);
    if (r < 0)
      b = false;
    else
      b = (oper == OpGreater) ? (r == 1) : (r == 0);
  } else if (oper == OpIn) {
      // Is all of this OK for host objects?
      if (!v2->isObject())
          return throwError(exec,  TypeError,
                             "Value %s (result of expression %s) is not an object. Cannot be used with IN expression.", v2, expr2.get());
      JSObject *o2(static_cast<JSObject*>(v2));
      b = o2->hasProperty(exec, Identifier(v1->toString(exec)));
  } else {
    if (!v2->isObject())
        return throwError(exec,  TypeError,
                           "Value %s (result of expression %s) is not an object. Cannot be used with instanceof operator.", v2, expr2.get());

    JSObject *o2(static_cast<JSObject*>(v2));
    if (!o2->implementsHasInstance())
      // According to the spec, only some types of objects "implement" the [[HasInstance]] property.
      // But we are supposed to throw an exception where the object does not "have" the [[HasInstance]]
      // property. It seems that all object have the property, but not all implement it, so in this
      // case we return false (consistent with mozilla)
      return jsBoolean(false);
    return jsBoolean(o2->hasInstance(exec, v1));
  }

  return jsBoolean(b);
}

// ------------------------------ EqualNode ------------------------------------

// ECMA 11.9
JSValue *EqualNode::evaluate(ExecState *exec)
{
  JSValue *v1 = expr1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *v2 = expr2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  bool result;
  if (oper == OpEqEq || oper == OpNotEq) {
    // == and !=
    bool eq = equal(exec,v1, v2);
    result = oper == OpEqEq ? eq : !eq;
  } else {
    // === and !==
    bool eq = strictEqual(exec,v1, v2);
    result = oper == OpStrEq ? eq : !eq;
  }
  return jsBoolean(result);
}

// ------------------------------ BitOperNode ----------------------------------

// ECMA 11.10
JSValue *BitOperNode::evaluate(ExecState *exec)
{
  JSValue *v1 = expr1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *v2 = expr2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  int i1 = v1->toInt32(exec);
  int i2 = v2->toInt32(exec);
  int result;
  if (oper == OpBitAnd)
    result = i1 & i2;
  else if (oper == OpBitXOr)
    result = i1 ^ i2;
  else
    result = i1 | i2;

  return jsNumber(result);
}

// ------------------------------ BinaryLogicalNode ----------------------------

// ECMA 11.11
JSValue *BinaryLogicalNode::evaluate(ExecState *exec)
{
  JSValue *v1 = expr1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  bool b1 = v1->toBoolean(exec);
  if ((!b1 && oper == OpAnd) || (b1 && oper == OpOr))
    return v1;

  JSValue *v2 = expr2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return v2;
}

// ------------------------------ ConditionalNode ------------------------------

// ECMA 11.12
JSValue *ConditionalNode::evaluate(ExecState *exec)
{
  JSValue *v = logical->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  bool b = v->toBoolean(exec);

  if (b)
    v = expr1->evaluate(exec);
  else
    v = expr2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return v;
}

// ECMA 11.13

static ALWAYS_INLINE JSValue *valueForReadModifyAssignment(ExecState * exec, JSValue *v1, JSValue *v2, Operator oper) KJS_FAST_CALL;
static ALWAYS_INLINE JSValue *valueForReadModifyAssignment(ExecState * exec, JSValue *v1, JSValue *v2, Operator oper)
{
  JSValue *v;
  int i1;
  int i2;
  unsigned int ui;
  switch (oper) {
  case OpMultEq:
    v = mult(exec, v1, v2, '*');
    break;
  case OpDivEq:
    v = mult(exec, v1, v2, '/');
    break;
  case OpPlusEq:
    v = add(exec, v1, v2, '+');
    break;
  case OpMinusEq:
    v = add(exec, v1, v2, '-');
    break;
  case OpLShift:
    i1 = v1->toInt32(exec);
    i2 = v2->toInt32(exec);
    v = jsNumber(i1 << i2);
    break;
  case OpRShift:
    i1 = v1->toInt32(exec);
    i2 = v2->toInt32(exec);
    v = jsNumber(i1 >> i2);
    break;
  case OpURShift:
    ui = v1->toUInt32(exec);
    i2 = v2->toInt32(exec);
    v = jsNumber(ui >> i2);
    break;
  case OpAndEq:
    i1 = v1->toInt32(exec);
    i2 = v2->toInt32(exec);
    v = jsNumber(i1 & i2);
    break;
  case OpXOrEq:
    i1 = v1->toInt32(exec);
    i2 = v2->toInt32(exec);
    v = jsNumber(i1 ^ i2);
    break;
  case OpOrEq:
    i1 = v1->toInt32(exec);
    i2 = v2->toInt32(exec);
    v = jsNumber(i1 | i2);
    break;
  case OpModEq: {
    double d1 = v1->toNumber(exec);
    double d2 = v2->toNumber(exec);
    v = jsNumber(fmod(d1, d2));
  }
    break;
  default:
    ASSERT(0);
    v = jsUndefined();
  }
  
  return v;
}

// ------------------------------ AssignResolveNode -----------------------------------

JSValue *AssignResolveNode::evaluate(ExecState *exec)
{
  const ScopeChain& chain = exec->context()->scopeChain();
  ScopeChainIterator iter = chain.begin();
  ScopeChainIterator end = chain.end();
  
  // we must always have something in the scope chain
  ASSERT(iter != end);

  PropertySlot slot;
  JSObject *base;
  do { 
    base = *iter;
    if (base->getPropertySlot(exec, m_ident, slot))
      goto found;

    ++iter;
  } while (iter != end);

  if (m_oper != OpEqual)
    return throwUndefinedVariableError(exec, m_ident);

 found:
  JSValue *v;

  if (m_oper == OpEqual) {
    v = m_right->evaluate(exec);
  } else {
    JSValue *v1 = slot.getValue(exec, base, m_ident);
    KJS_CHECKEXCEPTIONVALUE
    JSValue *v2 = m_right->evaluate(exec);
    v = valueForReadModifyAssignment(exec, v1, v2, m_oper);
  }

  KJS_CHECKEXCEPTIONVALUE

  base->put(exec, m_ident, v);
  return v;
}

// ------------------------------ AssignDotNode -----------------------------------

JSValue *AssignDotNode::evaluate(ExecState *exec)
{
  JSValue *baseValue = m_base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSObject *base = baseValue->toObject(exec);

  JSValue *v;

  if (m_oper == OpEqual) {
    v = m_right->evaluate(exec);
  } else {
    PropertySlot slot;
    JSValue *v1 = base->getPropertySlot(exec, m_ident, slot) ? slot.getValue(exec, base, m_ident) : jsUndefined();
    KJS_CHECKEXCEPTIONVALUE
    JSValue *v2 = m_right->evaluate(exec);
    v = valueForReadModifyAssignment(exec, v1, v2, m_oper);
  }

  KJS_CHECKEXCEPTIONVALUE

  base->put(exec, m_ident, v);
  return v;
}

// ------------------------------ AssignErrorNode -----------------------------------

JSValue* AssignErrorNode::evaluate(ExecState* exec)
{
    throwError(exec, ReferenceError, "Left side of assignment is not a reference.");
    handleException(exec);
    return jsUndefined();
}

// ------------------------------ AssignBracketNode -----------------------------------

JSValue *AssignBracketNode::evaluate(ExecState *exec)
{
  JSValue *baseValue = m_base->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *subscript = m_subscript->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  JSObject *base = baseValue->toObject(exec);

  uint32_t propertyIndex;
  if (subscript->getUInt32(propertyIndex)) {
    JSValue *v;
    if (m_oper == OpEqual) {
      v = m_right->evaluate(exec);
    } else {
      PropertySlot slot;
      JSValue *v1 = base->getPropertySlot(exec, propertyIndex, slot) ? slot.getValue(exec, base, propertyIndex) : jsUndefined();
      KJS_CHECKEXCEPTIONVALUE
      JSValue *v2 = m_right->evaluate(exec);
      v = valueForReadModifyAssignment(exec, v1, v2, m_oper);
    }

    KJS_CHECKEXCEPTIONVALUE

    base->put(exec, propertyIndex, v);
    return v;
  }

  Identifier propertyName(subscript->toString(exec));
  JSValue *v;

  if (m_oper == OpEqual) {
    v = m_right->evaluate(exec);
  } else {
    PropertySlot slot;
    JSValue *v1 = base->getPropertySlot(exec, propertyName, slot) ? slot.getValue(exec, base, propertyName) : jsUndefined();
    KJS_CHECKEXCEPTIONVALUE
    JSValue *v2 = m_right->evaluate(exec);
    v = valueForReadModifyAssignment(exec, v1, v2, m_oper);
  }

  KJS_CHECKEXCEPTIONVALUE

  base->put(exec, propertyName, v);
  return v;
}

// ------------------------------ CommaNode ------------------------------------

// ECMA 11.14
JSValue *CommaNode::evaluate(ExecState *exec)
{
  expr1->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE
  JSValue *v = expr2->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return v;
}

// ------------------------------ AssignExprNode -------------------------------

// ECMA 12.2
JSValue *AssignExprNode::evaluate(ExecState *exec)
{
  return expr->evaluate(exec);
}

// ------------------------------ VarDeclNode ----------------------------------

    
VarDeclNode::VarDeclNode(const Identifier &id, AssignExprNode *in, Type t)
    : varType(t), ident(id), init(in)
{
    m_mayHaveDeclarations = true; 
}

void VarDeclNode::getDeclarations(DeclarationStacks& stacks)
{ 
    stacks.varStack.append(this); 
}

JSValue* VarDeclNode::handleSlowCase(ExecState* exec, const ScopeChain& chain, JSValue* val)
{
    ScopeChainIterator iter = chain.begin();
    ScopeChainIterator end = chain.end();        
    
    // we must always have something in the scope chain
    ASSERT(iter != end);
    
    PropertySlot slot;
    JSObject* base;
    
    do {
        base = *iter;
        if (base->getPropertySlot(exec, ident, slot))
            break;
        
        ++iter;
    } while (iter != end);
    
    unsigned flags = 0;
    base->getPropertyAttributes(ident, flags);
    if (varType == VarDeclNode::Constant)
        flags |= ReadOnly;
    
    base->put(exec, ident, val, flags);
    return 0;
}

// ECMA 12.2
JSValue* VarDeclNode::evaluate(ExecState* exec)
{
    const ScopeChain& chain = exec->context()->scopeChain();
    JSObject* variableObject = exec->context()->variableObject();

    ASSERT(!chain.isEmpty());

    bool inGlobalScope = ++chain.begin() == chain.end();

    if (inGlobalScope && (init || !variableObject->getDirect(ident))) {
        JSValue* val = init ? init->evaluate(exec) : jsUndefined();
        int flags = Internal;
        if (exec->context()->codeType() != EvalCode)
            flags |= DontDelete;
        if (varType == VarDeclNode::Constant)
            flags |= ReadOnly;
        variableObject->putDirect(ident, val, flags);
    } else if (init) {
        JSValue* val = init->evaluate(exec);
        KJS_CHECKEXCEPTIONVALUE
            
        // if the variable object is the top of the scope chain, then that must
        // be where this variable is declared, processVarDecls would have put 
        // it there. Don't search the scope chain, to optimize this very common case.
        if (chain.top() != variableObject)
            return handleSlowCase(exec, chain, val);

        unsigned flags = 0;
        variableObject->getPropertyAttributes(ident, flags);
        if (varType == VarDeclNode::Constant)
            flags |= ReadOnly;
        
        ASSERT(variableObject->getDirect(ident) || ident == exec->propertyNames().arguments);
        variableObject->put(exec, ident, val, flags);
    }

    // no caller of this function actually uses the return value. 
    // FIXME: It would be better to change the inheritence hierarchy so this
    // node doesn't even have an evaluate method, but instead a differently named
    // one with a void return.
    return 0;
}

void VarDeclNode::processDeclaration(ExecState* exec)
{
  JSObject* variable = exec->context()->variableObject();

  // If a variable by this name already exists, don't clobber it -
  // it might be a function parameter
  if (!variable->hasProperty(exec, ident)) {
    int flags = Internal;
    if (exec->context()->codeType() != EvalCode)
      flags |= DontDelete;
    if (varType == VarDeclNode::Constant)
      flags |= ReadOnly;
    variable->put(exec, ident, jsUndefined(), flags);
  }
}

// ------------------------------ VarDeclListNode ------------------------------

// ECMA 12.2
JSValue *VarDeclListNode::evaluate(ExecState *exec)
{
  for (VarDeclListNode *n = this; n; n = n->next.get()) {
    n->var->evaluate(exec);
    KJS_CHECKEXCEPTIONVALUE
  }
  return jsUndefined();
}

void VarDeclListNode::getDeclarations(DeclarationStacks& stacks) 
{
    if (next) {
        ASSERT(next->mayHaveDeclarations());
        stacks.nodeStack.append(next.get()); 
    }
    stacks.varStack.append(var.get()); 
}

void VarDeclListNode::breakCycle() 
{ 
    next = 0;
}

// ------------------------------ VarStatementNode -----------------------------

// ECMA 12.2
Completion VarStatementNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  (void) next->evaluate(exec);
  KJS_CHECKEXCEPTION

  return Completion(Normal);
}

void VarStatementNode::getDeclarations(DeclarationStacks& stacks)
{
    ASSERT(next->mayHaveDeclarations());
    stacks.nodeStack.append(next.get());
}

// ------------------------------ BlockNode ------------------------------------

BlockNode::BlockNode(SourceElementsNode *s)
{
  if (s) {
    m_mayHaveDeclarations = true; 
    source = s->next.release();
    Parser::removeNodeCycle(source.get());
    setLoc(s->firstLine(), s->lastLine());
  } else {
    source = 0;
  }
}

void BlockNode::getDeclarations(DeclarationStacks& stacks)
{ 
    ASSERT(source && source->mayHaveDeclarations());
    stacks.nodeStack.append(source.get()); 
}

// ECMA 12.1
Completion BlockNode::execute(ExecState *exec)
{
  if (!source)
    return Completion(Normal);

  return source->execute(exec);
}

// ------------------------------ EmptyStatementNode ---------------------------

// ECMA 12.3
Completion EmptyStatementNode::execute(ExecState *)
{
  return Completion(Normal);
}

// ------------------------------ ExprStatementNode ----------------------------

// ECMA 12.4
Completion ExprStatementNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTION

  return Completion(Normal, v);
}

// ------------------------------ IfNode ---------------------------------------

// ECMA 12.5
Completion IfNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTION
  bool b = v->toBoolean(exec);

  // if ... then
  if (b)
    return statement1->execute(exec);

  // no else
  if (!statement2)
    return Completion(Normal);

  // else
  return statement2->execute(exec);
}

void IfNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (statement2 && statement2->mayHaveDeclarations()) 
        stacks.nodeStack.append(statement2.get()); 
    if (statement1->mayHaveDeclarations()) 
        stacks.nodeStack.append(statement1.get()); 
}

// ------------------------------ DoWhileNode ----------------------------------

// ECMA 12.6.1
Completion DoWhileNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  JSValue *bv;
  Completion c;
  JSValue* value = 0;

  do {
    // bail out on error
    KJS_CHECKEXCEPTION

    exec->context()->pushIteration();
    c = statement->execute(exec);
    exec->context()->popIteration();
    
    if (exec->dynamicInterpreter()->timedOut())
        return Completion(Interrupted);

    if (c.isValueCompletion())
        value = c.value();

    if (!((c.complType() == Continue) && ls.contains(c.target()))) {
      if ((c.complType() == Break) && ls.contains(c.target()))
        return Completion(Normal, value);
      if (c.complType() != Normal)
        return c;
    }
    bv = expr->evaluate(exec);
    KJS_CHECKEXCEPTION
  } while (bv->toBoolean(exec));

  return Completion(Normal, value);
}

void DoWhileNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (statement->mayHaveDeclarations()) 
        stacks.nodeStack.append(statement.get()); 
}

// ------------------------------ WhileNode ------------------------------------

// ECMA 12.6.2
Completion WhileNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  JSValue *bv;
  Completion c;
  bool b(false);
  JSValue *value = 0;

  while (1) {
    bv = expr->evaluate(exec);
    KJS_CHECKEXCEPTION
    b = bv->toBoolean(exec);

    // bail out on error
    KJS_CHECKEXCEPTION

    if (!b)
      return Completion(Normal, value);

    exec->context()->pushIteration();
    c = statement->execute(exec);
    exec->context()->popIteration();

    if (exec->dynamicInterpreter()->timedOut())
        return Completion(Interrupted);
    
    if (c.isValueCompletion())
      value = c.value();

    if ((c.complType() == Continue) && ls.contains(c.target()))
      continue;
    if ((c.complType() == Break) && ls.contains(c.target()))
      return Completion(Normal, value);
    if (c.complType() != Normal)
      return c;
  }

  return Completion(); // work around gcc 4.0 bug
}

void WhileNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (statement->mayHaveDeclarations()) 
        stacks.nodeStack.append(statement.get()); 
}

// ------------------------------ ForNode --------------------------------------

// ECMA 12.6.3
Completion ForNode::execute(ExecState *exec)
{
  JSValue *v, *cval = 0;

  if (expr1) {
    v = expr1->evaluate(exec);
    KJS_CHECKEXCEPTION
  }
  while (1) {
    if (expr2) {
      v = expr2->evaluate(exec);
      KJS_CHECKEXCEPTION
      if (!v->toBoolean(exec))
        return Completion(Normal, cval);
    }
    // bail out on error
    KJS_CHECKEXCEPTION

    exec->context()->pushIteration();
    Completion c = statement->execute(exec);
    exec->context()->popIteration();
    if (c.isValueCompletion())
      cval = c.value();
    if (!((c.complType() == Continue) && ls.contains(c.target()))) {
      if ((c.complType() == Break) && ls.contains(c.target()))
        return Completion(Normal, cval);
      if (c.complType() != Normal)
      return c;
    }
    
    if (exec->dynamicInterpreter()->timedOut())
        return Completion(Interrupted);
    
    if (expr3) {
      v = expr3->evaluate(exec);
      KJS_CHECKEXCEPTION
    }
  }
  
  return Completion(); // work around gcc 4.0 bug
}

void ForNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (statement->mayHaveDeclarations()) 
        stacks.nodeStack.append(statement.get()); 
    if (expr1 && expr1->mayHaveDeclarations()) 
        stacks.nodeStack.append(expr1.get()); 
}

// ------------------------------ ForInNode ------------------------------------

ForInNode::ForInNode(Node *l, Node *e, StatementNode *s)
  : init(0L), lexpr(l), expr(e), varDecl(0L), statement(s)
{
    m_mayHaveDeclarations = true; 
}

ForInNode::ForInNode(const Identifier &i, AssignExprNode *in, Node *e, StatementNode *s)
  : ident(i), init(in), expr(e), statement(s)
{
  m_mayHaveDeclarations = true; 

  // for( var foo = bar in baz )
  varDecl = new VarDeclNode(ident, init.get(), VarDeclNode::Variable);
  lexpr = new ResolveNode(ident);
}

void ForInNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (statement->mayHaveDeclarations()) 
        stacks.nodeStack.append(statement.get()); 
    if (varDecl && varDecl->mayHaveDeclarations()) 
        stacks.nodeStack.append(varDecl.get()); 
}

// ECMA 12.6.4
Completion ForInNode::execute(ExecState *exec)
{
  JSValue *e;
  JSValue *retval = 0;
  JSObject *v;
  Completion c;
  PropertyNameArray propertyNames;

  if (varDecl) {
    varDecl->evaluate(exec);
    KJS_CHECKEXCEPTION
  }

  e = expr->evaluate(exec);

  // for Null and Undefined, we want to make sure not to go through
  // the loop at all, because their object wrappers will have a
  // property list but will throw an exception if you attempt to
  // access any property.
  if (e->isUndefinedOrNull()) {
    return Completion(Normal, 0);
  }

  KJS_CHECKEXCEPTION
  v = e->toObject(exec);
  v->getPropertyNames(exec, propertyNames);
  
  PropertyNameArrayIterator end = propertyNames.end();
  for (PropertyNameArrayIterator it = propertyNames.begin(); it != end; ++it) {
      const Identifier &name = *it;
      if (!v->hasProperty(exec, name))
          continue;

      JSValue *str = jsOwnedString(name.ustring());

      if (lexpr->isResolveNode()) {
        const Identifier &ident = static_cast<ResolveNode *>(lexpr.get())->identifier();

        const ScopeChain& chain = exec->context()->scopeChain();
        ScopeChainIterator iter = chain.begin();
        ScopeChainIterator end = chain.end();
  
        // we must always have something in the scope chain
        ASSERT(iter != end);

        PropertySlot slot;
        JSObject *o;
        do { 
            o = *iter;
            if (o->getPropertySlot(exec, ident, slot)) {
                o->put(exec, ident, str);
                break;
            }
            ++iter;
        } while (iter != end);
        
        if (iter == end)
            o->put(exec, ident, str);
    } else if (lexpr->isDotAccessorNode()) {
        const Identifier& ident = static_cast<DotAccessorNode *>(lexpr.get())->identifier();
        JSValue *v = static_cast<DotAccessorNode *>(lexpr.get())->base()->evaluate(exec);
        KJS_CHECKEXCEPTION
        JSObject *o = v->toObject(exec);

        o->put(exec, ident, str);
    } else {
        ASSERT(lexpr->isBracketAccessorNode());
        JSValue *v = static_cast<BracketAccessorNode *>(lexpr.get())->base()->evaluate(exec);
        KJS_CHECKEXCEPTION
        JSValue *v2 = static_cast<BracketAccessorNode *>(lexpr.get())->subscript()->evaluate(exec);
        KJS_CHECKEXCEPTION
        JSObject *o = v->toObject(exec);

        uint32_t i;
        if (v2->getUInt32(i))
            o->put(exec, i, str);
        o->put(exec, Identifier(v2->toString(exec)), str);
    }

    KJS_CHECKEXCEPTION

    exec->context()->pushIteration();
    c = statement->execute(exec);
    exec->context()->popIteration();
    if (c.isValueCompletion())
      retval = c.value();

    if (!((c.complType() == Continue) && ls.contains(c.target()))) {
      if ((c.complType() == Break) && ls.contains(c.target()))
        break;
      if (c.complType() != Normal) {
        return c;
      }
    }
  }

  // bail out on error
  KJS_CHECKEXCEPTION

  return Completion(Normal, retval);
}

// ------------------------------ ContinueNode ---------------------------------

// ECMA 12.7
Completion ContinueNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  if (ident.isEmpty() && !exec->context()->inIteration())
    return createErrorCompletion(exec, SyntaxError, "Invalid continue statement.");
  else if (!ident.isEmpty() && !exec->context()->seenLabels()->contains(ident))
    return createErrorCompletion(exec, SyntaxError, "Label %s not found.", ident);
  else
    return Completion(Continue, 0, ident);
}

// ------------------------------ BreakNode ------------------------------------

// ECMA 12.8
Completion BreakNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  if (ident.isEmpty() && !exec->context()->inIteration() &&
      !exec->context()->inSwitch())
    return createErrorCompletion(exec, SyntaxError, "Invalid break statement.");
  else if (!ident.isEmpty() && !exec->context()->seenLabels()->contains(ident))
    return createErrorCompletion(exec, SyntaxError, "Label %s not found.");
  else
    return Completion(Break, 0, ident);
}

// ------------------------------ ReturnNode -----------------------------------

// ECMA 12.9
Completion ReturnNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  CodeType codeType = exec->context()->codeType();
  if (codeType != FunctionCode)
    return createErrorCompletion(exec, SyntaxError, "Invalid return statement.");

  if (!value)
    return Completion(ReturnValue, jsUndefined());

  JSValue *v = value->evaluate(exec);
  KJS_CHECKEXCEPTION

  return Completion(ReturnValue, v);
}

// ------------------------------ WithNode -------------------------------------

void WithNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (statement->mayHaveDeclarations()) 
        stacks.nodeStack.append(statement.get()); 
}

// ECMA 12.10
Completion WithNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTION
  JSObject *o = v->toObject(exec);
  KJS_CHECKEXCEPTION
  exec->context()->pushScope(o);
  Completion res = statement->execute(exec);
  exec->context()->popScope();

  return res;
}

// ------------------------------ CaseClauseNode -------------------------------

void CaseClauseNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (source && source->mayHaveDeclarations()) 
        stacks.nodeStack.append(source.get()); 
}

// ECMA 12.11
JSValue *CaseClauseNode::evaluate(ExecState *exec)
{
  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTIONVALUE

  return v;
}

// ECMA 12.11
Completion CaseClauseNode::evalStatements(ExecState *exec)
{
  if (source)
    return source->execute(exec);
  else
    return Completion(Normal, jsUndefined());
}

// ------------------------------ ClauseListNode -------------------------------

void ClauseListNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (next && next->mayHaveDeclarations()) 
        stacks.nodeStack.append(next.get()); 
    if (clause->mayHaveDeclarations()) 
        stacks.nodeStack.append(clause.get()); 
}

JSValue *ClauseListNode::evaluate(ExecState *)
{
  // should never be called
  ASSERT(false);
  return 0;
}

void ClauseListNode::breakCycle() 
{ 
    next = 0;
}

// ------------------------------ CaseBlockNode --------------------------------

CaseBlockNode::CaseBlockNode(ClauseListNode *l1, CaseClauseNode *d,
                             ClauseListNode *l2)
{
  m_mayHaveDeclarations = true; 
  if (l1) {
    list1 = l1->next.release();
    Parser::removeNodeCycle(list1.get());
  } else {
    list1 = 0;
  }

  def = d;

  if (l2) {
    list2 = l2->next.release();
    Parser::removeNodeCycle(list2.get());
  } else {
    list2 = 0;
  }
}
 
void CaseBlockNode::getDeclarations(DeclarationStacks& stacks) 
{ 
    if (list2 && list2->mayHaveDeclarations()) 
        stacks.nodeStack.append(list2.get());
    if (def && def->mayHaveDeclarations()) 
        stacks.nodeStack.append(def.get()); 
    if (list1 && list1->mayHaveDeclarations()) 
        stacks.nodeStack.append(list1.get()); 
}

JSValue *CaseBlockNode::evaluate(ExecState *)
{
  // should never be called
  ASSERT(false);
  return 0;
}

// ECMA 12.11
Completion CaseBlockNode::evalBlock(ExecState *exec, JSValue *input)
{
  JSValue *v;
  Completion res;
  ClauseListNode *a = list1.get();
  ClauseListNode *b = list2.get();
  CaseClauseNode *clause;

    while (a) {
      clause = a->getClause();
      a = a->getNext();
      v = clause->evaluate(exec);
      KJS_CHECKEXCEPTION
      if (strictEqual(exec, input, v)) {
        res = clause->evalStatements(exec);
        if (res.complType() != Normal)
          return res;
        while (a) {
          res = a->getClause()->evalStatements(exec);
          if (res.complType() != Normal)
            return res;
          a = a->getNext();
        }
        break;
      }
    }

  while (b) {
    clause = b->getClause();
    b = b->getNext();
    v = clause->evaluate(exec);
    KJS_CHECKEXCEPTION
    if (strictEqual(exec, input, v)) {
      res = clause->evalStatements(exec);
      if (res.complType() != Normal)
        return res;
      goto step18;
    }
  }

  // default clause
  if (def) {
    res = def->evalStatements(exec);
    if (res.complType() != Normal)
      return res;
  }
  b = list2.get();
 step18:
  while (b) {
    clause = b->getClause();
    res = clause->evalStatements(exec);
    if (res.complType() != Normal)
      return res;
    b = b->getNext();
  }

  // bail out on error
  KJS_CHECKEXCEPTION

  return Completion(Normal);
}

// ------------------------------ SwitchNode -----------------------------------

void SwitchNode::getDeclarations(DeclarationStacks& stacks) 
{ 
    if (block->mayHaveDeclarations()) 
        stacks.nodeStack.append(block.get()); 
}

// ECMA 12.11
Completion SwitchNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTION

  exec->context()->pushSwitch();
  Completion res = block->evalBlock(exec,v);
  exec->context()->popSwitch();

  if ((res.complType() == Break) && ls.contains(res.target()))
    return Completion(Normal, res.value());
  return res;
}

// ------------------------------ LabelNode ------------------------------------

void LabelNode::getDeclarations(DeclarationStacks& stacks) 
{ 
    if (statement->mayHaveDeclarations()) 
        stacks.nodeStack.append(statement.get()); 
}

// ECMA 12.12
Completion LabelNode::execute(ExecState *exec)
{
  if (!exec->context()->seenLabels()->push(label))
    return createErrorCompletion(exec, SyntaxError, "Duplicated label %s found.", label);
  Completion e = statement->execute(exec);
  exec->context()->seenLabels()->pop();

  if ((e.complType() == Break) && (e.target() == label))
    return Completion(Normal, e.value());
  return e;
}

// ------------------------------ ThrowNode ------------------------------------

// ECMA 12.13
Completion ThrowNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  JSValue *v = expr->evaluate(exec);
  KJS_CHECKEXCEPTION

  handleException(exec, v);
  return Completion(Throw, v);
}

// ------------------------------ TryNode --------------------------------------

void TryNode::getDeclarations(DeclarationStacks& stacks) 
{ 
    if (finallyBlock && finallyBlock->mayHaveDeclarations()) 
        stacks.nodeStack.append(finallyBlock.get()); 
    if (catchBlock && catchBlock->mayHaveDeclarations()) 
        stacks.nodeStack.append(catchBlock.get()); 
    if (tryBlock->mayHaveDeclarations()) 
        stacks.nodeStack.append(tryBlock.get()); 
}

// ECMA 12.14
Completion TryNode::execute(ExecState *exec)
{
  KJS_BREAKPOINT;

  Completion c = tryBlock->execute(exec);

  if (catchBlock && c.complType() == Throw) {
    JSObject *obj = new JSObject;
    obj->put(exec, exceptionIdent, c.value(), DontDelete);
    exec->context()->pushScope(obj);
    c = catchBlock->execute(exec);
    exec->context()->popScope();
  }

  if (finallyBlock) {
    Completion c2 = finallyBlock->execute(exec);
    if (c2.complType() != Normal)
      c = c2;
  }

  return c;
}

// ------------------------------ ParameterNode --------------------------------

// ECMA 13
JSValue *ParameterNode::evaluate(ExecState *)
{
  return jsUndefined();
}

void ParameterNode::breakCycle() 
{ 
    next = 0;
}

// ------------------------------ FunctionBodyNode -----------------------------

FunctionBodyNode::FunctionBodyNode(SourceElementsNode *s)
    : BlockNode(s)
    , m_sourceURL(Lexer::curr()->sourceURL())
    , m_sourceId(Parser::sid)
{

  setLoc(-1, -1);
}

void FunctionBodyNode::processDeclarations(ExecState* exec)
{
    Node* node = source.get();
    if (!node)
        return;

    DeclarationStacks stacks;
    DeclarationStacks::NodeStack& nodeStack = stacks.nodeStack;
    
    while (true) {
        ASSERT(node->mayHaveDeclarations()); // Otherwise, we wasted time putting an irrelevant node on the stack.
        node->getDeclarations(stacks);
        
        size_t size = nodeStack.size();
        if (!size)
            break;
        --size;
        node = nodeStack[size];
        nodeStack.shrink(size);
    }
    
    size_t i, size;

    DeclarationStacks::FunctionStack& functionStack = stacks.functionStack;
    for (i = 0, size = functionStack.size(); i < size; ++i)
        functionStack[i]->processDeclaration(exec);

    DeclarationStacks::VarStack& varStack = stacks.varStack;
    for (i = 0, size = varStack.size(); i < size; ++i)
        varStack[i]->processDeclaration(exec);
}

void FunctionBodyNode::addParam(const Identifier& ident)
{
  m_parameters.append(ident);
}

UString FunctionBodyNode::paramString() const
{
  UString s("");
  size_t count = numParams();
  for (size_t pos = 0; pos < count; ++pos) {
    if (!s.isEmpty())
        s += ", ";
    s += paramName(pos).ustring();
  }

  return s;
}


// ------------------------------ FuncDeclNode ---------------------------------

void FuncDeclNode::addParams() 
{
  for (ParameterNode *p = param.get(); p != 0L; p = p->nextParam())
    body->addParam(p->ident());
}

void FuncDeclNode::getDeclarations(DeclarationStacks& stacks) 
{
    stacks.functionStack.append(this);
}

void FuncDeclNode::processDeclaration(ExecState* exec)
{
  Context *context = exec->context();

  // TODO: let this be an object with [[Class]] property "Function"
  FunctionImp *func = new DeclaredFunctionImp(exec, ident, body.get(), context->scopeChain());

  JSObject *proto = exec->lexicalInterpreter()->builtinObject()->construct(exec, List::empty());
  proto->put(exec, exec->propertyNames().constructor, func, ReadOnly | DontDelete | DontEnum);
  func->put(exec, exec->propertyNames().prototype, proto, Internal|DontDelete);

  func->put(exec, exec->propertyNames().length, jsNumber(body->numParams()), ReadOnly|DontDelete|DontEnum);

  // ECMA 10.2.2
  context->variableObject()->put(exec, ident, func, Internal | (context->codeType() == EvalCode ? 0 : DontDelete));
}

Completion FuncDeclNode::execute(ExecState *)
{
    return Completion(Normal);
}

// ------------------------------ FuncExprNode ---------------------------------

// ECMA 13
void FuncExprNode::addParams()
{
  for (ParameterNode *p = param.get(); p != 0L; p = p->nextParam())
    body->addParam(p->ident());
}

JSValue *FuncExprNode::evaluate(ExecState *exec)
{
  Context *context = exec->context();
  bool named = !ident.isNull();
  JSObject *functionScopeObject = 0;

  if (named) {
    // named FunctionExpressions can recursively call themselves,
    // but they won't register with the current scope chain and should
    // be contained as single property in an anonymous object.
    functionScopeObject = new JSObject;
    context->pushScope(functionScopeObject);
  }

  FunctionImp* func = new DeclaredFunctionImp(exec, ident, body.get(), context->scopeChain());
  JSObject* proto = exec->lexicalInterpreter()->builtinObject()->construct(exec, List::empty());
  proto->put(exec, exec->propertyNames().constructor, func, ReadOnly | DontDelete | DontEnum);
  func->put(exec, exec->propertyNames().prototype, proto, Internal | DontDelete);

  if (named) {
    functionScopeObject->put(exec, ident, func, Internal | ReadOnly | (context->codeType() == EvalCode ? 0 : DontDelete));
    context->popScope();
  }

  return func;
}

// ------------------------------ SourceElementsNode ---------------------------

int SourceElementsNode::count = 0;

SourceElementsNode::SourceElementsNode(StatementNode *s1)
  : node(s1), next(this)
{
    m_mayHaveDeclarations = true; 
    Parser::noteNodeCycle(this);
    setLoc(s1->firstLine(), s1->lastLine());
}

SourceElementsNode::SourceElementsNode(SourceElementsNode *s1, StatementNode *s2)
  : node(s2), next(s1->next)
{
  m_mayHaveDeclarations = true; 
  s1->next = this;
  setLoc(s1->firstLine(), s2->lastLine());
}

void SourceElementsNode::getDeclarations(DeclarationStacks& stacks)
{ 
    if (next && next->mayHaveDeclarations())
        stacks.nodeStack.append(next.get());
    if (node->mayHaveDeclarations())
        stacks.nodeStack.append(node.get());
}

// ECMA 14
Completion SourceElementsNode::execute(ExecState *exec)
{
  KJS_CHECKEXCEPTION
  JSValue* v = 0;
  SourceElementsNode* n = this;
  while (1) {
    Completion c = n->node->execute(exec);

    if (JSValue* v2 = c.value())
      v = v2;
    c.setValue(v);

    if (c.complType() != Normal)
        return c;

    n = n->next.get();
    if (!n)
        return c;
  }
}

void SourceElementsNode::breakCycle() 
{ 
    next = 0;
}

ProgramNode::ProgramNode(SourceElementsNode *s) : FunctionBodyNode(s)
{
}
