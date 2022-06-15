#!/usr/bin/env python3

import itertools
import os.path
import pathlib
import re

project_dir = pathlib.Path(os.path.dirname(os.path.realpath(__file__))).parent

asm_files = [
    'SourceCode/Arch/aarch64/src/start.cc',
    'SourceCode/Arch/aarch64/src/secondary.cc',
    'SourceCode/Arch/aarch64/src/el.cc',
]
asm_files = [os.path.join(project_dir, f) for f in asm_files]

source_code_dir = os.path.join(project_dir, "SourceCode")
source_files = [[os.path.join(root, f) for f in files] for (root, _, files) in os.walk(source_code_dir) if files]
source_files = [f for f in itertools.chain(*source_files)]

for asm_file in asm_files:
    file_lines = open(asm_file, 'r').readlines()
    for index, line in enumerate([line for line in file_lines if line]):
        match = re.match(r'.*?\.include\s+["<]([a-zA-Z\d_./]+)[">].*?', line)
        if not match:
            continue
        source_file = [f for f in source_files if f.endswith(match.group(1))]
        if not source_file:
            continue
        if len(source_file) > 1:
            print(f'found {len(source_file)} for {match.group(1)}, substitution skipped')
            continue
        file_lines[index] = re.sub(r'\.include\s+(["<][a-zA-Z\d_./]+[">])', f'.include "{source_file[0]}"', line)
    print(f'replacing {asm_file}')
    open(asm_file, 'w').write(''.join(file_lines))
