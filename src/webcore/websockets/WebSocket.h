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

#ifndef WebSocket_h
#define WebSocket_h

#include "config.h"

#if ENABLE(WEB_SOCKETS)

#include "PlatformString.h"
#include "Shared.h"
#include "Timer.h"
#include <wtf/Vector.h>

typedef void CURL;

namespace WebCore {

typedef int ExceptionCode;

// Receives WebSocket lifecycle events. The binding layer implements this to
// dispatch onopen/onmessage/onclose/onerror to JavaScript.
class WebSocketClient {
public:
    virtual ~WebSocketClient() { }
    virtual void didOpen() = 0;
    virtual void didReceiveMessage(const String& message) = 0;
    virtual void didClose() = 0;
    virtual void didError() = 0;
};

// A WebSocket connection backed by libcurl's WebSocket support (curl_ws_*).
// Uses a dedicated easy handle in CONNECT_ONLY mode and a polling Timer to
// pump incoming frames, integrating with the engine's single-threaded loop.
class WebSocket : public Shared<WebSocket> {
public:
    enum State { CONNECTING = 0, OPEN = 1, CLOSING = 2, CLOSED = 3 };

    // Shared starts with refCount 0, so a plain assignment (which calls ref())
    // is required here. adoptRef would leave the count at 0 and the first deref
    // would free a still-referenced object.
    static PassRefPtr<WebSocket> create(WebSocketClient* client) { return RefPtr<WebSocket>(new WebSocket(client)); }
    ~WebSocket();

    void connect(const String& url, ExceptionCode&);
    void send(const String& message, ExceptionCode&);
    void close();

    State readyState() const { return m_state; }
    const String& url() const { return m_url; }

    void clearClient() { m_client = 0; }

private:
    WebSocket(WebSocketClient*);

    void connectTimerFired(Timer<WebSocket>*);
    void doConnect();
    void pollTimerFired(Timer<WebSocket>*);
    void fail();

    WebSocketClient* m_client; // not owned (the binding owns this WebSocket)
    State m_state;
    String m_url;
    CURL* m_handle;
    Vector<char> m_messageBuffer; // accumulates payload across chunked/fragmented reads
    Timer<WebSocket> m_connectTimer;
    Timer<WebSocket> m_pollTimer;
};

} // namespace WebCore

#endif // ENABLE(WEB_SOCKETS)

#endif // WebSocket_h
