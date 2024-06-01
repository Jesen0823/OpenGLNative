//
// Created by xie_s on 2024/5/23.
//

#ifndef OPENGLNATIVE_GLUTILS_H
#define OPENGLNATIVE_GLUTILS_H

#include <GLES3/gl3.h>
#include <GLES3/gl31.h>
#include <string>

class GLUtils {
public:
    static GLuint LoadShader(GLenum shaderType, const char *pSource);

    static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource,
                                GLuint &vertexShaderHandle,
                                GLuint &fragShaderHandle);

    static void DeleteProgram(GLuint &program);

    static void CheckGLError(const char *pGLOperation);

    static GLuint loadDDS(const char *imagePath);

    static GLuint
    CreateProgramWithFeedback(const char *pVertexShaderSource, const char *pFragShaderSource,
                              GLuint &vertexShaderHandle,
                              GLuint &fragShaderHandle, const GLchar **varying, int varyingCount);
};


#endif //OPENGLNATIVE_GLUTILS_H
