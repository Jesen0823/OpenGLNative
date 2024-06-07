package com.jesen.openglnative.utils

import android.Manifest
import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Environment
import android.provider.Settings
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat

class PermissionHelper {
    fun checkPermission(activity: AppCompatActivity){

        val multiplePermission =
            activity.registerForActivityResult(ActivityResultContracts.RequestMultiplePermissions()) {

            }

        // Android11 API 33以上
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            if (ActivityCompat.checkSelfPermission(
                    activity,
                    Manifest.permission.READ_MEDIA_AUDIO
                ) != PackageManager.PERMISSION_GRANTED ||
                ActivityCompat.checkSelfPermission(
                    activity,
                    Manifest.permission.READ_MEDIA_VIDEO
                ) != PackageManager.PERMISSION_GRANTED ||
                    ActivityCompat.checkSelfPermission(
                        activity,
                        Manifest.permission.RECORD_AUDIO
                    ) != PackageManager.PERMISSION_GRANTED){
                ActivityCompat.requestPermissions(
                    activity, arrayOf<String>(
                        Manifest.permission.READ_MEDIA_AUDIO,
                        Manifest.permission.READ_MEDIA_VIDEO,
                        Manifest.permission.RECORD_AUDIO
                    ), 200
                )
            }
        }

        // Android10 API 30以上 所有外部卡权限
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            val storageManagerLauncher =
                activity.registerForActivityResult(ActivityResultContracts.StartActivityForResult()) { result ->
                    if (result.resultCode == Activity.RESULT_OK) {
                        val isPermissionAllow = Environment.isExternalStorageManager()
                    }
                }
            if (Environment.isExternalStorageManager()) {
                val isPermissionAllow = true
            } else {
                val intent = Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION)
                intent.data = Uri.parse("package:" + activity.packageName)
                storageManagerLauncher.launch(intent)
            }
        } else { // Android10以下
            multiplePermission.launch(
                arrayOf(
                    Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.RECORD_AUDIO
                )
            )
        }
    }
}