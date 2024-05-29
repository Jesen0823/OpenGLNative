//
// Created by xie_s on 2024/5/28.
//

#include "FBOLegLengthSample.h"
#include "GLUtils.h"

#define VERTEX_POS_INDX 0
#define TEXTURE_POS_INDX 1

FBOLegLengthSample::FBOLegLengthSample() {
    m_VAOIds[2] = {GL_NONE};
    m_VBOIds[3] = {GL_NONE};
    m_ImageTextureId = GL_NONE;
    m_FBOTextureId = GL_NONE;
    m_SamplerLoc = GL_NONE;

    m_FBOId = GL_NONE;
    m_FBOProgramObj = GL_NONE;
    m_FBOVertexShader = GL_NONE;
    m_FBOFragShader = GL_NONE;
    m_FBOSamplerLoc = GL_NONE;

    m_dt = 0.0;
    m_isGo = true;
}

FBOLegLengthSample::~FBOLegLengthSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void FBOLegLengthSample::Init() {
    if (m_dt <= 0) m_isGo = true;
    if (m_dt >= 0.2) m_isGo = false;
    if (m_isGo) {
        m_dt += 0.01;
    } else {
        m_dt -= 0.01;
    }
    // 顶点坐标
    GLfloat vVertices[] = {
            -0.8f, 0.8f + m_dt, 0.0f,
            -0.8f, 0.0f + m_dt, 0.0f,
            0.8f, 0.0f + m_dt, 0.0f,
            0.8f, 0.8f + m_dt, 0.0f,

            -0.8f, -0.55f - m_dt, 0.0f,
            -0.8f, -0.8f - m_dt, 0.0f,
            0.8f, -0.8f - m_dt, 0.0f,
            0.8f, -0.55f - m_dt, 0.0f,
    };

    // 正常纹理坐标
    GLfloat vTexCoors[] = {
            0.0f, 0.0f,
            0.0f, 0.5f,
            1.0f, 0.5f,
            1.0f, 0.0f,

            0.0f, 0.84f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.84f,
    };
    // FBO纹理坐标与正常纹理方向不同
    GLfloat vFBOTexCoors[] = {
            0.0f, 1.0f,
            0.0f, 0.5f,
            1.0f, 0.5f,
            1.0f, 1.0f,

            0.0f, 0.16f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 0.16f,
    };

    GLushort indices[] = {0, 1, 2, 0, 2, 3, 1, 4, 7, 1, 7, 2, 4, 5, 6, 4, 6, 7};
    char vShaderStr[] =
            "#version 300 es                                     \n"
            "layout(location = 0) in vec4 a_position;            \n"
            "layout(location = 1) in vec2 a_texCoord;            \n"
            "out vec2 v_texCoord;                                \n"
            "void main(){                                        \n"
            "  gl_Position = a_position;                         \n"
            "  v_texCoord = a_texCoord;                          \n"
            "}";
    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 v_texCoord;                                 \n"
            "layout(location =0) out vec4 outColor;              \n"
            "uniform sampler2D s_TextureMap;                     \n"
            "void main(){                                        \n"
            "   outColor = texture(s_TextureMap, v_texCoord);    \n"
            "}";
    char fFBOShaderStr[] =
            "#version 300 es                                                                     \n"
            "precision mediump float;                                                            \n"
            "in vec2 v_texCoord;                                                                 \n"
            "layout(location = 0) out vec4 outColor;                                             \n"
            "uniform sampler2D s_TextureMap;                                                     \n"
            "void main(){                                                                        \n"
            "   vec4 tempColor = texture(s_TextureMap, v_texCoord);                              \n"
            "   float luminance = tempColor.r * 0.299 + tempColor.g * 0.587 + tempColor.b * 0.114;\n"
            "   outColor = vec4(vec3(luminance), tempColor.a); \n"
            "}"; // 输出灰度图

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr,
                                          m_VertexShader, m_FragmentShader);
    m_FBOProgramObj = GLUtils::CreateProgram(vShaderStr, fFBOShaderStr,
                                             m_FBOVertexShader, m_FBOFragShader);
    if (m_ProgramObj == GL_NONE || m_FBOProgramObj == GL_NONE) {
        LOGCATE("FBOLegLengthenSample::Init m_ProgramObj == GL_NONE");
        return;
    }
    m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
    m_FBOSamplerLoc = glGetUniformLocation(m_FBOProgramObj, "s_TextureMap");

    // Generate VBO Ids and load the VBOs with data
    glGenBuffers(4, m_VBOIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoors), vTexCoors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vFBOTexCoors), vFBOTexCoors, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GO_CHECK_GL_ERROR();

    // Generate VAO Ids
    glGenVertexArrays(2, m_VAOIds);
    // Normal rendering VAO
    glBindVertexArray(m_VAOIds[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[1]);
    glEnableVertexAttribArray(TEXTURE_POS_INDX);
    glVertexAttribPointer(TEXTURE_POS_INDX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(GL_NONE);

    // FBO off screen rendering VAO
    glBindVertexArray(m_VAOIds[1]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
    glEnableVertexAttribArray(TEXTURE_POS_INDX);
    glVertexAttribPointer(TEXTURE_POS_INDX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(GL_NONE);

    glGenTextures(1, &m_ImageTextureId);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    GO_CHECK_GL_ERROR();

    if (!CreateFrameBufferObj()) {
        LOGCATE("FBOLegLengthenSample::Init CreateFrameBufferObj fail");
        return;
    }
}

void FBOLegLengthSample::LoadImage(NativeImage *pImage) {
    LOGCATE("FBOLegLengthSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage) {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }
}

void FBOLegLengthSample::Draw(int screenW, int screenH) {
    // 纹理就是一个“可以被采样的复杂的数据集合”，纹理作为 GPU 图像数据结构    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glViewport(0, 0, static_cast<GLsizei>(m_RenderImage.width / 0.8),
               static_cast<GLsizei>(m_RenderImage.height / 0.8));

    // Do FBO off screen rendering
    glUseProgram(m_FBOProgramObj);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId);

    glBindVertexArray(m_VAOIds[1]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
    glUniform1i(m_FBOSamplerLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, screenW, screenH);
    // Do normal rendering
    glUseProgram(m_ProgramObj);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(m_VAOIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
    glUniform1i(m_SamplerLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
}

bool FBOLegLengthSample::CreateFrameBufferObj() {
    glGenTextures(1, &m_FBOTextureId);
    glBindTexture(GL_TEXTURE_2D, m_FBOTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glGenFramebuffers(1, &m_FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId);
    glBindTexture(GL_TEXTURE_2D, m_FBOTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_FBOTextureId, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width,
                 m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("FBOLegLengthenSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}

void FBOLegLengthSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
    }

    if (m_FBOProgramObj) {
        glDeleteProgram(m_FBOProgramObj);
    }

    if (m_ImageTextureId) {
        glDeleteTextures(1, &m_ImageTextureId);
    }

    if (m_FBOTextureId) {
        glDeleteTextures(1, &m_FBOTextureId);
    }

    if (m_VBOIds[0]) {
        glDeleteBuffers(4, m_VBOIds);
    }

    if (m_VAOIds[0]) {
        glDeleteVertexArrays(2, m_VAOIds);
    }

    if (m_FBOId) {
        glDeleteFramebuffers(1, &m_FBOId);
    }
}
