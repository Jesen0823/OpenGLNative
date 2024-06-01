//
// Created by xie_s on 2024/5/24.
//

#ifndef OPENGLNATIVE_GLSAMPLEBASE_H
#define OPENGLNATIVE_GLSAMPLEBASE_H

#include <stdint.h>
#include <GLES3/gl3.h>
#include "ImageDef.h"

//For PI define
#define MATH_PI 3.1415926535897932384626433832802

#define ROTATE_ANGLE_PARAM_TYPE                     300
#define SAMPLE_TYPE                                 200
#define SAMPLE_TYPE_KEY_TRIANGLE                    SAMPLE_TYPE+0
#define SAMPLE_TYPE_KEY_TEXTURE_MAP                 SAMPLE_TYPE+1
#define SAMPLE_TYPE_KEY_YUV_TEXTURE_MAP             SAMPLE_TYPE+2
#define SAMPLE_TYPE_KEY_VAO                         SAMPLE_TYPE+3
#define SAMPLE_TYPE_KEY_FBO                         SAMPLE_TYPE+4
#define SAMPLE_TYPE_KEY_FBO_LEG_LENGTHEN            SAMPLE_TYPE+6
#define SAMPLE_TYPE_KEY_COORD_SYSTEM                SAMPLE_TYPE+7
#define SAMPLE_TYPE_KEY_BASIC_LIGHTING              SAMPLE_TYPE+8
#define SAMPLE_TYPE_KEY_TRANSFORM_FEEDBACK          SAMPLE_TYPE+9
#define SAMPLE_TYPE_KEY_MULTI_LIGHTS                SAMPLE_TYPE+10
#define SAMPLE_TYPE_KEY_DEPTH_TESTING               SAMPLE_TYPE+11
#define SAMPLE_TYPE_KEY_INSTANCING                  SAMPLE_TYPE + 12

class GLSampleBase {
protected:
    GLuint m_VertexShader;
    GLuint m_FragmentShader;
    GLuint m_ProgramObj;

public:
    GLSampleBase() {
        this->m_FragmentShader = 0;
        this->m_VertexShader = 0;
        this->m_ProgramObj = 0;
    }

    virtual ~GLSampleBase() {}

    virtual void Init() = 0;

    virtual void LoadImage(NativeImage *pImage){}

    virtual void Draw(int width, int height) = 0;

    virtual void Destroy() = 0;

    virtual void SetParamsInt(int paramType, int value0, int value1) {

    };
};

#endif //OPENGLNATIVE_GLSAMPLEBASE_H
