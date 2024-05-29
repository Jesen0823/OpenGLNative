package com.jesen.openglnative;

public interface Constants {
    public static final String TAG = "MainActivity";
    public static final int ANGLE_TYPE = 300;

    public static final int SAMPLE_TYPE = 200;

    public static final int SAMPLE_TYPE_TRIANGLE = SAMPLE_TYPE;
    public static final int SAMPLE_TYPE_TEXTURE_MAP = SAMPLE_TYPE + 1;
    public static final int SAMPLE_TYPE_YUV_TEXTURE_MAP = SAMPLE_TYPE + 2;
    public static final int SAMPLE_TYPE_VAO = SAMPLE_TYPE + 3;
    public static final int SAMPLE_TYPE_FBO = SAMPLE_TYPE + 4;
    public static final int SAMPLE_TYPE_EGL = SAMPLE_TYPE + 5;
    public static final int SAMPLE_TYPE_FBO_LEG = SAMPLE_TYPE + 6;
    public static final int SAMPLE_TYPE_COORD_SYSTEM = SAMPLE_TYPE + 7;
    public static final int SAMPLE_TYPE_BASIC_LIGHTING = SAMPLE_TYPE + 8;

    public static final int PARAM_TYPE_ROTATE_ANGLE = 300;

    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;

    public static final String[] SAMPLE_TITLES = {
            /*"DrawTriangle",
            "TextureMapping",
            "YUV Rendering",
            "VAO&VBO",
            "FBO Offscreen Rendering",
            "EGL Background Rendering",*/
            "绘制三角形", "纹理映射", "YUV 渲染", "VAO&VBO", "FBO 离屏渲染", "EGL 后台渲染", "FBO 拉伸", "坐标系统", "基础光照"
    };

}
