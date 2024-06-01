//
// Created by xie_s on 2024/6/1.
//

#ifndef OPENGLNATIVE_INSTANCING3DSAMPLE_H
#define OPENGLNATIVE_INSTANCING3DSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "GLSampleBase.h"

class Instancing3DSample : public GLSampleBase {
public:
    Instancing3DSample();

    ~Instancing3DSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void SetParamsInt(int paramType, int value0, int value1);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    void
    UpdateMatrix(glm::mat4 &mvpMatrix, glm::mat4 &modelMatrix, int angleXRotate, int angleYRotate,
                 float scale, glm::vec3 transVec3, float ratio);

private:
    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    GLint m_ModelMatrixLoc;
    GLint m_ViewPosLoc;

    GLuint m_VaoId;
    GLuint m_VboIds[2];
    NativeImage m_RenderImage;
    glm::mat4 m_MVPMatrix;
    glm::mat4 m_ModelMatrix;

    int m_AngleX;
    int m_AngleY;
};


#endif //OPENGLNATIVE_INSTANCING3DSAMPLE_H
