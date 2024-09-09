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
    --disable-ajax \
    --disable-xml \
    --disable-xslt \
    --disable-plugin \
    --disable-xpath \
    --disable-ftpdir \
    --disable-schemeextension \
    --disable-jsnativebinding \
    --disable-nativeerror
