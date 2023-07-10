//
// Created by daweibayu on 2023/6/30.
//

#include "AssimpLoader.h"
#include <string>
#include <jni.h>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <android/log.h>
#include "../utils/FileUtils.h"
#include "../utils/LogHelper.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../shader/ShaderLoader.h"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/opencv.hpp"

using namespace std;

std::vector<struct MeshInfo>& AssimpLoader::getModelMeshes() {
    return modelMeshes;
}

void AssimpLoader::loadObj(const std::string& objPath, const std::string& mtlPath, std::vector<std::string>& imagesPath) {

    std::string objFullPath, mtlFullPath, imageFullPath;
    copyAssetFileToStorage(objPath, androidStoragePath, objFullPath);
    copyAssetFileToStorage(mtlPath, androidStoragePath, mtlFullPath);
    for (auto imagePath : imagesPath) {
        copyAssetFileToStorage(imagePath, androidStoragePath, imageFullPath);
    }

    importer = new Assimp::Importer();

    LOG_D("Importer ReadFile before *********");
    scene = importer->ReadFile(objFullPath,aiProcessPreset_TargetRealtime_Quality);
    if (nullptr == scene) {
        string errorMessage = importer->GetErrorString();
        LOG_D("Importer ReadFile failed %s *********", errorMessage.c_str());
    } else {
        LOG_D("Importer ReadFile succeed *********");
        std::string resultStr = std::to_string(1);
    }

    LoadTexturesToGL(parseFileDir(imageFullPath));
    GenerateGLBuffers();
    LOG_D("Importer ReadFile end %d *********", scene->mNumMaterials);
}

void parseMashInfoFromScene(const aiMesh* mesh, MeshInfo& mashInfo) {
    // create array with faces
    // convert from Assimp's format to array for GLES
    unsigned int *faceArray = new unsigned int[mesh->mNumFaces * 3];
    unsigned int faceIndex = 0;
    for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {

        // read a face from assimp's mesh and copy it into faceArray
        const aiFace *face = &mesh->mFaces[t];
        memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
        faceIndex += 3;

    }
    mashInfo.numberOfFaces = mesh->mNumFaces;

    // buffer for faces
    if (mashInfo.numberOfFaces) {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray,
                     GL_STATIC_DRAW);
        mashInfo.faceBuffer = buffer;

    }
    delete[] faceArray;

    // buffer for vertex positions
    if (mesh->HasPositions()) {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices,
                     GL_STATIC_DRAW);
        mashInfo.vertexBuffer = buffer;

    }

    // buffer for vertex texture coordinates
    // ***ASSUMPTION*** -- handle only one texture for each mesh
    if (mesh->HasTextureCoords(0)) {
        GLuint buffer;
        float * textureCoords = new float[2 * mesh->mNumVertices];
        for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
            textureCoords[k * 2] = mesh->mTextureCoords[0][k].x;
            textureCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;
        }
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(float) * 2 * mesh->mNumVertices, textureCoords,
                     GL_STATIC_DRAW);
        mashInfo.textureCoordBuffer = buffer;
        delete[] textureCoords;

    }

    // unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void parseTexture(aiMaterial *mtl, std::map<std::string, GLuint>& textureMap, MeshInfo& mashInfo) {
    // copy texture index (= texture name in GL) for the mesh from textureNameMap
    aiString texturePath;	//contains filename of texture
    if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath)) {
        unsigned int textureId = textureMap[texturePath.data];
        mashInfo.textureIndex = textureId;
    } else {
        mashInfo.textureIndex = 0;
    }
}

void AssimpLoader::GenerateGLBuffers() {
    struct MeshInfo newMeshInfo; // this struct is updated for each mesh in the model
    // For every mesh -- load face indices, vertex positions, vertex texture coords
    // also copy texture index for mesh into newMeshInfo.textureIndex
    for (unsigned int n = 0; n < scene->mNumMeshes; ++n) {
        const aiMesh *mesh = scene->mMeshes[n]; // read the n-th mesh
        parseMashInfoFromScene(mesh, newMeshInfo);
        aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
        parseTexture(mtl, textureNameMap, newMeshInfo);
        modelMeshes.push_back(newMeshInfo);
    }
}

bool AssimpLoader::LoadTexturesToGL(const std::string& imagePath) {

    // read names of textures associated with all materials
    textureNameMap.clear();

    for (unsigned int m = 0; m < scene->mNumMaterials; ++m) {

        int textureIndex = 0;
        aiString textureFilename;
        aiReturn isTexturePresent = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE,
                                                                     textureIndex,
                                                                     &textureFilename);
        while (isTexturePresent == AI_SUCCESS) {
            //fill map with textures, OpenGL image ids set to 0
            textureNameMap.insert(std::pair<std::string, GLuint>(textureFilename.data, 0));

            // more textures? more than one texture could be associated with a material
            textureIndex++;
            isTexturePresent = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE,
                                                                textureIndex, &textureFilename);
        }
    }

    int numTextures = (int) textureNameMap.size();

    // create and fill array with texture names in GL
    GLuint * textureGLNames = new GLuint[numTextures];
    glGenTextures(numTextures, textureGLNames);

    // iterate over the textures, read them using OpenCV, load into GL
    std::map<std::string, GLuint>::iterator textureIterator = textureNameMap.begin();
    int i = 0;
    for (; textureIterator != textureNameMap.end(); ++i, ++textureIterator) {

        std::string textureFilename = (*textureIterator).first;  // get filename
        std::string textureFullPath = imagePath + "/" + textureFilename;
        (*textureIterator).second = textureGLNames[i];	  // save texture id for filename in map

        // load the texture using OpenCV
        cv::Mat textureImage = cv::imread(textureFullPath);
        if (!textureImage.empty()) {

            // opencv reads textures in BGR format, change to RGB for GL
            cv::cvtColor(textureImage, textureImage, CV_BGR2RGB);
            // opencv reads image from top-left, while GL expects it from bottom-left
            // vertically flip the image
            cv::flip(textureImage, textureImage, 0);

            // bind the texture
            glBindTexture(GL_TEXTURE_2D, textureGLNames[i]);
            // specify linear filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // load the OpenCV Mat into GLES
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.cols,
                         textureImage.rows, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         textureImage.data);

        } else {
            //Cleanup and return
            delete[] textureGLNames;
            return false;

        }
    }

    //Cleanup and return
    delete[] textureGLNames;
    return true;
}
