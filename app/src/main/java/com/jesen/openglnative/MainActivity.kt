package com.jesen.openglnative

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.LayoutInflater
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.widget.Button
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.jesen.openglnative.Constants.IMAGE_FORMAT_RGBA
import com.jesen.openglnative.databinding.ActivityMainBinding
import com.jesen.openglnative.egl.EGLActivity
import com.jesen.openglnative.utils.FileUtil
import com.jesen.openglnative.utils.PermissionHelper
import java.nio.ByteBuffer

class MainActivity : AppCompatActivity() {
    private lateinit var mGLSurfaceView: MineGLSurfaceView
    private lateinit var binding: ActivityMainBinding
    private var mSampleSelectedIndex = 0

    private val permissionHelper = PermissionHelper()
    private var sharedPre: SharedPreferences? = null
    private var editor: SharedPreferences.Editor? = null

    @RequiresApi(Build.VERSION_CODES.R)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if (sharedPre == null) {
            sharedPre = getSharedPreferences("config", MODE_PRIVATE)
            editor = sharedPre!!.edit()
        }

        permissionHelper.checkPermission(this)
        if (!sharedPre!!.getBoolean("initIO", false)) {
            Log.d("MainActivity", " need do initIO")
            Thread { initIO() }.start()
        }

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        mGLSurfaceView = binding.mineGlSurfaceView
        mGLSurfaceView.getEGLRender().init()
    }

    @RequiresApi(Build.VERSION_CODES.R)
    private fun initIO() {
        if (Environment.isExternalStorageManager()) {
            Log.d("MainActivity", " initIO() has allowed permission")
            val fileDir = getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS)!!.absolutePath
            FileUtil.copyAssetsDirToSDCard(this@MainActivity, "poly", "$fileDir/model")

            editor!!.putBoolean("initIO", true)
        }
    }

    private fun loadRGBAImage(resId: Int) {
        resources.openRawResource(resId).use { ins ->
            val bitmap = BitmapFactory.decodeStream(ins)
            bitmap?.let { bp ->
                val buf = ByteBuffer.allocate(bp.byteCount)
                bp.copyPixelsToBuffer(buf)
                val byteArray = buf.array()
                mGLSurfaceView.getEGLRender()
                    .setImageData(IMAGE_FORMAT_RGBA, bp.width, bp.height, byteArray)
            }
        }
    }

    private fun loadRGBAImage(resId: Int, index: Int) {
        resources.openRawResource(resId).use { ins ->
            val bitmap = BitmapFactory.decodeStream(ins)
            bitmap?.let { bp ->
                val buf = ByteBuffer.allocate(bp.byteCount)
                bp.copyPixelsToBuffer(buf)
                val byteArray = buf.array()
                mGLSurfaceView.getEGLRender()
                    .setImageDataWithIndex(index, IMAGE_FORMAT_RGBA, bp.width, bp.height, byteArray)
            }
        }
    }

    private fun loadNV21Image() {
        assets.open("YUV_Image_840x1074.NV21").use {
            val length = it.available()
            val buffer = ByteArray(length)
            it.read(buffer)
            mGLSurfaceView.getEGLRender()
                .setImageData(Constants.IMAGE_FORMAT_NV21, 840, 1074, buffer)
        }
    }

    private fun showGLSampleDialog() {
        val builder = AlertDialog.Builder(this)
        val inflater = LayoutInflater.from(this)
        val viewRoot = inflater.inflate(R.layout.sample_select_layout, null)
        val dialog = builder.create()
        val confirmBtn = viewRoot.findViewById<Button>(R.id.confirm_btn)
        confirmBtn.setOnClickListener {
            dialog.cancel()
        }
        val recycler = viewRoot.findViewById<RecyclerView>(R.id.resolution_list_view)
        val adapter = MainRecyclerAdapter(this, Constants.SAMPLE_TITLES.toList())
        adapter.setSelectIndex(mSampleSelectedIndex)
        adapter.addOnItemClickListener(object : OnItemClickListener {
            override fun onItemClick(view: View, position: Int) {
                Log.i("MainActivity", "---OnItemClick, position = $position")
                val selectIndex = adapter.getSelectIndex()
                adapter.apply {
                    setSelectIndex(position)
                    notifyItemChanged(selectIndex)
                    notifyItemChanged(position)
                }
                mSampleSelectedIndex = position

                mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY

                mGLSurfaceView.getEGLRender()
                    .setParamsInt(Constants.SAMPLE_TYPE, position + Constants.SAMPLE_TYPE, 0)
                when (position + Constants.SAMPLE_TYPE) {
                    Constants.SAMPLE_TYPE_TRIANGLE -> {}
                    Constants.SAMPLE_TYPE_TEXTURE_MAP -> loadRGBAImage(R.drawable.dzzz)

                    Constants.SAMPLE_TYPE_YUV_TEXTURE_MAP -> loadNV21Image()
                    Constants.SAMPLE_TYPE_VAO -> {}
                    Constants.SAMPLE_TYPE_FBO -> loadRGBAImage(R.drawable.java)
                    Constants.SAMPLE_TYPE_EGL -> {
                        startActivity(Intent(this@MainActivity, EGLActivity::class.java))
                    }

                    Constants.SAMPLE_TYPE_FBO_LEG -> loadRGBAImage(R.drawable.leg)
                    Constants.SAMPLE_TYPE_COORD_SYSTEM, Constants.SAMPLE_TYPE_BASIC_LIGHTING,
                    Constants.SAMPLE_TYPE_TRANS_FEEDBACK, Constants.SAMPLE_TYPE_MULTI_LIGHTS,
                    Constants.SAMPLE_TYPE_DEPTH_TESTING, Constants.SAMPLE_TYPE_INSTANCING,
                    Constants.SAMPLE_TYPE_STENCIL_TESTING -> {
                        loadRGBAImage(R.drawable.board_texture)
                    }

                    Constants.SAMPLE_TYPE_BLENDING -> {
                        loadRGBAImage(R.drawable.board_texture, 0)
                        loadRGBAImage(R.drawable.floor, 1)
                        loadRGBAImage(R.drawable.window, 2)
                    }

                    Constants.SAMPLE_TYPE_PARTICLES -> loadRGBAImage(R.drawable.board_texture)
                    Constants.SAMPLE_TYPE_SKYBOX -> {
                        loadRGBAImage(R.drawable.right, 0)
                        loadRGBAImage(R.drawable.left, 1)
                        loadRGBAImage(R.drawable.top, 2)
                        loadRGBAImage(R.drawable.bottom, 3)
                        loadRGBAImage(R.drawable.back, 4)
                        loadRGBAImage(R.drawable.front, 5)
                    }

                    Constants.SAMPLE_TYPE_PBO -> loadRGBAImage(R.drawable.front)
                    else -> {}
                }
                mGLSurfaceView.requestRender()

                dialog.cancel()
            }
        })

        val layoutManager = LinearLayoutManager(this)
        layoutManager.orientation = LinearLayoutManager.VERTICAL
        recycler.layoutManager = layoutManager
        recycler.adapter = adapter

        dialog.show()
        dialog.window?.setContentView(viewRoot)
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if (item.itemId == R.id.action_select_sample) {
            showGLSampleDialog()
        }
        return true
    }

    override fun onDestroy() {
        super.onDestroy()
        mGLSurfaceView.getEGLRender().unInit()
    }
}