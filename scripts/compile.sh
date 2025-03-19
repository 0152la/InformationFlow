#!/bin/bash

export CC=${CC:-clang}
export CXX=${CXX:-clang++}
export CMAKE_BUILD_TYPE=Debug

src_dir=$(realpath $(dirname $0)/..)
build_dir=$src_dir/build

cmake -G Ninja -B $build_dir -S $src_dir
cmake --build $build_dir

if [ "$1" == "san" ]
then
    cmake -G Ninja -DCMAKE_CXX_FLAGS="-fsanitize=address" -B $build_dir\_asan -S $src_dir
    cmake --build $build_dir\_asan

    cmake -G Ninja -DCMAKE_CXX_FLAGS="-fsanitize=memory" -B $build_dir\_msan -S $src_dir
    cmake --build $build_dir\_msan
fi
