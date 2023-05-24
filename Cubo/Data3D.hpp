#ifndef DATA3D_HPP
#define DATA3D_HPP

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

using namespace std;
struct Point{
  float latitude;
  float longitude;
  float z;
  int i=0;
  int j=0;
  int k=0;
  bool show=true;
  std::vector<float> y;
  float min=std::numeric_limits<float>::max();
  float max=-std::numeric_limits<float>::max();
  float minAxis;
  float maxAxis;
  string name;
};


class DataRaw3D{
  public:
    int Nx,Ny,Nz;
    float min=std::numeric_limits<float>::max();
    float max=-std::numeric_limits<float>::max();
    float minAxis;
    float maxAxis;
    std::vector<float> val;
    std::vector< Point > registry;
};

class Data3D
{
public:
    void loadXML(pugi::xml_node_iterator it);
    void draw();
    DataRaw3D *data;
    //Data3D();
private:
    std::string fileName;
    int jumpLines=0;
    //std::string startWith;
friend class VolumeRendering;
};

#endif // DATA3D_HPP
