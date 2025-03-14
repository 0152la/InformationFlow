cmake_minimum_required(VERSION 3.20)
project(InformationFlow LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Set global compilation options
add_compile_options(-pedantic -Wextra $<$<CONFIG:Debug>:-Werror>)

# Enable `compile_commands.json` generations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set useful directory variables
#set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Build sources
add_subdirectory(${SRC_DIR})
