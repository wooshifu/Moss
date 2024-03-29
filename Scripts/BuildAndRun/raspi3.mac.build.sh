#!/usr/bin/env bash

THIS_DIR=$(realpath "$(dirname "$0")")
echo "current dir: ${THIS_DIR}"

BUILD_DIR="${THIS_DIR}"/build/Raspi3/mac
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

cd "${BUILD_DIR}" || exit
cmake -DCMAKE_TOOLCHAIN_FILE="${THIS_DIR}"/../../CMake/Toolchain/ToolchainMacAarch64.cmake -DCONFIG_BOARD=raspberry_pi3 "${THIS_DIR}/../.."
cmake --build . -j"$(nproc)"
cd - || exit
