#ifndef COMPUTE_H
#define COMPUTE_H

#include <fstream>
#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "Data2D.hpp"
#include "LoadSimulationData.hpp"

class DataRaw2D;

class Compute
{
public:
    //Compute();
    void loadXML(pugi::xml_node_iterator it);

private:
    DataRaw2D *data_inA=0;
    DataRaw2D *data_inB=0;
    DataRaw2D *data_inC=0;
    DataRaw2D *data_out=0;
    string operation;
    double pK=6.79e-6;
    double pE=1.9;
};

#endif // COMPUTE_H
