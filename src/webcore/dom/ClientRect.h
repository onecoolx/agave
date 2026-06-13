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

#ifndef ClientRect_h
#define ClientRect_h

#include "IntRect.h"
#include "Shared.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {

// The result object returned by Element::getBoundingClientRect(). Holds an
// immutable snapshot of a rectangle in CSS pixels relative to the viewport.
class ClientRect : public Shared<ClientRect> {
public:
    static PassRefPtr<ClientRect> create() { return new ClientRect(); }
    static PassRefPtr<ClientRect> create(const IntRect& r) { return new ClientRect(r); }

    float top() const { return m_rect.y(); }
    float right() const { return m_rect.right(); }
    float bottom() const { return m_rect.bottom(); }
    float left() const { return m_rect.x(); }
    float width() const { return m_rect.width(); }
    float height() const { return m_rect.height(); }
    float x() const { return m_rect.x(); }
    float y() const { return m_rect.y(); }

private:
    ClientRect() { }
    explicit ClientRect(const IntRect& r) : m_rect(r) { }

    IntRect m_rect;
};

} // namespace WebCore

#endif // ClientRect_h
