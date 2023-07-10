//
// Created by daweibayu on 2023/7/10.
//

#ifndef ANDROIDASSIMP_SHADERUTILS_H
#define ANDROIDASSIMP_SHADERUTILS_H
#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../modelAssimp/AssimpLoader.h"

GLuint loadShader(const std::string& shaderAssetFile, const GLenum shaderType);
GLuint LoadShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
GLuint GetAttributeLocation(GLuint programID, const std::string& variableName);
GLint GetUniformLocation(GLuint programID, const std::string& uniformName);

#endif //ANDROIDASSIMP_SHADERUTILS_H
