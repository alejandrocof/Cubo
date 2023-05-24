#include "ColorMap.hpp"

int l(int a, int b, double lambda){
    return a+lambda*(b-a);
}
ColorMapClass::ColorMapClass(){
  //lutColor.clear();
  //glGenTextures(1,&colorTextureID);
  //glGenTextures(1,&alphaTextureID);
  colorTextureEnum=TextureManager().newTexture();
  alphaTextureEnum=TextureManager().newTexture();
  std::cout<<"colorTextureEnum"<<colorTextureEnum<<std::endl;
  std::cout<<"alphaTextureEnum"<<alphaTextureEnum<<std::endl;
  
//  int k=6;
//  pushColor(153,102,255,k);
//  pushColor(0,0,255,k);
//  pushColor(0,255,0,k);
//  pushColor(255,255,0,k);
//  pushColor(255,102,0,k);
//  pushColor(255,0,0,k);

  pushColor(153,102,255);
  pushColor(0,0,255);
  pushColor(0,255,0);
  pushColor(255,255,0);
  pushColor(255,102,0);
  pushColor(255,0,0);
  pushAlpha(255);
  //pushAlpha(240);
  //glGenTextures(1,&colorTextureID);
  
  //glActiveTexture(alphaTextureEnum);
  //glGenTextures(1,&alphaTextureID);
}

void ColorMapClass::pushColor(GLubyte r,GLubyte g,GLubyte b){
  lutColor.push_back(r);
  lutColor.push_back(g);
  lutColor.push_back(b);
}

void ColorMapClass::pushColor(GLubyte r,GLubyte g,GLubyte b,int k){
    int size=lutColor.size();
    if(size!=0){
        GLubyte r1=lutColor[size-3];
        GLubyte g1=lutColor[size-2];
        GLubyte b1=lutColor[size-1];
        for(int i=0;i<k;i++){
            double lambda=(double)(i+1)/(k+1);
            lutColor.push_back(r1+lambda*(r-r1));
            lutColor.push_back(g1+lambda*(g-g1));
            lutColor.push_back(b1+lambda*(b-b1));
        }
    }
    lutColor.push_back(r);
    lutColor.push_back(g);
    lutColor.push_back(b);
}

void ColorMapClass::pushAlpha(GLubyte alpha){
  lutAlpha.push_back(alpha);
}

void ColorMapClass::update(){
  glGenTextures(1,&colorTextureID);
  glActiveTexture(GL_TEXTURE0+colorTextureEnum);
  
  //glActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D,colorTextureID);
	glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,lutColor.size()/3,0,GL_RGB,GL_UNSIGNED_BYTE,lutColor.data() );
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //glDisable(GL_TEXTURE_1D);

  glGenTextures(1,&alphaTextureID);
  glActiveTexture(GL_TEXTURE0+alphaTextureEnum);
  
	//glActiveTexture(GL_TEXTURE2);
	//glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D,alphaTextureID);
	glTexImage1D(GL_TEXTURE_1D,0,GL_ALPHA,lutAlpha.size(),0,GL_ALPHA,GL_UNSIGNED_BYTE,lutAlpha.data() );
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glDisable(GL_TEXTURE_1D);
}

void ColorMapClass::loadXML(pugi::xml_node_iterator node_it){
  std::cout<<"name:"<<node_it->name()<<std::endl;

  lutColor.clear();
  lutAlpha.clear();

  for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
    std::cout<<"    <"<<it->name();
    
    std::string nodeName( it->name() );
    
    if( nodeName.compare("PushColor")==0 ){
      int r,g,b;
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "r", r);
        ifTheAttributeIsFoundAssign(ait, "g", g);
        ifTheAttributeIsFoundAssign(ait, "b", b);
      }
      lutColor.push_back(r);
      lutColor.push_back(g);
      lutColor.push_back(b);
    }

    if( nodeName.compare("PushLabel")==0 ){
      string lab;
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssignString(ait,"label",lab);
      }
      label.push_back(lab);
    }

    
    if( nodeName.compare("PushAlpha")==0 ){
      int alpha;
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "a", alpha);
      }
      lutAlpha.push_back(alpha);
    }
    
    if( nodeName.compare("ExtremeValues")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "min", minimumValue);
        ifTheAttributeIsFoundAssign(ait, "max", maximumValue);
        if(ifTheAttributeIsFoundAssign(ait, "absmin", minimumValue))bAbsMinAvailable=true;
        if(ifTheAttributeIsFoundAssign(ait, "absmax", maximumValue))bAbsMaxAvailable=true;
      }
      if(bAbsMinAvailable && bAbsMaxAvailable && minimumValue==maximumValue){
         bAbsMinAvailable=false;
         bAbsMaxAvailable=false;
      }
    }
    
    std::cout <<"/>"<< std::endl;
  }
}

void ColorMapClass::draw(){
  
}

void ColorMapClass::transferLambda(float val, float &lambda){
  lambda=(val-minimumValue)/(maximumValue-minimumValue);
  if(lambda>1.0)lambda=1.0;
  if(lambda<0.0)lambda=0.0;
}

void ColorMapClass::transferColor(float val, int &r, int &g, int &b){
  float lambda=(val-minimumValue)/(maximumValue-minimumValue);
  if(lambda>1.0)lambda=1.0;
  if(lambda<0.0)lambda=0.0;
  int size=lutColor.size()/3;
  float f=lambda*(size-1);
  float indice=floor(f);
  float delta=f-indice;
  r = lutColor[3*indice] + delta*( lutColor[3*(indice+1)]-lutColor[3*indice] );
  g = lutColor[3*indice+1] + delta*( lutColor[3*(indice+1)+1]-lutColor[3*indice+1] );
  b = lutColor[3*indice+2] + delta*( lutColor[3*(indice+1)+2]-lutColor[3*indice+2] );
}

void ColorMapClass::transferColorN(float val, GLubyte &r, GLubyte &g, GLubyte &b){
  float lambda=val;
  if(lambda>1.0)lambda=1.0;
  if(lambda<0.0)lambda=0.0;
  int size=lutColor.size()/3;
  float f=lambda*(size-1);
  float indice=floor(f);
  float delta=f-indice;
  r = lutColor[3*indice] + delta*( lutColor[3*(indice+1)]-lutColor[3*indice] );
  g = lutColor[3*indice+1] + delta*( lutColor[3*(indice+1)+1]-lutColor[3*indice+1] );
  b = lutColor[3*indice+2] + delta*( lutColor[3*(indice+1)+2]-lutColor[3*indice+2] );
}

void ColorMapClass::transferAlpha(float val, int &alpha){
  float lambda=(val-minimumValue)/(maximumValue-minimumValue);
  if(lambda>1.0)lambda=1.0;
  if(lambda<0.0)lambda=0.0;
  int size=lutAlpha.size();
  float f=lambda*(size-1);
  float indice=floor(f);
  float delta=f-indice;
  alpha = lutAlpha[indice] + delta*( lutAlpha[indice+1]-lutAlpha[indice] );
}

int ColorMapClass::transferR(float val){
  float lambda=(val-minimumValue)/(maximumValue-minimumValue);
  return lambda*255;
}

int ColorMapClass::transferG(float val){
  float lambda=(val-minimumValue)/(maximumValue-minimumValue);
  return lambda*255;
}

int ColorMapClass::transferB(float val){
  float lambda=(val-minimumValue)/(maximumValue-minimumValue);
  return lambda*255;
}

int ColorMapClass::transferAlpha(float val){
  float lambda=(val-minimumValue)/(maximumValue-minimumValue);
  return lambda*255;
  //return 255;
}
    

