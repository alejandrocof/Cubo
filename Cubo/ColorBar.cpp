#include "ColorBar.hpp"

//ColorBar::precision=4;

void ColorBar::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("ColorMap")==0 ){
            std::string IdName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "getId", IdName);
            }
            //DrawableIdentifier::data(name,cm);

            //ColorMap *color_map;//=DrawableIdentifier::data(IdName,color_map2);
            //std::cout<<"direccion en ColorBar antes de asignar:"<<color_map<<std::endl;
            //std::cout<<"get Id="<<IdName<<" de tipo:"<<typeid(color_map).name()<<std::endl;
            DrawableIdentifier::data(IdName,color_map);
            //std::cout<<"direccion en ColorBar:"<<color_map<<std::endl;
            //std::cout<<"**get Id="<<IdName<<" de tipo:"<<typeid(color_map).name()<<std::endl;
            //(*color_map).lutColor.size();
        }

        if( nodeName.compare("Position")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "x", x);
                ifTheAttributeIsFoundAssign(ait, "y", y);
            }
        }

        if( nodeName.compare("Width")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "width", width);
            }
        }

        if( nodeName.compare("High")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "high", high);
            }
        }

        if( nodeName.compare("PrecisionValues")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "precision", precision);
            }
        }

        if( nodeName.compare("TextColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", textColorR);
                ifTheAttributeIsFoundAssign(ait, "g", textColorG);
                ifTheAttributeIsFoundAssign(ait, "b", textColorB);
                ifTheAttributeIsFoundAssign(ait, "a", textColorA);
            }
        }

        if( nodeName.compare("TextSize")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "size", textSize);
            }
        }

        if( nodeName.compare("TextWidth")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textWidth", textWidth);
            }
        }

        if( nodeName.compare("Title")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "title", titleText);
                replaceAll(titleText,"á","a`");
                replaceAll(titleText,"é","e`");
                replaceAll(titleText,"í","i`");
                replaceAll(titleText,"ó","o`");
                replaceAll(titleText,"ú","u`");
                replaceAll(titleText,"ñ","n~");
                replaceAll(titleText,"Á","A'");
                replaceAll(titleText,"É","E'");
                replaceAll(titleText,"Í","I'");
                replaceAll(titleText,"Ó","O'");
                replaceAll(titleText,"Ú","U'");
                replaceAll(titleText,"Ñ","N~");
            }
        }

        if( nodeName.compare("TitleTextColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", titleTextColorR);
                ifTheAttributeIsFoundAssign(ait, "g", titleTextColorG);
                ifTheAttributeIsFoundAssign(ait, "b", titleTextColorB);
                ifTheAttributeIsFoundAssign(ait, "a", titleTextColorA);
            }
        }

        if( nodeName.compare("TitleTextSize")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "size", titleTextSize);
            }
        }

        if( nodeName.compare("TitleTextWidth")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "titleTextWidth", titleTextWidth);
            }
        }

        if( nodeName.compare("Texture")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "filter", filter);
            }
        }

        std::cout <<"/>"<< std::endl;
    }
}

void ColorBar::draw(){

    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);

    glLineWidth(1);

    int size=color_map->lutColor.size()/3;

//    for(int i=0; i<size; i++ ){
//        //glColor3f(color_map->lutColor[3*i], color_map->lutColor[3*i+1], color_map->lutColor[3*i+2]);
//        glColor3ub(color_map->lutColor[3*i], color_map->lutColor[3*i+1], color_map->lutColor[3*i+2]);
//        glVertex3f( x, y+high*(float)i/(size-1),0.0f);
//        glVertex3f( x+width, y+high*(float)i/(size-1),0.0f);
//    }


    if( color_map->label.size()!=0 ){
        for(int i=0; i<color_map->label.size(); i++ ){
            glBegin(GL_QUADS );

            GLubyte r,g,b;
            color_map->transferColorN((float)i/(color_map->label.size()-1), r, g, b);
            glColor3ub(r,g,b);

            glVertex3f( x, y+high*(float)i/(color_map->label.size()-1)-0.5*width,0.0f);
            glVertex3f( x+width, y+high*(float)i/(color_map->label.size()-1)-0.5*width,0.0f);
            glVertex3f( x+width, y+high*(float)i/(color_map->label.size()-1)+0.5*width,0.0f);
            glVertex3f( x, y+high*(float)i/(color_map->label.size()-1)+0.5*width,0.0f);
            glEnd();
        }
    }
    else{
        if( filter.compare("LINEAR")==0 ){
            glBegin(GL_QUAD_STRIP);
            for(int i=0; i<size; i++ ){
                glColor3ub(color_map->lutColor[3*i], color_map->lutColor[3*i+1], color_map->lutColor[3*i+2]);
                glVertex3f( x, y+high*(float)i/(size-1),0.0f);
                glVertex3f( x+width, y+high*(float)i/(size-1),0.0f);
                //cout<<x<<","<<y<<" ";
            }
            glEnd();
            //cout<<endl;
        }

        if( filter.compare("NEAREST")==0 ){
            for(int i=0; i<size; i++ ){
                glBegin(GL_QUAD_STRIP);
                //glColor3f(color_map->lutColor[3*i], color_map->lutColor[3*i+1], color_map->lutColor[3*i+2]);
                glColor3ub(color_map->lutColor[3*i], color_map->lutColor[3*i+1], color_map->lutColor[3*i+2]);
                glVertex3f( x, y+high*(float)i/(size),0.0f);
                glVertex3f( x+width, y+high*(float)i/(size),0.0f);
                glVertex3f( x, y+high*(float)(i+1)/(size),0.0f);
                glVertex3f( x+width, y+high*(float)(i+1)/(size),0.0f);
                glEnd();

                //cout<<x<<","<<y<<" ";
            }
            //cout<<endl;
        }
    }




    glLineWidth(textWidth);
    glColor4ub(textColorR,textColorG,textColorB,textColorA);
    int wMaxString=0;
//    for(int i=0; i<size; i++ ){
//        std::stringstream text;
//        text << std::fixed<< std::setprecision(precision) << color_map->minimumValue + (color_map->maximumValue-color_map->minimumValue)*i/(size-1);
//        int wString=0;
//        int hString=119;

//        for (unsigned int j=0; j<text.str().length(); j++)
//            wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
//        if(wMaxString<wString)wMaxString=wString;

//        glPushMatrix();
//        glTranslatef(x, y+high*(float)i/(size-1),0.0f);
//        glScalef(textSize/hString,textSize/hString,textSize/hString);
//        glTranslatef(-wString,-0.5*hString,0.0f);
//        for (unsigned int i=0; i<text.str().length(); i++)
//            glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[i]);
//        glPopMatrix();
//    }


    if( color_map->label.size()!=0 ){
        for(int i=0; i<color_map->label.size(); i++ ){
            std::stringstream text;
            text << color_map->label[i];
            int wString=0;
            int hString=119;



            glPushMatrix();
            glTranslatef(x, y+high*(float)i/(color_map->label.size()-1),0.0f);
            float scale=(float)4.0*high/size;
            glScalef(textSize*scale/hString,textSize*scale/hString,textSize*scale/hString);

            for (unsigned int j=0; j<text.str().length(); j++){
                float sw=1.0;
                if(text.str().c_str()[j]==' ')wString-=0.5*hString;
                wString+=sw*glutStrokeWidth(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
            }
            if(wMaxString<wString)wMaxString=wString;

            glTranslatef(-wString,-0.5*hString,0.0f);
            for (unsigned int i=0; i<text.str().length(); i++){
                if( text.str().c_str()[i]==' '){
                    glTranslatef(-0.5*hString,0.0, 0.0f);
                }
                glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[i]);
            }

            glPopMatrix();
        }
    }
    else{
        for(int i=0; i<size; i++ ){
            std::stringstream text;
            text << std::fixed<< std::setprecision(precision) << color_map->minimumValue + (color_map->maximumValue-color_map->minimumValue)*i/(size-1);
            int wString=0;
            int hString=119;

            for (unsigned int j=0; j<text.str().length(); j++)
                wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
            if(wMaxString<wString)wMaxString=wString;

            glPushMatrix();
            glTranslatef(x, y+high*(float)i/(size-1),0.0f);
            float scale=(float)4.0*high/size;
            glScalef(textSize*scale/hString,textSize*scale/hString,textSize*scale/hString);
            glTranslatef(-wString,-0.5*hString,0.0f);
            for (unsigned int i=0; i<text.str().length(); i++)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[i]);
            glPopMatrix();
        }

    }


    glLineWidth(titleTextWidth);
    glColor4ub(titleTextColorR,titleTextColorG,titleTextColorB,titleTextColorA);
    int wString=0;
    int hString=119;
    for (unsigned int j=0; j<titleText.length(); j++)
        wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, titleText.c_str()[j])+textWidth;

    glPushMatrix();
    glTranslatef(x, y+0.5*high,0.0f);
    //glScalef(titleTextSize/hString,titleTextSize/hString,titleTextSize/hString);
    float scale=(float)4.0*high/size;
    glScalef(textSize*scale/hString,textSize*scale/hString,textSize*scale/hString);
    //glTranslatef(-wMaxString,0.0,0.0f);
    glRotatef( 90.0,0.0,0.0,1.0);
    //glTranslatef(-0.5*wString,-0.5*hString,0.0f);
    glTranslatef(0.0,1.05*wMaxString,0.0f);
    glScalef(titleTextSize/(textSize*scale),titleTextSize/(textSize*scale),titleTextSize/(textSize*scale));
    glTranslatef(-0.5*wString,0.0,0.0f);
    //glTranslatef(-wString-wMaxString,0.0,0.0f);
    for (unsigned int i=0; i<titleText.length(); i++){
        glTranslatef(textWidth,0.0,0.0);
        if(titleText.c_str()[i]=='`'){
            glPushMatrix();
            glTranslatef(0.0,0.2*hString,0.0);
            glScalef(-0.3,0.8,1.0);
            glutStrokeCharacter(GLUT_STROKE_ROMAN, titleText.c_str()[i]);
            glPopMatrix();
        }
        else{
            if(titleText.c_str()[i]=='\''){
                glPushMatrix();
                glTranslatef(0.0,0.5*hString,0.0);
                glScalef(-0.3,0.8,1.0);
                glutStrokeCharacter(GLUT_STROKE_ROMAN, '`');
                glPopMatrix();
            }
            else{
                if(titleText.c_str()[i]=='~'){
                    glPushMatrix();
                    glTranslatef(-0.5*hString,0.55*hString,0.0);
                    glScalef(0.5,0.5,1.0);
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, titleText.c_str()[i]);
                    glPopMatrix();
                }
                else{
                    if(titleText.c_str()[i]=='^'){
                        i++;
                        glPushMatrix();
                        glTranslatef(0.0, 0.5*hString, 0.0f);
                        glScalef(0.5,0.5,1.0);
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, titleText.c_str()[i]);
                        glPopMatrix();
                        glTranslatef(0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, titleText.c_str()[i]),0.0,0.0f);

                    }
                    else{
                        if(titleText.c_str()[i]=='_'){
                            i++;
                            glPushMatrix();
                            glTranslatef(0.0, -0.25*hString, 0.0f);
                            glScalef(0.5,0.5,1.0);
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, titleText.c_str()[i]);
                            glPopMatrix();
                            glTranslatef(0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, titleText.c_str()[i]),0.0,0.0f);

                        }
                        else{
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, titleText.c_str()[i]);
                        }
                    }
                }
            }
        }
    }
    glPopMatrix();


    //glDisable(GL_LINE_SMOOTH);
    //glDisable (GL_BLEND);
}


void ColorBar::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
