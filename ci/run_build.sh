#!/usr/bin/bash

set -e
set -o pipefail

mkdir -p /code/build
cd /code/build
cmake .. .
cd /code
cmake --build build -j 12 --target pg_service_template
cmake --build build -j 12 --target pg_service_template_unittest
cmake --build build -j 12 --target pg_service_template_inttest
