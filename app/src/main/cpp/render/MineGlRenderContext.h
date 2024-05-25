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

class MineGlRenderContext {
public:
    MineGlRenderContext();

    ~MineGlRenderContext();

    void SetParamsInt(int paramType, int value);

    void SetImageData(int format, int width, int height, uint8_t *pData);

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    static void DestroyInstance();

    static MineGlRenderContext *GetInstance();

private:
    static MineGlRenderContext *m_pContext;
    GLSampleBase *m_Sample;
};


#endif //OPENGLNATIVE_MINEGLRENDERCONTEXT_H
