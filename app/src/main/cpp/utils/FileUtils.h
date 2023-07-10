//
// Created by daweibayu on 2023/7/4.
//

#ifndef ANDROIDASSIMP_FILEUTILS_H
#define ANDROIDASSIMP_FILEUTILS_H
#include <string>
#include <android/asset_manager_jni.h>

extern AAssetManager *apkAssetManager;
extern std::string androidStoragePath;
bool fileExists(const std::string& path);
bool createDir(const std::string& path);
std::string parseFileName(const std::string& filePath);
std::string parseFileDir(const std::string& filePath);
bool copyAssetFileToStorage(const std::string& assetFileName, const std::string& storagePath, std::string& targetFilePath);
std::string readAssetFile(const std::string& assetFileName);
#endif //ANDROIDASSIMP_FILEUTILS_H
