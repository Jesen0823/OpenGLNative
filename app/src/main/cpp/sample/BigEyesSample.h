//
// Created by xie_s on 2024/6/6.
//

#ifndef OPENGLNATIVE_BIGEYESSAMPLE_H
#define OPENGLNATIVE_BIGEYESSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "GLSampleBase.h"
#include "LockUtil.h"
#include "GLUtils.h"

class BigEyesSample : public GLSampleBase {

public:
    BigEyesSample();

    virtual ~BigEyesSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    GLuint m_VaoId;
    GLuint m_VboIds[3];
    NativeImage m_RenderImage;
    glm::mat4 m_MVPMatrix;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;
    int m_FrameIndex;
};


#endif //OPENGLNATIVE_BIGEYESSAMPLE_H
