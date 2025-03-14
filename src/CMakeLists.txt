add_executable(iflow
    main.cpp
    entropy.cpp
)
target_include_directories(iflow PRIVATE ${INCLUDE_DIR})
