cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# "../" --> output directory relative same to running in IDE
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

set(PROJ_NAME warp)

project(${PROJ_NAME})

# no OpenGL REQUIRED :-)

include_directories(./src)

add_executable (${PROJ_NAME} WIN32 ./src/demo.cpp ./src/demo.h ./src/bitmap.hpp ./src/vector.hpp ./src/text.cpp ./src/text.h ./src/stb_image.h warp.rc)

# place these AFTER add_executable
target_link_libraries(${PROJ_NAME} winmm.lib)
