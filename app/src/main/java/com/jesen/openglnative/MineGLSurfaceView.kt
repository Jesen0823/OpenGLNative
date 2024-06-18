package com.jesen.openglnative

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import com.jesen.openglnative.Constants.SAMPLE_TYPE_KEY_SCRATCH_CARD

class MineGLSurfaceView(context: Context, mineGLRender: MineGLRender?, attrs: AttributeSet?) :
    GLSurfaceView(context, attrs), ScaleGestureDetector.OnScaleGestureListener {

    private val TAG = "MineGLSurfaceView"
    private val TOUCH_SCALE_FACTOR = 180.0f / 320
    private var mMineGLRender: MineGLRender
    private var mPreviousY = 0f
    private var mPreviousX = 0f
    private var mXAngle = 0f
    private var mYAngle = 0f
    private var mScaleGestureDetector: ScaleGestureDetector
    private var mPreScale = 1.0f
    private var mCurScale = 1.0f
    private var mLastMultiTouchTime: Long = 0L
    private var mRatioWidth = 0
    private var mRatioHeight = 0

    constructor(context: Context, mineGLRender: MineGLRender?) : this(context, mineGLRender, null)

    init {
        this.setEGLContextClientVersion(2)
        mMineGLRender = mineGLRender ?: MineGLRender()
        setEGLConfigChooser(8, 8, 8, 8, 16, 8);
        setRenderer(mMineGLRender)
        renderMode = RENDERMODE_WHEN_DIRTY
        mScaleGestureDetector = ScaleGestureDetector(context, this)
    }

    fun getEGLRender(): MineGLRender = mMineGLRender

    override fun onTouchEvent(event: MotionEvent): Boolean {
        if (event.pointerCount == 1) {
            consumeTouchEvent(event)
            val currentTimeMillis = System.currentTimeMillis()
            if (currentTimeMillis - mLastMultiTouchTime > 200) {
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
                when (mMineGLRender.getSampleType()) {
                    Constants.SAMPLE_TYPE_FBO_LEG,
                    Constants.SAMPLE_TYPE_COORD_SYSTEM,
                    Constants.SAMPLE_TYPE_BASIC_LIGHTING,
                    Constants.SAMPLE_TYPE_TRANS_FEEDBACK,
                    Constants.SAMPLE_TYPE_MULTI_LIGHTS,
                    Constants.SAMPLE_TYPE_DEPTH_TESTING,
                    Constants.SAMPLE_TYPE_INSTANCING,
                    Constants.SAMPLE_TYPE_PARTICLES,
                    Constants.SAMPLE_TYPE_SKYBOX,
                    Constants.SAMPLE_TYPE_3D_MODEL,
                    Constants.SAMPLE_TYPE_PBO,
                    Constants.SAMPLE_TYPE_KEY_VISUALIZE_AUDIO,
                    Constants.SAMPLE_TYPE_KEY_UBO,
                    Constants.SAMPLE_TYPE_KEY_TEXT_RENDER,
                    Constants.SAMPLE_TYPE_KEY_MSAA-> {
                        mMineGLRender.updateTransformMatrix(mXAngle, mYAngle, mCurScale, mCurScale)
                        requestRender()
                    }

                    else -> {}
                }
            }
        } else {
            mScaleGestureDetector.onTouchEvent(event)
        }
        return true
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val wSize = MeasureSpec.getSize(widthMeasureSpec)
        val hSize = MeasureSpec.getSize(heightMeasureSpec)
        if (mRatioWidth == 0 || mRatioHeight == 0) {
            setMeasuredDimension(wSize, hSize)
        } else {
            if (wSize < wSize * mRatioWidth / mRatioHeight) {
                setMeasuredDimension(wSize, wSize * mRatioHeight / mRatioWidth)
            } else {
                setMeasuredDimension(hSize * mRatioWidth / mRatioHeight, hSize)
            }
        }
    }

    fun setAspectRatio(w: Int, h: Int) {
        Log.d(TAG, "setAspectRatio() called with: width = [$w], height = [$h]")
        require(!(w < 0 || h < 0)) { "Size cannot be negative." }
        mRatioWidth = w
        mRatioHeight = h
        requestLayout()
    }

    override fun onScale(detector: ScaleGestureDetector): Boolean {
        when (mMineGLRender.getSampleType()) {
            Constants.SAMPLE_TYPE_COORD_SYSTEM,
            Constants.SAMPLE_TYPE_BASIC_LIGHTING,
            Constants.SAMPLE_TYPE_INSTANCING,
            Constants.SAMPLE_TYPE_3D_MODEL,
            Constants.SAMPLE_TYPE_KEY_VISUALIZE_AUDIO,
            Constants.SAMPLE_TYPE_KEY_TEXT_RENDER,
            Constants.SAMPLE_TYPE_KEY_MSAA -> {
                val preSpan = detector.previousSpan
                val curSpan = detector.currentSpan
                mCurScale = if (curSpan < preSpan) {
                    mPreScale - (preSpan - curSpan) / 200
                } else {
                    mPreScale + (curSpan - preSpan) / 200
                }
                mCurScale = 0.05f.coerceAtLeast(mCurScale.coerceAtMost(80.0f))
                mMineGLRender.updateTransformMatrix(mXAngle, mYAngle, mCurScale, mCurScale)
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

    private fun consumeTouchEvent(e: MotionEvent) {
        dealClickEvent(e)

        var touchX = -1f
        var touchY = -1f
        when (e.action) {
            MotionEvent.ACTION_MOVE -> {
                touchX = e.x
                touchY = e.y
            }

            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                touchX = -1f
                touchY = -1f
            }
        }
        when (mMineGLRender.getSampleType()) {
            SAMPLE_TYPE_KEY_SCRATCH_CARD -> {
                mMineGLRender.setTouchLoc(touchX, touchY)
                requestRender()
            }

            else -> {}
        }
    }

    private fun dealClickEvent(e:MotionEvent){
        var touchX = -1f
        var touchY = -1f
        when(e.action){
            MotionEvent.ACTION_UP->{
                touchX = e.x
                touchY = e.y
                when(mMineGLRender.getSampleType()){
                    Constants.SAMPLE_TYPE_KEY_SHOCK_WAVE->mMineGLRender.setTouchLoc(touchX,touchY)
                    else->{}
                }
            }
            else->{}
        }
    }
}