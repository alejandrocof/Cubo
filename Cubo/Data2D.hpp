#ifndef DATA_2D_HPP
#define DATA_2D_HPP
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


class DataRaw2D{
  public:
    int column, row;
    std::vector<float> val;
    std::vector<unsigned char> mask;
    std::vector<double> xy_linear_o;
    std::vector<double> xy_linear_t;
    std::vector<double> xy_nearest_o;
    std::vector<double> xy_nearest_t;
    std::vector<double> stroke;

    float xomin,yomin,xomax,yomax;
    double xtmin,ytmin,xtmax,ytmax;
    bool wearMask=false;
    float min=std::numeric_limits<float>::max();
    float max=-std::numeric_limits<float>::max();
    float minAxis;
    float maxAxis;
    float xmin,ymin,xmax,ymax;
    std::string name;
};

class Data2D{
  public:
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    DataRaw2D *data;
    //DataRaw2D data;
    
  private:
    std::string fileName;
    std::string startWith;
    
    //int column, row;
    //std::vector<float> val;
    int jumpLines=0;
    
    bool isStaticName;
    std::string locationFile;
    std::string fileNamePrefix;
    int numberOfIntegerDigits;
    int numberOfDecimalDigits;
    std::string variableIndexGetId;
    std::string fileNameSuffix;
    
    int spaceDecimalPoint;
    int horizontal=0,vertical=0;
    
  friend class ViewingPlane;
};

#endif
