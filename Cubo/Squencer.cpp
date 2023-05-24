#include "Squencer.hpp"

sequenceControl Squencer::sControl;
bool Squencer::change=false;
bool Squencer::reset=false;

void Squencer::loadXML(pugi::xml_node_iterator node_it){
  std::cout<<"name:"<<node_it->name()<<std::endl;
  sequenceControl sc;

  for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
    std::cout<<"    <"<<it->name();
    std::string nodeName( it->name() );
  
    if( nodeName.compare("Index")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "N", sc.indexN);
        ifTheAttributeIsFoundAssign(ait, "index", sc.index);
      }
    }
    
    if( nodeName.compare("Time")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "initial", sc.timeInitial);
        ifTheAttributeIsFoundAssign(ait, "increment", sc.timeIncrement);
      }
    }
  
    if( nodeName.compare("Label2D")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "x", sc.textX);
        ifTheAttributeIsFoundAssign(ait, "y", sc.textY);
        ifTheAttributeIsFoundAssign(ait, "r", sc.textColorR);
        ifTheAttributeIsFoundAssign(ait, "g", sc.textColorG);
        ifTheAttributeIsFoundAssign(ait, "b", sc.textColorB);
        ifTheAttributeIsFoundAssign(ait, "a", sc.textColorA);
        ifTheAttributeIsFoundAssign(ait, "textWidth", sc.textWidth);
        ifTheAttributeIsFoundAssign(ait, "textSize", sc.textSize);
        ifTheAttributeIsFoundAssignString(ait, "textInitial", sc.text1);
        ifTheAttributeIsFoundAssignString(ait, "textFinal", sc.text2);
        ifTheAttributeIsFoundAssign(ait, "precision", sc.precision);
      }
    }
    
    std::cout <<"/>"<< std::endl;
  }
  
    sControl=sc;

}

  
void Squencer::update(){


  //sControl.index++;
  //if(sControl.index>sControl.indexN)sControl.index=0;
  if( reset ){
    sControl.index=0;
    reset=false;
    change=true;
  }
  else{

  if( sControl.play || sControl.fbf ){
    sControl.fbf=false;
    
    if( !sControl.reverse ){
      if( sControl.index+1>sControl.indexN /*sControl.val + sControl.inc > sControl.max*/ ){
        if( sControl.loop ){
          sControl.index=0;
          //sControl.val = sControl.min;

          change=true;
        }
        else{
          change=false;
        }
      }
      else{
        sControl.index++;
        //sControl.val += sControl.inc;

        change=true;
      }
    }
    else{
      if( sControl.index-1<0/*sControl.val - sControl.inc < sControl.min*/ ){
        if( sControl.loop ){
          //sControl.val = sControl.max;
          sControl.index=sControl.indexN-1;
          change=true;
        }
        else{
          change=false;
        }
      }
      else{
        //sControl.val -= sControl.inc;
        sControl.index--;
        change=true;
      }
    }
  }
  else{
    change=false;
  }
  }
  if(change)
    std::cout<<"Squencer: "<<sControl.index<<std::endl;

}


void Squencer::play(bool _play){
  sControl.play=_play;
}

void Squencer::reverse(bool _reverse){
  sControl.reverse=_reverse;
}

void Squencer::loop(bool _loop){
  sControl.loop=_loop;
}

void Squencer::fbf(bool _fbf){
  sControl.fbf=_fbf;
}



void Squencer::draw(){

  int hString=120;
  
  glDisable(GL_DEPTH_TEST);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);

  
  glLineWidth(sControl.textWidth);

  glPushMatrix();
  
      
      int wString=0;
      
      std::stringstream ss;
      //<<std::fixed<<std::setfill('0')<<std::setw(numberOfIntegerDigits+numberOfDecimalDigits+spaceDecimalPoint)<<std::setprecision(numberOfDecimalDigits)
      if(sControl.otherText.empty()){
          ss<<sControl.text1<<std::fixed<<std::setprecision(sControl.precision)<<sControl.timeInitial+sControl.timeIncrement*sControl.index<<sControl.text2;
      }
      else{
          ss<<sControl.otherText;
      }
      for (unsigned int j=0; j<ss.str().length(); j++){
        if(ss.str().c_str()[j]!='`')
          wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, ss.str().c_str()[j]);
      }
      
      
      glTranslatef(sControl.textX ,sControl.textY,0.0);
      //std::cout<<"text1"<<sControl.text1.c_str()<<std::endl;
      //glPushMatrix();
        glColor4ub(sControl.textColorR,sControl.textColorG,sControl.textColorB,sControl.textColorA);
        glScalef(sControl.textSize/hString,sControl.textSize/hString,sControl.textSize/hString);
        glTranslatef(-0.5*wString,0.0,0.0f);
        for (unsigned int j=0; j<ss.str().length(); j++){
          if(ss.str().c_str()[j]=='`'){
            glPushMatrix();
            glTranslatef(0.0,0.2*hString,0.0);
            glScalef(-0.3,0.8,1.0);
            glutStrokeCharacter(GLUT_STROKE_ROMAN, ss.str().c_str()[j]);
            glPopMatrix();
          }
          else{
            glutStrokeCharacter(GLUT_STROKE_ROMAN, ss.str().c_str()[j]);
          }
        }
        
        
      //glPopMatrix();
      
  glPopMatrix();

  glDisable(GL_LINE_SMOOTH);
  glDisable (GL_BLEND);
}

