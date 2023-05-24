#ifndef REGISTRY_GRAPH_HPP
#define REGISTRY_GRAPH_HPP
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
#include "DrawableIdentifier.hpp"
#include "TextureManager.hpp"
#include "ColorMap.hpp"
#include "LoadSimulationData.hpp"
#include "Squencer.hpp"

class ColorMapClass;
class DataRaw3D;

class RegistryGraph{
  public:
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    
  private:
    float x,y;
    float width,high;
    int precisiony=4;
    int precisiont=2;
    float textSize;
    float textColorR,textColorG,textColorB,textColorA;
    float textWidth;
    float titleTextSize;
    float titleTextColorR,titleTextColorG,titleTextColorB,titleTextColorA;
    float titleTextWidth;
    std::string titleText;
    int backBoxColorR, backBoxColorG, backBoxColorB, backBoxColorA;
    const ColorMapClass *color_map;
    DataRaw3D *data3D;
    Squencer *sequencer;
    
};

#endif



