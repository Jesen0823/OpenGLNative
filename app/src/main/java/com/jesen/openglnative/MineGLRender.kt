package com.jesen.openglnative

import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MineGLRender(val nativeRender: MineNativeRender) :GLSurfaceView.Renderer {
    private val TAG = "MineGLRender"
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
}