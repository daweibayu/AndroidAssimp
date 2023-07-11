//
// Created by daweibayu on 2023/7/10.
//

#include "ModelInteraction.h"
#include "assimp/Importer.hpp"
#include <opencv2/opencv.hpp>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <string>

/**
 * Class constructor
 */
ModelInteraction::~ModelInteraction() {
    if (myGLCamera) {
        delete myGLCamera;
    }
    if (assimpLoader) {
        delete assimpLoader;
    }
    if (shaderLoader) {
        delete shaderLoader;
    }
}

void ModelInteraction::init() {
    initShaderLoader();
    initCamera();
    initAssimpLoader();
}

void ModelInteraction::initGl() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void ModelInteraction::initCamera() {
    myGLCamera = new MyGLCamera();
    float pos[]={0.,0.,0.,0.2,0.5,0.};
    std::copy(&pos[0], &pos[5], std::back_inserter(modelDefaultPosition));
    myGLCamera->SetModelPosition(modelDefaultPosition);
}

void ModelInteraction::initShaderLoader() {
    shaderLoader = new ShaderLoader();
}

void ModelInteraction::initAssimpLoader() {
    assimpLoader = new AssimpLoader();
    std::vector<std::string> imageList;
    imageList.emplace_back("models/Globe.jpg");
    assimpLoader->loadObj("models/Globe.obj", "models/Globe.mtl", imageList);
}

/**
 * Render to the display
 */
void ModelInteraction::render() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 mvpMat = myGLCamera->GetMVP();
    shaderLoader->renderObj(&mvpMat, assimpLoader->getModelMeshes());

//    CheckGLError("ModelAssimp::Render");

}

/**
 * set the viewport, function is also called when user changes device orientation
 */
void ModelInteraction::setViewPort(int width, int height) {
    screenHeight = (float)height;
    screenWidth = (float)width;
    glViewport(0, 0, width, height);
//    CheckGLError("Cube::SetViewport");

    myGLCamera->SetAspectRatio(screenWidth/ screenHeight);
}

//void initGl() {
//
//
//
////    MyLOGI("OpenGL %s, GLSL %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
//
//    // check if the device supports GLES 3 or GLES 2
//    const char* versionStr = (const char*)glGetString(GL_VERSION);
////    if (strstr(versionStr, "OpenGL ES 3.")) {
////        MyLOGD("Device supports GLES 3");
////    } else {
////        MyLOGD("Device supports GLES 2");
////    }
////
////    CheckGLError("MyGLInits");
//}

/**
 * Checks for OpenGL errors.
 */
//void CheckGLError(std::string funcName){
//
//    GLenum err = glGetError();
//    if (err == GL_NO_ERROR) {
//        return;
//    } else {
//        MyLOGF("[FAIL GL] %s", funcName.c_str());
//    }
//
//    switch(err) {
//
//        case GL_INVALID_ENUM:
//            MyLOGE("GL_INVALID_ENUM: GLenum argument out of range");
//            break;
//
//        case GL_INVALID_VALUE:
//            MyLOGE("GL_INVALID_VALUE: numeric argument out of range");
//            break;
//
//        case GL_INVALID_OPERATION:
//            MyLOGE("GL_INVALID_OPERATION: operation illegal in current state");
//            break;
//
//        case GL_INVALID_FRAMEBUFFER_OPERATION:
//            MyLOGE("GL_INVALID_FRAMEBUFFER_OPERATION: framebuffer object is not complete");
//            break;
//
//        case GL_OUT_OF_MEMORY:
//            MyLOGE( "GL_OUT_OF_MEMORY: not enough memory left to execute command");
//            break;
//
//        default:
//            MyLOGE("unlisted error");
//            break;
//    }
//}


/**
 * reset model's position in double-tap
 */
void ModelInteraction::onDoubleTab() {
    myGLCamera->SetModelPosition(modelDefaultPosition);
}

/**
 * rotate the model if user scrolls with one finger
 */
void ModelInteraction::onScroll(float distanceX, float distanceY, float positionX, float positionY) {
    float dX = (float) distanceX / screenWidth;
    float dY = -(float) distanceY / screenHeight;
    float posX = 2*positionX/ screenWidth - 1.;
    float posY = -2*positionY / screenHeight + 1.;
    posX = fmax(-1., fmin(1., posX));
    posY = fmax(-1., fmin(1., posY));
    myGLCamera->RotateModel(dX, dY, posX, posY);
}

/**
 * pinch-zoom: move the model closer or farther away
 */
void ModelInteraction::onScale(float scaleFactor) {
    myGLCamera->ScaleModel(scaleFactor);
}

/**
 * two-finger drag: displace the model by changing its x-y coordinates
 */
void ModelInteraction::onMove(float distanceX, float distanceY) {
    myGLCamera->TranslateModel(distanceX, distanceY);
}