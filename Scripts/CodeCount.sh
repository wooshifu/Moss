#!/usr/bin/env bash

set -euo pipefail

THIS_DIR=$(realpath "$(dirname "$0")")
PROJECT_DIR="${THIS_DIR}/.."
scc -M Kconfiglib -M Scripts/IWYU "${PROJECT_DIR}"
