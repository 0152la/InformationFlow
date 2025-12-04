#!/bin/bash

base_dir=/home/andreilascu/Documents/Repos/InformationFlow/
test_dir=./tests/third_party
out_dir=./tmp/em_data

cd $base_dir || exit
while read -r test_f
do
    echo "=== Running test $test_f"
    IF_LLVM_TEST_FILE=$test_f ./build/tests/llvm-entropy_map > $base_dir/$out_dir/"$(basename -s ".ll" "$test_f")".out 2>/dev/null
done < "$(find $test_dir -iname "*.ll")"
cd - || exit
