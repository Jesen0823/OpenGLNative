//
// Created by xie_s on 2024/6/14.
//

#ifndef OPENGLNATIVE_BLITFRAMEBUFFERSAMPLE_H
#define OPENGLNATIVE_BLITFRAMEBUFFERSAMPLE_H

#include "ImageDef.h"
#include "GLSampleBase.h"

class BlitFrameBufferSample : public GLSampleBase {

public:
    BlitFrameBufferSample();

    virtual ~BlitFrameBufferSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    bool CreateFrameBufferObj();

private:
    GLuint m_ImageTextureId;
    GLuint m_FboTextureId;
    GLuint m_FboId;
    GLuint m_VaoIds[2];
    GLuint m_VboIds[4];
    GLint m_SamplerLoc;
    NativeImage m_RenderImage;
    GLuint m_FboProgramObj;
    GLuint m_FboVertexShader;
    GLuint m_FboFragmentShader;
    GLint m_FboSamplerLoc;
};


#endif //OPENGLNATIVE_BLITFRAMEBUFFERSAMPLE_H
