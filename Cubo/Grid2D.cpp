#include "Grid2D.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Grid2D::Grid2D(){
    r=0; g=0; b=0; a=255;
    width=1.0;

    rTics=0; gTics=0; bTics=0; aTics=255;
    widthTics=1.0;

    rLabel=0; gLabel=0; bLabel=0; aLabel=255;
    widthLabel=2.0;
    scaleLabel=1.0;//0.05
    precisionLabel=2;

    rAxisTitle=0; gAxisTitle=0; bAxisTitle=0; aAxisTitle=255;
    widthAxisTitle=2.0;
    scaleAxisTitle=1.0;
    xAxisTitle="";
    yAxisTitle="";

    xmin=0.0; xmax=1.0;
    ymin=0.0; ymax=1.0;

    mxTics=10; myTics=10;
    absolute=false;
}



void Grid2D::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("StyleLine")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", r);
                ifTheAttributeIsFoundAssign(ait, "g", g);
                ifTheAttributeIsFoundAssign(ait, "b", b);
                ifTheAttributeIsFoundAssign(ait, "a", a);
                ifTheAttributeIsFoundAssign(ait, "width", width);
            }
        }

        if( nodeName.compare("StyleTics")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", rTics);
                ifTheAttributeIsFoundAssign(ait, "g", gTics);
                ifTheAttributeIsFoundAssign(ait, "b", bTics);
                ifTheAttributeIsFoundAssign(ait, "a", aTics);
                ifTheAttributeIsFoundAssign(ait, "width", widthTics);
            }
        }

        if( nodeName.compare("StyleLabel")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", rLabel);
                ifTheAttributeIsFoundAssign(ait, "g", gLabel);
                ifTheAttributeIsFoundAssign(ait, "b", bLabel);
                ifTheAttributeIsFoundAssign(ait, "a", aLabel);
                ifTheAttributeIsFoundAssign(ait, "width", widthLabel);
                ifTheAttributeIsFoundAssign(ait, "size", scaleLabel);
                ifTheAttributeIsFoundAssign(ait, "precision", precisionLabel);
            }
        }

        if( nodeName.compare("Titles")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", rAxisTitle);
                ifTheAttributeIsFoundAssign(ait, "g", gAxisTitle);
                ifTheAttributeIsFoundAssign(ait, "b", bAxisTitle);
                ifTheAttributeIsFoundAssign(ait, "a", aAxisTitle);
                ifTheAttributeIsFoundAssign(ait, "width", widthAxisTitle);
                ifTheAttributeIsFoundAssign(ait, "size", scaleAxisTitle);
                ifTheAttributeIsFoundAssignString(ait, "xAxis", xAxisTitle);
                ifTheAttributeIsFoundAssignString(ait, "yAxis", yAxisTitle);
                replaceAll(xAxisTitle,"á","a`");
                replaceAll(xAxisTitle,"é","e`");
                replaceAll(xAxisTitle,"í","i`");
                replaceAll(xAxisTitle,"ó","o`");
                replaceAll(xAxisTitle,"ú","u`");
                replaceAll(xAxisTitle,"ñ","n~");
                replaceAll(yAxisTitle,"á","a`");
                replaceAll(yAxisTitle,"é","e`");
                replaceAll(yAxisTitle,"í","i`");
                replaceAll(yAxisTitle,"ó","o`");
                replaceAll(yAxisTitle,"ú","u`");
                replaceAll(yAxisTitle,"ñ","n~");
            }
        }

        if( nodeName.compare("Settings")==0 ){
            std::string IdName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){

                ifTheAttributeIsFoundAssign(ait, "xmin", xmin);
                ifTheAttributeIsFoundAssign(ait, "xmax", xmax);
                ifTheAttributeIsFoundAssign(ait, "ymin", ymin);
                ifTheAttributeIsFoundAssign(ait, "ymax", ymax);
                ifTheAttributeIsFoundAssign(ait, "mxTics", mxTics);
                ifTheAttributeIsFoundAssign(ait, "myTics", myTics);
                ifTheAttributeIsFoundAssign(ait, "absolute", absolute);
                ifTheAttributeIsFoundAssignString(ait, "getDataId", IdName);
            }
            if(!IdName.empty()){
                DataRaw2D *data;
                LoadShapeFileCSV *shapefile;
                if(DrawableIdentifier::data(IdName,data)){
                    xmin=data->xmin;
                    xmax=data->xmax;
                    ymin=data->ymin;
                    ymax=data->ymax;
                }
                if(DrawableIdentifier::data(IdName,shapefile)){
                    xmin=shapefile->xmin;
                    xmax=shapefile->xmax;
                    ymin=shapefile->ymin;
                    ymax=shapefile->ymax;
                }

            }

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

    if( !shaderh.loadFileVertexShader("../Shaders/Grid.vert","../Shaders/Grid.frag") ){
        std::cout<<std::endl<<"ERROR en shader de grid!!!"<<std::endl;
    }

    shaderh.vector4fvLink("gridColor", rTics/255.0, gTics/255.0, bTics/255.0, aTics/255.0);

//    float dxdymax=fabs(xmax-xmin)/mxTics;
//    if(dxdymax<fabs(ymax-ymin)/myTics)dxdymax=fabs(ymax-ymin)/myTics;
//    sizeLabel=scaleLabel*dxdymax/(18.0*DisplaySettings::distance());//0.05
//    sizeAxisTitle=1.5*scaleAxisTitle*dxdymax/(18.0*DisplaySettings::distance());//0.01
    //sizeLabel=1.0;
    make();


   // cout<<"sizeLabel "<<sizeLabel<<" DisplaySettings::distance() "<<DisplaySettings::distance()<<endl;
    setSurface();
}


void Grid2D::draw(){

//    float dxdymax=fabs(xmax-xmin)/mxTics;
//    if(dxdymax<fabs(ymax-ymin)/myTics)dxdymax=fabs(ymax-ymin)/myTics;
//    sizeLabel=scaleLabel*dxdymax/(18.0*DisplaySettings::distance());//0.05
//    sizeAxisTitle=1.5*scaleAxisTitle*dxdymax/(18.0*DisplaySettings::distance());//0.01

    glDisable(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    //glDepthMask(false);

    //glLineWidth(lw);
    //glDrawArrays(GL_LINE_STRIP, offset, count);

    shaderh.useProgram();
//glUseProgram(0);
    glLineWidth(widthTics);
    glColor4ub(rTics,gTics,bTics,aTics);

    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);

    glEnableVertexAttribArray(0);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), BUFFER_OFFSET(0));      // The starting point of the VBO, for the vertices

    glEnableVertexAttribArray(1);    // We like submitting normals on stream 1 for no special reason
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), BUFFER_OFFSET(12));     // The starting point of normals, 12 bytes away

    glEnableVertexAttribArray(2);    // We like submitting texcoords on stream 2 for no special reason
    glVertexAttribPointer(texCoord0Loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), BUFFER_OFFSET(24));     // The starting point of texcoords, 24 bytes away

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

    for(int i=0;i<myTics;i++)
        glDrawElements(GL_LINE_STRIP, mxTics, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint)*mxTics*i));    // The starting point of the IBO

    int hoffset=sizeof(GLuint)*mxTics*myTics;
    for(int i=0;i<mxTics;i++)
        glDrawElements(GL_LINE_STRIP, myTics, GL_UNSIGNED_INT, BUFFER_OFFSET(hoffset+sizeof(GLuint)*myTics*i));    // The starting point of the IBO

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glUseProgram(0);

    glLineWidth(widthLabel);
    glColor4ub(rLabel,gLabel,bLabel,aLabel);
    int wMaxString=0;

    //cout<<"ymin="<<ymin<<" ymax="<<ymax<<endl;
    //cout<<"dl="<<DisplaySettings::dL()<<endl;
    double hString=119.0;
    //scaleAxisTitle;scaleLabel
    //double sizeLabel=0.02*scaleLabel*DisplaySettings::dL()/hString;
    //double sizeAxisTitle=0.02*scaleAxisTitle*DisplaySettings::dL()/hString;
    double sizeLabel=0.02*scaleLabel*DisplaySettings::dL()/hString;
    double sizeAxisTitle=0.02*scaleAxisTitle*DisplaySettings::dL()/hString;
    for(int i=0; i<myTics; i++ ){
        std::stringstream text;
        float val=ymin+(ymax-ymin)*i/(myTics-1);
        if(absolute)val=abs(val);
        text << std::fixed<< std::setprecision(precisionLabel) << val <<" ";
        int wString=0;
        for (unsigned int j=0; j<text.str().length(); j++)
            wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
        if(wMaxString<wString)wMaxString=wString;

        glPushMatrix();
        //glTranslatef(xmin, val,0.0f);
        glTranslated(vxytics[2*i], vxytics[2*i+1],0.0f);
        glScaled(sizeLabel,sizeLabel,sizeLabel);
        glTranslated(-wString,-0.5*hString,0.0f);
        for (unsigned int j=0; j<text.str().length(); j++){
            glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
            //cout<<text.str().c_str()[j];
        }
        //cout<<endl;
        glPopMatrix();
    }
    glLineWidth(widthAxisTitle);
    glColor4ub(rAxisTitle,gAxisTitle,bAxisTitle,aAxisTitle);
    {
        int wString=0;
        for (unsigned int j=0; j<yAxisTitle.length(); j++)
            wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, yAxisTitle.c_str()[j]);
        glPushMatrix();
        double x=vxytics[2*(myTics/2)];
        double y=vxytics[2*(myTics/2)+1];
        glTranslatef(x, y,0.0f);
        glScaled(sizeAxisTitle,sizeAxisTitle,sizeAxisTitle);
//        glScalef(0.0003,0.0003,0.0003);
//        glScalef(sizeAxisTitle/hString,sizeAxisTitle/hString,sizeAxisTitle/hString);
        glTranslatef(-wMaxString*sizeLabel/sizeAxisTitle,0.0,0.0f);
        glRotatef(90.0, 0.0, 0.0, 1.0); // rotate on z-axis
        glTranslatef(-0.5*wString,0.5*hString,0.0f);
        for (unsigned int j=0; j<yAxisTitle.length(); j++){
            if( yAxisTitle.c_str()[j]=='*' ){
                glPushMatrix();
                glTranslatef(-0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, '*'),0.7*hString,0.0);
                glScalef(0.3,0.3,1.0);
                glutStrokeCharacter(GLUT_STROKE_ROMAN, 'o');
                glPopMatrix();
            }
            else{
                glutStrokeCharacter(GLUT_STROKE_ROMAN, yAxisTitle.c_str()[j]);
            }
        }
        glPopMatrix();
    }

    glLineWidth(widthLabel);
    glColor4ub(rLabel,gLabel,bLabel,aLabel);
    wMaxString=0;
    for(int i=0; i<mxTics; i++ ){
        std::stringstream text;
        float val=xmin+(xmax-xmin)*i/(mxTics-1);
        if(absolute)val=abs(val);
        text << std::fixed<< std::setprecision(precisionLabel) << val <<" ";
        int wString=0;
        for (unsigned int j=0; j<text.str().length(); j++)
            wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
        if(wMaxString<wString)wMaxString=wString;

        glPushMatrix();
        glTranslatef(hxytics[2*i], hxytics[2*i+1],0.0f);
        glScaled(sizeLabel,sizeLabel,sizeLabel);
//        glScalef(0.0003,0.0003,0.0003);
//        glScalef(sizeLabel/hString,sizeLabel/hString,sizeLabel/hString);
        glRotatef(90.0, 0.0, 0.0, 1.0); // rotate on z-axis
        glTranslatef(-wString,-0.5*hString,0.0f);
        for (unsigned int j=0; j<text.str().length(); j++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[j]);
        glPopMatrix();
    }
    glLineWidth(widthAxisTitle);
    glColor4ub(rAxisTitle,gAxisTitle,bAxisTitle,aAxisTitle);
    {
        int wString=0;
        for (unsigned int j=0; j<xAxisTitle.length(); j++)
            wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, xAxisTitle.c_str()[j]);
        glPushMatrix();
        double x=hxytics[2*(mxTics/2)];
        double y=hxytics[2*(mxTics/2)+1];
        glTranslatef(x, y,0.0f);
        glScaled(sizeAxisTitle,sizeAxisTitle,sizeAxisTitle);
//        glScalef(0.0003,0.0003,0.0003);
//        glScalef(sizeAxisTitle/hString,sizeAxisTitle/hString,sizeAxisTitle/hString);
        glTranslatef(0.0,-wMaxString*sizeLabel/sizeAxisTitle,0.0f);
        //glRotatef(90.0, 0.0, 0.0, 1.0); // rotate on z-axis
        glTranslatef(-0.5*wString,-hString,0.0f);
        for (unsigned int j=0; j<xAxisTitle.length(); j++){
            if( xAxisTitle.c_str()[j]=='*' ){
                glPushMatrix();
                glTranslatef(-0.5*glutStrokeWidth(GLUT_STROKE_ROMAN, '*'),0.7*hString,0.0);
                glScalef(0.3,0.3,1.0);
                glutStrokeCharacter(GLUT_STROKE_ROMAN, 'o');
                glPopMatrix();
            }
            else{
                glutStrokeCharacter(GLUT_STROKE_ROMAN, xAxisTitle.c_str()[j]);
            }
        }
        glPopMatrix();
    }

}

void Grid2D::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void Grid2D::make(){

    if(proj!=0){
        hxytics.resize(2*mxTics);
        vxytics.resize(2*myTics);
        std::vector<double> xy( 2*mxTics*myTics );
        pvertex.resize( mxTics*myTics );
        pindices.resize( 2*mxTics*myTics );
        //pindices.resize( mxTics+myTics );

        for(int i=0;i<mxTics;i++){
            hxytics[2*i]=(xmin+(xmax-xmin)*i/(mxTics-1));
            hxytics[2*i+1]=ymin;
//            hxytics[2*i]*=DEG_TO_RAD;
//            hxytics[2*i+1]*=DEG_TO_RAD;
        }
        proj->transform(hxytics);
        for(int i=0;i<mxTics;i++){
            //hxytics[2*i]=(hxytics[2*i]-Shapes::xtmin)/(Shapes::xtmax-Shapes::xtmin);
            //hxytics[2*i+1]=(hxytics[2*i+1]-Shapes::ytmin)/(Shapes::ytmax-Shapes::ytmin);

            if(Shapes::xtmin>hxytics[2*i])Shapes::xtmin=hxytics[2*i];
            if(Shapes::xtmax<hxytics[2*i])Shapes::xtmax=hxytics[2*i];
            if(Shapes::ytmin>hxytics[2*i+1])Shapes::ytmin=hxytics[2*i+1];
            if(Shapes::ytmax<hxytics[2*i+1])Shapes::ytmax=hxytics[2*i+1];
            //cout<<"H Tics["<<i<<"]=("<<hxytics[2*i]<<","<<hxytics[2*i+1]<<")"<<endl;
        }

        for(int j=0;j<myTics;j++){
//            vxytics[2*j]=xmin*DEG_TO_RAD;
//            vxytics[2*j+1]=(ymin+(ymax-ymin)*j/(myTics-1))*DEG_TO_RAD;
            vxytics[2*j]=xmin;
            vxytics[2*j+1]=(ymin+(ymax-ymin)*j/(myTics-1));
        }
        proj->transform(vxytics);
        for(int j=0;j<myTics;j++){
            //vxytics[2*j]=(vxytics[2*j]-Shapes::xtmin)/(Shapes::xtmax-Shapes::xtmin);
            //vxytics[2*j+1]=(vxytics[2*j+1]-Shapes::ytmin)/(Shapes::ytmax-Shapes::ytmin);

            if(Shapes::xtmin>vxytics[2*j])Shapes::xtmin=vxytics[2*j];
            if(Shapes::xtmax<vxytics[2*j])Shapes::xtmax=vxytics[2*j];
            if(Shapes::ytmin>vxytics[2*j+1])Shapes::ytmin=vxytics[2*j+1];
            if(Shapes::ytmax<vxytics[2*j+1])Shapes::ytmax=vxytics[2*j+1];
            //cout<<"V Tics["<<j<<"]=("<<vxytics[2*j]<<","<<vxytics[2*j+1]<<")"<<endl;
        }

        for(int j=0;j<myTics;j++)
            for(int i=0;i<mxTics;i++){
                int k=j*mxTics+i;
                pindices[k]=k;
            }
        for(int i=0;i<mxTics;i++)
            for(int j=0;j<myTics;j++){
                int k=j*mxTics+i;
                pindices[mxTics*myTics+i*myTics+j]=k;
            }

        for(int j=0;j<myTics;j++)
            for(int i=0;i<mxTics;i++){
                int k=j*mxTics+i;
                pindices[k]=k;
                xy[2*k]=(xmin+(xmax-xmin)*i/(mxTics-1));
                xy[2*k+1]=(ymin+(ymax-ymin)*j/(myTics-1));

                //cout<<pindices[k]<<" xy["<<k<<"]=("<<xy[2*k]<<","<<xy[2*k+1]<<")   ";

//                xy[2*k]*=DEG_TO_RAD;
//                xy[2*k+1]*=DEG_TO_RAD;
                //cout<<"Rxy["<<k<<"]=("<<xy[2*k]<<","<<xy[2*k+1]<<")"<<endl;
            }
        proj->transform(xy);

        for(int j=0;j<myTics;j++){
            for(int i=0;i<mxTics;i++){
                int k=j*mxTics+i;
//                pvertex[k].p.x=(xy[2*k]-Shapes::xtmin)/(Shapes::xtmax-Shapes::xtmin);
//                pvertex[k].p.y=(xy[2*k+1]-Shapes::ytmin)/(Shapes::ytmax-Shapes::ytmin);
//                pvertex[k].p.z=0.0;
                pvertex[k].p.x=xy[2*k];
                pvertex[k].p.y=xy[2*k+1];
                pvertex[k].p.z=0.0;

                if(Shapes::xtmin>pvertex[k].p.x)Shapes::xtmin=pvertex[k].p.x;
                if(Shapes::xtmax<pvertex[k].p.x)Shapes::xtmax=pvertex[k].p.x;
                if(Shapes::ytmin>pvertex[k].p.y)Shapes::ytmin=pvertex[k].p.y;
                if(Shapes::ytmax<pvertex[k].p.y)Shapes::ytmax=pvertex[k].p.y;

                //cout<<"vertex["<<k<<"]=("<<pvertex[k].p.x<<","<<pvertex[k].p.y<<","<<pvertex[k].p.z<<")"<<endl;
            }
        }

    }
}

void Grid2D::setSurface(){
    cout<<"setSurface()"<<endl;
    shaderh.useProgram();
    glGenBuffers(1, &VertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D)*pvertex.size(), &pvertex[0], GL_STATIC_DRAW);

    vertexLoc = shaderh.getAttribLocation("InVertex");
    normalLoc = shaderh.getAttribLocation("InNormal");
    texCoord0Loc = shaderh.getAttribLocation("InTexCoord0");

    glGenBuffers(1, &IndexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*pindices.size(), &pindices[0], GL_STATIC_DRAW);
    glUseProgram(0);

    cout<<"pindices.size()="<<pindices.size()<<endl;
    cout<<"fin setSurface()"<<endl;
}
