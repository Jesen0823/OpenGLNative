//
// Created by xie_s on 2024/6/15.
//

#ifndef OPENGLNATIVE_RENDERNV21SAMPLE_H
#define OPENGLNATIVE_RENDERNV21SAMPLE_H

#include "ImageDef.h"
#include "GLSampleBase.h"

class RenderNV21Sample : public GLSampleBase {

public:
    RenderNV21Sample() {
        m_TextureId = GL_NONE;
        m_TextureLoc = GL_NONE;
    }

    ~RenderNV21Sample() {
        NativeImageUtil::FreeNativeImage(&m_RenderImage);
    }

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:
    GLuint m_TextureId;
    GLuint m_TextureLoc;
    NativeImage m_RenderImage;
};


#endif //OPENGLNATIVE_RENDERNV21SAMPLE_H
