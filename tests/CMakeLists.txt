# Custom targets
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sample.ll
    COMMAND ${CMAKE_CXX_COMPILER} -S -emit-llvm -o ${CMAKE_CURRENT_BINARY_DIR}/sample.ll sample.c
    DEPENDS sample.c
    WORKING_DIRECTORY ${TEST_DIR}
)

add_custom_target(sample-ll
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sample.ll
)

# Generic functions
function(add_binary_test test_name)
    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endfunction()

# Entropy tests
set(ENTROPY_LIBS
    entropy)

add_library(test_harness SHARED test_harness.cpp)
target_include_directories(test_harness PRIVATE ${INCLUDE_DIR})

function(add_entropy_test test_name)
    add_executable(${test_name}
        ${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE test_harness ${ENTROPY_LIBS})
    target_include_directories(${test_name} PRIVATE ${INCLUDE_DIR})
    add_binary_test(${test_name})
endfunction()

set(entropy_tests
    entropy-coin_flip
    entropy-dice_roll
)

foreach(entropy_t IN LISTS entropy_tests)
    add_entropy_test(${entropy_t})
endforeach()
