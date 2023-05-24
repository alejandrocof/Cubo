#ifndef LOAD_CSV_HPP
#define LOAD_CSV_HPP
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include <cmath>
#include <vector>
#include <list>
#include <limits>
#include <map>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "Triangulate.hpp"
#include "Label.hpp"
#include "Projection.hpp"
#include "Utils.hpp"
#include "ColorChart.hpp"
#include "Text3D.hpp"

using namespace std;

class Label3DClass;
class Projection;
class ColorChart;
class Text3D;

enum WKT_TYPE{POINT,LINESTRING,MULTIPOINT,MULTILINESTRING,MULTIPOLYGON,POLYGON,GEOMCOLLECTION};
enum PARENTESIS{ABIERTO,CERRADO};


struct PointsWKT
{
    WKT_TYPE type;
    vector<vector<vector<point>>> p;//geometría, objetos, puntos
    std::string label;
    Label3DClass *labelWKT;
    float x,y,z;
    int r=127;
    int g=127;
    int b=255;
    int a=255;
    //vector<string> col;
};

struct Category
{
    std::string name;
    int r,g,b,a;
//    bool operator==(const Category& c) const{
//        return name == c.name;
//    }
    bool operator==(const string& name) const{
        return this->name == name;
    }

    Category(string name){
        this->name=name;
        r=127+rand()%127;
        g=127+rand()%127;
        b=127+rand()%127;
        //r=rand()%255;
        //g=rand()%255;
        //b=rand()%255;
        a=255;
    }

//    Category(string name, int j){
//        this->name=name;
//        int N=30;
//        int i=(j+5)%N;
//        float phi=M_PI*(3.0-sqrt(5.0));
//        float theta=phi*i;
//        float z=(1.0-1.0/N)*(1.0-2.0*i/(N-1.0));
//        float radio=sqrt(1.0-z*z);
//        float x=radio*cos(theta);
//        float y=radio*sin(theta);
//        cout<<i<<" ";
//        cout<<"("<<x<<","<<y<<","<<z<<") ";
//        x=fabs(x);
//        y=fabs(y);
//        z=fabs(z);
//        cout<<"("<<x<<","<<y<<","<<z<<") ";

//        r=255.0*x;
//        g=255.0*y;
//        b=255.0*z;
//        a=255;

//        cout<<"("<<r<<","<<g<<","<<b<<")"<<endl;
//    }
};

class LoadShapeFileCSV{
public:
    LoadShapeFileCSV();
    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void setWidth(float w);
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    void extraer(string line,PointsWKT &pwkt);
    float xmin=std::numeric_limits<float>::max();
    float ymin=std::numeric_limits<float>::max();
    float xmax=-std::numeric_limits<float>::max();
    float ymax=-std::numeric_limits<float>::max();
    float xtmin=std::numeric_limits<float>::max();
    float ytmin=std::numeric_limits<float>::max();
    float xtmax=-std::numeric_limits<float>::max();
    float ytmax=-std::numeric_limits<float>::max();


private:
    int markerR,markerG,markerB,markerA;
    float markerRadio=0.0;
    bool marker=false;
    float markerX[8];
    float markerY[8];
    int strokeR,strokeG,strokeB,strokeA;
    float strokeWidth;
    bool stroke=false;
    int fillR,fillG,fillB,fillA;
    bool fill=false;
    bool fillSpin=true;
    float x0,y0,x1,y1,x2,y2;
    bool cut=false;
    bool transf=false;
    float westbc,eastbc,northbc,southbc,leftbc,rightbc,bottombc,topbc;
    int labelColumn=0;
    int labelR,labelG,labelB,labelA;
    float labelSize=1.0;
    float labelWidth=1.0;
    bool boundingBox=true;

    std::string fileName;
    //std::vector<std::vector<point>> vPoints;
    std::vector<std::vector<float>> vStroke;
    std::vector<std::vector<float>> vFill;
    std::string IdTriang;

    vector<PointsWKT> WKTs;
    vector<Category> classification;
    bool bClassification=false;
    Projection *proj=0;

    Triangulate triang;

    Label3DClass *pruebaLabel;
    Text3D *text3d=0;

    ColorChart *color_chart;
};

#endif
