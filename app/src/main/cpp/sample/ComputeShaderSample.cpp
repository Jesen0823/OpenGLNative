//
// Created by xie_s on 2024/6/18.
//

#include "GLUtils.h"
#include "ComputeShaderSample.h"

void ComputeShaderSample::Init() {
    char fComputeShaderStr[] =
        "#version 310 es                                                   \n"
        "layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in; \n"
        "layout (std430, binding = 0) buffer DataBuffer {                  \n"
        "    float data[];                                                 \n"
        "} buffer1;                                                        \n"
        "void main() {                                                     \n"
        "    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);                  \n"
        "    buffer1.data[pos.y * int(gl_NumWorkGroups.x) + pos.x] *= float(pos.y); \n"
        "}";

    // Load the shaders and get a linked program object
    GLUtils::LoadComputeShader(fComputeShaderStr);
    glGenBuffers(1, &m_DataBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DataBuffer);
    float data[2][4] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    m_DataSize = sizeof(data);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), data, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_DataBuffer);
}

void ComputeShaderSample::LoadImage(NativeImage *pImage) {
    LOGCATE("ComputeShaderSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage) {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }
}

void ComputeShaderSample::Draw(int screenW, int screenH) {
    LOGCATE("ComputeShaderSample::Draw()");
    if (m_ProgramObj == GL_NONE) return;

    // Use the program object
    glUseProgram(m_ProgramObj);

    //2x4
    int numGroupX = 2;
    int numGroupY = 4;
    glDispatchCompute(numGroupX, numGroupY, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // 读取并打印处理后的数据
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DataBuffer);
    auto *mappedData = (float *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_DataSize,
                                                  GL_MAP_READ_BIT);
    LOGCATE("ComputeShaderSample::Draw() Data after compute shader:\n");
    for (int i = 0; i < m_DataSize / sizeof(float); ++i) {
        LOGCATE("ComputeShaderSample::Draw() => %f", mappedData[i]);
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void ComputeShaderSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        glDeleteBuffers(1, &m_DataBuffer);
        m_ProgramObj = GL_NONE;
    }
}
