#!/bin/sh

. "$(dirname "$0")/run_tests-envs.sh"

cd build || return
ctest --output-on-failure
cd - || return
