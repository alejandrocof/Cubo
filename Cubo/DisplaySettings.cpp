#include "DisplaySettings.hpp"

int DisplaySettings::bg_r=255;
int DisplaySettings::bg_g=255;
int DisplaySettings::bg_b=255;
int DisplaySettings::bg_a=255;
double DisplaySettings::lookAtX=0.0;
double DisplaySettings::lookAtY=0.0;
double DisplaySettings::lookAtZ=0.0;
double DisplaySettings::_distance=0.0;
double DisplaySettings::_dL=1.0;
int DisplaySettings::_windowSizeWidth=500;
int DisplaySettings::_windowSizeHeight=500;

void DisplaySettings::loadXML(pugi::xml_node_iterator node_it){
  std::cout<<"name:"<<node_it->name()<<std::endl;
  for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
    std::cout<<"    <"<<it->name()<<" ";
    
    std::string nodeName( it->name() );
    
    if( nodeName.compare("BackgroundColor")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "r", bg_r);
        ifTheAttributeIsFoundAssign(ait, "g", bg_g);
        ifTheAttributeIsFoundAssign(ait, "b", bg_b);
        ifTheAttributeIsFoundAssign(ait, "a", bg_a);
      }
    }
    
    if( nodeName.compare("LookAt")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "lat", lookAtY);
        ifTheAttributeIsFoundAssign(ait, "long", lookAtX);
        ifTheAttributeIsFoundAssign(ait, "z", lookAtZ);
      }
    }
    
    if( nodeName.compare("DistanceToTheObserver")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "distance", _distance);
      }
    }
    
    if( nodeName.compare("WindowSize")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "width", _windowSizeWidth);
        ifTheAttributeIsFoundAssign(ait, "height", _windowSizeHeight);
      }
    }
    std::cout <<"/>"<< std::endl;
  }
}

void DisplaySettings::draw(){
    lookAtX=0.5*(Shapes::xtmin+Shapes::xtmax);
    lookAtY=0.5*(Shapes::ytmin+Shapes::ytmax);
//    double wh=DisplaySettings::windowSizeWidth()/DisplaySettings::windowSizeHeight();
//    double whmax=fabs(Shapes::xtmax-Shapes::xtmin)/fabs(Shapes::ytmax-Shapes::ytmin);
    DisplaySettings::_dL=std::max(Shapes::xtmax-Shapes::xtmin,Shapes::ytmax-Shapes::ytmin);
    //DisplaySettings::_dL=fabs(Shapes::ytmax-Shapes::ytmin);
    _distance=1.5;
    //        lookAtX=0.5*(Shapes::xtmin+Shapes::xtmax);
    //        lookAtY=0.5*(Shapes::ytmin+Shapes::ytmax);
    //        _distance=1.4*std::max(Shapes::xtmax-Shapes::xtmin,Shapes::ytmax-Shapes::ytmin);
    //        lookAtX=0.5;
    //        lookAtY=0.5;
    //        _distance=1.4;


    //glTranslatef( -DisplaySettings::lookAt_X(), -DisplaySettings::lookAt_Y(), -DisplaySettings::lookAt_Z() );
    glScaled(1.0/DisplaySettings::_dL,1.0/DisplaySettings::_dL,1.0/DisplaySettings::_dL);
    glTranslated( -DisplaySettings::lookAtX, -DisplaySettings::lookAtY, -DisplaySettings::lookAtZ );
    //cout<<"dl="<<1.0/dl<<endl;
//    cout<<"Shapesx("<<Shapes::xtmin<<","<<Shapes::xtmax<<")";
//    cout<<"-Shapesy("<<Shapes::ytmin<<","<<Shapes::ytmax<<")"<<endl;
}
