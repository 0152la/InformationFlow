# Executable for `llvm_snippet.ll` generation
set(LLVM_GEN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/llvm_gen)
add_executable(lgen
    config.cpp
    ${LLVM_GEN_SRC_DIR}/llvm_gen.cpp
    ${LLVM_GEN_SRC_DIR}/llvm_gen-names.cpp
)
target_include_directories(lgen PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS} ${FMT_INCLUDE_DIR})
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
