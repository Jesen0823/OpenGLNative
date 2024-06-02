//
// Created by xie_s on 2024/6/2.
//

#ifndef OPENGLNATIVE_SKYBOXSAMPLE_H
#define OPENGLNATIVE_SKYBOXSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "GLSampleBase.h"

class SkyBoxSample : public GLSampleBase {
public:
    SkyBoxSample();

    ~SkyBoxSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage);

    virtual void Draw(int width, int height);

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    virtual void Destroy();

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float scale, float ratio);

private:
    GLuint m_TextureId;
    GLuint m_CubeProgramObj;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;

    GLint m_CubeSamplerLoc;
    GLint m_CubeMVPMatLoc;
    GLint m_CubeModelMatLoc;
    GLint m_ViewPosLoc;

    GLuint m_CubeVaoId;
    GLuint m_CubeVboId;
    GLuint m_SkyBoxVaoId;
    GLuint m_SkyBoxVboId;

    NativeImage m_RenderImage;
    NativeImage m_pSkyBoxRenderImg[6];

    glm::mat4 m_MVPMatrix;
    glm::mat4 m_ModelMatrix;

    int m_AngleX;
    int m_AngleY;

    float m_ScaleX;
    float m_ScaleY;
};


#endif //OPENGLNATIVE_SKYBOXSAMPLE_H
