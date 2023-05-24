#include "BarScale.hpp"

BarScale::BarScale()
{
    step={-1.0/2.0,-7.0/16.0,-3.0/8.0,-5.0/16.0,-1.0/4.0,0.0,1.0/4.0,1.0/2.0};
    stepTxt={-1.0/2.0,-1.0/4.0,0.0,1.0/4.0,1.0/2.0};
}

void BarScale::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("FillColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssign(ait, "r", f_r));
                if(ifTheAttributeIsFoundAssign(ait, "g", f_g));
                if(ifTheAttributeIsFoundAssign(ait, "b", f_b));
                if(ifTheAttributeIsFoundAssign(ait, "a", f_a));
            }
        }

        if( nodeName.compare("StrokeColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssign(ait, "r", s_r));
                if(ifTheAttributeIsFoundAssign(ait, "g", s_g));
                if(ifTheAttributeIsFoundAssign(ait, "b", s_b));
                if(ifTheAttributeIsFoundAssign(ait, "a", s_a));
            }
        }

        if( nodeName.compare("StrokeWidth")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "w", w);
            }
        }

        if( nodeName.compare("TextColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", t_r);
                ifTheAttributeIsFoundAssign(ait, "g", t_g);
                ifTheAttributeIsFoundAssign(ait, "b", t_b);
                ifTheAttributeIsFoundAssign(ait, "a", t_a);
            }
        }

        if( nodeName.compare("TextWidth")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textWidth", txtWidth);
            }
        }

        if( nodeName.compare("TextSize")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textSize", textSize);
            }
        }

        if( nodeName.compare("TextAlignment")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textAlignment", textAlignment);
            }
        }

        if( nodeName.compare("Projection")==0 ){
            std::string IdProy;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdProy);
            }
            DrawableIdentifier::data(IdProy,proj);

        }

        if( nodeName.compare("Settings")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "x", x);
                ifTheAttributeIsFoundAssign(ait, "y", y);
                ifTheAttributeIsFoundAssign(ait, "size", size);
                ifTheAttributeIsFoundAssign(ait, "aspectRatio", aspectRatio);
            }
        }

        std::cout <<"/>"<< std::endl;
    }

    double h=1000.0*size*aspectRatio/2.0;

    for(int i=0;i<stepTxt.size();i++){
        double x1,y1,x2,y2,azi;
        proj->geodDirect(y,x,90, 1000.0*size*stepTxt[i], y1, x1, azi);
        proj->geodDirect(y1,x1,0, 1.2*h, y2, x2, azi);
        xyTxt.push_back(x2);xyTxt.push_back(y2);
    }
    proj->transform(xyTxt);
    txt=new Text3D();
    txt->setWidth(txtWidth);
    txt->setSize(textSize);

    for(int i=0;i<step.size()-1;i++){

        double x1,y1,x2,y2,x3,y3,x4,y4,azi;

        proj->geodDirect(y,x,90, 1000.0*size*step[i], y1, x1, azi);
        proj->geodDirect(y,x,90, 1000.0*size*step[i+1], y2, x2, azi);
        proj->geodDirect(y2,x2,0, (1-2*(i%2))*h, y3, x3, azi);
        proj->geodDirect(y1,x1,0, (1-2*(i%2))*h, y4, x4, azi);

        xy.push_back(x1);xy.push_back(y1);
        xy.push_back(x2);xy.push_back(y2);
        xy.push_back(x3);xy.push_back(y3);
        xy.push_back(x4);xy.push_back(y4);

        //cout<<i<<" ("<<x1<<","<<y1<<") ("<<x2<<","<<y2<<") ("<<x3<<","<<y3<<") ("<<x4<<","<<y4<<")"<<endl;
    }
    proj->transform(xy);


    //txt->projection(proj);
    //txt.projection(proj);

//    double distmax=fabs(xy[0]-xy[xy.size()-3]);
//    cout<<"distancia maxima="<<distmax<<endl;



}

void BarScale::draw(){

    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);

    glColor4ub(f_r,f_g,f_b,f_a);

    glBegin(GL_QUADS);
    for(int i=0; i<xy.size(); i+=8 ){
        glVertex3f( xy[i], xy[i+1],0.0f);
        glVertex3f( xy[i+2], xy[i+3],0.0f);
        glVertex3f( xy[i+4], xy[i+5],0.0f);
        glVertex3f( xy[i+6], xy[i+7],0.0f);
    }
    glEnd();

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glLineWidth(1);
    for(int i=0; i<xy.size(); i+=8 ){
        glBegin(GL_LINE_LOOP);
        glVertex3f( xy[i], xy[i+1],0.0f);
        glVertex3f( xy[i+2], xy[i+3],0.0f);
        glVertex3f( xy[i+4], xy[i+5],0.0f);
        glVertex3f( xy[i+6], xy[i+7],0.0f);
        glEnd();
        //glVertex3f( xy[i], xy[i+1],0.0f);
    }

    for(int i=0; i<stepTxt.size(); i++ ){
        std::stringstream text;
        text << std::fixed<< std::setprecision(precisionLabel) << size*(0.5+stepTxt[i]) <<"km";
        txt->print(xyTxt[2*i], xyTxt[2*i+1],text.str());
    }

}
