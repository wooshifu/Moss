#!/usr/bin/env bash

BASEDIR=$(dirname "$0")

files=$(find "${BASEDIR}/SourceCode" -type file \( -name '*.h' -o -name '*.hh' -o -name '*.hpp' -o -name '*.c' -o -name '*.cc' -o -name '*.cpp' \) | grep -v .git | grep  -v LibGccStd)

for file in ${files}; do
  echo "clang-format: ${file}"
  clang-format -i "${file}"
done
