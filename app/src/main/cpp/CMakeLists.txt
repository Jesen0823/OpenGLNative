
cmake_minimum_required(VERSION 3.22.1)

project("openglnative")

include_directories(
        util
        render
        sample
)

find_library(
        log-lib
        log)

find_library(
        android-lib
        android)

file(GLOB src_files
        ${CMAKE_SOURCE_DIR}/util/*.cpp
        ${CMAKE_SOURCE_DIR}/render/*.cpp
        ${CMAKE_SOURCE_DIR}/sample/*.cpp)

add_library(${CMAKE_PROJECT_NAME} SHARED
        JNIImpl.cpp
        util/GLUtils.cpp
        sample/TriangleSample.cpp
        sample/TextureMapSample.cpp
        sample/NV21TextureMapSample.cpp
        sample/VaoSample.cpp
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${src_files})

target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        ${log-lib}
        ${android-lib}
        GLESv3
        EGL)