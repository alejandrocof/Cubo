#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <cmath>
#include <vector>
#include <limits>
#include <map>

#include <GL/glew.h>
#include <GL/glut.h>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"

        
struct sequenceControl{

  bool play;
  bool reverse;
  bool loop;
  bool fbf; //frame by frame animation
  int index;
  int indexN;
  
  float timeInitial;
  float timeIncrement;

  float textX,textY;
  int textColorR,textColorG,textColorB,textColorA;
  float textWidth,textSize;
  std::string text1,text2,otherText;
  int precision;
  
  
  
  sequenceControl():play(false),reverse(false),loop(false),fbf(false),index(0),indexN(1),precision(2){};
};

class Squencer{
  public:
    static void loadXML(pugi::xml_node_iterator it);
    static void update();
    static void play(bool _play);
    static void reverse(bool _reverse);
    static void loop(bool _loop);
    static void fbf(bool _fbf);
    static const bool &changes(void){return change;};
    static const int &index(void){return sControl.index;};
    static void init(){reset=true;};
    
    void draw();
    static sequenceControl sControl;
  private:
    
    static bool change;
    static bool reset;
};


#endif
