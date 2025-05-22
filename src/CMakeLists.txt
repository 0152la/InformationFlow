add_executable(iflow
    entropy.cpp
    entropy_map.cpp
    entropy_map_graph.cpp
    entropy_map_path_printer.cpp
    fuzz_engine.cpp
    instr_emulator.cpp
    main.cpp
    randgen.cpp
    reader.cpp
)
target_include_directories(iflow PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(iflow PRIVATE ${llvm_libs})

#add_executable(reader
    #reader.cpp
    #reader-basicblock.cpp
#target_include_directories(reader PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
#llvm_map_components_to_libnames(llvm_libs support core irreader)
#target_link_libraries(reader PRIVATE ${llvm_libs})

add_library(entropy
    entropy.cpp
    randgen.cpp
)
target_include_directories(entropy PUBLIC ${INCLUDE_DIR})
