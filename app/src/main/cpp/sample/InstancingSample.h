//
// Created by xie_s on 2024/6/1.
//

#ifndef OPENGLNATIVE_INSTANCINGSAMPLE_H
#define OPENGLNATIVE_INSTANCINGSAMPLE_H

#include <detail/type_mat.hpp>
#include "GLSampleBase.h"
#include "ImageDef.h"

class InstancingSample: public GLSampleBase{
public:
    InstancingSample();
    ~InstancingSample();

    virtual void Init();
    virtual void Draw(int screenW,int screenH);
    virtual void Destroy();

private:
    GLuint m_InstanceVBO;
    GLuint m_VerticesVBO;
    GLuint m_VAO;
};


#endif //OPENGLNATIVE_INSTANCINGSAMPLE_H
