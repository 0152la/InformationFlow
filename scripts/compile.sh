#!/bin/bash

if [[ -z "${LLVM_DIR}" ]]
then
    export CC=${CC:-clang}
    export CXX=${CXX:-clang++}
else
    export CC=${LLVM_DIR}/../../../bin/clang
    export CXX=${LLVM_DIR}/../../../bin/clang++
fi

src_dir=$(realpath "$(dirname "$0")"/..)
if [ "$1" == "rel" ]
then
    export CMAKE_BUILD_TYPE=Release
    build_dir=$src_dir/build-rel
else
    export CMAKE_BUILD_TYPE=Debug
    build_dir=$src_dir/build
fi

cmake -G Ninja -B "$build_dir" -S "$src_dir"
cmake --build "$build_dir"

if [ "$1" == "ll" ]
then
    cmake --build "$build_dir" --target gen-lls
fi

if [ "$1" == "san" ]
then
    cmake -G Ninja -DCMAKE_CXX_FLAGS="-fsanitize=address" -B "$build_dir"_asan -S "$src_dir"
    cmake --build "$build_dir"_asan

    cmake -G Ninja -DCMAKE_CXX_FLAGS="-fsanitize=memory" -B "$build_dir"_msan -S "$src_dir"
    cmake --build "$build_dir"_msan
fi
