#ifndef BARSCALE_HPP
#define BARSCALE_HPP

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
//#include "ColorMap.hpp"
//#include "ColorBar.hpp"
#include "Projection.hpp"
#include "Text3D.hpp"

class Projection;
class Text3D;

class BarScale
{
public:
    BarScale();
    void loadXML(pugi::xml_node_iterator it);
    void draw();
private:
    Projection *proj=0;
    double size=0.0;
    double x=0.0,y=0.0;
    double aspectRatio=0.05;
    int f_r=0,f_g=0,f_b=0,f_a=255;
    int s_r=0,s_g=0,s_b=0,s_a=255;
    int t_r=0,t_g=0,t_b=0,t_a=255;
    float w;
    float txtWidth=2;
    double textSize=1.0;
    int precisionLabel=0;
    std::vector<double> xy;
    std::vector<double> step;
    std::vector<double> xyTxt;
    std::vector<double> stepTxt;

    std::string titleBarScale;
    std::string textAlignment;

    Text3D *txt;


};

#endif // BARSCALE_HPP
