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

#ifndef JPEG_DECODER_H_
#define JPEG_DECODER_H_

#include "config.h"

#ifdef JPEG_SUPPORT

#include "ImageDecoder.h"

namespace WebCore {

class JPEGImageReader;

// This class decodes the JPEG image format.
class JPEGImageDecoder : public ImageDecoder
{
public:
    JPEGImageDecoder();
    ~JPEGImageDecoder();

    // Take the data and store it.
    virtual void setData(const Vector<char>& data, bool allDataReceived);

    // Whether or not the size information has been decoded yet.
    virtual bool isSizeAvailable();

    virtual PixelsBuffer* frameBufferAtIndex(size_t index);
    
    virtual bool supportsAlpha() const { return false; }

    void decode(bool sizeOnly = false);

    JPEGImageReader* reader() { return m_reader; }

    void setSize(int width, int height) {
        if (!m_sizeAvailable) {
            m_sizeAvailable = true;
            m_size = IntSize(width, height);
        }
    }

    bool outputScanlines();
    void jpegComplete();

private:
    mutable JPEGImageReader* m_reader;
};

}
#endif /* JPEG_SUPPORT */

#endif
