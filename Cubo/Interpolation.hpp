#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <typeinfo>
#include <cmath>
#include <vector>
#include <limits>
#include <map>

#include<functional>

#include <GL/glew.h>
#include <GL/glut.h>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "ColorMap.hpp"
#include "Data2D.hpp"
#include "Projection.hpp"
#include "Stroke2D.hpp"
#include "LoadSimulationData.hpp"
#include "Utils.hpp"
//#include "ShaderHandling.hpp"
#include "Triangulate.hpp"

#include <functional>

#include "Eigen/Dense"
#include <limits>
//#include <boost/algorithm/string.hpp>





using namespace std;
using namespace Eigen;
//using namespace boost;

class ColorMapClass;
class Data2D;
class DataRaw2D;
class Stroke2D;



//! Clase para interpolación de datos
/*!
 * Esta clase permite interpolar datos del tipo:\n
 *   - Interpolación de datos dispersos por medio de funciones radiales.\n
 * Las funciones de base radial pueden ser usadas para realizar una interpolación con datos distribuidos de forma aleatoria, si se tiene una distribución de  nodos  sobre una región acotada  y un conjunto de datos  que determina el valor a interpolar para cada nodo, se puede construir una función  como una combinación lineal de  funciones radiales de manera que pasen por dicha distribución de datos.\n
 * Función de interpolación.\n
 *     \f$ \sigma\left( x,y \right) = \sum_{j=1}^{n}\lambda_{j}\phi_{j}\left(r\right) + ax + by + c \f$.\n
 * Función radial multicuádrica.\n
 *     \f$ \phi_{j}\left(r\right)=\sqrt{r^2+\varepsilon^2} \qquad r=\sqrt{\left(x-x_j\right)^2 + \left(y-y_j\right)^2} \f$.\n
 * Condiciones de momentos.\n
 *     \f$  \sum_{j=1}^{n}x_j\lambda_{j}=\sum_{j=1}^{n}y_j\lambda_{j}=\sum_{j=1}^{n}\lambda_{j}=0 \f$.\n
 *    Ejemplo para archivo de configuración en XML\n
 * \code
 *    <Interpolation Id="Nombre">
 *      <DataFile type="scattered" name="./DatosDispersos.txt" nVar="3"/>
 *    </Interpolation>
 * \endcode
 *   - (En desarrollo) Interpolación de datos sobre una malla regular uniforme usando interpolación lineal.\n
 *    Ejemplo para archivo de configuración en XML\n
 * \code
 *    <Interpolation Id="Nombre">
 *      <DataFile type="regularGrid" name="./DatosMalla.txt" nVar="3"/>
 *    </Interpolation>
 * \endcode
 */

//struct PointMonotone{
//	double x, y;

//	bool operator <(const PointMonotone &p) const {
//		return x < p.x || (x == p.x && y < p.y);
//	}
//};

class Interpolation{
  public:

    //! Constructor de la interpolación.
    /*!
     * \param id Es el identificador de la interpolación a construir.
     */
    Interpolation(string id);

    //! Lee la descripción XML para construir la interpolación.
    /*!
     * \param it Es el nodo del XML de configuración.
     */
    void loadXML(pugi::xml_node_iterator it);

    //! Se genera la visualización de los datos interpolados de acuerdo a la descripción definida en el XML.
    /*!
     *
     */
    void draw();
  private:

    struct pointi{
      float latitude;
      float longitude;
      float value;
    };

    struct point{
      float latitude;
      float longitude;
      float value;
      std::string name;

      bool operator <(const point &p) const {
          return longitude < p.longitude || (longitude == p.longitude && latitude < p.latitude);
      }
    };

    struct rg{
      DataRaw2D *data2D;
      double xi,yi,xf,yf;
      double xcellsize;
      double ycellsize;
    };

    float cross(const point &O, const point &A, const point &B){
        return (A.longitude - O.longitude) * (B.latitude - O.latitude) - (A.latitude - O.latitude) * (B.longitude - O.longitude);
    }

    vector<point> convex_hull(vector<point> &P){
        int n = P.size(), k = 0;
        vector<point> H(2*n);
        // Sort points lexicographically
        sort(P.begin(), P.end());
        // Build lower hull
        for (int i = 0; i < n; i++) {
            while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
            H[k++] = P[i];
        }
        // Build upper hull
        for (int i = n-2, t = k+1; i >= 0; i--) {
            while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
            H[k++] = P[i];
        }
        H.resize(k);
        return H;
    }

//    int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy){
//        int i, j, c = 0;
//        for (i = 0, j = nvert-1; i < nvert; j = i++) {
//            if ( ((verty[i]>testy) != (verty[j]>testy)) &&
//                 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
//                c = !c;
//        }
//        return c;
//    }

    //Determina si un punto se encuentra al interior del poligono de puntos del contorno
    int pnpoly(float longitud, float latitud){
        int i, j, c = 0;
        int n=vStrokeCovexHull.size();
        for (i = 0, j = n-1; i < n; j = i++) {
            if ( ((vStrokeCovexHull[i].latitude>latitud) != (vStrokeCovexHull[j].latitude>latitud)) &&
                 (longitud < (vStrokeCovexHull[j].longitude-vStrokeCovexHull[i].longitude) * (latitud-vStrokeCovexHull[i].latitude) / (vStrokeCovexHull[j].latitude-vStrokeCovexHull[i].latitude) + vStrokeCovexHull[i].longitude) )
                c = !c;
        }
        return c;
    }


//    vector<point> convex_hull(vector< VectorXd > &P)
//    {
//        int n = P.size(), k = 0;
//        vector<point> H(2*n);
//        // Sort points lexicographically
//        sort(P.begin(), P.end());
//        // Build lower hull
//        for (int i = 0; i < n; i++) {
//            while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
//            H[k++] = P[i];
//        }
//        // Build upper hull
//        for (int i = n-2, t = k+1; i >= 0; i--) {
//            while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
//            H[k++] = P[i];
//        }
//        H.resize(k);
//        return H;
//    }

    
    int nVar=0;
    int cVar=0;
    int nData=0;
    int nX=0;

    /**variables de arreglos regular grid**/
    int nXP=0;/**variables de arreglos regular grid**/
    int nXRG=0; //regular grid
    bool mask=true;
    int typeOfData;
    VectorXd lambda;
    vector< VectorXd > nTuple;
    vector< VectorXd > nTupleNorm;
    vector< VectorXd > nV;
    vector< VectorXd > nVRG;
    vector< double > nV_min;
    vector< double > nVRG_min;
    vector< double > nV_max;
    vector< double > nVRG_max;
    vector< double > nTmin;
    vector< double > nTmax;
    vector<vector<pointi>> point_interpola;
    double Tmin,Tmax;
    double vmin,vmax;
    bool bmin=false,bmax=false;
    double threshold_min,threshold_max;
    bool b_th_min=false,b_th_max=false;

    double threshold_mask_min,threshold_mask_max;
    bool b_th_msk_min=false,b_th_msk_max=false;

    int buildLambda(double c=0.000001);
    double interpolateCoordinate(vector< double > &node);
    double interpolateRG(int l,vector< double > &node);
    double interpolateParameter(vector<double> &vP );
    int readFileScattered(string fileName, double scale=1.0);
    int readFileRegularGrid(string fileName, double scale=1.0);
    int readFileApprox(string fileName);

    double cellsize=0.0;
    bool positionDependence=true;
    bool cellsizemask=false;
    double scale=1.0;

    bool calculate=false;
    bool save=false;
    string fileNameSaveRG;
    string fileNameSaveScatt;
    string fileNameSaveApprox;

    
    double lat0,long0,lat1,long1,lat2,long2,lat3,long3;
    double xmin,ymin,xmax,ymax;
    int row,column;
    
    //static std::vector<std::function<float(vector< float > &node)>> vInterpolation;
//    static vector<Interpolation> vInterpolation;
    static unordered_map< string, Interpolation* > mInterpolation;
    vector < Interpolation* > vInterpolation;
    vector < rg* > vInterpolationRG;
    
    
    //float dlat0,dlong0,dlat1,dlong1,dlat2,dlong2,dlat3,dlong3;
    
    //ColorMapClass *color_map;
    DataRaw2D *data;
    Projection *proj=0;
    std::string IdProy;

    string outputDataID;
    
    std::vector<point> vPoints;
    std::vector<point> vPoints2;
    std::vector<point> vApprox;
    std::vector<point> vPointsCovexHull;
    std::vector<point> vStrokeCovexHull;
    Stroke2D *stroke2D;
    


    //GLuint tex;
    //GLuint texColorMap;
    //GLuint texAlphaMap;
    //ShaderHandling shaderh;
    //Triangulate *triang=NULL;

//    void setTexture2D();
//    void setColorMap();
    //void init();
    int averaging_i=1;

    int averaging_o=1;

    float c=0.000001;
    float ck;
    std::function<double(double,double)> phi;
    //std::function<int(int)> fn4 = [](int x){return x/4;};
    //float pixels[12];


};



#endif
