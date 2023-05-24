#include "C3_GL_Text.hpp"
//https://www.khronos.org/opengl/wiki/Tutorial1:_Rendering_shapes_with_glDrawRangeElements,_VAO,_VBO,_shaders_(C%2B%2B_/_freeGLUT)
//https://www.khronos.org/opengl/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)

C3_GL_Text::C3_GL_Text(int w, int h)
{
    this->window_width=w;
    this->window_hight=h;

//    vector<string> attrib;

//    attrib.push_back("position");
//    attrib.push_back("colour");


    attribLocation["coords"]=0;
    attribLocation["texcoords"]=1;

    // Configure VAO/VBO for texture quads
    shaderProgram=0;
    sh.loadShaders("../Shaders/Text.vert","../Shaders/Text.frag",attribLocation);
    shaderProgram=sh.getProgramObject();

    //textureAttribute = glGetAttribLocation(shaderProgram, "colour");
    //positionAttribute = glGetAttribLocation(shaderProgram, "position");
    textColor_location=glGetUniformLocation(shaderProgram, "textColor");
    tex_Text_location = glGetUniformLocation(shaderProgram,"text");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(attribLocation["coords"], 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
    glVertexAttribPointer(attribLocation["texcoords"]  , 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (char*)0+2*sizeof(GLfloat));
    glEnableVertexAttribArray(attribLocation["coords"]);
    glEnableVertexAttribArray(attribLocation["texcoords"] );


//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    //wtext = FromUTF8("unused");
    wtext = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes("unused");

}





void C3_GL_Text::draw(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);



//    glVertexAttribPointer((GLuint)positionAttribute, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
//    glEnableVertexAttribArray((GLuint)positionAttribute);

//    glVertexAttribPointer((GLuint)textureAttribute  , 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (char*)0+2*sizeof(GLfloat));
//    glEnableVertexAttribArray((GLuint)textureAttribute  );


//    glVertexAttribPointer(attribLocation["position"], 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
//    glEnableVertexAttribArray(attribLocation["position"]);

//    glVertexAttribPointer(attribLocation["colour"]  , 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (char*)0+2*sizeof(GLfloat));
//    glEnableVertexAttribArray(attribLocation["colour"] );


    //glUniform3f(textColor_location,this->r/255.0,this->g/255.0,this->b/255.0);
    //glUniform3f((GLuint)textColor_location,this->r/255.0,this->g/255.0,this->b/255.0);
    //glUniform1i(tex_Text_location,0);



    // Iterate through all characters
    //std::string::const_iterator c;


    for (auto& it : vTxt) {
        glUniform3f((GLuint)textColor_location,it.r/255.0,it.g/255.0,it.b/255.0);
        GLfloat scalexw=it.scale*50.0/(1000.0*fontTexture->hight_pixel_size);
        GLfloat scaleyw=it.scale*50*window_width/(1000.0*window_hight*fontTexture->hight_pixel_size);

        GLfloat xo=0;
        GLfloat yo=0;

        switch(it.h_align){
        case HORIZONTAL_ALIGN_CENTER:
            xo=-it.width*scalexw/2;
            break;
        case HORIZONTAL_ALIGN_LEFT:
            xo=-it.width*scalexw;
            break;
        case HORIZONTAL_ALIGN_RIGHT:
            xo=0;
            break;
        }

        switch(it.v_align){
        case VERTICAL_ALIGN_MIDDLE:
            yo=-it.hight_bearing*scaleyw/2;
            break;
        case VERTICAL_ALIGN_TOP:
            yo=it.hight_bearing_low*scaleyw;
            break;
        case VERTICAL_ALIGN_TOP_BASELINE:
            yo=0;
            break;
        case VERTICAL_ALIGN_BOTTOM:
            yo=-it.hight_bearing*scaleyw;
            break;
        }

        GLfloat xtext;
        GLfloat ytext;

        std::wstring::const_iterator c;
        switch(it.dir){
        case DIRECTION_ROTATE90:
            xtext=it.x-yo;
            ytext=it.y+xo;
            for (c = it.wtext.begin(); c != it.wtext.end(); c++)
            {
                Character ch = it.fontTexture->Characters[*c];
                GLfloat xpos = xtext + (ch.Size.y - ch.Bearing.y) * scalexw;
                GLfloat ypos = ytext + ch.Bearing.x * scaleyw;
                GLfloat w = ch.Size.x * scaleyw;
                GLfloat h = ch.Size.y * scalexw;
                GLfloat vertices[16] = {
                    xpos  , ypos  ,   0.0, 1.0,
                    xpos-h  , ypos,   0.0, 0.0 ,
                    xpos-h, ypos+w,     1.0, 0.0 ,
                    xpos, ypos+w  ,   1.0, 1.0};
                ytext += (ch.Advance >> 6) * scaleyw; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                //glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
                //glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
            break;
        case DIRECTION_ROTATE270:
            xtext=it.x+yo;
            ytext=it.y-xo;
            for (c = it.wtext.begin(); c != it.wtext.end(); c++)
            {
                Character ch = it.fontTexture->Characters[*c];
                GLfloat xpos = xtext - (ch.Size.y - ch.Bearing.y) * scalexw;
                GLfloat ypos = ytext - ch.Bearing.x * scaleyw;
                GLfloat w = ch.Size.x * scaleyw;
                GLfloat h = ch.Size.y * scalexw;
                GLfloat vertices[16] = {
                    xpos  , ypos  ,   0.0, 1.0,
                    xpos+h  , ypos,   0.0, 0.0 ,
                    xpos+h, ypos-w,     1.0, 0.0 ,
                    xpos, ypos-w  ,   1.0, 1.0};
                ytext -= (ch.Advance >> 6) * scaleyw; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                //glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
                //glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
            break;
        case DIRECTION_HORIZONTAL:
            xtext=it.x+xo;
            ytext=it.y+yo;
            for (c = it.wtext.begin(); c != it.wtext.end(); c++)
            {
                Character ch = it.fontTexture->Characters[*c];
                GLfloat xpos = xtext + ch.Bearing.x * scalexw;
                GLfloat ypos = ytext - (ch.Size.y - ch.Bearing.y) * scaleyw;
                GLfloat w = ch.Size.x * scalexw;
                GLfloat h = ch.Size.y * scaleyw;
                GLfloat vertices[16] = {
                    xpos  , ypos  ,   0.0, 1.0,
                    xpos  , ypos+h,   0.0, 0.0 ,
                    xpos+w, ypos+h,     1.0, 0.0 ,
                    xpos+w, ypos  ,   1.0, 1.0};
                //cout<<xpos<<","<<ypos<<","<<w<<","<<h<<endl;
                xtext += (ch.Advance >> 6) * scalexw; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                //glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
                //glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
            //cout<<endl;

            xtext=it.x+xo;
            ytext=it.y+yo-0.5;
            for (c = it.wtext.begin(); c != it.wtext.end(); c++)
            {
                Character ch = it.fontTexture->Characters[*c];
                GLfloat xpos = xtext + ch.Bearing.x * scalexw;
                GLfloat ypos = ytext - (ch.Size.y - ch.Bearing.y) * scaleyw;
                GLfloat w = ch.Size.x * scalexw;
                GLfloat h = ch.Size.y * scaleyw;
                glBegin(GL_QUADS);
                glVertex3f( xpos  , ypos  ,0.0f);
                glVertex3f( xpos  , ypos+h,0.0f);
                glVertex3f( xpos+w, ypos+h,0.0f);
                glVertex3f( xpos+w, ypos  ,0.0f);
                glEnd();
            }

            break;

        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glDisableVertexAttribArray((GLuint)positionAttribute);
//    glDisableVertexAttribArray((GLuint)textureAttribute  );
    glDisableVertexAttribArray(attribLocation["coords"]);
    glDisableVertexAttribArray(attribLocation["texcoords"] );
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    glDisable(GL_BLEND);
}

void C3_GL_Text::colorT(int r, int g, int b){
    this->r=r;
    this->g=g;
    this->b=b;
}
void C3_GL_Text::scaleT(GLfloat scale){
    this->scale=scale;
}
void C3_GL_Text::postionT(GLfloat x, GLfloat y){
    this->x=x;
    this->y=y;
}
void C3_GL_Text::directionT(DIRECTION dir){
    this->dir=dir;
}

void C3_GL_Text::horizontalAlign(HORIZONTAL_ALIGN h_align){
    this->h_align=h_align;
}

void C3_GL_Text::verticalAlign(VERTICAL_ALIGN v_align){
    this->v_align=v_align;
}

void C3_GL_Text::font(C3_Texture_Text* fontTexture){
    this->fontTexture=fontTexture;
}

void C3_GL_Text::text(string text){
    //wtext = FromUTF8(text.c_str());
    //https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
    wtext = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(text.c_str());
    width=0;
    hight_bearing=0;
    hight_bearing_low=0;
    std::wstring::const_iterator c;
    for (c = wtext.begin(); c != wtext.end(); c++){
        Character ch = fontTexture->Characters[*c];
        width += (ch.Advance >> 6);
        if(hight_bearing<ch.Bearing.y)hight_bearing=ch.Bearing.y;
        if(hight_bearing_low<(ch.Size.y-ch.Bearing.y))hight_bearing_low=(ch.Size.y-ch.Bearing.y);
    }
}

void C3_GL_Text::push_text(string text){
    //wtext = FromUTF8(text.c_str());
    //https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
    wtext = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(text.c_str());
    width=0;
    hight_bearing=0;
    hight_bearing_low=0;
    std::wstring::const_iterator c;
    for (c = wtext.begin(); c != wtext.end(); c++){
        Character ch = fontTexture->Characters[*c];
        width += (ch.Advance >> 6);
        if(hight_bearing<ch.Bearing.y)hight_bearing=ch.Bearing.y;
        if(hight_bearing_low<(ch.Size.y-ch.Bearing.y))hight_bearing_low=(ch.Size.y-ch.Bearing.y);
    }
    TextOptions textOptTemp;
    textOptTemp.wtext=wtext;
    textOptTemp.fontTexture=fontTexture;
    textOptTemp.r=r;
    textOptTemp.g=g;
    textOptTemp.b=b;
    textOptTemp.x=x;
    textOptTemp.y=y;
    textOptTemp.scale=scale;
    textOptTemp.dir=dir;
    textOptTemp.h_align=h_align;
    textOptTemp.v_align=v_align;
    textOptTemp.width=width;
    textOptTemp.hight_bearing=hight_bearing;
    textOptTemp.hight_bearing_low=hight_bearing_low;
    vTxt.push_back(textOptTemp);
}

std::wstring C3_GL_Text::FromUTF8(const char* str)
{
    const unsigned char* s = reinterpret_cast<const unsigned char*>(str);
    static const wchar_t badchar = '?';
    std::wstring ret;
    unsigned i = 0;
    while(s[i]){
        try{
            if(s[i] < 0x80){         // 00-7F: 1 byte codepoint
                ret += s[i];
                ++i;
            }
            else if(s[i] < 0xC0)    // 80-BF: invalid for midstream
                throw 0;
            else if(s[i] < 0xE0){    // C0-DF: 2 byte codepoint
                if((s[i+1] & 0xC0) != 0x80)throw 1;
                ret +=  ((s[i  ] & 0x1F) << 6) |
                        ((s[i+1] & 0x3F));
                i += 2;
            }
            else if(s[i] < 0xF0){    // E0-EF: 3 byte codepoint
                if((s[i+1] & 0xC0) != 0x80)		throw 1;
                if((s[i+2] & 0xC0) != 0x80)		throw 2;
                wchar_t ch =
                        ((s[i  ] & 0x0F) << 12) |
                        ((s[i+1] & 0x3F) <<  6) |
                        ((s[i+2] & 0x3F));
                i += 3;
                // make sure it isn't a surrogate pair
                if((ch & 0xF800) == 0xD800)
                    ch = badchar;
                ret += ch;
            }
            else if(s[i] < 0xF8){    // F0-F7: 4 byte codepoint
                if((s[i+1] & 0xC0) != 0x80)		throw 1;
                if((s[i+2] & 0xC0) != 0x80)		throw 2;
                if((s[i+3] & 0xC0) != 0x80)		throw 3;
                unsigned long ch =
                        ((s[i  ] & 0x07) << 18) |
                        ((s[i+1] & 0x3F) << 12) |
                        ((s[i+2] & 0x3F) <<  6) |
                        ((s[i+3] & 0x3F));
                i += 4;
                // make sure it isn't a surrogate pair
                if((ch & 0xFFF800) == 0xD800)
                    ch = badchar;
                if(ch < 0x10000)	// overlong encoding -- but technically possible
                    ret += static_cast<wchar_t>(ch);
                else if(std::numeric_limits<wchar_t>::max() < 0x110000){
                    // wchar_t is too small for 4 byte code point
                    //  encode as UTF-16 surrogate pair
                    ch -= 0x10000;
                    ret += static_cast<wchar_t>( (ch >> 10   ) | 0xD800 );
                    ret += static_cast<wchar_t>( (ch & 0x03FF) | 0xDC00 );
                }
                else
                    ret += static_cast<wchar_t>(ch);
            }
            else                    // F8-FF: invalid
                throw 0;
        }
        catch(int skip){
            if(!skip){
                do{
                    ++i;
                }while((s[i] & 0xC0) == 0x80);
            }
            else
                i += skip;
        }
    }
    return ret;
}

//C3_GL_Text *GLtextA;
//    GLtextA=new C3_GL_Text(this->window_width, this->window_hight);
//    GLtextA->font(font);
//    GLtextA->colorT(255,255,255);
//    GLtextA->scaleT(1.0);
//    GLtextA->postionT(0.0,0.0);
//    GLtextA->verticalAlign(VERTICAL_ALIGN_TOP);
//    GLtextA->horizontalAlign(HORIZONTAL_ALIGN_CENTER);
//    GLtextA->directionT(DIRECTION_ROTATE90);
//    GLtextA->push_text("Aceleración (m/s²)");


//    GLtextA->scaleT(2.0);
//    GLtextA->postionT(0.0,0.0);
//    GLtextA->verticalAlign(VERTICAL_ALIGN_TOP);
//    GLtextA->horizontalAlign(HORIZONTAL_ALIGN_RIGHT);

//    GLtextA->colorT(128,0,0);
//    GLtextA->directionT(DIRECTION_HORIZONTAL);
//    //GLtextA->push_text("Horizontal");
//    GLtextA->push_text("Aspecto");

//    GLtextA->colorT(0,128,0);
//    GLtextA->directionT(DIRECTION_ROTATE90);
//    //GLtextA->push_text("Giro 90°");
//    GLtextA->push_text("Aspecto");

//    GLtextA->colorT(0,0,128);
//    GLtextA->directionT(DIRECTION_ROTATE270);
//    //GLtextA->push_text("Giro 270°");
//    GLtextA->push_text("Aspecto");

//    GLtextA->scaleT(1.0);
//    GLtextA->directionT(DIRECTION_HORIZONTAL);

//    GLtextA->colorT(128,0,0);
//    GLtextA->postionT(-0.5,0.5);
//    GLtextA->horizontalAlign(HORIZONTAL_ALIGN_LEFT);
//    GLtextA->push_text("Izquierda");

//    GLtextA->colorT(0,128,0);
//    GLtextA->postionT(-0.5,0.4);
//    GLtextA->horizontalAlign(HORIZONTAL_ALIGN_CENTER);
//    GLtextA->push_text("Centro");

//    GLtextA->colorT(0,0,128);
//    GLtextA->postionT(-0.5,0.3);
//    GLtextA->horizontalAlign(HORIZONTAL_ALIGN_RIGHT);
//    GLtextA->push_text("Derecha");

//    GLtextA->directionT(DIRECTION_ROTATE270);

//    GLtextA->colorT(128,0,0);
//    GLtextA->postionT(-0.6,-0.7);
//    GLtextA->verticalAlign(VERTICAL_ALIGN_TOP);
//    GLtextA->push_text("Arriba");

//    GLtextA->colorT(0,128,0);
//    GLtextA->postionT(-0.6,-0.4);
//    GLtextA->verticalAlign(VERTICAL_ALIGN_MIDDLE);
//    GLtextA->push_text("Medio");

//    GLtextA->colorT(0,0,128);
//    GLtextA->postionT(-0.6,-0.1);
//    GLtextA->verticalAlign(VERTICAL_ALIGN_BOTTOM);
//    GLtextA->push_text("Abajo");

//    GLtextA->verticalAlign(VERTICAL_ALIGN_TOP);
//    GLtextA->horizontalAlign(HORIZONTAL_ALIGN_RIGHT);
//    GLtextA->directionT(DIRECTION_HORIZONTAL);
//    GLtextA->font(font);
//    GLtextA->colorT(0,0,0);
//    GLtextA->postionT(0.2,-0.5);
//    GLtextA->push_text("ax²+bx+c=0");

//    display->addListDraw(&C3_GL_Text::draw,GLtextA);
