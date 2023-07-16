//
// Created by daweibayu on 2023/7/4.
//

#ifndef ANDROIDASSIMP_STRINGUTILS_H
#define ANDROIDASSIMP_STRINGUTILS_H
#include <string>
#include <android/asset_manager.h>
#include <unistd.h>
#include <jni.h>


std::string jStringToString(JNIEnv *env, jstring& jStr);

void strSplit(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = " ");

#endif //ANDROIDASSIMP_STRINGUTILS_H
