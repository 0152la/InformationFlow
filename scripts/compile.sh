#!/bin/bash

export CC=clang

src_dir=/home/sentenced/Documents/Work/UCL/Repos/InformationFlow
build_dir=$src_dir/build

cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -B $build_dir -S $src_dir
cmake --build $build_dir
