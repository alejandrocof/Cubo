#ifndef STROKE_2D_HPP
#define STROKE_2D_HPP
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

class Stroke2D{
  public:
    Stroke2D();
    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void setWidth(float w);
    void setPoint(float longitude, float latitude);
    void make();
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    void projection(std::string IdProy);
    
  private:
    int r,g,b,a;
    float width;
    struct point{
      float latitude;
      float longitude;
    };
    
    Projection *proj=0;
    std::vector<point> vPoints;
    std::vector<double> stroke;
};

#endif
