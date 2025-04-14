add_executable(iflow
    main.cpp
    entropy.cpp
    reader.cpp
    randgen.cpp
    instr_emulator.cpp
    fuzz_engine.cpp
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
