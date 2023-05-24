#ifndef MESH_HPP
#define MESH_HPP

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
#include "Text3D.hpp"

struct PointIndex{
    int i=0,j=0;
    double x,y;
    string label;

    double radio=-1.0;

    int strokeWidth=1.0;
    int strokeR=50;
    int strokeG=50;
    int strokeB=50;
    int strokeA=128;

    int fillR=250;
    int fillG=100;
    int fillB=250;
    int fillA=128;
    PointIndex(){}
    PointIndex(int i, int j):i(i),j(j){}
};

struct PointCircle{
    double x,y;
    PointCircle(double x, double y):x(x),y(y){}
};


class Mesh
{
public:
    Mesh(){
        int N=16;
        for(int i=0;i<N;i++){
            circle.emplace_back( cos(2.0*i*M_PI/N), sin(2.0*i*M_PI/N));
        }
    }
    void projection(Projection *proj);
    void loadXML(pugi::xml_node_iterator it);
    void draw();
private:

    Projection *proj=0;

    int strokeWidth=1.0;
    int strokeR=50;
    int strokeG=50;
    int strokeB=50;
    int strokeA=64;

    int fill1R=50;
    int fill1G=50;
    int fill1B=50;
    int fill1A=64;

    int fill2R=226;
    int fill2G=223;
    int fill2B=204;
    int fill2A=64;

    double longitude,latitude;
    int n_E=2;
    int n_W=2;
    int n_N=2;
    int n_S=2;
    int Nx;
    int Ny;
    double dx=0.0;
    double dy=0.0;
    double alpha=0.0;
    int point_i=0;
    int point_j=0;

    //vector<MeshPoint> vPoints;
    vector<double> vPoints;
    vector<PointIndex> ijPoints;
    vector<PointCircle> circle;
    PointIndex point;

    float txtWidth=2;
    double textSize=1.0;
    Text3D *txt;

};

#endif // MESH_HPP
