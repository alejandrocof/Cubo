#include "Label.hpp"


Label3DClass::Label3DClass(){
    textColorR=0;
    textColorG=0;
    textColorB=0;
    textColorA=255;
    x=0.0;
    y=0.0;
    z=0.0;
    textSize=1.0;
    textWidth=2.0;
    wMaxString=0.0;
    hString=119;

    backBoxColorR=128;
    backBoxColorG=128;
    backBoxColorB=128;
    backBoxColorA=128;
    backBoxBorderWidth=0.0;
    alignmentText=aCenter;

    backBoxShow="false";

}

void Label3DClass::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();
        std::string nodeName( it->name() );

        if( nodeName.compare("BackBoxColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", backBoxColorR);
                ifTheAttributeIsFoundAssign(ait, "g", backBoxColorG);
                ifTheAttributeIsFoundAssign(ait, "b", backBoxColorB);
                ifTheAttributeIsFoundAssign(ait, "a", backBoxColorA);
            }
        }

        if( nodeName.compare("BackBoxShow")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "show", backBoxShow);
            }
        }

        if( nodeName.compare("BackBoxBorderWidth")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "width", backBoxBorderWidth);
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


        if( nodeName.compare("TextWidth")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textWidth", textWidth);
            }
        }

        if( nodeName.compare("TextSize")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textSize", textSize);
            }
        }

        if( nodeName.compare("Text")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                std::string text;
                ifTheAttributeIsFoundAssignString(ait, "text", text);
                addText(text);
                //vTextSize.push_back(textSize);
            }
        }

        if( nodeName.compare("Coordinate")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", y);
                ifTheAttributeIsFoundAssign(ait, "long", x);
                ifTheAttributeIsFoundAssign(ait, "z", z);
            }
        }

        if( nodeName.compare("Position")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "x", x);
                ifTheAttributeIsFoundAssign(ait, "y", y);
                ifTheAttributeIsFoundAssign(ait, "z", z);
            }
        }

        if( nodeName.compare("Alignment")==0 ){
            std::string al;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "alignment", al);
                if(al.compare("left")==0)
                    alignmentText=aLeft;
                if(al.compare("center")==0)
                    alignmentText=aCenter;
                if(al.compare("right")==0)
                    alignmentText=aRight;

            }
        }

        if( nodeName.compare("Projection")==0 ){
            std::string IdProy;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdProy);
            }
            DrawableIdentifier::data(IdProy,proj);
        }

        std::cout <<"/>"<< std::endl;
    }

//    vector<double> xy(2);
//    xy[0]=x;
//    xy[1]=y;
////    xy[0]=x*DEG_TO_RAD;
////    xy[1]=y*DEG_TO_RAD;
//    proj->transform(xy);
//    x=xy[0];
//    y=xy[0];
    proj->transform(x,y);

    for(unsigned int i=0;i<vText.size();i++){

        int wString=0;

        for (unsigned int j=0; j<vText[i].length(); j++)
            wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, vText[i].c_str()[j])+textWidth;


        if(wMaxString<wString*vTextSize[i])wMaxString=wString*vTextSize[i];
    }
    wMaxString=wMaxString/hString;

    switch(alignmentText){
    case aLeft:
        xo=0.0;
        yo=0.0;
        break;
    case aCenter:
        xo=-wMaxString/2.0;
        yo=0.0;
        break;
    case aRight:
        xo=-wMaxString;
        yo=0.0;
        break;

    }


}

void Label3DClass::draw(){
    //cout<<"x:"<<x<<" y:"<<y<<" text:"<<vText[0]<<endl;
    //glUseProgram(0);

    //glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable (GL_BLEND);
    //glEnable(GL_LINE_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glDepthMask(false);
    //glLineWidth(lw);
    //glDrawArrays(GL_LINE_STRIP, offset, count);


    if(backBoxShow.compare("true")==0){
        glLineWidth(1);
        glBegin(GL_QUADS);
        glColor4ub(backBoxColorR,backBoxColorG,backBoxColorB,backBoxColorA);
        glVertex3f(xo+x-backBoxBorderWidth,y+backBoxBorderWidth+textSize,z);
        glVertex3f(xo+x+backBoxBorderWidth+wMaxString,y+backBoxBorderWidth+textSize,z);
        glVertex3f(xo+x+backBoxBorderWidth+wMaxString,y-backBoxBorderWidth-textSize*(vText.size()-0.7),z);
        glVertex3f(xo+x-backBoxBorderWidth,y-backBoxBorderWidth-textSize*(vText.size()-0.7),z);
        glEnd();
    }

    glLineWidth(textWidth);
    for(unsigned int i=0;i<vText.size();i++){
        glPushMatrix();
        glTranslated(xo+x,y-i*textSize,z);
        int wString=0;
        for (unsigned int j=0; j<vText[i].length(); j++){
            if(vText[i].c_str()[j]!='`')
                wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, vText[i].c_str()[j])+textWidth;
        }
        glPushMatrix();
        glColor4ub(textColorR,textColorG,textColorB,textColorA);

//        glColor4f(0.0,0.0,0.0,1.0);
//        glLineWidth(4);


        //glScaled(vTextSize[i]/hString,vTextSize[i]/hString,vTextSize[i]/hString);

        double sizeLabel=0.02*vTextSize[i]*DisplaySettings::dL()/hString;
        glScaled(sizeLabel,sizeLabel,sizeLabel);

        //glTranslatef(-0.5*wString,0.0,0.0f);
        for (unsigned int j=0; j<vText[i].length(); j++){
            glTranslatef(textWidth,0.0,0.0);
            if(vText[i].c_str()[j]=='`'){
                glPushMatrix();
                glTranslatef(0.0,0.2*hString,0.0);
                glScalef(-0.3,0.8,1.0);
                glutStrokeCharacter(GLUT_STROKE_ROMAN, vText[i].c_str()[j]);
                glPopMatrix();
            }
            else{
                if(vText[i].c_str()[j]=='\''){
                    glPushMatrix();
                    glTranslatef(0.0,0.5*hString,0.0);
                    glScalef(-0.3,0.8,1.0);
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, '`');
                    glPopMatrix();
                }
                else{
                    if(vText[i].c_str()[j]=='~'){
                        glPushMatrix();
                        glTranslatef(-0.5*hString,0.55*hString,0.0);
                        glScalef(0.5,0.5,1.0);
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, vText[i].c_str()[j]);
                        glPopMatrix();
                    }
                    else{
                        if(vText[i].c_str()[j]=='^'){
                            j++;
                            glPushMatrix();
                            glTranslatef(0.0, 0.5*hString, 0.0f);
                            glScalef(0.5,0.5,1.0);
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, vText[i].c_str()[j]);
                            glPopMatrix();
                            glTranslatef(0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, vText[i].c_str()[j]),0.0,0.0f);

                        }
                        else{
                            if(vText[i].c_str()[j]=='_'){
                                j++;
                                glPushMatrix();
                                glTranslatef(0.0, -0.25*hString, 0.0f);
                                glScalef(0.5,0.5,1.0);
                                glutStrokeCharacter(GLUT_STROKE_ROMAN, vText[i].c_str()[j]);
                                glPopMatrix();
                                glTranslatef(0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, vText[i].c_str()[j]),0.0,0.0f);

                            }
                            else{
                                glutStrokeCharacter(GLUT_STROKE_ROMAN, vText[i].c_str()[j]);
                            }
                        }
                    }
                }
            }
        }
        glPopMatrix();
        glPopMatrix();
    }
    //glDisable(GL_LINE_SMOOTH);
    glDisable (GL_BLEND);
    glDisable(GL_DEPTH_TEST);

}


void Label3DClass::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void Label3DClass::addText(std::string& str){
    replaceAll(str,"á","a`");
    replaceAll(str,"é","e`");
    replaceAll(str,"í","i`");
    replaceAll(str,"ó","o`");
    replaceAll(str,"ú","u`");
    replaceAll(str,"ñ","n~");
    replaceAll(str,"Á","A'");
    replaceAll(str,"É","E'");
    replaceAll(str,"Í","I'");
    replaceAll(str,"Ó","O'");
    replaceAll(str,"Ú","U'");
    replaceAll(str,"Ñ","N~");
    vText.push_back(str);
    vTextSize.push_back(textSize);

    for(unsigned int i=0;i<vText.size();i++){

        int wString=0;

        for (unsigned int j=0; j<vText[i].length(); j++)
            wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, vText[i].c_str()[j])+textWidth;


        if(wMaxString<wString*vTextSize[i])wMaxString=wString*vTextSize[i];
    }
    wMaxString=wMaxString/hString;

    switch(alignmentText){
    case aLeft:
        xo=0.0;
        yo=0.0;
        break;
    case aCenter:
        xo=-wMaxString/2.0;
        yo=0.0;
        break;
    case aRight:
        xo=-wMaxString;
        yo=0.0;
        break;

    }
}

void Label3DClass::position(float x=0.0, float y=0.0, float z=0.0){this->x=x;this->y=y;this->z=z;};

void Label3DClass::size(float _size){textSize=_size;};
void Label3DClass::width(float _width){textWidth=_width;};
void Label3DClass::color(int r, int g, int b){textColorR=r; textColorG=g; textColorB=b;};
void Label3DClass::color(int r, int g, int b, int a){textColorR=r; textColorG=g; textColorB=b; textColorA=a;};
