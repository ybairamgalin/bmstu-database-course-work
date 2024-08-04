#!/bin/bash

chaotic-gen \
  -f "(.*)={0}" \
  -n "/([^/]*)/=gen::{0}" \
  --clang-format=  \
  --generate-serializers \
  -o /gen \
  --relative-to /etc/schemas \
  /etc/schemas/handlers/request_post.yaml
