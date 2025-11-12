# Executable for `entropy_eval`
add_executable(entropy_eval
    entropy_eval-thread.c
    entropy_eval.c)

target_link_libraries(entropy_eval m)
