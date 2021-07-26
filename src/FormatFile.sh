#this script takes a file, removes empty lines, sorts it, removes any duplicates, and replaces it!

#!/bin/bash

tmp=.tmp.xtx

sed '/^\s*$/d' $1  | LC_ALL=C sort --general-numeric-sort -u > $tmp

nLines=$(awk 'END{print NR}' $tmp)
awk -v nLines=$nLines 'NR!=nLines {printf "%s\n", $0}; NR==nLines {printf "%s", $0};' $tmp > $1

rm $tmp
