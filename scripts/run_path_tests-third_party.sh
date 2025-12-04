#!/bin/bash

third_party_dir=$IF_BUILD_DIR/../tests/third_party
test_f=("dynarr" "kgflags")
test_bin=$IF_BUILD_DIR/tests/llvm-entropy_map_path_printer

for test_o in "${test_f[@]}"
do
    test_p="$third_party_dir"/"$test_o".ll
    echo "=== RUN $test_p"
    IF_LLVM_TEST_FILE=$test_p $test_bin
done
