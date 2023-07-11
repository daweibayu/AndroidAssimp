
#include <jni.h>
#include "../modelAssimp/AssimpLoader.h"
#include "../modelAssimp/ModelInteraction.h"
#include <vector>


//
// Created by daweibayu on 2023/7/3.
//

ModelInteraction* interaction = nullptr;

extern "C" {

    JNIEXPORT void JNICALL
    Java_com_daweibayu_androidassimp_AssimpRenderer_surfaceCreatedNative(JNIEnv *env, jobject obj) {
        interaction = new ModelInteraction();
        interaction->init();
    }

    JNIEXPORT void JNICALL
    Java_com_daweibayu_androidassimp_AssimpRenderer_DrawFrameNative(JNIEnv *env, jobject obj) {
        if (nullptr != interaction) {
            interaction->render();
        }

    }

    JNIEXPORT void JNICALL
    Java_com_daweibayu_androidassimp_AssimpRenderer_SurfaceChangedNative(JNIEnv *env, jobject obj,
                                                                         jint width, jint height) {
        if (nullptr != interaction) {
            interaction->setViewPort(width, height);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_daweibayu_androidassimp_AssimpRenderer_releaseNative(JNIEnv *env, jobject thiz) {
        delete interaction;
        interaction = nullptr;
    }

    JNIEXPORT void JNICALL
    Java_com_daweibayu_androidassimp_GestureTouchListener_onMoveNative(JNIEnv *env, jobject obj,
                                                                       jfloat distanceX,
                                                                       jfloat distanceY) {
        if (nullptr != interaction) {
            interaction->onMove(distanceX, distanceY);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_daweibayu_androidassimp_GestureTouchListener_onScaleNative(JNIEnv *env, jobject obj,
                                                                        jfloat scale_factor) {
        if (nullptr != interaction) {
            interaction->onScale(scale_factor);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_daweibayu_androidassimp_GestureTouchListener_onScrollNative(JNIEnv *env, jobject obj,
                                                                         jfloat distanceX,
                                                                         jfloat distanceY,
                                                                         jfloat positionX,
                                                                         jfloat positionY) {
        if (nullptr != interaction) {
            interaction->onScroll(distanceX, distanceY, positionX, positionY);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_daweibayu_androidassimp_GestureTouchListener_onDoubleTabNative(JNIEnv *env, jobject obj) {
        if (nullptr != interaction) {
            interaction->onDoubleTab();
        }
    }
}