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
    public static final int SAMPLE_TYPE_TRANS_FEEDBACK = SAMPLE_TYPE + 9;
    public static final int SAMPLE_TYPE_MULTI_LIGHTS = SAMPLE_TYPE + 10;
    public static final int SAMPLE_TYPE_DEPTH_TESTING = SAMPLE_TYPE + 11;
    public static final int SAMPLE_TYPE_INSTANCING = SAMPLE_TYPE + 12;
    public static final int SAMPLE_TYPE_STENCIL_TESTING = SAMPLE_TYPE + 13;
    public static final int SAMPLE_TYPE_BLENDING = SAMPLE_TYPE + 14;
    public static final int SAMPLE_TYPE_PARTICLES = SAMPLE_TYPE + 15;
    public static final int SAMPLE_TYPE_SKYBOX = SAMPLE_TYPE + 16;
    public static final int SAMPLE_TYPE_3D_MODEL = SAMPLE_TYPE + 17;
    public static final int SAMPLE_TYPE_PBO = SAMPLE_TYPE + 18;
    public static final int SAMPLE_TYPE_KEY_BEATING_HEART = SAMPLE_TYPE + 19;
    public static final int SAMPLE_TYPE_KEY_CLOUD = SAMPLE_TYPE + 20;
    public static final int SAMPLE_TYPE_KEY_TIME_TUNNEL = SAMPLE_TYPE + 21;
    public static final int SAMPLE_TYPE_KEY_BEZIER_CURVE = SAMPLE_TYPE + 22;
    public static final int SAMPLE_TYPE_KEY_FACE_SLENDER = SAMPLE_TYPE + 23;
    public static final int SAMPLE_TYPE_KEY_BIG_EYES = SAMPLE_TYPE + 24;
    public static final int SAMPLE_TYPE_KEY_BIG_HEAD = SAMPLE_TYPE + 25;
    public static final int SAMPLE_TYPE_KEY_ROTARY_HEAD = SAMPLE_TYPE + 26;
    public static final int SAMPLE_TYPE_KEY_VISUALIZE_AUDIO = SAMPLE_TYPE + 27;
    public static final int SAMPLE_TYPE_KEY_SCRATCH_CARD = SAMPLE_TYPE + 28;
    public static final int SAMPLE_TYPE_KEY_AVATAR = SAMPLE_TYPE + 29;
    public static final int SAMPLE_TYPE_KEY_SHOCK_WAVE = SAMPLE_TYPE + 30;
    public static final int SAMPLE_TYPE_KEY_MRT = SAMPLE_TYPE + 31;
    public static final int SAMPLE_TYPE_KEY_FBO_BLIT = SAMPLE_TYPE + 32;
    public static final int SAMPLE_TYPE_KEY_TBO = SAMPLE_TYPE + 33;
    public static final int SAMPLE_TYPE_KEY_UBO = SAMPLE_TYPE + 34;
    public static final int SAMPLE_TYPE_KEY_RGB2YUV = SAMPLE_TYPE + 35;
    public static final int SAMPLE_TYPE_KEY_MULTI_THREAD_RENDER = SAMPLE_TYPE + 36;
    public static final int SAMPLE_TYPE_KEY_TEXT_RENDER = SAMPLE_TYPE + 37;
    public static final int SAMPLE_TYPE_KEY_STAY_COLOR = SAMPLE_TYPE + 38;
    public static final int SAMPLE_TYPE_KEY_TRANSITIONS_1 = SAMPLE_TYPE + 39;
    public static final int SAMPLE_TYPE_KEY_TRANSITIONS_2 = SAMPLE_TYPE + 40;
    public static final int SAMPLE_TYPE_KEY_TRANSITIONS_3 = SAMPLE_TYPE + 41;
    public static final int SAMPLE_TYPE_KEY_TRANSITIONS_4 = SAMPLE_TYPE + 42;
    public static final int SAMPLE_TYPE_KEY_CONVEYOR_BELT = SAMPLE_TYPE + 43;
    public static final int SAMPLE_TYPE_SET_TOUCH_LOC = SAMPLE_TYPE + 999;
    public static final int SAMPLE_TYPE_SET_GRAVITY_XY = SAMPLE_TYPE + 1000;
    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;
    public static final int IMAGE_FORMAT_YUYV = 0x05;
    public static final int IMAGE_FORMAT_GARY = 0x06;

    public static final String[] SAMPLE_TITLES = {
            /*"DrawTriangle",
            "TextureMapping",
            "YUV Rendering",
            "VAO&VBO",
            "FBO Offscreen Rendering",
            "EGL Background Rendering",*/
            "绘制三角形", "纹理映射", "YUV 渲染", "VAO&VBO",
            "FBO 离屏渲染", "EGL 后台渲染", "FBO 拉伸", "坐标系统",
            "基础光照", "TransformFeedback", "MultiLights", "深度测试",
            "Instancing", "Stencil Testing", "Blending", "Particles",
            "SkyBox", "Assimp Load 3D Model", "PBO", "Beating Heart",
            "Cloud", "Time Tunnel", "Bezier Curve", "Face Slender", "Big Eyes",
            "Big Head", "Rotary Head", "Visualize Audio", "Scratch Card",
            "3D Avatar", "Shock Wave", "MRT", "FBO Blit", "Texture Buffer",
            "Uniform Buffer", "OpenGL RGB to YUV", "Multi-Thread Render",
            "Text Render", "Portrait stay color", "GL Transitions_1",
            "GL Transitions_2", "GL Transitions_3", "GL Transitions_4",
            "Conveyor Belt",
    };

}
