# Executable for `entropy_eval`
add_executable(entropy_eval
    entropy_eval-int.c
    entropy_eval-int_cmp.c
    entropy_eval-float.c
    entropy_eval-float_bool.c
    entropy_eval.c)

target_link_libraries(entropy_eval m)
