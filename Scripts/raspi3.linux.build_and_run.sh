#!/usr/bin/env bash

THIS_DIR=$(realpath "$(dirname "$0")")
echo "current dir: ${THIS_DIR}"

"${THIS_DIR}"/raspi3.linux.build.sh

BUILD_DIR="${THIS_DIR}"/output/Raspi3/linux

echo ">>>>>>>>>>>>>>>> RUNNING KERNEL <<<<<<<<<<<<<<<<"
cmd="qemu-system-aarch64 -M raspi3 -kernel ${BUILD_DIR}/bin/Kernel.elf -serial mon:stdio -nographic"
echo "$cmd"
${cmd}
