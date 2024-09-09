#!/bin/sh

echo n | libtoolize --copy --force
aclocal
autoconf
autoheader
automake --add-missing 

FLAGS="-Os -g"

CFLAGS="$FLAGS" \
CXXFLAGS="$FLAGS"  \
./configure \
    --disable-plugin \
    --disable-schemeextension \
    --disable-jsnativebinding \
    --disable-nativeerror \
    --disable-xml \
    --disable-xslt \
    --disable-xpath
