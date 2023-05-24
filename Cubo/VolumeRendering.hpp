#ifndef VOLUME_RENDERING_HPP
#define VOLUME_RENDERING_HPP

#include <string>

#include <GL/glew.h>
#include <GL/glut.h>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "ShaderHandling.hpp"
#include "TextureManager.hpp"
#include "DrawableIdentifier.hpp"
#include "ColorMap.hpp"
#include "LoadSimulationData.hpp"

class ColorMapClass;
class DataRaw3D;

class VolumeRendering{
  public:
    VolumeRendering();
    //void setData(int _Nx, int _Ny, int _Nz, unsigned char *_data);
    void setData(const DataRaw3D *_data3D);
    void update();
    void view();
    void init();
    void loadXML(pugi::xml_node_iterator node_it);
    void draw();
    void rotx(float angle);
    void roty(float angle);
    void rotz(float angle);
    void setColorMap(const ColorMapClass *color_map);
    
  private:
    Projection *proj=0;
    GLuint textureID;
    GLenum textureEnum;
    //unsigned char *data;
    int Nx,Ny,Nz;
    double center_x=0.0,center_y=0.0,center_z=0.0;
    double size_x=1.0,size_y=1.0,size_z=1.0;
    double rot_x=0.0,rot_y=0.0,rot_z=0.0;
    double px=0.5,py=0.5;
    ColorMapClass *color_map;
    DataRaw3D *data3D;
    std::string filter="NEAREST";

    
    GLuint texturest[4];
  
    ShaderHandling shaderh;
};

#endif
