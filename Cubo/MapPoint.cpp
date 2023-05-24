#include "MapPoint.hpp"


MapPoint::MapPoint(){

    f_r=64;
    f_g=64;
    f_b=64;
    f_a=128;

    s_r=32;
    s_g=32;
    s_b=32;
    s_a=255;

    t_r=0;
    t_g=0;
    t_b=0;
    t_a=255;

    w=1.0;
    radio=1.0;
    N=20;
    std::vector<float> circle;

    for(int i=0;i<=N;i++){
        shape.push_back( cos( 6.28318530718*float(i)/float(N) ) );
        shape.push_back( sin( 6.28318530718*float(i)/float(N) ) );
    }
    

}

void MapPoint::setFillColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
    f_r=_r;
    f_g=_g;
    f_b=_b;
    f_a=_a;
}

void MapPoint::setStrokeColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
    s_r=_r;
    s_g=_g;
    s_b=_b;
    s_a=_a;
}

void MapPoint::setStrokeWidth(float _w){
    w=_w;
}

void MapPoint::setRadio(double _radio){
    radio=_radio;
}

void MapPoint::setPoint(double latitude, double longitude, std::string name, int id){
    point p;

    p.f_r=f_r;
    p.f_g=f_g;
    p.f_b=f_b;
    p.f_a=f_a;

    p.s_r=s_r;
    p.s_g=s_g;
    p.s_b=s_b;
    p.s_a=s_a;

    p.t_r=t_r;
    p.t_g=t_g;
    p.t_b=t_b;
    p.t_a=t_a;
    p.textSize=1.0;
    p.w=w;
    p.radio=radio;
    p.latitude=latitude;
    p.longitude=longitude;
    p.z=0.0;
    p.name=name;
    p.id=id;
    vPoints.push_back(p);
}

void MapPoint::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("FillColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssign(ait, "r", f_r))bFill=true;
                if(ifTheAttributeIsFoundAssign(ait, "g", f_g))bFill=true;
                if(ifTheAttributeIsFoundAssign(ait, "b", f_b))bFill=true;
                if(ifTheAttributeIsFoundAssign(ait, "a", f_a))bFill=true;
            }
        }

        if( nodeName.compare("ColorMap")==0 ){
            std::string IdName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssign(ait, "getId", IdName)){
                    DrawableIdentifier::data(IdName,color_map);
                }
//                if(ifTheAttributeIsFoundAssign(ait, "min", CMapMin))bCMapMin=true;
//                if(ifTheAttributeIsFoundAssign(ait, "max", CMapMax))bCMapMax=true;

                //cout<<"bCMapMin:"<<bCMapMin<<" bCMapMax:"<<bCMapMax<<endl;

            }

        }


        if( nodeName.compare("StrokeColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssign(ait, "r", s_r))bStroke=true;
                if(ifTheAttributeIsFoundAssign(ait, "g", s_g))bStroke=true;
                if(ifTheAttributeIsFoundAssign(ait, "b", s_b))bStroke=true;
                if(ifTheAttributeIsFoundAssign(ait, "a", s_a))bStroke=true;
            }
        }

        if( nodeName.compare("TextColor")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", t_r);
                ifTheAttributeIsFoundAssign(ait, "g", t_g);
                ifTheAttributeIsFoundAssign(ait, "b", t_b);
                ifTheAttributeIsFoundAssign(ait, "a", t_a);
            }
        }

        if( nodeName.compare("TextWidth")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textWidth", textWidth);
            }
        }

        if( nodeName.compare("TextSize")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textSize", textSize);
                ifTheAttributeIsFoundAssign(ait, "textAngle", textAngle);
            }
        }

        if( nodeName.compare("TextAlignment")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "textAlignment", textAlignment);
            }
        }

        if( nodeName.compare("StrokeWidth")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "w", w);
            }
        }

        if( nodeName.compare("Circle")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "radio", radio);
            }

            shape.clear();
            for(int i=0;i<=N;i++){
                float alpha=6.28318530718*float(i)/float(N);
                shape.push_back( cos(alpha ) );
                shape.push_back( sin(alpha ) );
            }

        }

        if( nodeName.compare("Square")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "radio", radio);
            }

            shape.clear();
            for(int i=0;i<=4;i++){
                float alpha=6.283185307*(0.125+(float)i/4.0);
                shape.push_back( cos(alpha) );
                shape.push_back( sin(alpha) );
            }

        }

        /*

        if( nodeName.compare("Rectangle")==0 ){
            float width, high;
            float latitude, longitude;
            float angle;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", latitude);
                ifTheAttributeIsFoundAssign(ait, "long", longitude);
                ifTheAttributeIsFoundAssign(ait, "width", width);
                ifTheAttributeIsFoundAssign(ait, "high", high);
                ifTheAttributeIsFoundAssign(ait, "angle", angle);
            }

            shape.clear();

            shape.push_back( cos(angle) );
            shape.push_back( cos(angle) );

        }
*/


        if( nodeName.compare("Triangle")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "radio", radio);
            }

            shape.clear();
            for(int i=0;i<=3;i++){
                float alpha=6.283185307*(0.25+(float)i/3.0);
                shape.push_back( cos(alpha) );
                shape.push_back( sin(alpha) );
            }

        }

        if( nodeName.compare("Projection")==0 ){
            std::string IdProy;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdProy);
            }
            DrawableIdentifier::data(IdProy,proj);
        }


        if( nodeName.compare("Point")==0 ){
            cout<<"Color("<<f_r<<","<<f_g<<","<<f_b<<","<<f_a<<")"<<endl;
            point p;
            p.z=0.0;
            p.f_r=f_r; p.f_g=f_g; p.f_b=f_b; p.f_a=f_a;
            p.s_r=s_r; p.s_g=s_g; p.s_b=s_b; p.s_a=s_a;
            p.t_r=t_r; p.t_g=t_g; p.t_b=t_b; p.t_a=t_a;
            p.w=w; p.radio=radio;
            p.textWidth=textWidth;
            p.textSize=textSize;
            p.textAngle=textAngle;
            p.tx=0.0;
            p.ty=0.0;
            p.tz=0.0;
            p.lx=-0.5;
            p.ly=-0.5;


            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", p.latitude);
                ifTheAttributeIsFoundAssign(ait, "long", p.longitude);
                ifTheAttributeIsFoundAssign(ait, "z", p.z);
                if(ifTheAttributeIsFoundAssignString(ait, "name", p.name))showz=true;
                ifTheAttributeIsFoundAssign(ait, "id", p.id);
                ifTheAttributeIsFoundAssign(ait, "file", fileName);
                ifTheAttributeIsFoundAssign(ait, "showz", showz);
                ifTheAttributeIsFoundAssignString(ait, "title", titleColorBar);

            }

            p.tx=p.longitude;
            p.ty=p.latitude;
            p.tz=0;

            if(textAlignment.compare("up")==0){
                p.tx=p.longitude;
                p.ty=p.latitude+radio;
                p.tz=0;
                p.lx=-0.5;
                p.ly=0.0;
            }

            if(textAlignment.compare("down")==0){
                p.tx=p.longitude;
                p.ty=p.latitude-radio;
                p.tz=0;
                p.lx=-0.5;
                p.ly=-1.0;
            }

            if(textAlignment.compare("right")==0){
                p.tx=p.longitude+radio;
                p.ty=p.latitude;
                p.tz=0;
                p.lx= 0.0;
                p.ly=-0.5;
            }

            if(textAlignment.compare("left")==0){
                p.tx=p.longitude-radio;
                p.ty=p.latitude;
                p.tz=0;
                p.lx=-1.0;
                p.ly=-0.5;
            }

            vector<double> pxy(2);
//            pxy[0]=p.tx*DEG_TO_RAD;
//            pxy[1]=p.ty*DEG_TO_RAD;
            pxy[0]=p.tx;
            pxy[1]=p.ty;
            proj->transform(pxy);
            p.tx=pxy[0];
            p.ty=pxy[1];


            if(fileName.empty()){
                replaceAll(p.name,"á","a`");
                replaceAll(p.name,"é","e`");
                replaceAll(p.name,"í","i`");
                replaceAll(p.name,"ó","o`");
                replaceAll(p.name,"ú","u`");
                replaceAll(p.name,"ñ","n~");
                cout<<"Agregando punto:"<<p.name<<endl;
                vector<double> pxy(2);
//                pxy[0]=p.longitude*DEG_TO_RAD;
//                pxy[1]=p.latitude*DEG_TO_RAD;
                pxy[0]=p.longitude;
                pxy[1]=p.latitude;
                proj->transform(pxy);
                p.longitude=pxy[0];
                p.latitude=pxy[1];
                vPoints.push_back(p);
            }
            else{
                std::ifstream fileLines(fileName.c_str(),std::ios::in);

                //vector< VectorXf > x;

                if (fileLines.is_open()){
                    cout<<"Abriendo archivo de datos:"<<fileName<<endl;
                    std::string sline;
                    std::stringstream ssline;
                    int iLine=0;


                    while ( getline(fileLines,sline)){

                        if(!sline.empty()){
                            std::stringstream ssline(sline);
                            vector<double> _xtemp(3);
                            for(int iVar=0;iVar<3;iVar++){
                                if( !(ssline>>_xtemp[iVar]) ){
                                    cout<<endl<<"Error en la línea "<<iLine<<" del archivo:"<<endl;
                                    cout<<fileName<<endl;
                                    cout<<"Se esperaban leer 3 datos"<<endl;
                                    cout<<sline<<endl;
                                    //return 0;
                                }
                            }

                            p.longitude=_xtemp[0];
                            p.latitude=_xtemp[1];
                            p.z=_xtemp[2];
                            p.radio=radio;


                            vector<double> pxy(2);
                            //cout<<"("<<p.longitude<<","<<p.longitude<<","<<p.z<<")-";
//                            pxy[0]=p.longitude*DEG_TO_RAD;
//                            pxy[1]=p.latitude*DEG_TO_RAD;
                            pxy[0]=p.longitude;
                            pxy[1]=p.latitude;
                            proj->transform(pxy);
                            p.longitude=pxy[0];
                            p.latitude=pxy[1];
                            //p.z=_xtemp[2];
                            //cout<<"("<<p.longitude<<","<<p.longitude<<","<<p.z<<")"<<endl;

                            //std::cout<<"p lat="<<p.latitude<<" long=="<<p.longitude<<" z="<<p.z <<std::endl;
                            vPoints.push_back(p);

                        }
                        iLine++;
                    }
                }
                else
                    cout<<"No se pudo leer el archivo: "<<fileName<<endl;
//                replaceAll(p.name,"á","a`");
//                replaceAll(p.name,"é","e`");
//                replaceAll(p.name,"í","i`");
//                replaceAll(p.name,"ó","o`");
//                replaceAll(p.name,"ú","u`");
//                replaceAll(p.name,"ñ","n~");


//                p.f_r=f_r; p.f_g=f_g; p.f_b=f_b; p.f_a=f_a;
//                p.s_r=s_r; p.s_g=s_g; p.s_b=s_b; p.s_a=s_a;
//                p.t_r=t_r; p.t_g=t_g; p.t_b=t_b; p.t_a=t_a;
//                p.w=w; p.radio=radio;
//                p.textWidth=textWidth;
//                p.textSize=textSize;
//                p.tx=0.0;
//                p.ty=0.0;
//                p.tz=0.0;
//                p.lx=-0.5;
//                p.ly=-0.5;

//                if(textAlignment.compare("up")==0){
//                    p.ty=radio;
//                    p.lx=-0.5;
//                    p.ly=0.0;
//                }

//                if(textAlignment.compare("down")==0){
//                    p.ty=-radio;
//                    p.lx=-0.5;
//                    p.ly=-1.0;
//                }

//                if(textAlignment.compare("right")==0){
//                    p.tx=radio;
//                    p.lx= 0.0;
//                    p.ly=-0.5;
//                }

//                if(textAlignment.compare("left")==0){
//                    p.tx=-radio;
//                    p.lx=-1.0;
//                    p.ly=-0.5;
//                }

//                vPoints.push_back(p);
            }


        }





        std::cout <<"/>"<< std::endl;
    }



    if(!fileName.empty()){
        float min=std::numeric_limits<float>::max();
        float max=-std::numeric_limits<float>::max();
        vector<double> pxy(2);
        for (std::vector<point>::iterator it = vPoints.begin() ; it != vPoints.end(); ++it){
            if(max < it->z)max = it->z;
            if(min > it->z)min = it->z;
        }

        if(!bFill){
            cout<<"bFill="<<bFill<<endl;
            if(color_map==0){
                color_map=new ColorMapClass();

    //            color_bar=new ColorBar();
    //            color_bar->color_map=color_map;
    //            color_bar->titleText=titleColorBar;
    //            color_bar->precision=2.0;
    //            //cout<<"color_bar->title="<<data->name<<endl;
    //            Shapes::push_back_2D( &ColorBar::draw , color_bar );
            }

            color_map->min(min);
            color_map->max(max);

            color_bar=new ColorBar();
            color_bar->color_map=color_map;
            color_bar->titleText=titleColorBar;
            color_bar->precision=2.0;
            //cout<<"color_bar->title="<<data->name<<endl;
            //Shapes::push_back_2D( &ColorBar::draw , color_bar );
        }


    }





//    if(!fileName.empty()){
//    if(color_map==0){
//        color_map=new ColorMapClass();

//        if(bCMapMin) color_map->minimumValue=CMapMin;
//            else color_map->minimumValue=data->min;

//        if(bCMapMax) color_map->maximumValue=CMapMax;
//            else color_map->maximumValue=data->max;
//    }

//    if(viewColorBar){
//        color_bar=new ColorBar();
//        color_bar->color_map=color_map;
//        color_bar->titleText=data->name;
//        color_bar->precision=CBarPrecision;
//        cout<<"color_bar->title="<<data->name<<endl;
//        Shapes::push_back_2D( &ColorBar::draw , color_bar );
//    }
//    }


}


/*
FillColor r="255" g="0" b="255"/>
  <StrokeColor r="255" g="0" b="255"/>
  <StrokeWidth w="0.001"/>
  <Circle diameter="0.02"/>
  <Point lat="20.6710" long="103.3620" name="ARC" id="2"/>
*/
void MapPoint::draw(){

    /*
  std::cout<<std::endl<<std::endl<<std::endl;
  std::cout<<"**************"<<std::endl;
  std::cout<<"***MAPPOINT***"<<std::endl;
  std::cout<<"**************"<<std::endl<<std::endl<<std::endl;
*/
    glDisable(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    for (std::vector<point>::iterator it = vPoints.begin() ; it != vPoints.end(); ++it){
        //std::cout<<"lat="<<it->latitude<<" long=="<<it->longitude<<" z="<<it->z <<std::endl;

        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glPushMatrix();


        //glEnable(GL_POLYGON_SMOOTH);
        //glTranslatef(it->longitude, it->latitude,it->z);
        glTranslatef(it->longitude, it->latitude,0);

        //glLineWidth(1.0);
        /*
      glBegin(GL_TRIANGLE_FAN);
        glColor4ub(it->f_r, it->f_g, it->f_b, it->f_a);
        glVertex3f(0.0,0.0,0.0);
        glColor4ub(it->s_r, it->s_g, it->s_b, it->s_a);
        //glColor3f(1.0, 1.0, 1.0);
        //glColor3ub(255, 255, 255);
        for (int i = 0; i < shape.size()/2; i++){
          glVertex3f(it->radio*shape[2*i],it->radio*shape[2*i+1],0.0);
        }
      glEnd();
      */



        double scale=0.01*DisplaySettings::dL();

        if(bFill || color_map!=0){
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
            if(bFill){
                glColor4ub(it->f_r, it->f_g, it->f_b, it->f_a);
            }
            else{
                int r,g,b;
                color_map->transferColor(it->z,r,g,b);
                glColor4ub(r,g,b,255);
            }
            glLineWidth(0);
            glBegin(GL_POLYGON);
            for (unsigned int i = 0; i < shape.size()/2; i++){
                //glVertex3f(it->radio*scale*shape[2*i],it->radio*scale*shape[2*i+1],0.0);
                glVertex3f(it->radio*shape[2*i],it->radio*shape[2*i+1],0.0);
            }
            glEnd();
        }

        if(bStroke){
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);
            glColor4ub(it->s_r, it->s_g, it->s_b, it->s_a);
            glLineWidth(it->w);
            glLineWidth(0);
            glBegin(GL_LINE_STRIP);
            for (unsigned int i = 0; i < shape.size()/2; i++){
                //glVertex3f(it->radio*scale*shape[2*i],it->radio*scale*shape[2*i+1],0.0);
                glVertex3f(it->radio*shape[2*i],it->radio*shape[2*i+1],0.0);
            }
            glEnd();
        }


//        glBegin(GL_TRIANGLE_STRIP);
//        for (unsigned int i = 0; i < shape.size()/2; i++){
////            glVertex3f((it->radio*scale+0.0005*it->w)*shape[2*i],(it->radio*scale+0.0005*it->w)*shape[2*i+1],0.01*it->radio*scale);
////            glVertex3f((it->radio*scale-0.0005*it->w)*shape[2*i],(it->radio*scale-0.0005*it->w)*shape[2*i+1],0.01*it->radio*scale);
//            glVertex3f((it->radio+0.0005*it->w)*shape[2*i],(it->radio+0.0005*it->w)*shape[2*i+1],0.0);
//            glVertex3f((it->radio-0.0005*it->w)*shape[2*i],(it->radio-0.0005*it->w)*shape[2*i+1],0.0);
//        }
//        glEnd();
        //glDisable(GL_LINE_SMOOTH);
        //glDisable(GL_POLYGON_SMOOTH);

        //glEnable(GL_LINE_SMOOTH);
        glPopMatrix();

        /*
        if(showz){
            double scaleLabel=1.0;
            int wString=0;
            double hString=119.0;
            //double sizeLabel=0.02*it->textSize*DisplaySettings::dL()/hString;
            double sizeLabel=0.02*it->textSize*scaleLabel*DisplaySettings::dL()/hString;
            glLineWidth(it->textWidth);
            for (unsigned int j=0; j<it->name.length(); j++){
                if(it->name.c_str()[j]!='`')
                    wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, it->name.c_str()[j]);
            }

            glColor4ub(it->t_r, it->t_g, it->t_b, it->t_a);
            //glTranslatef(it->tx,it->ty,it->tz);
            //glScalef(it->textSize/hString,it->textSize/hString,it->textSize/hString);

            glPushMatrix();
            glTranslatef(it->tx,it->ty,it->tz);
            glScaled(sizeLabel,sizeLabel,sizeLabel);
            glTranslated(it->lx*wString,it->ly*hString,0.0f);
            for (unsigned int j=0; j<it->name.length(); j++){
                if(it->name.c_str()[j]=='`'){
                    glPushMatrix();
                    glTranslatef(0.0,0.2*hString,0.0);
                    glScalef(-0.3,0.8,1.0);
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, it->name.c_str()[j]);
                    glPopMatrix();
                }
                else{
                    if(it->name.c_str()[j]=='~'){
                        glPushMatrix();
                        glTranslatef(-0.5*hString,0.55*hString,0.0);
                        glScalef(0.5,0.5,1.0);
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, it->name.c_str()[j]);
                        glPopMatrix();
                    }
                    else{
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, it->name.c_str()[j]);
                    }
                }

            }
            glPopMatrix();

        }*/
        //glDisable(GL_LINE_SMOOTH);


    }



    for (std::vector<point>::iterator it = vPoints.begin() ; it != vPoints.end(); ++it){
        //std::cout<<"lat="<<it->latitude<<" long=="<<it->longitude<<" z="<<it->z <<std::endl;

        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);

        if(showz){
            double scaleLabel=1.0;
            int wString=0;
            double hString=119.0;
            //double sizeLabel=0.02*it->textSize*DisplaySettings::dL()/hString;
            double sizeLabel=0.02*it->textSize*scaleLabel*DisplaySettings::dL()/hString;
            glLineWidth(it->textWidth);
            for (unsigned int j=0; j<it->name.length(); j++){
                if(it->name.c_str()[j]!='`')
                    wString+=glutStrokeWidth(GLUT_STROKE_ROMAN, it->name.c_str()[j]);
            }

            glColor4ub(it->t_r, it->t_g, it->t_b, it->t_a);
            //glTranslatef(it->tx,it->ty,it->tz);
            //glScalef(it->textSize/hString,it->textSize/hString,it->textSize/hString);

            glPushMatrix();
            glTranslatef(it->tx,it->ty,it->tz);
            glScaled(sizeLabel,sizeLabel,sizeLabel);
            glRotated(textAngle,0,0,1);
            glTranslated(it->lx*wString,0.5*it->ly*hString,0.0f);

            for (unsigned int j=0; j<it->name.length(); j++){
                if(it->name.c_str()[j]=='`'){
                    glPushMatrix();
                    glTranslatef(0.0,0.2*hString,0.0);
                    glScalef(-0.3,0.8,1.0);
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, it->name.c_str()[j]);
                    glPopMatrix();
                }
                else{
                    if(it->name.c_str()[j]=='~'){
                        glPushMatrix();
                        glTranslatef(-0.5*hString,0.55*hString,0.0);
                        glScalef(0.5,0.5,1.0);
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, it->name.c_str()[j]);
                        glPopMatrix();
                    }
                    else{
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, it->name.c_str()[j]);
                    }
                }

            }
            glPopMatrix();

        }
        //glDisable(GL_LINE_SMOOTH);


    }

    glDisable (GL_BLEND);
    //lDisable(GL_DEPTH_TEST);
}


void MapPoint::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
