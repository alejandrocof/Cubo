#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <unordered_map>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"

using namespace std;
class Global
{
public:
    static string imagePath;
    static string imageName;
    static unordered_map< string, string > mapLabel;
    Global();
    void loadXML(pugi::xml_node_iterator node_it);
    //static string strFind(string value);



};


string replaceGlobalId(string value);
//string strFindqwert();
#endif // GLOBAL_HPP
