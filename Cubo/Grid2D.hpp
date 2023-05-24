#ifndef GRID_2D_HPP
#define GRID_2D_HPP
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
#include "vertex3d.hpp"
#include "Projection.hpp"
#include "LoadShapeFileCSV.hpp"

class Projection;


class Grid2D{
  public:
    Grid2D();
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    
  private:
    int r,g,b,a;
    float width;
    
    int rTics,gTics,bTics,aTics;
    float widthTics;
    
    int rLabel,gLabel,bLabel,aLabel;
    float widthLabel;
    double scaleLabel;//,sizeLabel;
    int precisionLabel;
    
    float xmin, xmax;
    float ymin,ymax;
    int mxTics, myTics;
    bool absolute=false;
    
    int rAxisTitle,gAxisTitle,bAxisTitle,aAxisTitle;
    float widthAxisTitle,sizeAxisTitle,scaleAxisTitle;
    std::string xAxisTitle,yAxisTitle;

    std::vector<float> edgeLines;
    std::vector<float> ticsLines;

    vector<Vertex3D> pvertex;
    vector<GLuint> pindices;
    vector<double> hxytics;
    vector<double> vxytics;

    ShaderHandling shaderh;

    GLuint vertexLoc;
    GLuint texCoord0Loc;
    GLuint normalLoc;
    GLuint VertexVBOID,IndexVBOID;

    Projection *proj=0;
    
    void replaceAll(std::string& str, const std::string& from, const std::string& to);
    void make();
    void setSurface();

};

#endif
