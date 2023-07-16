//
// Created by daweibayu on 2023/7/10.
//

#include "ShaderUtils.h"
#include "../utils/FileUtils.h"
#include "LogHelper.h"

void printShaderLog(GLuint shaderId);
bool checkShaderStatus(GLuint shaderId, GLenum statusType, bool printLog);

void printProgramLog(GLuint programId);
bool checkProgramStatus(GLuint programId, GLenum statusType, bool printLog);

inline std::string readShaderCode(const std::string& shaderFileName) {
    return readAssetFile(shaderFileName);
}

bool compileShader(GLuint& shaderID, const GLenum shaderType, const std::string& shaderCode) {

    // 创建着色器
    shaderID = glCreateShader(shaderType);
    char const * sourcePointer = shaderCode.c_str();

    // 绑定 shader 到对应的 shaderID
    glShaderSource(shaderID, 1, &sourcePointer, nullptr);

    // 编译 shader
    glCompileShader(shaderID);

    // 检查 shader 编译状态，如果错误的话，打印日志
    if (!checkShaderStatus(shaderID, GL_COMPILE_STATUS, true)) {
        // TODO 善后处理
        return false;
    }
    return true;
}


bool linkProgram(GLuint programID, GLuint vertexShaderID, GLuint fragmentShaderID) {

    // 关联 shader 到 program
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    // 链接 program
    glLinkProgram(programID);

    // 链接后就不再需要直接对 shader 的操作了，可以删除了
    if (vertexShaderID) {
        glDeleteShader(vertexShaderID);
    }
    if (fragmentShaderID) {
        glDeleteShader(fragmentShaderID);
    }

    // 检查 shader 编译状态，如果错误的话，打印日志
    if (!checkProgramStatus(programID, GL_LINK_STATUS, true)) {
        if (programID) {
            glDeleteProgram(programID);
        }
        return false;
    }
    return true;
}


GLuint loadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
    GLuint programID = glCreateProgram();
    GLuint vertexShaderID = loadShader(vertexShaderFilename, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = loadShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);
    if (!linkProgram(programID, vertexShaderID, fragmentShaderID)) {
        return 0;
    }
    return programID;
}

GLuint loadShader(const std::string& shaderAssetFile, const GLenum shaderType) {
    GLuint shaderId;
    std::string shaderCode = readShaderCode(shaderAssetFile);
    compileShader(shaderId, shaderType, shaderCode);
    return shaderId;
}

GLuint getAttributeLocation(GLuint programID, const std::string& variableName) {
    GLint loc = glGetAttribLocation(programID, variableName.c_str());
    if (loc == -1) {
        return (0);
    } else {
        return ((GLuint) loc);
    }
}

GLint getUniformLocation(GLuint programID, const std::string& uniformName) {
    GLint loc = glGetUniformLocation(programID, uniformName.c_str());
    if (loc == -1) {
        return (0);
    } else {
        return loc;
    }
}

/**
 * 检查 shader 对应的状态
 */
bool checkShaderStatus(GLuint shaderId, GLenum statusType, bool printLog) {
    // 获取 shader 编译状态
    GLint result = GL_FALSE;
    glGetShaderiv(shaderId, statusType, &result);
    if (result == GL_TRUE) {
        return true;
    }

    if (printLog) {
        printShaderLog(shaderId);
    }
    return false;
}

/**
 * 打印 shader 相关日志
 */
void printShaderLog(GLuint shaderId) {
    int logLength = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        auto* logInfo = new GLchar[logLength + 1];
        glGetShaderInfoLog(shaderId, logLength, nullptr, logInfo);
        LOG_E("shader error:%s", logInfo);
        delete[] logInfo;
    }
}

/**
 * 检查 program 指定的 @param statusType 的状态，如果是错误，则可以根据 @param printLog 值来输入日志
 */
bool checkProgramStatus(GLuint programId, GLenum statusType, bool printLog) {
    GLint result = GL_FALSE;
    glGetProgramiv(programId, statusType, &result);
    if (result == GL_TRUE) {
        return true;
    }

    if (printLog) {
        printProgramLog(programId);
    }
    return false;
}

/**
 * 打印 program 相关日志
 */
void printProgramLog(GLuint programId) {
    int logLength = 0;

    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        auto* logInfo = new GLchar[logLength + 1];
        glGetProgramInfoLog(programId, logLength, nullptr, logInfo);
        LOG_E("program error:%s", logInfo);
        delete[] logInfo;
    }
}