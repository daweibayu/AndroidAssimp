//
// Created by daweibayu on 2023/7/10.
//

#include "ShaderUtils.h"
#include "../utils/FileUtils.h"

inline std::string readShaderCode(const std::string& shaderFileName) {
    return readAssetFile(shaderFileName);
}

bool compileShader(GLuint& shaderID, const GLenum shaderType, const std::string& shaderCode) {
    shaderID = glCreateShader(shaderType);
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, nullptr);
    glCompileShader(shaderID);

    GLint result = GL_FALSE;
    int infoLogLength;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (result == 0) {
        std::vector<char> shaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &shaderErrorMessage[0]);
        return false;
    }
    return true;
}


bool linkProgram(GLuint programID, GLuint vertexShaderID, GLuint fragmentShaderID) {
    GLint result = GL_FALSE;
    int infoLogLength;

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

    // common deletes
    if (vertexShaderID) {
        glDeleteShader(vertexShaderID);
    }
    if (fragmentShaderID) {
        glDeleteShader(fragmentShaderID);
    }

    if (result == 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, nullptr,
                            &programErrorMessage[0]);
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