#ifndef TRIANGULATE_HPP
#define TRIANGULATE_HPP
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <cmath>
#include <vector>
#include <list>
#include <limits>
#include <map>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

using namespace std;
//http://vterrain.org/Implementation/Libs/triangulate.html
//http://www.glprogramming.com/red/chapter11.html
//http://www.songho.ca/opengl/gl_tessellation.html
struct Vertex{
    double dpos[3];
};

struct Triangle{
    unsigned int vidx[3]; // Indices into the vertex array for the triangle's verts.
};

struct TexCoord {
    double xy[2];
};

struct Triangles{
    Vertex *v = NULL;
    Triangle *trigs = NULL;
    TexCoord *tcoord = NULL;
    int numTrigs=0;
};

struct Polygon{
    Triangles triangles;
    int r,g,b,a;
};

struct point{
    double latitude;
    double longitude;
};

class Triangulate{
public:
    Triangulate(){};
    void make(std::vector<std::vector<point>> &vP, int r, int g, int b, int a);
    void draw();

private:

    //vector<Triangles> vv;
    vector<Polygon> vPolygon;
};

#endif
