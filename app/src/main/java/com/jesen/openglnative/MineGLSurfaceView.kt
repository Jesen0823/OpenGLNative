package com.jesen.openglnative

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent

class MineGLSurfaceView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null) :
    GLSurfaceView(context, attrs) {
    private val TAG = "MineGLSurfaceView"
    private val TOUCH_SCALE_FACTOR = 180.0f / 320
    private var mGLRender: MineGLRender
    private var mNativeRender: MineNativeRender
    private var mPreviousY = 0f
    private var mPreviousX = 0f
    private var mXAngle = 0f
    private var mYAngle = 0f

    init {
        this.setEGLContextClientVersion(3)
        mNativeRender = MineNativeRender()
        mGLRender = MineGLRender(mNativeRender)
        setRenderer(mGLRender)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    fun getNativeRender(): MineNativeRender = mNativeRender

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val x = event.x
        val y = event.y
        when(event.action){
            MotionEvent.ACTION_MOVE->{
                val dx = x - mPreviousX
                val dy = x - mPreviousY
                mXAngle += dx * TOUCH_SCALE_FACTOR
                mYAngle += dy * TOUCH_SCALE_FACTOR
            }
            else->{}
        }
        mPreviousX = x
        mPreviousY =y
        mNativeRender.native_SetParamsInt(300,mXAngle.toInt(),mYAngle.toInt())
        requestRender()
        return true
    }
}