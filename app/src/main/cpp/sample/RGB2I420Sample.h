//
// Created by xie_s on 2024/6/14.
//

#ifndef OPENGLNATIVE_RGB2I420SAMPLE_H
#define OPENGLNATIVE_RGB2I420SAMPLE_H

#include "GLSampleBase.h"
#include "ImageDef.h"

class RGB2I420Sample : public GLSampleBase {

public:
    RGB2I420Sample();

    virtual ~RGB2I420Sample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    bool CreateFrameBufferObj();

private:
    GLuint m_ImageTextureId;
    GLuint m_FboTextureId;
    GLuint m_FboId;
    GLuint m_VaoIds[2] = {GL_NONE};
    GLuint m_VboIds[4] = {GL_NONE};
    GLint m_SamplerLoc;
    NativeImage m_RenderImage;
    GLuint m_FboProgramObj;
    GLuint m_FboVertexShader;
    GLuint m_FboFragmentShader;
    GLint m_FboSamplerLoc;
};


#endif //OPENGLNATIVE_RGB2I420SAMPLE_H
