//
// Created by xie_s on 2024/5/24.
//

#include "TriangleSample.h"
#include "../util/ImageDef.h"
#include "../util/LogUtil.h"
#include "../util/GLUtils.h"

TriangleSample::TriangleSample() {

}

TriangleSample::~TriangleSample() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
    }
}

void TriangleSample::Init() {
    if(m_ProgramObj) return;

    //char vShaderStr[] =
            "#version 300 es                             \n"
            "layout(location = 0) in vec4 vPosition;     \n"
            "void main(){                                \n"
            "   gl_Position = vPosition;                 \n"
            "}                                           \n";
    char vShaderStr[] =
            "attribute vec4 vPosition;                    \n"
            "uniform mat4 vMatrix;                        \n"
            "void main(){                                 \n"
            "    gl_Position = vPosition;                 \n"
            "}";

    // 片元着色器
    char fShaderStr[] =
            "precision mediump float;                     \n"
            "uniform vec4 vColor;                         \n"
            "void main(){                                 \n"
                "gl_FragColor=vColor;                     \n"
            "}";


    //char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "void main(){                                 \n"
            "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
            "}                                            \n";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
}

void TriangleSample::Draw(int width, int height) {
    LOGCATE("TriangleSample::Draw");
    GLfloat vVertices[] = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
    };
    if (m_ProgramObj == 0) return;

    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Use the program object
    glUseProgram(m_ProgramObj);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(GL_NONE);
}

void TriangleSample::LoadImage(NativeImage *pImage) {
    //null implement
}

void TriangleSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        m_ProgramObj = GL_NONE;
    }
}
