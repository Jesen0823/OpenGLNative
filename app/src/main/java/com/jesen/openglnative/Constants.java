package com.jesen.openglnative;

public interface Constants {
    public static final String TAG = "MainActivity";
    public static final int SAMPLE_TYPE = 200;

    public static final int SAMPLE_TYPE_KEY_TRIANGLE = 0;
    public static final int SAMPLE_TYPE_KEY_TEXTURE_MAP = 1;
    public static final int SAMPLE_TYPE_KEY_YUV_TEXTURE_MAP = 2;
    public static final int SAMPLE_TYPE_KEY_VAO = 3;
    public static final int SAMPLE_TYPE_KEY_FBO = 4;
    public static final int SAMPLE_TYPE_KEY_EGL = 5;

    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;

    public static final String[] SAMPLE_TITLES = {
            "DrawTriangle",
            "TextureMapping",
            "YUV Rendering",
            "VAO&VBO",
            "FBO Offscreen Rendering",
            "EGL Background Rendering",
    };

}
