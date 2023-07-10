//
// Created by daweibayu on 2023/7/3.
//
#include <string>
#include <android/asset_manager.h>
#include <unistd.h>
#include <jni.h>
#include <vector>

using namespace std;

string jStringToString(JNIEnv *env, jstring& jStr) {
    const char * cPathToInternalDir = env->GetStringUTFChars(jStr, NULL ) ;
    string resultStr = string(cPathToInternalDir);
    env->ReleaseStringUTFChars(jStr, cPathToInternalDir);
    return resultStr;
}

void strSplit(const string& s, vector<string>& tokens, const string& delimiters = " ") {
    tokens.clear();
    string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}