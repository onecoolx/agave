/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "SharedBuffer.h"
#include "Debug.h"

namespace WebCore {

SharedBuffer::SharedBuffer()
{
}

SharedBuffer::SharedBuffer(const char* data, int size)
{
    m_buffer.append(data, size);
}

SharedBuffer::SharedBuffer(const unsigned char* data, int size)
{
    m_buffer.append(data, size);
}

unsigned SharedBuffer::size() const
{
    if (hasPlatformData())
        return platformDataSize();
    
    return m_buffer.size();
}

const char* SharedBuffer::data() const
{
    if (hasPlatformData())
        return platformData();
    
    return m_buffer.data();
}

void SharedBuffer::append(const char* data, int len)
{
    maybeTransferPlatformData();
    
    m_buffer.append(data, len);
}

void SharedBuffer::clear()
{
    clearPlatformData();
    
    m_buffer.resize(0);
}

PassRefPtr<SharedBuffer> SharedBuffer::copy() const
{
    return new SharedBuffer(data(), size());
}
inline void SharedBuffer::clearPlatformData()
{
}

inline void SharedBuffer::maybeTransferPlatformData()
{
}

inline bool SharedBuffer::hasPlatformData() const
{
    return false;
}

inline const char* SharedBuffer::platformData() const
{
    return 0;
}

inline unsigned SharedBuffer::platformDataSize() const
{
    return 0;
}


PassRefPtr<SharedBuffer> SharedBuffer::createWithContentsOfFile(const String& filePath)
{
    notImplemented();
    return 0;
}


}
