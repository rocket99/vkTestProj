#!bash

export CC=clang
export CXX=clang++
cd ./linux
cmake -G "Unix Makefiles" \
      -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
      CMAKE_C_COMPILER=clang \
      CMAKE_CXX_COMPILER=clang++ ..
