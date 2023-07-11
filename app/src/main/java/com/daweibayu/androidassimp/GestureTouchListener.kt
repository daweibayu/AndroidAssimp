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
            // 双指点击，放大固定倍数
            onScaleNative(2f)
            return true
        }

        override fun onScroll(e1: MotionEvent, e2: MotionEvent, distanceX: Float, distanceY: Float): Boolean {
            // 单指滑动，旋转物体
            onScrollNative(distanceX, distanceY, e2.x, e2.y);
            return true
        }

        override fun onFling(e1: MotionEvent, e2: MotionEvent, velocityX: Float, velocityY: Float): Boolean {
            return super.onFling(e1, e2, velocityX, velocityY)
        }
    })

    private val scaleGestureDetector = ScaleGestureDetector(context, object : ScaleGestureDetector.SimpleOnScaleGestureListener() {
        override fun onScale(detector: ScaleGestureDetector): Boolean {
            // 双指放缩
            onScaleNative(detector.scaleFactor)
            return true
        }
    })


    override fun onTouch(v: View?, event: MotionEvent?): Boolean {
        event?.let {
            scaleGestureDetector.onTouchEvent(event)
            gestureDetector.onTouchEvent(event)
        }
        return true
    }
}

