cmake_minimum_required(VERSION 3.16)

# PlatformIO only registers src/ as a component dir. Add our own modules
# so ESP-IDF picks up their CMakeLists.
list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/lib")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(twilight_switch)
