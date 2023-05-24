#include "Triangulate.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#define APIENTRY
#endif



//http://vterrain.org/Implementation/Libs/triangulate.html

//intersección
//https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluTessCallback.xml
//http://www.songho.ca/opengl/gl_tessellation.html



int __num_verts=10;
Vertex   *__verts = NULL; // All the verts that define the polygon.
TexCoord *__texcoord = NULL;
// here I assume that the verts are ordered like so:
//   Verts 0   to (N-1)     : defines the (clockwise) perimeter of the poly (has N verts).
//   Verts N   to (N+M-1)   : defines the (counter clockwise) perimiter of the first hole in the poly (has M verts).
//   Verts N+M to (N+M+P-1) : defines the (counter clockwise) perimiter of the second hole in the poly (has P verts).
//   ... etc for more holes.

int __num_contours = 0;        // Total number of contours in poly, should be = 1 + # holes.
int __contour_vert_count[100]; // Number of verts per contour, so we have:
//   __contour_vert_count[0] = number of verts for perimeter in poly
//   __contour_vert_count[1] = number of verts for first hole in poly
//   __contour_vert_count[2] = number of verts for second hole in poly
//   ... etc, assumes 100 max contours (or 99 max holes.)

int __num_trigs=0;

GLenum         __tess_type;      // Current type of tesselation: Strip, Fan, List.
int            __tess_idx[2];    // Keeps track of last two verts of the current triangle.
int            __tess_nidx;      // Number of indecies defined.
bool           __tess_odd;       // For triangle strips, keep track of (flipping) winding order.
int            __tess_num_trigs; // Total number of triangles generated (tesselated).
const GLubyte *__tess_err;       // An error occured.


/// These next variables are assumed to be defined by you based on your needs:






Triangle *__trigs = NULL; // Will be filled out by the tesselation code.


///



//////////////////////////////////////////////////////////////////
//
// TessErrorCallback()
//
// - Error callback handler if some problem occured while
//   tesselating.
//
//////

void APIENTRY TessErrorCallback(GLenum errorCode)
{
    __tess_err = gluErrorString(errorCode);
}

// TessErrorCallback() ///////////////////////////////////////////



//////////////////////////////////////////////////////////////////
//
// TessBeginCallback()
//
// - GLU calls this to let us know about the next list of
//   triangles (given as triangles, a triangle fan or a triangle
//   strip) in the triangluation.
//
//////

void APIENTRY TessBeginCallback(GLenum which)
{
    __tess_type = which; // How next verts should be interpreted to form triangles.
    __tess_nidx = 0;     //
    __tess_odd  = false; // Reset odd-even winding.
}

// TessBeginCallback() ///////////////////////////////////////////



//////////////////////////////////////////////////////////////////
//
// TessAddTrig()
//
// - Keep track of a new triangle defined by vertex indecies a,
//   b, and c.
//
//////

void TessAddTrig(int a, int b, int c)
{
    // Add triangle defined by a,b and c to list...
    if ( __tess_num_trigs >= __num_trigs )
    {
        fprintf(stderr, "Tried to trianglulate too many trigs!\n");
    }

    Triangle *t = __trigs + __tess_num_trigs;
    t->vidx[0] = a;
    t->vidx[1] = b;
    t->vidx[2] = c;

    __tess_num_trigs++;
}

// TessAddTrig() /////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////
//
// TessParseTriangleXXX()
//
// - Interpret last three vertex indices and keep track of the
//   triangle defined.
//
//////

void TessParseTriangle(int idx)
{
    TessAddTrig(__tess_idx[0], __tess_idx[1], idx);
    // Three unique verts make 1 triangle.
    __tess_nidx = 0;
}

void TessParseTriangleFan(int idx)
{
    TessAddTrig(__tess_idx[0], __tess_idx[1], idx);
    // First vert is anchor of fan, latest index
    // should make next triangle's first edge.
    __tess_idx[1] = idx;
}

void TessParseTriangleStrip(int idx)
{
    if ( !__tess_odd )
    { TessAddTrig(__tess_idx[0], __tess_idx[1], idx); }
    else
    { TessAddTrig(__tess_idx[0], idx, __tess_idx[1]); }

    // Last two verts should make first edge of next triangle
    __tess_idx[0] = __tess_idx[1];
    __tess_idx[1] = idx;

    __tess_odd = !__tess_odd;
}

// TessParseTriangleXXX() ////////////////////////////////////////



//////////////////////////////////////////////////////////////////
//
// TessVertexCallback()
//
// - GLU will first call TessBeginCallback() to let us know how
//   verticies passed to this function should be interepreted.
//   Verticies are then passed in one at a time, so this
//   function keeps state to track the last two verts that make
//   up a full triangle as well as the winding order if the
//   verticies define a triangle strip.
//
//////

void APIENTRY TessVertexCallback(GLvoid *vertex)
{
    // This is a bit of a hack, I assume the vertex
    // passed in is a pointer into the vertex array
    // we gave GLU, and I compute the index into
    // that array via simple pointer arithmetic.

    GLdouble *dpos = (GLdouble *) vertex;
    int vidx = (((unsigned char *)dpos) -
                ((unsigned char *)__verts)) / sizeof(Vertex);

    if ( __tess_nidx == 2 )
    {
        // If two verts were previously given, then this
        // vertex completes a triangle.
        switch (__tess_type)
        {
        case GL_TRIANGLES: TessParseTriangle(vidx); break;
        case GL_TRIANGLE_FAN: TessParseTriangleFan(vidx); break;
        case GL_TRIANGLE_STRIP: TessParseTriangleStrip(vidx); break;
        default:
            printf("Unknown tesselasion enum: %d\n", __tess_type);
            // Don't create triangles from these tesselations
            break;
        }
    }
    else
    {
        // All triangle tesselation types need at least two verts.
        __tess_idx[__tess_nidx] = vidx;
        __tess_nidx++;
    }
}

// TessVertexCallback() //////////////////////////////////////////



//////////////////////////////////////////////////////////////////
//
// TessWithGlu()
//
// - Function that uses GLU to triangulate a polygon defined by
//   lists of countours of its perimeter and any hole(s) it may
//   have.
//
//////

bool TessWithGlu(void)
{
    // Allocate space for the triangle data. A polygon with N
    // verts will have N-2 trigs in the triangulation.  If the
    // polygon has holes in it (Number of countours is > 1),
    // then it will have (N-2) + 2*(# countours - 1) instead.

    if ( __num_contours < 1 ) { return false; }

    __num_trigs = __num_verts - 2 + 2 * (__num_contours - 1);
    __num_trigs = 2*__num_trigs;

    //free(__trigs);
    __trigs = (Triangle *)malloc(__num_trigs * sizeof(Triangle));
    if ( !__trigs ) { return false; }
    //std::cout<<"Almacena memoria"<<std::endl;

    // Setup triangluation state
    __tess_type = GL_INVALID_ENUM; // No triangulation type defined.
    __tess_nidx = 0;               // No vertex indices defined.
    __tess_num_trigs = 0;          // No triangles triangulated.
    __tess_err = NULL;             // No errors.

    // Create a GLU tesselator and setup our callbacks.
    GLUtesselator *tess = gluNewTess();
    //std::cout<<"GLU_TESS_BEGIN"<<std::endl;
    gluTessCallback(tess, GLU_TESS_BEGIN,   (GLvoid (APIENTRY *) ()) &TessBeginCallback);
    //std::cout<<"GLU_TESS_VERTEX"<<std::endl;
    gluTessCallback(tess, GLU_TESS_VERTEX,  (GLvoid (APIENTRY *) ()) &TessVertexCallback);
    //std::cout<<"GLU_TESS_ERROR"<<std::endl;
    gluTessCallback(tess, GLU_TESS_ERROR,   (GLvoid (APIENTRY *) ()) &TessErrorCallback);
    //std::cout<<"GLU_TESS_WINDING_RULE"<<std::endl;
    gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

    //GLU_TESS_WINDING_ABS_GEQ_TWO
    //gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
    //gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ABS_GEQ_TWO);

    //std::cout<<"gluTessNormal"<<std::endl;
    gluTessNormal(tess, 0,0,1); // Polygon in XY-plane, +Z axis is 'up'.

    // Tesselate the polygon.  First countour is outside perimeter of
    // polygon (clockwise vertex order), and all others define holes
    // in the polygon (counter clockwise vertex order).

    //std::cout<<"gluTessBeginPolygon"<<std::endl;
    gluTessBeginPolygon(tess, NULL);

    //std::cout<<"__num_contours"<<__num_contours<<std::endl;

    Vertex *vert = __verts;
    for ( int c = 0; c < __num_contours; c++ )
    {
        int num_verts = __contour_vert_count[c];
        gluTessBeginContour(tess);
        for ( int i = 0; i < num_verts; i++ )
        {
            gluTessVertex(tess, vert->dpos, vert->dpos);
            vert++;
        }
        gluTessEndContour(tess);
    }

    //std::cout<<"gluTessEndPolygon"<<std::endl;
    gluTessEndPolygon(tess);

    //std::cout<<"gluDeleteTess"<<std::endl;
    gluDeleteTess(tess);

    // Handle errors
    if ( __tess_err )
    {
        fprintf(stderr, "Tessellation Error: %s\n", __tess_err);
        free(__trigs);
        __trigs = NULL;
        __num_trigs = 0;
        return false;
    }

    // Some sanity
    //printf("Alloced %d trigs, tesselated %d.\n", __num_trigs, __tess_num_trigs) ;
    __num_trigs = __tess_num_trigs;

    return true;
}

// TessWithGlu() /////////////////////////////////////////////////


void Triangulate::make(std::vector<std::vector<point>> &vP, int r, int g, int b, int a){
    //std::cout<<"Entrando a Triangulate::make "<<vP.size()<<std::endl;
    __num_verts=0;
    __num_contours = vP.size();
    for(unsigned int j=0;j<vP.size();j++){
        __contour_vert_count[j]=vP[j].size();
        __num_verts+=vP[j].size();
    }
    __verts=new Vertex[__num_verts];
    __texcoord=new TexCoord[__num_verts];
    int k=0;

    for(unsigned int j=0;j<vP.size();j++)
        for (auto it = vP[j].begin(); it != vP[j].end(); ++it) {
            __verts[k].dpos[0]=it->longitude;
            __verts[k].dpos[1]=it->latitude;
            __verts[k].dpos[2]=0.0;
            k++;
        }
   // std::cout<<"CORRECTO1"<<std::endl;
    if(TessWithGlu()){
        Polygon pol;
        pol.triangles.v=__verts;
        pol.triangles.trigs=__trigs;
        pol.triangles.numTrigs=__num_trigs;
        pol.r=r;
        pol.g=g;
        pol.b=b;
        pol.a=a;
        vPolygon.push_back(pol);

//        Triangles t;
//        t.v=__verts;
//        t.trigs=__trigs;
//        t.numTrigs=__num_trigs;
        //vv.push_back(t);
    }
    else{
        std::cout<<"Error: TessWithGlu()"<<std::endl;
    }
    //std::cout<<"CORRECTO2"<<std::endl;
}

void Triangulate::draw(){
    //std::cout<<"Draw triangulate size:"<<vPolygon.size()<<std::endl;
//    for(unsigned int i=0; i<vv.size(); i++){
//        glEnableClientState(GL_VERTEX_ARRAY);
//        glVertexPointer(3, GL_DOUBLE, 0, vv[i].v);
//        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//        glTexCoordPointer(2, GL_DOUBLE, 0, vv[i].tcoord);

//        glDrawElements(GL_TRIANGLES, 3*vv[i].numTrigs, GL_UNSIGNED_INT, vv[i].trigs);

//        glDisableClientState(GL_VERTEX_ARRAY);
//        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//    }



//for(unsigned int i=0; i<vv.size(); i++){
//    GLuint VertexVBOID,IndexVBOID;
//    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
//    glEnableVertexAttribArray(0);    // We like submitting vertices on stream 0 for no special reason
//    glVertexAttribPointer(3, GL_DOUBLE, 0, vv[i].v);      // The starting point of the VBO, for the vertices
//    glEnableVertexAttribArray(1);    // We like submitting texcoords on stream 2 for no special reason
//    glVertexAttribPointer(2, GL_DOUBLE, 0, vv[i].tcoord);


//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

//    glDrawElements(GL_TRIANGLES, 3*vv[i].numTrigs, GL_UNSIGNED_INT, vv[i].trigs);    // The starting point of the IBO

//    glDisableVertexAttribArray(0);
//    glDisableVertexAttribArray(1);
//    }



    glLineWidth(0);

//    srand (345);
//    for(unsigned int i=0; i<vv.size(); i++){
//        float val=(rand()%128)/254.0;
//        glColor4f(val,val,val,0.2);
//        //glColor4f((rand()%254)/254.0,(rand()%254)/254.0,(rand()%254)/254.0,0.2);
//        //glColor4f(0.5+(rand()%128)/254.0,0.5+(rand()%128)/254.0,0.5+(rand()%128)/254.0,0.8);
//        glBegin(GL_TRIANGLES);
//        for(int j=0;j<vv[i].numTrigs;j++){
//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[0]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[0]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[0]].dpos[2]);
//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[1]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[1]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[1]].dpos[2]);
//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[2]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[2]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[2]].dpos[2]);
//        }
//        glEnd();

//    }

//    for(unsigned int i=0; i<vPolygon.size(); i++){
//        glColor4ub(vPolygon[i].r,vPolygon[i].g,vPolygon[i].b,vPolygon[i].a);
//        glBegin(GL_TRIANGLES);
//        for(int j=0;j<vPolygon[i].triangles.numTrigs;j++){
//            int id0=vPolygon[i].triangles.trigs[j].vidx[0];
//            int id1=vPolygon[i].triangles.trigs[j].vidx[1];
//            int id2=vPolygon[i].triangles.trigs[j].vidx[2];
//            glVertex3f(vPolygon[i].triangles.v[id0].dpos[0],vPolygon[i].triangles.v[id0].dpos[1],vPolygon[i].triangles.v[id0].dpos[2]);
//            glVertex3f(vPolygon[i].triangles.v[id1].dpos[0],vPolygon[i].triangles.v[id1].dpos[1],vPolygon[i].triangles.v[id1].dpos[2]);
//            glVertex3f(vPolygon[i].triangles.v[id2].dpos[0],vPolygon[i].triangles.v[id2].dpos[1],vPolygon[i].triangles.v[id2].dpos[2]);
////            glVertex3f(vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[0]].dpos[0],vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[0]].dpos[1],vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[0]].dpos[2]);
////            glVertex3f(vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[1]].dpos[0],vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[1]].dpos[1],vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[1]].dpos[2]);
////            glVertex3f(vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[2]].dpos[0],vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[2]].dpos[1],vPolygon[i].triangles.v[vPolygon[i].triangles.trigs[j].vidx[2]].dpos[2]);
//        }
//        glEnd();

//    }

    //cout<<"triangles.numTrigs="<<vPolygon[i].triangles.numTrigs<<" vPolygon[i].triangles.trigs="<<vPolygon[i].triangles.trigs->size()<<endl;



    for (auto& polygon : vPolygon) {
        glColor4ub(polygon.r,polygon.g,polygon.b,polygon.a);
        glBegin(GL_TRIANGLES);
        for(int j=0;j<polygon.triangles.numTrigs;j++){
            int id0=polygon.triangles.trigs[j].vidx[0];
            int id1=polygon.triangles.trigs[j].vidx[1];
            int id2=polygon.triangles.trigs[j].vidx[2];
            glVertex3f(polygon.triangles.v[id0].dpos[0],polygon.triangles.v[id0].dpos[1],polygon.triangles.v[id0].dpos[2]);
            glVertex3f(polygon.triangles.v[id1].dpos[0],polygon.triangles.v[id1].dpos[1],polygon.triangles.v[id1].dpos[2]);
            glVertex3f(polygon.triangles.v[id2].dpos[0],polygon.triangles.v[id2].dpos[1],polygon.triangles.v[id2].dpos[2]);
        }
        glEnd();

    }
/*
    glLineWidth(2);
    for (auto& polygon : vPolygon) {
        glColor4ub(polygon.r,polygon.g,polygon.b,polygon.a);
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_LINES);
        for(int j=0;j<polygon.triangles.numTrigs;j++){
            int id0=polygon.triangles.trigs[j].vidx[0];
            int id1=polygon.triangles.trigs[j].vidx[1];
            int id2=polygon.triangles.trigs[j].vidx[2];
            glVertex3f(polygon.triangles.v[id0].dpos[0],polygon.triangles.v[id0].dpos[1],polygon.triangles.v[id0].dpos[2]);
            glVertex3f(polygon.triangles.v[id1].dpos[0],polygon.triangles.v[id1].dpos[1],polygon.triangles.v[id1].dpos[2]);
            glVertex3f(polygon.triangles.v[id2].dpos[0],polygon.triangles.v[id2].dpos[1],polygon.triangles.v[id2].dpos[2]);
        }
        glEnd();

    }
*/
//    glLineWidth(0);
//    for(unsigned int i=0; i<vv.size(); i++){
//        glColor3f(0.0,0.0,0.0);
//        glBegin(GL_LINES);
//        for(int j=0;j<vv[i].numTrigs;j++){
//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[0]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[0]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[0]].dpos[2]);
//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[1]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[1]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[1]].dpos[2]);

//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[1]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[1]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[1]].dpos[2]);
//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[2]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[2]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[2]].dpos[2]);

//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[2]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[2]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[2]].dpos[2]);
//            glVertex3f(vv[i].v[vv[i].trigs[j].vidx[0]].dpos[0],vv[i].v[vv[i].trigs[j].vidx[0]].dpos[1],vv[i].v[vv[i].trigs[j].vidx[0]].dpos[2]);
//        }
//        glEnd();

//    }

}



