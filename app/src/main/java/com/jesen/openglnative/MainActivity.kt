package com.jesen.openglnative

import android.content.Intent
import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.LayoutInflater
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.jesen.openglnative.databinding.ActivityMainBinding
import com.jesen.openglnative.egl.EGLActivity
import java.nio.ByteBuffer

class MainActivity : AppCompatActivity() {
    private lateinit var mGLSurfaceView: MineGLSurfaceView
    private lateinit var binding: ActivityMainBinding
    private var mSampleSelectedIndex = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.mineGlSurfaceView.getNativeRender().native_OnInit()
        mGLSurfaceView = binding.mineGlSurfaceView
    }

    private fun loadRGBAImage(resId: Int) {
        resources.openRawResource(resId).use {
            val bitmap = BitmapFactory.decodeStream(it)
            val bytes = bitmap.byteCount
            val byteBuf = ByteBuffer.allocate(bytes)
            bitmap.copyPixelsFromBuffer(byteBuf)
            val byteArray = byteBuf.array()
            mGLSurfaceView.getNativeRender()
                .native_SetImageData(
                    Constants.IMAGE_FORMAT_RGBA,
                    bitmap.width,
                    bitmap.height,
                    byteArray
                )
        }
    }

    private fun loadNV21Image() {
        assets.open("YUV_Image_840x1074.NV21").use {
            val length = it.available()
            val buffer = ByteArray(length)
            it.read(buffer)
            mGLSurfaceView.getNativeRender()
                .native_SetImageData(Constants.IMAGE_FORMAT_NV21, 840, 1074, buffer)
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
                val selectIndex = adapter.getSelectIndex()
                adapter.apply {
                    setSelectIndex(position)
                    notifyItemChanged(selectIndex)
                    notifyItemChanged(position)
                }
                mSampleSelectedIndex = position

                mGLSurfaceView.getNativeRender()
                    .native_SetParamsInt(Constants.SAMPLE_TYPE, position, 0)
                when (position) {
                    Constants.SAMPLE_TYPE_KEY_TRIANGLE,
                    Constants.SAMPLE_TYPE_KEY_TEXTURE_MAP -> loadRGBAImage(R.raw.dzzz)

                    Constants.SAMPLE_TYPE_KEY_YUV_TEXTURE_MAP -> loadNV21Image()
                    Constants.SAMPLE_TYPE_KEY_VAO -> {}
                    Constants.SAMPLE_TYPE_KEY_FBO -> loadRGBAImage(R.raw.java)
                    Constants.SAMPLE_TYPE_KEY_EGL -> {
                        startActivity(Intent(this@MainActivity, EGLActivity::class.java))
                    }

                    Constants.SAMPLE_TYPE_KEY_FBO_LEG -> loadRGBAImage(R.raw.leg)
                    Constants.SAMPLE_TYPE_KEY_COORD_SYSTEM -> loadRGBAImage(R.raw.dzzz)
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
        mGLSurfaceView.getNativeRender().native_OnUnInit()
    }
}