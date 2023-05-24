#ifndef CONFIGURATION_FILE_READER_HPP
#define CONFIGURATION_FILE_READER_HPP
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
#include <unordered_map>

#include <GL/glew.h>
#include <GL/glut.h>

#include<functional>

#include <typeinfo>


#include "pugixml.hpp"
#include "DrawableIdentifier.hpp"


#include "MapPoint.hpp"
#include "Stroke2D.hpp"
#include "Label.hpp"
#include "DisplaySettings.hpp"
#include "VolumeRendering.hpp"
#include "ColorMap.hpp"
#include "ColorBar.hpp"
#include "Shapes.hpp"
#include "LoadShapeFileCSV.hpp"
#include "ViewingPlane.hpp"
#include "Data2D.hpp"
#include "Data3D.hpp"
#include "Squencer.hpp"
#include "Format.hpp"
#include "LoadSimulationData.hpp"
#include "Grid2D.hpp"
#include "Interpolation.hpp"
#include "RegistryGraph.hpp"
#include "Compute.hpp"
#include "Projection.hpp"
#include "LoadArcASCII.hpp"
#include "ColorChart.hpp"
#include "BarScale.hpp"
#include "Text3D.hpp"
#include "Distance.hpp"
#include "Mesh.hpp"
#include "Global.hpp"

//#include "C3_Texture_Text.hpp"
#include "C3_GL_Text.hpp"

using namespace std;

class Global;
class DisplaySettings;


class ConfigurationFileReader{
  public:
    //ConfigurationFileReader();
    bool load(char *fileName);
    
  private:
   // DrawableIdentifier drawable_id;
    C3_Texture_Text* font;
};

string replaceGlobalId(string value);
//string Global::strFind(string value);
template<typename T>
bool ifTheAttributeIsFoundAssign(const pugi::xml_attribute_iterator &it, const std::string expectedName, T &val){
  if( expectedName.compare(it->name())==0 ){
      if( stringstream(replaceGlobalId(it->value()))>>val ){
          cout<<" "<<it->name()<<"="<<val<<" ";
          return true;
      }
      else{
          cout<<endl<<"Error: In attribute "<<it->name()<<" expected value of another type"<<std::endl;
          return false;
      }
  }
  return false;
}

bool ifTheAttributeIsFoundAssignString(const pugi::xml_attribute_iterator &it, const std::string expectedName,std::string &val);


#endif



