### Overview

This tool is a proof-of-concept of abstracting the execution of a program to
its control flow via the uncertainty coefficient [^1]. For a given LLVM IR
file, we statically identify all potential paths in it, and compute an
associated uncertainty coefficient with the path. The idea is that places in
the paths where more information is lost is more susceptible to failed error
propagation [^2], thus meaning that an associated test suite for the program
under test should have more tests covering those program points.

A more in-depth write-up can be found in `./notes/LatexNotes/LLVMOpsDiscussion`.

### Building and Running

To compile the analyser, run `./scripts/compile.sh`. This produces
`./build/src/if_analyse`, which can then be executed with a single
LLVM-compatible source file (tests `C` and `C++`) to run the analysis and
produce the uncertainty coefficient for static paths in the program.

#### Testing

The script `./scripts/run_tests.sh` should set and run everything.

Alternatively, build the `ll` files from the tests, then set environment
variables, and finally run the tests:

```
./scripts/compile.sh ll
. ./scripts/run_tests-envs.sh
(cd build && ctest)
```

This approach allows for setting the target LLVM IR source via
`IF_LLVM_TEST_FILE`.

#### Requirements

* `llvm` (tested with version 22)
* `clang` (tested with version 22)
* `cmake` >= 3.20

#### Docker

To build the project in a fresh Docker image:

```
cd ./script/Docker
./docker-build.sh
```

For additional options, run `docker-build.sh -h`.

Links:
[^1]: https://en.wikipedia.org/wiki/Uncertainty_coefficient
[^2]: http://www0.cs.ucl.ac.uk/staff/d.clark/pubs/icse_sq.pdf
