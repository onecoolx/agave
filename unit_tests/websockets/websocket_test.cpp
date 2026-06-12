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

#include "test.h"

#if ENABLE(WEB_SOCKETS)

#include "WebSocket.h"
#include "ExceptionCode.h"

using namespace WebCore;

namespace {

// Records callbacks for assertions. A live connection requires the timer event
// loop, so these tests focus on the state machine and reference handling that
// can be exercised deterministically without a server.
class RecordingClient : public WebSocketClient
{
public:
    RecordingClient() : opened(0), closed(0), errored(0) { }
    virtual void didOpen() { opened++; }
    virtual void didReceiveMessage(const String& m) { messages.append(m); }
    virtual void didClose() { closed++; }
    virtual void didError() { errored++; }

    int opened;
    int closed;
    int errored;
    Vector<String> messages;
};

} // namespace

TEST(WebSocketTest, InitialStateIsConnecting)
{
    RecordingClient client;
    RefPtr<WebSocket> ws = WebSocket::create(&client);
    EXPECT_EQ((int)ws->readyState(), (int)WebSocket::CONNECTING);
    EXPECT_TRUE(ws->url().isEmpty());
    ws->clearClient();
}

TEST(WebSocketTest, SendBeforeOpenThrows)
{
    RecordingClient client;
    RefPtr<WebSocket> ws = WebSocket::create(&client);
    ExceptionCode ec = 0;
    ws->send("hello", ec);
    // Sending while still CONNECTING is an invalid-state error.
    EXPECT_NE(ec, 0);
    ws->clearClient();
}

TEST(WebSocketTest, CloseBeforeConnectTransitionsToClosed)
{
    RecordingClient client;
    RefPtr<WebSocket> ws = WebSocket::create(&client);
    ws->close();
    EXPECT_EQ((int)ws->readyState(), (int)WebSocket::CLOSED);
    // didClose fires exactly once even if close() is called again.
    EXPECT_EQ(client.closed, 1);
    ws->close();
    EXPECT_EQ(client.closed, 1);
    ws->clearClient();
}

TEST(WebSocketTest, ConnectRecordsUrl)
{
    RecordingClient client;
    RefPtr<WebSocket> ws = WebSocket::create(&client);
    ExceptionCode ec = 0;
    // connect() only schedules the handshake on a timer; without pumping the
    // event loop it stays CONNECTING but the url must be retained immediately.
    ws->connect("ws://example.com/socket", ec);
    EXPECT_EQ(ec, 0);
    EXPECT_EQ(ws->url(), "ws://example.com/socket");
    EXPECT_EQ((int)ws->readyState(), (int)WebSocket::CONNECTING);
    // Tear down without firing the deferred timer.
    ws->close();
    ws->clearClient();
}

TEST(WebSocketTest, ClearClientStopsCallbacks)
{
    RecordingClient client;
    RefPtr<WebSocket> ws = WebSocket::create(&client);
    ws->clearClient();
    ws->close();
    // No callbacks after the client is detached.
    EXPECT_EQ(client.closed, 0);
}

#endif // ENABLE(WEB_SOCKETS)
