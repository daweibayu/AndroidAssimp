package com.daweibayu.androidassimp

import android.opengl.GLSurfaceView.Renderer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class AssimpRenderer : Renderer {

    private external fun DrawFrameNative()

    private external fun surfaceCreatedNative()

    private external fun SurfaceChangedNative(width: Int, height: Int)

    external fun releaseNative()

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        surfaceCreatedNative()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        SurfaceChangedNative(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        DrawFrameNative()
    }
}