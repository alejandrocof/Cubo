#ifndef C3_GL_TEXT_HPP
#define C3_GL_TEXT_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <codecvt>

#include "ShaderHandling.hpp"
#include"C3_Texture_Text.hpp"
//https://learnopengl.com/In-Practice/Text-Rendering
using namespace std;

enum DIRECTION{DIRECTION_HORIZONTAL,DIRECTION_ROTATE90,DIRECTION_ROTATE270,DIRECTION_STACKED};
enum HORIZONTAL_ALIGN{HORIZONTAL_ALIGN_CENTER,HORIZONTAL_ALIGN_LEFT,HORIZONTAL_ALIGN_RIGHT};
enum VERTICAL_ALIGN{VERTICAL_ALIGN_MIDDLE,VERTICAL_ALIGN_TOP,VERTICAL_ALIGN_BOTTOM,VERTICAL_ALIGN_TOP_BASELINE};

struct TextOptions{
    GLint r=255,g=0,b=0;
    GLfloat x=0,y=0,scale=1.0;
    DIRECTION dir=DIRECTION_HORIZONTAL;
    HORIZONTAL_ALIGN h_align=HORIZONTAL_ALIGN_RIGHT;
    VERTICAL_ALIGN v_align=VERTICAL_ALIGN_TOP;
    C3_Texture_Text* fontTexture;
    //string stext="";
    wstring wtext;
    int width=0;
    int hight_bearing=0;
    int hight_bearing_low=0;
};

class C3_GL_Text
{
    ShaderHandling sh;
    GLuint shaderProgram;

    GLint textureAttribute;
    GLint positionAttribute;

    GLint textColor_location;
    GLint tex_Text_location;
    GLuint VAO, VBO;
    map<std::string,GLint> attribLocation;

    int window_width,window_hight;

    C3_Texture_Text* fontTexture;
    GLint r=255,g=0,b=0;
    GLfloat x=0,y=0,scale=1.0;
    DIRECTION dir=DIRECTION_HORIZONTAL;
    HORIZONTAL_ALIGN h_align=HORIZONTAL_ALIGN_RIGHT;
    VERTICAL_ALIGN v_align=VERTICAL_ALIGN_TOP;
    wstring wtext;
    int width=0;
    int hight_bearing=0;
    int hight_bearing_low=0;
    vector<TextOptions> vTxt;


    wstring FromUTF8(const char* str);
public:
    C3_GL_Text(int window_width, int window_hight);
    void draw();
    void colorT(GLint r, GLint g, GLint b);
    void scaleT(GLfloat scale);
    void postionT(GLfloat x, GLfloat y);
    void directionT(DIRECTION dir);
    void horizontalAlign(HORIZONTAL_ALIGN h_align);
    void verticalAlign(VERTICAL_ALIGN v_align);
    void text(string tex);
    void push_text(string tex);
    //void font(C3_Texture_Text* fontText);
    void font(C3_Texture_Text* fontText);
};

#endif // C3_GL_TEXT_HPP
