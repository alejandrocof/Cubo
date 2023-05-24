#include "VolumeRendering.hpp"


VolumeRendering::VolumeRendering(){
    std::cout<<"creando objeto VolumeRendering"<<std::endl;
    glGenTextures(1, &textureID);
    textureEnum=TextureManager().newTexture();
    std::cout<<"textureEnum"<<textureEnum<<std::endl;

}

void VolumeRendering::init(){
    std::cout<<"Carga de Sahders"<<std::endl;
    //shaderh.loadFileVertexShader("PosicionesRayos.vert","TrazaRayos.frag");
    if( !shaderh.loadFileVertexShader("../Shaders/PosicionesRayos.vert","../Shaders/TrazaRayos.frag") ){
        std::cout<<std::endl<<"ERROR en shader de VolumeRendering!"<<std::endl;
    }
    //shaderh.textureLink("VolumeData",textureEnum);
    //shaderh.textureLink("ColorTransferFunction",1);
    //shaderh.textureLink("AlphaTransferFunction",2);
    shaderh.vector4fvLink("sizeVR", 1.0, 1.0, 1.0, 1.0);
}

void VolumeRendering::setData(const DataRaw3D *_data3D){
    //shaderh.textureLink("VolumeData",textureEnum);
    //glActiveTexture(GL_TEXTURE0+textureEnum);
    int N=_data3D->Nx*_data3D->Ny*_data3D->Nz;
    unsigned char *data=new unsigned char[N];
    for(int i=0;i<N;i++){
        if(_data3D->val[i]<_data3D->minAxis) data[i]=0;
        else{
            if(_data3D->val[i]>_data3D->maxAxis) data[i]=255;
            else
                data[i]=255.0*(_data3D->val[i]-_data3D->minAxis)/(_data3D->maxAxis-_data3D->minAxis);
        }

        //cout<<_data3D->val[i]<<"->"<<(int)data[i]<<endl;

    }

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D,texturest[0]);
    if( filter.compare("LINEAR")==0 ){
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if( filter.compare("NEAREST")==0 ){
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//GL_CLAMP GL_CLAMP_TO_EDGE GL_CLAMP_TO_BORDER


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glTexImage3D(GL_TEXTURE_3D,0,GL_ALPHA,Nx,Ny,Nz,0,GL_ALPHA,GL_UNSIGNED_BYTE,data);
    glTexImage3D(GL_TEXTURE_3D,0,GL_ALPHA,_data3D->Nx,_data3D->Ny,_data3D->Nz,0,GL_ALPHA,GL_UNSIGNED_BYTE,data );
    glDisable(GL_TEXTURE_3D);
    //glBindTexture(GL_TEXTURE_3D, 0);

    delete[] data;

}

void VolumeRendering::setColorMap(const ColorMapClass *color_map){
    std::cout<<"Carga de ColorTransferFunction"<<std::endl;
    //shaderh.textureLink("ColorTransferFunction",color_map->colorTextureEnum);

    /*
  glActiveTexture(GL_TEXTURE0+color_map->colorTextureEnum);
  glEnable(GL_TEXTURE_1D);
  glBindTexture(GL_TEXTURE_1D,color_map->colorTextureID);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,color_map->lutColor.size()/3 ,0,GL_RGB,GL_UNSIGNED_BYTE,&(color_map->lutColor[0]) );
  glDisable(GL_TEXTURE_1D);
  glBindTexture(GL_TEXTURE_1D, 0);
  */

    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D,texturest[1]);
    glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,color_map->lutColor.size()/3 ,0,GL_RGB,GL_UNSIGNED_BYTE,&(color_map->lutColor[0]) );
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glDisable(GL_TEXTURE_1D);

    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D,texturest[2]);
    glTexImage1D(GL_TEXTURE_1D,0,GL_ALPHA,color_map->lutAlpha.size(),0,GL_ALPHA,GL_UNSIGNED_BYTE,&(color_map->lutAlpha[0]) );
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glDisable(GL_TEXTURE_1D);

}







void VolumeRendering::update(){
    //std::cout<<"VolumeRendering::update()"<<std::endl;
    //setColorMap(color_map);
    setData(data3D);
    //glActiveTexture(GL_TEXTURE0);

    /*
  glActiveTexture(GL_TEXTURE0+textureEnum);
  
  glBindTexture(GL_TEXTURE_3D,textureID);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//GL_CLAMP GL_CLAMP_TO_EDGE GL_CLAMP_TO_BORDER
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage3D(GL_TEXTURE_3D,0,GL_ALPHA,Nx,Ny,Nz,0,GL_ALPHA,GL_UNSIGNED_BYTE,data);
  glBindTexture(GL_TEXTURE_3D, 0);
  */
}

void VolumeRendering::view(){

    //glutSolidCube(1.0);

}


void VolumeRendering::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("CenterPosition")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", center_y);
                ifTheAttributeIsFoundAssign(ait, "long", center_x);
                ifTheAttributeIsFoundAssign(ait, "z", center_z);
                ifTheAttributeIsFoundAssign(ait, "px", px);
                ifTheAttributeIsFoundAssign(ait, "py", py);
            }
            std::cout<<std::endl;
            std::cout<<"*********************************************"<<std::endl;
            std::cout<<"*********************************************"<<std::endl;
            std::cout<<"Center Position ( "<<center_x<<", "<<center_y<<", "<<center_z<<")"<<std::endl;
            std::cout<<"*********************************************"<<std::endl;
            std::cout<<"*********************************************"<<std::endl;
            std::cout<<std::endl;
        }

        if( nodeName.compare("Size")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lx", size_x);
                ifTheAttributeIsFoundAssign(ait, "ly", size_y);
                ifTheAttributeIsFoundAssign(ait, "lz", size_z);
            }
        }
        if( nodeName.compare("RotX")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                float rot=0.0;
                ifTheAttributeIsFoundAssign(ait, "angle", rot);
                rot_x+=rot;
            }
        }
        if( nodeName.compare("RotY")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                float rot=0.0;
                ifTheAttributeIsFoundAssign(ait, "angle", rot);
                rot_y+=rot;
            }
        }
        if( nodeName.compare("RotZ")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                float rot=0.0;
                ifTheAttributeIsFoundAssign(ait, "angle", rot);
                rot_z+=rot;
            }
        }

        if( nodeName.compare("Texture")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "filter", filter);
            }
        }

        if( nodeName.compare("ColorMap")==0 ){
            std::string IdName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "getId", IdName);

                //const ColorMap *color_map=static_cast<const ColorMap *>(DrawableIdentifier::data(IdName));
                //const ColorMap *color_map=DrawableIdentifier::data(IdName);
                //::data(IdName);
                //const ColorMap *color_map=static_cast<const ColorMap *>(DrawableIdentifier::data(IdName));
                //const ColorMap *color_map=DrawableIdentifier::data<const ColorMap *>(IdName);


                //const ColorMap *color_map=DrawableIdentifier::data(IdName);
                //const ColorMap *color_map;
                DrawableIdentifier::data(IdName,color_map);

                std::cout<<"ColorMap="<<(int)color_map->lutColor[0]<<","<<(int)color_map->lutColor[1]<<","<<(int)color_map->lutColor[2]<<std::endl;
            }
        }


        if( nodeName.compare("Data")==0 ){
            std::string IdName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "getId", IdName);
            }
            DrawableIdentifier::data(IdName,data3D);
        }

        if( nodeName.compare("Projection")==0 ){
            std::string IdProy;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdProy);
            }
            DrawableIdentifier::data(IdProy,proj);
        }

        std::cout <<"/>"<< std::endl;
    }

    size_x*=1000;
    size_y*=1000;

    double x0,y0,x1,y1,azi1,d1,d2;
    x0=center_x;
    y0=center_y;

    proj->transform(center_x,center_y);

    cout<<x0<<" "<<y0<<endl;
    proj->geodDirect(y0,x0, 90, size_x, y1, x1, azi1);
    proj->transform(x1,y1);
    d1=sqrt((center_x-x1)*(center_x-x1)+(center_y-y1)*(center_y-y1));
    cout<<x1<<" "<<y1<<" d2="<<d1<<endl;
    proj->geodDirect(y0,x0, 270, size_x, y1, x1, azi1);
    proj->transform(x1,y1);
    d2=sqrt((center_x-x1)*(center_x-x1)+(center_y-y1)*(center_y-y1));
    cout<<x1<<" "<<y1<<" d1="<<d2<<endl;
    size_x=(d1+d2)/2.0;
    cout<<"size_x="<<size_x<<endl;

    proj->geodDirect(y0,x0, 0, size_y, y1, x1, azi1);
    proj->transform(x1,y1);
    d1=sqrt((center_x-x1)*(center_x-x1)+(center_y-y1)*(center_y-y1));
    cout<<x1<<" "<<y1<<" d2="<<d1<<endl;
    proj->geodDirect(y0,x0, 180, size_y, y1, x1, azi1);
    proj->transform(x1,y1);
    d2=sqrt((center_x-x1)*(center_x-x1)+(center_y-y1)*(center_y-y1));
    cout<<x1<<" "<<y1<<" d1="<<d2<<endl;
    size_y=(d1+d2)/2.0;
    cout<<"size_y="<<size_y<<endl;

    cout<<"size_x="<<size_x<<" size_y="<<size_y<<endl;

    //center_x-=size_x*(px-0.5);
    //center_y-=size_y*(py-0.5);

    if(Shapes::xmin>center_x-0.5*size_x)Shapes::xmin=center_x-0.5*size_x;
    if(Shapes::xmax<center_x+0.5*size_x)Shapes::xmax=center_x+0.5*size_x;
    if(Shapes::ymin>center_y-0.5*size_y)Shapes::ymin=center_y-0.5*size_y;
    if(Shapes::ymax<center_y+0.5*size_y)Shapes::ymax=center_y+0.5*size_y;

    std::cout<<"cargando shaders"<<std::endl;
    //shaderh.setShaders();

    if(!shaderh.setShaders()){
        std::cout<<"ERROR en shader!!!"<<std::endl;
    }
    shaderh.vector4fvLink("sizeVR", size_x, size_y, size_z, 1.0);

    glGenTextures(1,&texturest[0]);
    glGenTextures(1,&texturest[1]);
    glGenTextures(1,&texturest[2]);
    setColorMap(color_map);
    setData(data3D);
    /*
  glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D,texturest[1]);
    glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,color_map->lutColor.size()/3 ,0,GL_RGB,GL_UNSIGNED_BYTE,&(color_map->lutColor[0]) );
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glDisable(GL_TEXTURE_1D);
    */



    /*
    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D,texturest[2]);
    glTexImage1D(GL_TEXTURE_1D,0,GL_ALPHA,color_map->lutAlpha.size(),0,GL_ALPHA,GL_UNSIGNED_BYTE,&(color_map->lutAlpha[0]) );
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glDisable(GL_TEXTURE_1D);
*/


    /*
    glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_3D);
  glBindTexture(GL_TEXTURE_3D,texturest[0]);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//http://www.java-gaming.org/index.php/topic,12177.0
  glTexImage3D(GL_TEXTURE_3D,0,GL_ALPHA,domainSizei, domainSizej, domainSizek,0,GL_ALPHA,GL_UNSIGNED_BYTE,&data[0]);
  glDisable(GL_TEXTURE_3D);
    */


    //shaderh.loadFileVertexShader("PosicionesRayos.vert","TrazaRayos.frag");
    //setColorMap(color_map);
    //shaderh.textureLink("VolumeData",0);
    //shaderh.textureLink("ColorTransferFunction",1);
    //shaderh.textureLink("AlphaTransferFunction",2);
    //shaderh.vector4fvLink("sizeVR", size_x, size_y, size_z, 1.0);
    std::cout<<"cargando terminada de shaders"<<std::endl;
}

void VolumeRendering::draw(){
    update();
    glPushMatrix();

    glTranslatef( center_x,center_y,center_z);
    glRotatef(rot_x,1.0,0.0,0.0f);
    glRotatef(rot_y,0.0,1.0,0.0f);
    glRotatef(rot_z,0.0,0.0,1.0f);
    glTranslatef( size_x*(px-0.5),size_y*(py-0.5),0);


    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glDepthMask(true);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);

    //dibuja cubo
    /*
    glPushMatrix();
    glLineWidth(1.0);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glScalef(size_x,size_y,size_z);
    glColor4f(0.0,0.0,0.0,1.0);
    glutWireCube(1.0);
    glPopMatrix();
*/


    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    glBlendFunc (GL_ONE, GL_SRC_ALPHA);

    shaderh.useProgram();
    //glUseProgramObjectARB(programObject);//HABILITA SHADER PARA VOLUME RENDERING
    glColor4f(1.0,0.0,0.0,1.0);
    glutSolidCube(1.0);
    glUseProgramObjectARB(0);//DES-HABILITA SHADER PARA VOLUME RENDERING
    //glDisable(GL_CULL_FACE);

    //glEnable(GL_POLYGON_SMOOTH);
    //glEnable(GL_LINE_SMOOTH);

    //dibuja cubo
    /*
    glPushMatrix();
    glLineWidth(1.0);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glScalef(size_x,size_y,size_z);
    glColor4f(0.0,0.0,0.0,1.0);
    glutWireCube(1.0);
    glPopMatrix();
    */

    glDisable (GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    

    glPopMatrix();

    //std::cout<<"center"<<center_x<<","<<center_y<<","<<center_z<<"***angle "<<rot_z<<std::endl;

}

void VolumeRendering::rotx(float angle){
    rot_x=angle;
}

void VolumeRendering::roty(float angle){
    rot_y=angle;
}

void VolumeRendering::rotz(float angle){
    rot_z=angle;
}
