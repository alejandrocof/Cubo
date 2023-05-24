#include "Text3D.hpp"

void Text3D::projection(Projection *proj){
    this->proj=proj;
}

void Text3D::setWidth(float  width){
    this->width=width;
}

void Text3D::setSize(float  size){
    this->size=size;
}
void Text3D::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );


        if( nodeName.compare("Config")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", r);
                ifTheAttributeIsFoundAssign(ait, "g", g);
                ifTheAttributeIsFoundAssign(ait, "b", b);
                ifTheAttributeIsFoundAssign(ait, "a", a);
                ifTheAttributeIsFoundAssign(ait, "size", size);
                ifTheAttributeIsFoundAssign(ait, "width", width);
            }
        }

        if( nodeName.compare("AddText")==0 ){
            std::string salignment;
            alignmentTxt alignment=alignmentLeft;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssignString(ait, "text", str)){
                    int wStr;
                    double xo,yo;
                    alignmentString(str, alignment, wStr, xo, yo);
                    replace(str);
                    x=longitude;
                    y=latitude;
                    proj->transform(x,y);
                    sTxt newtxt(str,x,y,z,wStr,xo,yo,width,r,g,b,a,size);
                    vTxt.push_back(newtxt);
                }
                ifTheAttributeIsFoundAssign(ait, "long", longitude);
                ifTheAttributeIsFoundAssign(ait, "lat", latitude);
                if(ifTheAttributeIsFoundAssign(ait, "alignment", salignment)){
                    if(salignment=="Left" || salignment=="left")alignment=alignmentLeft;
                    if(salignment=="Center" || salignment=="center")alignment=alignmentCenter;
                    if(salignment=="Right" || salignment=="right")alignment=alignmentRight;
                }


            }
        }

//        if( nodeName.compare("TextAlignment")==0 ){
//            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
//                ifTheAttributeIsFoundAssign(ait, "textAlignment", textAlignment);
//            }
//        }

        if( nodeName.compare("Projection")==0 ){
            std::string IdProy;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdProy);
            }
            DrawableIdentifier::data(IdProy,proj);
        }
        std::cout <<"/>"<< std::endl;
    }


}

void Text3D::draw(){
    //cout<<"x:"<<vTxt[0].x<<" y:"<<vTxt[0].y<<" text:"<<vTxt[0].str<<endl;

    double distmax=fabs(Shapes::xtmax-Shapes::xtmin);
    double hString=119.0;


    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    for (auto&& txt : vTxt){
        double sizeLabel=txt.size*distmax/(80*hString);
        glPushMatrix();
        glTranslated(txt.x,txt.y,txt.z);
        glScaled(sizeLabel,sizeLabel,sizeLabel);
        glTranslated(txt.xo,txt.yo,0);
        glLineWidth(1.5*txt.width);
        glColor4ub(~txt.r,~txt.g,~txt.b,txt.a);
        printString(txt.str);
        glPopMatrix();
    }
    for (auto&& txt : vTxt){
        double sizeLabel=txt.size*distmax/(80*hString);
        glPushMatrix();
        glTranslated(txt.x,txt.y,txt.z);
        glScaled(sizeLabel,sizeLabel,sizeLabel);
        //glTranslated(-0.5*txt.wString,0,0);
        glTranslated(txt.xo,txt.yo,0);
        glLineWidth(txt.width);
        glColor4ub(txt.r,txt.g,txt.b,txt.a);
        printString(txt.str);
        glPopMatrix();
    }
}

int Text3D::widthString(string text){
    int wString=0;
    for (unsigned int j=0; j<text.length(); j++)
        wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.c_str()[j]);
    return wString;
}

void Text3D::alignmentString(string text, alignmentTxt alignment, int &wString, double &xo, double &yo){
    wString=0;
    for (unsigned int j=0; j<text.length(); j++)
        wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.c_str()[j]);
    xo=0;
    yo=0;

    switch(alignment){
        case alignmentLeft:
            xo=0.0;
            yo=0.0;
            break;
        case alignmentCenter:
            xo=-wString/2.0;
            yo=0.0;
            break;
        case alignmentRight:
            xo=-wString;
            yo=0.0;
            break;

        }

}

void Text3D::printString(string text){
    double hString=119.0;
    for (unsigned int j=0; j<text.length(); j++){
        if(text.c_str()[j]=='`'){
            glPushMatrix();
            glTranslatef(0.0,0.2*hString,0.0);
            glScalef(-0.3,0.8,1.0);
            glutStrokeCharacter(GLUT_STROKE_ROMAN, text.c_str()[j]);
            glPopMatrix();
        }
        else
        {
            if(text.c_str()[j]=='\''){
                glPushMatrix();
                glTranslatef(0.0,0.5*hString,0.0);
                glScalef(-0.3,0.8,1.0);
                glutStrokeCharacter(GLUT_STROKE_ROMAN, '`');
                glPopMatrix();
            }
            else{
                if(text.c_str()[j]=='~'){
                    glPushMatrix();
                    glTranslatef(-0.5*hString,0.55*hString,0.0);
                    glScalef(0.5,0.5,1.0);
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, text.c_str()[j]);
                    glPopMatrix();
                }
                else{
                    if(text.c_str()[j]=='^'){
                        j++;
                        glPushMatrix();
                        glTranslatef(0.0, 0.5*hString, 0.0f);
                        glScalef(0.5,0.5,1.0);
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, text.c_str()[j]);
                        glPopMatrix();
                        glTranslatef(0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, text.c_str()[j]),0.0,0.0f);

                    }
                    else{
                        if(text.c_str()[j]=='_'){
                            j++;
                            glPushMatrix();
                            glTranslatef(0.0, -0.25*hString, 0.0f);
                            glScalef(0.5,0.5,1.0);
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, text.c_str()[j]);
                            glPopMatrix();
                            glTranslatef(0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, text.c_str()[j]),0.0,0.0f);

                        }
                        else{
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, text.c_str()[j]);
                        }
                    }
                }
            }
        }
    }
}

void Text3D::print(double x, double y,string text){

    double distmax=fabs(Shapes::xtmax-Shapes::xtmin);
    double hString=119.0;
    double sizeLabel=size*distmax/(80*hString);
    int wString=widthString(text);

    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(width);
    glColor4ub(r,g,b,a);

    glPushMatrix();
    glTranslated(x,y,0.0f);
    glScaled(sizeLabel,sizeLabel,sizeLabel);
    glTranslated(-0.5*wString,0,0);
    printString(text);
    glPopMatrix();

}


void Text3D::addTextlonlat(double longitude, double latitude, string str, alignmentTxt alignment=alignmentLeft){

    int wStr;
    double xo,yo;
    alignmentString(str, alignment, wStr, xo, yo);
    replace(str);
    x=longitude;
    y=latitude;
    proj->transform(x,y);
    sTxt newtxt(str,x,y,z,wStr,xo,yo,width,r,g,b,a,size);
    vTxt.push_back(newtxt);

}

void Text3D::addTextXY(double x, double y, string str, alignmentTxt alignment=alignmentLeft){

    int wStr;
    double xo,yo;
    alignmentString(str, alignment, wStr, xo, yo);
    replace(str);
    sTxt newtxt(str,x,y,z,wStr,xo,yo,width,r,g,b,a,size);
    vTxt.push_back(newtxt);

}

void Text3D::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void Text3D::replace(std::string& str) {
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
}
