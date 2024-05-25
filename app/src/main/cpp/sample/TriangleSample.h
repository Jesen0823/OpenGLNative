//
// Created by xie_s on 2024/5/24.
//

#ifndef OPENGLNATIVE_TRIANGLESAMPLE_H
#define OPENGLNATIVE_TRIANGLESAMPLE_H

#include "GLSampleBase.h"

class TriangleSample : public GLSampleBase {
public:
    TriangleSample();

    ~TriangleSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw();

    virtual void Destroy();
};


#endif //OPENGLNATIVE_TRIANGLESAMPLE_H
