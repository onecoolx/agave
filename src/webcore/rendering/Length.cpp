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
#include "Length.h"

#include <wtf/Vector.h>

namespace WebCore {

// Process-wide side-table for calc() expressions. Length is a trivially-copyable
// value type and cannot embed an expression, so a Calculated length stores an
// index into this table. Entries are immutable and deduplicated; the table is
// append-only and bounded by kMaxCalcExpressions to cap memory from untrusted
// CSS. A 0th sentinel entry maps to a no-op so an out-of-range index is safe.
static const int kMaxCalcExpressions = 4096;

static Vector<CalcExpression>& calcTable()
{
    static Vector<CalcExpression>* table = 0;
    if (!table) {
        table = new Vector<CalcExpression>();
        table->append(CalcExpression(0, 0)); // index 0: sentinel / no-op
    }
    return *table;
}

int storeCalcExpression(const CalcExpression& expr)
{
    Vector<CalcExpression>& table = calcTable();
    // Deduplicate against existing entries.
    for (size_t i = 0; i < table.size(); i++) {
        if (table[i] == expr)
            return (int)i;
    }
    if ((int)table.size() >= kMaxCalcExpressions)
        return 0; // table full: fall back to the no-op sentinel
    table.append(expr);
    return (int)table.size() - 1;
}

const CalcExpression& calcExpression(int index)
{
    Vector<CalcExpression>& table = calcTable();
    if (index < 0 || index >= (int)table.size())
        return table[0];
    return table[index];
}

} // namespace WebCore
