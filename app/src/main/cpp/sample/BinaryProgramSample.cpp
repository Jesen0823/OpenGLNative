//
// Created by xie_s on 2024/6/14.
//

#include <vector>
#include "GLUtils.h"
#include "BinaryProgramSample.h"

#define VERTEX_POS_INDX  0
#define TEXTURE_POS_INDX 1

BinaryProgramSample::BinaryProgramSample() {
    m_VaoIds[0] = GL_NONE;
    m_VboIds[0] = GL_NONE;

    m_ImageTextureId = GL_NONE;
    m_FboTextureId = GL_NONE;
    m_SamplerLoc = GL_NONE;
    m_FboId = GL_NONE;
    m_FboProgramObj = GL_NONE;
    m_FboVertexShader = GL_NONE;
    m_FboFragmentShader = GL_NONE;
    m_FboSamplerLoc = GL_NONE;
}

BinaryProgramSample::~BinaryProgramSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void BinaryProgramSample::Init() {
//顶点坐标
    GLfloat vVertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
    };

    //正常纹理坐标
    GLfloat vTexCoors[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
    };

    //fbo 纹理坐标与正常纹理方向不同，原点位于左下角
    GLfloat vFboTexCoors[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
    };

    GLushort indices[] = {0, 1, 2, 1, 3, 2};

    char vShaderStr[] =
            "#version 300 es                                                                    \n"
            "layout(location = 0) in vec4 a_position;                                           \n"
            "layout(location = 1) in vec2 a_texCoord;                                           \n"
            "out vec2 v_texCoord;                                                               \n"
            "void main(){                                                                       \n"
            "   gl_Position = a_position;                                                       \n"
            "   v_texCoord = a_texCoord;                                                        \n"
            "}";

    // 用于普通渲染的片段着色器脚本，简单纹理映射
    char fShaderStr[] =
            "#version 300  es                                                                   \n"
            "precision mediump float;                                                           \n"
            "in vec2 v_texCoord;                                                                \n"
            "layout(location = 0) out vec4 outColor;                                            \n"
            "uniform sampler2D s_TextureMap;                                                    \n"
            "void main(){                                                                       \n"
            "   outColor = texture(s_TextureMap, v_texCoord);                                   \n"
            "}";

    // 用于离屏渲染的片段着色器脚本，取每个像素的灰度值
    char fFboShaderStr[] =
            "#version 300 es                                                                    \n"
            "precision mediump float;                                                           \n"
            "in vec2 v_texCoord;                                                                \n"
            "layout(location = 0) out vec4 outColor;                                            \n"
            "uniform sampler2D s_TextureMap;                                                    \n"
            "void main(){                                                                       \n"
            "    outColor = texture(s_TextureMap, v_texCoord);                                  \n"
            "}";

    //检查是否支持获取二进制 program
    GLint programBinaryFormats = 0;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &programBinaryFormats);
    if (programBinaryFormats <= GL_NONE) {
        LOGCATE("BinaryProgramSample Init(), not support binary programe.");
    }
    LOGCATE("BinaryProgramSample Init(),programBinaryFormats=%d", programBinaryFormats);

    // 编译链接用于普通渲染的着色器程序
    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);

    // 编译链接用于离屏渲染的着色器程序
    m_FboProgramObj = GLUtils::CreateProgram(vShaderStr, fFboShaderStr, m_FboVertexShader,
                                             m_FboFragmentShader);


    if (programBinaryFormats > GL_NONE) {
        GLint length = 0;
        glGetProgramiv(m_ProgramObj, GL_PROGRAM_BINARY_LENGTH, &length);

        std::vector<unsigned char> buffer(length);
        GLenum format = 0;
        glGetProgramBinary(m_ProgramObj, length, nullptr, &format, buffer.data());

        ///sdcard/Android/data/com.byteflow.app/files/Download/shader_program.bin_format_34624
        char binaryFilePath[256] = {0};
        sprintf(binaryFilePath, "%s/shader_program.bin_format_%d", DEFAULT_OGL_ASSETS_DIR, format);
        FILE *fp = fopen(binaryFilePath, "wb");
        LOGCATE("BinaryProgramSample fp=%p, file=%s, binFormat=%d", fp, binaryFilePath, format);
        if (fp) {
            fwrite(buffer.data(), length, 1, fp);
            fclose(fp);

            //删除 program 重新加载
            glDeleteProgram(m_ProgramObj);
            m_ProgramObj = GL_NONE;
        }

        LoadBinary2Program(&m_ProgramObj, binaryFilePath);
    }

    if (m_ProgramObj == GL_NONE || m_FboProgramObj == GL_NONE) {
        LOGCATE("BinaryProgramSample::Init m_ProgramObj == GL_NONE");
        return;
    }
    m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
    m_FboSamplerLoc = glGetUniformLocation(m_FboProgramObj, "s_TextureMap");

    // 生成 VBO ，加载顶点数据和索引数据
    glGenBuffers(4, m_VboIds);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoors), vTexCoors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vFboTexCoors), vFboTexCoors, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GO_CHECK_GL_ERROR();

    // 生成 2 个 VAO，一个用于普通渲染，另一个用于离屏渲染
    glGenVertexArrays(2, m_VaoIds);

    // 初始化用于普通渲染的 VAO
    glBindVertexArray(m_VaoIds[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glEnableVertexAttribArray(TEXTURE_POS_INDX);
    glVertexAttribPointer(TEXTURE_POS_INDX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);
    GO_CHECK_GL_ERROR();

    glBindVertexArray(GL_NONE);

    // 初始化用于离屏渲染的 VAO
    glBindVertexArray(m_VaoIds[1]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[2]);
    glEnableVertexAttribArray(TEXTURE_POS_INDX);
    glVertexAttribPointer(TEXTURE_POS_INDX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);
    GO_CHECK_GL_ERROR();
    
    glBindVertexArray(GL_NONE);

    // 创建并初始化图像纹理
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
        LOGCATE("BinaryProgramSample::Init CreateFrameBufferObj fail");
        return;
    }
}

void BinaryProgramSample::LoadImage(NativeImage *pImage) {
    LOGCATE("BinaryProgramSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage) {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }
}

void BinaryProgramSample::Draw(int screenW, int screenH) {
    // 离屏渲染
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glViewport(0, 0, m_RenderImage.width, m_RenderImage.height);

    // Do FBO off screen rendering
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
    glUseProgram(m_FboProgramObj);
    glBindVertexArray(m_VaoIds[1]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
    glUniform1i(m_FboSamplerLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 普通渲染
    // Do normal rendering
    glViewport(0, 0, screenW, screenH);
    glUseProgram(m_ProgramObj);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(m_VaoIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glUniform1i(m_SamplerLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
}

void BinaryProgramSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
    }

    if (m_FboProgramObj) {
        glDeleteProgram(m_FboProgramObj);
    }

    if (m_ImageTextureId) {
        glDeleteTextures(1, &m_ImageTextureId);
    }

    if (m_FboTextureId) {
        glDeleteTextures(1, &m_FboTextureId);
    }

    if (m_VboIds[0]) {
        glDeleteBuffers(4, m_VboIds);
    }

    if (m_VaoIds[0]) {
        glDeleteVertexArrays(2, m_VaoIds);
    }

    if (m_FboId) {
        glDeleteFramebuffers(1, &m_FboId);
    }
}

bool BinaryProgramSample::CreateFrameBufferObj() {
    // 创建并初始化 FBO 纹理
    glGenTextures(1, &m_FboTextureId);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    // 创建并初始化 FBO
    glGenFramebuffers(1, &m_FboId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FboTextureId, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("BinaryProgramSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}

void BinaryProgramSample::LoadBinary2Program(GLuint *pProgram, char *pBinFilePath) {
    FILE *fp = fopen(pBinFilePath, "rb");
    LOGCATE("BinaryProgramSample LoadBinary2Program fp=%p, file=%s", fp, pBinFilePath);
    if (fp) {
        fseek(fp, 0L, SEEK_END);
        int size = ftell(fp);
        rewind(fp);
        unsigned char *buffer = new unsigned char[size];
        fread(buffer, size, 1, fp);
        fclose(fp);

        *pProgram = glCreateProgram();
        std::string path(pBinFilePath);
        int pos = path.find("format_");
        std::string strFormat = path.substr(pos + 7);
        int format = std::stoi(strFormat);
        LOGCATE("BinaryProgramSample format=%d", format);
        glProgramBinary(*pProgram, format, buffer, size);

        //检查是否加载成功
        GLint status;
        glGetProgramiv(*pProgram, GL_LINK_STATUS, &status);
        if (GL_FALSE == status) {
            //加载失败
            LOGCATE("BinaryProgramSample glProgramBinary fail.");
            glDeleteProgram(*pProgram);
            *pProgram = GL_NONE;
        }

        delete[] buffer;
    }
}