//
// Created by daweibayu on 2023/6/30.
//

#ifndef ANDROIDASSIMP_ASSIMPLOADER_H
#define ANDROIDASSIMP_ASSIMPLOADER_H

#include <string>
#include <stdio.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <vector>
#include <map>
#include "glm-0.9.9.8/glm.hpp"
#include "glm-0.9.9.8/gtc/matrix_transform.hpp"
#include "glm-0.9.9.8/gtc/quaternion.hpp"
#include "glm-0.9.9.8/gtx/quaternion.hpp"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

struct MeshInfo {
    GLuint  textureIndex;
    int     numberOfFaces;
    GLuint  faceBuffer;
    GLuint  vertexBuffer;
    GLuint  textureCoordBuffer;
};

class AssimpLoader {

public:
    void loadFiles(const std::string& objPath, const std::string& mtlPath, std::vector<std::string>& imagesPath);
    std::vector<struct MeshInfo>& getModelMeshes();

private:
    void loadObj(const std::string& objPath);
    void generateGLBuffers();
    bool loadTexturesToGL(const std::string& modelFilename);

    Assimp::Importer* importer;
    const aiScene* scene;                           // assimp's output data structure

    std::vector<struct MeshInfo> modelMeshes;       // contains one struct for every mesh in model
    std::map<std::string, GLuint> textureNameMap;   // (texture filename, texture name in GL)


};


#endif //ANDROIDASSIMP_ASSIMPLOADER_H
