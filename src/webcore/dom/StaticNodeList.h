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

#ifndef StaticNodeList_h
#define StaticNodeList_h

#include "NodeList.h"
#include <wtf/Vector.h>

namespace WebCore {

// A static (snapshot) node list, as returned by querySelectorAll(). Unlike the
// live NodeList subclasses, its contents are fixed at construction time.
class StaticNodeList : public NodeList {
public:
    static PassRefPtr<StaticNodeList> adopt(PassRefPtr<Node> rootNode, Vector<RefPtr<Node> >& nodes)
    {
        // Shared starts at refcount 0, so construct through PassRefPtr (whose
        // raw-pointer constructor calls ref()) rather than adoptRef().
        return PassRefPtr<StaticNodeList>(new StaticNodeList(rootNode, nodes));
    }

    virtual unsigned length() const { return m_nodes.size(); }
    virtual Node* item(unsigned index) const
    {
        return index < m_nodes.size() ? m_nodes[index].get() : 0;
    }

protected:
    virtual bool nodeMatches(Node*) const { return false; } // unused for static list

private:
    StaticNodeList(PassRefPtr<Node> rootNode, Vector<RefPtr<Node> >& nodes)
        : NodeList(rootNode)
    {
        m_nodes.swap(nodes);
    }

    Vector<RefPtr<Node> > m_nodes;
};

} // namespace WebCore

#endif // StaticNodeList_h
