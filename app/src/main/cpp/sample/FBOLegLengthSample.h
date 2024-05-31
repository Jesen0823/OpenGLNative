//
// Created by xie_s on 2024/5/28.
//

#ifndef OPENGLNATIVE_FBOLEGLENGTHSAMPLE_H
#define OPENGLNATIVE_FBOLEGLENGTHSAMPLE_H

#include "GLSampleBase.h"
#include "../util/ImageDef.h"
#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>

#define VERTICAL_STRETCH_8_POINTS          0x10
#define VERTICAL_STRETCH_TOP_6_POINTS      0x11
#define VERTICAL_STRETCH_BOTTOM_6_POINTS   0x12
#define VERTICAL_STRETCH_4_POINTS          0x13

#define HORIZONTAL_STRETCH_8_POINTS        0x20
#define HORIZONTAL_STRETCH_LEFT_6_POINTS   0x21
#define HORIZONTAL_STRETCH_RIGHT_6_POINTS  0x22
#define HORIZONTAL_STRETCH_4_POINTS        0x23

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
    GLuint m_VBOIds[6];
    GLint m_SamplerLoc;
    NativeImage m_RenderImage;
    GLuint m_FBOProgramObj;
    GLuint m_FBOVertexShader;
    GLuint m_FBOFragShader;
    GLint m_FBOSamplerLoc;

    glm::mat4 m_FBOMVPMatrix;
    GLint m_FBOMVPMatLoc;

    glm::mat4 m_MVPMatrix;
    GLint m_MVPMatLoc;

    GLfloat m_dt;
    bool m_isGo;

    RectF m_StretchRect;
    int   m_StretchMode;

    bool  m_bIsVerticalMode;
};


#endif //OPENGLNATIVE_FBOLEGLENGTHSAMPLE_H
