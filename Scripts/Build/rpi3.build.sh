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
  echo "-t  run smoke test after build"
  echo "-c  delete all cmake configuration caches"
  echo "-q  append qemu command args"

  exit 0
}

opt_smoke_test=0
opt_run_qemu=0
opt_reset_cmake=0
opt_qemu_args=
opt_build_type="Release"
opt_disable_iwyu="ON"

while getopts 'hdrtciq:' OPT; do
  case $OPT in
  d) opt_build_type="Debug" ;;
  r) opt_run_qemu=1 ;;
  t) opt_smoke_test=1 ;;
  c) opt_reset_cmake=1 ;;
  i) opt_disable_iwyu="OFF" ;;
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


if [ ${opt_reset_cmake} == "1" ]; then
  echo "delete ${BUILD_DIR}"
  rm -rf "${BUILD_DIR:?}"/*
fi

cmake_config_command="cmake -S ${THIS_DIR}/../.. -B ${BUILD_DIR} -DOPTION_DISABLE_IWYU=${opt_disable_iwyu} -DCMAKE_BUILD_TYPE=${opt_build_type} -DCMAKE_TOOLCHAIN_FILE=${THIS_DIR}/../../CMake/Toolchain/ToolchainClang.cmake -DCONFIG_PROJECT=rpi3 -GNinja"
echo "${cmake_config_command}"
${cmake_config_command}
cmake_build_command="cmake --build ${BUILD_DIR} -j$(nproc)"
echo "${cmake_build_command}"
eval "${cmake_build_command}"

rpi3_qemu_command="qemu-system-aarch64 -M raspi3b -kernel ${BUILD_DIR}/bin/kernel.elf -serial null -serial mon:stdio -nographic $opt_qemu_args"

kill_timeout=3
if [ "$(uname)" == "Darwin" ]; then
  kill_timeout=5
fi

if [ ${opt_smoke_test} == "1" ]; then
  msg="moss operating system"
  smoke_test_command="timeout -s KILL ${kill_timeout} ${rpi3_qemu_command} | tee output.log || true"
  echo "running smoke test: \"${smoke_test_command}\""
  echo "${smoke_test_command}" | eval "$(cat -)" 2>/dev/null
  grep "${msg}" output.log >/dev/null
  rm -f output.log
fi

if [ ${opt_run_qemu} == "1" ]; then
  echo "${rpi3_qemu_command}"
  eval "${rpi3_qemu_command}"
fi
