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

#include "config.h"
#include "AffineTransform.h"

#include "FloatRect.h"
#include "IntRect.h"

#include <wtf/MathExtras.h>

namespace WebCore {

bool AffineTransform::isInvertible() const
{
    return det() != 0.0f;
}

AffineTransform& AffineTransform::multiply(const AffineTransform& other)
{
    return (*this) *= other;
}

AffineTransform& AffineTransform::scale(float s)
{
    return scale(s, s);
}

AffineTransform& AffineTransform::scaleNonUniform(float sx, float sy)
{
    return scale(sx, sy);
}

AffineTransform& AffineTransform::rotateFromVector(float x, float y)
{
    return rotate(rad2deg(atan2(y, x)));
}

AffineTransform& AffineTransform::flipX()
{
    return scale(-1.0f, 1.0f);
}

AffineTransform& AffineTransform::flipY()
{
    return scale(1.0f, -1.0f);
}

AffineTransform& AffineTransform::skewX(float angle)
{
    return shear(tan(deg2rad(angle)), 0.0f);
}

AffineTransform& AffineTransform::skewY(float angle)
{
    return shear(0.0f, tan(deg2rad(angle)));
}

}
