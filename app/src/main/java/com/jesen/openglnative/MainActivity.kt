package com.jesen.openglnative

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.LayoutInflater
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.view.ViewTreeObserver
import android.widget.Button
import android.widget.RelativeLayout
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.jesen.openglnative.Constants.IMAGE_FORMAT_RGBA
import com.jesen.openglnative.audio.AudioCollector
import com.jesen.openglnative.databinding.ActivityMainBinding
import com.jesen.openglnative.egl.EGLActivity
import com.jesen.openglnative.utils.FileUtil
import com.jesen.openglnative.utils.PermissionHelper
import java.nio.ByteBuffer


class MainActivity : AppCompatActivity(), AudioCollector.Callback,
    ViewTreeObserver.OnGlobalLayoutListener {
    private lateinit var mGLSurfaceView: MineGLSurfaceView
    private lateinit var mRootView: ViewGroup
    private lateinit var binding: ActivityMainBinding
    private var mSampleSelectedIndex =
        Constants.SAMPLE_TYPE_KEY_BEATING_HEART - Constants.SAMPLE_TYPE

    private val permissionHelper = PermissionHelper()
    private var sharedPre: SharedPreferences? = null
    private var editor: SharedPreferences.Editor? = null
    private var mAudioCollector: AudioCollector? = null
    private val mMineGLRender: MineGLRender = MineGLRender()

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

        mRootView = binding.rootView
        mRootView.viewTreeObserver.addOnGlobalLayoutListener(this@MainActivity)
        mMineGLRender.init()

        //mGLSurfaceView = binding.mineGlSurfaceView
        //mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
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

    private fun loadRGBAImage(resId: Int): Bitmap {
        var bitmap: Bitmap
        resources.openRawResource(resId).use { ins ->
            bitmap = BitmapFactory.decodeStream(ins)
            bitmap.let { bp ->
                val buf = ByteBuffer.allocate(bp.byteCount)
                bp.copyPixelsToBuffer(buf)
                val byteArray = buf.array()
                mMineGLRender.setImageData(IMAGE_FORMAT_RGBA, bp.width, bp.height, byteArray)
            }
        }
        return bitmap
    }

    private fun loadRGBAImage(resId: Int, index: Int): Bitmap {
        var bitmap: Bitmap
        resources.openRawResource(resId).use { ins ->
            bitmap = BitmapFactory.decodeStream(ins)

            val buf = ByteBuffer.allocate(bitmap.byteCount)
            bitmap.copyPixelsToBuffer(buf)
            val byteArray = buf.array()
            mMineGLRender.setImageDataWithIndex(
                index,
                IMAGE_FORMAT_RGBA,
                bitmap.width,
                bitmap.height,
                byteArray
            )
        }
        return bitmap
    }

    private fun loadNV21Image() {
        assets.open("YUV_Image_840x1074.NV21").use {
            val length = it.available()
            val buffer = ByteArray(length)
            it.read(buffer)
            mMineGLRender.setImageData(Constants.IMAGE_FORMAT_NV21, 840, 1074, buffer)
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

                mRootView.removeView(mGLSurfaceView)
                val lp = RelativeLayout.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
                )
                lp.addRule(RelativeLayout.CENTER_IN_PARENT)
                mGLSurfaceView = MineGLSurfaceView(this@MainActivity, mMineGLRender)
                mRootView.addView(mGLSurfaceView, lp)

                val selectIndex = adapter.getSelectIndex()
                adapter.apply {
                    setSelectIndex(position)
                    notifyItemChanged(selectIndex)
                    notifyItemChanged(position)
                }
                mSampleSelectedIndex = position

                mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY

                if (mRootView.width != mGLSurfaceView.width
                    || mRootView.height != mGLSurfaceView.height
                ) {
                    mGLSurfaceView.setAspectRatio(mRootView.width, mRootView.height);
                }

                mMineGLRender.setParamsInt(
                    Constants.SAMPLE_TYPE,
                    position + Constants.SAMPLE_TYPE,
                    0
                )
                val sampleType = position + Constants.SAMPLE_TYPE
                when (sampleType) {
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

                    Constants.SAMPLE_TYPE_PARTICLES -> {
                        loadRGBAImage(R.drawable.board_texture)
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }
                    Constants.SAMPLE_TYPE_SKYBOX -> {
                        loadRGBAImage(R.drawable.right, 0)
                        loadRGBAImage(R.drawable.left, 1)
                        loadRGBAImage(R.drawable.top, 2)
                        loadRGBAImage(R.drawable.bottom, 3)
                        loadRGBAImage(R.drawable.back, 4)
                        loadRGBAImage(R.drawable.front, 5)
                    }

                    Constants.SAMPLE_TYPE_PBO -> {
                        loadRGBAImage(R.drawable.front)
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }

                    Constants.SAMPLE_TYPE_KEY_BEATING_HEART -> {
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }

                    Constants.SAMPLE_TYPE_KEY_CLOUD -> {
                        loadRGBAImage(R.drawable.noise)
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }

                    Constants.SAMPLE_TYPE_KEY_SHOCK_WAVE -> {
                        loadRGBAImage(R.drawable.front)
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }

                    Constants.SAMPLE_TYPE_KEY_BEZIER_CURVE -> {
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }

                    Constants.SAMPLE_TYPE_KEY_FACE_SLENDER,
                    Constants.SAMPLE_TYPE_KEY_BIG_EYES -> {
                        val bitmap = loadRGBAImage(R.drawable.yifei)
                        mGLSurfaceView.setAspectRatio(bitmap.width, bitmap.height)
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }

                    Constants.SAMPLE_TYPE_KEY_BIG_HEAD,
                    Constants.SAMPLE_TYPE_KEY_ROTARY_HEAD -> {
                        val bitmap = loadRGBAImage(R.drawable.huge)
                        mGLSurfaceView.setAspectRatio(bitmap.width, bitmap.height)
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }

                    Constants.SAMPLE_TYPE_KEY_VISUALIZE_AUDIO -> {
                        if (mAudioCollector == null) {
                            mAudioCollector = AudioCollector(applicationContext)
                        }
                        mAudioCollector!!.addCallback(this@MainActivity)
                        mAudioCollector!!.initAudio()
                        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
                    }

                    Constants.SAMPLE_TYPE_KEY_SCRATCH_CARD -> {
                        val bitmap = loadRGBAImage(R.drawable.yifei)
                        mGLSurfaceView.setAspectRatio(bitmap.width, bitmap.height)
                    }

                    else -> {}
                }
                mGLSurfaceView.requestRender()

                if (sampleType != Constants.SAMPLE_TYPE_KEY_VISUALIZE_AUDIO && mAudioCollector != null) {
                    mAudioCollector!!.unInitAudio()
                    mAudioCollector = null
                }

                dialog.cancel()
            }
        })

        val layoutManager = LinearLayoutManager(this)
        layoutManager.orientation = LinearLayoutManager.VERTICAL
        recycler.layoutManager = layoutManager
        recycler.adapter = adapter
        recycler.scrollToPosition(mSampleSelectedIndex)
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

    override fun onPause() {
        super.onPause()
        mAudioCollector?.unInitAudio()
        mAudioCollector = null
    }

    override fun onDestroy() {
        super.onDestroy()
        mMineGLRender.unInit()
    }

    override fun onAudioBufferCallback(buffer: ShortArray) {
        Log.e(
            "MainActivity",
            "onAudioBufferCallback() called with: buffer[0] = [" + buffer[0] + "]"
        )
        mMineGLRender.setAudioData(buffer)
    }

    override fun onGlobalLayout() {
        mRootView.viewTreeObserver.removeOnGlobalLayoutListener(this)
        val lp = RelativeLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
        )
        lp.addRule(RelativeLayout.CENTER_IN_PARENT)
        mGLSurfaceView = MineGLSurfaceView(this, mMineGLRender)
        mRootView.addView(mGLSurfaceView, lp)
        mGLSurfaceView.renderMode = RENDERMODE_CONTINUOUSLY
    }
}