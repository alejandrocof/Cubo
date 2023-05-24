// ./cubo ./input/config
//PROGRAMA DE VISUALIZACION DE DATOS CON VOLUME RENDERING USANDO LA TECNICA DE RAY-CASTING
//REALIZADO POR SALAZAR SANCHEZ ALEJANDRO


//

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
#include<functional>

#include <GL/glew.h>

#include <GL/glut.h>


#include <Eigen/Geometry>

//#include <GL/freeglut.h>

#include "Shapes.hpp"
#include "Global.hpp"
#include "ColorMap.hpp"
#include "ConfigurationFileReader.hpp"
#include "DisplaySettings.hpp"
#include "Squencer.hpp"
#include "TinyPngOut.hpp"


#define DEBUG

ConfigurationFileReader configFile;

//fullscreen=true DESPLIEGA EN PANTALLA COMPLETA
//fullscreen=false DESPLIEGA EN UNA VENTANA
bool fullscreen = false;

GLint viewport[4];
bool rotateScene = false;
bool moveScene = false;
bool zoomScene = false;

float xrot = 0.0f;
float yrot = 0.0f;

float R_alpha = 0.0f;
float R_beta = 0.0f;


float translatex=0.0;
float translatey=0.0;
float zoom=1.0;

bool PerspectiveActivated=true;
float xo,yo;

Quaternion<float> q1,q2;
Quaternion<float> cameraQuaternion(1,0,0,0);
float sensitivity = 0.01f;
float qm[16]={	1.0,0.0,0.0,0.0,
                0.0,1.0,0.0,0.0,
                0.0,0.0,1.0,0.0,
                0.0,0.0,0.0,1.0};

bool saveImages=false;
bool RotateXY=false;
float alfa_x=0.0,alfa_y=0.0,alfa_z=0.0;
std::string nameppm;



//DEFINE COLOR DE FONDO Y HABILITA TEST DE PROFUNDIDAD
bool init()
{


    glClearColor( DisplaySettings::bg_R(), DisplaySettings::bg_G(), DisplaySettings::bg_B(), DisplaySettings::bg_A() );
    //glClearColor(0.4f, 0.4f, 0.8f, 0.0f);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    //glClearDepth(10.0f);



    //   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    //   GLfloat mat_shininess[] = { 100.0 };
    //   GLfloat light0_position[] = { 3.0, 1.0, 1.0, 0.0 };
    //glClearColor (0.0, 0.0, 0.0, 0.0);
    //glShadeModel (GL_SMOOTH);

    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    //   glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    //   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);


    //glEnable(GL_DEPTH_TEST);

    //glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    //glEnable(GL_MAP1_VERTEX_3);

    return true;
}

//sudo apt-get install libmagick++-dev

//FUNCION QUE PERMITE GUARDAR LAS IMAGENES GENERADAS EN UN ARCHIVO DE IMAGEN EN FORMATO PPM
//https://stackoverflow.com/questions/12157646/how-to-render-offscreen-on-opengl
void GuardaPPM(int numero_de_imagen)
{
    int i,j;
    char nombre[100];
    GLint iViewport[4];
    unsigned long lImageSize;
    glGetIntegerv(GL_VIEWPORT,iViewport);
    lImageSize=3*iViewport[2]*iViewport[3];
    char *pBits = new char [lImageSize];

    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ROW_LENGTH,0);
    glPixelStorei(GL_PACK_SKIP_ROWS,0);
    glPixelStorei(GL_PACK_SKIP_PIXELS,0);

    if(pBits){
        std::cout<<"Read pixels"<<std::endl;
        glReadPixels(0,0,iViewport[2],iViewport[3],GL_RGB,GL_UNSIGNED_BYTE,pBits);

        sprintf(nombre,"./../imagenes/%s%06d.ppm",nameppm.c_str(),numero_de_imagen);
        //sprintf(nombre,"/media/alex/3b204704-ead0-4f48-b913-c2faf189a437/videos/%s%06d.ppm",nameppm.c_str(),numero_de_imagen);

        std::ofstream archivo_imagen(nombre);
        if (archivo_imagen.is_open() ){
            std::cout<<"guardando: "<<nombre<<std::endl;
            archivo_imagen << "P6" << std::endl;
            archivo_imagen << "# " << "Mi imagen ppm"<< std::endl;
            archivo_imagen << iViewport[2] << " " << iViewport[3] << std::endl;
            archivo_imagen << 255 << std::endl;
            for(j=iViewport[3]-1;j>-1;j--){
                for(i=0;i<iViewport[2];i++){
                    archivo_imagen  << (char)(pBits[3*j*iViewport[2]+3*i]);
                    archivo_imagen  << (char)(pBits[3*j*iViewport[2]+3*i+1]);
                    archivo_imagen  << (char)(pBits[3*j*iViewport[2]+3*i+2]);
                }
            }
            archivo_imagen.close();
        }
        else{
            std::cout<<"Error: no pude guardar: "<<nombre<<std::endl;
        }
        delete[] pBits;
    }
    else{
        std::cout<<"Error read pixels"<<std::endl;
    }



}


void GuardaPNG(int numero_de_imagen)
{
    int i,j;
    //char nombre[100];
    GLint iViewport[4];
    unsigned long lImageSize;
    glGetIntegerv(GL_VIEWPORT,iViewport);
    lImageSize=3*iViewport[2]*iViewport[3];
    uint8_t *pBits = new uint8_t [lImageSize];

    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ROW_LENGTH,0);
    glPixelStorei(GL_PACK_SKIP_ROWS,0);
    glPixelStorei(GL_PACK_SKIP_PIXELS,0);

    if(pBits){
        std::cout<<"Read pixels"<<std::endl;
        glReadPixels(0,0,iViewport[2],iViewport[3],GL_RGB,GL_UNSIGNED_BYTE,pBits);

        std::stringstream text;
        string name;
        if(Global::imageName.empty()){
            name=nameppm;
        }
        else{
            name=Global::imageName;
        }

        text << Global::imagePath<<"/"<<name<<setfill ('0')<<setw(6)<<numero_de_imagen<<".png";
        //sprintf(nombre,"./../imagenes/%s%06d.png",nameppm.c_str(),numero_de_imagen);
        //sprintf(nombre,"/media/alex/3b204704-ead0-4f48-b913-c2faf189a437/videos/%s%06d.ppm",nameppm.c_str(),numero_de_imagen);
        cout<<"Guardando imagen:"<<text.str()<<endl;
        std::ofstream outX(text.str(), std::ios::binary);
        TinyPngOut pngoutX(static_cast<uint32_t>(iViewport[2]), static_cast<uint32_t>(iViewport[3]), outX);
//        pngoutX.write(pBits, static_cast<size_t>(iViewport[2]*iViewport[3]));
        //for(int j=iViewport[3]-1;j>=0;j++)
        for(int j=0;j<iViewport[3];j++)
            pngoutX.write(&pBits[3*(iViewport[3]-j-1)*iViewport[2]], static_cast<size_t>(iViewport[2]));
/*
        std::ofstream archivo_imagen(nombre);
        if (archivo_imagen.is_open() ){
            std::cout<<"guardando: "<<nombre<<std::endl;
            archivo_imagen << "P6" << std::endl;
            archivo_imagen << "# " << "Mi imagen ppm"<< std::endl;
            archivo_imagen << iViewport[2] << " " << iViewport[3] << std::endl;
            archivo_imagen << 255 << std::endl;
            for(j=iViewport[3]-1;j>-1;j--){
                for(i=0;i<iViewport[2];i++){
                    archivo_imagen  << (char)(pBits[3*j*iViewport[2]+3*i]);
                    archivo_imagen  << (char)(pBits[3*j*iViewport[2]+3*i+1]);
                    archivo_imagen  << (char)(pBits[3*j*iViewport[2]+3*i+2]);
                }
            }
            archivo_imagen.close();

        }
        else{
            std::cout<<"Error: no pude guardar: "<<nombre<<std::endl;
        }*/
        delete[] pBits;
    }
    else{
        std::cout<<"Error read pixels"<<std::endl;
    }



}

float wh;

//DESPLIEGA EN EL FORMATO DEFINIDO POR EL USUARIO
void display()
{

    //DIBUJA LA ESCENA
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    float fovy=45.0;
    if(PerspectiveActivated)
        gluPerspective(fovy, wh, 0.0f, 100.0f);
    else{
        float s=tan(0.01745329252*fovy/2.0);
        glOrtho(-wh*s, wh*s,-s, s,-100.0,100.0);
    }
    
    /*
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  
  glVertexPointer(3,GL_FLOAT,0,vVertex);
  glNormalPointer(GL_FLOAT,0,vNormals);
  glColorPointer (4, GL_FLOAT, 0, vColor);
  glDrawElements(GL_TRIANGLES,sIndexes,GL_UNSIGNED_INT,vIndexes);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_COLOR_MATERIAL);
  */

    //vr.draw();


    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(alpha, (GLfloat)width / (GLfloat)height, fMin, fMax);
    gluPerspective(fovy, wh, 0.01f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    gluLookAt(0.0f, 0.0f, DisplaySettings::distance(),
//              -translatex, -translatey, 0.0,
//              0.0f, 1.0f, 0.0f);

    gluLookAt(0.0f, 0.0f, DisplaySettings::distance(),
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);

    //glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    //glMatrixMode(GL_MODELVIEW);
    memcpy ( &qm[0], &cameraQuaternion.normalized().toRotationMatrix().data()[0], 3*sizeof(qm[0]) );
    memcpy ( &qm[4], &cameraQuaternion.normalized().toRotationMatrix().data()[3], 3*sizeof(qm[4]) );
    memcpy ( &qm[8], &cameraQuaternion.normalized().toRotationMatrix().data()[6], 3*sizeof(qm[8]) );

//    double dxt=Shapes::xtmax-Shapes::xtmin;
//    double dyt=Shapes::ytmax-Shapes::ytmin;
//    double dt=(dxt>dyt)?dxt/2.0:dyt/2.0;

//    //glTranslatef( -DisplaySettings::lookAt_X(), -DisplaySettings::lookAt_Y(), -DisplaySettings::lookAt_Z() );
//    glScaled(1.0/dt,1.0/dt,1.0/dt);
    glScaled(zoom,zoom,zoom);
    glMultMatrixf( qm );
    glTranslatef( translatex, translatey, 0 );
    //glTranslatef( -(Shapes::xtmin+Shapes::xtmax)/2.0, -(Shapes::ytmin+Shapes::ytmax)/2.0, 0 );


    //glTranslatef( -DisplaySettings::lookAt_X(), -DisplaySettings::lookAt_Y(), -DisplaySettings::lookAt_Z() );

    //glutSolidTeapot(0.5);
    //quat x;
    //Eigen::Transform tsss();


    Shapes::draw3D();
//    glBegin(GL_LINES);
//        glVertex3f(0.0, 0.0, 0.0);
//        glVertex3f(0.2, 0.0, 0.0);
//        glVertex3f(0.0, 0.0, 0.0);
//        glVertex3f(0.0, 0.2, 0.0);
//        glVertex3f(0.0, 0.0, 0.0);
//        glVertex3f(0.0, 0.0, 0.2);
//    glEnd();

    //glLoadIdentity();
    //https://www.allegro.cc/forums/thread/601538

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-wh, wh, -1.0, 1.0);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Shapes::draw2D();





    //VACIA LO DIBUJADO A LA PANTALLA E INTERCAMBIA EL BUFER PARA DIBUJAR NUEVAMENTE
    //glFlush();
    glutSwapBuffers();
}


//FUNCION QUE ES LLAMADA CADA CIERTO INTERVVALO DE TIEMPO PARA CARGAR NUEVO ARCHIVO DE DATOS A DESPLEGAR 
void TimerFunction(int)
{


    //simulationData.increaseTimeAndUpdateData();
    //vr.setData(simulationData.getNx(),simulationData.getNy(), simulationData.getNz(), simulationData.getData());

    //for ( std::vector< std::function<void()> >::iterator it = updateObject.begin() ; it != updateObject.end(); ++it)
    //      (*it)();
    //Squencer::update();
    //updateObject();

    if(saveImages){
        //GuardaPPM( Squencer::index() );
        GuardaPNG( Squencer::index() );
    }

    
    Squencer::update();
    if( Squencer::changes() ){
        Updates::update();
    }
    else{
        saveImages=false;
    }


    glutPostRedisplay();



    glutTimerFunc(50, TimerFunction, 1);
    //  glutPostRedisplay();
}


void resize(int w, int h)
{
    DisplaySettings::windowSizeWidth(w);
    DisplaySettings::windowSizeHeight(h);

    glViewport(0, 0, w, h);
    glGetIntegerv(GL_VIEWPORT,viewport);
    wh=1.0f*w/h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //gluPerspective(45.0f, 1.0f * w / h, 1.5f, 100.0f);
    gluPerspective(45.0f, 1.0f * w / h, 0.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    display();
    //glLoadIdentity();
}

/*void idle()
{
 if (!mouseDown)
 {
    xrot += 0.3f;
    yrot += 0.3f;
 }
 glutPostRedisplay();
}*/

//ESTA FUNCION SE ACTIVA CADA VEZ QUE SE USA EL TECLADO
//void keyboard(unsigned char key, int x, int y)
void keyboard(unsigned char key, int , int )
{
    std::stringstream sstr2;
    switch(key)
    {
    //AL OPRIMIR LA TECLA ESC SALE DEL PROGRAMA
    case 27 :
        exit(1);
        break;

        //AL OPRIMIR LA BARRA ESPACIADORA COMIENZA LA ANIMACION
    case ' ':
        saveImages=true;
        Squencer::init();
        Squencer::reverse(false);
        Squencer::loop(false);
        Squencer::play(true);



        break;

    case 'p':
        //GuardaPPM( Squencer::index() );
        GuardaPNG( Squencer::index() );
        break;



        //AL OPRIMIR 'c' GIRA EL OBJETO EN EL EJE Y
    case 'c':
        alfa_x=0.5;
        RotateXY=true;
        break;

        //AL OPRIMIR 'z' GIRA EL OBJETO EN EL EJE Y
    case 'z':
        alfa_x=-0.5;
        RotateXY=true;
        break;

        //AL OPRIMIR 'x' GIRA EL OBJETO EN EL EJE X
    case 'x':
        alfa_y=0.5;
        RotateXY=true;
        break;

        //AL OPRIMIR 's' GIRA EL OBJETO EN EL EJE X
    case 's':
        alfa_y=-0.5;
        RotateXY=true;
        break;

        //AL OPRIMIR 'o' HACE ZOOM ACERCANDOSE
    case 'o':
        zoom*=1.01;
        RotateXY=true;
        break;

        //AL OPRIMIR 'l' HACE ZOOM ALEJANDOSE
    case 'l':
        zoom/=1.01;
        RotateXY=true;
        break;

    case 'y':
        translatex-=0.02/zoom;
        RotateXY=true;
        break;

    case 'i':
        translatex+=0.02/zoom;
        RotateXY=true;
        break;

    case 'u':
        translatey+=0.02/zoom;
        RotateXY=true;
        break;

    case 'j':
        translatey-=0.02/zoom;
        RotateXY=true;
        break;

    case 'v':
        PerspectiveActivated=!PerspectiveActivated;
        break;

        //AL OPRIMIR 'q' LA ESCENA 3D REGRESA A SU POSICION INICIAL
    case 'q':
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        cameraQuaternion=Quaternion<float>(1.0f,0.0f,0.0f,0.0f);
        translatex=translatey=0.0;
        zoom=1.0;
        RotateXY=false;
        break;

    }
    key=0;
    if(RotateXY){
        q1 = AngleAxis<float>( alfa_x * sensitivity, Vector3f::UnitY());
        q2 = AngleAxis<float>( alfa_y * sensitivity, Vector3f::UnitX());
        cameraQuaternion=q1*q2*cameraQuaternion;
        alfa_x=alfa_y=0.0;
        RotateXY=false;
    }

    //glutTimerFunc(50, TimerFunction, 1);
    //glutPostRedisplay();
}

//void specialKeyboard(int key, int x, int y)
void specialKeyboard(int key, int , int )
{

    //ORPIMIR F1 PARA INTERCAMBIAR ENTRE DESPLIEGUE EN PANTALLA COMPLETA Y VENTANA
    if (key == GLUT_KEY_F1)
    {
        fullscreen = !fullscreen;

        if (fullscreen)
            glutFullScreen();
        else
        {
            //glutReshapeWindow(1000, 1000);
            //glutReshapeWindow(500, 500);
            glutPositionWindow(0, 0);
            glutReshapeWindow(DisplaySettings::windowSizeWidth(), DisplaySettings::windowSizeHeight() );
        }
    }

    if (key == GLUT_KEY_RIGHT)
    {
        Squencer::reverse(false);
        Squencer::play(true);
    }

    if (key == GLUT_KEY_LEFT)
    {
        Squencer::reverse(true);
        Squencer::play(true);
    }

    if (key == GLUT_KEY_UP)
    {
        Squencer::play(false);
        Squencer::reverse(false);
        Squencer::fbf(true);
    }

    if (key == GLUT_KEY_DOWN)
    {
        Squencer::play(false);
        Squencer::reverse(true);
        Squencer::fbf(true);
    }

//    if (key == GLUT_KEY_HOME)
//    {
//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
//        cameraQuaternion=Quaternion<float>(1.0f,0.0f,0.0f,0.0f);
//        translatex=translatey=0.0;
//        zoom=1.0;
//        RotateXY=false;
//    }

    if (key == GLUT_KEY_PAGE_UP ){
       DisplaySettings::windowSizeWidth(2*DisplaySettings::windowSizeWidth());
       DisplaySettings::windowSizeHeight(2*DisplaySettings::windowSizeHeight());
       glutReshapeWindow(DisplaySettings::windowSizeWidth(), DisplaySettings::windowSizeHeight() );
    }


}

void mouse(int button, int , int x, int y)//int button, int state, int x, int y
{
    //mouseDown = false;
    rotateScene=false;
    moveScene=false;
    zoomScene=false;
    xo=x;
    yo=y;
    if (button == GLUT_LEFT_BUTTON)//&& state == GLUT_DOWN
    {
        rotateScene = true;
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        moveScene = true;
    }

    if (button == GLUT_MIDDLE_BUTTON)
    {
        zoomScene = true;
    }

    //    // Wheel reports as button 3(scroll up) and button 4(scroll down)
    //       if ((button == 3) || (button == 4)) // It's a wheel event
    //       {
    //           // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
    //           if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
    //           printf("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
    //       }else{  // normal button event
    //           printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
    //       }

}

void mouseMotion(int x, int y)
{
    float dx=(x-xo)/ viewport[2];
    float dy=(y-yo)/ viewport[3];
    if (rotateScene)
    {
        q1 = AngleAxis<float>( 2.0*M_PI*dx, Vector3f::UnitY());
        q2 = AngleAxis<float>( 2.0*M_PI*dy, Vector3f::UnitX());
        cameraQuaternion=q1*q2*cameraQuaternion;
        glutPostRedisplay();
    }

    if (moveScene)
    {
        translatex+=dx*DisplaySettings::distance()/2.0;
        translatey-=dy*DisplaySettings::distance()/2.0;
        glutPostRedisplay();
    }

    if (zoomScene)
    {
        if(dy>0.0){
            zoom*=1+dy*DisplaySettings::distance();
        }
        else{
            zoom/=1-dy*DisplaySettings::distance();
        }
        glutPostRedisplay();
    }
    xo=x;
    yo=y;

}












int main(int argc, char *argv[])
{
    glutInit(&argc, argv);



    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Cubo");

    // glEnable(GL_DEPTH_TEST);

    //glutIdleFunc(idle);

    std::cout<<"Antes de glewInit()"<<std::endl;
    glewInit();
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
        printf("Ready for GLSL\n");
    else
    {
        printf("No GLSL support\n");
        exit(1);
    }

    if(argc<2){
        std::cout<<std::endl<<"Error:"<<argv[0]<<" config_file"<<std::endl<<std::endl;
        return 0;
    }
    if( !configFile.load( argv[1] ) ) return 0;
    nameppm=argv[1];
    std::size_t found = nameppm.find_last_of("/\\");
    nameppm = nameppm.substr(found+1);
    nameppm = nameppm.substr(0, nameppm.size()-4);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(DisplaySettings::windowSizeWidth(), DisplaySettings::windowSizeHeight() );
    glutReshapeWindow(DisplaySettings::windowSizeWidth(), DisplaySettings::windowSizeHeight() );
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(resize);




    if (!init())return 1;


    glutTimerFunc(50, TimerFunction, 1);

    glutMainLoop();

    return 0;
}
