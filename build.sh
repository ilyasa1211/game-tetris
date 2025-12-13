#!/usr/bin/env sh

export PRESET=default

cmake --preset ${PRESET}
cmake --build out/build/${PRESET}
ctest --test-dir out/build/${PRESET}