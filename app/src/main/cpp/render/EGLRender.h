//
// Created by xie_s on 2024/5/25.
//

#ifndef OPENGLNATIVE_EGLRENDER_H
#define OPENGLNATIVE_EGLRENDER_H

#define EGL_FEATURE_NUM   7

#include <stdint.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "ImageDef.h"

class EGLRender {
public:
    EGLRender();

    ~EGLRender();

    void Init();

    int CreateGlesEnv();

    void SetImageData(uint8_t *pData, int width, int height);

    void SetIntParams(int paramType, int param);

    void Draw();

    void DestroyGlesEnv();

    void UnInit();

    static EGLRender *GetInstance() {
        if (m_Instance == nullptr) {
            m_Instance = new EGLRender();
        }
        return m_Instance;
    }

    static void DestroyInstance() {
        if (m_Instance) {
            delete m_Instance;
            m_Instance = nullptr;
        }
    }

private:
    static EGLRender *m_Instance;

    GLuint m_ImageTextureId;
    GLuint m_FBOTextureId;
    GLuint m_FBOId;
    GLuint m_VAOIds[1];
    GLuint m_VBOIds[3];
    GLint m_SamplerLoc;
    GLint m_TexSizeLoc;
    NativeImage m_RenderImage;
    GLuint m_ProgramObj;
    GLuint m_VertexShader;
    GLuint m_FragShader;

    EGLConfig m_eglConfig;
    EGLSurface m_eglSurface;
    EGLContext m_eglCtx;
    EGLDisplay m_eglDisplay;
    bool m_IsGLContextReady;
    const char*m_fShaderStrs[EGL_FEATURE_NUM];
    int m_ShaderIndex;
};


#endif //OPENGLNATIVE_EGLRENDER_H
