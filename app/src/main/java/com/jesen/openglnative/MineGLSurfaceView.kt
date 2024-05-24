package com.jesen.openglnative

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet

class MineGLSurfaceView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null) :
    GLSurfaceView(context, attrs) {
    private val TAG = "MineGLSurfaceView"
    private var mGLRender: MineGLRender
    private var mNativeRender: MineNativeRender

    init {
        this.setEGLContextClientVersion(3)
        mNativeRender = MineNativeRender()
        mGLRender = MineGLRender(mNativeRender)
        setRenderer(mGLRender)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    fun getNativeRender(): MineNativeRender = mNativeRender
}