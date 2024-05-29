//
// Created by xie_s on 2024/5/28.
//

#ifndef OPENGLNATIVE_FBOLEGLENGTHSAMPLE_H
#define OPENGLNATIVE_FBOLEGLENGTHSAMPLE_H

#include "GLSampleBase.h"
#include "../util/ImageDef.h"

class FBOLegLengthSample: public GLSampleBase{
public:
    FBOLegLengthSample();
    ~FBOLegLengthSample();

    virtual void Init();
    virtual void LoadImage(NativeImage *pImage);
    virtual void Draw(int screenW, int screenH);
    virtual void Destroy();

    bool CreateFrameBufferObj();

private:
    GLuint m_ImageTextureId;
    GLuint m_FBOTextureId;
    GLuint m_FBOId;
    GLuint m_VAOIds[2];
    GLuint m_VBOIds[4];
    GLint m_SamplerLoc;
    NativeImage m_RenderImage;
    GLuint m_FBOProgramObj;
    GLuint m_FBOVertexShader;
    GLuint m_FBOFragShader;
    GLint m_FBOSamplerLoc;

    GLfloat m_dt;
    bool m_isGo;
};


#endif //OPENGLNATIVE_FBOLEGLENGTHSAMPLE_H
