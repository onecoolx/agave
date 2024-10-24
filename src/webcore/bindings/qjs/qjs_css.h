 // -*- c-basic-offset: 2 -*-
/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2004, 2006, 2007 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef qjs_css_h
#define qjs_css_h

#include "Color.h"
#include "qjs_binding.h"

namespace WebCore {

    class JSRGBColor {
    public:
        static void init(JSContext*);
        static JSValue create(JSContext*, uint32_t color);

        static JSValue getValueProperty(JSContext * ctx, JSValueConst this_val, int token);

        static JSClassID js_class_id;

        enum { Red, Green, Blue };
    };

    JSValue getJSRGBColor(JSContext*, uint32_t color);

} // namespace WebCore

#endif // kjs_css_h
