#include "ColorChart.hpp"

ColorChart::ColorChart( vector<Category> vCategory ){
 this->vCategory=vCategory;
}

void ColorChart::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("Position")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "x", x);
                ifTheAttributeIsFoundAssign(ait, "y", y);
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

        std::cout <<"/>"<< std::endl;
    }
}

void ColorChart::draw(){

    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);

    glLineWidth(0);

    //cout<<"vCategory->size():"<<vCategory.size()<<endl;


    //if(vCategory.size()>2){
    glBegin(GL_QUADS);
    for(int i=0; i<vCategory.size(); i++ ){
        glColor4ub(vCategory[i].r, vCategory[i].g, vCategory[i].b, vCategory[i].a);
        glVertex3f( x, y+high*(float)i/(vCategory.size()-1),0.0f);
        glVertex3f( x+width, y+high*(float)i/(vCategory.size()-1),0.0f);
        glVertex3f( x+width, y+high*(float)(i+0.5)/(vCategory.size()-1),0.0f);
        glVertex3f( x, y+high*(float)(i+0.5)/(vCategory.size()-1),0.0f);
    }
    glEnd();

    glLineWidth(textWidth);
    glColor4ub(textColorR,textColorG,textColorB,textColorA);
    int wMaxString=0;
    for(int i=0; i<vCategory.size(); i++ ){
        int wString=0;
        int hString=119;
        for (unsigned int j=0; j<vCategory[i].name.length(); j++)
            wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, vCategory[i].name.c_str()[j]);
        if(wMaxString<wString)wMaxString=wString;

        glPushMatrix();
        glTranslatef(x+1.2*width, y+high*(float)i/(vCategory.size()-1),0.0f);
        float scale=1.0;//0.4 //(float)4.0*high/(vCategory.size());
        glScalef(textSize*scale/hString,textSize*scale/hString,textSize*scale/hString);
        //glTranslatef(-wString,-0.5*hString,0.0f);
        for (unsigned int j=0; j<vCategory[i].name.length(); j++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, vCategory[i].name.c_str()[j]);
        glPopMatrix();
    }

    //}



}
