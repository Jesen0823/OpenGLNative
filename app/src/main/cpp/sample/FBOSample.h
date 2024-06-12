//
// Created by xie_s on 2024/5/25.
//

#ifndef OPENGLNATIVE_FBOSAMPLE_H
#define OPENGLNATIVE_FBOSAMPLE_H

#include "GLSampleBase.h"
#include "../util/ImageDef.h"
#include "../util/GLUtils.h"

class FBOSample : public GLSampleBase {
public:
    FBOSample();

    virtual ~FBOSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int width, int height);

    virtual void Destroy();

    bool CreateFrameBufferObj();

private:
    GLuint m_ImageTextureId;
    GLuint m_FBOTextureId;
    GLuint m_FBOId;
    GLuint m_VAOIds[2] = {GL_NONE};;
    GLuint m_VBOIds[4] = {GL_NONE};;
    GLuint m_SamplerLoc;
    NativeImage m_RenderImage;
    GLuint m_FBOProgramObj;
    GLuint m_FBOVertexShader;
    GLuint m_FBOFragShader;
    GLint m_FBOSamplerLoc;
};


#endif //OPENGLNATIVE_FBOSAMPLE_H
