package com.jesen.openglnative.egl

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLES20
import android.opengl.GLException
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.jesen.openglnative.R
import com.jesen.openglnative.databinding.ActivityEglactivityBinding
import java.nio.ByteBuffer
import java.nio.IntBuffer


class EGLActivity : AppCompatActivity() {
    private lateinit var nativeBgRender:NativeBgRender
    private lateinit var binding:ActivityEglactivityBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityEglactivityBinding.inflate(layoutInflater)
        setContentView(binding.root)

        nativeBgRender = NativeBgRender()

        binding.button.setOnClickListener {
            val btn = it as Button
            if(btn.text ==resources.getString(R.string.btn_txt_reset)){
                binding.imageView.setImageResource(R.drawable.java)
                btn.setText(R.string.bg_render_txt)
            }else{
                startBgRender()
                btn.setText(R.string.btn_txt_reset)
            }
        }

    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.menu_main,menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if(item.itemId == R.id.action_select_sample){

        }
        return true
    }

    private fun startBgRender(){
        nativeBgRender.native_BgRenderInit()
        loadRGBAImage(R.drawable.java, nativeBgRender);
        nativeBgRender.native_BgRenderDraw();
        binding.imageView.setImageBitmap(createBitmapFromGLSurface(0, 0, 421, 586));
        nativeBgRender.native_BgRenderUnInit();
    }

    private fun loadRGBAImage(resId: Int, render: NativeBgRender) {
        resources.openRawResource(resId).use {
            val bitmap = BitmapFactory.decodeStream(it)
            bitmap?.let {
                val bytes = bitmap.byteCount
                val buf = ByteBuffer.allocate(bytes)
                bitmap.copyPixelsFromBuffer(buf)
                val byteArray = buf.array()
                render.native_BgRenderSetImageData(byteArray,bitmap.width,bitmap.height)
            }
        }
    }

    private fun createBitmapFromGLSurface(x: Int, y: Int, w: Int, h: Int): Bitmap? {
        val bitmapBuffer = IntArray(w * h)
        val bitmapSource = IntArray(w * h)
        val intBuffer = IntBuffer.wrap(bitmapBuffer)
        intBuffer.position(0)
        try {
            GLES20.glReadPixels(
                x, y, w, h, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE,
                intBuffer
            )
            var offset1: Int
            var offset2: Int
            for (i in 0 until h) {
                offset1 = i * w
                offset2 = (h - i - 1) * w
                for (j in 0 until w) {
                    val texturePixel = bitmapBuffer[offset1 + j]
                    val blue = texturePixel shr 16 and 0xff
                    val red = texturePixel shl 16 and 0x00ff0000
                    val pixel = texturePixel and -0xff0100 or red or blue
                    bitmapSource[offset2 + j] = pixel
                }
            }
        } catch (e: GLException) {
            return null
        }
        return Bitmap.createBitmap(bitmapSource, w, h, Bitmap.Config.ARGB_8888)
    }
}