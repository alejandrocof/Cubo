#ifndef LABEL_HPP
#define LABEL_HPP
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
#include "Projection.hpp"

//revisar https://learnopengl.com/In-Practice/Text-Rendering
enum alignment{aLeft,aCenter,aRight};
class Label3DClass{
  public:
    Label3DClass();
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    void addText(std::string& str);
    void position(float x, float y, float z);
    void size(float _size);
    void width(float _width);
    void color(int r, int g, int b);
    void color(int r, int g, int b, int a);
  private:
    void replaceAll(std::string& str, const std::string& from, const std::string& to);


    double x,y,z;
    std::vector<std::string> vText;
    std::vector<float> vTextSize;
    int textColorR, textColorG, textColorB, textColorA;
    float textSize;
    float textWidth;

    alignment alignmentText=aLeft;
    float xo,yo;
    int backBoxColorR, backBoxColorG, backBoxColorB, backBoxColorA;
    float backBoxBorderWidth;
    std::string backBoxShow;
    float wMaxString;
    int hString;

    Projection *proj=0;
};


#endif
