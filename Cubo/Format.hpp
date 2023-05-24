#ifndef FORMAT_HPP
#define FORMAT_HPP
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "Utils.hpp"

class Format{
  public:
    void loadXML(pugi::xml_node_iterator it);
    //void draw();

    template<typename T>
    std::string type2str(T val){
      std::stringstream ssVal;
      ssVal.copyfmt(stringFormat);
      ssVal<<val;
      return ssVal.str();
    };

  private:
    std::stringstream stringFormat;
    std::string notation;
    int width, precision;
    unsigned char fill;
    
};


#endif


