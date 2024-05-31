//
// Created by xie_s on 2024/5/31.
//

#ifndef OPENGLNATIVE_DEPTHTESTINGSAMPLE_H
#define OPENGLNATIVE_DEPTHTESTINGSAMPLE_H

#include "GLSampleBase.h"
#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>

class DepthTestingSample:public GLSampleBase {

public:
    DepthTestingSample();

    ~DepthTestingSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void SetParamsInt(int paramType, int value0, int value1);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    void
    UpdateMatrix(glm::mat4 &mvpMatrix, glm::mat4 &modelMatrix, int angleXRotate, int angleYRotate,
                 float scale, glm::vec3 transVec3, float ratio);
    virtual void Destroy();

private:
    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;

    GLuint m_VAOId;
    GLuint m_VBOIds[1];
    NativeImage m_RenderImage;
    glm::mat4 m_MVPMatrix;
    glm::mat4 m_ModelMatrix;

    int m_AngleX;
    int m_AngleY;
};


#endif //OPENGLNATIVE_DEPTHTESTINGSAMPLE_H
