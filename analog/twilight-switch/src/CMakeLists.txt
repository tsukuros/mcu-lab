FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.c)

idf_component_register(SRCS ${app_sources}
                       INCLUDE_DIRS "."
                       REQUIRES ldr twilight relay)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wextra -Wunused-parameter)
