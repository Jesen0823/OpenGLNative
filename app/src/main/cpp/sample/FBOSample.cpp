//
// Created by xie_s on 2024/5/25.
//

#include "FBOSample.h"
#include "LogUtil.h"

#define VERTEX_POS_INDX 0
#define TEXTURE_POS_INDX 1

FBOSample::FBOSample() {
    m_ImageTextureId = GL_NONE;
    m_FBOTextureId = GL_NONE;
    m_SamplerLoc = GL_NONE;
    m_FBOId = GL_NONE;
    m_FBOProgramObj = GL_NONE;
    m_FBOVertexShader = GL_NONE;
    m_FBOFragShader = GL_NONE;
    m_FBOSamplerLoc = GL_NONE;
}

FBOSample::~FBOSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void FBOSample::Init() {
    // 顶点坐标
    GLfloat vVertices[] = {
            -1.0f, -1.0f, 0.0f, // bottom left
            1.0f, -1.0f, 0.0f, // bottom right
            -1.0f, 1.0f, 0.0f, // top left
            1.0f, 1.0f, 0.0f, // top right
    };
    // 正常纹理坐标
    GLfloat vTexCoors[] = {
            0.0f, 1.0f, // bottom left
            1.0f, 1.0f, // bottom right
            0.0f, 0.0f, // top left
            1.0f, 0.0f, // top right
    };
    // FBO纹理坐标与正常纹理方向不同，原点位于左下角
    GLfloat vFboTexCoors[] = {
            0.0f, 0.0f,  // bottom left
            1.0f, 0.0f,  // bottom right
            0.0f, 1.0f,  // top left
            1.0f, 1.0f,  // top right
    };

    GLushort indices[] = {0, 1, 2, 1, 3, 2};

    char vShaderStr[] =
            "#version 300 es                           \n"
            "layout(location = 0) in vec4 a_position;  \n"
            "layout(location = 1) in vec2 a_texCoord;  \n"
            "out vec2 v_texCoord;                      \n"
            "void main(){                              \n"
            "   gl_Position = a_position;              \n"
            "   v_texCoord = a_texCoord;               \n"
            "}                                         \n";
    // 用于普通渲染的片段着色器脚本，简单纹理映射
    char fShaderStr[] =
            "#version 300 es                                          \n"
            "precision mediump float;                                 \n"
            "in vec2 v_texCoord;                                      \n"
            "layout(location = 0) out vec4 outColor;                  \n"
            "uniform sampler2D s_TextureMap;                          \n"
            "void main(){                                             \n"
            "outColor = texture(s_TextureMap, v_texCoord);        \n"
            "}                                                        \n";
    // 用于离屏渲染的片段着色器脚本，取每个像素的灰度值
    char fFBOShaderStr[] =
            "#version 300 es                                                                     \n"
            "precision mediump float;                                                            \n"
            "in vec2 v_texCoord;                                                                 \n"
            "layout(location = 0) out vec4 outColor;                                             \n"
            "uniform sampler2D s_TextureMap;                                                     \n"
            "void main(){                                                                        \n"
            "vec4 tempColor = texture(s_TextureMap, v_texCoord);                             \n"
            "float luminance = tempColor.r * 0.299 + tempColor.g * 0.587 + tempColor.b * 0.114;\n"
            "outColor = vec4(vec3(luminance), tempColor.a);                                  \n"
            "}                                                                               \n";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    m_FBOProgramObj = GLUtils::CreateProgram(vShaderStr, fFBOShaderStr, m_FBOVertexShader,
                                             m_FBOFragShader);
    if (m_ProgramObj == GL_NONE || m_FBOProgramObj == GL_NONE) {
        LOGCATE("FBOSample::Init m_ProgramObj == GL_NONE");
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vFboTexCoors), vFboTexCoors, GL_STATIC_DRAW);

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
        LOGCATE("FBOSample::Init CreateFrameBufferObj fail");
        return;
    }
}

void FBOSample::LoadImage(NativeImage *pImage) {
    LOGCATE("FBOSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage) {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }
}

void FBOSample::Draw(int screenW, int screenH) {
    // 离屏渲染
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glViewport(0, 0, m_RenderImage.width, m_RenderImage.height);

    // Do FBO off screen rendering
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId);
    glUseProgram(m_FBOProgramObj);
    glBindVertexArray(m_VAOIds[1]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
    glUniform1i(m_FBOSamplerLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

//	uint8_t *pBuffer = new uint8_t[m_RenderImage.width * m_RenderImage.height * 4];
//
//	NativeImage nativeImage = m_RenderImage;
//	nativeImage.format = IMAGE_FORMAT_RGBA;
//	nativeImage.ppPlane[0] = pBuffer;
//	FUN_BEGIN_TIME("FBO glReadPixels")
//		glReadPixels(0, 0, nativeImage.width, nativeImage.height, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
//	FUN_END_TIME("FBO cost glReadPixels")
//
//	NativeImageUtil::DumpNativeImage(&nativeImage, "/sdcard/DCIM", "NDK");
//	delete []pBuffer;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Do normal rendering
    glViewport(0, 0, screenW, screenH);
    glUseProgram(m_ProgramObj);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(m_VAOIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FBOTextureId);
    glUniform1i(m_SamplerLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
}

void FBOSample::Destroy() {
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

bool FBOSample::CreateFrameBufferObj() {
    // 创建并初始化 FBO 纹理
    glGenTextures(1, &m_FBOTextureId);
    glBindTexture(GL_TEXTURE_2D, m_FBOTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    // 创建并初始化 FBO
    glGenFramebuffers(1, &m_FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId);
    glBindTexture(GL_TEXTURE_2D, m_FBOTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTextureId, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("FBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}
