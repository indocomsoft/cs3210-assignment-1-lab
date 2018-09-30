#!/bin/bash

if [ "$1" = "" ]; then
  echo 'Usage: test.sh <directory containing test files>'
  exit 1
fi

out="${1%/}-out"
finished="${1%/}-finished"

mkdir -p $out
mkdir -p $finished

for i in ${1%/}/*; do
  echo sim "<" $i ">" "$out/$(basename $i)"
  # echo $(ls $1 | wc -l) remaining
  # echo $(ls $out | wc -l) finished
  (perf stat -e branch-misses,cache-misses,instructions,cycles,context-switches ./sim < "$i") > "$out/$(basename $i)" 2>&1
  mv $i $finished/
done
echo $out
