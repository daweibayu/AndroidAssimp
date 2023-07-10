package com.daweibayu.androidassimp

import android.app.Application

class AssimpApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        val assimpTask = AssimpNativeInitTask()
        assimpTask.loadNativeLib()
        assimpTask.initNativeEnv(this)
    }
}