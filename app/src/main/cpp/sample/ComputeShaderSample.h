//
// Created by xie_s on 2024/6/18.
//

#ifndef OPENGLNATIVE_COMPUTESHADERSAMPLE_H
#define OPENGLNATIVE_COMPUTESHADERSAMPLE_H

#include "GLSampleBase.h"

class ComputeShaderSample : public GLSampleBase {

public:
    ComputeShaderSample() {

    }

    ~ComputeShaderSample() {
        NativeImageUtil::FreeNativeImage(&m_RenderImage);
    }

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:
    GLuint m_DataBuffer;
    int m_DataSize;
    NativeImage m_RenderImage;
};


#endif //OPENGLNATIVE_COMPUTESHADERSAMPLE_H
