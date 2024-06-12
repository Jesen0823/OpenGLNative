//
// Created by xie_s on 2024/5/28.
//

#include <GLUtils.h>
#include <gtc/matrix_transform.hpp>
#include "FBOLegLengthSample.h"

#define VERTEX_POS_INDX  0
#define TEXTURE_POS_INDX 1

const char vShaderStr[] =
        "#version 300 es                            \n"
        "layout(location = 0) in vec4 a_position;   \n"
        "layout(location = 1) in vec2 a_texCoord;   \n"
        "out vec2 v_texCoord;                       \n"
        "uniform mat4 u_MVPMatrix;                  \n"
        "void main()                                \n"
        "{                                          \n"
        "   gl_Position = u_MVPMatrix * a_position; \n"
        "   v_texCoord = a_texCoord;                \n"
        "}                                          \n";

const char fShaderStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform sampler2D s_TextureMap;\n"
        "void main()\n"
        "{\n"
        "    outColor = texture(s_TextureMap, v_texCoord);\n"
        "}";

const char vFboShaderStr[] =
        "#version 300 es                            \n"
        "layout(location = 0) in vec4 a_position;   \n"
        "layout(location = 1) in vec2 a_texCoord;   \n"
        "out vec2 v_texCoord;                       \n"
        "uniform mat4 u_MVPMatrix;\n"
        "void main()                                \n"
        "{                                          \n"
        "   gl_Position = u_MVPMatrix * a_position; \n"
        "   v_texCoord = a_texCoord;                \n"
        "}                                          \n";

const char fFboShaderStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform sampler2D s_TextureMap;\n"
        "void main()\n"
        "{\n"
        "    vec4 tempColor = texture(s_TextureMap, v_texCoord);\n"
        "    float luminance = tempColor.r * 0.299 + tempColor.g * 0.587 + tempColor.b * 0.114;\n"
        "    //outColor = vec4(vec3(luminance), tempColor.a);\n"
        "    outColor = tempColor;\n"
        "}";

const GLushort V_EIGHT_POINT_INDICES[] = {0, 1, 2, 0, 2, 3, 1, 4, 7, 1, 7, 2, 4, 5, 6, 4, 6, 7};
const GLushort V_SIX_POINT_INDICES[] = {0, 1, 2, 0, 2, 3, 1, 4, 5, 1, 5, 2};

const GLushort H_EIGHT_POINT_INDICES[] = {0, 1, 2, 0, 2, 3, 3, 2, 5, 3, 5, 4, 4, 5, 6, 4, 6, 7};
const GLushort H_SIX_POINT_INDICES[] = {0, 1, 2, 0, 2, 3, 3, 2, 5, 3, 5, 4,};

const GLushort FOUR_POINT_INDICES[] = {0, 1, 2, 0, 2, 3};

FBOLegLengthSample::FBOLegLengthSample() {
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

    m_StretchMode = VERTICAL_STRETCH_8_POINTS;
}

FBOLegLengthSample::~FBOLegLengthSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
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

void FBOLegLengthSample::Init() {
    m_bIsVerticalMode = true;

    RectF inRectF;
    inRectF.left = m_RenderImage.width / 2;
    inRectF.right = m_RenderImage.width / 1.5f;
    inRectF.top = 0.0f;
    inRectF.bottom = m_RenderImage.height;

    m_StretchRect.left = 0.0f;
    m_StretchRect.right = m_RenderImage.width;
    m_StretchRect.top = 0.5 * m_RenderImage.height;
    m_StretchRect.bottom = m_RenderImage.height;

    if (m_bIsVerticalMode) {
        if (m_StretchRect.top == 0 && m_StretchRect.bottom == 1.0f) {
            m_StretchMode = VERTICAL_STRETCH_4_POINTS;
        } else if (m_StretchRect.top == 0.0f) {
            m_StretchMode = VERTICAL_STRETCH_TOP_6_POINTS;
        } else if (m_StretchRect.bottom == 1.0f) {
            m_StretchMode = VERTICAL_STRETCH_BOTTOM_6_POINTS;
        } else {
            m_StretchMode = VERTICAL_STRETCH_8_POINTS;
        }
    } else {
        if (m_StretchRect.left == 0 && m_StretchRect.right == 1.0f) {
            m_StretchMode = HORIZONTAL_STRETCH_4_POINTS;
        } else if (m_StretchRect.left == 0.0f) {
            m_StretchMode = HORIZONTAL_STRETCH_LEFT_6_POINTS;
        } else if (m_StretchRect.right == 1.0f) {
            m_StretchMode = HORIZONTAL_STRETCH_RIGHT_6_POINTS;
        } else {
            m_StretchMode = HORIZONTAL_STRETCH_8_POINTS;
        }
    }

    if (m_dt <= -0.2) {
        m_isGo = true;
    }

    if (m_dt >= 0.2) {
        m_isGo = false;
    }

    if (m_isGo) {
        m_dt += 0.01;
    } else {
        m_dt -= 0.01;
    }

    float y1 = 1 - 2 * m_StretchRect.top;
    float y2 = 1 - 2 * m_StretchRect.bottom;
    float x1 = 2 * m_StretchRect.left - 1;
    float x2 = 2 * m_StretchRect.right - 1;

    float wbl = m_RenderImage.width * 1.0f / m_RenderImage.height;
    float hbl = 1 / wbl;
    if (wbl > hbl) {
        wbl = 1.0f;
    } else {
        hbl = 1.0f;
    }

    /**vertical 椤剁偣鍧愭爣*/
    GLfloat vVertices[] = {
            -0.8f * wbl, 0.8f * hbl + m_dt * 0.8f, 0.0f,
            -0.8f * wbl, -0.8f * hbl - m_dt * 0.8f, 0.0f,
            0.8f * wbl, -0.8f * hbl - m_dt * 0.8f, 0.0f,
            0.8f * wbl, 0.8f * hbl + m_dt * 0.8f, 0.0f,
    };

    /**horizontal 椤剁偣鍧愭爣*/
    GLfloat vHVertices[] = {
            -0.8f * wbl - m_dt * 0.8f, 0.8f * hbl, 0.0f,
            -0.8f * wbl - m_dt * 0.8f, -0.8f * hbl, 0.0f,
            0.8f * wbl + m_dt * 0.8f, -0.8f * hbl, 0.0f,
            0.8f * wbl + m_dt * 0.8f, 0.8f * hbl, 0.0f,
    };
//	GLfloat vHVertices[] = {
//			-0.8f   - m_dt*0.8f,   0.8f, 0.0f,
//			-0.8f   - m_dt*0.8f,  -0.8f, 0.0f,
//			0.8f  + m_dt*0.8f,  -0.8f, 0.0f,
//			0.8f  + m_dt*0.8f,   0.8f, 0.0f,
//	};

    //姝ｅ父绾圭悊鍧愭爣
    GLfloat vTexCoors[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };


    /** 8 points vertical*/
    GLfloat vEightPointsFBOVertices[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, y1 + m_dt, 0.0f,
            1.0f, y1 + m_dt, 0.0f,
            1.0f, 1.0f, 0.0f,

            -1.0f, y2 - m_dt, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, y2 - m_dt, 0.0f,
    };

    //fbo 绾圭悊鍧愭爣
    GLfloat vEightPointsFBOTexCoors[] = {
            0.0f, 0.0f,
            0.0f, m_StretchRect.top,
            1.0f, m_StretchRect.top,
            1.0f, 0.0f,

            0.0f, m_StretchRect.bottom,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, m_StretchRect.bottom,
    };

    /** 8 points horizontal*/
    GLfloat vHEightPointsFBOVertices[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            x1 - m_dt, -1.0f, 0.0f,
            x1 - m_dt, 1.0f, 0.0f,

            x2 + m_dt, 1.0f, 0.0f,
            x2 + m_dt, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
    };

    //fbo 绾圭悊鍧愭爣
    GLfloat vHEightPointsFBOTexCoors[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            m_StretchRect.left, 1.0f,
            m_StretchRect.left, 0.0f,

            m_StretchRect.right, 0.0f,
            m_StretchRect.right, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };


    /** 6 points vertical top == 0 **/
    GLfloat vSixPointsTopFBOVertices[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, y2 - 2 * m_dt, 0.0f,
            1.0f, y2 - 2 * m_dt, 0.0f,
            1.0f, 1.0f, 0.0f,

            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,

    };

    //fbo 绾圭悊鍧愭爣
    GLfloat vSixPointsTopFBOTexCoors[] = {
            0.0f, 0.0f,
            0.0f, m_StretchRect.bottom,
            1.0f, m_StretchRect.bottom,
            1.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
    };

    /** 6 points horizontal left == 0 **/
    GLfloat vHSixPointsTopFBOVertices[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            x2 + 2 * m_dt, -1.0f, 0.0f,
            x2 + 2 * m_dt, 1.0f, 0.0f,

            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,

    };

    //fbo 绾圭悊鍧愭爣
    GLfloat vHSixPointsTopFBOTexCoors[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            m_StretchRect.right, 1.0f,
            m_StretchRect.right, 0.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
    };

    /** 6 points vertical bottom == height*/
    GLfloat vSixPointsBottomFBOVertices[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, y1 + 2 * m_dt, 0.0f,
            1.0f, y1 + 2 * m_dt, 0.0f,
            1.0f, 1.0f, 0.0f,

            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
    };

    //fbo 绾圭悊鍧愭爣
    GLfloat vSixPointsBottomFBOTexCoors[] = {
            0.0f, 0.0f,
            0.0f, m_StretchRect.top,
            1.0f, m_StretchRect.top,
            1.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
    };

    /** 6 points horizontal right == width*/
    GLfloat vHSixPointsBottomFBOVertices[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            x1 - 2 * m_dt, -1.0f, 0.0f,
            x1 - 2 * m_dt, 1.0f, 0.0f,

            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
    };

    //fbo 绾圭悊鍧愭爣
    GLfloat vHSixPointsBottomFBOTexCoors[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            m_StretchRect.left, 1.0f,
            m_StretchRect.left, 0.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
    };

    /**4 points vertical top == 0 && bottom == height, horizontal left == 0 && right == width*/
    GLfloat vFourPointsFBOVertices[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,

    };

    //fbo 绾圭悊鍧愭爣
    GLfloat vFourPointsFBOTexCoors[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };


    if (m_FBOProgramObj) {
        if (m_bIsVerticalMode) {
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[0]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vVertices), vVertices);
        } else {
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[0]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vHVertices), vHVertices);
        }


        switch (m_StretchMode) {
            case VERTICAL_STRETCH_8_POINTS:
                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vEightPointsFBOVertices),
                                vEightPointsFBOVertices);

                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vEightPointsFBOTexCoors),
                                vEightPointsFBOTexCoors);
                break;
            case VERTICAL_STRETCH_TOP_6_POINTS:
                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vSixPointsTopFBOVertices),
                                vSixPointsTopFBOVertices);

                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vSixPointsTopFBOTexCoors),
                                vSixPointsTopFBOTexCoors);
                break;
            case VERTICAL_STRETCH_BOTTOM_6_POINTS:
                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vSixPointsBottomFBOVertices),
                                vSixPointsBottomFBOVertices);

                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vSixPointsBottomFBOTexCoors),
                                vSixPointsBottomFBOTexCoors);
                break;
            case VERTICAL_STRETCH_4_POINTS:
            case HORIZONTAL_STRETCH_4_POINTS:
                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vFourPointsFBOVertices),
                                vFourPointsFBOVertices);

                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vFourPointsFBOTexCoors),
                                vFourPointsFBOTexCoors);
                break;
            case HORIZONTAL_STRETCH_8_POINTS:
                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vHEightPointsFBOVertices),
                                vHEightPointsFBOVertices);

                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vHEightPointsFBOTexCoors),
                                vHEightPointsFBOTexCoors);
                break;
            case HORIZONTAL_STRETCH_LEFT_6_POINTS:
                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vHSixPointsTopFBOVertices),
                                vHSixPointsTopFBOVertices);

                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vHSixPointsTopFBOTexCoors),
                                vHSixPointsTopFBOTexCoors);
                break;
            case HORIZONTAL_STRETCH_RIGHT_6_POINTS:
                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vHSixPointsBottomFBOVertices),
                                vHSixPointsBottomFBOVertices);

                glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vHSixPointsBottomFBOTexCoors),
                                vHSixPointsBottomFBOTexCoors);
                break;
            default:
                LOGCATE("FBOLegLengthSample::Init don't support this mode");
                return;
        }


        if (m_FBOTextureId) {
            glDeleteTextures(1, &m_FBOTextureId);
        }

        glGenTextures(1, &m_FBOTextureId);
        glBindTexture(GL_TEXTURE_2D, m_FBOTextureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId);
        glBindTexture(GL_TEXTURE_2D, m_FBOTextureId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTextureId,
                               0);
        if (m_bIsVerticalMode) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width,
                         static_cast<GLsizei>(m_RenderImage.height * (1 + 2 * m_dt)), 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, nullptr);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         static_cast<GLsizei>(m_RenderImage.width * (1 + 2 * m_dt)),
                         m_RenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGCATE("FBOLegLengthSample::Init glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
        }
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
        return;
    }

    // Model matrix vertical mirror
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(1.0f, -1.0f, 1.0f));
    //Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));

    m_FBOMVPMatrix = Model;

    float ratio = 1080 / 1950.f;
    glm::mat4 Projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.0f, 1.0f);
    //glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
    //glm::mat4 Projection = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 0.1f, 100.0f);
    //glm::mat4 Projection = glm::perspective(45.0f,ratio, 0.1f,100.f);

    // View matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 1), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix
    Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(1.0f, 1.0f, 1.0f));
    Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));

    m_MVPMatrix = Projection * View * Model;


    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);

    m_FBOProgramObj = GLUtils::CreateProgram(vFboShaderStr, fFboShaderStr, m_FBOVertexShader,
                                             m_FBOFragShader);
    if (m_ProgramObj == GL_NONE || m_FBOProgramObj == GL_NONE) {
        LOGCATE("FBOLegLengthSample::Init m_ProgramObj == GL_NONE");
        return;
    }
    m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
    m_FBOSamplerLoc = glGetUniformLocation(m_FBOProgramObj, "s_TextureMap");

    m_MVPMatLoc = glGetUniformLocation(m_ProgramObj, "u_MVPMatrix");
    m_FBOMVPMatLoc = glGetUniformLocation(m_FBOProgramObj, "u_MVPMatrix");

    // Generate VBO Ids and load the VBOs with data
    if (m_bIsVerticalMode) {
        glGenBuffers(6, m_VBOIds);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_DYNAMIC_DRAW);
    } else {
        glGenBuffers(6, m_VBOIds);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vHVertices), vHVertices, GL_DYNAMIC_DRAW);
    }


    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoors), vTexCoors, GL_STATIC_DRAW);

    switch (m_StretchMode) {
        case VERTICAL_STRETCH_8_POINTS:
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vEightPointsFBOTexCoors), vEightPointsFBOTexCoors,
                         GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(V_EIGHT_POINT_INDICES),
                         V_EIGHT_POINT_INDICES, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vEightPointsFBOVertices), vEightPointsFBOVertices,
                         GL_DYNAMIC_DRAW);

            break;
        case VERTICAL_STRETCH_TOP_6_POINTS:
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vSixPointsTopFBOTexCoors),
                         vSixPointsTopFBOTexCoors, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(V_SIX_POINT_INDICES), V_SIX_POINT_INDICES,
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vSixPointsTopFBOVertices),
                         vSixPointsTopFBOVertices, GL_DYNAMIC_DRAW);

            break;
        case VERTICAL_STRETCH_BOTTOM_6_POINTS:
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vSixPointsBottomFBOTexCoors),
                         vSixPointsBottomFBOTexCoors, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(V_SIX_POINT_INDICES), V_SIX_POINT_INDICES,
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vSixPointsBottomFBOVertices),
                         vSixPointsBottomFBOVertices, GL_DYNAMIC_DRAW);
            break;
        case VERTICAL_STRETCH_4_POINTS:
        case HORIZONTAL_STRETCH_4_POINTS:
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vFourPointsFBOTexCoors), vFourPointsFBOTexCoors,
                         GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FOUR_POINT_INDICES), FOUR_POINT_INDICES,
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vFourPointsFBOVertices), vFourPointsFBOVertices,
                         GL_DYNAMIC_DRAW);
            break;
        case HORIZONTAL_STRETCH_8_POINTS:
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vHEightPointsFBOTexCoors),
                         vHEightPointsFBOTexCoors, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(H_EIGHT_POINT_INDICES),
                         H_EIGHT_POINT_INDICES, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vHEightPointsFBOVertices),
                         vHEightPointsFBOVertices, GL_DYNAMIC_DRAW);

            break;
        case HORIZONTAL_STRETCH_LEFT_6_POINTS:
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vHSixPointsTopFBOTexCoors),
                         vHSixPointsTopFBOTexCoors, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(H_SIX_POINT_INDICES), H_SIX_POINT_INDICES,
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vHSixPointsTopFBOVertices),
                         vHSixPointsTopFBOVertices, GL_DYNAMIC_DRAW);

            break;
        case HORIZONTAL_STRETCH_RIGHT_6_POINTS:
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vHSixPointsBottomFBOTexCoors),
                         vHSixPointsBottomFBOTexCoors, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(H_SIX_POINT_INDICES), H_SIX_POINT_INDICES,
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vHSixPointsBottomFBOVertices),
                         vHSixPointsBottomFBOVertices, GL_DYNAMIC_DRAW);
            break;

        default:
            LOGCATE("FBOLegLengthSample::Init don't support this mode");
            return;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FOUR_POINT_INDICES), FOUR_POINT_INDICES,
                 GL_STATIC_DRAW);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[5]);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(GL_NONE);


    // FBO off screen rendering VAO
    glBindVertexArray(m_VAOIds[1]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOIds[4]);
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
        LOGCATE("FBOLegLengthSample::Init CreateFrameBufferObj fail");
        return;
    }
}

void FBOLegLengthSample::Draw(int screenW, int screenH) {
    LOGCATE("FBOLegLengthSample::Draw [screenW, screenH] = [%d, %d]", screenW, screenH);
    //绾圭悊灏辨槸涓€涓ª鈥滃彲浠ヨ¢«閲囨牱鐨勫¤嶆潅鐨勬暟鎹®闆嗗悎鈥 绾圭悊浣滀负 GPU 鍥惧儚鏁版嵁缁撴瀯
    //glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    if (m_bIsVerticalMode) {
        glViewport(0, 0, static_cast<GLsizei>(m_RenderImage.width),
                   static_cast<GLsizei>(m_RenderImage.height * (1 + 2 * m_dt)));
    } else {
        glViewport(0, 0, static_cast<GLsizei>(m_RenderImage.width * (1 + 2 * m_dt)),
                   static_cast<GLsizei>(m_RenderImage.height));
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Do FBO off screen rendering
    glUseProgram(m_FBOProgramObj);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId);

    glBindVertexArray(m_VAOIds[1]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
    glUniform1i(m_FBOSamplerLoc, 0);
    glUniformMatrix4fv(m_FBOMVPMatLoc, 1, GL_FALSE, &m_FBOMVPMatrix[0][0]);
    GO_CHECK_GL_ERROR();
    GLsizei indicesNum;
    switch (m_StretchMode) {
        case VERTICAL_STRETCH_8_POINTS:
        case HORIZONTAL_STRETCH_8_POINTS:
            indicesNum = sizeof(V_EIGHT_POINT_INDICES) / sizeof(GLushort);
            break;
        case VERTICAL_STRETCH_TOP_6_POINTS:
        case VERTICAL_STRETCH_BOTTOM_6_POINTS:
        case HORIZONTAL_STRETCH_LEFT_6_POINTS:
        case HORIZONTAL_STRETCH_RIGHT_6_POINTS:
            indicesNum = sizeof(V_SIX_POINT_INDICES) / sizeof(GLushort);
            break;
        case VERTICAL_STRETCH_4_POINTS:
        case HORIZONTAL_STRETCH_4_POINTS:
            indicesNum = sizeof(FOUR_POINT_INDICES) / sizeof(GLushort);
            break;
        default:
            break;
    }
    glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, screenW, screenH);
    // Do normal rendering
    glUseProgram(m_ProgramObj);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(m_VAOIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FBOTextureId);
    glUniform1i(m_SamplerLoc, 0);
    glUniformMatrix4fv(m_MVPMatLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
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
        glDeleteBuffers(6, m_VBOIds);
    }

    if (m_VAOIds[0]) {
        glDeleteVertexArrays(2, m_VAOIds);
    }

    if (m_FBOId) {
        glDeleteFramebuffers(1, &m_FBOId);
    }
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
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTextureId, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("FBOLegLengthSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;

}