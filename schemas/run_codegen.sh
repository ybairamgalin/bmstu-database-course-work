#!/bin/bash

echo $(find "$(pwd -P)" -regex ".*\(yaml\)" -type f)

find "$(pwd -P)" -regex ".*\(yaml\)" -type f -exec \
  chaotic-gen \
    -f "(.*)={0}" \
    -n "/([^/]*)/=gen::{0}" \
    --clang-format= \
    --generate-serializers \
    -o /gen \
    --relative-to /etc/schemas \
    {} +
