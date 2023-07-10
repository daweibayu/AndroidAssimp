package com.daweibayu.androidassimp

import android.app.Application
import android.content.res.AssetManager

class AssimpNativeInitTask {

    fun loadNativeLib() {
        System.loadLibrary("androidassimp")
    }

    fun initNativeEnv(context: Application) {
        androidEnvInit(context.assets, context.filesDir.absolutePath)
    }

    private external fun androidEnvInit(assetManager: AssetManager, internalPath: String)
}