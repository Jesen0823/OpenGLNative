package com.jesen.openglnative.utils

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import com.jesen.openglnative.Constants
import java.nio.ByteBuffer

class LoadUtil private constructor(){
    companion object{
        private var instance:LoadUtil?=null
            get() {
                if(field == null) field = LoadUtil()
                return field
            }
        fun instance():LoadUtil{
            return instance!!
        }
    }

     fun loadRGBAImage(context: Context,resId: Int,callback:(Bitmap)->Unit): Bitmap {
        var bitmap: Bitmap
         context.resources.openRawResource(resId).use { ins ->
            bitmap = BitmapFactory.decodeStream(ins)
            bitmap.let { bp ->
                val buf = ByteBuffer.allocate(bp.byteCount)
                bp.copyPixelsToBuffer(buf)
                val byteArray = buf.array()
                callback(bitmap)
            }
        }
        return bitmap
    }
}