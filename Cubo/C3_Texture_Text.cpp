#include "C3_Texture_Text.hpp"


C3_Texture_Text::C3_Texture_Text(string font, int hight_pixel_size)
{
    this->hight_pixel_size=hight_pixel_size;
    // FreeType
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        // Load font as face
        FT_Face face;
        if (FT_New_Face(ft, font.c_str(), 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
// /usr/share/fonts/truetype/lyx/cmr10.ttf
//        if (FT_New_Face(ft, "/usr/share/fonts/truetype/malayalam/Keraleeyam.ttf", 0, &face))
//            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        // Set size to load glyphs as

        FT_Set_Pixel_Sizes(face, 0, this->hight_pixel_size);



        glActiveTexture(GL_TEXTURE0);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Load first 255 characters of ASCII set
        for (GLubyte c = 0; c < 255; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
//            cout<<" c="<<(unsigned int)c<<endl;
//            {
//                for(int j=0;j<face->glyph->bitmap.rows;j++){
//                    for(int i=0;i<face->glyph->bitmap.width;i++){
//                       if(face->glyph->bitmap.buffer[i+j*face->glyph->bitmap.width])
//                           cout<<"*";
//                       else
//                           cout<<" ";
//                    }
//                    cout<<endl;
//                }
//            }
            glGenerateMipmap( GL_TEXTURE_2D );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            GLfloat max_aniso = 0.0f;
            glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso );
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso );
            // Now store character for later use
            Character character = {
                texture,
                ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint)face->glyph->advance.x
            };
            Characters.insert(std::pair<GLchar, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
}

std::map<std::string, C3_Texture_Text*> C3_Font::mapFont;

C3_Texture_Text* C3_Font::add(string file,int size){
    std::map<std::string, C3_Texture_Text*>::iterator it;
    it=mapFont.find(file);

    if(it!=mapFont.end()){//ya existe, entonces regresa el apuntador
        return it->second;
    }

    C3_Texture_Text* newFont=new C3_Texture_Text(file,size);
    mapFont.insert ( std::pair<string, C3_Texture_Text*>(file,newFont) );
    return newFont;
}
