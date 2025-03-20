add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sample.ll
    COMMAND ${CMAKE_CXX_COMPILER} -S -emit-llvm -o ${CMAKE_CURRENT_BINARY_DIR}/sample.ll sample.c
    DEPENDS sample.c
    WORKING_DIRECTORY ${TEST_DIR}
)

add_custom_target(sample-ll
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sample.ll
)
