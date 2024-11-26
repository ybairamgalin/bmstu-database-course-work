!/usr/bin/bash

set -e

mkdir -p /code/build
cd /code/build && cmake build

cmake --build build_debug -j 8 --target pg_service_template
