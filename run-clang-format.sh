#!/usr/bin/env bash

# run this bash script to format all source code under SourceCode directory

current_dir=$(dirname "$0")

source_code_files=$(find "${current_dir}/SourceCode" -type f \( -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.h' -o -name '*.hh' -o -name '*.hpp' \))
echo -e "will format following files:\n$source_code_files"
files=$(echo "${source_code_files}" | awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}')
# shellcheck disable=SC2086
clang-format -i ${files}
