/*
 * Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef Icon_h
#define Icon_h

#include "Shared.h"
#include <wtf/Forward.h>

namespace WebCore {

class GraphicsContext;
class IntRect;
class String;
    
class Icon : public Shared<Icon> {
public:
    virtual ~Icon();
    
    static PassRefPtr<Icon> newIconForFile(const String& filename);

    void paint(GraphicsContext*, const IntRect&);

private:
    void* m_hIcon;
};

}

#endif
