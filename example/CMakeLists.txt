SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dlineinfo -fpic")

# SET(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_80,code=sm_80")

find_package(CUDA 12.2 REQUIRED)

get_filename_component(CUDA_COMPILER_BIN "${CMAKE_CUDA_COMPILER}" DIRECTORY)
get_filename_component(CUDA_DIR "${CUDA_COMPILER_BIN}" DIRECTORY)
set(CUDA_INCLUDE "${CUDA_DIR}/include")

set(PROJ_NAME "example_app")
set(SOURCE
    main.cu
    main.cpp
)
add_executable(${PROJ_NAME})
target_sources(${PROJ_NAME}
    PRIVATE
        ${SOURCE}
)
target_include_directories(${PROJ_NAME}
    PRIVATE
        ${PROJECT_DIR}
        ${CUDA_INCLUDE}
)
target_link_libraries(${PROJ_NAME} 
    PUBLIC
        cuda
        cublas
        cudart
        ${CUDA_LIBRARIES}
)

set_target_properties(${PROJ_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION OFF)
set_target_properties(${PROJ_NAME} PROPERTIES CUDA_ARCHITECTURES "86")
set_property(TARGET ${PROJ_NAME}  PROPERTY POSITION_INDEPENDENT_CODE ON)
