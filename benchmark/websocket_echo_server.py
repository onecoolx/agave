#!/usr/bin/env python3
"""Minimal RFC 6455 WebSocket echo server (no third-party dependencies).

Used by benchmark/websocket_echo_test.html to validate the engine's WebSocket
client end to end. Listens on localhost:8765 and echoes back each text message
prefixed with "echo:". Exits after 40 seconds of inactivity.
"""
import base64
import hashlib
import socket
import struct
import threading

GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


def handshake(conn):
    data = conn.recv(4096).decode("utf-8", "ignore")
    key = ""
    for line in data.split("\r\n"):
        if line.lower().startswith("sec-websocket-key:"):
            key = line.split(":", 1)[1].strip()
    accept = base64.b64encode(hashlib.sha1((key + GUID).encode()).digest()).decode()
    conn.send((
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: %s\r\n\r\n" % accept
    ).encode())


def recv_frame(conn):
    header = conn.recv(2)
    if len(header) < 2:
        return None
    opcode = header[0] & 0x0F
    masked = header[1] & 0x80
    length = header[1] & 0x7F
    if length == 126:
        length = struct.unpack(">H", conn.recv(2))[0]
    elif length == 127:
        length = struct.unpack(">Q", conn.recv(8))[0]
    mask = conn.recv(4) if masked else b""
    payload = bytearray(conn.recv(length))
    if masked:
        for i in range(len(payload)):
            payload[i] ^= mask[i % 4]
    return opcode, bytes(payload)


def send_frame(conn, data, opcode=0x1):
    out = bytearray([0x80 | opcode])
    n = len(data)
    if n < 126:
        out.append(n)
    elif n < 65536:
        out += bytes([126]) + struct.pack(">H", n)
    else:
        out += bytes([127]) + struct.pack(">Q", n)
    out += data if isinstance(data, bytes) else data.encode()
    conn.send(out)


def handle(conn):
    handshake(conn)
    while True:
        frame = recv_frame(conn)
        if frame is None:
            break
        opcode, payload = frame
        if opcode == 0x8:  # close
            break
        if opcode == 0x1:  # text
            send_frame(conn, b"echo:" + payload, 0x1)
    conn.close()


def main():
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(("localhost", 8765))
    srv.listen(5)
    srv.settimeout(40)
    try:
        while True:
            conn, _ = srv.accept()
            threading.Thread(target=handle, args=(conn,), daemon=True).start()
    except socket.timeout:
        pass


if __name__ == "__main__":
    main()
