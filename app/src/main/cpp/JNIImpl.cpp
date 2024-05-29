//
// Created by xie_s on 2024/5/24.
//
#include "LogUtil.h"
#include "MineGlRenderContext.h"
#include "jni.h"
#include "BgRender.h"

#define NATIVE_RENDER_CLASS_NAME "com/jesen/openglnative/MineNativeRender"
#define NATIVE_BG_RENDER_CLASS_NAME "com/jesen/openglnative/egl/NativeBgRender"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
native_OnInit(JNIEnv *env, jobject thiz) {
    MineGlRenderContext::GetInstance();
}

extern "C"
JNIEXPORT void JNICALL
native_OnUnInit(JNIEnv *env, jobject thiz) {
    MineGlRenderContext::DestroyInstance();
}

extern "C"
JNIEXPORT void JNICALL
native_SetImageData(JNIEnv *env, jobject thiz,
                    jint format, jint width,
                    jint height, jbyteArray imageData) {
    int len = env->GetArrayLength(imageData);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte *>(buf));
    MineGlRenderContext::GetInstance()->SetImageData(format, width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(imageData);
}

extern "C"
JNIEXPORT void JNICALL
native_SetParamsInt(JNIEnv *env, jobject thiz,
                    jint param_type, jint value0,jint value1) {
    MineGlRenderContext::GetInstance()->SetParamsInt(param_type, value0,value1);
}

extern "C"
JNIEXPORT void JNICALL
native_OnSurfaceCreated(JNIEnv *env, jobject thiz) {
    MineGlRenderContext::GetInstance()->OnSurfaceCreated();
}
extern "C"
JNIEXPORT void JNICALL
native_OnSurfaceChanged(JNIEnv *env, jobject thiz,
                        jint width, jint height) {
    MineGlRenderContext::GetInstance()->OnSurfaceChanged(width, height);
}
extern "C"
JNIEXPORT void JNICALL
native_OnDrawFrame(JNIEnv *env, jobject thiz) {
    MineGlRenderContext::GetInstance()->OnDrawFrame();
}

#ifdef __cplusplus
}
#endif

static JNINativeMethod g_NativeMethods[] = {
        {"native_OnInit",           "()V",      (void *) (native_OnInit)},
        {"native_OnUnInit",         "()V",      (void *) (native_OnUnInit)},
        {"native_SetImageData",     "(III[B)V", (void *) (native_SetImageData)},
        {"native_SetParamsInt",     "(III)V",    (void *) (native_SetParamsInt)},
        {"native_OnSurfaceCreated", "()V",      (void *) (native_OnSurfaceCreated)},
        {"native_OnSurfaceChanged", "(II)V",    (void *) (native_OnSurfaceChanged)},
        {"native_OnDrawFrame",      "()V",      (void *) (native_OnDrawFrame)},
};

static int
RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum) {
    LOGCATE("====RegisterNativeMethods===");
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        LOGCATE("RegisterNativeMethods fail. clazz == NULL");
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodNum) < 0) {
        LOGCATE("RegisterNativeMethods fail");
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnregisterNativeMethods(JNIEnv *env, const char *className) {
    LOGCATE("UnregisterNativeMethods");
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGCATE("UnregisterNativeMethods fail. clazz == NULL");
        return;
    }
    if (env != NULL) {
        env->UnregisterNatives(clazz);
    }
}


////////////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT void JNICALL
native_BgRenderInit(JNIEnv *env, jobject thiz) {
    BgRender::GetInstance()->Init();
}

extern "C"
JNIEXPORT void JNICALL
native_BgRenderSetImageData(JNIEnv *env, jobject thiz, jbyteArray data, jint width, jint height) {
    int len = env->GetArrayLength(data);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(data, 0, len, reinterpret_cast<jbyte *>(buf));
    BgRender::GetInstance()->SetImageData(buf, width, height);
    delete[] buf;
    env->DeleteLocalRef(data);
}

extern "C"
JNIEXPORT void JNICALL
native_BgRenderSetIntParams(JNIEnv *env,jobject thiz,jint param_type,jint param0,jint param1) {
    BgRender::GetInstance()->SetIntParams(param_type,param0,param1);
}

extern "C"
JNIEXPORT void JNICALL
native_BgRenderDraw(JNIEnv *env, jobject thiz) {
    BgRender::GetInstance()->Draw();
}

extern "C"
JNIEXPORT void JNICALL
native_BgRenderUnInit(JNIEnv *env, jobject thiz) {
    BgRender::GetInstance()->UnInit();
}

static JNINativeMethod g_BgRenderMethods[] = {
        {"native_BgRenderInit",         "()V",     (void *) (native_BgRenderInit)},
        {"native_BgRenderSetImageData", "([BII)V", (void *) (native_BgRenderSetImageData)},
        {"native_BgRenderSetIntParams", "(III)V",   (void *) (native_BgRenderSetIntParams)},
        {"native_BgRenderDraw",         "()V",     (void *) (native_BgRenderDraw)},
        {"native_BgRenderUnInit",       "()V",     (void *) (native_BgRenderUnInit)},
};

///////////////////////////////////////////////////////
extern "C" jint JNI_OnLoad(JavaVM *jvm, void *p) {
    LOGCATE("======= JNI_OnLoad===");
    jint jniRet = JNI_ERR;
    JNIEnv *env = nullptr;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return jniRet;
    }
    jint regRet = RegisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME, g_NativeMethods,
                                        sizeof(g_NativeMethods) /
                                        sizeof(g_NativeMethods[0]));
    if (regRet != JNI_TRUE) {
        return JNI_ERR;
    }

    regRet = RegisterNativeMethods(env, NATIVE_BG_RENDER_CLASS_NAME, g_BgRenderMethods,
                                   sizeof(g_BgRenderMethods) / sizeof(g_BgRenderMethods[0]));
    if (regRet != JNI_TRUE) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *jvm, void *p) {
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    UnregisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME);
    UnregisterNativeMethods(env, NATIVE_BG_RENDER_CLASS_NAME);
}