//
// Created by daweibayu on 2023/7/4.
//

#ifndef ANDROIDASSIMP_SHADERLOADER_H
#define ANDROIDASSIMP_SHADERLOADER_H

#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../modelAssimp/AssimpLoader.h"


GLuint loadShader(const std::string& shaderAssetFile, const GLenum shaderType);
GLuint LoadShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
GLuint GetAttributeLocation(GLuint programID, const std::string& variableName);
GLint GetUniformLocation(GLuint programID, const std::string& uniformName);

class ShaderLoader {

public:
    ShaderLoader();
    void renderObj(glm::mat4 *mvpMat, const std::vector<struct MeshInfo>& modelMeshes);

private:
    GLuint  vertexAttribute, vertexUVAttribute;     // attributes for shader variables
    GLuint  shaderProgramID;
    GLint   mvpLocation, textureSamplerLocation;    // location of MVP in the shader

    void initShader();
    void readerMesh(const MeshInfo& mashInfo);
};

#endif //ANDROIDASSIMP_SHADERLOADER_H
