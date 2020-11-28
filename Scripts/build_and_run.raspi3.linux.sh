#!/usr/bin/env bash

THIS_DIR=$(realpath "$(dirname "$0")")
echo "current dir: ${THIS_DIR}"

BUILD_DIR="${THIS_DIR}"/output/Raspi3/linux
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

cd "${BUILD_DIR}" || exit
cmake -DCMAKE_TOOLCHAIN_FILE="${THIS_DIR}"/../CMake/Toolchain/ToolchainLinuxArm64.cmake -DARCH=arm64 "${THIS_DIR}/.."
make
cd - || exit

echo ">>>>>>>>>>>>>>>> RUNNING KERNEL <<<<<<<<<<<<<<<<"
cmd="qemu-system-aarch64 -M raspi3 -kernel ${BUILD_DIR}/bin/Kernel.elf -serial mon:stdio -nographic"
echo "$cmd"
${cmd}
