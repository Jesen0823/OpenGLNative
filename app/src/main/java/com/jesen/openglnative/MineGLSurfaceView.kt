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
    private var mPreviousY = 0f
    private var mPreviousX = 0f
    private var mXAngle = 0f
    private var mYAngle = 0f

    init {
        this.setEGLContextClientVersion(2)
        mGLRender = MineGLRender()
        setEGLConfigChooser(8,8,8,8,16,8);
        setRenderer(mGLRender)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    fun getEGLRender(): MineGLRender = mGLRender

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val x = event.x
        val y = event.y
        when (event.action) {
            MotionEvent.ACTION_MOVE -> {
                val dx = x - mPreviousX
                val dy = x - mPreviousY
                mXAngle += dx * TOUCH_SCALE_FACTOR
                mYAngle += dy * TOUCH_SCALE_FACTOR
            }

            else -> {}
        }
        mPreviousX = x
        mPreviousY = y
        when (mGLRender.getSampleType()) {
            Constants.SAMPLE_TYPE_FBO_LEG,
            Constants.SAMPLE_TYPE_COORD_SYSTEM,
            Constants.SAMPLE_TYPE_BASIC_LIGHTING,
            Constants.SAMPLE_TYPE_TRANS_FEEDBACK,
            Constants.SAMPLE_TYPE_MULTI_LIGHTS,
            Constants.SAMPLE_TYPE_DEPTH_TESTING,
            Constants.SAMPLE_TYPE_INSTANCING -> {
                mGLRender.setParamsInt(
                    Constants.PARAM_TYPE_ROTATE,
                    mXAngle.toInt(),
                    mYAngle.toInt()
                )
                requestRender()
            }
        }
        return true
    }


}