//
// Created by xie_s on 2024/6/12.
//

#ifndef OPENGLNATIVE_PORTRAITSTAYCOLOREXAMPLE_H
#define OPENGLNATIVE_PORTRAITSTAYCOLOREXAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "GLSampleBase.h"

class PortraitStayColorExample : public GLSampleBase {

public:
    PortraitStayColorExample();

    virtual ~PortraitStayColorExample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    GLuint m_TextureId;
    GLuint m_GrayTexId;
    GLuint m_MappingTexId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    GLuint m_VaoId;
    GLuint m_VboIds[3];
    NativeImage m_RenderImage;
    NativeImage m_GrayImage;
    NativeImage m_MappingImage;
    glm::mat4 m_MVPMatrix;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;

    int m_frameIndex;
};


#endif //OPENGLNATIVE_PORTRAITSTAYCOLOREXAMPLE_H
