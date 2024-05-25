//
// Created by xie_s on 2024/5/25.
//

#ifndef OPENGLNATIVE_NV21TEXTUREMAPSAMPLE_H

#include "GLSampleBase.h"
#include "../util//ImageDef.h"

#define OPENGLNATIVE_NV21TEXTUREMAPSAMPLE_H


class NV21TextureMapSample : public GLSampleBase {
public:
    NV21TextureMapSample() {
        m_yTextureId = GL_NONE;
        m_uvTextureId = GL_NONE;
        m_ySamplerLoc = GL_NONE;
        m_uvSamplerLoc = GL_NONE;
    }

    ~NV21TextureMapSample() {
        NativeImageUtil::FreeNativeImage(&m_RenderImage);
    }

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int width,int height);

    virtual void Destroy();

private:
    GLuint m_yTextureId;
    GLuint m_uvTextureId;
    GLint m_ySamplerLoc;
    GLint m_uvSamplerLoc;

    NativeImage m_RenderImage;
};


#endif //OPENGLNATIVE_NV21TEXTUREMAPSAMPLE_H
