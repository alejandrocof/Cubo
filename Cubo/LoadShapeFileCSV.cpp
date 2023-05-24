#include "LoadShapeFileCSV.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#define APIENTRY
#endif

template <class T>
T nextC(T i, T beg,  T end){
    i++;
    if(i == end){i = beg;}
    return i;
}

template <class T>
T prevC(T i, T beg,  T end){
    if(i == beg){i = end; i--;}
    else{i--;}
    return i;
}

LoadShapeFileCSV::LoadShapeFileCSV(){
    strokeR=255;
    strokeG=0;
    strokeB=0;
    strokeA=128;
    strokeWidth=1.0;
    fillR=fillG=fillB=fillA=0;
    labelSize=1.0;
}

void LoadShapeFileCSV::setColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
    strokeR=_r;
    strokeG=_g;
    strokeB=_b;
    strokeA=_a;
}

void LoadShapeFileCSV::setWidth(float _w){
    strokeWidth=_w;
}


string WKT2string(WKT_TYPE key){
    string s;
    switch(key){
    case POINT:s="POINT";
        break;
    case MULTIPOINT:s="MULTIPOINT";
        break;
    case LINESTRING:s="LINESTRING";
        break;
    case MULTILINESTRING:s="MULTILINESTRING";
        break;
    case POLYGON:s="POLYGON";
        break;
    case MULTIPOLYGON:s="MULTIPOLYGON";
        break;
    case GEOMCOLLECTION:s="GEOMCOLLECTION";
        break;
    };
    return s;
}

WKT_TYPE string2WKT(string s){
    WKT_TYPE type;
    if(s=="POINT")type=POINT;
    if(s=="MULTIPOINT")type=MULTIPOINT;
    if(s=="LINESTRING")type=LINESTRING;
    if(s=="MULTILINESTRING")type=MULTILINESTRING;
    if(s=="POLYGON")type=POLYGON;
    if(s=="MULTIPOLYGON")type=MULTIPOLYGON;
    if(s=="GEOMCOLLECTION")type=GEOMCOLLECTION;
    return type;
}

void LoadShapeFileCSV::extraer(string line,PointsWKT &pwkt){
    size_t p1=line.find_first_of("\"");
    size_t p2=line.find_first_of("\"",p1+1);
    stringstream sWKT(line.substr(p1+1,p2-p1-1));
    stringstream lfields( line.substr( p2+1 ) );

    //cout<<endl;
    pwkt.x=pwkt.y=pwkt.z=0.0;
    float n=0.0;
    //float xmin,ymin,xmax,ymax;


    int count=0;
    int Nobjeto=0;
    size_t pos1,pos2;
    pos1 = pos2 = -1;
    PARENTESIS parentesis;//{ABIERTO,CERRADO};

    string type;
    sWKT>>type;
    //cout<<"type:"<<type<<" ";
    pwkt.type=string2WKT(type);
    //cout<<"type:"<<pwkt.type<<" "<<endl;
    std::vector<std::vector<point>> objetos;
    while( sWKT.str().find_first_of("()",pos1+1)!=std::string::npos ){
        pos2=sWKT.str().find_first_of("()",pos1+1);
        if (sWKT.str()[pos2] == '('){
            count++;
            parentesis=ABIERTO;
        }else if (sWKT.str()[pos2] == ')'){
            count--;
            parentesis=CERRADO;
        }
        //cout<<"count:"<<count<<" Parentesis:"<<parentesis<<endl;
        std::vector<point> puntos;
        if(parentesis==CERRADO && count>=0){
            //cout<<"Objeto:"<<Nobjeto<<" Nivel:"<<count<<endl;
            std::stringstream sspuntos( sWKT.str().substr(pos1+1,pos2-pos1-1) );
            std::string spunto;
            while ( getline(sspuntos,spunto,',') ){
                std::stringstream sspunto( spunto );
                point punto;
                sspunto>>punto.longitude>>punto.latitude;

                if(xmin>punto.longitude)xmin=punto.longitude;
                if(xmax<punto.longitude)xmax=punto.longitude;
                if(ymin>punto.latitude)ymin=punto.latitude;
                if(ymax<punto.latitude)ymax=punto.latitude;


                if(proj!=0){
                    //cout<<"("<<punto.longitude<<","<<punto.latitude<<")";
//                    punto.longitude*=DEG_TO_RAD;
//                    punto.latitude*=DEG_TO_RAD;
                    //cout<<"-("<<punto.longitude<<","<<punto.latitude<<")";
                    proj->transform(punto.longitude,punto.latitude);
                    //cout<<"-("<<punto.longitude<<","<<punto.latitude<<")";
//                    punto.longitude=(punto.longitude-Shapes::xtmin)/(Shapes::xtmax-Shapes::xtmin);
//                    punto.latitude=(punto.latitude-Shapes::ytmin)/(Shapes::ytmax-Shapes::ytmin);

                    if(xtmin>punto.longitude)xtmin=punto.longitude;
                    if(xtmax<punto.longitude)xtmax=punto.longitude;
                    if(ytmin>punto.latitude)ytmin=punto.latitude;
                    if(ytmax<punto.latitude)ytmax=punto.latitude;
                    //cout<<"-("<<punto.longitude<<","<<punto.latitude<<")";
                    //cout<<"-Shapesx("<<Shapes::xtmin<<","<<Shapes::xtmax<<")";
                    //cout<<"-Shapesy("<<Shapes::ytmin<<","<<Shapes::ytmax<<")"<<endl;
                }
                //cout<<"("<<punto.longitude<<","<<punto.latitude<<") ";
                puntos.push_back(punto);
                pwkt.x+=punto.longitude;
                pwkt.y+=punto.latitude;
                n++;
//                if(xmax<punto.longitude)xmax=punto.longitude;
//                if(xmin>punto.longitude)xmin=punto.longitude;
//                if(ymax<punto.latitude)ymax=punto.latitude;
//                if(ymin>punto.latitude)ymin=punto.latitude;
            }
            //cout<<endl;
            if(puntos.size()>0){
                objetos.push_back(puntos);
                //cout<<"agregando objeto"<<endl;
            }

            if(pwkt.type!=MULTIPOLYGON && count==0){
                pwkt.p.push_back(objetos);
                objetos.clear();
                Nobjeto++;
                //cout<<"agregando Geometria"<<endl;
            }
            if(pwkt.type==MULTIPOLYGON && count==1){
                pwkt.p.push_back(objetos);
                objetos.clear();
                Nobjeto++;
                //cout<<"agregando Geometria"<<endl;
            }
        }


        pos1=pos2;
    }

    int i=0;
    string field;
    //static int iii=0;
    while ( getline(lfields,field,',') ){

        cleanString(field);
        //pwkt.col.push_back(field);

//        if(classification.size()==0){
//            classification.push_back(field);
//        }
        if(i==4 && !field.empty() ){

            auto resultFind = std::find(classification.begin(), classification.end(), field);
            if ( resultFind == classification.end() ){
                classification.emplace_back(field);
                //classification.emplace_back(field,iii);
                resultFind=prev(classification.end());
                //iii++;
            }
            pwkt.r=resultFind->r;
            pwkt.g=resultFind->g;
            pwkt.b=resultFind->b;
            pwkt.a=resultFind->a;



        }





        if(labelColumn!=0 && i==labelColumn){
            pwkt.label=field;
            cout<<i<<"->"<<field<<" "<<pwkt.x/n<<" "<<pwkt.y/n<<endl;
            text3d->addTextXY(pwkt.x/n,pwkt.y/n,field,alignmentCenter);
        }
        i++;
    }
//    pwkt.r=0;
//    pwkt.g=0;
//    pwkt.b=255;
//    pwkt.a=128;


//    for(int j=0;j<pwkt.col.size();j++)
//        cout<<pwkt.col[j]<<",";
//    cout<<endl;


//    if(count==0){

//        if(labelColumn!=0){
//            string field;
//            int i=0;
//            while ( getline(lfields,field,',') ){
//                if(i==labelColumn){
//                    pwkt.label=field;
//                    cout<<i<<"->"<<field<<" ";
//                }
//                i++;
//            }
//            pwkt.x/=n;
//            pwkt.y/=n;
//            pwkt.labelWKT=new Label3DClass();
//            pwkt.labelWKT->position(pwkt.x,pwkt.y,pwkt.z);
//            //pwkt.labelWKT->size((xmax-xmin)/(2.0*pwkt.label.size()));
//            pwkt.labelWKT->size(labelSize*DisplaySettings::windowSizeWidth()/100000.0);
//            //pwkt.labelWKT->size(100000.0*DisplaySettings::distance());
//            //pwkt.labelWKT->size(0.005);
//            pwkt.labelWKT->addText(pwkt.label);
//            pwkt.labelWKT->width(2);
//            pwkt.labelWKT->color(labelR,labelG,labelB,labelA);
//            Shapes::push_back_3D( &Label3DClass::draw , pwkt.labelWKT );
//            cout<<"DisplaySettings"<<DisplaySettings::distance()<<endl;
//        }


//        //cout<<"línea correcta"<<endl;
//        //cout<<endl;
//    }
//    else{
//        cout<<"error"<<endl;
//    }
}


void LoadShapeFileCSV::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("Marker")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", markerR);
                ifTheAttributeIsFoundAssign(ait, "g", markerG);
                ifTheAttributeIsFoundAssign(ait, "b", markerB);
                ifTheAttributeIsFoundAssign(ait, "a", markerA);
                ifTheAttributeIsFoundAssign(ait, "radio", markerRadio);
            }
            float r=markerRadio;
            float l=0.70710678118654752439*r;
            markerX[0]=r;markerY[0]=0.0;
            markerX[1]=l;markerY[1]=l;
            markerX[2]=0;markerY[2]=r;
            markerX[3]=-l;markerY[3]=l;
            markerX[4]=-r;markerY[4]=0.0;
            markerX[5]=-l;markerY[5]=-l;
            markerX[6]=0;markerY[6]=-r;
            markerX[7]=l;markerY[7]=-l;
            marker=true;
        }

        if( nodeName.compare("Stroke")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", strokeR);
                ifTheAttributeIsFoundAssign(ait, "g", strokeG);
                ifTheAttributeIsFoundAssign(ait, "b", strokeB);
                ifTheAttributeIsFoundAssign(ait, "a", strokeA);
                ifTheAttributeIsFoundAssign(ait, "width", strokeWidth);
            }
            stroke=true;
        }

        if( nodeName.compare("Fill")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", fillR);
                ifTheAttributeIsFoundAssign(ait, "g", fillG);
                ifTheAttributeIsFoundAssign(ait, "b", fillB);
                ifTheAttributeIsFoundAssign(ait, "a", fillA);
                ifTheAttributeIsFoundAssign(ait, "spin", fillSpin);
            }
            fill=true;
            //cout<<"fill:"<<fill<<endl;
        }

        if( nodeName.compare("Label")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "column", labelColumn);
                ifTheAttributeIsFoundAssign(ait, "size", labelSize);
                ifTheAttributeIsFoundAssign(ait, "width", labelWidth);
                ifTheAttributeIsFoundAssign(ait, "r", labelR);
                ifTheAttributeIsFoundAssign(ait, "g", labelG);
                ifTheAttributeIsFoundAssign(ait, "b", labelB);
                ifTheAttributeIsFoundAssign(ait, "a", labelA);
            }
            text3d=new Text3D();
        }

        if( nodeName.compare("Cut")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "cut", cut);
                ifTheAttributeIsFoundAssign(ait, "lat0", y0);
                ifTheAttributeIsFoundAssign(ait, "long0", x0);
                ifTheAttributeIsFoundAssign(ait, "lat1", y1);
                ifTheAttributeIsFoundAssign(ait, "long1", x1);
                ifTheAttributeIsFoundAssign(ait, "lat2", y2);
                ifTheAttributeIsFoundAssign(ait, "long2", x2);
            }
        }

        if( nodeName.compare("Transformation")==0 ){
            transf=true;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "westbc",   westbc);
                ifTheAttributeIsFoundAssign(ait, "eastbc",   eastbc);
                ifTheAttributeIsFoundAssign(ait, "northbc",  northbc);
                ifTheAttributeIsFoundAssign(ait, "southbc",  southbc);
                ifTheAttributeIsFoundAssign(ait, "leftbc",   leftbc);
                ifTheAttributeIsFoundAssign(ait, "rightbc",  rightbc);
                ifTheAttributeIsFoundAssign(ait, "bottombc", bottombc);
                ifTheAttributeIsFoundAssign(ait, "topbc",    topbc);
            }
        }

        if( nodeName.compare("FileName")==0 ){

            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "fileName", fileName);
                ifTheAttributeIsFoundAssign(ait, "Id", IdTriang);
            }
        }

        if( nodeName.compare("Projection")==0 ){
            std::string IdProy;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdProy);
            }
            DrawableIdentifier::data(IdProy,proj);
        }

        if( nodeName.compare("BoundingBox")==0 ){
            std::string IdProy;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "boundingBox", boundingBox);
            }
        }

        std::cout <<"/>"<< std::endl;
    }

    if(text3d!=0){
        text3d->projection(proj);
        text3d->setSize(labelSize);
        text3d->setWidth(labelWidth);
    }

    std::ifstream fileLines(fileName.c_str(),std::ios::in);
    if (fileLines.is_open()){

        std::string sline;
        while ( getline(fileLines,sline) ){

           // std::stringstream ssline(sline);
            //std::vector<point> vp2D;
            //bool inside0=false;
           // bool inside1=false;
           // bool isItAClosedCurve=true;
            if(std::string::npos!=sline.find("WKT")){
                std::cout<<"WKT"<<std::endl;

                while ( getline(fileLines,sline) ){
                    PointsWKT WKT;
                    WKT.r=fillR;
                    WKT.g=fillG;
                    WKT.b=fillB;
                    WKT.a=fillA;
                    extraer(sline,WKT);
                    WKTs.push_back(WKT);
                }

                //for(auto s:classification){
                //    cout<<" x :"<<s.name<<endl;
                //}
                cout<<"WKTs.size:"<<WKTs.size()<<endl;
                for(int i=0;i<WKTs.size();i++){
                    //cout<<"WKTs["<<i<<"],type="<<WKTs[i].type<<endl;
                    if(WKTs[i].type==POLYGON || WKTs[i].type==MULTIPOLYGON){
                        for(int j=0;j<WKTs[i].p.size();j++){//geometria
                            //triang.make(WKTs[i].p[j],WKTs[i].);
                            triang.make(WKTs[i].p[j],WKTs[i].r,WKTs[i].g,WKTs[i].b,WKTs[i].a);
                        }
                    }

                }

            }


        }



        if(boundingBox){
            if(Shapes::xtmin>xtmin)Shapes::xtmin=xtmin;
            if(Shapes::xtmax<xtmax)Shapes::xtmax=xtmax;
            if(Shapes::ytmin>ytmin)Shapes::ytmin=ytmin;
            if(Shapes::ytmax<ytmax)Shapes::ytmax=ytmax;
            //cout<<"Shapesx("<<Shapes::xtmin<<","<<Shapes::xtmax<<")";
            //cout<<"-Shapesy("<<Shapes::ytmin<<","<<Shapes::ytmax<<")"<<endl;
        }

        fileLines.close();



        float p0x,p0y,p1x,p1y,p2x,p2y,ux,uy,uLx,uLy,vx,vy,r;
        float lambda;

        //vStroke.resize(vPoints.p.size());

        //        std::cout<<"Construyendo lineas vPoints="<<vPoints.p.size()<<std::endl;
        //        for(unsigned int j=0;j<vPoints.p.size();j++){
        //            if(vPoints.p[j].size()>2){

        //                if( vIsItAClosedCurve[j]==true ){



        //                    //p0x=vPoints[j][vPoints[j].size()-2].longitude; p0y=vPoints[j][vPoints[j].size()-2].latitude;
        //                    //p1x=vPoints[j][0].longitude;   p1y=vPoints[j][0].latitude;
        //                    //p2x=vPoints[j][1].longitude; p2y=vPoints[j][1].latitude;
        //                    //*std::prev(mylist.end()
        //                    p0x=std::prev(vPoints.p[j].end(),2)->longitude; p0y=std::prev(vPoints.p[j].end(),2)->latitude;
        //                    p1x=vPoints.p[j].begin()->longitude;   p1y=vPoints.p[j].begin()->latitude;
        //                    p2x=std::next(vPoints.p[j].begin())->longitude; p2y=std::next(vPoints.p[j].begin())->latitude;

        //                    ux=p1x-p0x; uy=p1y-p0y;
        //                    r=sqrt(ux*ux+uy*uy); ux/=r; uy/=r;
        //                    vx=p2x-p1x; vy=p2y-p1y;
        //                    r=sqrt(vx*vx+vy*vy); vx/=r; vy/=r;
        //                    if(fabs(ux+vx)>fabs(uy+vy))
        //                        lambda=strokeWidth*(vy-uy)/(ux+vx);
        //                    else
        //                        lambda=strokeWidth*(ux-vx)/(uy+vy);

        //                    if(ux*vx+uy*vy<0.0){
        //                        if( fabs(lambda)>10.0*strokeWidth)lambda=0.0;

        //                        if(ux*vy-uy*vx<0.0){
        //                            vStroke[j].push_back(p1x+strokeWidth*uy+lambda*ux);
        //                            vStroke[j].push_back(p1y-strokeWidth*ux+lambda*uy);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x-strokeWidth*uy);
        //                            vStroke[j].push_back(p1y+strokeWidth*ux);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x+strokeWidth*uy+lambda*ux);
        //                            vStroke[j].push_back(p1y-strokeWidth*ux+lambda*uy);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x-strokeWidth*vy);
        //                            vStroke[j].push_back(p1y+strokeWidth*vx);
        //                            vStroke[j].push_back(0.0);
        //                        }
        //                        else{
        //                            vStroke[j].push_back(p1x+strokeWidth*uy);
        //                            vStroke[j].push_back(p1y-strokeWidth*ux);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x-strokeWidth*uy-lambda*ux);
        //                            vStroke[j].push_back(p1y+strokeWidth*ux-lambda*uy);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x+strokeWidth*vy);
        //                            vStroke[j].push_back(p1y-strokeWidth*vx);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x-strokeWidth*uy-lambda*ux);
        //                            vStroke[j].push_back(p1y+strokeWidth*ux-lambda*uy);
        //                            vStroke[j].push_back(0.0);
        //                        }
        //                    }
        //                    else{
        //                        vStroke[j].push_back(p1x+strokeWidth*uy+lambda*ux);
        //                        vStroke[j].push_back(p1y-strokeWidth*ux+lambda*uy);
        //                        vStroke[j].push_back(0.0);
        //                        vStroke[j].push_back(p1x-strokeWidth*uy-lambda*ux);
        //                        vStroke[j].push_back(p1y+strokeWidth*ux-lambda*uy);
        //                        vStroke[j].push_back(0.0);
        //                    }

        //                }
        //                else
        //                {
        //                    //p0x=vPoints[j][0].longitude; p0y=vPoints[j][0].latitude;
        //                    //p1x=vPoints[j][1].longitude; p1y=vPoints[j][1].latitude;
        //                    p0x=vPoints.p[j].begin()->longitude; p0y=vPoints.p[j].begin()->latitude;
        //                    p1x=std::next(vPoints.p[j].begin())->longitude; p1y=std::next(vPoints.p[j].begin())->latitude;
        //                    ux=p1x-p0x; uy=p1y-p0y;
        //                    r=sqrt(ux*ux+uy*uy); ux/=r; uy/=r;
        //                    uLx=uy; uLy=-ux;

        //                    vStroke[j].push_back(p0x+strokeWidth*uLx);
        //                    vStroke[j].push_back(p0y+strokeWidth*uLy);
        //                    vStroke[j].push_back(0.0);
        //                    vStroke[j].push_back(p0x-strokeWidth*uLx);
        //                    vStroke[j].push_back(p0y-strokeWidth*uLy);
        //                    vStroke[j].push_back(0.0);
        //                }

        //                //for(int i=1;i<vPoints[j].size()-1;i++){
        //                for (auto it = std::next(vPoints.p[j].begin()); it != std::prev(vPoints.p[j].end()); ++it) {

        //                    //p0x=vPoints[j][i-1].longitude; p0y=vPoints[j][i-1].latitude;
        //                    //p1x=vPoints[j][i].longitude;   p1y=vPoints[j][i].latitude;
        //                    //p2x=vPoints[j][i+1].longitude; p2y=vPoints[j][i+1].latitude;

        //                    p0x=std::prev(it)->longitude; p0y=std::prev(it)->latitude;
        //                    p1x=it->longitude;   p1y=it->latitude;
        //                    p2x=std::next(it)->longitude; p2y=std::next(it)->latitude;


        //                    ux=p1x-p0x; uy=p1y-p0y;
        //                    r=sqrt(ux*ux+uy*uy); ux/=r; uy/=r;
        //                    vx=p2x-p1x; vy=p2y-p1y;
        //                    r=sqrt(vx*vx+vy*vy); vx/=r; vy/=r;
        //                    if(fabs(ux+vx)>fabs(uy+vy))
        //                        lambda=strokeWidth*(vy-uy)/(ux+vx);
        //                    else
        //                        lambda=strokeWidth*(ux-vx)/(uy+vy);

        //                    if(ux*vx+uy*vy<0.0){
        //                        if( fabs(lambda)>10.0*strokeWidth)lambda=0.0;

        //                        if(ux*vy-uy*vx<0.0){
        //                            vStroke[j].push_back(p1x+strokeWidth*uy+lambda*ux);
        //                            vStroke[j].push_back(p1y-strokeWidth*ux+lambda*uy);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x-strokeWidth*uy);
        //                            vStroke[j].push_back(p1y+strokeWidth*ux);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x+strokeWidth*uy+lambda*ux);
        //                            vStroke[j].push_back(p1y-strokeWidth*ux+lambda*uy);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x-strokeWidth*vy);
        //                            vStroke[j].push_back(p1y+strokeWidth*vx);
        //                            vStroke[j].push_back(0.0);
        //                        }
        //                        else{
        //                            vStroke[j].push_back(p1x+strokeWidth*uy);
        //                            vStroke[j].push_back(p1y-strokeWidth*ux);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x-strokeWidth*uy-lambda*ux);
        //                            vStroke[j].push_back(p1y+strokeWidth*ux-lambda*uy);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x+strokeWidth*vy);
        //                            vStroke[j].push_back(p1y-strokeWidth*vx);
        //                            vStroke[j].push_back(0.0);

        //                            vStroke[j].push_back(p1x-strokeWidth*uy-lambda*ux);
        //                            vStroke[j].push_back(p1y+strokeWidth*ux-lambda*uy);
        //                            vStroke[j].push_back(0.0);
        //                        }
        //                    }
        //                    else{
        //                        vStroke[j].push_back(p1x+strokeWidth*uy+lambda*ux);
        //                        vStroke[j].push_back(p1y-strokeWidth*ux+lambda*uy);
        //                        vStroke[j].push_back(0.0);
        //                        vStroke[j].push_back(p1x-strokeWidth*uy-lambda*ux);
        //                        vStroke[j].push_back(p1y+strokeWidth*ux-lambda*uy);
        //                        vStroke[j].push_back(0.0);
        //                    }
        //                }

        //                if( vIsItAClosedCurve[j]==true ){
        //                    //if( true ){
        //                    float x0=vStroke[j][0];
        //                    float y0=vStroke[j][1];
        //                    float x1=vStroke[j][3];
        //                    float y1=vStroke[j][4];

        //                    vStroke[j].push_back(x0);
        //                    vStroke[j].push_back(y0);
        //                    vStroke[j].push_back(0.0);
        //                    vStroke[j].push_back(x1);
        //                    vStroke[j].push_back(y1);
        //                    vStroke[j].push_back(0.0);

        //                }
        //                else
        //                {
        //                    //p0x=vPoints[j][vPoints[j].size()-2].longitude; p0y=vPoints[j][vPoints[j].size()-2].latitude;
        //                    //p1x=vPoints[j][vPoints[j].size()-1].longitude; p1y=vPoints[j][vPoints[j].size()-1].latitude;
        //                    p0x=std::prev(vPoints.p[j].end(),2)->longitude; p0y=std::prev(vPoints.p[j].end(),2)->latitude;
        //                    p1x=std::prev(vPoints.p[j].end())->longitude; p1y=std::prev(vPoints.p[j].end())->latitude;
        //                    ux=p1x-p0x; uy=p1y-p0y;
        //                    r=sqrt(ux*ux+uy*uy); ux/=r; uy/=r;
        //                    uLx=uy; uLy=-ux;
        //                    uLx=uy; uLy=-ux;

        //                    vStroke[j].push_back(p1x+strokeWidth*uLx);
        //                    vStroke[j].push_back(p1y+strokeWidth*uLy);
        //                    vStroke[j].push_back(0.0);
        //                    vStroke[j].push_back(p1x-strokeWidth*uLx);
        //                    vStroke[j].push_back(p1y-strokeWidth*uLy);
        //                    vStroke[j].push_back(0.0);
        //                }
        //            }
        //        }

        //        if(fill){
        //            vFill.resize(vPoints.p.size());
        //            for(unsigned int j=0;j<vPoints.p.size();j++){//por curva
        //                if( true /*vIsItAClosedCurve[j]==true*/ ){
        //                    //std::cout<<"Tamaño inicialN="<<vPoints[j].size()<<std::endl;
        //                    unsigned int ni=0;
        //                    bool salir=false;
        //                    while(vPoints.p[j].size()>3 && !salir){

        //                        if(ni==vPoints.p[j].size())
        //                            salir=true;
        //                        ni=vPoints.p[j].size();

        //                        //std::cout<<"Tamaño N="<<vPoints[j].size()<<" "<<nn<<std::endl;
        //                        for (auto it = vPoints.p[j].begin(); it!=vPoints.p[j].end(); ++it) {
        //                            auto itp=prevC(it,vPoints.p[j].begin(),vPoints.p[j].end());
        //                            auto itn=nextC(it,vPoints.p[j].begin(),vPoints.p[j].end());

        //                            //std::cout<<"triangulo ("<<itp->longitude<<","<<itp->latitude<<")-("<<it->longitude<<","<<it->latitude<<")-("<<itn->longitude<<","<<itn->latitude<<")"<<std::endl;
        //                            if( orientation(*itn,*it,*itp)>0.0){


        //                                bool addTriangle=true;
        //                                for (auto itc = vPoints.p[j].begin(); itc!=vPoints.p[j].end(); ++itc){
        //                                    //std::cout<<"  punto ("<<itc->longitude<<","<<itc->latitude<<")"<<std::endl;
        //                                    if( itc!=itp && itc!=it && itc!=itn){
        //                                        //std::cout<<"  punto ("<<itc->longitude<<","<<itc->latitude<<")"<<std::endl;
        //                                        if( orientation(*itp,*itc,*it)>0.0 &&
        //                                                orientation(*it,*itc,*itn)>0.0 &&
        //                                                orientation(*itn,*itc,*itp)>0.0 ){
        //                                            addTriangle=false;
        //                                            break;
        //                                        }
        //                                    }
        //                                }

        //                                if(addTriangle){
        //                                    vFill[j].push_back(itp->longitude);
        //                                    vFill[j].push_back(itp->latitude);
        //                                    vFill[j].push_back(0.0);
        //                                    vFill[j].push_back(it->longitude);
        //                                    vFill[j].push_back(it->latitude);
        //                                    vFill[j].push_back(0.0);
        //                                    vFill[j].push_back(itn->longitude);
        //                                    vFill[j].push_back(itn->latitude);
        //                                    vFill[j].push_back(0.0);
        //                                    it = vPoints.p[j].erase (it);

        //                                    break;
        //                                }


        //                            }
        //                        }
        //                    }
        //                }
        //            }
        //        }

    }
    else{
        std::cout<<std::endl<<"Error opening file:"<<fileName<<std::endl;

    }

    if( bClassification && classification.size()>0){
        color_chart=new ColorChart(classification);
        Shapes::push_back_2D( &ColorChart::draw , color_chart );
    }
}



void LoadShapeFileCSV::draw(){
    //glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glDepthMask(false);

    //glBlendFunc (GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);


    //triang.draw();
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glBlendFunc( GL_SRC_ALPHA_SATURATE, GL_ONE );

    //cout<<"en draw fill:"<<fill<<endl;
    if(fill){
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POLYGON_SMOOTH);
        glColor4ub(fillR,fillG,fillB,fillA);
        //glColor4ub(255,0,0,255);
        glLineWidth(1);
        triang.draw();
    }

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    if(stroke){
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glColor4ub(strokeR,strokeG,strokeB,strokeA);
        glLineWidth(strokeWidth);
//        for(int i=0;i<vPoints.size();i++){
//            if(vPoints[i].type==POLYGON || vPoints[i].type==MULTIPOLYGON){
//                for(unsigned int j=0;j<vPoints[i].p.size();j++)
//                {
//                    glBegin(GL_LINE_STRIP);
//                    for (auto it = vPoints[i].p[j].begin(); it != vPoints[i].p[j].end(); ++it) {
//                        glVertex3f(it->longitude,it->latitude,0.0);
//                    }
//                    glEnd();
//                }
//            }
//        }

        for(int i=0;i<WKTs.size();i++){
            for(int j=0;j<WKTs[i].p.size();j++){//geometria
                for(int k=0;k<WKTs[i].p[j].size();k++){
                    glBegin(GL_LINE_STRIP);
                    for(int l=0;l<WKTs[i].p[j][k].size();l++){
                        glVertex3f(WKTs[i].p[j][k][l].longitude,WKTs[i].p[j][k][l].latitude,0.0);
                    }
                    glEnd();
                }
            }
        }
    }

    if(marker){
        glColor4ub(markerR,markerG,markerB,markerA);
        for(int i=0;i<WKTs.size();i++){
            if(WKTs[i].type==POINT){
                for(int j=0;j<WKTs[i].p.size();j++){//geometria
                    for(int k=0;k<WKTs[i].p[j].size();k++){
                        for(int l=0;l<WKTs[i].p[j][k].size();l++){
                            glBegin(GL_TRIANGLE_FAN);
                            glVertex3f(WKTs[i].p[j][k][l].longitude,WKTs[i].p[j][k][l].latitude,0.0);
                            for(int m=0;m<8;m++)
                                glVertex3f(WKTs[i].p[j][k][l].longitude+markerX[m],WKTs[i].p[j][k][l].latitude+markerY[m],0.0);
                            glVertex3f(WKTs[i].p[j][k][l].longitude+markerX[0],WKTs[i].p[j][k][l].latitude+markerY[0],0.0);
                            glEnd();


                        }
                    }
                }
            }
        }
    }

//    if(labelColumn!=0){

//    }

    if(text3d!=0){
        //cout<<"draw text3d"<<endl;
        text3d->draw();
    }
    glDisable (GL_BLEND);
    //glDisable(GL_DEPTH_TEST);
}









