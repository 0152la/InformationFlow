add_subdirectory(llvm_gen)
add_subdirectory(entropy_eval)
add_subdirectory(path_flow)
add_subdirectory(path_flow_2)

# Abstract required LLVM components
llvm_map_components_to_libnames(llvm_libs support core irreader)

include(./cmake/if_analyse.cmake)
