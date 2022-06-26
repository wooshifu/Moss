#!/usr/bin/env bash

set -euo pipefail

THIS_DIR=$(realpath "$(dirname "$0")")

BUILD_DIR="${THIS_DIR}"/build/rpi3
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

function print_help() {
  echo "Example Usage:"
  echo "rpi3.build.sh -d -r -c -q \"-d in_asm\""
  echo
  echo "Options:"
  echo "-h  show this help"
  echo "-d  build debug version"
  echo "-r  run qemu after build"
  echo "-c  delete all cmake configuration caches"
  echo "-q  append qemu command args"

  exit 0
}

opt_run_qemu=0
opt_reset_cmake=0
opt_qemu_args=
opt_build_type="Release"

while getopts 'hdrcq:' OPT; do
  case $OPT in
  d) opt_build_type="Debug" ;;
  r) opt_run_qemu=1 ;;
  c) opt_reset_cmake=1 ;;
  q) opt_qemu_args=$OPTARG ;;
  h)
    print_help
    exit 0
    ;;
  *)
    echo "unexpected $OPT -$OPTARG"
    exit 1
    ;;
  esac
done

pushd "${BUILD_DIR}" >/dev/null

if [ ${opt_reset_cmake} == "1" ]; then
  echo "delete ${BUILD_DIR}"
  rm -rf "${BUILD_DIR:?}"/*
fi

cmake_config_command="cmake -DCMAKE_BUILD_TYPE=${opt_build_type} -DCMAKE_TOOLCHAIN_FILE=${THIS_DIR}/../../CMake/Toolchain/ToolchainClang.cmake -DCONFIG_BOARD=rpi3 -GNinja ${THIS_DIR}/../.."
echo "${cmake_config_command}"
${cmake_config_command}
cmake_build_command="cmake --build . -j$(nproc)"
echo "${cmake_build_command}"
${cmake_build_command}

if [ ${opt_run_qemu} == "1" ]; then
  rpi3_qemu_command="qemu-system-aarch64 -M raspi3b -kernel bin/kernel.elf -semihosting -serial mon:null -serial mon:stdio -nographic $opt_qemu_args"
  echo "${rpi3_qemu_command}"
  ${rpi3_qemu_command}
fi

popd >/dev/null
