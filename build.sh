#!/bin/bash

export CC=clang
export CXX=clang++
mkdir -p build && cd build

cmake -G "Unix Makefiles" \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
      CMAKE_C_COMPILER=clang \
      CMAKE_CXX_COPMPILER=clang++ \
      ..
