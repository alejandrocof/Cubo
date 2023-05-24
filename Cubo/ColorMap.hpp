#ifndef COLOR_MAP_HPP
#define COLOR_MAP_HPP
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
#include "TextureManager.hpp"
#include "ColorBar.hpp"
#include "ViewingPlane.hpp"

class ColorMapClass{
  public:
    ColorMapClass();
    void update();
    void pushColor(GLubyte r,GLubyte g,GLubyte b);
    void pushColor(GLubyte r,GLubyte g,GLubyte b,int k);
    void pushAlpha(GLubyte alpha);
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    void transferLambda(float val, float &lambda);
    void transferColor(float val, int &r, int &g, int &b);
    void transferColorN(float val, GLubyte &r, GLubyte &g, GLubyte &b);
    void transferAlpha(float val, int &alpha);
    int transferR(float val);
    int transferG(float val);
    int transferB(float val);
    int transferAlpha(float val);
    float min(){return minimumValue;}
    float max(){return maximumValue;}
    void min(float min){if(!bAbsMinAvailable)minimumValue=min;}
    void max(float max){if(!bAbsMaxAvailable)maximumValue=max;}
    bool absMin(float min){bAbsMinAvailable=true;minimumValue=min;}
    bool absMax(float max){bAbsMaxAvailable=true;maximumValue=max;}

    //unsigned char *get();
    
  private:
    std::vector<GLubyte> lutColor;
    std::vector<GLubyte> lutAlpha;
    GLuint colorTextureID;
    GLuint alphaTextureID;
    GLenum colorTextureEnum;
    GLenum alphaTextureEnum;
    float minimumValue=0.0;
    float maximumValue=0.0;
    bool bAbsMinAvailable=false;
    bool bAbsMaxAvailable=false;
    vector<string> label;

    
  friend class ColorBar;
  friend class ViewingPlane;
  friend class VolumeRendering;
  friend class Interpolation;
  friend class RegistryGraph;
};

#endif


