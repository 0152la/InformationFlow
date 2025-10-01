# Build `fmt`
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmt)
set(FMT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include PARENT_SCOPE)
