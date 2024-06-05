package com.jesen.openglnative.utils

import android.content.Context
import android.os.Environment
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.InputStream
import java.nio.channels.FileChannel


object FileUtil {
    /**
     * 将assets文件拷贝到sd卡根目录
     * @param assetsName assets文件名
     * @param fileName sd卡文件名
     * @return sd卡目标文件路径
     */
    @JvmStatic
    fun copyAssets(context: Context, assetsName: String, fileName: String): String {
        val file = File("${Environment.getExternalStorageDirectory()}/model/poly/", fileName)
        val path = file.absolutePath
        if (!file.exists()) {
            file.createNewFile()
        }
        val assetFileDescriptor = context.applicationContext.assets.openFd(assetsName)
        val from = FileInputStream(assetFileDescriptor.fileDescriptor).channel
        val to: FileChannel = FileOutputStream(path).channel
        from.transferTo(assetFileDescriptor.startOffset, assetFileDescriptor.length, to)
        return path
    }

    fun copyAssetsDirToSDCard(context: Context, assetsDirName: String, sdCardPath: String) {
        var sdCardPath = sdCardPath
        try {
            val list = context.assets.list(assetsDirName)
            if (list.isNullOrEmpty()) {
                val inputStream = context.assets.open(assetsDirName)
                val mByte = ByteArray(1024)
                var bt = 0
                val file = File(
                    sdCardPath + File.separator
                            + assetsDirName.substring(assetsDirName.lastIndexOf('/'))
                )
                if (!file.exists()) {
                    file.createNewFile()
                } else {
                    return
                }
                val fos = FileOutputStream(file)
                while ((inputStream.read(mByte).also { bt = it }) != -1) {
                    fos.write(mByte, 0, bt)
                }
                fos.flush()
                inputStream.close()
                fos.close()
            } else {
                var subDirName = assetsDirName
                if (assetsDirName.contains("/")) {
                    subDirName = assetsDirName.substring(assetsDirName.lastIndexOf('/') + 1)
                }
                sdCardPath = sdCardPath + File.separator + subDirName
                val file = File(sdCardPath)
                if (!file.exists()) file.mkdirs()
                for (s: String in list) {
                    copyAssetsDirToSDCard(context, assetsDirName + File.separator + s, sdCardPath)
                }
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }


    /**
     * 复制res/raw中的文件到指定目录
     *
     * @param context     上下文
     * @param id          资源ID
     * @param fileName    文件名
     * @param storagePath 目标文件夹的路径
     */
    @JvmStatic
    fun copyFilesFromRaw(context: Context, rawRes: Int, fileName: String): String {
        val inputStream = context.resources.openRawResource(rawRes)
        val path = File(Environment.getExternalStorageDirectory(), fileName).absolutePath
        readInputStream(path, inputStream)
        return path
    }

    /**
     * 读取输入流中的数据写入输出流
     *
     * @param storagePath 目标文件路径
     * @param inputStream 输入流
     */
    private fun readInputStream(storagePath: String, inputStream: InputStream) {
        val file = File(storagePath)
        if (!file.exists()) {
            inputStream.use {
                // 1.建立通道对象
                val fos = FileOutputStream(file)
                // 2.定义存储空间
                val buffer = ByteArray(inputStream.available())
                // 3.开始读文件
                var lenght = 0
                while (it.read(buffer).also { lenght = it } != -1) { // 循环从输入流读取buffer字节
                    // 将Buffer中的数据写到outputStream对象中
                    fos.write(buffer, 0, lenght)
                }
            }
        }
    }
}