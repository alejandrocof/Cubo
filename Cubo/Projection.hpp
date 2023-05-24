#ifndef PROJECTION_HPP
#define PROJECTION_HPP

#define ACCEPT_USE_OF_DEPRECATED_PROJ_API_H
#include <iostream>
#include <string>
#include <proj_api.h>
#include <geodesic.h>
#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "Data2D.hpp"

using namespace std;

class DataRaw2D;

//Conformal cylindrical (Mercator)
//https://proj.org/operations/projections/merc.html
//+proj=merc

//Central Cylindrical
//https://proj.org/operations/projections/cc.html Central Cylindrical
//+proj=cc

//Universal Transverse Mercator (UTM)
//https://proj.org/operations/projections/utm.html
//+proj=utm +zone=33N +datum=WGS84

//Miller Cylindrical
//https://proj.org/operations/projections/mill.html
//+proj=mill

//https://community.khronos.org/t/utm-coordinate-system-projection/58529/16


//    const char* sProjIn = "+proj=longlat +datum=WGS84";
//    //const char* sProjOut = "+proj=cc";
//    const char* sProjOut = "+proj=utm +zone=33N +datum=WGS84";
//    //const char* sProjOut = "+proj=bertin1953";

//! Clase que permite realizar transformación de coordenadas de los datos de entrada
/*!
 * Esta clase está basada en la biblioteca PROJ https://proj.org el cual es un software que permite transformar coordenadas.
 */

class Projection
{
private:
    string sProjIn;// = "+proj=longlat +datum=WGS84";
    string sProjOut;// = "+proj=utm +zone=33N +datum=WGS84";
    projPJ projIn;
    projPJ projOut;
    bool bProj=false;
    struct geod_geodesic geodesical;
public:
    Projection();

    //! Lee la descripción XML que describe el tipo de transformación a realizar.
    /*!
     * \param it Es el nodo del XML de configuración.
     */
    void loadXML(pugi::xml_node_iterator it);

    //! Realiza la transformación de los datos, el vector debera contener cada coordenada con la 'x' seguida de la 'y' en el caso de coordanadas geográficas la 'longitud' seguida de la 'latitud'
    /*!
     *
     */
    int transform(std::vector<double> &xy);
    int transform(double &x, double &y);
    int transform3d(std::vector<double> &xyz);

    void geodDirect(double lat1, double lon1, double azi1, double s12, double &lat2, double &lon2, double &azi2);
    void geodInverse(double lat1, double lon1, double lat2, double lon2, double &s12, double &azi1, double &azi2);

};

#endif // PROJECTION_HPP
