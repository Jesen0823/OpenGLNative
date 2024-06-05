//
// Created by xie_s on 2024/6/5.
//

#ifndef OPENGLNATIVE_PBOSAMPLE_H
#define OPENGLNATIVE_PBOSAMPLE_H

#include "../glm/detail/type_mat.hpp"
#include "GLSampleBase.h"
#include "../util/ImageDef.h"
#include "../util/GLUtils.h"

class PBOSample : public GLSampleBase {

public:
    PBOSample();

    virtual ~PBOSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    bool CreateFrameBufferObj();

    void UploadPixels();

    void DownloadPixels();

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
    GLint m_MVPMatrixLoc;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;
    glm::mat4 m_MVPMatrix;

    GLuint m_UploadPboIds[2];
    GLuint m_DownloadPboIds[2];
    int m_FrameIndex;
};


#endif //OPENGLNATIVE_PBOSAMPLE_H
