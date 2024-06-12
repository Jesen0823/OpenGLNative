//
// Created by xie_s on 2024/6/11.
//

#ifndef OPENGLNATIVE_RGB2YUVSAMPLE_H
#define OPENGLNATIVE_RGB2YUVSAMPLE_H

#include "GLSampleBase.h"
#include "ImageDef.h"

class RGB2YUVSample : public GLSampleBase {
public:
    RGB2YUVSample();

    virtual ~RGB2YUVSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    bool CreateFrameBufferObj();

private:
    GLuint m_ImageTextureId;
    GLuint m_FboTextureId;
    GLuint m_FboId;
    GLuint m_VaoIds[2] = {GL_NONE};;
    GLuint m_VboIds[4] = {GL_NONE};;
    GLint m_SamplerLoc;
    NativeImage m_RenderImage;
    GLuint m_FboProgramObj;
    GLuint m_FboVertexShader;
    GLuint m_FboFragmentShader;
    GLint m_FboSamplerLoc;

};


#endif //OPENGLNATIVE_RGB2YUVSAMPLE_H
