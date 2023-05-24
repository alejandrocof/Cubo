#include "Distance.hpp"

//Distance::Distance(){
//    proj=0;
//    aspectRatio=0.05;;
//    strokeWidth=1.0;
//    strokeR=0;strokeG=0;strokeB=0;strokeA=128;

//    fillR=0;
//    fillG=255;
//    fillB=0;
//    fillA=128;

//    txtWidth=1.0;
//    txtSize=1.0;
//    txtR=0;txtG=0;txtB=0;txtA=255;

//    nPoints=10;
//    dl=0.0;
//    distance=0.0;
//}

void Distance::projection(Projection *proj){
    this->proj=proj;
}

void Distance::loadXML(pugi::xml_node_iterator node_it){
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

        if( nodeName.compare("Points")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "long1", longitude1);
                ifTheAttributeIsFoundAssign(ait, "lat1", latitude1);
                ifTheAttributeIsFoundAssign(ait, "long2", longitude2);
                ifTheAttributeIsFoundAssign(ait, "lat2", latitude2);
                ifTheAttributeIsFoundAssign(ait, "name", name);
                ifTheAttributeIsFoundAssign(ait, "dl", dl);
            }
            sGeodesicLine l;
            l.strokeWidth=strokeWidth;
            l.strokeR=strokeR;
            l.strokeG=strokeG;
            l.strokeB=strokeB;
            l.strokeA=strokeA;
            l.fill1R=fill1R;
            l.fill1G=fill1G;
            l.fill1B=fill1B;
            l.fill1A=fill1A;
            l.fill2R=fill2R;
            l.fill2G=fill2G;
            l.fill2B=fill2B;
            l.fill2A=fill2A;
            l.longitude1=longitude1;
            l.longitude2=longitude2;
            l.latitude1=latitude1;
            l.latitude2=latitude2;
            l.dl=dl;
            l.name=name;
            GeodesicLines.push_back(l);
        }
/*
        if( nodeName.compare("StepSize")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "dl", dl);
            }
        }

        if( nodeName.compare("AddPoint")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "long", longitude1);
                ifTheAttributeIsFoundAssign(ait, "lat", latitude1);
            }
            //vStrokePoint.emplace()

        }
*/
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
    double azi1,azi2,aziN1,aziN2;
    double dx,dy;

    for (auto &&geol : GeodesicLines){
        proj->geodInverse(geol.latitude1, geol.longitude1, geol.latitude1, geol.longitude2, dx, azi1, azi2);
        proj->geodInverse(geol.latitude1, geol.longitude1, geol.latitude2, geol.longitude1, dy, azi1, azi2);
        proj->geodInverse(geol.latitude1, geol.longitude1, geol.latitude2, geol.longitude2, distance, azi1, azi2);
        cout<<geol.name<<" ";
        cout<<geol.latitude1<<" "<<geol.longitude1<<" ";
        cout<<geol.latitude2<<" "<<geol.longitude2<<" ";
        cout<<distance/1000.0<<" "<<dx/1000.0<<" "<<dy/1000.0<<endl;
        dn=1.5e4*fabs(Shapes::xtmax-Shapes::xtmin);
        if(dl>0.0){
            double dxy;
            for (dxy = 0.0; dxy < distance; dxy+=(dl*1000.0)) {
                GeodesicPoint p;
                proj->geodDirect(geol.latitude1, geol.longitude1, azi1, dxy, p.y, p.x, azi2);
                proj->geodDirect(p.y, p.x, azi2+90, dn, p.n1y, p.n1x, aziN1);
                proj->geodDirect(p.y, p.x, azi2-90, dn, p.n2y, p.n2x, aziN2);
                //cout<<"punto("<<p.x<<","<<p.y<<") ";
                //cout<<"n1("<<p.n1x<<","<<p.n1y<<") ";
                //cout<<"n2("<<p.n2x<<","<<p.n2y<<")"<<endl;
                proj->transform(p.x,p.y);
                proj->transform(p.n1x,p.n1y);
                proj->transform(p.n2x,p.n2y);
                geol.GeodesicLine.push_back(p);
            }
            if(dxy != distance){
                dxy=distance;
                GeodesicPoint p;
                proj->geodDirect(geol.latitude1, geol.longitude1, azi1, dxy, p.y, p.x, azi2);
                proj->geodDirect(p.y, p.x, azi2+90, dn, p.n1y, p.n1x, aziN1);
                proj->geodDirect(p.y, p.x, azi2-90, dn, p.n2y, p.n2x, aziN2);
                //cout<<"punto("<<p.x<<","<<p.y<<") ";
                //cout<<"n1("<<p.n1x<<","<<p.n1y<<") ";
                //cout<<"n2("<<p.n2x<<","<<p.n2y<<")"<<endl;
                proj->transform(p.x,p.y);
                proj->transform(p.n1x,p.n1y);
                proj->transform(p.n2x,p.n2y);
                geol.GeodesicLine.push_back(p);
            }
        }
        else{
            for (int i = 0; i < nPoints; i++) {
                GeodesicPoint p;
                proj->geodDirect(geol.latitude1, geol.longitude1, azi1, distance*i/(nPoints-1), p.y, p.x, azi2);
                proj->geodDirect(p.y, p.x, azi2+90, dn, p.n1y, p.n1x, aziN1);
                proj->geodDirect(p.y, p.x, azi2-90, dn, p.n2y, p.n2x, aziN2);
                //cout<<"punto("<<p.x<<","<<p.y<<") ";
                //cout<<"n1("<<p.n1x<<","<<p.n1y<<") ";
                //cout<<"n2("<<p.n2x<<","<<p.n2y<<")"<<endl;
                proj->transform(p.x,p.y);
                proj->transform(p.n1x,p.n1y);
                proj->transform(p.n2x,p.n2y);
                geol.GeodesicLine.push_back(p);
            }
        }
    }



//    cout<<"ColorStroke("<<strokeR<<","<<strokeG<<","<<strokeB<<","<<strokeA<<")"<<endl;
//    cout<<"ColorFill("<<fillR<<","<<fillG<<","<<fillB<<","<<fillA<<")"<<endl;

}

void Distance::draw(){


    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    for (auto &&geol : GeodesicLines){
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POLYGON_SMOOTH);

        glLineWidth(geol.strokeWidth);
        for(int i=0;i<geol.GeodesicLine.size()-1;i++){
            if( (i%2)==0 )
                glColor4ub(geol.fill1R,geol.fill1G,geol.fill1B,geol.fill1A);
            else
                glColor4ub(geol.fill2R,geol.fill2G,geol.fill2B,geol.fill2A);
            glBegin(GL_QUADS);
            glVertex3f( geol.GeodesicLine[i].n1x, geol.GeodesicLine[i].n1y,0.0f);
            glVertex3f( geol.GeodesicLine[i].n2x, geol.GeodesicLine[i].n2y,0.0f);
            glVertex3f( geol.GeodesicLine[i+1].n2x, geol.GeodesicLine[i+1].n2y,0.0f);
            glVertex3f( geol.GeodesicLine[i+1].n1x, geol.GeodesicLine[i+1].n1y,0.0f);
            glEnd();
        }


        glColor4ub(geol.strokeR,geol.strokeG,geol.strokeB,geol.strokeA);

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);

        for(int i=0;i<geol.GeodesicLine.size()-1;i++){
            glBegin(GL_LINE_LOOP);
            glVertex3f( geol.GeodesicLine[i].n1x, geol.GeodesicLine[i].n1y,0.0f);
            glVertex3f( geol.GeodesicLine[i].n2x, geol.GeodesicLine[i].n2y,0.0f);
            glVertex3f( geol.GeodesicLine[i+1].n2x, geol.GeodesicLine[i+1].n2y,0.0f);
            glVertex3f( geol.GeodesicLine[i+1].n1x, geol.GeodesicLine[i+1].n1y,0.0f);
            glEnd();
        }
    }




//    glBegin(GL_LINE_STRIP);
//    for (auto&& punto : GeodesicLine){
//        glVertex3f( punto.x, punto.y,0.0f);
//    }
//    glEnd();
}
