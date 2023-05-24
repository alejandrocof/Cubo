#ifndef LOADARCASCII_HPP
#define LOADARCASCII_HPP

#include <iostream>
#include <string>
#include <sstream>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "Data2D.hpp"
#include "Projection.hpp"
#include "Utils.hpp"

using namespace std;

class DataRaw2D;

class LoadArcASCII
{
public:
    //LoadArcASCII();
    void loadXML(pugi::xml_node_iterator it);
    bool load(string name);
    DataRaw2D *data2D;
    Projection *proj=0;

private:
    string fileName;
    float scale=1.0;
};

#endif // LOADARCASCII_HPP
