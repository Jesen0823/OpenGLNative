//
// Created by xie_s on 2024/5/28.
//

#ifndef OPENGLNATIVE_COORDSYSTEMSAMPLE_H
#define OPENGLNATIVE_COORDSYSTEMSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "GLSampleBase.h"

#define MATH_PI 3.1415926535897932384626433832802
#define ANGLE_PARAM_TYPE           300

class CoordSystemSample :public GLSampleBase{
public:
    CoordSystemSample();

    ~CoordSystemSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void SetParamsInt(int paramType, int value0, int value1);

    virtual void Destroy();

    /**
     * @param angleX 绕X轴旋转度数
     * @param angleY 绕Y轴旋转度数
     * @param ratio 宽高比
     */
    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    GLuint m_VAOId;
    GLuint m_VBOIds[3];
    NativeImage m_RenderImage;
    glm::mat4 m_MVPMatrix;

    int m_AngleX;
    int m_AngleY;
};


#endif //OPENGLNATIVE_COORDSYSTEMSAMPLE_H
