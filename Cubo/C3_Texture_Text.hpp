#ifndef C3_TEXTURE_TEXT_HPP
#define C3_TEXTURE_TEXT_HPP

#include <iostream>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <ft2build.h>
//#include <FTGL/
#include <string>
#include FT_FREETYPE_H
//https://learnopengl.com/In-Practice/Text-Rendering
using namespace std;

struct ivec2{
    float x; float y;
    ivec2(float x,float y):x(x),y(y){}
    ivec2():x(0.0),y(0.0){}
};

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    //glm::ivec2 Size;    // Size of glyph
    //glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    ivec2 Size;    // Size of glyph
    ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

class C3_Texture_Text
{
public:
    C3_Texture_Text(string font="/usr/share/fonts/truetype/freefont/FreeMono.ttf", int hight_pixel_size=100);
    std::map<GLchar, Character> Characters;
    int hight_pixel_size;
private:
  GLuint texture;

  //GLfloat max_aniso;
};


class C3_Font{
    static std::map<string, C3_Texture_Text*> mapFont;
public:
    static C3_Texture_Text* add(string file,int size=100);
};



#endif // C3_TEXTURE_TEXT_HPP
