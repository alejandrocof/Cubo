#ifndef DISPLAY_SETTINGS_HPP
#define DISPLAY_SETTINGS_HPP
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <cmath>
#include <limits>
#include <map>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glut.h>

#include "pugixml.hpp"
#include "ConfigurationFileReader.hpp"

class DisplaySettings{
  public:
    void draw();
    static void loadXML(pugi::xml_node_iterator it);
    static inline float bg_R(){return bg_r/255.0f;};
    static inline float bg_G(){return bg_g/255.0f;};
    static inline float bg_B(){return bg_b/255.0f;};
    static inline float bg_A(){return bg_a/255.0f;};
    static inline float lookAt_X(){return lookAtX;};
    static inline float lookAt_Y(){return lookAtY;};
    static inline float lookAt_Z(){return lookAtZ;};
    static inline float distance(){return _distance;};
    static inline double dL(){return _dL;};
    static inline int windowSizeWidth(){return _windowSizeWidth;};
    static inline void windowSizeWidth(int width){_windowSizeWidth=width;};
    static inline int windowSizeHeight(){return _windowSizeHeight;};
    static inline void windowSizeHeight(int height){_windowSizeHeight=height;};
    
  private:
    static int bg_r, bg_g, bg_b, bg_a;
    static double lookAtX,lookAtY,lookAtZ;
    static double _distance;
    static double _dL;
    static int _windowSizeWidth;
    static int _windowSizeHeight;

};


#endif
