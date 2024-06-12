//
// Created by xie_s on 2024/6/12.
//

#ifndef OPENGLNATIVE_TEXTRENDERSAMPLE_H
#define OPENGLNATIVE_TEXTRENDERSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <detail/type_vec2.hpp>
#include <freetype/ftglyph.h>
#include <string>
#include <map>
#include <ft2build.h>

#include "GLSampleBase.h"

// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint textureID;   // ID handle of the glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
    GLuint advance;    // Horizontal offset to advance to next glyph
};

class TextRenderSample : public GLSampleBase {

public:
    TextRenderSample();

    virtual ~TextRenderSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    /**
	 * (x,y)为屏幕坐标系的位置，即原点位于屏幕中心，x(-1.0,1.0), y(-1.0,1.0)
	 * */
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color,
                    glm::vec2 viewport);

    void LoadFacesByASCII();

    void LoadFacesByUnicode(int *unicodeArr, int size);

    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    GLuint m_VaoId;
    GLuint m_VboId;
    NativeImage m_RenderImage;
    glm::mat4 m_MVPMatrix;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;

    std::map<GLint, Character> m_Characters;

};


#endif //OPENGLNATIVE_TEXTRENDERSAMPLE_H
