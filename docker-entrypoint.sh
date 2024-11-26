#!/usr/bin/env bash

make build-debuf && make -- --in-docker-start-debug
