//
// Created by xie_s on 2024/5/25.
//

#ifndef OPENGLNATIVE_TEXTUREMAPSAMPLE_H
#define OPENGLNATIVE_TEXTUREMAPSAMPLE_H


#include "GLSampleBase.h"
#include "../util/ImageDef.h"
#include "../util/GLUtils.h"

class TextureMapSample : public GLSampleBase {
public:
    TextureMapSample();

    virtual ~TextureMapSample();

    void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int width,int height);

    virtual void Destroy();

private:
    void CreateTexture();

    GLuint m_TextureId;
    GLint m_SamplerLoc;
    NativeImage m_RenderImage;
};


#endif //OPENGLNATIVE_TEXTUREMAPSAMPLE_H
