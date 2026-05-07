llvm_map_components_to_libnames(llvm_libs support core irreader)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME CURR_ROOT_DIR)
set(THIS_INCLUDE_DIR ${INCLUDE_DIR}/${CURR_ROOT_DIR})

# Executable for `llvm_snippet.ll` generation
add_executable(lgen
    llvm_gen.cpp
    llvm_gen-names.cpp
)
target_include_directories(lgen PRIVATE ${THIS_INCLUDE_DIR} ${LLVM_INCLUDE_DIRS} ${FMT_INCLUDE_DIR})
target_link_libraries(lgen PRIVATE ${llvm_libs} fmt::fmt)

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
