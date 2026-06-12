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

#if ENABLE(WEB_SOCKETS)

#include "WebSocket.h"

#include "CString.h"
#include "ExceptionCode.h"
#include <curl/curl.h>

namespace WebCore {

// Largest single incoming message we buffer, to bound memory from a hostile peer.
static const size_t kMaxWebSocketMessage = 4 * 1024 * 1024; // 4MB
// How often to poll for incoming frames.
static const double kPollIntervalSeconds = 0.025;

WebSocket::WebSocket(WebSocketClient* client)
    : m_client(client)
    , m_state(CONNECTING)
    , m_handle(0)
    , m_connectTimer(this, &WebSocket::connectTimerFired)
    , m_pollTimer(this, &WebSocket::pollTimerFired)
{
}

WebSocket::~WebSocket()
{
    if (m_handle) {
        curl_easy_cleanup((CURL*)m_handle);
        m_handle = 0;
    }
}

void WebSocket::connect(const String& url, ExceptionCode& ec)
{
    if (m_state != CONNECTING) {
        ec = INVALID_STATE_ERR;
        return;
    }
    m_url = url;
    // Defer the (blocking) handshake to a zero-delay timer so the constructor
    // returns immediately and the page script is not frozen during connect.
    m_connectTimer.startOneShot(0);
}

void WebSocket::connectTimerFired(Timer<WebSocket>*)
{
    doConnect();
}

void WebSocket::doConnect()
{
    if (m_state != CONNECTING)
        return;

    CURL* handle = curl_easy_init();
    if (!handle) {
        fail();
        return;
    }
    m_handle = handle;

    CString u = m_url.utf8();
    curl_easy_setopt(handle, CURLOPT_URL, u.data());
    // CONNECT_ONLY=2 selects libcurl's WebSocket mode: perform() completes the
    // HTTP upgrade handshake, then curl_ws_send/recv drive frames.
    curl_easy_setopt(handle, CURLOPT_CONNECT_ONLY, 2L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 30L);

    CURLcode res = curl_easy_perform(handle);
    if (res != CURLE_OK) {
        fail();
        return;
    }

    m_state = OPEN;
    if (m_client)
        m_client->didOpen();

    // Poll for incoming frames on the engine's main loop.
    m_pollTimer.startRepeating(kPollIntervalSeconds);
}

void WebSocket::send(const String& message, ExceptionCode& ec)
{
    if (m_state != OPEN) {
        ec = INVALID_STATE_ERR;
        return;
    }
    CString data = message.utf8();
    size_t sent = 0;
    CURLcode res = curl_ws_send((CURL*)m_handle, data.data(), data.length(),
                                &sent, 0, CURLWS_TEXT);
    if (res != CURLE_OK)
        fail();
}

void WebSocket::close()
{
    if (m_state == CLOSED || m_state == CLOSING)
        return;
    m_state = CLOSING;
    m_connectTimer.stop();
    m_pollTimer.stop();

    if (m_handle) {
        size_t sent = 0;
        curl_ws_send((CURL*)m_handle, "", 0, &sent, 0, CURLWS_CLOSE);
        curl_easy_cleanup((CURL*)m_handle);
        m_handle = 0;
    }
    m_state = CLOSED;
    if (m_client)
        m_client->didClose();
}

void WebSocket::fail()
{
    m_connectTimer.stop();
    m_pollTimer.stop();
    if (m_handle) {
        curl_easy_cleanup((CURL*)m_handle);
        m_handle = 0;
    }
    State prev = m_state;
    m_state = CLOSED;
    if (m_client) {
        m_client->didError();
        if (prev != CONNECTING)
            m_client->didClose();
    }
}

void WebSocket::pollTimerFired(Timer<WebSocket>*)
{
    if (m_state != OPEN || !m_handle)
        return;

    // Dispatching a message runs JS that may call close() (freeing m_handle) or
    // drop the last external reference to this object. Hold a self-ref for the
    // duration and re-check state after every dispatch to avoid use-after-free.
    RefPtr<WebSocket> protect(this);

    // Drain any frames that have arrived without blocking.
    for (;;) {
        if (m_state != OPEN || !m_handle)
            return;

        char buffer[8192];
        size_t got = 0;
        const struct curl_ws_frame* meta = 0;
        CURLcode res = curl_ws_recv((CURL*)m_handle, buffer, sizeof(buffer), &got, &meta);
        if (res == CURLE_AGAIN)
            return; // no more data right now
        if (res != CURLE_OK) {
            fail();
            return;
        }
        if (meta && (meta->flags & CURLWS_CLOSE)) {
            close();
            return;
        }

        // A single message may span multiple recv() calls (large payloads are
        // chunked, and CURLWS_CONT marks continuation frames). Accumulate until
        // the whole payload is present, bounding total size against a hostile
        // peer, then dispatch one message.
        if (got > 0) {
            if (m_messageBuffer.size() + got > kMaxWebSocketMessage) {
                fail();
                return;
            }
            m_messageBuffer.append(buffer, got);
        }

        bool complete = !meta || meta->bytesleft == 0;
        if (complete && !m_messageBuffer.isEmpty()) {
            String message = String::fromUTF8(m_messageBuffer.data(), m_messageBuffer.size());
            m_messageBuffer.clear();
            if (m_client)
                m_client->didReceiveMessage(message);
        }
        if (got == 0)
            return;
    }
}

} // namespace WebCore

#endif // ENABLE(WEB_SOCKETS)
