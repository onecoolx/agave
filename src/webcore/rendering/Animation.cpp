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
#include "Animation.h"

namespace WebCore {

// Evaluate a unit cubic-bezier with end points (0,0) and (1,1) and control
// points (x1,y1),(x2,y2). The curve is parameterized by s in [0,1]:
//   x(s) = 3(1-s)^2 s x1 + 3(1-s) s^2 x2 + s^3
//   y(s) = 3(1-s)^2 s y1 + 3(1-s) s^2 y2 + s^3
// Given a linear time fraction t we solve x(s) == t for s (Newton-Raphson with
// bisection fallback), then return y(s).
double TimingFunction::evaluate(double t) const
{
    if (t <= 0.0)
        return 0.0;
    if (t >= 1.0)
        return 1.0;

    // Polynomial coefficients for x(s) and y(s) in the form a*s^3 + b*s^2 + c*s.
    const double cx = 3.0 * m_x1;
    const double bx = 3.0 * (m_x2 - m_x1) - cx;
    const double ax = 1.0 - cx - bx;

    const double cy = 3.0 * m_y1;
    const double by = 3.0 * (m_y2 - m_y1) - cy;
    const double ay = 1.0 - cy - by;

    // Solve x(s) = t for s.
    double s = t; // initial guess
    for (int i = 0; i < 8; ++i) {
        double x = ((ax * s + bx) * s + cx) * s - t;
        if (x > -1e-6 && x < 1e-6)
            break;
        double dx = (3.0 * ax * s + 2.0 * bx) * s + cx;
        if (dx < 1e-6 && dx > -1e-6)
            break; // derivative too small; fall back to bisection below
        s -= x / dx;
    }

    // Bisection fallback to keep s within [0,1] and refine if Newton stalled.
    double lo = 0.0, hi = 1.0;
    if (s < lo)
        s = lo;
    else if (s > hi)
        s = hi;
    {
        double x = ((ax * s + bx) * s + cx) * s;
        if (x < t - 1e-6 || x > t + 1e-6) {
            for (int i = 0; i < 20; ++i) {
                double mid = (lo + hi) * 0.5;
                double xm = ((ax * mid + bx) * mid + cx) * mid;
                if (xm < t)
                    lo = mid;
                else
                    hi = mid;
                if (xm > t - 1e-6 && xm < t + 1e-6) {
                    s = mid;
                    break;
                }
                s = mid;
            }
        }
    }

    return ((ay * s + by) * s + cy) * s;
}

} // namespace WebCore
