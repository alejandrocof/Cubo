#ifndef MAP_POINT_HPP
#define MAP_POINT_HPP
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
#include "ColorMap.hpp"
#include "ColorBar.hpp"
#include "Projection.hpp"


class ColorMapClass;
class ColorBar;
class Projection;

class MapPoint{
  public:
    MapPoint();
    void setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void setStrokeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void setStrokeWidth(float w);
    void setRadio(double radio);
    void setPoint(double latitude, double longitude, std::string name, int id=-1);
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    void replaceAll(std::string& str, const std::string& from, const std::string& to);
    
  private:

    ColorMapClass *color_map=0;
    ColorBar *color_bar=0;
    float CMapMin,CMapMax;
    bool bCMapMin=false;
    bool bCMapMax=false;

    Projection *proj=0;
    bool showz=1;
    int f_r,f_g,f_b,f_a;
    int s_r,s_g,s_b,s_a;
    int t_r,t_g,t_b,t_a;
    float w;
    double radio;
    float textWidth;
    double textSize=1.0;
    double textAngle=0.0;

    bool bStroke=false;
    bool bFill=false;

    std::string titleColorBar;
    std::string textAlignment;
    std::string fileName;


    struct point{
      unsigned char f_r,f_g,f_b,f_a;
      unsigned char s_r,s_g,s_b,s_a;
      unsigned char t_r,t_g,t_b,t_a;
      float w;
      float radio;
      double latitude=0.0;
      double longitude=0.0;
      float z=0.0;
      std::string name;
      float textWidth;
      double textSize=1.0;
      double textAngle=0.0;
      float tx,ty,tz;
      float lx,ly;
      int id;
    };
    
    int N;
    std::vector<float> shape;
    std::vector<float> stroke;
    std::vector<point> vPoints;


};

#endif
