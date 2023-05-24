#ifndef DISTANCE_HPP
#define DISTANCE_HPP

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

struct GeodesicPoint{
    double x,y;
    double n1x,n1y;
    double n2x,n2y;
};

struct sGeodesicLine{
    int strokeWidth=1.0;
    int strokeR=50;
    int strokeG=50;
    int strokeB=50;
    int strokeA=200;

    int fill1R=50;
    int fill1G=50;
    int fill1B=50;
    int fill1A=200;

    int fill2R=226;
    int fill2G=223;
    int fill2B=204;
    int fill2A=200;

    double longitude1,latitude1;
    double longitude2,latitude2;

    double dl=0.0;
    string name;
    vector<GeodesicPoint> GeodesicLine;
};

/*
struct StrokePoint{
    double lon,lat;
};
*/
class Distance
{
public:
    Distance(){};
    void projection(Projection *proj);
    void loadXML(pugi::xml_node_iterator it);
    void draw();

private:

    Projection *proj=0;

    double aspectRatio=0.05;

    int strokeWidth=1.0;
    int strokeR=50;
    int strokeG=50;
    int strokeB=50;
    int strokeA=200;

    int fill1R=50;
    int fill1G=50;
    int fill1B=50;
    int fill1A=200;

    int fill2R=226;
    int fill2G=223;
    int fill2B=204;
    int fill2A=200;

    int txtWidth=1.0;
    double txtSize=1.0;
    int txtR=0,txtG=0,txtB=0,txtA=255;

    double longitude1,latitude1;
    double longitude2,latitude2;
    int nPoints=10;
    double dl=0.0;
    double dn;
    double distance=0.0;
    string name;
    //vector<GeodesicPoint> GeodesicLine;
    vector<sGeodesicLine> GeodesicLines;
    //vector<vector<GeodesicPoint>> GeodesicStroke;
    //vector<StrokePoint> vStrokePoint;
};

#endif // DISTANCE_HPP
