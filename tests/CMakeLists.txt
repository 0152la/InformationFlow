#Generic functions
function(add_binary_test test_name)
    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endfunction()

#Entropy tests
add_library(test_harness STATIC test_harness.cpp)
target_include_directories(test_harness PRIVATE ${INCLUDE_DIR})

set(ENTROPY_TEST_LIBS
    entropy
    test_harness
)

function(add_entropy_test test_name)
    add_executable(${test_name}
        ${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE ${ENTROPY_TEST_LIBS})
    add_binary_test(${test_name})
endfunction()

set(entropy_tests
    entropy-coin_flip
    entropy-dice_roll
    entropy-add_raw
    entropy-func_loss
    simple-entropies
)

foreach(entropy_t IN LISTS entropy_tests)
    add_entropy_test(${entropy_t})
endforeach()

target_sources(entropy-func_loss PUBLIC entropy-func_loss-exec.ll-src)
set_source_files_properties(entropy-func_loss-exec.ll-src PROPERTIES LANGUAGE CXX)
target_link_libraries(entropy-func_loss PRIVATE lflow)

# Functionality tests
function(add_func_test test_name)
    add_executable(${test_name}
        ${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE lflow)
    add_binary_test(${test_name})
endfunction()

set(func_tests
    llvm-parse
    llvm-entropy_map
    #llvm-entropy_graph
    #llvm-entropy_map_path_printer
)

foreach(func_t IN LISTS func_tests)
    add_func_test(${func_t})
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
    simple-arith
    simple-cycle
    simple-cycle-early-ret
    simple-cycle-double
    simple-vector
    sample
    minimal
    entropy-func_loss-exec
)

foreach(ll_src_t IN LISTS ll_srcs)
    add_ll_gen_test(${ll_src_t})
endforeach()

# Special `ll` tests for `entropy-func_loss
function(add_special_ll_test target_name out_name src_path extra_flags)
    cmake_path(ABSOLUTE_PATH out_name
               BASE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               OUTPUT_VARIABLE out_path)
    add_custom_command(
        OUTPUT ${out_path}
        COMMAND ${CMAKE_CXX_COMPILER} ${extra_flags} -x c++ -S -emit-llvm -g
                -o ${out_path} ${src_path}
        DEPENDS ${src_path}
        WORKING_DIRECTORY ${TEST_DIR}
    )
    add_custom_target(${target_name}
        DEPENDS ${out_path})
    add_dependencies(gen-lls ${target_name})
endfunction()

add_special_ll_test(efo0 entropy-func_loss-exec-O0.ll entropy-func_loss-exec.ll-src -O0)
add_special_ll_test(efo2 entropy-func_loss-exec-O2.ll entropy-func_loss-exec.ll-src -O2)
