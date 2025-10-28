cmake_minimum_required(VERSION 3.20)
project(InformationFlow LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Build third party dependencies
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party)

# Set global compilation options
add_compile_options(-pedantic -Wextra)
add_compile_options($<$<CONFIG:Debug>:-Werror>)

# Enable `compile_commands.json` generations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# If we set `LLVM_DIR`, lookg for packages
if(DEFINED ENV{LLVM_DIR})
    find_package(LLVM REQUIRED CONFIG)
    include_directories(${LLVM_INCLUDE_DIRS})
    separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
    add_definitions(${LLVM_DEFINITIONS_LIST})
endif()

# Set useful directory variables
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Build sources
add_subdirectory(${SRC_DIR})

# Enable CTest and build tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
add_subdirectory(${TEST_DIR})
