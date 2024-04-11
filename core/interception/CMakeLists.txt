find_package(CUDA 12.2 REQUIRED)

get_filename_component(CUDA_COMPILER_BIN "${CMAKE_CUDA_COMPILER}" DIRECTORY)
get_filename_component(CUDA_DIR "${CUDA_COMPILER_BIN}" DIRECTORY)
set(CUDA_INCLUDE "${CUDA_DIR}/include")

set(PROJ_NAME "interwarp_lib")
set(SOURCE
    interwarp_lib.cpp
)
add_library(${PROJ_NAME} SHARED)
target_sources(${PROJ_NAME}
    PRIVATE
        ${SOURCE}
)
target_include_directories(${PROJ_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_DIR}
        ${CUDA_INCLUDE}
)
target_link_libraries(${PROJ_NAME} 
    PRIVATE
    dl
)
set_property(TARGET ${PROJ_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)