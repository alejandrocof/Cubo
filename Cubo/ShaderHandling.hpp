#ifndef SHADER_HANDLING_HPP
#define SHADER_HANDLING_HPP
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

using namespace std;
class ShaderHandling{
  public:
    bool loadShaders(std::string nameVertexShader,std::string nameFragmentShader,const map<std::string,GLint> &attrib);
    GLuint getProgramObject();

    bool loadFileVertexShader(std::string nameVertexShader,std::string nameFragmentShader);
    void textureLink(std::string textureName,int base);
    void vector4fvLink(std::string textureName, float f0, float f1, float f2, float f3);
    void vector1fvLink(std::string textureName, float f);
    void vector1iLink(std::string textureName, int i);
    GLuint getAttribLocation(std::string textureName);
    GLuint getUniformLocation(std::string textureName);

    inline void useProgram(){ glUseProgram(programObject);};
    bool setShaders();
    bool setShaders2();
    
  private:
    char *textFileRead(const char *fn);
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);
    
    GLuint vShaderObj,fShaderObj,programObject;
};

#endif
