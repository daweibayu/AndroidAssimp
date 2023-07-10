//
// Created by daweibayu on 2023/7/2.
//

#ifndef ANDROIDASSIMP_LOGHELPER_H
#define ANDROIDASSIMP_LOGHELPER_H

#include <android/log.h>

#define LOG_TAG "AssimpAndroid"
#define  LOG_D(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOG_E(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOG_V(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG,__VA_ARGS__)
#define  LOG_I(...)  __android_log_print(ANDROID_LOG_INFO   , LOG_TAG,__VA_ARGS__)
#define  LOG_W(...)  __android_log_print(ANDROID_LOG_WARN   , LOG_TAG,__VA_ARGS__)
#define  LOG_F(...)  __android_log_print(ANDROID_LOG_FATAL   , LOG_TAG,__VA_ARGS__)

#endif //ANDROIDASSIMP_LOGHELPER_H
