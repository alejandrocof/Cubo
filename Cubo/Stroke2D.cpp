#include "Stroke2D.hpp"


Stroke2D::Stroke2D(){
  r=255;
  g=0;
  b=0;
  a=128;
  width=1.0;
}

void Stroke2D::setColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
  r=_r;
  g=_g;
  b=_b;
  a=_a;
}

void Stroke2D::setWidth(float _w){
  width=_w;
}

void Stroke2D::setPoint(float longitude, float latitude){
  point p;
  p.longitude=longitude;
  p.latitude=latitude;
  vPoints.push_back(p);
}

void Stroke2D::projection(std::string IdProy){
    DrawableIdentifier::data(IdProy,proj);
}


void Stroke2D::make(){
    double p0x,p0y,p1x,p1y,ux,uy,uLx,uLy,r;
    if(vPoints.size()>=2){
        for(unsigned int i=0;i<vPoints.size()-1;i++){
            p0x=vPoints[i].longitude; p0y=vPoints[i].latitude;
            p1x=vPoints[i+1].longitude; p1y=vPoints[i+1].latitude;
            ux=p1x-p0x; uy=p1y-p0y;
            r=sqrt(ux*ux+uy*uy); ux/=r; uy/=r;
            uLx=uy; uLy=-ux;

            stroke.push_back(p0x+width*uLx);
            stroke.push_back(p0y+width*uLy);
            stroke.push_back(0.0);
            stroke.push_back(p0x-width*uLx);
            stroke.push_back(p0y-width*uLy);
            stroke.push_back(0.0);

            stroke.push_back(p1x+width*uLx);
            stroke.push_back(p1y+width*uLy);
            stroke.push_back(0.0);
            stroke.push_back(p1x-width*uLx);
            stroke.push_back(p1y-width*uLy);
            stroke.push_back(0.0);
        }
    }

    //std::cout<<" size="<<stroke.size()<<" -> "<<stroke.size()/3<<std::endl;
//    for(unsigned int i=0;i<stroke.size();i+=3){
//        //std::cout<<" ("<<stroke[i]<<", "<<stroke[i+1]<<", "<<stroke[i+2]<<")"<<std::endl;
//        stroke[i]*=DEG_TO_RAD;
//        stroke[i+1]*=DEG_TO_RAD;
//    }

//    for(unsigned int i=0;i<stroke.size();i+=3){
//        std::cout<<" ("<<stroke[i]<<", "<<stroke[i+1]<<", "<<stroke[i+2]<<")"<<std::endl;
//    }
    proj->transform3d(stroke);
//    for(unsigned int i=0;i<stroke.size();i+=3){
//        std::cout<<" ("<<stroke[i]<<", "<<stroke[i+1]<<", "<<stroke[i+2]<<")"<<std::endl;
//    }
}

void Stroke2D::loadXML(pugi::xml_node_iterator node_it){
  std::cout<<"name:"<<node_it->name()<<std::endl;
  for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
    std::cout<<"    <"<<it->name();
    
    std::string nodeName( it->name() );
    
    if( nodeName.compare("Color")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "r", r);
        ifTheAttributeIsFoundAssign(ait, "g", g);
        ifTheAttributeIsFoundAssign(ait, "b", b);
        ifTheAttributeIsFoundAssign(ait, "a", a);
      }
    }
    
    if( nodeName.compare("Width")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "width", width);
      }
    }
    
    if( nodeName.compare("Point")==0 ){
      point p;
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "lat", p.latitude);
        ifTheAttributeIsFoundAssign(ait, "long", p.longitude);
      }
      vPoints.push_back(p);
      std::cout<<std::endl<<"lat="<<p.latitude<<" long="<<p.longitude<<std::endl;
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
  
  make();

}



void Stroke2D::draw(){
    /*glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_POLYGON_SMOOTH);
  glLineWidth(0);
  glColor4ub(r,g,b,a);
  glEnableClientState(GL_VERTEX_ARRAY);
//  glVertexPointer(3, GL_DOUBLE, 0, stroke.data());
//  glDrawArrays(GL_QUAD_STRIP, 0, stroke.size()/3);
//  //glDrawArrays(GL_QUADS, 0, stroke.size()/3);
//  glDisableClientState(GL_VERTEX_ARRAY);
//  //glDisable(GL_POLYGON_SMOOTH);

  glDisable (GL_BLEND);*/

    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glLineWidth(0);
    glColor4ub(r,g,b,a);

    glBegin(GL_QUAD_STRIP);
    for(unsigned int i=0;i<stroke.size();i+=3){
        glVertex3f( stroke[i], stroke[i+1],stroke[i+2]);
    }
    glEnd();

}
