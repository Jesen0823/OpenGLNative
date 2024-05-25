//
// Created by xie_s on 2024/5/25.
//

#ifndef OPENGLNATIVE_VAOSAMPLE_H
#define OPENGLNATIVE_VAOSAMPLE_H

#include "GLSampleBase.h"

class VaoSample : public GLSampleBase{
public:
    VaoSample();
    ~VaoSample();

    virtual void Init();
    virtual void LoadImage(NativeImage *pImage);
    virtual void Draw();
    virtual void Destroy();

private:
    GLuint m_VaoId;
    GLuint  m_VboIds[2];
};


#endif //OPENGLNATIVE_VAOSAMPLE_H
