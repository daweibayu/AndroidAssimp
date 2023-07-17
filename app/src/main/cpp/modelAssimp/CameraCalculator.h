//
// Created by daweibayu on 2023/7/17.
//

#ifndef ANDROIDASSIMP_CAMERACALCULATOR_H
#define ANDROIDASSIMP_CAMERACALCULATOR_H

#include <vector>
#include <stdio.h>
#include <string>

#include <../include/glm-0.9.9.8/glm.hpp>
#include <../include/glm-0.9.9.8/gtc/matrix_transform.hpp>
#include <../include/glm-0.9.9.8/gtc/quaternion.hpp>
#include <../include/glm-0.9.9.8/gtx/quaternion.hpp>



#define SCALE_TO_Z_TRANSLATION  20
#define TRANSLATION_TO_ANGLE    5
#define XY_TRANSLATION_FACTOR   10

class CameraCalculator {
public:
    CameraCalculator(
            float FOV = 45,          // in degrees
            float zPosition = 10,    // camera's location on Z axis
            float nearPlaneDistance = 1.0f, // as large as possible
            float farPlaneDistance = 2000.0f // as small as possible
    );
    void        setModelPosition(std::vector<float> modelPosition);
    void        setAspectRatio(float aspect);
    glm::mat4   getMVP(){ return mvpMat; }
    void        rotateModel(float distanceX, float distanceY, float endPositionX, float endPositionY);
    void        scaleModel(float scaleFactor);
    void        translateModel(float distanceX, float distanceY);

private:
    void        computeMVPMatrix();

    float       FOV;
    float       nearPlaneDistance, farPlaneDistance;

    glm::mat4   projectionViewMat;
    glm::mat4   rotateMat, translateMat;
    glm::mat4   modelMat;
    glm::mat4   viewMat;
    glm::mat4   mvpMat;     // ModelViewProjection: obtained by multiplying Projection, View, & Model

    // six degrees-of-freedom of the model contained in a quaternion and x-y-z coordinates
    glm::quat   modelQuaternion;
    float       deltaX, deltaY, deltaZ;
};


#endif //ANDROIDASSIMP_CAMERACALCULATOR_H
