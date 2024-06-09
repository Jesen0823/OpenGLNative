//
// Created by xie_s on 2024/6/5.
//

#ifndef OPENGLNATIVE_TIMETUNNELSAMPLE_H
#define OPENGLNATIVE_TIMETUNNELSAMPLE_H

#include <detail/type_mat.hpp>
#include "ImageDef.h"
#include "GLSampleBase.h"

class TimeTunnelSample : public GLSampleBase {
public:
    TimeTunnelSample();

    virtual ~TimeTunnelSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    bool CreateFrameBufferObj();

private:
    GLuint m_ImageTextureId;
    GLuint m_FboTextureId;
    GLuint m_FboId;
    GLuint m_VaoIds[2];
    GLuint m_VboIds[4];
    GLint m_SamplerLoc;
    GLint m_ImgSizeLoc;
    GLint m_TimeLoc;
    NativeImage m_RenderImage;
    GLuint m_FboProgramObj;
    GLuint m_FboVertexShader;
    GLuint m_FboFragmentShader;
    GLint m_FboSamplerLoc;
    GLint m_MVPMatrixLoc;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;
    glm::mat4 m_MVPMatrix;

    int m_FrameIndex;
};


#endif //OPENGLNATIVE_TIMETUNNELSAMPLE_H
