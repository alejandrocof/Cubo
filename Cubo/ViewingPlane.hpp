#ifndef VIEWING_PLANE_HPP
#define VIEWING_PLANE_HPP
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
#include "Data2D.hpp"
#include "LoadSimulationData.hpp"
#include "ShaderHandling.hpp"
#include "Triangulate.hpp"
#include "vertex3d.hpp"

class ColorMapClass;
class ColorBar;
class Data2D;
class DataRaw2D;


class ViewingPlane{
  public:
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    
  private:
    float lat0,long0,lat1,long1,lat2,long2,lat3,long3;
    int sub=0;
    ColorMapClass *color_map=0;
    ColorBar *color_bar=0;
    bool viewColorBar=true;
    //Data2D *data;
    DataRaw2D *data;
    float CMapMin,CMapMax;
    bool bCMapMin,bCMapMax;


    GLuint tex;
    GLuint texColorMap;
    GLuint texAlphaMap;
    GLuint texMask;

    GLuint vertexLoc;
    GLuint texCoord0Loc;
    GLuint normalLoc;
    GLuint VertexVBOID,IndexVBOID;

    GLuint texDataLoc;
    GLuint texColorMapLoc;
    GLuint texAlphaMapLoc;
    GLuint texMaskDataLoc;
    //static GLenum texture;

    ShaderHandling shaderh;
    Triangulate *triang=NULL;

    bool remove=false;
    float removeValue=0.0;
    bool showValues=false;
    float sizeText=1;
    float widthText=4;
    int precision=1;
    int isosurface_N=0;
    float isosurface_width=1.0;
    int is3D=0;
    std::string filter="NEAREST";
    int CBarPrecision=0;

    bool mode3D=false;
    float m3D=0.0;
    float b3D=0.0;
    std::string position="ABSOLUTE";//ABSOLUTE RELATIVE

    bool gridShow=false;
    float gridWidth=1.0;
    int gridColorR=0, gridColorG=0, gridColorB=0, gridColorA=255;

    void setTexture2D();
    void setColorMap();
    void setSurface();
    void makeGrid();

    vector<Vertex3D> pvertex;
    vector<Quad> pindices;
    vector<V3D> points;
    float pointsRadio=1.0;
    int  readFileApprox(string fileName);

};


#endif
