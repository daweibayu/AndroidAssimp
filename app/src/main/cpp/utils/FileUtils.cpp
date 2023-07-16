//
// Created by daweibayu on 2023/7/3.
//
#include "FileUtils.h"
#include <android/asset_manager.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include "StringUtils.h"

using namespace std;

AAssetManager *apkAssetManager;
std::string androidStoragePath;

const string FILE_SEPARATOR   = "/";

/**
 * 文件（文件夹）是否存在
 */
bool fileExists(const std::string& path) {
    return ( access( path.c_str(), F_OK ) != -1 );
}

/**
 * 递归创建文件夹
 */
bool createDir(const std::string& path) {
    if (fileExists(path)) {
        return true;
    }

    vector<string> nameVector;
    strSplit(path, nameVector, FILE_SEPARATOR);

    string dirPath = FILE_SEPARATOR;
    for (const auto& name : nameVector) {
        dirPath = dirPath.append(name).append(FILE_SEPARATOR);
        if (!fileExists(dirPath)) {
            if (mkdir(dirPath.c_str(), S_IRWXU) < 0) {
                return false;
            }
        }
    }
    return fileExists(path);
}

/**
 * 从完整路径中解析出最后一级文件（夹）的名字
 */
string parseFileName(const string& filePath) {
    string::size_type lastIndex = filePath.find_last_of(FILE_SEPARATOR);
    if (string::npos == lastIndex) {
        return filePath;
    }
    return filePath.substr(lastIndex + 1, std::string::npos);
}

/**
 * 从完整路径中解析出上一级路径
 */
string parseFileDir(const string& filePath) {
    string::size_type lastIndex = filePath.find_last_of(FILE_SEPARATOR);
    if (string::npos == lastIndex) {
        return "";
    }
    return filePath.substr(0, lastIndex + 1);
}

/**
 * 读取文件到字符串中
 */
bool readFileToStrByLine(const string& filePath, string& fileContent) {
    std::ifstream fileStream(filePath.c_str(), std::ios::in);
    if (fileStream.is_open()) {
        std::string line;
        while (getline(fileStream, line)) {
            fileContent += "\n" + line;
        }
        fileStream.close();
        return true;
    } else {
        return false;
    }
}

/**
 * 将 Android asset 文件拷贝到 app 内部的缓存文件夹中，方便读取
 */
bool copyAssetFileToStorage(const string& assetFilePath, const string& storagePath, string& targetFilePath) {
    targetFilePath = storagePath + "/" + assetFilePath;

    if (fileExists(targetFilePath)) {
        return true;
    }

    if(!createDir(parseFileDir(targetFilePath))) {
        return false;
    }

    if (nullptr == apkAssetManager) {
        return false;
    }

    AAsset* asset = AAssetManager_open(apkAssetManager, assetFilePath.c_str(), AASSET_MODE_STREAMING);
    if (nullptr == asset) {
        return false;
    }

    char buf[BUFSIZ];
    int nb_read = 0;
    FILE* out = fopen(targetFilePath.c_str(), "w");
    while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0) {
        fwrite(buf, nb_read, 1, out);
    }
    fclose(out);
    AAsset_close(asset);

    return true;
}

/**
 * 读取 asset 文件到 string 中
 */
std::string readAssetFile(const std::string& assetFileName) {
    std::string filePath, fileContent;
    copyAssetFileToStorage(assetFileName, androidStoragePath, filePath);
    readFileToStrByLine(filePath, fileContent);
    return fileContent;
}

