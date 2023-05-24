#include "ShaderHandling.hpp"


char *ShaderHandling::textFileRead(const char *fn) {
    FILE *fp;
    char *content = NULL;
    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {
            std::cout<<"Abriendo el shader: "<<fn<<std::endl;
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }

            fclose(fp);
        }
        else{
            std::cout<<"No se pudo abrir el shader: "<<fn<<std::endl;
        }

    }
    else{
        std::cout<<"No se especifico el nombre del shader"<<std::endl;
    }


    return content;
}


void ShaderHandling::printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = new char[infologLength];
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        std::cout << infoLog << std::endl;
        delete infoLog;
    }
}

void ShaderHandling::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = new char[infologLength];
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        std::cout << infoLog << std::endl;
        delete infoLog;
    }
}

bool ShaderHandling::setShaders() {

    char *vertexSource = NULL;
    char *fragmentSource = NULL;

    vShaderObj = glCreateShader(GL_VERTEX_SHADER);
    fShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

    vertexSource = textFileRead((char*)"../Shaders/PosicionesRayos.vert");
    fragmentSource = textFileRead((char*)"../Shaders/TrazaRayos.frag");

    if( !vertexSource || !fragmentSource )
    {
        glDeleteShader(vShaderObj);
        glDeleteShader(fShaderObj);
        return false;
    }

    const char * vs = vertexSource;
    const char * fs = fragmentSource;

    glShaderSource(vShaderObj, 1, &vs,NULL);
    glShaderSource(fShaderObj, 1, &fs,NULL);

    free(vertexSource);free(fragmentSource);

    glCompileShader(vShaderObj);
    glCompileShader(fShaderObj);

    printShaderInfoLog(vShaderObj);
    printShaderInfoLog(fShaderObj);

    programObject = glCreateProgram();
    glAttachShader(programObject,vShaderObj);
    glAttachShader(programObject,fShaderObj);

    glLinkProgram(programObject);
    printProgramInfoLog(programObject);

    glUseProgram(programObject);

    glUniform1i(glGetUniformLocation(programObject,"VolumeData"),0);
    glUniform1i(glGetUniformLocation(programObject,"ColorTransferFunction"),1);
    glUniform1i(glGetUniformLocation(programObject,"AlphaTransferFunction"),2);

    float sizeVR[4];
    sizeVR[0]=2.0;
    sizeVR[1]=1.0;
    sizeVR[2]=1.0;
    sizeVR[3]=1.0;
    //glProgramUniform4fv( programObject, glGetUniformLocation(programObject, "sizeVR"), 1, sizeVR);
    glUniform4fv( glGetUniformLocation(programObject,"sizeVR"), 1, sizeVR);

    //glRotatef(180.0-22.65935204, 0.0f, 0.0f, 1.0f);
    //float theta=-22.65935204*3.1416/180.0;
    /*
    float matrixVR[16] = { (float)cos(theta),(float)-sin(theta), 0.0, -104.50,
                           (float)sin(theta),(float) cos(theta), 0.0,   20.0,
                                         0.0,               0.0, 1.0,    0.0,
                                         0.0,               0.0, 0.0,    1.0 };
    glUniformMatrix4fv( glGetUniformLocation(programObject,"matrixVR"), 1, false, matrixVR);
    */
    //http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
    return true;
}


bool ShaderHandling::setShaders2() {

    char *vertexSource = NULL;
    char *fragmentSource = NULL;

    vShaderObj = glCreateShader(GL_VERTEX_SHADER);
    fShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

    vertexSource = textFileRead((char*)"../Shaders/Texture2D.vert");
    fragmentSource = textFileRead((char*)"../Shaders/Texture2D.frag");

    if( !vertexSource || !fragmentSource )
    {
        glDeleteShader(vShaderObj);
        glDeleteShader(fShaderObj);
        return false;
    }

    const char * vs = vertexSource;
    const char * fs = fragmentSource;

    glShaderSource(vShaderObj, 1, &vs,NULL);
    glShaderSource(fShaderObj, 1, &fs,NULL);

    free(vertexSource);free(fragmentSource);

    glCompileShader(vShaderObj);
    glCompileShader(fShaderObj);

    printShaderInfoLog(vShaderObj);
    printShaderInfoLog(fShaderObj);

    programObject = glCreateProgram();
    glAttachShader(programObject,vShaderObj);
    glAttachShader(programObject,fShaderObj);

    glLinkProgram(programObject);
    printProgramInfoLog(programObject);

    glUseProgram(programObject);

//    glUniform1i(glGetUniformLocation(programObject,"Texture2D"),0);
//    glUniform1i(glGetUniformLocation(programObject,"ColorTransferFunction"),1);
//    glUniform1i(glGetUniformLocation(programObject,"AlphaTransferFunction"),2);

    //  float sizeVR[4];
    //  sizeVR[0]=2.0;
    //  sizeVR[1]=1.0;
    //  sizeVR[2]=1.0;
    //  sizeVR[3]=1.0;
    //  //glProgramUniform4fv( programObject, glGetUniformLocation(programObject, "sizeVR"), 1, sizeVR);
    //    glUniform4fv( glGetUniformLocation(programObject,"sizeVR"), 1, sizeVR);

    //glRotatef(180.0-22.65935204, 0.0f, 0.0f, 1.0f);
    //float theta=-22.65935204*3.1416/180.0;
    /*
    float matrixVR[16] = { (float)cos(theta),(float)-sin(theta), 0.0, -104.50,
                           (float)sin(theta),(float) cos(theta), 0.0,   20.0,
                                         0.0,               0.0, 1.0,    0.0,
                                         0.0,               0.0, 0.0,    1.0 };
    glUniformMatrix4fv( glGetUniformLocation(programObject,"matrixVR"), 1, false, matrixVR);
    */
    //http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
    return true;
}


bool ShaderHandling::loadFileVertexShader(std::string nameVertexShader,
                                          std::string nameFragmentShader){

    std::cout<<"ini loadFileVertexShader"<<std::endl;
    char *vertexSource = NULL;
    char *fragmentSource = NULL;
    vShaderObj = glCreateShader(GL_VERTEX_SHADER);
    fShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

    vertexSource = textFileRead(nameVertexShader.c_str());
    fragmentSource = textFileRead(nameFragmentShader.c_str());

    if( !vertexSource || !fragmentSource )
    {
        glDeleteShader(vShaderObj);
        glDeleteShader(fShaderObj);
        return false;
    }

    const char * vs = vertexSource;
    const char * fs = fragmentSource;

    glShaderSource(vShaderObj, 1, &vs,NULL);
    glShaderSource(fShaderObj, 1, &fs,NULL);

    free(vertexSource);free(fragmentSource);

    glCompileShader(vShaderObj);
    glCompileShader(fShaderObj);

    printShaderInfoLog(vShaderObj);
    printShaderInfoLog(fShaderObj);

    programObject = glCreateProgram();
    glAttachShader(programObject,vShaderObj);
    glAttachShader(programObject,fShaderObj);

    glLinkProgram(programObject);
    printProgramInfoLog(programObject);

    glUseProgram(programObject);
    std::cout<<"end loadFileVertexShader"<<std::endl;
    return true;
}

void ShaderHandling::textureLink(std::string textureName,int base){
    //glUseProgram(programObject);
    glUniform1i(glGetUniformLocation(programObject,textureName.c_str()),base);
    //glUseProgram(0);
}


void ShaderHandling::vector4fvLink(std::string textureName, float f0, float f1, float f2, float f3){
    float vf[4];
    vf[0]=f0;
    vf[1]=f1;
    vf[2]=f2;
    vf[3]=f3;
    //glUseProgram(programObject);
    glUniform4fv( glGetUniformLocation( programObject, textureName.c_str() ), 1, vf);
    //glUseProgram(0);
}

void ShaderHandling::vector1fvLink(std::string textureName, float f){
    //glUseProgram(programObject);
    glUniform1fv( glGetUniformLocation( programObject, textureName.c_str() ), 1, &f);
    //glUseProgram(0);
}

void ShaderHandling::vector1iLink(std::string textureName, int i){
    //glUseProgram(programObject);
    glUniform1i( glGetUniformLocation( programObject, textureName.c_str() ), i);
    //glUseProgram(0);
}

GLuint ShaderHandling::getAttribLocation(std::string textureName){
    //glUseProgram(programObject);
    return glGetAttribLocation( programObject, textureName.c_str() );
    //glUseProgram(0);
}

GLuint ShaderHandling::getUniformLocation(std::string textureName){
    return glGetUniformLocation( programObject, textureName.c_str() );
}

bool ShaderHandling::loadShaders(std::string nameVertexShader,
                                 std::string nameFragmentShader,
                                 const map<std::string,GLint> &attrib){

    std::cout<<"ini loadFileVertexShader"<<std::endl;
    char *vertexSource = NULL;
    char *fragmentSource = NULL;
    vShaderObj = glCreateShader(GL_VERTEX_SHADER);
    fShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

    vertexSource = textFileRead(nameVertexShader.c_str());
    fragmentSource = textFileRead(nameFragmentShader.c_str());

    if( !vertexSource || !fragmentSource )
    {
        glDeleteShader(vShaderObj);
        glDeleteShader(fShaderObj);
        return false;
    }

    const char * vs = vertexSource;
    const char * fs = fragmentSource;

    glShaderSource(vShaderObj, 1, &vs,NULL);
    glShaderSource(fShaderObj, 1, &fs,NULL);

    free(vertexSource);free(fragmentSource);

    glCompileShader(vShaderObj);
    glCompileShader(fShaderObj);

    printShaderInfoLog(vShaderObj);
    printShaderInfoLog(fShaderObj);

    programObject = glCreateProgram();
    glAttachShader(programObject,vShaderObj);
    glAttachShader(programObject,fShaderObj);

//    map<std::string,GLint>::iterator it;
//    for(unsigned int i=0; i<attrib.size(); i++)
//        glBindAttribLocation(programObject, i, attrib[i].c_str());
    for (auto const& x : attrib){
        glBindAttribLocation(programObject, x.second, x.first.c_str());
    }

    for( auto const& [key, val] : attrib )
    {
        std::cout << key         // string (key)
                  << ':'
                  << val        // string's value
                  << std::endl ;
    }

    glLinkProgram(programObject);
    printProgramInfoLog(programObject);

    glDeleteShader(vShaderObj);
    glDeleteShader(fShaderObj);
    return true;
}

GLuint ShaderHandling::getProgramObject(){
    return programObject;
}
