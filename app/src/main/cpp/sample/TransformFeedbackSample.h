//
// Created by xie_s on 2024/5/30.
//

#ifndef OPENGLNATIVE_TRANSFORMFEEDBACKSAMPLE_H
#define OPENGLNATIVE_TRANSFORMFEEDBACKSAMPLE_H

#include "GLSampleBase.h"
#include "../util/ImageDef.h"

class TransformFeedbackSample : public GLSampleBase {
public:
    TransformFeedbackSample();

    virtual ~TransformFeedbackSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:
    GLuint m_ImageTextureId;
    GLuint m_VAOId;
    GLuint m_VBOIds[2] = {GL_NONE};;
    GLint m_SamplerLoc;
    GLuint m_TransFeedbackObjId;
    GLuint m_TransFeedbackBufId;
    NativeImage m_RenderImage;
};


#endif //OPENGLNATIVE_TRANSFORMFEEDBACKSAMPLE_H
