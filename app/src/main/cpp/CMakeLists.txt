# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)
project("androidassimp")

aux_source_directory(jni jniFiles)
aux_source_directory(utils utilsFiles)
aux_source_directory(modelAssimp modelAssimpFiles)
aux_source_directory(shader shaderFiles)
add_library(androidassimp SHARED
        ${jniFiles}
        ${utilsFiles}
        ${modelAssimpFiles}
        ${shaderFiles}
        modelAssimp/AssimpLoader.cpp
        modelAssimp/AssimpLoader.h)

find_library(log-lib log)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(libs "${CMAKE_SOURCE_DIR}/../jniLibs")
set(opengl EGL GLESv2 GLESv3 m)

aux_source_directory (${CMAKE_SOURCE_DIR}/include/glm-0.9.9.8/ GLM)
aux_source_directory (${GLM}/gtc GTC)
aux_source_directory (${GLM}/gtx GTX)
aux_source_directory (${GLM}/detail DETAIL)

target_include_directories(androidassimp PRIVATE ${GLM} ${GTC} ${DETAIL} ${GTX})

add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${libs}/arm64-v8a/libassimp.so)

add_library(opencv SHARED IMPORTED)
set_target_properties(opencv PROPERTIES IMPORTED_LOCATION ${libs}/arm64-v8a/libopencv_java4.so)

target_link_libraries(
        androidassimp
        assimp
        opencv
        ${opengl}
        android
        ${log-lib})