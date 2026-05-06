add_subdirectory(entropy_eval)
add_subdirectory(entropy_eval2)
add_subdirectory(path_flow)
add_subdirectory(path_flow_2)

# Abstract required LLVM components
llvm_map_components_to_libnames(llvm_libs support core irreader)

include(./cmake/llvm_gen.cmake)
include(./cmake/if_analyse.cmake)
