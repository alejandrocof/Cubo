#ifndef TEXT3D_HPP
#define TEXT3D_HPP

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
#include "Shapes.hpp"

class Projection;

enum alignmentTxt{alignmentLeft,alignmentCenter,alignmentRight};

struct sTxt{
    string str;
    double x=0,y=0,z=0;
    double xo=0,yo=0;
    int wString;
    int width=1;
    int r=0,g=0,b=0,a=255;
    double size=1.0;
    sTxt(string str,double x, double y, double z,
         int wString,double xo,double yo,int width,
         int r, int g, int b, int a,
         double size):
         str(str),x(x),y(y),z(z),xo(xo),yo(yo),
         wString(wString),width(width),
         r(r),g(g),b(b),a(a),
         size(size){};
};

class Text3D
{
public:
    Text3D(){};
    void projection(Projection *proj);
    void setWidth(float  width);
    void setSize(float  size);
    //void operator()(string str);
    void print(double x, double y,string str);
    //void addText(double longitude, double latitude, string str);
    void loadXML(pugi::xml_node_iterator it);
    void addTextlonlat(double longitude, double latitude, string str, alignmentTxt alignment);
    void addTextXY(double x, double y, string str, alignmentTxt alignment);

    void draw();

private:
    int widthString(string text);
    void alignmentString(string text, alignmentTxt alignment, int &wString, double &xo, double &yo);
    void printString(string text);
    void replaceAll(std::string& str, const std::string& from, const std::string& to);
    void replace(std::string& str);

    Projection *proj=0;
    int width=1.0;
    int r=0,g=0,b=0,a=255;
    double size=1.0;
    double x=0,y=0,z=0;
    double longitude,latitude;
    string str;

    vector<sTxt> vTxt;

};

#endif // TEXT3D_HPP


