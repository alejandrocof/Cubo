#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP
#include <vector>
#include <GL/glut.h>
#include <iostream>

class TextureManager{
  public:
    inline GLenum newTexture(){std::cout<<"TextureManager="<<account<<std::endl;return account++;};
    //void deleteTexture(int _id);
    
  private:
    static GLenum account;
};

#endif
