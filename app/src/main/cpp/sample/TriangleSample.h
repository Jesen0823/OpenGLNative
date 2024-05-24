//
// Created by xie_s on 2024/5/24.
//

#ifndef OPENGLNATIVE_TRIANGLESAMPLE_H
#define OPENGLNATIVE_TRIANGLESAMPLE_H
#include "GLSampleBase.h"

class TriangleSample: public GLSampleBase{
public:
    TriangleSample();
    ~TriangleSample();

    virtual void Init();
    virtual void Draw();

    virtual void LoadImage(NativeImage *pImage);
};


#endif //OPENGLNATIVE_TRIANGLESAMPLE_H
