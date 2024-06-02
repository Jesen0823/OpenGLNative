package com.jesen.openglnative

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.ScaleGestureDetector

class MineGLSurfaceView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null) :
    GLSurfaceView(context, attrs),ScaleGestureDetector.OnScaleGestureListener {
    private val TAG = "MineGLSurfaceView"
    private val TOUCH_SCALE_FACTOR = 180.0f / 320
    private var mGLRender: MineGLRender
    private var mPreviousY = 0f
    private var mPreviousX = 0f
    private var mXAngle = 0f
    private var mYAngle = 0f
    private var mScaleGestureDetector: ScaleGestureDetector
    private var mPreScale = 1.0f
    private var mCurScale = 1.0f
    private var mLastMultiTouchTime: Long = 0L

    init {
        this.setEGLContextClientVersion(2)
        mGLRender = MineGLRender()
        setEGLConfigChooser(8,8,8,8,16,8);
        setRenderer(mGLRender)
        renderMode = RENDERMODE_WHEN_DIRTY
        mScaleGestureDetector = ScaleGestureDetector(context,this)
    }

    fun getEGLRender(): MineGLRender = mGLRender

    override fun onTouchEvent(event: MotionEvent): Boolean {
        if(event.pointerCount == 1){
            val currentTimeMillis = System.currentTimeMillis()
            if(currentTimeMillis - mLastMultiTouchTime >200){
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
                    Constants.SAMPLE_TYPE_INSTANCING,
                    Constants.SAMPLE_TYPE_PARTICLES,
                    Constants.SAMPLE_TYPE_SKYBOX-> {
                        mGLRender.updateTransformMatrix(mXAngle,mYAngle,mCurScale,mCurScale)
                        requestRender()
                    }else->{}
                }
            }
        }else{
            mScaleGestureDetector.onTouchEvent(event)
        }
        return true
    }

    override fun onScale(detector: ScaleGestureDetector): Boolean {
        when (mGLRender.getSampleType()) {
            Constants.SAMPLE_TYPE_COORD_SYSTEM,
            Constants.SAMPLE_TYPE_BASIC_LIGHTING,
            Constants.SAMPLE_TYPE_INSTANCING -> {
                val preSpan = detector.previousSpan
                val curSpan = detector.currentSpan
                mCurScale = if (curSpan < preSpan) {
                    mPreScale - (preSpan - curSpan) / 500
                } else {
                    mPreScale + (curSpan - preSpan) / 500
                }
                mCurScale = 0.1f.coerceAtLeast(mCurScale.coerceAtMost(5.0f))
                mGLRender.updateTransformMatrix(mXAngle, mYAngle, mCurScale, mCurScale)
                requestRender()
            }
            else -> {}
        }
        return false
    }

    override fun onScaleBegin(detector: ScaleGestureDetector): Boolean {
        return true
    }

    override fun onScaleEnd(detector: ScaleGestureDetector) {
        mPreScale = mCurScale
        mLastMultiTouchTime = System.currentTimeMillis()
    }
}