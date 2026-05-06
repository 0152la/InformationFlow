# Core source files
set(core_sources
    config.cpp
    entropy.cpp
    hashing.cpp
    randgen.cpp
)

set(flow_sources
    ${core_sources}
    entropy_map.cpp
    entropy_map_graph.cpp
    entropy_map_path_printer.cpp
    instr_emulator.cpp
    const_inst_emulator.cpp
    ${LLVM_GEN_SRC_DIR}/llvm_gen-names.cpp
    reader.cpp
)

set(LINK_LIBS fmt::fmt ${llvm_libs})
set(ALL_INCLUDES ${INCLUDE_DIR} ${FMT_INCLUDE_DIR})

################################################################################
# Executable for developing

add_executable(if_analyse
    ${flow_sources}
    main.cpp
)
target_include_directories(if_analyse PRIVATE ${ALL_INCLUDES} ${LLVM_INCLUDE_DIRS})
target_include_directories(if_analyse PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(if_analyse PUBLIC ${TOMLPLUSPLUS_INCLUDE_DIR})
target_link_libraries(if_analyse PRIVATE ${LINK_LIBS})
#target_link_libraries(if_analyse PRIVATE ${llvm_snippets})
#add_dependencies(if_analyse llvm_snippets)
add_dependencies(if_analyse lgen_make_snippets)

################################################################################
# Main library for functional tests

add_library(lflow
    ${flow_sources}
)
target_include_directories(lflow PUBLIC ${ALL_INCLUDES} ${LLVM_INCLUDE_DIRS})
target_include_directories(lflow PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(lflow PUBLIC ${TOMLPLUSPLUS_INCLUDE_DIR})
target_link_libraries(lflow PRIVATE ${LINK_LIBS})
add_dependencies(lflow llvm_snippets)

################################################################################
# Entropy library for entropy tests

add_library(entropy STATIC
    ${core_sources}
)
target_include_directories(entropy PUBLIC ${ALL_INCLUDES})
target_link_libraries(entropy PUBLIC fmt::fmt)
