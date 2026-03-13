# Executable for `entropy_eval2`
add_executable(ee2)

set(SOURCE_FILE_NAMES
    entropy
    main
    result
    runner
    tests
    utils
)

set(SOURCE_FILES ${SOURCE_FILE_NAMES})
list(TRANSFORM SOURCE_FILES APPEND .cpp)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME CURR_ROOT_DIR)
set(THIS_INCLUDE_DIR ${INCLUDE_DIR}/${CURR_ROOT_DIR})
set(HEADER_FILES ${SOURCE_FILE_NAMES})
list(TRANSFORM HEADER_FILES APPEND .hpp)
list(APPEND HEADER_FILES
    runner.tpp
    tests.tpp
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
