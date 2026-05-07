#!/bin/bash

build_path="/home/andreilascu/Documents/Repos/InformationFlow/build-rel"
support_files_path=$build_path/src/llvm_gen
export EE_DEF_PATH=$support_files_path/llvm_snippets.def
export EE_LIB_PATH=$support_files_path/libllvm_snippets.so

$build_path/src/entropy_eval/ee2
