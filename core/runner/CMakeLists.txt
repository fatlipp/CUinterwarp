set(PROJ_NAME "interwarp_runner")
set(SOURCE
    interwarp_runner.cpp
)
add_executable(${PROJ_NAME})
target_sources(${PROJ_NAME}
    PRIVATE
        ${SOURCE}
)
target_include_directories(${PROJ_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_DIR}
)
target_link_libraries(${PROJ_NAME} 
    PRIVATE
    dl
)
set_property(TARGET ${PROJ_NAME}  PROPERTY POSITION_INDEPENDENT_CODE ON)