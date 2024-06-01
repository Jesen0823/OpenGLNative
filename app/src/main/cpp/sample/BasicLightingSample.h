//
// Created by xie_s on 2024/5/29.
//

#ifndef OPENGLNATIVE_BASICLIGHTINGSAMPLE_H
#define OPENGLNATIVE_BASICLIGHTINGSAMPLE_H

#include "GLSampleBase.h"
#include "GLUtils.h"
#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>

class BasicLightingSample :public GLSampleBase{
public:
    BasicLightingSample();
    ~BasicLightingSample();

    virtual void Init();
    virtual void LoadImage(NativeImage *pImage);
    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);
    virtual void Draw(int screenW,int screenH);
    virtual void Destroy();

    /**
     * @param angleX 绕X轴旋转度数
     * @param angleY 绕Y轴旋转度数
     * @param ratio 宽高比
     * */
    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    GLint m_ModelMatrixLoc;
    GLint m_LightPosLoc;
    GLint m_LightColorLoc;
    GLint m_ViewPosLoc;

    GLuint m_VAOId;
    GLuint m_VBOIds[1];
    GLuint m_TfoId;
    GLuint m_TfoBufId;
    NativeImage m_RenderImage;
    glm::mat4 m_MVPMatrix;
    glm::mat4 m_ModelMatrix;

    int m_AngleX;
    int m_AngleY;

    float m_ScaleX;
    float m_ScaleY;

};


#endif //OPENGLNATIVE_BASICLIGHTINGSAMPLE_H
