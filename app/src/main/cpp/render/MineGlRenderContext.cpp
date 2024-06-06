//
// Created by xie_s on 2024/5/24.
//
#include "MineGlRenderContext.h"
#include "LogUtil.h"

MineGlRenderContext *MineGlRenderContext::m_pContext = nullptr;

MineGlRenderContext::MineGlRenderContext() {
    m_pCurSample = new BezierCurveSample();
    m_pBeforeSample = nullptr;
}

MineGlRenderContext::~MineGlRenderContext() {
    if (m_pCurSample) {
        delete m_pCurSample;
        m_pCurSample = nullptr;
    }
    if (m_pBeforeSample) {
        delete m_pBeforeSample;
        m_pBeforeSample = nullptr;
    }
}

void MineGlRenderContext::SetParamsInt(int paramType, int value0, int value1) {
    LOGCATE("MineGLREnderContext::SetParamsInt paramType = %d, value0 = %d, value1 = %d", paramType,
            value0, value1);

    if (paramType == SAMPLE_TYPE) {
        m_pBeforeSample = m_pCurSample;
        LOGCATE("MineGLREnderContext::SetParamsInt 0 m_pBeforeSample = %p", m_pBeforeSample);

        switch (value0) {
            case SAMPLE_TYPE_KEY_TRIANGLE:
                m_pCurSample = new TriangleSample();
                break;
            case SAMPLE_TYPE_KEY_TEXTURE_MAP:
                m_pCurSample = new TextureMapSample();
                break;
            case SAMPLE_TYPE_KEY_YUV_TEXTURE_MAP:
                m_pCurSample = new NV21TextureMapSample();
                break;
            case SAMPLE_TYPE_KEY_VAO:
                m_pCurSample = new VaoSample();
                break;
            case SAMPLE_TYPE_KEY_FBO:
                m_pCurSample = new FBOSample();
                break;
            case SAMPLE_TYPE_KEY_FBO_LEG_LENGTHEN:
                m_pCurSample = new FBOLegLengthSample();
                break;
            case SAMPLE_TYPE_KEY_COORD_SYSTEM:
                m_pCurSample = new CoordSystemSample();
                break;
            case SAMPLE_TYPE_KEY_BASIC_LIGHTING:
                m_pCurSample = new BasicLightingSample();
                break;
            case SAMPLE_TYPE_KEY_TRANSFORM_FEEDBACK:
                m_pCurSample = new TransformFeedbackSample();
                break;
            case SAMPLE_TYPE_KEY_MULTI_LIGHTS:
                m_pCurSample = new MultiLightsSample();
                break;
            case SAMPLE_TYPE_KEY_DEPTH_TESTING:
                m_pCurSample = new DepthTestingSample();
                break;
            case SAMPLE_TYPE_KEY_INSTANCING:
                m_pCurSample = new Instancing3DSample();
                break;
            case SAMPLE_TYPE_KEY_STENCIL_TESTING:
                m_pCurSample = new StencilTestingSample();
                break;
            case SAMPLE_TYPE_KEY_BLENDING:
                m_pCurSample = new BasicLightingSample();
                break;
            case SAMPLE_TYPE_KEY_PARTICLES:
                m_pCurSample = new ParticlesSample();
                break;
            case SAMPLE_TYPE_KEY_SKYBOX:
                m_pCurSample = new SkyBoxSample();
                break;
            case SAMPLE_TYPE_KEY_3D_MODEL:
                m_pCurSample = new Model3DSample();
                break;
            case SAMPLE_TYPE_KEY_PBO:
                m_pCurSample = new PBOSample();
                break;
            case SAMPLE_TYPE_KEY_BEATING_HEART:
                m_pCurSample = new BeatingHeartSample();
                break;
            case SAMPLE_TYPE_KEY_CLOUD:
                m_pCurSample = new CloudSample();
                break;
            case SAMPLE_TYPE_KEY_SHOCK_WAVE:
                m_pCurSample = new ShockWaveSample();
                break;
            case SAMPLE_TYPE_KEY_BEZIER_CURVE:
                m_pCurSample = new BezierCurveSample();
                break;
            case SAMPLE_TYPE_KEY_FACE_SLENDER:
                m_pCurSample = new FaceSlenderSample();
                break;
            default:
                m_pCurSample = nullptr;
                break;
        }
        LOGCATE("MineGlRenderContext::SetParamsInt m_pBeforeSample = %p, m_pCurSample=%p",
                m_pBeforeSample, m_pCurSample);
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
    if (m_pCurSample) {
        m_pCurSample->LoadImage(&nativeImage);
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
    if (m_pBeforeSample) {
        m_pBeforeSample->Destroy();
        delete m_pBeforeSample;
        m_pBeforeSample = nullptr;
    }
    if (m_pCurSample) {
        m_pCurSample->Init();
        m_pCurSample->Draw(m_ScreenW, m_ScreenH);
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

void MineGlRenderContext::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX,
                                                float scaleY) {
    LOGCATE("MineGlRenderContext::UpdateTransformMatrix [rotateX, rotateY, scaleX, scaleY] = [%f, %f, %f, %f]",
            rotateX, rotateY, scaleX, scaleY);
    if (m_pCurSample) {
        m_pCurSample->UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    }
}

void MineGlRenderContext::SetImageDataWithIndex(int index, int format, int width, int height,
                                                uint8_t *pData) {
    LOGCATE("MyGLRenderContext::SetImageDataWithIndex index=%d, format=%d, width=%d, height=%d, pData=%p",
            index, format, width, height, pData);
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

    if (m_pCurSample) {
        m_pCurSample->LoadMultiImageWithIndex(index, &nativeImage);
    }
}
