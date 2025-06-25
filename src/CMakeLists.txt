# Library for llvm snippets, used for fuzzing
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libllvm_snippets.so
    COMMAND clang++ -shared -O2
            -o ${CMAKE_CURRENT_BINARY_DIR}/libllvm_snippets.so
            ${CMAKE_CURRENT_SOURCE_DIR}/instr_snippets.ll
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/instr_snippets.ll
)
add_custom_target(llvm_snippets
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libllvm_snippets.so
)

# Core source files
set(flow_sources
    entropy.cpp
    entropy_map.cpp
    entropy_map_graph.cpp
    entropy_map_path_printer.cpp
    fuzz_engine.cpp
    hashing.cpp
    instr_emulator.cpp
    randgen.cpp
    reader.cpp
)

# Executable for developing
add_executable(iflow
    ${flow_sources}
    main.cpp
)
target_include_directories(iflow PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(iflow PRIVATE ${llvm_libs})
add_dependencies(iflow llvm_snippets)

# Main library for functional tests
add_library(lflow
    ${flow_sources}
)
target_include_directories(lflow PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(lflow PRIVATE ${llvm_libs})
add_dependencies(lflow llvm_snippets)

# Entropy library for entropy tests
add_library(entropy
    entropy.cpp
    randgen.cpp
)
target_include_directories(entropy PUBLIC ${INCLUDE_DIR})

