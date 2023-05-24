#include "ViewingPlane.hpp"
// Define this somewhere in your header file
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//GLenum ViewingPlane::texture=0;
void ViewingPlane::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("Vertices")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat0", lat0);
                ifTheAttributeIsFoundAssign(ait, "long0", long0);
                ifTheAttributeIsFoundAssign(ait, "lat1", lat1);
                ifTheAttributeIsFoundAssign(ait, "long1", long1);
                ifTheAttributeIsFoundAssign(ait, "lat2", lat2);
                ifTheAttributeIsFoundAssign(ait, "long2", long2);
                ifTheAttributeIsFoundAssign(ait, "lat3", lat3);
                ifTheAttributeIsFoundAssign(ait, "long3", long3);
            }
        }

        if( nodeName.compare("Subdivision")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "n", sub);
            }
        }

        if( nodeName.compare("ColorMap")==0 ){
            std::string IdName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssign(ait, "getId", IdName)){
                    DrawableIdentifier::data(IdName,color_map);
                }
                ifTheAttributeIsFoundAssign(ait, "view", viewColorBar);
                ifTheAttributeIsFoundAssign(ait, "precision", CBarPrecision);


                //cout<<"bCMapMin:"<<bCMapMin<<" bCMapMax:"<<bCMapMax<<endl;

            }

        }

        if( nodeName.compare("Data")==0 ){
            std::string IdName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdName);
            }
            DrawableIdentifier::data(IdName,data);
        }

        if( nodeName.compare("Remove")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssign(ait, "val", removeValue)){
                    remove=true;
                }
            }
        }

        if( nodeName.compare("Cut")==0 ){
            std::string IdName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "getId", IdName);
            }
            DrawableIdentifier::data(IdName,triang);
        }

        if( nodeName.compare("Values")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "show", showValues);
                ifTheAttributeIsFoundAssign(ait, "sizeText", sizeText);
                ifTheAttributeIsFoundAssign(ait, "widthText", widthText);
                ifTheAttributeIsFoundAssign(ait, "precision", precision);
            }
        }

        if( nodeName.compare("Texture")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "filter", filter);
            }
        }

        if( nodeName.compare("Grid")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "show", gridShow);
                ifTheAttributeIsFoundAssign(ait, "width", gridWidth);
                ifTheAttributeIsFoundAssign(ait, "r", gridColorR);
                ifTheAttributeIsFoundAssign(ait, "g", gridColorG);
                ifTheAttributeIsFoundAssign(ait, "b", gridColorB);
                ifTheAttributeIsFoundAssign(ait, "a", gridColorA);
            }
        }

        if( nodeName.compare("View3D")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "m", m3D);
                ifTheAttributeIsFoundAssign(ait, "b", b3D);
                ifTheAttributeIsFoundAssign(ait, "position", position);

            }
            mode3D=true;
            is3D=1;
        }

        if( nodeName.compare("Isovalue")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "N", isosurface_N);
                ifTheAttributeIsFoundAssign(ait, "width", isosurface_width);
            }
        }

        if( nodeName.compare("addPoint")==0 ){
            V3D p;
            string name;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "long", p.x);
                ifTheAttributeIsFoundAssign(ait, "lat", p.y);
                ifTheAttributeIsFoundAssign(ait, "value", p.z);
                ifTheAttributeIsFoundAssign(ait, "radio", pointsRadio);
                ifTheAttributeIsFoundAssign(ait, "name", name);
            }
            if(name.empty()){
                points.push_back(p);
            }
            else{
                readFileApprox(name);
            }
        }



        std::cout <<"/>"<< std::endl;
    }

    //if(!shaderh.setShaders2()){
    if( !shaderh.loadFileVertexShader("../Shaders/Texture2D.vert","../Shaders/Texture2D.frag") ){
        std::cout<<std::endl<<"ERROR en shader de interpolación!!!"<<std::endl;
    }


//    float west,east,north,south;
//    west=(long0<long1)?long0:long1;
//    west=(west<long2)?west:long2;
//    west=(west<long3)?west:long3;
//    east=(long0>long1)?long0:long1;
//    east=(east>long2)?east:long2;
//    east=(east>long3)?east:long3;
//    south=(lat0<lat1)?lat0:lat1;
//    south=(south<lat2)?south:lat2;
//    south=(south<lat3)?south:lat3;
//    north=(lat0>lat1)?lat0:lat1;
//    north=(north>lat2)?north:lat2;
//    north=(north>lat3)?north:lat3;

    cout<<"xmin="<<data->xmin<<" xmax="<<data->xmax<<" ymin="<<data->ymin<<" ymax="<<data->ymax<<endl;
    if(Shapes::xmin>data->xmin)Shapes::xmin=data->xmin;
    if(Shapes::xmax<data->xmax)Shapes::xmax=data->xmax;
    if(Shapes::ymin>data->ymin)Shapes::ymin=data->ymin;
    if(Shapes::ymax<data->ymax)Shapes::ymax=data->ymax;
//    shaderh.vector1fvLink("West", data->xmin);
//    shaderh.vector1fvLink("East", data->xmax);
//    shaderh.vector1fvLink("South", data->ymin);
//    shaderh.vector1fvLink("North", data->ymax);
    shaderh.vector1fvLink("West", 0.0);
    shaderh.vector1fvLink("East", 1.0);
    shaderh.vector1fvLink("South", 0.0);
    shaderh.vector1fvLink("North", 1.0);
    shaderh.vector1iLink("remove", remove);
    shaderh.vector1iLink("isovalue_N", isosurface_N);
    shaderh.vector1fvLink("isovalue_width", isosurface_width);
    shaderh.vector1iLink("is3D", is3D);


    if(color_map==0){
        color_map=new ColorMapClass();


    }
    color_map->min(data->min);
    color_map->max(data->max);

//    if(viewColorBar){
//        color_bar=new ColorBar();
//        color_bar->color_map=color_map;
//        color_bar->titleText=data->name;
//        color_bar->precision=CBarPrecision;
//        color_bar->textSize=1.0;
//        cout<<"color_bar->title="<<data->name<<endl;
//        Shapes::push_back_2D( &ColorBar::draw , color_bar );
//    }

    makeGrid();

    //    glGenTextures(1, &tex);
    //    glGenTextures(1, &texColorMap);
    //    glGenTextures(1, &texAlphaMap);

    setSurface();
    setColorMap();
    setTexture2D();




}



void ViewingPlane::draw(){
    //setTexture2D();
    color_map->min(data->min);
    color_map->max(data->max);

//    color_map->minimumValue=data->min;
//    color_map->maximumValue=data->max;


    //    glEnable (GL_BLEND);
    //    //glBlendFunc (GL_SRC_ALPHA, );
    //    glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //    //glShadeModel(GL_SMOOTH);
    //    glEnable(GL_DEPTH_TEST);
    //    //glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    //glFrontFace(GL_CW);

    //glEnable(GL_LINE_SMOOTH);

    //    glBindTexture(GL_TEXTURE_2D,tex);
    //    glBindTexture(GL_TEXTURE_1D,texColorMap);
    //    glBindTexture(GL_TEXTURE_1D,texAlphaMap);
    // setTexture2D();
    // setColorMap();

//    setSurface();
//    setColorMap();
//    setTexture2D();


    glColor4f(0.0,1.0,0.0,1.0);

    if(triang!=NULL){
        triang->draw();
    }
    else{
//https://www.khronos.org/opengl/wiki/VBO_-_just_examples
        //if(mode3D)

            shaderh.useProgram();

            glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);

            glEnableVertexAttribArray(0);    // We like submitting vertices on stream 0 for no special reason
            glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), BUFFER_OFFSET(0));      // The starting point of the VBO, for the vertices

            glEnableVertexAttribArray(1);    // We like submitting normals on stream 1 for no special reason
            glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), BUFFER_OFFSET(12));     // The starting point of normals, 12 bytes away

            glEnableVertexAttribArray(2);    // We like submitting texcoords on stream 2 for no special reason
            glVertexAttribPointer(texCoord0Loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), BUFFER_OFFSET(24));     // The starting point of texcoords, 24 bytes away

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

            glDrawElements(GL_QUADS, 4*pindices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));    // The starting point of the IBO

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glUseProgram(0);


//            glEnable(GL_COLOR_MATERIAL);
//            glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//            glColor3f(1.0,0.0,0.0);

//            GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };
//            GLfloat mat_specular[] = { 0.2, 0.2, 0.2, 1.0 };
//            glShadeModel (GL_SMOOTH);
//            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
//            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//            glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

//            glEnable ( GL_LIGHT0 );
//            glEnable ( GL_LIGHTING );
//            glColorMaterial(GL_FRONT, GL_DIFFUSE);
//            glEnable(GL_COLOR_MATERIAL);
//            for(int i=0;i<points.size();i++){
//                glPushMatrix();
//                glTranslatef(points[i].x,points[i].y,points[i].nz);
//                glutSolidSphere (pointsRadio,8,8);
//                glPopMatrix();
//            }
//            glDisable(GL_COLOR_MATERIAL);
//            glDisable ( GL_LIGHT0 );
//            glDisable ( GL_LIGHTING );




            if(gridShow){
                 //glDisable(GL_DEPTH_TEST);
                glColor4ub(gridColorR,gridColorG,gridColorB,gridColorA);
                glLineWidth(gridWidth);
                for(int j=0;j<data->row;j++){
                    glBegin(GL_LINE_STRIP);
                    for(int i=0;i<data->column;i++){
                        glVertex3fv(&(pvertex[j*data->column+i].p.x));

                    }
                    glEnd();
                }

                for(int i=0;i<data->column;i++){
                    glBegin(GL_LINE_STRIP);
                    for(int j=0;j<data->row;j++){
                        glVertex3fv(&(pvertex[j*data->column+i].p.x));
                    }
                    glEnd();
                }

            }

        /*
        else{
            shaderh.useProgram();
            glBegin(GL_QUADS);
//            glVertex3f( long0,lat0,0.0);
//            glVertex3f( long1,lat1,0.0);
//            glVertex3f( long2,lat2,0.0);
//            glVertex3f( long3,lat3,0.0);
            glVertex3f( data->xmin,data->ymin,0.0);
            glVertex3f( data->xmax,data->ymin,0.0);
            glVertex3f( data->xmax,data->ymax,0.0);
            glVertex3f( data->xmin,data->ymax,0.0);
            glEnd();
            glUseProgram(0);
            //glDisable(GL_DEPTH_TEST);

            for(int i=0;i<points.size();i++){
                int N=8;
                int r,g,b;
                //color_map->transferColor(points[i].nz,r,g,b);
                glColor4ub(r,g,b,255);
                //glColor4ub(255,0,0,255);
                //glColor4ub(255,gridColorG,gridColorB,gridColorA);
                glBegin(GL_POLYGON);
                for (unsigned int j = 0; j < N; j++){
                    float theta=6.28318530718*float(j)/float(N);
                    glVertex3f(points[i].x+pointsRadio*cos(theta),points[i].y+pointsRadio*sin(theta),0.0);
                }
                glEnd();

                glColor4ub(0,0,0,255);
                glBegin(GL_LINE_LOOP);
                for (unsigned int j = 0; j < N; j++){
                    float theta=6.28318530718*float(j)/float(N);
                    glVertex3f(points[i].x+pointsRadio*cos(theta),points[i].y+pointsRadio*sin(theta),0.0);
                }
                glEnd();

            }


        }*/

    }
//    glUseProgram(0);




    if(showValues)
    {
        glLineWidth(widthText);
        glColor4f(0.1,0.1,0.1,1.0);
        for(int j=0;j<data->row;j++)
            for(int i=0;i<data->column;i++){
                int index_ij=j*data->column+i;
                //if(data->val[index_ij]!=0)
                {
                    int hString=120;
                    std::stringstream text;
                    text << std::fixed<< std::setprecision(precision) << data->val[index_ij];
                    glPushMatrix();
                    //glTranslatef(long0+(long1-long0)*(i+0.5)/data->column, lat0+(lat3-lat0)*(j+0.5)/data->row,0.001f);
                    glTranslatef(data->xmin+(data->xmax-data->xmin)*(i+0.5)/data->column, data->ymin+(data->ymax-data->ymin)*(j+0.5)/data->row,0.001f);
                    glScalef(sizeText/hString,sizeText/hString,sizeText/hString);
                    int wString=0;
                    for (unsigned int i=0; i<text.str().length(); i++)
                        wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.str().c_str()[i]);

                    glTranslatef(-0.5*wString,-0.5*hString,0.0f);
                    for (unsigned int i=0; i<text.str().length(); i++)
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[i]);
                    glPopMatrix();
                }
            }
    }


//    glColor4f(0.0,0.0,0.0,1.0);
//    glLineWidth(10);
//    glBegin(GL_LINES);
//    glVertex3f( data->xmin,data->ymin,0.0);
//    glVertex3f( data->xmax,data->ymax,0.0);
//    glEnd();
//    glPushMatrix();
//    glTranslatef(data->xmin,data->ymin,0.0);
//    glScalef(0.001,0.001,0.001);
//    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'X');
//    glPopMatrix();

//    glDisable(GL_BLEND);
//    glDisable(GL_DEPTH_TEST);


}



void ViewingPlane::setTexture2D(){
    shaderh.useProgram();
    //int N=data.3D->Nx*_data3D->Ny*_data3D->Nz;
    int N=data->column*data->row;
    unsigned char *__data=new unsigned char[N];

    color_map->min(data->min);
    color_map->max(data->max);
//    //color_map->maximumValue
//    float min=color_map->min();
//    float max=color_map->max();
//    if(min==0.0 && max==0.0){
//        min=data->min;
//        max=data->max;

//    }

    if(remove){
        for(int i=0;i<N;i++){
            if( data->val[i] < color_map->min() ) __data[i]=1;
            else{
                if( data->val[i] > color_map->max() ) __data[i]=255;
                else
                    __data[i]=1+254.0*(data->val[i]-color_map->min())/(color_map->max()-color_map->min());
            }
            //if( round(data->val[i]) == removeValue ) __data[i]=0;
            if( data->val[i] == removeValue ) __data[i]=0;
        }
    }
    else{
        for(int i=0;i<N;i++){
            if( data->val[i] < color_map->min() ) __data[i]=0;
            else{
                if( data->val[i] > color_map->max() ) __data[i]=255;
                else
                    __data[i]=255.0*(data->val[i]-color_map->min())/(color_map->max()-color_map->min());
            }
        }
    }

    glGenTextures(1, &tex);
    //    glActiveTexture(GL_TEXTURE0+texture);
    //    texture++;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,tex);

    texDataLoc = shaderh.getUniformLocation("Texture2D");
    glUniform1i(texDataLoc,0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if( filter.compare("LINEAR")==0 ){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if( filter.compare("NEAREST")==0 ){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,data->column,data->row,0,GL_ALPHA,GL_UNSIGNED_BYTE,__data );
    //glDisable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D,0);
    delete[] __data;

    glGenTextures(1, &texMask);
    //    glActiveTexture(GL_TEXTURE0+texture);
    //    texture++;
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,texMask);

    texMaskDataLoc = shaderh.getUniformLocation("TextureMask2D");
    glUniform1i(texMaskDataLoc,3);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if( filter.compare("LINEAR")==0 ){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if( filter.compare("NEAREST")==0 ){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,data->column,data->row,0,GL_ALPHA,GL_UNSIGNED_BYTE,data->mask.data() );
    glUseProgram(0);
}

void ViewingPlane::setColorMap(){
    shaderh.useProgram();

    //std::cout<<"Carga de ColorTransferFunction"<<std::endl;

    glGenTextures(1, &texColorMap);
    //    glActiveTexture(GL_TEXTURE0+texture);
    //    texture++;
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D,texColorMap);

    texColorMapLoc = shaderh.getUniformLocation("ColorTransferFunction");
    glUniform1i(texColorMapLoc,1);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,color_map->lutColor.size()/3 ,0,GL_RGB,GL_UNSIGNED_BYTE,&(color_map->lutColor[0]) );
    //glDisable(GL_TEXTURE_1D);

    glGenTextures(1, &texAlphaMap);
    //    glActiveTexture(GL_TEXTURE0+texture);
    //    texture++;
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_1D,texAlphaMap);

    texAlphaMapLoc = shaderh.getUniformLocation("AlphaTransferFunction");
    glUniform1i(texAlphaMapLoc,2);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D,0,GL_ALPHA,color_map->lutAlpha.size(),0,GL_ALPHA,GL_UNSIGNED_BYTE,&(color_map->lutAlpha[0]) );
    //glDisable(GL_TEXTURE_1D);

    //glBindTexture(GL_TEXTURE_1D,0);
    glUseProgram(0);

}

void ViewingPlane::setSurface(){
    shaderh.useProgram();


    //https://www.khronos.org/opengl/wiki/VBO_-_just_examples
    //http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/
    //https://stackoverflow.com/questions/8704801/glvertexattribpointer-clarification
    //https://www.khronos.org/registry/OpenGL-Refpages/gl4/
    //https://www.khronos.org/opengl/wiki/Generic_Vertex_Attribute_-_examples

    glGenBuffers(1, &VertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D)*pvertex.size(), &pvertex[0].p.x, GL_STATIC_DRAW);

    vertexLoc = shaderh.getAttribLocation("InVertex");
    normalLoc = shaderh.getAttribLocation("InNormal");
    texCoord0Loc = shaderh.getAttribLocation("InTexCoord0");

    glGenBuffers(1, &IndexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Quad)*pindices.size(), &pindices[0].a, GL_STATIC_DRAW);



    glUseProgram(0);
}

void ViewingPlane::makeGrid(){
    float m=0,b=0;

    float Dx=fabs(data->xtmax-data->xtmin);
    float Dy=fabs(data->ytmax-data->ytmin);
    float Dl=(Dx>Dy)?Dx:Dy;

    cout<<"valmin="<<data->min<<" valmax="<<data->max<<endl;

    float nmin=std::numeric_limits<float>::max();
    float nmax=-std::numeric_limits<float>::max();
    for(int i=0;i<data->val.size();i++){
        if(nmin>data->val[i])nmin=data->val[i];
        if(nmax<data->val[i])nmax=data->val[i];
    }
    cout<<"nvalmin="<<nmin<<" valmax="<<nmax<<endl;

    if( position.compare("ABSOLUTE")==0 ){
        m=m3D;
        b=b3D;
    }
    if( position.compare("RELATIVE")==0 ){
        m=Dl*m3D/(data->max-data->min);
        b=Dl*(b3D-m3D*data->min/(data->max-data->min));
    }
    cout<<"m="<<m<<" b="<<b<<endl;

//    if( filter.compare("LINEAR")==0 ){
//        pvertex.resize(data->column*data->row);
//        pindices.resize( (data->column-1)*(data->row-1) );

//    //    for(int i=0;i<points.size();i++){
//    //        points[i].nz=m*points[i].z+b;
//    //    }

//        for(int j=0;j<data->row;j++)
//            for(int i=0;i<data->column;i++){
//                int index_ij=j*data->column+i;
//    //            float x=long0+(long1-long0)*(i+0.5)/data->column;
//    //            float y=lat0+(lat3-lat0)*(j+0.5)/data->row;
//    //            float x=data->xmin+(data->xmax-data->xmin)*(i+0.5)/data->column;
//    //            float y=data->ymin+(data->ymax-data->ymin)*(j+0.5)/data->row;
//    //            pvertex[index_ij].p.x=x;
//    //            pvertex[index_ij].p.y=y;
//    //            pvertex[index_ij].p.z=m*data->val[index_ij]+b;

//                //pvertex[index_ij].p.x=data->xtmin+data->xyt[2*index_ij]*(data->xtmax-data->xtmin);
//                //pvertex[index_ij].p.y=data->ytmin+data->xyt[2*index_ij+1]*(data->ytmax-data->ytmin);
//                //pvertex[index_ij].p.z=m*data->val[index_ij]+b;
//                pvertex[index_ij].p.x=data->xy_linear_t[2*index_ij];
//                pvertex[index_ij].p.y=data->xy_linear_t[2*index_ij+1];
//                //pvertex[index_ij].p.z=m*data->val[index_ij]+b;
//                pvertex[index_ij].p.z=0.0;

//                pvertex[index_ij].s0=(float)i/(float)(data->column);
//                pvertex[index_ij].t0=(float)j/(float)(data->row);
//            }

//        {//i=0 j=0
//            V3D pCenter=pvertex[0].p;
//            V3D pRight=pvertex[1].p;
//            V3D pUp=pvertex[data->column].p;
//            V3D n=pCenter.normal(pRight,pUp);
//            pvertex[0].n=n.normalized();
//        }

//        {//i=data->column-1 j=0
//            V3D pCenter=pvertex[data->column-1].p;
//            V3D pLeft=pvertex[data->column-2].p;
//            V3D pUp=pvertex[2*data->column-1].p;
//            V3D n=pCenter.normal(pUp,pLeft);
//            pvertex[data->column-1].n=n.normalized();
//        }

//        {//i=0 j=data->row-1
//            V3D pCenter=pvertex[(data->row-1)*data->column].p;
//            V3D pRight=pvertex[(data->row-1)*data->column+1].p;
//            V3D pDown=pvertex[(data->row-2)*data->column].p;
//            V3D n=pCenter.normal(pDown,pRight);
//            pvertex[(data->row-1)*data->column].n=n.normalized();
//        }

//        {//i=data->column-1 j=data->row-1
//            V3D pCenter=pvertex[(data->row-1)*data->column+data->column-1].p;
//            V3D pLeft=pvertex[(data->row-1)*data->column+(data->column-2)].p;
//            V3D pDown=pvertex[(data->row-2)*data->column+data->column-1].p;
//            V3D n=pCenter.normal(pLeft,pDown);
//            pvertex[(data->row-1)*data->column+data->column-1].n=n.normalized();
//        }


//        for(int i=1;i<data->column-1;i++){//j=0
//            V3D pCenter=pvertex[i].p;
//            V3D pRight=pvertex[i+1].p;
//            V3D pLeft=pvertex[i-1].p;
//            V3D pUp=pvertex[data->column+i].p;
//            V3D n=pCenter.normal(pRight,pUp)+pCenter.normal(pUp,pLeft);
//            pvertex[i].n=n.normalized();
//        }

//        for(int i=1;i<data->column-1;i++){//j=data->row-1
//            V3D pCenter=pvertex[(data->row-1)*data->column+i].p;
//            V3D pRight=pvertex[(data->row-1)*data->column+(i+1)].p;
//            V3D pLeft=pvertex[(data->row-1)*data->column+(i-1)].p;
//            V3D pDown=pvertex[(data->row-2)*data->column+i].p;
//            V3D n=pCenter.normal(pLeft,pDown)+pCenter.normal(pDown,pRight);
//            pvertex[(data->row-1)*data->column+i].n=n.normalized();
//        }

//        for(int j=1;j<data->row-1;j++){//i=0
//            V3D pCenter=pvertex[j*data->column].p;
//            V3D pRight=pvertex[j*data->column+1].p;
//            V3D pUp=pvertex[(j+1)*data->column].p;
//            V3D pDown=pvertex[(j-1)*data->column].p;
//            V3D n=pCenter.normal(pRight,pUp)+pCenter.normal(pDown,pRight);
//            pvertex[j*data->column].n=n.normalized();
//        }

//        for(int j=1;j<data->row-1;j++){//i=data->column-1
//            V3D pCenter=pvertex[j*data->column+data->column-1].p;
//            V3D pLeft=pvertex[j*data->column+(data->column-2)].p;
//            V3D pUp=pvertex[(j+1)*data->column+data->column-1].p;
//            V3D pDown=pvertex[(j-1)*data->column+data->column-1].p;
//            V3D n=pCenter.normal(pUp,pLeft)+pCenter.normal(pLeft,pDown);
//            pvertex[j*data->column+data->column-1].n=n.normalized();
//        }

//        for(int j=1;j<data->row-1;j++)
//            for(int i=1;i<data->column-1;i++){
//                V3D pCenter=pvertex[j*data->column+i].p;
//                V3D pRight=pvertex[j*data->column+(i+1)].p;
//                V3D pLeft=pvertex[j*data->column+(i-1)].p;
//                V3D pUp=pvertex[(j+1)*data->column+i].p;
//                V3D pDown=pvertex[(j-1)*data->column+i].p;
//                V3D n=pCenter.normal(pRight,pUp)+pCenter.normal(pUp,pLeft)+pCenter.normal(pLeft,pDown)+pCenter.normal(pDown,pRight);
//                pvertex[j*data->column+i].n=n.normalized();
//            }

//        for(int j=0;j<data->row-1;j++)
//            for(int i=0;i<data->column-1;i++){
//                int ij=j*(data->column-1)+i;
//                unsigned int index_ij=j*data->column+i;
//                unsigned int index_i1j=j*data->column+(i+1);
//                unsigned int index_ij1=(j+1)*data->column+i;
//                unsigned int index_i1j1=(j+1)*data->column+(i+1);
//                pindices[ij].a=index_ij;
//                pindices[ij].b=index_i1j;
//                pindices[ij].c=index_i1j1;
//                pindices[ij].d=index_ij1;
//            }
//    }

   // if( filter.compare("NEAREST")==0 ){
        pvertex.resize((data->column+1)*(data->row+1));
        pindices.resize( (data->column)*(data->row) );

        for(int j=0;j<(data->row+1);j++)
            for(int i=0;i<(data->column+1);i++){
                int index_ij=j*(data->column+1)+i;
                pvertex[index_ij].p.x=data->xy_nearest_t[2*index_ij];
                pvertex[index_ij].p.y=data->xy_nearest_t[2*index_ij+1];
                //pvertex[index_ij].p.z=m*data->val[index_ij]+b;
                pvertex[index_ij].p.z=0.0;

                pvertex[index_ij].s0=(float)i/(float)(data->column);
                pvertex[index_ij].t0=(float)j/(float)(data->row);
            }
cout<<"pvertex.size="<<pvertex.size()<<" pindices.size="<<pindices.size()<<endl;
        {//i=0 j=0
            V3D pCenter=pvertex[0].p;
            V3D pRight=pvertex[1].p;
            V3D pUp=pvertex[data->column].p;
            V3D n=pCenter.normal(pRight,pUp);
            pvertex[0].n=n.normalized();
        }

        {//i=data->column-1 j=0
            V3D pCenter=pvertex[data->column-1].p;
            V3D pLeft=pvertex[data->column-2].p;
            V3D pUp=pvertex[2*data->column-1].p;
            V3D n=pCenter.normal(pUp,pLeft);
            pvertex[data->column-1].n=n.normalized();
        }

        {//i=0 j=data->row-1
            V3D pCenter=pvertex[(data->row-1)*data->column].p;
            V3D pRight=pvertex[(data->row-1)*data->column+1].p;
            V3D pDown=pvertex[(data->row-2)*data->column].p;
            V3D n=pCenter.normal(pDown,pRight);
            pvertex[(data->row-1)*data->column].n=n.normalized();
        }

        {//i=data->column-1 j=data->row-1
            V3D pCenter=pvertex[(data->row-1)*data->column+data->column-1].p;
            V3D pLeft=pvertex[(data->row-1)*data->column+(data->column-2)].p;
            V3D pDown=pvertex[(data->row-2)*data->column+data->column-1].p;
            V3D n=pCenter.normal(pLeft,pDown);
            pvertex[(data->row-1)*data->column+data->column-1].n=n.normalized();
        }


        for(int i=1;i<data->column-1;i++){//j=0
            V3D pCenter=pvertex[i].p;
            V3D pRight=pvertex[i+1].p;
            V3D pLeft=pvertex[i-1].p;
            V3D pUp=pvertex[data->column+i].p;
            V3D n=pCenter.normal(pRight,pUp)+pCenter.normal(pUp,pLeft);
            pvertex[i].n=n.normalized();
        }

        for(int i=1;i<data->column-1;i++){//j=data->row-1
            V3D pCenter=pvertex[(data->row-1)*data->column+i].p;
            V3D pRight=pvertex[(data->row-1)*data->column+(i+1)].p;
            V3D pLeft=pvertex[(data->row-1)*data->column+(i-1)].p;
            V3D pDown=pvertex[(data->row-2)*data->column+i].p;
            V3D n=pCenter.normal(pLeft,pDown)+pCenter.normal(pDown,pRight);
            pvertex[(data->row-1)*data->column+i].n=n.normalized();
        }

        for(int j=1;j<data->row-1;j++){//i=0
            V3D pCenter=pvertex[j*data->column].p;
            V3D pRight=pvertex[j*data->column+1].p;
            V3D pUp=pvertex[(j+1)*data->column].p;
            V3D pDown=pvertex[(j-1)*data->column].p;
            V3D n=pCenter.normal(pRight,pUp)+pCenter.normal(pDown,pRight);
            pvertex[j*data->column].n=n.normalized();
        }

        for(int j=1;j<data->row-1;j++){//i=data->column-1
            V3D pCenter=pvertex[j*data->column+data->column-1].p;
            V3D pLeft=pvertex[j*data->column+(data->column-2)].p;
            V3D pUp=pvertex[(j+1)*data->column+data->column-1].p;
            V3D pDown=pvertex[(j-1)*data->column+data->column-1].p;
            V3D n=pCenter.normal(pUp,pLeft)+pCenter.normal(pLeft,pDown);
            pvertex[j*data->column+data->column-1].n=n.normalized();
        }

        for(int j=1;j<data->row-1;j++)
            for(int i=1;i<data->column-1;i++){
                V3D pCenter=pvertex[j*data->column+i].p;
                V3D pRight=pvertex[j*data->column+(i+1)].p;
                V3D pLeft=pvertex[j*data->column+(i-1)].p;
                V3D pUp=pvertex[(j+1)*data->column+i].p;
                V3D pDown=pvertex[(j-1)*data->column+i].p;
                V3D n=pCenter.normal(pRight,pUp)+pCenter.normal(pUp,pLeft)+pCenter.normal(pLeft,pDown)+pCenter.normal(pDown,pRight);
                pvertex[j*data->column+i].n=n.normalized();
            }

        for(int j=0;j<data->row;j++)
            for(int i=0;i<data->column;i++){
                int ij=j*(data->column)+i;
                unsigned int index_ij=j*(data->column+1)+i;
                unsigned int index_i1j=j*(data->column+1)+(i+1);
                unsigned int index_ij1=(j+1)*(data->column+1)+i;
                unsigned int index_i1j1=(j+1)*(data->column+1)+(i+1);
                pindices[ij].a=index_ij;
                pindices[ij].b=index_i1j;
                pindices[ij].c=index_i1j1;
                pindices[ij].d=index_ij1;
            }

   // }

}


int  ViewingPlane::readFileApprox(string fileName){

    std::ifstream fileLines(fileName.c_str(),std::ios::in);

    if (fileLines.is_open()){
        cout<<"Abriendo archivo de puntos:"<<fileName<<endl;
        std::string sline;
        std::stringstream ssline;
        int iLine=0;
        float _long,_lat,_value;

        while ( getline(fileLines,sline)){
            if(!sline.empty()){
                std::stringstream ssline(sline);
                if( !(ssline>>_long>>_lat>>_value) ){
                    cout<<endl<<"Error en la línea "<<iLine<<" del archivo de puntos:"<<endl;
                    cout<<fileName<<endl;
                    cout<<"Se esperaban leer 3 valores"<<endl;
                    cout<<sline<<endl;
                    return 0;
                }
                V3D p;
                p.x=_long;
                p.y=_lat;
                p.z=_value;
                points.push_back(p);
            }
            iLine++;
        }


    }
    else
        cout<<"No se pudo leer el archivo de puntos: "<<fileName<<endl;


    return 0;
}
