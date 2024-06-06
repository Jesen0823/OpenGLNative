//
// Created by xie_s on 2024/6/6.
//

#ifndef OPENGLNATIVE_BIGHEADSAMPLE_H
#define OPENGLNATIVE_BIGHEADSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "GLSampleBase.h"
#include "CommonDef.h"

using namespace glm;

#define TRIANGLE_COUNT           32
#define KEY_POINTS_COUNT         9

class BigHeadSample : public GLSampleBase {
public:
    BigHeadSample();

    virtual ~BigHeadSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    void CalculateMesh(float warpLevel);

    vec2 CalculateIntersection(vec2 inputPoint, vec2 centerPoint);

    vec2 WarpKeyPoint(vec2 input, vec2 centerPoint, float level);

private:
    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    NativeImage m_RenderImage;
    mat4 m_MVPMatrix;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;

    int m_FrameIndex;

    vec3 m_Vertices[TRIANGLE_COUNT * 3];
    vec2 m_TexCoords[TRIANGLE_COUNT * 3];
    vec2 m_MeshPoints[24];
    vec2 m_KeyPointsInts[KEY_POINTS_COUNT];
    vec2 m_KeyPoints[KEY_POINTS_COUNT];
    GLuint m_VaoId;
    GLuint m_VboIds[2];
};


#endif //OPENGLNATIVE_BIGHEADSAMPLE_H
