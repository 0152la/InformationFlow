# Executable for `entropy_eval2`
add_executable(ee2
    main.cpp
    entropy.cpp
    runner.cpp
    result.cpp
)

target_link_libraries(ee2 m fmt::fmt)
