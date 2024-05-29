//
// Created by xie_s on 2024/5/24.
//
#include "TriangleSample.h"
#include "MineGlRenderContext.h"
#include "LogUtil.h"
#include "VaoSample.h"
#include "FBOSample.h"
#include "FBOLegLengthSample.h"
#include "CoordSystemSample.h"

const int SAMPLE_TYPE =                     200;
const int SAMPLE_TYPE_KEY_TRIANGLE =        0;
const int SAMPLE_TYPE_KEY_TEXTURE_MAP =     1;
const int SAMPLE_TYPE_KEY_YUV_TEXTURE_MAP = 2;
const int SAMPLE_TYPE_KEY_VAO =             3;
const int SAMPLE_TYPE_KEY_FBO             = 4;
const int SAMPLE_TYPE_KEY_FBO_LEG_LENGTHEN   = 6;
const int SAMPLE_TYPE_KEY_COORD_SYSTEM       = 7;

MineGlRenderContext *MineGlRenderContext::m_pContext = nullptr;

MineGlRenderContext::MineGlRenderContext() {
    m_Sample = new TriangleSample();
}

MineGlRenderContext::~MineGlRenderContext() {
    if (m_Sample) {
        delete m_Sample;
        m_Sample = nullptr;
    }
}

void MineGlRenderContext::SetParamsInt(int paramType, int value0,int value1) {
    LOGCATE("MineGLREnderContext::SetParamsInt paramType = %d, value0 = %d, value1 = %d", paramType, value0,value1);

    if (paramType == SAMPLE_TYPE) {
        if (m_Sample) {
            delete m_Sample;
            m_Sample = nullptr;
        }
        switch (value0) {
            case SAMPLE_TYPE_KEY_TRIANGLE:
                m_Sample = new TriangleSample();
                break;
            case SAMPLE_TYPE_KEY_TEXTURE_MAP:
                m_Sample = new TextureMapSample();
                break;
            case SAMPLE_TYPE_KEY_YUV_TEXTURE_MAP:
                m_Sample = new NV21TextureMapSample();
                break;
            case SAMPLE_TYPE_KEY_VAO:
                m_Sample = new VaoSample();
                break;
            case SAMPLE_TYPE_KEY_FBO:
                m_Sample = new FBOSample();
            case SAMPLE_TYPE_KEY_FBO_LEG_LENGTHEN:
                m_Sample = new FBOLegLengthSample();
                break;
            case SAMPLE_TYPE_KEY_COORD_SYSTEM:
                m_Sample = new CoordSystemSample();
                break;
            default:
                break;
        }
    }
    if(paramType == ANGLE_PARAM_TYPE && m_Sample){
        m_Sample->SetParamsInt(paramType,value0,value1);
    }
}

void MineGlRenderContext::SetImageData(int format, int width, int height, uint8_t *pData) {
    LOGCATE("MineGLRenderContext::SetImageData format=%d, width=%d, height=%d, pData=%p", format,
            width, height, pData);
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;
    switch (format) {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }
    if (m_Sample) {
        m_Sample->LoadImage(&nativeImage);
    }
}

void MineGlRenderContext::OnSurfaceCreated() {
    LOGCATE("MineGLRenderContext::OnSurfaceCreated");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MineGlRenderContext::OnSurfaceChanged(int width, int height) {
    LOGCATE("MineGlRenderContext::OnSurfaceChanged [w, h] = [%d, %d]", width, height);
    glViewport(0, 0, width, height);
    m_ScreenW = width;
    m_ScreenH = height;
}

void MineGlRenderContext::OnDrawFrame() {
    LOGCATE("MineGLRenderContext::OnDrawFrame");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    if (m_Sample) {
        m_Sample->Init();
        m_Sample->Draw(m_ScreenW,m_ScreenH);
        m_Sample->Destroy();
    }
}

MineGlRenderContext *MineGlRenderContext::GetInstance() {
    LOGCATE("MineGlRenderContext::GetInstance");
    if (m_pContext == nullptr) {
        m_pContext = new MineGlRenderContext();
    }
    return m_pContext;;
}

void MineGlRenderContext::DestroyInstance() {
    LOGCATE("MineGlRenderContext::DestroyInstance");
    if (m_pContext) {
        delete m_pContext;
        m_pContext = nullptr;
    }
}
