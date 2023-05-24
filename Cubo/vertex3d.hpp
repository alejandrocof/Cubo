#ifndef VERTEX3D
#define VERTEX3D

struct V3D
  {
    GLfloat x, y, z;//,nz;

    V3D(GLfloat x=0, GLfloat y=0, GLfloat z=0)
            : x(x), y(y), z(z)
        {
        }

    V3D& operator=(const V3D& a)
        {
            x=a.x;
            y=a.y;
            z=a.z;
            return *this;
        }

    V3D operator+(const V3D& a) const
        {
            return V3D(a.x+x, a.y+y, a.z+z);
        }

    V3D operator-(const V3D& a) const
        {
            return V3D(x-a.x, y-a.y, z-a.z);
        }

    V3D operator/(const GLfloat& q) const
        {
        //cout<<"("<<x<<","<<y<<","<<z<<")/"<<q<<"=("<<x/q<<","<<y/q<<","<<z/q<<")"<<endl;
            return V3D(x/q, y/q, z/q);
        }

    //     normal
    //       N
    //       |
    //      p0 ---p2
    //      /
    //    p1
    //https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal

    V3D normal(const V3D& p1,const V3D& p2){
        V3D u=p1-*this;
        V3D v=p2-*this;
        return V3D(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x).normalized();
    }

    GLfloat norm(){
        return sqrt(x*x+y*y+z*z);
    }

    V3D normalized(){
        return *this/this->norm();
    }

    void normalize(){
//        V3D V=V3D(10,5,6);
//        cout<<"antes ("<<V.x<<","<<V.y<<","<<V.z<<")"<<endl;
//        V = V/V.norm(); //this->norm();
//        cout<<"despues ("<<V.x<<","<<V.y<<","<<V.z<<")"<<endl;
        *this = *this/this->norm();
    }


};

struct Vertex3D
  {
    V3D p;        // Vertex
    V3D n;     // Normal
    GLfloat s0, t0;         // Texcoord0
  };

struct Quad
  {
    GLuint a,b,c,d;
  };

struct Line
  {
    GLuint a,b;
  };

#endif // VERTEX3D

