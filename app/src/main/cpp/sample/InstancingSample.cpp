//
// Created by xie_s on 2024/6/1.
//

#include <vec2.hpp>
#include "InstancingSample.h"
#include "GLUtils.h"
#include "LogUtil.h"

InstancingSample::InstancingSample() {
    m_InstanceVBO = GL_NONE;
    m_VerticesVBO = GL_NONE;
    m_VAO = GL_NONE;
}

InstancingSample::~InstancingSample() {

}

void InstancingSample::Init() {
    char vShaderStr[] =
            "#version 300 es                                 \n"
            "layout(location = 0) in vec2 vPosition;         \n"
            "layout(location = 1) in vec3 color;             \n"
            "layout(location = 2) in vec2 offset;            \n"
            "out vec3 fColor;                                \n"
            "void main()                                     \n"
            "{                                               \n"
            "   float instanceID = float(gl_InstanceID);     \n"
            "   vec2 pos = vPosition * (instanceID / 100.0); \n"
            "   gl_Position = vec4(pos + offset, 0.0, 1.0);  \n"
            "   fColor = color;                              \n"
            "}                                               \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "in  vec3 fColor;                             \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vec4 (fColor, 1.0);           \n"
            "}                                            \n";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);

    GLfloat vVertices[] = {
            // Positions     // Colors
            -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
            -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

            -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.0f, 1.0f, 1.0f
    };

    // Generate a list of 100 quad locations/translation-vectors
    glm::vec2 translations[100];
    int index = 0;
    GLfloat offset = 0.1f;
    for (GLint y = -10; y < 10; y += 2) {
        for (GLint x = -10; x < 10; x += 2) {
            glm::vec2 translation;
            translation.x = (GLfloat) x / 10.0f + offset;
            translation.y = (GLfloat) y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    glGenBuffers(1, &m_InstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VerticesVBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VerticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (GLvoid *) (2 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // Tell OpenGL this is an instanced vertex attribute.
    glBindVertexArray(0);
}

void InstancingSample::Draw(int screenW, int screenH) {
    LOGCATE("InstancingSample::Draw");

    if (m_ProgramObj == 0)
        return;

    // Clear buffers
    glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(m_ProgramObj);

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
    glBindVertexArray(0);
}

void InstancingSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
    }

    if (m_InstanceVBO) {
        glDeleteBuffers(1, &m_InstanceVBO);
    }

    if (m_VerticesVBO) {
        glDeleteBuffers(1, &m_VerticesVBO);
    }

    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
    }
}
