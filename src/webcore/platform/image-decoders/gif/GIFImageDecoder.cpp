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

#include "GIFImageDecoder.h"
#include "Debug.h"

#ifdef GIF_SUPPORT

#include "GIFImageReader.h"

namespace WebCore {

class GIFImageDecoderPrivate
{
public:
    GIFImageDecoderPrivate(GIFImageDecoder* decoder = 0)
        : m_reader(decoder)
    {
        m_readOffset = 0;
    }

    ~GIFImageDecoderPrivate()
    {
        m_reader.close();
    }

    bool decode(const Vector<char>& data, 
                GIFImageDecoder::GIFQuery query = GIFImageDecoder::GIFFullQuery,
                unsigned int haltFrame = -1)
    {
        return m_reader.read((const unsigned char*)data.data() + m_readOffset, data.size() - m_readOffset, 
                             query,
                             haltFrame);
    }

    unsigned frameCount() const { return m_reader.images_count; }
    int repetitionCount() const { return m_reader.loop_count; }

    void setReadOffset(unsigned o) { m_readOffset = o; }

    bool isTransparent() const { return m_reader.frame_reader->is_transparent; }

    void getColorMap(unsigned char*& map, unsigned& size) const {
        if (m_reader.frame_reader->is_local_colormap_defined) {
            map = m_reader.frame_reader->local_colormap;
            size = (unsigned)m_reader.frame_reader->local_colormap_size;
        } else {
            map = m_reader.global_colormap;
            size = m_reader.global_colormap_size;
        }
    }

    unsigned frameXOffset() const { return m_reader.frame_reader->x_offset; }
    unsigned frameYOffset() const { return m_reader.frame_reader->y_offset; }
    unsigned frameWidth() const { return m_reader.frame_reader->width; }
    unsigned frameHeight() const { return m_reader.frame_reader->height; }

    int transparentPixel() const { return m_reader.frame_reader->tpixel; }

    unsigned duration() const { return m_reader.frame_reader->delay_time; }

private:
    GIFImageReader m_reader;
    unsigned m_readOffset;
};

GIFImageDecoder::GIFImageDecoder()
: m_frameCountValid(true), m_reader(0)
{}

GIFImageDecoder::~GIFImageDecoder()
{
    delete m_reader;
}

// Take the data and store it.
void GIFImageDecoder::setData(const Vector<char>& data, bool allDataReceived)
{
    if (m_failed)
        return;

    // Cache our new data.
    ImageDecoder::setData(data, allDataReceived);

    // Our frame count is now unknown.
    m_frameCountValid = false;

    // Create the GIF reader.
    if (!m_reader && !m_failed)
        m_reader = new GIFImageDecoderPrivate(this);
}

// Whether or not the size information has been decoded yet.
bool GIFImageDecoder::isSizeAvailable()
{
    // If we have pending data to decode, send it to the GIF reader now.
    if (!m_sizeAvailable && m_reader) {
        if (m_failed)
            return false;

        // The decoder will go ahead and aggressively consume everything up until the first
        // size is encountered.
        decode(GIFSizeQuery, 0);
    }

    return m_sizeAvailable;
}

// The total number of frames for the image.  Will scan the image data for the answer
// (without necessarily decoding all of the individual frames).
int GIFImageDecoder::frameCount()
{
    // If the decoder had an earlier error, we will just return what we had decoded
    // so far.
    if (!m_frameCountValid) {
        // FIXME: Scanning all the data has O(n^2) behavior if the data were to come in really
        // slowly.  Might be interesting to try to clone our existing read session to preserve
        // state, but for now we just crawl all the data.  Note that this is no worse than what
        // ImageIO does on Mac right now (it also crawls all the data again).
        GIFImageDecoderPrivate reader;
        reader.decode(m_data, GIFFrameCountQuery);
        m_frameCountValid = true;
        m_frameBufferCache.resize(reader.frameCount());
    }

    return m_frameBufferCache.size();
}

// The number of repetitions to perform for an animation loop.
int GIFImageDecoder::repetitionCount() const
{
    // We don't have to do any decoding to determine this, since the loop count was determined after
    // the initial query for size.
    if (m_reader)
        return m_reader->repetitionCount();
    return cAnimationNone;
}

PixelsBuffer* GIFImageDecoder::frameBufferAtIndex(size_t index)
{
    if (index >= (size_t)frameCount())
        return 0;

    PixelsBuffer& frame = m_frameBufferCache[index];
    if (frame.status() != PixelsBuffer::FrameComplete && m_reader)
        // Decode this frame.
        decode(GIFFullQuery, index+1);

    return &frame;
}

// Feed data to the GIF reader.
void GIFImageDecoder::decode(GIFQuery query, unsigned haltAtFrame)
{
    if (m_failed)
        return;

    m_failed = !m_reader->decode(m_data, query, haltAtFrame);
    
    if (m_failed) {
        delete m_reader;
        m_reader = 0;
        m_data.clear();
    }
}

// Callbacks from the GIF reader.
void GIFImageDecoder::sizeNowAvailable(unsigned width, unsigned height)
{
    m_size = IntSize(width, height);
    m_sizeAvailable = true;
}

void GIFImageDecoder::decodingHalted(unsigned bytesLeft)
{
    m_reader->setReadOffset(m_data.size() - bytesLeft);
}

void GIFImageDecoder::initFrameBuffer(unsigned frameIndex)
{
    // Initialize the frame rect in our buffer.
    IntRect frameRect(m_reader->frameXOffset(), m_reader->frameYOffset(),
                      m_reader->frameWidth(), m_reader->frameHeight());

    // Make sure the frameRect doesn't extend past the bottom-right of the buffer.
    if (frameRect.right() > m_size.width())
        frameRect.setWidth(m_size.width() - m_reader->frameXOffset());
    if (frameRect.bottom() > m_size.height())
        frameRect.setHeight(m_size.height() - m_reader->frameYOffset());

    PixelsBuffer* const buffer = &m_frameBufferCache[frameIndex];
    buffer->setRect(frameRect);
    
    if (frameIndex == 0) {
        // This is the first frame, so we're not relying on any previous data.
        prepEmptyFrameBuffer(buffer);
    } else {
        // The starting state for this frame depends on the previous frame's
        // disposal method.
        //
        // Frames that use the DisposeOverwritePrevious method are effectively
        // no-ops in terms of changing the starting state of a frame compared to
        // the starting state of the previous frame, so skip over them.  (If the
        // first frame specifies this method, it will get treated like
        // DisposeOverwriteBgcolor below and reset to a completely empty image.)
        const PixelsBuffer* prevBuffer = &m_frameBufferCache[--frameIndex];
        PixelsBuffer::FrameDisposalMethod prevMethod =
            prevBuffer->disposalMethod();
        while ((frameIndex > 0) &&
                (prevMethod == PixelsBuffer::DisposeOverwritePrevious)) {
            prevBuffer = &m_frameBufferCache[--frameIndex];
            prevMethod = prevBuffer->disposalMethod();
        }

        if ((prevMethod == PixelsBuffer::DisposeNotSpecified) ||
                (prevMethod == PixelsBuffer::DisposeKeep)) {
            // Preserve the last frame as the starting state for this frame.
            buffer->bytes() = prevBuffer->bytes();
            buffer->setHasAlpha(prevBuffer->hasAlpha());
        } else {
            // We want to clear the previous frame to transparent, without
            // affecting pixels in the image outside of the frame.
            const IntRect& prevRect = prevBuffer->rect();
            if ((frameIndex == 0) ||
                    prevRect.contains(IntRect(IntPoint(0, 0), m_size))) {
                // Clearing the first frame, or a frame the size of the whole
                // image, results in a completely empty image.
                prepEmptyFrameBuffer(buffer);
            } else {
              // Copy the whole previous buffer, then clear just its frame.
              buffer->bytes() = prevBuffer->bytes();
              buffer->setHasAlpha(prevBuffer->hasAlpha());
              for (int y = prevRect.y(); y < prevRect.bottom(); ++y) {
                  unsigned rgba;
                  unsigned currentRow = y*m_size.width();
                  for (int x = prevRect.x(); x < prevRect.right(); ++x){
                      buffer->setRGBA(rgba, 0, 0, 0, 0);
                      buffer->bytes().setPixel(currentRow+x, rgba);
                  }
              }
              if ((prevRect.width() > 0) && (prevRect.height() > 0))
                buffer->setHasAlpha(true);
            }
        }
    }

    // Update our status to be partially complete.
    buffer->setStatus(PixelsBuffer::FramePartial);

    // Reset the alpha pixel tracker for this frame.
    m_currentBufferSawAlpha = false;
}

void GIFImageDecoder::prepEmptyFrameBuffer(PixelsBuffer* buffer) const
{
    buffer->bytes().resize(m_size.width() * m_size.height());
    buffer->bytes().init(0x0);
    buffer->setHasAlpha(true);
}

void GIFImageDecoder::haveDecodedRow(unsigned frameIndex,
                                     unsigned char* rowBuffer,   // Pointer to single scanline temporary buffer
                                     unsigned char* rowEnd,
                                     unsigned rowNumber,  // The row index
                                     unsigned repeatCount,  // How many times to repeat the row
                                     bool writeTransparentPixels)
{
    // Initialize the frame if necessary.
    PixelsBuffer& buffer = m_frameBufferCache[frameIndex];
    if (buffer.status() == PixelsBuffer::FrameEmpty)
        initFrameBuffer(frameIndex);

    // Do nothing for bogus data.
    if (rowBuffer == 0 || static_cast<int>(m_reader->frameYOffset() + rowNumber) >= m_size.height())
      return;

    unsigned colorMapSize;
    unsigned char* colorMap;
    m_reader->getColorMap(colorMap, colorMapSize);
    if (!colorMap)
        return;

    // The buffers that we draw are the entire image's width and height, so a final output frame is
    // width * height RGBA32 values in size.
    //
    // A single GIF frame, however, can be smaller than the entire image, i.e., it can represent some sub-rectangle
    // within the overall image.  The rows we are decoding are within this
    // sub-rectangle.  This means that if the GIF frame's sub-rectangle is (x,y,w,h) then row 0 is really row
    // y, and each row goes from x to x+w.
    unsigned dstPos = (m_reader->frameYOffset() + rowNumber) * m_size.width() + m_reader->frameXOffset();
    unsigned dstEnd = m_size.width() - m_reader->frameXOffset();
    unsigned rgba = 0;
    unsigned char* currentRowByte = rowBuffer;
    unsigned i = 0;
	int isTransparent = m_reader->isTransparent() ? 1 : 0;
	int transparentPixel = m_reader->transparentPixel();
	int writeTransparent = writeTransparentPixels ? 1 : 0;
    while (currentRowByte != rowEnd && i < dstEnd) {
        if ((!isTransparent || *currentRowByte != transparentPixel) && *currentRowByte < colorMapSize) {
            unsigned colorIndex = *currentRowByte * 3;
            unsigned red = colorMap[colorIndex];
            unsigned green = colorMap[colorIndex + 1];
            unsigned blue = colorMap[colorIndex + 2];
            PixelsBuffer::setRGBA(rgba, red, green, blue, 255);
            buffer.bytes().setPixel(dstPos+i, rgba);
        } else {
            m_currentBufferSawAlpha = true;
            // We may or may not need to write transparent pixels to the buffer.
            // If we're compositing against a previous image, it's wrong, and if
            // we're writing atop a cleared, fully transparent buffer, it's
            // unnecessary; but if we're decoding an interlaced gif and
            // displaying it "Haeberli"-style, we must write these for passes
            // beyond the first, or the initial passes will "show through" the
            // later ones.
            if (writeTransparent) {
                PixelsBuffer::setRGBA(rgba, 0, 0, 0, 0);
                buffer.bytes().setPixel(dstPos+i, rgba);
            }
        }
        i++;
        currentRowByte++;
    }

    if (repeatCount > 1) {
        // Copy the row |repeatCount|-1 times.
        unsigned char* dst = buffer.bytes().data() + dstPos * buffer.bytes().bytePixel();
        unsigned size = i * buffer.bytes().bytePixel();
        unsigned width = m_size.width();
        unsigned pitch = width * buffer.bytes().bytePixel();
        unsigned char* end = buffer.bytes().data() + m_size.height() * pitch;
        unsigned char *curr = dst + pitch;
        for (unsigned j = 1; j < repeatCount; j++) {
            if (curr + size > end) // Protect against a buffer overrun from a bogus repeatCount.
                break;
            memcpy(curr, dst, size);
            curr += pitch;
        }
    }

    // Our partial height is rowNumber + 1, e.g., row 2 is the 3rd row, so that's a height of 3.
    // Adding in repeatCount - 1 to rowNumber + 1 works out to just be rowNumber + repeatCount.
    buffer.ensureHeight(rowNumber + repeatCount);
}

void GIFImageDecoder::frameComplete(unsigned frameIndex, unsigned frameDuration, PixelsBuffer::FrameDisposalMethod disposalMethod)
{
    PixelsBuffer& buffer = m_frameBufferCache[frameIndex];
    buffer.ensureHeight(m_size.height());
    buffer.setStatus(PixelsBuffer::FrameComplete);
    buffer.setDuration(frameDuration);
    buffer.setDisposalMethod(disposalMethod);

    if (!m_currentBufferSawAlpha) {
        // The whole frame was non-transparent, so it's possible that the entire
        // resulting buffer was non-transparent, and we can setHasAlpha(false).
        if (buffer.rect().contains(IntRect(IntPoint(0, 0), m_size))) {
            buffer.setHasAlpha(false);
        } else if (frameIndex > 0) {
            // Tricky case.  This frame does not have alpha only if everywhere
            // outside its rect doesn't have alpha.  To know whether this is
            // true, we check the start state of the frame -- if it doesn't have
            // alpha, we're safe.
            //
            // First skip over prior DisposeOverwritePrevious frames (since they
            // don't affect the start state of this frame) the same way we do in
            // initFrameBuffer().
            const PixelsBuffer* prevBuffer = &m_frameBufferCache[--frameIndex];
            while ((frameIndex > 0) &&
                    (prevBuffer->disposalMethod() ==
                        PixelsBuffer::DisposeOverwritePrevious))
                prevBuffer = &m_frameBufferCache[--frameIndex];

            // Now, if we're at a DisposeNotSpecified or DisposeKeep frame, then
            // we can say we have no alpha if that frame had no alpha.  But
            // since in initFrameBuffer() we already copied that frame's alpha
            // state into the current frame's, we need do nothing at all here.
            //
            // The only remaining case is a DisposeOverwriteBgcolor frame.  If
            // it had no alpha, and its rect is contained in the current frame's
            // rect, we know the current frame has no alpha.
            if ((prevBuffer->disposalMethod() ==
                    PixelsBuffer::DisposeOverwriteBgcolor) &&
                    !prevBuffer->hasAlpha() &&
                    buffer.rect().contains(prevBuffer->rect()))
                buffer.setHasAlpha(false);
        }
    }
}

void GIFImageDecoder::gifComplete()
{
    delete m_reader;
    m_reader = 0;
    m_data.clear();
}

}

#endif /* GIF_SUPPORT */

