//
// Created by xie_s on 2024/5/24.
//

#ifndef OPENGLNATIVE_GLSAMPLEBASE_H
#define OPENGLNATIVE_GLSAMPLEBASE_H

#include <stdint.h>
#include <GLES3/gl3.h>
#include "ImageDef.h"

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

    virtual void LoadImage(NativeImage *pImage) = 0;

    virtual void Draw(int width,int height) = 0;

    virtual void Destroy() = 0;

    void SetParamsInt(int paramType, int value0, int value1) {

    }
};

#endif //OPENGLNATIVE_GLSAMPLEBASE_H
