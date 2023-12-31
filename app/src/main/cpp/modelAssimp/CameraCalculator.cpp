//
// Created by daweibayu on 2023/7/17.
//

#include "CameraCalculator.h"
#include <cmath>

CameraCalculator::CameraCalculator(float FOV, float zPosition, float nearPlaneDistance, float farPlaneDistance) {

    // camera position is fixed
    glm::vec3 cameraPosition = glm::vec3(0, 0, zPosition);
    viewMat = glm::lookAt(cameraPosition,        // Camera location in World Space
                          glm::vec3(0, 0, -1),   // direction in which camera it is pointed
                          glm::vec3(0, 1, 0));   // camera is pointing up

    this->nearPlaneDistance = nearPlaneDistance;
    this->farPlaneDistance = farPlaneDistance;
    this->FOV       = FOV;

    // 6DOF describing model's position
    deltaX = deltaY = deltaZ = 0;                  // translations
    modelQuaternion = glm::quat(glm::vec3(0,0,0)); // rotation

    modelMat        = glm::mat4(1.0f);
    translateMat    = glm::mat4(1.0f);
    rotateMat       = glm::mat4(1.0f);
    mvpMat = glm::mat4(1.0f); // projection is not known -> initialize MVP to identity
}

/**
 * Use the display's aspect ratio to compute projection matrix
 */
void CameraCalculator::setAspectRatio(float aspect) {

    glm::mat4 projectionMat;
    projectionMat = glm::perspective(FOV * float(M_PI / 180), // camera's field-of-view
                                     aspect,                  // camera's aspect ratio
                                     nearPlaneDistance,       // distance to the near plane
                                     farPlaneDistance);       // distance to the far plane
    projectionViewMat = projectionMat * viewMat;
    computeMVPMatrix();

}

/**
 * Model's position has 6 degrees-of-freedom: 3 for x-y-z locations and
 * 3 for alpha-beta-gamma Euler angles
 * Convert euler angles to quaternion and update MVP
 */
void CameraCalculator::setModelPosition(std::vector<float> modelPosition) {

    deltaX = modelPosition[0];
    deltaY = modelPosition[1];
    deltaZ = modelPosition[2];
    float pitchAngle = modelPosition[3];
    float yawAngle   = modelPosition[4];
    float rollAngle  = modelPosition[5];

    modelQuaternion = glm::quat(glm::vec3(pitchAngle, yawAngle, rollAngle));
    rotateMat = glm::toMat4(modelQuaternion);
    computeMVPMatrix();
}


/**
 * Compute the translation matrix from x-y-z position and rotation matrix from
 * quaternion describing the rotation
 * MVP = Projection * View * (Translation * Rotation)
 */
void CameraCalculator::computeMVPMatrix() {

    translateMat = glm::mat4(1, 0, 0, 0,                  // col0
                             0, 1, 0, 0,	              // col1
                             0, 0, 1, 0,	              // col2
                             deltaX, deltaY, deltaZ, 1);  // col3

    modelMat    = translateMat * rotateMat;
    mvpMat = projectionViewMat * modelMat;
}

/**
 * Simulate change in scale by pushing or pulling the model along Z axis
 */
void CameraCalculator::scaleModel(float scaleFactor) {
    deltaZ += SCALE_TO_Z_TRANSLATION * (scaleFactor - 1);
    computeMVPMatrix();
}

/**
 * Finger drag movements are converted to rotation of model by deriving a
 * quaternion from the drag movement
 */
void CameraCalculator::rotateModel(float distanceX, float distanceY,
                             float endPositionX, float endPositionY) {

    // algo in brief---
    // assume that a sphere with its center at (0,0), i.e., center of screen, and
    // radius 1 is placed on the device.
    // drag movement on the surface is translated to a drag on the imaginary sphere's surface
    // since we know (x,y) coordinates of the drag, we only need to determine z-coordinate
    // corresponding to the height of sphere's surface corresponding to the drag position.
    // begin and end of drag define two points on sphere and we create two vectors joining those
    // points with the origin (0,0).
    // lastly we create a quaternion responsible for rotation between the two vectors.

    // compute ending vector (using endPositionX, endPositionY)
    float dist = sqrt(fmin(1, endPositionX * endPositionX + endPositionY * endPositionY));
    float positionZ = sqrt(1 - dist*dist);
    glm::vec3 endVec = glm::vec3(endPositionX, endPositionY, positionZ);
    endVec = glm::normalize(endVec);

    // compute starting vector by adding (distanceX, distanceY) to ending positions
    endPositionX += distanceX;
    endPositionY += distanceY;
    dist = sqrt(fmin(1, endPositionX * endPositionX + endPositionY * endPositionY));
    positionZ = sqrt(1 - dist*dist);
    glm::vec3 beginVec = glm::vec3(endPositionX, endPositionY, positionZ);
    beginVec = glm::normalize(beginVec);

    // compute cross product of vectors to find axis of rotation
    glm::vec3 rotationAxis = glm::cross(beginVec, endVec);
    rotationAxis = glm::normalize(rotationAxis);

    // compute angle between vectors using the dot product
    float dotProduct = fmax(fmin(glm::dot(beginVec, endVec), 1.), -1.);
    float rotationAngle = TRANSLATION_TO_ANGLE*acos(dotProduct);

    // compute quat using above
    modelQuaternion = glm::angleAxis(rotationAngle, rotationAxis);
    rotateMat = glm::toMat4(modelQuaternion)*rotateMat;

    computeMVPMatrix();
}

/**
 * displace model by changing x-y coordinates
 */
void CameraCalculator::translateModel(float distanceX, float distanceY) {
    deltaX += XY_TRANSLATION_FACTOR * distanceX;
    deltaY += XY_TRANSLATION_FACTOR * distanceY;
    computeMVPMatrix();
}