//
// Created by daweibayu on 2023/7/4.
//

#include <vector>
#include "ShaderLoader.h"
#include "../modelAssimp/AssimpLoader.h"
#include "../utils/ShaderUtils.h"


ShaderLoader::ShaderLoader() {
    initShader();
}

void ShaderLoader::initShader() {
    std::string vertexShader    = "shaders/shader.vs";
    std::string fragmentShader  = "shaders/shader.fs";
    shaderProgramID         = loadShaders(vertexShader, fragmentShader);
    vertexAttribute         = getAttributeLocation(shaderProgramID, "vertexPosition");
    vertexUVAttribute       = getAttributeLocation(shaderProgramID, "vertexUV");
    mvpLocation             = getUniformLocation(shaderProgramID, "mvpMat");
    textureSamplerLocation  = getUniformLocation(shaderProgramID, "textureSampler");
}

void ShaderLoader::renderMash(const MeshInfo& mashInfo) {
    // Texture
    if (mashInfo.textureIndex) {
        glBindTexture( GL_TEXTURE_2D, mashInfo.textureIndex);
    }

    // Faces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mashInfo.faceBuffer);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, mashInfo.vertexBuffer);
    glEnableVertexAttribArray(vertexAttribute);
    glVertexAttribPointer(vertexAttribute, 3, GL_FLOAT, 0, 0, 0);

    // Texture coords
    glBindBuffer(GL_ARRAY_BUFFER, mashInfo.textureCoordBuffer);
    glEnableVertexAttribArray(vertexUVAttribute);
    glVertexAttribPointer(vertexUVAttribute, 2, GL_FLOAT, 0, 0, 0);

    glDrawElements(GL_TRIANGLES, mashInfo.numberOfFaces * 3, GL_UNSIGNED_INT, 0);

    // unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ShaderLoader::renderObj(glm::mat4 *mvpMat, const std::vector<struct MeshInfo>& modelMeshes) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat *) mvpMat);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textureSamplerLocation, 0);

    for(auto meshInfo : modelMeshes) {
        renderMash(meshInfo);
    }
}