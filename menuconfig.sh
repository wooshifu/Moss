#!/usr/bin/env bash

set -euo pipefail

THIS_DIR=$(realpath "$(dirname "$0")")

pushd "${THIS_DIR}" >/dev/null

./Kconfiglib/Kconfiglib/menuconfig.py

popd >/dev/null
