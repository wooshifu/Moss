#!/usr/bin/env bash

set -euo pipefail

THIS_DIR=$(realpath "$(dirname "$0")")

BUILD_DIR="${THIS_DIR}"/build/rpi3
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

function print_help() {
  echo "Example Usage:"
  echo "rpi3.build.sh -r -c -q \"-d in_asm\""
  echo
  echo "Options:"
  echo "-h  show this help"
  echo "-r  run qemu after build"
  echo "-c  delete all cmake configuration caches"
  echo "-q  append qemu command args"

  exit 0
}

opt_run_qemu=0
opt_reset_cmake=0
opt_qemu_args=

while getopts 'hrcq:' OPT; do
  case $OPT in
  r) opt_run_qemu=1 ;;
  c) opt_reset_cmake=1 ;;
  q) opt_qemu_args=$OPTARG ;;
  h)
    print_help
    exit 0
    ;;
  :)
    echo "This option -$OPTARG requires an argument."
    exit 1
    ;;
  ?)
    echo "-$OPTARG is not an option"
    exit 2
    ;;
  esac
done

pushd "${BUILD_DIR}" >/dev/null

if [ ${opt_reset_cmake} == "1" ]; then
  echo "delete ${BUILD_DIR}"
  rm -rf "${BUILD_DIR:?}"/*
fi

cmake -DCMAKE_TOOLCHAIN_FILE="${THIS_DIR}"/../../CMake/Toolchain/ToolchainClang.cmake -DCONFIG_BOARD=rpi3 -GNinja "${THIS_DIR}/../.."
cmake --build . -j"$(nproc)"

if [ ${opt_run_qemu} == "1" ]; then
  rpi3_qemu_command="qemu-system-aarch64 -M raspi3b -kernel bin/kernel.elf -nographic -serial mon:stdio $opt_qemu_args"
  echo "${rpi3_qemu_command}"
  ${rpi3_qemu_command}
fi

popd >/dev/null
