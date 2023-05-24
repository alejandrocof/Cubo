#ifndef LOAD_SIMULATION_DATA_H
#define LOAD_SIMULATION_DATA_H

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

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "Data2D.hpp"
#include "Data3D.hpp"
#include "Squencer.hpp"

using namespace std;
class DataRaw2D;
class DataRaw3D;
class Squencer;


class LoadSimulationData{
  public:
  
    void loadXML(pugi::xml_node_iterator it);
  

    //float *getDataRaw(){return &dataRaw[0];};
    
    int getNx(){return domainSizeX;};
    int getNy(){return domainSizeY;};
    int getNz(){return domainSizeZ;};
    
    void setFileNameFormat(
      std::string locationFiles,
      std::string filenamePrefix,
      unsigned int numberDigitsSubdomainIdentifier,
      unsigned int numberDigitsTimeIndex);

    void setTheNumberOfSubdomains(
      unsigned int numberSubdomainsX,
      unsigned int numberSubdomainsY,
      unsigned int numberSubdomainsZ);
      
    bool pushSubdomainIdentifierAndSize(
      unsigned int subdomainId,
      unsigned int subdomainSizeX,
      unsigned int subdomainSizeY,
      unsigned int subdomainSizeZ);
    
      
    bool loadSubdomain(int _timeIndex);
    
    void setTimeIndexParameters(
      unsigned int initialTimeIndex,
      unsigned int endTimeIndex,
      unsigned int increaseTimeIndex);

    void setTimeSecondsParameters(
      float initialTimeSeconds,
      float increaseTimeSeconds);
      
    void increaseTimeAndUpdateData();
    
    //void setTimeIndex(
    //  unsigned int _timeIndex);
    void setTimeIndex();
      
    
    DataRaw2D *data2D;
    DataRaw3D *data3D;
    //error: field ‘sequencer’ has incomplete type
  private:
    std::vector< std::vector<float> > buffer;
    std::vector<float> dataRaw;
    std::vector<float> dataMax;
    unsigned int domainSizeX;
    unsigned int domainSizeY;
    unsigned int domainSizeZ;
    
    std::string locationFiles;
    std::vector<std::string> filenamePrefix;
    //std::string filenamePrefix;
    unsigned int numberDigitsSubdomainIdentifier;
    unsigned int numberDigitsTimeIndex;
    
    unsigned int initialTimeIndex;
    unsigned int endTimeIndex;
    unsigned int increaseTimeIndex;
    
    float initialTimeSeconds;
    float increaseTimeSeconds;
    
    unsigned int numberSubdomainsX;
    unsigned int numberSubdomainsY;
    unsigned int numberSubdomainsZ;
    bool completedFilling=false;
    
    unsigned int timeIndex;
    unsigned int subX;
    unsigned int subY;
    unsigned int subZ;
    
    unsigned int maximumBufferSize=0;
    
    float Scale=1.0;
    bool useMinimumMaximum=true;
    bool useMinimumMaximumCell=true;
    struct organizationData{
      unsigned int id;
      unsigned int Nx;
      unsigned int Ny;
      unsigned int Nz;
      unsigned int Sx;
      unsigned int Sy;
      unsigned int Sz;
    };
    std::vector<std::vector<std::vector<organizationData>>> od;
    
};

#endif
