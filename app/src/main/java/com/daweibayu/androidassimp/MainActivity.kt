package com.daweibayu.androidassimp

import android.opengl.GLSurfaceView
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.daweibayu.androidassimp.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private val assimpRenderer = AssimpRenderer()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        initSurfaceView()
    }

    private fun initSurfaceView() {
        binding.surfaceView.setEGLContextClientVersion(2)
        binding.surfaceView.setRenderer(assimpRenderer)
        binding.surfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
        binding.surfaceView.setOnTouchListener(GestureTouchListener(applicationContext))

    }

    override fun onDestroy() {
        assimpRenderer.releaseNative()
        super.onDestroy()
    }
}