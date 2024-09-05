/*
 * Copyright (C) 2005, 2006 Apple Computer, Inc.  All rights reserved.
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

#ifndef AffineTransform_h
#define AffineTransform_h

#include <picasso.h>

namespace WebCore {

class IntPoint;
class FloatPoint;
class IntRect;
class FloatRect;

typedef ps_matrix* PlatformTransform;

class AffineTransform {
public:
    AffineTransform();
    AffineTransform(PlatformTransform);
    AffineTransform(float a, float b, float c, float d, float e, float f);
	AffineTransform(const AffineTransform& other);
	~AffineTransform();

    void setMatrix(float a, float b, float c, float d, float e, float f);

    void map(int x, int y, int *x2, int *y2) const;
    void map(float x, float y, float *x2, float *y2) const;
    IntRect mapRect(const IntRect&) const;
    FloatRect mapRect(const FloatRect&) const;
    IntPoint mapPoint(const IntPoint&) const;
    FloatPoint mapPoint(const FloatPoint&) const;
    
    bool isIdentity() const;
    
    float a() const;
    void setA(float a);

    float b() const;
    void setB(float b);

    float c() const;
    void setC(float c);

    float d() const;
    void setD(float d);

    float e() const;
    void setE(float e);

    float f() const;
    void setF(float f);

    void reset();

    AffineTransform& multiply(const AffineTransform&);
    AffineTransform& scale(float); 
    AffineTransform& scale(float sx, float sy); 
    AffineTransform& scaleNonUniform(float sx, float sy);
    AffineTransform& rotate(float d);
    AffineTransform& rotateFromVector(float x, float y);
    AffineTransform& translate(float tx, float ty);
    AffineTransform& shear(float sx, float sy);
    AffineTransform& flipX();
    AffineTransform& flipY();
    AffineTransform& skewX(float angle);
    AffineTransform& skewY(float angle);
 
    float det() const;
    bool isInvertible() const;
    AffineTransform inverse() const;

    bool operator==(const AffineTransform&) const;
    bool operator!=(const AffineTransform& other) const { return !(*this == other); }
    AffineTransform& operator*=(const AffineTransform&);
    AffineTransform operator*(const AffineTransform&);
    
	AffineTransform& operator=(const AffineTransform& other);

	PlatformTransform platformTransform(void) const { return m; }
private:
	PlatformTransform m;
};

} // namespace WebCore

#endif // AffineTransform_h
