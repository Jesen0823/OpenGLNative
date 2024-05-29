package com.jesen.openglnative.egl

class NativeBgRender {

    external fun native_BgRenderInit()

    external fun native_BgRenderSetImageData(data: ByteArray, width: Int, height: Int)

    external fun native_BgRenderSetIntParams(paramType: Int, param0: Int,param1:Int)

    external fun native_BgRenderDraw()

    external fun native_BgRenderUnInit()
}