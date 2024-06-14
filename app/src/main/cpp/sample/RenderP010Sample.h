//
// Created by xie_s on 2024/6/14.
//

#ifndef OPENGLNATIVE_RENDERP010SAMPLE_H
#define OPENGLNATIVE_RENDERP010SAMPLE_H

#include "ImageDef.h"
#include "GLSampleBase.h"

class RenderP010Sample : public GLSampleBase {

public:
    RenderP010Sample() {
        m_yTextureId = GL_NONE;
        m_uvTextureId = GL_NONE;

        m_ySamplerLoc = GL_NONE;
        m_uvSamplerLoc = GL_NONE;
    }

    virtual ~RenderP010Sample() {
        NativeImageUtil::FreeNativeImage(&m_RenderImage);
    }

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:
    GLuint m_yTextureId;
    GLuint m_uvTextureId;

    GLint m_ySamplerLoc;
    GLint m_uvSamplerLoc;

    NativeImage m_RenderImage;

};


#endif //OPENGLNATIVE_RENDERP010SAMPLE_H
