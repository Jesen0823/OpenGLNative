//
// Created by xie_s on 2024/6/14.
//

#ifndef OPENGLNATIVE_RENDER16BITGRAYSAMPLE_H
#define OPENGLNATIVE_RENDER16BITGRAYSAMPLE_H

#include "ImageDef.h"
#include "GLSampleBase.h"

class Render16BitGraySample : public GLSampleBase {

public:
    Render16BitGraySample() {
        m_yTextureId = GL_NONE;
        m_ySamplerLoc = GL_NONE;
    }

    virtual ~Render16BitGraySample() {
        NativeImageUtil::FreeNativeImage(&m_RenderImage);
    }

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:

    GLuint m_yTextureId;
    GLint m_ySamplerLoc;
    NativeImage m_RenderImage;
};


#endif //OPENGLNATIVE_RENDER16BITGRAYSAMPLE_H
