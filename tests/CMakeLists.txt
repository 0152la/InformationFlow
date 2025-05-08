#Generic functions
function(add_binary_test test_name)
    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endfunction()

#Entropy tests
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

# `ll` generation tests
add_custom_target(gen-lls)

function(add_ll_gen_test ll_src_t)
    cmake_path(REPLACE_EXTENSION ll_src_t "ll-src")
    cmake_path(REPLACE_EXTENSION ll_src_t "ll" OUTPUT_VARIABLE ll_test_name)
    cmake_path(ABSOLUTE_PATH ll_test_name
               BASE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               OUTPUT_VARIABLE ll_test_path)
    add_custom_command(
        OUTPUT ${ll_test_path}
        COMMAND ${CMAKE_CXX_COMPILER} -x c++ -S -emit-llvm -g
                -o ${ll_test_path} ${ll_src_t}
        DEPENDS ${ll_src_t}
        WORKING_DIRECTORY ${TEST_DIR}
    )
    add_custom_target(${ll_src_t}
        DEPENDS ${ll_test_path})
    add_dependencies(gen-lls ${ll_src_t})
endfunction()

set(ll_srcs
    simple-add
    sample
    simple-arith
    simple-vector
)

foreach(ll_src_t IN LISTS ll_srcs)
    add_ll_gen_test(${ll_src_t})
endforeach()
