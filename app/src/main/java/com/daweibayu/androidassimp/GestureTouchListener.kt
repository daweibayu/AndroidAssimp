package com.daweibayu.androidassimp

import android.content.Context
import android.view.GestureDetector.SimpleOnGestureListener
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.View
import androidx.core.view.GestureDetectorCompat

class GestureTouchListener(val context: Context): View.OnTouchListener {

    private external fun  onDoubleTabNative()
    private external fun  onScrollNative(distanceX: Float, distanceY: Float, positionX: Float, positionY: Float)
    private external fun  onScaleNative(scaleFactor: Float)
    private external fun  onMoveNative(distanceX: Float, distanceY: Float)

    private val gestureDetector = GestureDetectorCompat(context, object : SimpleOnGestureListener() {
        override fun onDoubleTap(e: MotionEvent): Boolean {
            onDoubleTabNative()
            return true
        }

        override fun onScroll(e1: MotionEvent, e2: MotionEvent, distanceX: Float, distanceY: Float): Boolean {
            onScrollNative(distanceX, distanceY, e2.x, e2.y);
            return true
        }
    })

    private val scaleGestureDetector = ScaleGestureDetector(context, object : ScaleGestureDetector.SimpleOnScaleGestureListener() {
        override fun onScale(detector: ScaleGestureDetector): Boolean {
            onScaleNative(detector.scaleFactor)
            return true
        }
    })


    override fun onTouch(v: View?, event: MotionEvent?): Boolean {
        event?.let {
            if (gestureDetector.onTouchEvent(event) || scaleGestureDetector.onTouchEvent(event)) {
                return true
            }
        }
        return false
    }
}

