#include "Mesh.hpp"


void Mesh::projection(Projection *proj){
    this->proj=proj;
}

void Mesh::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );


        if( nodeName.compare("Stroke")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", strokeR);
                ifTheAttributeIsFoundAssign(ait, "g", strokeG);
                ifTheAttributeIsFoundAssign(ait, "b", strokeB);
                ifTheAttributeIsFoundAssign(ait, "a", strokeA);
                ifTheAttributeIsFoundAssign(ait, "width", strokeWidth);
            }
        }

        if( nodeName.compare("Fill")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r1", fill1R);
                ifTheAttributeIsFoundAssign(ait, "g1", fill1G);
                ifTheAttributeIsFoundAssign(ait, "b1", fill1B);
                ifTheAttributeIsFoundAssign(ait, "a1", fill1A);
                ifTheAttributeIsFoundAssign(ait, "r2", fill2R);
                ifTheAttributeIsFoundAssign(ait, "g2", fill2G);
                ifTheAttributeIsFoundAssign(ait, "b2", fill2B);
                ifTheAttributeIsFoundAssign(ait, "a2", fill2A);
            }
        }

        if( nodeName.compare("StrokePoint")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", point.strokeR);
                ifTheAttributeIsFoundAssign(ait, "g", point.strokeG);
                ifTheAttributeIsFoundAssign(ait, "b", point.strokeB);
                ifTheAttributeIsFoundAssign(ait, "a", point.strokeA);
                ifTheAttributeIsFoundAssign(ait, "width", point.strokeWidth);
            }
        }

        if( nodeName.compare("FillPoint")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", point.fillR);
                ifTheAttributeIsFoundAssign(ait, "g", point.fillG);
                ifTheAttributeIsFoundAssign(ait, "b", point.fillB);
                ifTheAttributeIsFoundAssign(ait, "a", point.fillA);
            }
        }

        if( nodeName.compare("Origin")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "long", longitude);
                ifTheAttributeIsFoundAssign(ait, "lat", latitude);
                ifTheAttributeIsFoundAssign(ait, "dx", dx);
                ifTheAttributeIsFoundAssign(ait, "dy", dy);
                ifTheAttributeIsFoundAssign(ait, "n_E", n_E);
                ifTheAttributeIsFoundAssign(ait, "n_W", n_W);
                ifTheAttributeIsFoundAssign(ait, "n_N", n_N);
                ifTheAttributeIsFoundAssign(ait, "n_S", n_S);
                ifTheAttributeIsFoundAssign(ait, "alpha", alpha);
            }
        }

        if( nodeName.compare("AddPoint")==0 ){

            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "i", point.i);
                ifTheAttributeIsFoundAssign(ait, "j", point.j);
                ifTheAttributeIsFoundAssign(ait, "radio", point.radio);
                ifTheAttributeIsFoundAssign(ait, "label", point.label);
            }
            ijPoints.push_back(point);

        }
        if( nodeName.compare("AddCoordinate")==0 ){

            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "i", point.i);
                ifTheAttributeIsFoundAssign(ait, "j", point.j);
                ifTheAttributeIsFoundAssign(ait, "radio", point.radio);
                ifTheAttributeIsFoundAssign(ait, "label", point.label);
            }
            ijPoints.push_back(point);

        }


//        if( nodeName.compare("TextAlignment")==0 ){
//            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
//                ifTheAttributeIsFoundAssign(ait, "textAlignment", textAlignment);
//            }
//        }

        if( nodeName.compare("Projection")==0 ){
            std::string IdProy;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdProy);
            }
            DrawableIdentifier::data(IdProy,proj);
        }
        std::cout <<"/>"<< std::endl;
    }

    Nx=n_E+n_W+1;
    Ny=n_S+n_N+1;
    vPoints.resize( 2*Nx*Ny );

    for(int j=0;j<n_N;j++){
        double ly=dy*1000*(1+j);
        for(int i=0;i<n_W;i++){
            int ij=2*((n_S+1+j)*Nx+i);
            double lx=dx*1000*(n_W-i);

            double x1,y1,azi1;
            double x2,y2,azi2;
            proj->geodDirect(latitude, longitude, 90-alpha, -lx, y1, x1, azi1);
            proj->geodDirect(y1,x1, -alpha, ly, y2, x2, azi2);
            vPoints[ij]=x2;
            vPoints[ij+1]=y2;
        }
    }

    for(int j=0;j<n_N;j++){
        double ly=dy*1000*(1+j);
        int ij=2*((n_S+1+j)*Nx+n_W);
        double x2,y2,azi2;
        proj->geodDirect(latitude, longitude, -alpha, ly, y2, x2, azi2);
        vPoints[ij]=x2;
        vPoints[ij+1]=y2;
    }

    for(int j=0;j<n_N;j++){
        double ly=dy*1000*(1+j);
        for(int i=0;i<n_E;i++){
            int ij=2*((n_S+1+j)*Nx+n_W+1+i);
            double lx=dx*1000*(i+1);

            double x1,y1,azi1;
            double x2,y2,azi2;
            proj->geodDirect(latitude, longitude, 90-alpha, lx, y1, x1, azi1);
            proj->geodDirect(y1,x1, -alpha, ly, y2, x2, azi2);
            vPoints[ij]=x2;
            vPoints[ij+1]=y2;
        }
    }

    for(int i=0;i<n_W;i++){
        int ij=2*(n_S*Nx+i);
        double lx=dx*1000*(n_W-i);
        double x1,y1,azi1;
        proj->geodDirect(latitude, longitude, 90-alpha, -lx, y1, x1, azi1);
        vPoints[ij]=x1;
        vPoints[ij+1]=y1;
    }

    {
        int ij=2*(n_S*Nx+n_W);
        vPoints[ij]=longitude;
        vPoints[ij+1]=latitude;
    }

    for(int i=0;i<n_E;i++){
        int ij=2*(n_S*Nx+n_W+1+i);
        double lx=dx*1000*(i+1);
        double x1,y1,azi1;
        proj->geodDirect(latitude, longitude, 90-alpha, lx, y1, x1, azi1);
        vPoints[ij]=x1;
        vPoints[ij+1]=y1;
    }

    for(int j=0;j<n_S;j++){
        double ly=dy*1000*(n_S-j);
        for(int i=0;i<n_W;i++){
            int ij=2*(j*Nx+i);
            double lx=dx*1000*(n_W-i);

            double x1,y1,azi1;
            double x2,y2,azi2;
            proj->geodDirect(latitude, longitude, 90-alpha, -lx, y1, x1, azi1);
            proj->geodDirect(y1,x1, -alpha, -ly, y2, x2, azi2);
            vPoints[ij]=x2;
            vPoints[ij+1]=y2;
        }
    }

    for(int j=0;j<n_S;j++){
        double ly=dy*1000*(n_S-j);
        int ij=2*(j*Nx+n_W);

        double x2,y2,azi2;
        proj->geodDirect(latitude, longitude, -alpha, -ly, y2, x2, azi2);
        vPoints[ij]=x2;
        vPoints[ij+1]=y2;
    }

    for(int j=0;j<n_S;j++){
        double ly=dy*1000*(n_S-j);
        for(int i=0;i<n_E;i++){
            int ij=2*(j*Nx+n_W+1+i);
            double lx=dx*1000*(i+1);

            double x1,y1,azi1;
            double x2,y2,azi2;
            proj->geodDirect(latitude, longitude, 90-alpha, lx, y1, x1, azi1);
            proj->geodDirect(y1,x1, -alpha, -ly, y2, x2, azi2);
            vPoints[ij]=x2;
            vPoints[ij+1]=y2;
        }
    }
    {
        int i=0;
        int j=0;
        int ij=2*(j*Nx+i);
        cout<<"MESH"<<endl;
        cout<<"<Point long=\""<<vPoints[ij]<<"\" lat=\""<< vPoints[ij+1]<<"\" name=\"("<<i<<","<<j<<")\"/>"<<endl;
        i=Nx-1;
        j=0;
        ij=2*(j*Nx+i);
        cout<<"<Point long=\""<<vPoints[ij]<<"\" lat=\""<< vPoints[ij+1]<<"\" name=\"("<<i<<","<<j<<")\"/>"<<endl;
        i=0;
        j=Ny-1;
        ij=2*(j*Nx+i);
        cout<<"<Point long=\""<<vPoints[ij]<<"\" lat=\""<< vPoints[ij+1]<<"\" name=\"("<<i<<","<<j<<")\"/>"<<endl;
        i=Nx-1;
        j=Ny-1;
        ij=2*(j*Nx+i);
        cout<<"<Point long=\""<<vPoints[ij]<<"\" lat=\""<< vPoints[ij+1]<<"\" name=\"("<<i<<","<<j<<")\"/>"<<endl;
    }
    proj->transform(vPoints);

    double dl=(dx<dy)?dx:dy;
    for(auto &&p: ijPoints){
        //proj->geodDirect(latitude, longitude, 90-alpha, lx, y1, x1, azi1);
        if(p.radio<-1.0){
            p.radio=dl/3.0;
        }
    }
    cout<<"dl="<<dl<<endl;

    txt=new Text3D();
    txt->setWidth(txtWidth);
    txt->setSize(textSize);


//    cout<<"ColorStroke("<<strokeR<<","<<strokeG<<","<<strokeB<<","<<strokeA<<")"<<endl;
//    cout<<"ColorFill("<<fillR<<","<<fillG<<","<<fillB<<","<<fillA<<")"<<endl;

}

void Mesh::draw(){

    double distmax=fabs(Shapes::xtmax-Shapes::xtmin);
//    double hString=119.0;


    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glLineWidth(strokeWidth);

    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);

    for(int j=0;j<Ny-1;j++){
        for(int i=0;i<Nx-1;i++){

            if( ((i+j)%2)==0 )
                glColor4ub(fill1R,fill1G,fill1B,fill1A);
            else
                glColor4ub(fill2R,fill2G,fill2B,fill2A);

            int ij=2*(j*Nx+i);
            int i1j=2*(j*Nx+i+1);
            int ij1=2*((j+1)*Nx+i);
            int i1j1=2*((j+1)*Nx+i+1);
            glBegin(GL_QUADS);
            glVertex3f( vPoints[ij], vPoints[ij+1],0.0f);
            glVertex3f( vPoints[i1j], vPoints[i1j+1],0.0f);
            glVertex3f( vPoints[i1j1], vPoints[i1j1+1],0.0f);
            glVertex3f( vPoints[ij1], vPoints[ij1+1],0.0f);
            glEnd();
        }
    }

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glColor4ub(strokeR,strokeG,strokeB,strokeA);
    for(int j=0;j<Ny-1;j++){
        for(int i=0;i<Nx-1;i++){
            int ij=2*(j*Nx+i);
            int i1j=2*(j*Nx+i+1);
            int ij1=2*((j+1)*Nx+i);
            int i1j1=2*((j+1)*Nx+i+1);
            glBegin(GL_LINE_LOOP);
            glVertex3f( vPoints[ij], vPoints[ij+1],0.0f);
            glVertex3f( vPoints[i1j], vPoints[i1j+1],0.0f);
            glVertex3f( vPoints[i1j1], vPoints[i1j1+1],0.0f);
            glVertex3f( vPoints[ij1], vPoints[ij1+1],0.0f);
            glEnd();
        }
    }


    for(auto &&p: ijPoints){
        int ij=2*(p.j*Nx+p.i);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POLYGON_SMOOTH);
        glColor4ub(p.fillR,p.fillG,p.fillB,p.fillA);
        glBegin(GL_POLYGON);
        for(auto &&c: circle){
            glVertex3f( vPoints[ij]+p.radio*1e-3*c.x, vPoints[ij+1]+p.radio*1e-3*c.y,0.0f);
        }
        glEnd();

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glColor4ub(p.strokeR,p.strokeG,p.strokeB,p.strokeA);
        glBegin(GL_LINE_LOOP);
        for(auto &&c: circle){
            glVertex3f( vPoints[ij]+p.radio*1e-3*c.x, vPoints[ij+1]+p.radio*1e-3*c.y,0.0f);
        }
        glEnd();

        if(!p.label.empty()){

            //txt->print(vPoints[ij], vPoints[ij+1],p.label);

            std::stringstream text;
            //text << std::fixed<< std::setprecision(precisionLabel) << size*(0.5+stepTxt[i]) <<"km";
            text <<p.label<<"("<<p.i<<","<<p.j<<")";
            txt->print(vPoints[ij], vPoints[ij+1], text.str());
        }
    }

//    for(int i=0;i<vPoints.size()-1;i++){
//        if( (i%2)==0 )
//            glColor4ub(fill1R,fill1G,fill1B,fill1A);
//        else
//            glColor4ub(fill2R,fill2G,fill2B,fill2A);
//        glBegin(GL_QUADS);
//        glVertex3f( GeodesicLine[i].n1x, GeodesicLine[i].n1y,0.0f);
//        glVertex3f( GeodesicLine[i].n2x, GeodesicLine[i].n2y,0.0f);
//        glVertex3f( GeodesicLine[i+1].n2x, GeodesicLine[i+1].n2y,0.0f);
//        glVertex3f( GeodesicLine[i+1].n1x, GeodesicLine[i+1].n1y,0.0f);
//        glEnd();
//    }





//    glBegin(GL_LINE_STRIP);
//    for (auto&& punto : GeodesicLine){
//        glVertex3f( punto.x, punto.y,0.0f);
//    }
//    glEnd();
}
