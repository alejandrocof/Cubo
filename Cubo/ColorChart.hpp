#ifndef COLORCHART_HPP
#define COLORCHART_HPP

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
#include "LoadShapeFileCSV.hpp"

using namespace std;

struct Category;

class ColorChart
{
public:
    ColorChart( vector<Category> vCategory );
    void loadXML(pugi::xml_node_iterator it);
    void draw();

    std::string titleText;
    float width=0.02,high=1.6;//1.9
    float x=0.65;//0.75
    float y=-high/2.0;
    float textSize=0.03;
    GLubyte textColorR=0,textColorG=0,textColorB=0,textColorA=255;
    float textWidth=1.5;
    float titleTextSize=0.04;
    GLubyte titleTextColorR=0,titleTextColorG=0,titleTextColorB=0,titleTextColorA=255;
    float titleTextWidth=2.0;

    vector<Category> vCategory;

};

#endif // COLORCHART_HPP
