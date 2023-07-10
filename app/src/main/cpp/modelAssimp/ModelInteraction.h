//
// Created by daweibayu on 2023/7/10.
//

#ifndef ANDROIDASSIMP_MODELINTERACTION_H
#define ANDROIDASSIMP_MODELINTERACTION_H

#include "AssimpLoader.h"
#include "myGLCamera.h"
#include "AssimpLoader.h"
#include "../shader/ShaderLoader.h"
#include <vector>

class ModelInteraction {
public:
    ~ModelInteraction();

    void init();

    void    render();
    void    setViewPort(int width, int height);
    void    onDoubleTab();
    void    onScroll(float distanceX, float distanceY, float positionX, float positionY);
    void    onScale(float scaleFactor);
    void    onMove(float distanceX, float distanceY);
    int     getScreenWidth() const { return screenWidth; }
    int     getScreenHeight() const { return screenHeight; }

private:
    bool    initsDone;
    int     screenWidth, screenHeight;

    std::vector<float> modelDefaultPosition;

    MyGLCamera* myGLCamera;
    AssimpLoader* assimpLoader;
    ShaderLoader* shaderLoader;

    void initGl();
    void initCamera();
    void initAssimpLoader();
    void initShaderLoader();
};

#endif //ANDROIDASSIMP_MODELINTERACTION_H
