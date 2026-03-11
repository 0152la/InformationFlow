# Executable for `entropy_eval2`
add_executable(ee2)

set(SOURCE_FILES
    main.cpp
    entropy.cpp
    runner.cpp
    result.cpp
)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME CURR_ROOT_DIR)
set(THIS_INCLUDE_DIR ${INCLUDE_DIR}/${CURR_ROOT_DIR})
set(HEADER_FILES
    main.hpp
    entropy.hpp
    runner.hpp
    runner.tpp
    result.hpp
)
list(TRANSFORM HEADER_FILES PREPEND ${THIS_INCLUDE_DIR}/)

target_sources(ee2
    PRIVATE ${SOURCE_FILES}
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${THIS_INCLUDE_DIR}
    FILES ${HEADER_FILES}
)

target_link_libraries(ee2 m fmt::fmt)
