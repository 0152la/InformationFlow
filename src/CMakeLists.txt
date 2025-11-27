#add_subdirectory(entropy_eval)
add_subdirectory(path_flow)

# Abstract required LLVM components
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Executable for `llvm_snippet.ll` generation
set(LLVM_GEN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/llvm_gen)
add_executable(lgen
    config.cpp
    ${LLVM_GEN_SRC_DIR}/llvm_gen.cpp
    ${LLVM_GEN_SRC_DIR}/llvm_gen-names.cpp
)
target_include_directories(lgen PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(lgen PRIVATE ${llvm_libs})

# Custom targets to generate `llvm_snippets.ll` and the corresponding dynamic
# library
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.ll
           ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.hpp
    COMMAND lgen
)
add_custom_target(lgen_make_snippets
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.ll
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libllvm_snippets.so
    COMMAND ${CMAKE_CXX_COMPILER} -shared -O2
            -o ${CMAKE_CURRENT_BINARY_DIR}/libllvm_snippets.so
            ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.ll
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/llvm_snippets.ll
)

add_custom_target(llvm_snippets
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libllvm_snippets.so
)

# Core source files
set(core_sources
    config.cpp
    entropy.cpp
    hashing.cpp
    randgen.cpp
)

set(flow_sources
    ${core_sources}
    entropy_map.cpp
    entropy_map_graph.cpp
    entropy_map_path_printer.cpp
    instr_emulator.cpp
    ${LLVM_GEN_SRC_DIR}/llvm_gen-names.cpp
    reader.cpp
)

# Executable for developing
add_executable(iflow
    ${flow_sources}
    main.cpp
)
target_include_directories(iflow PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
target_include_directories(iflow PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(iflow PUBLIC ${FMT_INCLUDE_DIR})
target_link_libraries(iflow PRIVATE ${llvm_libs})
target_link_libraries(iflow PRIVATE fmt::fmt)
#target_link_libraries(iflow PRIVATE ${llvm_snippets})
#add_dependencies(iflow llvm_snippets)
add_dependencies(iflow lgen_make_snippets)

# Main library for functional tests
add_library(lflow
    ${flow_sources}
)
target_include_directories(lflow PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
target_include_directories(lflow PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(lflow PUBLIC ${FMT_INCLUDE_DIR})
target_link_libraries(lflow PRIVATE ${llvm_libs})
target_link_libraries(lflow PRIVATE fmt::fmt)
add_dependencies(lflow llvm_snippets)

# Entropy library for entropy tests
add_library(entropy STATIC
    ${core_sources}
)
target_include_directories(entropy PRIVATE ${INCLUDE_DIR})
