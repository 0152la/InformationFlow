# Abstract required LLVM components
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Executable for `llvm_snippet.ll` generation
add_executable(lgen
    llvm_gen.cpp
)
target_include_directories(lgen PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(lgen PRIVATE ${llvm_libs})

# Custom targets to generate `llvm_snippets.ll` and the corresponding dynamic
# library
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.ll
    COMMAND lgen
)
add_custom_target(lgen_make_snippets
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.ll
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libllvm_snippets.so
    COMMAND clang++ -shared -O2
            -o ${CMAKE_CURRENT_BINARY_DIR}/libllvm_snippets.so
            ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.ll
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.ll
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
add_library(entropy STATIC
    entropy.cpp
    randgen.cpp
    hashing.cpp
)
target_include_directories(entropy PUBLIC ${INCLUDE_DIR})

