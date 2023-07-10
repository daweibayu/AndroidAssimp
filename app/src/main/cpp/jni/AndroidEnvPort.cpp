//
// Created by daweibayu on 2023/7/3.
//
#include <jni.h>
#include <string>
#include "../utils/StringUtils.h"
#include <android/asset_manager_jni.h>
#include "../utils/FileUtils.h"
#include "../modelAssimp/AssimpLoader.h"
#include <vector>

extern "C"
JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_AssimpNativeInitTask_androidEnvInit(JNIEnv *env, jobject obj,
                                                                     jobject assetManager,
                                                                     jstring internalPath) {
    apkAssetManager = AAssetManager_fromJava(env, assetManager);
    androidStoragePath = jStringToString(env, internalPath);
}