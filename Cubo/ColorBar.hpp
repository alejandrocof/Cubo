#ifndef COLOR_BAR_HPP
#define COLOR_BAR_HPP
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

#include <GL/glew.h>
#include <GL/glut.h>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"
#include "DrawableIdentifier.hpp"
#include "TextureManager.hpp"
#include "ColorMap.hpp"

class ColorMapClass;

//! Clase para desplegar una barra de color
/*!
 * Esta clase permite desplegar la "barra de color" de un "mapa de color" definido con anterioridad.
 */

class ColorBar{
  public:
    //! Lee la descripción XML que describe la barra de color a visualizar.
    /*!
     * \param it Es el nodo del XML de configuración.
     */
    void loadXML(pugi::xml_node_iterator it);

    //! Se visualiza la barra de color.
    /*!
     *
     */
    void draw();


    std::string titleText;

    float width=0.05,high=1.333;
    float x=0.94,y=-high/2.0;
    int precision=2;
    float textSize=0.03;
    GLubyte textColorR=0,textColorG=0,textColorB=0,textColorA=255;
    float textWidth=1.5;
    float titleTextSize=0.04;
    GLubyte titleTextColorR=0,titleTextColorG=0,titleTextColorB=0,titleTextColorA=255;
    float titleTextWidth=2.0;
    ColorMapClass *color_map;
    //std::string filter="NEAREST";
    std::string filter="LINEAR";
  private:


    void replaceAll(std::string& str, const std::string& from, const std::string& to);
    
};

#endif



