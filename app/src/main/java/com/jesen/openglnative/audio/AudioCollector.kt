package com.jesen.openglnative.audio

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.core.app.ActivityCompat

class AudioCollector(val context: Context) : AudioRecord.OnRecordPositionUpdateListener {
    companion object {
        private const val TAG = "AudioCollector"
        private const val RECORD_SAMPLE_RATE = 44100
        private const val RECORD_CHANNEL = 1
        private const val RECORD_ENCODING_BIT = 16
        private const val RECORD_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT
    }

    private var mAudioRecord: AudioRecord? = null
    private lateinit var mThread: Thread
    private lateinit var mAudioBuffer: ShortArray
    private lateinit var mHandler: Handler
    private var mBufferSize = 0
    private var mCallback: Callback? = null

    init {
        mBufferSize = 2 * AudioRecord.getMinBufferSize(
            RECORD_SAMPLE_RATE, RECORD_CHANNEL,
            RECORD_AUDIO_ENCODING
        )
    }

    fun initAudio() {
        if (ActivityCompat.checkSelfPermission(
                context,
                Manifest.permission.RECORD_AUDIO
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            Log.e(TAG,"Must need Manifest.permission.RECORD_AUDIO allow!")
            return
        }
        mAudioRecord = AudioRecord(
            MediaRecorder.AudioSource.MIC, RECORD_SAMPLE_RATE, RECORD_CHANNEL,
            RECORD_AUDIO_ENCODING, mBufferSize
        )
        mAudioRecord?.startRecording()
        mThread = Thread {
            mAudioBuffer = ShortArray(mBufferSize)
            Looper.prepare()
            mHandler = Looper.myLooper()?.let { Handler(it) }!!
            mAudioRecord?.setRecordPositionUpdateListener(this@AudioCollector, mHandler)

            val bytePerSample = RECORD_ENCODING_BIT / 8
            val sampleToDraw = mBufferSize / bytePerSample
            mAudioRecord?.positionNotificationPeriod = sampleToDraw
            Looper.loop()
        }
        mThread.start()
    }

    fun unInitAudio() {
        mAudioRecord?.stop()
        mAudioRecord?.release()
        mHandler.looper.quitSafely()
        mAudioRecord = null
    }

    fun addCallback(callback: Callback) {
        mCallback = callback
    }

    override fun onMarkerReached(recorder: AudioRecord?) {

    }

    override fun onPeriodicNotification(recorder: AudioRecord?) {
        if (mAudioRecord?.recordingState == AudioRecord.RECORDSTATE_RECORDING
            && mAudioRecord?.read(mAudioBuffer, 0, mAudioBuffer.size) != -1
        ) {
            mCallback?.onAudioBufferCallback(mAudioBuffer)
        }
    }

    interface Callback {
        fun onAudioBufferCallback(buffer: ShortArray)
    }
}