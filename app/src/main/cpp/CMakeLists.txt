# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)
project("AndroidAssimp")

# 添加本地代码
aux_source_directory(jni JNI_FILES)
aux_source_directory(utils UTIL_FILES)
aux_source_directory(modelAssimp ASSIMP_FILES)
aux_source_directory(shader SHADER_FILES)
add_library(AndroidAssimp SHARED
        ${JNI_FILES}
        ${UTIL_FILES}
        ${ASSIMP_FILES}
        ${SHADER_FILES})

find_library(LOG_LIB log)

target_include_directories(AndroidAssimp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIBS "${CMAKE_SOURCE_DIR}/../jniLibs")
set(OPENGL EGL GLESv2 GLESv3 m)

# 添加 glm
aux_source_directory (${CMAKE_SOURCE_DIR}/include/glm-0.9.9.8/ GLM)
aux_source_directory (${GLM}/gtc GTC)
aux_source_directory (${GLM}/gtx GTX)
aux_source_directory (${GLM}/detail DETAIL)

target_include_directories(AndroidAssimp PRIVATE ${GLM} ${GTC} ${DETAIL} ${GTX})

# 添加 assimp
add_library(ASSIMP SHARED IMPORTED)
set_target_properties(ASSIMP PROPERTIES IMPORTED_LOCATION ${LIBS}/arm64-v8a/libassimp.so)

# 添加 opencv
add_library(OPENCV SHARED IMPORTED)
set_target_properties(OPENCV PROPERTIES IMPORTED_LOCATION ${LIBS}/arm64-v8a/libopencv_java4.so)

target_link_libraries(
        AndroidAssimp
        ASSIMP
        OPENCV
        ${OPENGL}
        android
        ${LOG_LIB})