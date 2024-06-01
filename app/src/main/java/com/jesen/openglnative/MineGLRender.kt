package com.jesen.openglnative

import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MineGLRender() :GLSurfaceView.Renderer {
    private val TAG = "MineGLRender"
    private val nativeRender = MineNativeRender()
    private var mSampleType = 0
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        Log.d(TAG, "onSurfaceCreated() called with: gl = [$gl], config = [$config]");
        nativeRender.native_OnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        Log.d(TAG,
            "onSurfaceChanged() called with: gl = [$gl], width = [$width], height = [$height]"
        );
        nativeRender.native_OnSurfaceChanged(width, height);
    }

    override fun onDrawFrame(gl: GL10?) {
        Log.d(TAG, "onDrawFrame() called with: gl = [$gl]");
        nativeRender.native_OnDrawFrame();
    }

    fun setParamsInt(paramType:Int, value0:Int,value1:Int){
        if(paramType == Constants.SAMPLE_TYPE){
            mSampleType = value0
        }
        nativeRender.native_SetParamsInt(paramType, value0, value1)
    }

    fun setImageData(format: Int, width: Int, height: Int, bytes: ByteArray) {
        nativeRender.native_SetImageData(format, width, height, bytes)
    }

    fun init() {
        nativeRender.native_OnInit()
    }

    fun unInit() {
        nativeRender.native_OnUnInit()
    }

    fun getSampleType() = mSampleType
}