#ifndef LOADNETCDF_H
#define LOADNETCDF_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "boost/variant.hpp"
#include <boost/algorithm/string.hpp>
#include <netcdf>
#include <time.h>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "Data2D.hpp"
#include "Squencer.hpp"
#include "Projection.hpp"
using namespace std;
using namespace netCDF;

static const int NC_ERR = 2;

class DataRaw2D;
class Squencer;
class Projection;


class LoadNetCDF
{
public:
    //    LoadNetCDF();

    //! Clase que permite cargar datos que se encuentren en formato NetCDF
    /*!
     * Esta clase está basada en la biblioteca NetCDF https://www.unidata.ucar.edu/software/netcdf/.
     */
    void loadXML(pugi::xml_node_iterator it);
    bool load(string name);
    vector<float> &operator()();
    void netCDFTime(int _time);
    int netCDFTime();
    int timeMax();

    void setTimeIndex();


    DataRaw2D *data2D;
    Projection *proj=0;
    //struct tm datei= {0};


private:


    string fileName;

    NcFile *dataFile;
    struct tm datei;
    struct tm  addinterval(struct tm x, int y, int m, int d);
    bool increaseYear=false;
    bool increaseMon=false;
    bool increaseDay=false;

    vector<float> vlon;
    vector<float> vlat;
    vector<float> vtime;
    vector<float> vdata;
    vector<size_t> start;
    vector<size_t> count;
    NcVar varData;
    size_t *iTime;
    size_t nTimes;

    void addinterval(int d,int m,int y);
};

#endif // LOADNETCDF_H
