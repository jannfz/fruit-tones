#!/bin/bash
# Usage: ./html_to_header.sh web/index.html src/index_html.h index_html

INFILE=$1
OUTFILE=$2
VAR_NAME=$3

echo "const char ${VAR_NAME}[] PROGMEM = R\"rawliteral(" > "$OUTFILE"
cat "$INFILE" >> "$OUTFILE"
echo ")rawliteral\";" >> "$OUTFILE"
