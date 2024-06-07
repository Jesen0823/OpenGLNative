//
// Created by xie_s on 2024/5/24.
//

#ifndef OPENGLNATIVE_MINEGLRENDERCONTEXT_H
#define OPENGLNATIVE_MINEGLRENDERCONTEXT_H

#include "stdint.h"
#include <GLES3/gl3.h>
#include "TriangleSample.h"
#include "TextureMapSample.h"
#include "NV21TextureMapSample.h"
#include "VaoSample.h"
#include "FBOSample.h"
#include "FBOLegLengthSample.h"
#include "CoordSystemSample.h"
#include "BasicLightingSample.h"
#include "TransformFeedbackSample.h"
#include "MultiLightsSample.h"
#include "DepthTestingSample.h"
#include "Instancing3DSample.h"
#include "StencilTestingSample.h"
#include "BlendingSample.h"
#include "ParticlesSample.h"
#include "SkyBoxSample.h"
#include "Model3DSample.h"
#include "PBOSample.h"
#include "BeatingHeartSample.h"
#include "CloudSample.h"
#include "ShockWaveSample.h"
#include "BezierCurveSample.h"
#include "FaceSlenderSample.h"
#include "BigEyesSample.h"
#include "BigHeadSample.h"
#include "RotaryHeadSample.h"
#include "VisualizeAudioSample.h"
#include "ScratchCardSample.h"

class MineGlRenderContext {
public:
    MineGlRenderContext();

    ~MineGlRenderContext();

    void SetParamsInt(int paramType, int value0, int value1);

    void SetParamsFloat(int paramType, float value0, float value1);

    void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void SetImageData(int format, int width, int height, uint8_t *pData);

    void SetImageDataWithIndex(int index, int format, int width, int height, uint8_t *pData);

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    void SetParamsShortArr(short *const pShortArr, int arrSize);

    static void DestroyInstance();

    static MineGlRenderContext *GetInstance();

private:
    static MineGlRenderContext *m_pContext;
    GLSampleBase *m_pBeforeSample;
    GLSampleBase *m_pCurSample;
    int m_ScreenW;
    int m_ScreenH;
};


#endif //OPENGLNATIVE_MINEGLRENDERCONTEXT_H
