#!/bin/bash

set -euo pipefail

mkdir build || true
pushd build
cmake ..
make
popd
