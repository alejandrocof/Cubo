#include "RegistryGraph.hpp"


void RegistryGraph::loadXML(pugi::xml_node_iterator node_it){
  std::cout<<"name:"<<node_it->name()<<std::endl;
  for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
    std::cout<<"    <"<<it->name();
    
    std::string nodeName( it->name() );
    
    if( nodeName.compare("ColorMap")==0 ){
      std::string IdName;
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "getId", IdName);
      }
      DrawableIdentifier::data(IdName,color_map);
    }
    
    if( nodeName.compare("Data")==0 ){
      std::string IdName;
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "getId", IdName);
      }
      DrawableIdentifier::data(IdName,data3D);
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
        ifTheAttributeIsFoundAssign(ait, "precisiont", precisiont);
        ifTheAttributeIsFoundAssign(ait, "precisiony", precisiony);
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
    
    if( nodeName.compare("BackBoxColor")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "r", backBoxColorR);
        ifTheAttributeIsFoundAssign(ait, "g", backBoxColorG);
        ifTheAttributeIsFoundAssign(ait, "b", backBoxColorB);
        ifTheAttributeIsFoundAssign(ait, "a", backBoxColorA);
      }
    }
    
    std::cout <<"/>"<< std::endl;
  }
}

void RegistryGraph::draw(){

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
  
  /*
  glLineWidth(1);
  glBegin(GL_QUAD_STRIP);
  int size=color_map->lutColor.size()/3;

    for(int i=0; i<size; i++ ){
        //glColor3f(color_map->lutColor[3*i], color_map->lutColor[3*i+1], color_map->lutColor[3*i+2]);
        glColor3ub(color_map->lutColor[3*i], color_map->lutColor[3*i+1], color_map->lutColor[3*i+2]);
 		    glVertex3f( x, y+high*(float)i/(size-1),0.0f);
 		    glVertex3f( x+width, y+high*(float)i/(size-1),0.0f);
 	    }
  glEnd();
  
  */ 
  
  glLineWidth(1);
  glBegin(GL_QUADS);
  glColor4ub(backBoxColorR,backBoxColorG,backBoxColorB,backBoxColorA);
  glVertex3f( x, y,0.0f);
  glVertex3f( x+width, y,0.0f);
  glVertex3f( x+width, y+high,0.0f);
  glVertex3f( x, y+high,0.0f);
  glEnd();
    
  int wMaxString=0;
  
  
  
  //std::cout<<"RegistryGraph "<<data3D->registry.size()<<std::endl;
  float dy=0.2;
  //for (auto itRegistry = data3D->registry.begin(); itRegistry!=data3D->registry.end(); ++itRegistry) {
  for (unsigned int ireg=0; ireg<data3D->registry.size(); ireg++) {
    unsigned int size=data3D->registry[ireg].y.size();
    
    float yf=y+high*((float)ireg+1.0-0.5*dy)/data3D->registry.size();
    float yi=y+high*((float)ireg+0.5*dy)/data3D->registry.size();
    
    
    glLineWidth(1);
  
    glBegin(GL_LINES);
      glColor3ub(0,0,0);
      glVertex3f( x, (yi+yf)/2.0, 0.0f);
      glVertex3f( x+width, (yi+yf)/2.0,0.0f);
      glVertex3f( x, yi, 0.0f);
      glVertex3f( x+width, yi,0.0f);
      glVertex3f( x, yf, 0.0f);
      glVertex3f( x+width, yf,0.0f);
      
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glColor3ub(255,0,0);
    for(int i=0; i<sequencer->sControl.index; i++ ){
      glVertex3f( x+width*(float)i/(size-1), yi+(yf-yi)*(data3D->registry[ireg].y[i]-data3D->registry[ireg].minAxis)/(data3D->registry[ireg].maxAxis-data3D->registry[ireg].minAxis),0.0f);
    }
    glEnd();

    glLineWidth(textWidth);
    glColor4ub(textColorR,textColorG,textColorB,textColorA);
    //glColor3ub(10,10,10);

      std::stringstream textmin;
	    //text << std::fixed<< std::setprecision(precision) << itRegistry->min;
	    textmin << std::fixed<< std::setprecision(precisiony) <<  data3D->registry[ireg].minAxis;
	    int wString=0;
      int hString=119;
	    
        for (unsigned int j=0; j<textmin.str().length(); j++)
	      wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, textmin.str().c_str()[j]);
	    if(wMaxString<wString)wMaxString=wString;
	    
 		  glPushMatrix();
	      glTranslatef(x, yi,0.0f);
	      glScalef(textSize/hString,textSize/hString,textSize/hString);
	      glTranslatef(-wString,-0.5*hString,0.0f);
          for (unsigned int i=0; i<textmin.str().length(); i++)
	        glutStrokeCharacter(GLUT_STROKE_ROMAN, textmin.str().c_str()[i]);
      glPopMatrix();
      
      //text << std::fixed<< std::setprecision(precision) << itRegistry->max;
      std::stringstream textmax;
      textmax << std::fixed<< std::setprecision(precisiony) << data3D->registry[ireg].maxAxis;
	    wString=0;
	    
        for (unsigned int j=0; j<textmax.str().length(); j++)
	      wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, textmax.str().c_str()[j]);
	    if(wMaxString<wString)wMaxString=wString;
	    
 		  glPushMatrix();
	      glTranslatef(x, yf,0.0f);
	      glScalef(textSize/hString,textSize/hString,textSize/hString);
	      glTranslatef(-wString,-0.5*hString,0.0f);
          for (unsigned int i=0; i<textmax.str().length(); i++)
	        glutStrokeCharacter(GLUT_STROKE_ROMAN, textmax.str().c_str()[i]);
      glPopMatrix();
      
      glPushMatrix();
	      glTranslatef(x+0.9*width, yi+3.0*(yf-yi)/4.0,0.0f);
	      
  	      
  	      if(ireg==0){
    	      glBegin(GL_POLYGON);
    	      glColor3ub(128,255,255);
	          for(int i=0;i<=4;i++){
              float alpha=6.283185307*(0.125+(float)i/4.0);
              glVertex3f( (yf-yi)*0.1*cos(alpha), (yf-yi)*0.1*sin(alpha), 0.0f);
            }
            glEnd();
            
            glBegin(GL_LINE_LOOP);
            glColor3ub(0,43,122);
	          for(int i=0;i<=4;i++){
              float alpha=6.283185307*(0.125+(float)i/4.0);
              glVertex3f( (yf-yi)*0.1*cos(alpha), (yf-yi)*0.1*sin(alpha), 0.0f);
            }
            glEnd();
          }
          
          if(ireg==1){
            glBegin(GL_POLYGON);
    	      glColor3ub(128,255,255);
	          for(int i=0;i<=3;i++){
              float alpha=6.283185307*(0.25+(float)i/3.0);
              glVertex3f( (yf-yi)*0.1*cos(alpha), (yf-yi)*0.1*sin(alpha), 0.0f);
            }
            glEnd();
            
            glBegin(GL_LINE_LOOP);
            glColor3ub(0,43,122);
	          for(int i=0;i<=3;i++){
              float alpha=6.283185307*(0.25+(float)i/3.0);
              glVertex3f( (yf-yi)*0.1*cos(alpha), (yf-yi)*0.1*sin(alpha), 0.0f);
            }
            glEnd();
          }
        
      glPopMatrix();

 	}
  

  int Nx=11;
  for(int i=0; i<Nx; i++ ){
      std::stringstream text;
	    text << std::fixed<< std::setprecision(precisiont) << sequencer->sControl.timeInitial+ sequencer->sControl.timeIncrement*sequencer->sControl.indexN*i/(Nx-1);
	    int wString=0;
      int hString=119;
	    
        for (unsigned int j=0; j<text.str().length(); j++)
	      wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
	    if(wMaxString<wString)wMaxString=wString;
	    
	    glColor4ub(textColorR,textColorG,textColorB,textColorA);
	    
 		  glPushMatrix();
	      glTranslatef(x+width*i/(Nx-1), y,0.0f);
	      glScalef(textSize/hString,textSize/hString,textSize/hString);
	      glTranslatef(-wString,-0.5*hString,0.0f);
          for (unsigned int i=0; i<text.str().length(); i++)
	        glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[i]);
      glPopMatrix();
 	  }
 	{
 	
   	std::stringstream text;
	  text << "T(seg)";
	    int wString=0;
      int hString=119;
      
      for (unsigned int j=0; j<text.str().length(); j++)
	      wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
 	  
 	  glPushMatrix();
	      glTranslatef(x+0.5*width, y,0.0f);
	      glScalef(textSize/hString,textSize/hString,textSize/hString);
	      glTranslatef(-wString,-1.5*hString,0.0f);
          for (unsigned int i=0; i<text.str().length(); i++)
	        glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[i]);
      glPopMatrix();
  }
 	glLineWidth(titleTextWidth);
  glColor4ub(titleTextColorR,titleTextColorG,titleTextColorB,titleTextColorA);  
 	int wString=0;
 	int hString=119;
    for (unsigned int j=0; j<titleText.length(); j++)
 	  wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, titleText.c_str()[j]);
  
  glPushMatrix();
    glTranslatef(x, y+0.5*high,0.0f);
    glScalef(textSize/hString,textSize/hString,textSize/hString);
    //glTranslatef(-wMaxString,0.0,0.0f);
    glRotatef( 90.0,0.0,0.0,1.0);
    //glTranslatef(-0.5*wString,-0.5*hString,0.0f);
    glTranslatef(-0.5*wString,hString+wMaxString,0.0f);
    //glTranslatef(-wString-wMaxString,0.0,0.0f);
      for (unsigned int i=0; i<titleText.length(); i++){
	    if(titleText.c_str()[i]=='^'){
	      i++;
  	    glPushMatrix();
  	    glTranslatef(0.0, 0.5*hString, 0.0f);
  	    glScalef(0.5,0.5,1.0);
	        glutStrokeCharacter(GLUT_STROKE_ROMAN, titleText.c_str()[i]);
	      glPopMatrix();
	      glTranslatef(0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, titleText.c_str()[i]),0.0,0.0f);
	      
	    }
	    else
  	    glutStrokeCharacter(GLUT_STROKE_ROMAN, titleText.c_str()[i]);
	  }
	glPopMatrix();
	
	


  glDisable(GL_LINE_SMOOTH);
  glDisable (GL_BLEND);
}


