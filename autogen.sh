#!/bin/sh

echo n | libtoolize --copy --force
aclocal
autoconf
autoheader
automake --add-missing 


CFLAGS="-Os -g" \
CXXFLAGS="-O1 -g" \
./configure \
    --disable-png \
    --disable-ajax \
    --disable-xml \
    --disable-xslt \
    --disable-plugin \
    --disable-xpath \
    --disable-ftpdir \
    --disable-schemeextension \
    --disable-jsnativebinding \
    --disable-nativeerror
