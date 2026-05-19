#!/bin/bash
# Regenerate QuickJS binding code from IDL files
# Usage: ./tools/generate_qjs_bindings.sh [clean]

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJ_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
WEBCORE="$PROJ_ROOT/src/webcore"
OUTPUT_DIR="$WEBCORE/buildQJS"

if [ "$1" = "clean" ]; then
    echo "Cleaning generated QJS bindings..."
    rm -f "$OUTPUT_DIR"/QJS*.h "$OUTPUT_DIR"/QJS*.cpp
    echo "Done."
    exit 0
fi

echo "Generating QuickJS bindings..."
echo "  WebCore: $WEBCORE"
echo "  Output:  $OUTPUT_DIR"

mkdir -p "$OUTPUT_DIR"

make -C "$OUTPUT_DIR" \
    -f "$WEBCORE/DerivedSourcesQJS.make" \
    WebCore="$WEBCORE" \
    FEATURE_DEFINES="ENABLE_SVG ENABLE_XPATH ENABLE_XSLT"

echo ""
echo "QuickJS bindings generated successfully."
echo "  Output directory: $OUTPUT_DIR"
echo "  Generated files:  $(ls "$OUTPUT_DIR"/QJS*.h 2>/dev/null | wc -l) headers"
