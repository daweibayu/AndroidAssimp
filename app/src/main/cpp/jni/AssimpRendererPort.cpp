
#include <jni.h>
#include "../modelAssimp/AssimpLoader.h"
#include "../modelAssimp/ModelInteraction.h"
#include <vector>


//
// Created by daweibayu on 2023/7/3.
//

ModelInteraction* interaction = NULL;

extern "C" {

JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_AssimpRenderer_surfaceCreatedNative(JNIEnv *env, jobject obj) {
    interaction = new ModelInteraction();
    interaction->init();
}

JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_AssimpRenderer_DrawFrameNative(JNIEnv *env, jobject obj) {
    interaction->render();
}

JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_AssimpRenderer_SurfaceChangedNative(JNIEnv *env, jobject obj,
                                                                     jint width, jint height) {
    interaction->setViewPort(width, height);
}

JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_AssimpRenderer_releaseNative(JNIEnv *env, jobject thiz) {
    delete interaction;
    interaction = NULL;
}

JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_GestureTouchListener_onMoveNative(JNIEnv *env, jobject obj,
                                                                   jfloat distanceX,
                                                                   jfloat distanceY) {
    interaction->onMove(distanceX, distanceY);
}

JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_GestureTouchListener_onScaleNative(JNIEnv *env, jobject obj,
                                                                    jfloat scale_factor) {
    interaction->onScale(scale_factor);
}

JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_GestureTouchListener_onScrollNative(JNIEnv *env, jobject obj,
                                                                     jfloat distanceX,
                                                                     jfloat distanceY,
                                                                     jfloat positionX,
                                                                     jfloat positionY) {
    interaction->onScroll(distanceX, distanceY, positionX, positionY);
}

JNIEXPORT void JNICALL
Java_com_daweibayu_androidassimp_GestureTouchListener_onDoubleTabNative(JNIEnv *env, jobject obj) {
    interaction->onDoubleTab();
}
}