#include "Interpolation.hpp"


//float Interpolation::c=0.000001;

unordered_map< string, Interpolation* > Interpolation::mInterpolation;

Interpolation::Interpolation( string id ){
    cout<<"Interpolation: "<<id<<endl;
    mInterpolation.insert( { id,this } );
    phi= [](double r,double c){return sqrt(r*r+c*c);};
    ck=0.000001;
}

//void Interpolation::init(){
//    std::cout<<"Carga de Sahders para interpolación"<<std::endl;
//    //shaderh.loadFileVertexShader("Texture2D.vert","Texture2D.frag");
//}
/*
float Interpolation::phi( float r ){
    //if(r==0.0)return 0.0;
    return pow(r,2)*log(r);
    //return sqrt(r*r+c*c);
    //return 1.0/sqrt(r*r+c*c);
}*/



void Interpolation::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );


        //        if( nodeName.compare("Cut")==0 ){
        //            std::string IdName;
        //            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        //                ifTheAttributeIsFoundAssign(ait, "getId", IdName);
        //            }
        //            DrawableIdentifier::data(IdName,triang);
        //        }


        if( nodeName.compare("Kernel")==0 ){
            std::string sPhi;
            double c=-1.0;
            double k=-1.0;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){

                ifTheAttributeIsFoundAssign(ait, "phi", sPhi);
                ifTheAttributeIsFoundAssign(ait, "c", c);
                ifTheAttributeIsFoundAssign(ait, "k", k);
                //ifTheAttributeIsFoundAssign(ait, "c", c);
            }
            switch( FnvHash(sPhi.c_str()) ){
            case FnvHash("Multicuadrica"):
                phi= [](double r,double c){return sqrt(r*r+c*c);};
                if(c<0.0)ck=0.000001;
                break;
            case FnvHash("PlacaDelgada"):
                phi= [](double r,double k){return (r>0.0)?pow(r,k)*log(r):0.0;};
                if(k<0.0)ck=2.0;
                break;
            case FnvHash("Potencia"):
                phi= [](double r,double k){return pow(r,k);};
                if(k<0.0)ck=1.0;
                break;
            case FnvHash("Gaussiana"):
                phi= [](double r,double c){return exp(-r*r*c*c);};
                if(c<0.0)ck=0.000001;
                break;
            case FnvHash("InvMuticuadrica"):
                phi= [](double r,double c){return 1.0/sqrt(r*r+c*c);};
                if(c<0.0)ck=0.000001;
                break;
            default:
                cout<<"Función invalida, se asignara multicuádrica"<<endl;
                break;
            }
        }

        if( nodeName.compare("Grid")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "column", column);
                ifTheAttributeIsFoundAssign(ait, "row", row);
            }
        }

        //        if( nodeName.compare("ColorMap")==0 ){
        //            std::string IdName;
        //            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        //                ifTheAttributeIsFoundAssign(ait, "getId", IdName);
        //            }
        //            DrawableIdentifier::data(IdName,color_map);
        //        }

        if( nodeName.compare("Point")==0 ){
            point p;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", p.latitude);
                ifTheAttributeIsFoundAssign(ait, "long", p.longitude);
                ifTheAttributeIsFoundAssign(ait, "value", p.value);
                ifTheAttributeIsFoundAssign(ait, "name", p.name);
            }
            bool repeated=false;
            for(auto &dat: vPoints){
                if( dat.latitude==p.latitude && dat.longitude==p.longitude ){
                    repeated=true;
                    break;
                }
            }

            if(repeated){
                std::cout<<"**Repeated"<<std::endl;
            }
            else{
                vPoints.push_back(p);
            }
        }
        
        if( nodeName.compare("Marker")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", mark.R);
                ifTheAttributeIsFoundAssign(ait, "g", mark.G);
                ifTheAttributeIsFoundAssign(ait, "b", mark.B);
                ifTheAttributeIsFoundAssign(ait, "a", mark.A);
                ifTheAttributeIsFoundAssign(ait, "radio", mark.radio);
                ifTheAttributeIsFoundAssign(ait, "sides", mark.N);
                ifTheAttributeIsFoundAssign(ait, "show", mark.show);
            }
            mark.build();
        }
        
        if( nodeName.compare("Marker2")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", mark2.R);
                ifTheAttributeIsFoundAssign(ait, "g", mark2.G);
                ifTheAttributeIsFoundAssign(ait, "b", mark2.B);
                ifTheAttributeIsFoundAssign(ait, "a", mark2.A);
                ifTheAttributeIsFoundAssign(ait, "radio", mark2.radio);
                ifTheAttributeIsFoundAssign(ait, "sides", mark2.N);
                ifTheAttributeIsFoundAssign(ait, "show", mark2.show);
            }
            mark2.build();
        }
        
        if( nodeName.compare("MarkAprox")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "r", markAprox.R);
                ifTheAttributeIsFoundAssign(ait, "g", markAprox.G);
                ifTheAttributeIsFoundAssign(ait, "b", markAprox.B);
                ifTheAttributeIsFoundAssign(ait, "a", markAprox.A);
                ifTheAttributeIsFoundAssign(ait, "radio", markAprox.radio);
                ifTheAttributeIsFoundAssign(ait, "sides", markAprox.N);
                ifTheAttributeIsFoundAssign(ait, "show", markAprox.show);
            }
            markAprox.build();
        }
        
        if( nodeName.compare("Point2")==0 ){
            point p2;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", p2.latitude);
                ifTheAttributeIsFoundAssign(ait, "long", p2.longitude);
                ifTheAttributeIsFoundAssign(ait, "value", p2.value);
                ifTheAttributeIsFoundAssign(ait, "name", p2.name);
            }
            bool repeated=false;
            for(auto &dat: vPoints2){
                if( dat.latitude==p2.latitude && dat.longitude==p2.longitude ){
                    repeated=true;
                    break;
                }
            }

            if(repeated){
                std::cout<<"**Repeated"<<std::endl;
            }
            else{
                vPoints2.push_back(p2);
            }
        }
        
        
        

        if( nodeName.compare("Approx")==0 ){
            point p;
            p.value=0.0;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", p.latitude);
                ifTheAttributeIsFoundAssign(ait, "long", p.longitude);
                ifTheAttributeIsFoundAssign(ait, "name", p.name);
                //ifTheAttributeIsFoundAssign(ait, "value", p.value);
            }
            vApprox.push_back(p);
        }
        
        if( nodeName.compare("DataFile")==0 ){
            string name;
            string stype;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "type", stype);
                ifTheAttributeIsFoundAssign(ait, "name", name);
                ifTheAttributeIsFoundAssign(ait, "nVar", nVar);

                if(ifTheAttributeIsFoundAssign(ait, "threshold_min",threshold_min)){
                    b_th_min=true;
                }

                if(ifTheAttributeIsFoundAssign(ait, "threshold_max",threshold_max)){
                    b_th_max=true;
                }
                
                if( ifTheAttributeIsFoundAssign(ait, "min", Tmin) )
                    bmin=true;

                if( ifTheAttributeIsFoundAssign(ait, "max", Tmax) )
                    bmax=true;


                
            }

            switch( FnvHash(stype.c_str()) ){
            case FnvHash("regularGrid"):
                typeOfData=0;
                readFileRegularGrid(name);//nueva
                break;
                
            case FnvHash("scattered"):
                cout<<"*nVar="<<nVar<<endl;
                typeOfData=1;
                readFileScattered(name);
                break;
                
            default:
                cout<<"Se requiere que defina el tipo de dato:"<<endl;
                cout<<"type=\"regularGrid\""<<endl;
                cout<<"type=\"scattered\""<<endl;
                break;
            }
        }
        


        if( nodeName.compare("Calculate")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "xmin", xmin);
                ifTheAttributeIsFoundAssign(ait, "ymin", ymin);
                ifTheAttributeIsFoundAssign(ait, "xmax", xmax);
                ifTheAttributeIsFoundAssign(ait, "ymax", ymax);

                ifTheAttributeIsFoundAssign(ait, "column", column);
                ifTheAttributeIsFoundAssign(ait, "row", row);
                ifTheAttributeIsFoundAssign(ait, "average",averaging );//submalla de tamaño averagingxaveraging de donde se obtiene los valores para promediar
                ifTheAttributeIsFoundAssign(ait, "setID", outputDataID);
                if(ifTheAttributeIsFoundAssign(ait, "fileName", fileNameSave)){
                    save=true;
                }
            }
            calculate=true;
        }


        
        if( nodeName.compare("AddData")==0 ){
            string getId,namefileRG;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                if(ifTheAttributeIsFoundAssign(ait, "getId", getId)){
                    unordered_map< string, Interpolation* >::const_iterator it = mInterpolation.find(getId);
                    if ( it == mInterpolation.end() ){
                        cout << "Id: "<< getId <<" not found"<<endl;
                    }
                    else{
                        vInterpolation.push_back( it->second );
                    }
                    
                }

                if(ifTheAttributeIsFoundAssign(ait, "getRG", namefileRG)){
                    readFileRegularGrid(namefileRG );
                }
            }
        }

        
        
        std::cout <<"/>"<< std::endl;
    }

    nXRG=vInterpolationRG.size();
    if(typeOfData==1)//nuevo scattered
        buildLambda();
    
    if(calculate){
        nV.resize(nXP);
        for(int l=0;l<nXP;l++){
            nV[l].resize(nData);
            for(int i=0;i<nData; ++i){
                vector< double > node(nX);
                for(int k=0;k<nX;k++)
                    node[k]=nTuple[k][i];
                nV[l][i]=vInterpolation[l]->interpolate(node);
            }
        }

        nVRG.resize(nXRG);
        for(int l=0;l<nXRG;l++){
            nVRG[l].resize(nData);
            for(int i=0;i<nData; ++i){
                vector< double > node(nX);
                for(int k=0;k<nX;k++)
                    node[k]=nTuple[k][i];
                nVRG[l][i]=interpolateRG(l,node);
            }
        }
        data=new DataRaw2D();

        if(!outputDataID.empty()){
            DrawableIdentifier::insert(outputDataID,data);
        }

        data->column=column;
        data->row=row;
        data->val.resize(data->column*data->row);
        data->xmin=xmin;
        data->ymin=ymin;
        data->xmax=xmax;
        data->ymax=ymax;
        data->name=outputDataID;

        vector< double > node(2);
        vector< double > subNode(2);
        for(int j=0;j<data->row;j++){
            //double ly=(double)j/(data->row-1);
            for(int i=0;i<data->column;i++){
                //double lx=(double)i/(data->column-1);
                //node[0]=plong0+lx*dlong10+ly*dlong30+lx*ly*dlong0123;
                //node[1]=plat0+lx*dlat10+ly*dlat30+lx*ly*dlat0123;
                //data->val[j*data->column+i]=nTmin[nX]+interpolate(node)*(nTmax[nX]-nTmin[nX]);
                node[0]=data->xmin+(data->xmax-data->xmin)*(i+0.5)/data->column;
                node[1]=data->ymin+(data->ymax-data->ymin)*(j+0.5)/data->row;

                if(averaging==1){
                    //cout<<"data="<<nTmin[nX]<<endl;
                    data->val[j*data->column+i]=nTmin[nX]+interpolate(node)*(nTmax[nX]-nTmin[nX]);
                    // cout<<data->val[j*data->column+i]<<endl;
                    //data->val[j*data->column+i]=nTmin[nX];
                }
                else{
                    data->val[j*data->column+i]=0.0;
                    for(int jAverage=0;jAverage<averaging;jAverage++){
                        float ja=(float)jAverage/averaging;
                        subNode[1]=data->ymin+(data->ymax-data->ymin)*(j+ja)/data->row;
                        for(int iAverage=0;iAverage<averaging;iAverage++){
                            float ia=(float)iAverage/averaging;
                            subNode[0]=data->xmin+(data->xmax-data->xmin)*(i+ia)/data->column;
                            data->val[j*data->column+i]+=nTmin[nX]+interpolate(subNode)*(nTmax[nX]-nTmin[nX]);
                        }
                    }
                    data->val[j*data->column+i]/=(averaging*averaging);
                    //data->val[j*data->column+i]=(6.79e-6)*pow(data->val[j*data->column+i],1.6);
                    //6.79 x 10^-6  x Sa^1.6 ; ;
                }


                if( data->max < data->val[j*data->column+i] )
                    data->max = data->val[j*data->column+i];
                if( data->min > data->val[j*data->column+i] )
                    data->min = data->val[j*data->column+i];
            }
        }
        cout<<"Interpolación"<<endl;
        cout<<"Mínimo= "<<data->min<<endl;
        cout<<"Máximo= "<<data->max<<endl;

        std::ofstream fileSave;//(fileName.c_str(),std::ios::out);
        if(save){
            fileSave.open(fileNameSave.c_str(),std::ios::out);
            if (fileSave.is_open()){
                cout<<"Guardando interpolación en el archivo:"<<fileNameSave<<endl;
                fileSave<<"ncols "<<data->column<<endl;
                fileSave<<"nrows "<<data->row<<endl;
                fileSave<<"xllcorner "<<xmin<<endl;
                fileSave<<"yllcorner "<<ymin<<endl;
                fileSave<<"xcellsize "<<(xmax-xmin)/(data->column-1)<<endl;
                fileSave<<"ycellsize "<<(ymax-ymin)/(data->row-1)<<endl;
                for(int j=0;j<data->row;j++){
                    for(int i=0;i<data->column-1;i++){
                        fileSave<<data->val[j*data->column+i]<<" ";
                    }
                    fileSave<<data->val[j*data->column+data->column-1]<<endl;
                }
            }
            else{
                cout<<"No se pudo crear el archivo: "<<fileNameSave<<endl;
                save=false;
            }
        }

        //        if(save){
        //            for(int j=0;j<data->row;j++){
        //                double ly=(double)j/(data->row-1);
        //                for(int i=0;i<data->column-1;i++){
        //                    double lx=(double)i/(data->column-1);
        //                    node[0]=plong0+lx*dlong10+ly*dlong30+lx*ly*dlong0123;
        //                    node[1]=plat0+lx*dlat10+ly*dlat30+lx*ly*dlat0123;
        //                    data->val[j*data->column+i]=nTmin[nX]+interpolate(node)*(nTmax[nX]-nTmin[nX]);
        //                    if( data->max < data->val[j*data->column+i] )
        //                        data->max = data->val[j*data->column+i];
        //                    if( data->min > data->val[j*data->column+i] )
        //                        data->min = data->val[j*data->column+i];
        //                    fileSave<<data->val[j*data->column+i]<<" ";
        //                }
        //                double lx=(double)(data->column-1)/(data->column-1);
        //                node[0]=plong0+lx*dlong10+ly*dlong30+lx*ly*dlong0123;
        //                node[1]=plat0+lx*dlat10+ly*dlat30+lx*ly*dlat0123;
        //                data->val[j*data->column+data->column-1]=nTmin[nX]+interpolate(node)*(nTmax[nX]-nTmin[nX]);
        //                if( data->max < data->val[j*data->column+data->column-1] )
        //                    data->max = data->val[j*data->column+data->column-1];
        //                if( data->min > data->val[j*data->column+data->column-1] )
        //                    data->min = data->val[j*data->column+data->column-1];
        //                fileSave<<data->val[j*data->column+data->column-1]<<endl;
        //            }
        //        }
        //        else{
        //            for(int j=0;j<data->row;j++){
        //                double ly=(double)j/(data->row-1);
        //                for(int i=0;i<data->column;i++){
        //                    double lx=(double)i/(data->column-1);
        //                    node[0]=plong0+lx*dlong10+ly*dlong30+lx*ly*dlong0123;
        //                    node[1]=plat0+lx*dlat10+ly*dlat30+lx*ly*dlat0123;
        //                    data->val[j*data->column+i]=nTmin[nX]+interpolate(node)*(nTmax[nX]-nTmin[nX]);
        //                    if( data->max < data->val[j*data->column+i] )
        //                        data->max = data->val[j*data->column+i];
        //                    if( data->min > data->val[j*data->column+i] )
        //                        data->min = data->val[j*data->column+i];
        //                }
        //            }
        //        }


        cout<<"Approx"<<endl;
        for (auto &nApprox : vApprox){
            node[0]=nApprox.longitude;
            node[1]=nApprox.latitude;
            cout<<nApprox.name<<" "<<node[0]<<" "<<node[1]<<" "<<nTmin[nX]+interpolate(node)*(nTmax[nX]-nTmin[nX])<<endl;
        }

        //        if(!shaderh.setShaders2()){
        //            std::cout<<std::endl<<"ERROR en shader de interpolación!!!"<<std::endl;
        //        }

        //        shaderh.vector1fvLink("West", long0);
        //        shaderh.vector1fvLink("East", long1);
        //        shaderh.vector1fvLink("South", lat0);
        //        shaderh.vector1fvLink("North", lat2);
        //        shaderh.vector1iLink("remove", 0);
        //        shaderh.vector1iLink("isovalue_N", 0);
        //        shaderh.vector1fvLink("isovalue_width", 1.0);

        //        //        glGenTextures(1, &tex);
        //        //        glGenTextures(1, &texColorMap);
        //        //        glGenTextures(1, &texAlphaMap);
        //        setColorMap();
        //        setTexture2D();
    }
}


void Interpolation::draw(){
    //    glUseProgramObjectARB(0);
    //    glEnable(GL_DEPTH_TEST);
    //    glEnable (GL_BLEND);

    //    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //    if(view){
    //        //glEnable(GL_CULL_FACE);

    //        //glBlendFunc (GL_ONE, GL_SRC_ALPHA);
    //        //glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    //        //glShadeModel(GL_SMOOTH);
    //        //glEnable(GL_DEPTH_TEST);
    //        //glEnable(GL_CULL_FACE);
    //        glLineWidth(1);


    //        glBindTexture(GL_TEXTURE_2D,tex);
    //        glBindTexture(GL_TEXTURE_1D,texColorMap);
    //        glBindTexture(GL_TEXTURE_1D,texAlphaMap);

    //        shaderh.useProgram();
    //        glColor4f(1.0,0.0,0.0,1.0);

    //        if(triang!=NULL){
    //            triang->draw();
    //        }
    //        else{

    //            glBegin(GL_QUADS);
    //            glVertex3f( long0,lat0,0.0);
    //            glVertex3f( long1,lat1,0.0);
    //            glVertex3f( long2,lat2,0.0);
    //            glVertex3f( long3,lat3,0.0);
    //            glEnd();

    //        }

    //        glUseProgramObjectARB(0);
    //        /*
    //        glColor4f(0.6,0.7,0.2,1.0);
    //        for(int j=0;j<data->row;j++)
    //            for(int i=0;i<data->column;i++){
    //                int index_ij=j*data->column+i;
    //                if(data->val[index_ij]!=0){
    //                    std::stringstream text;
    //                    text << std::fixed<< std::setprecision(5) << data->val[index_ij];
    //                    glPushMatrix();
    //                    glTranslatef(long0+(long1-long0)*(i+0.2)/data->column, lat0+(lat3-lat0)*(j+0.2)/data->row,0.0f);
    //                    glScalef(0.00001,0.00001,0.00001);
    //                    for (unsigned int i=0; i<text.str().length(); i++)
    //                        glutStrokeCharacter(GLUT_STROKE_ROMAN, text.str().c_str()[i]);
    //                    glPopMatrix();
    //                }
    //            }
    //*/

    //        //                glBegin(GL_QUADS);
    //        //                for(int j=0;j<data->row-1;j++)
    //        //                    for(int i=0;i<data->column-1;i++){
    //        //                        int index_ij=j*data->column+i;
    //        //                        int index_i1j=j*data->column+i+1;
    //        //                        int index_ij1=(j+1)*data->column+i;
    //        //                        int index_i1j1=(j+1)*data->column+i+1;

    //        //                        color_map->transferColor(data->val[index_ij],r,g,b);
    //        //                        color_map->transferAlpha(data->val[index_ij],alpha);
    //        //                        //color_map->transferLambda(data->val[index_ij],lambda);
    //        //                        glColor4ub( r,g,b,alpha);
    //        //                        float lx=(float)i/(data->column-1);
    //        //                        float ly=(float)j/(data->row-1);
    //        //                        float px=long0+lx*(long1-long0)+ly*(long3-long0)+lx*ly*(long0-long1+long2-long3);
    //        //                        float py=lat0+lx*(lat1-lat0)+ly*(lat3-lat0)+lx*ly*(lat0-lat1+lat2-lat3);
    //        //                        glVertex3f( px,py,0.0);
    //        //                        //glVertex3f( long0+(long1-long0)*i/(data->column-1),lat0+(lat1-lat0)*j/(data->row-1),0.0);//0.0002*val

    //        //                        color_map->transferColor(data->val[index_i1j],r,g,b);
    //        //                        color_map->transferAlpha(data->val[index_i1j],alpha);
    //        //                        //color_map->transferLambda(data->val[index_i1j],lambda);
    //        //                        glColor4ub( r,g,b,alpha);
    //        //                        lx=(float)(i+1)/(data->column-1);
    //        //                        ly=(float)j/(data->row-1);
    //        //                        px=long0+lx*(long1-long0)+ly*(long3-long0)+lx*ly*(long0-long1+long2-long3);
    //        //                        py=lat0+lx*(lat1-lat0)+ly*(lat3-lat0)+lx*ly*(lat0-lat1+lat2-lat3);
    //        //                        glVertex3f( px,py,0.0);
    //        //                        //glVertex3f( long0+(long1-long0)*(i+1)/(data->column-1),lat0+(lat1-lat0)*j/(data->row-1),0.0);

    //        //                        color_map->transferColor(data->val[index_i1j1],r,g,b);
    //        //                        color_map->transferAlpha(data->val[index_i1j1],alpha);
    //        //                        //color_map->transferLambda(data->val[index_i1j1],lambda);
    //        //                        glColor4ub( r,g,b,alpha);
    //        //                        lx=(float)(i+1)/(data->column-1);
    //        //                        ly=(float)(j+1)/(data->row-1);
    //        //                        px=long0+lx*(long1-long0)+ly*(long3-long0)+lx*ly*(long0-long1+long2-long3);
    //        //                        py=lat0+lx*(lat1-lat0)+ly*(lat3-lat0)+lx*ly*(lat0-lat1+lat2-lat3);
    //        //                        glVertex3f( px,py,0.0);
    //        //                        //glVertex3f( long0+(long1-long0)*(i+1)/(data->column-1),lat0+(lat1-lat0)*(j+1)/(data->row-1),0.0);

    //        //                        color_map->transferColor(data->val[index_ij1],r,g,b);
    //        //                        color_map->transferAlpha(data->val[index_ij1],alpha);
    //        //                        //color_map->transferLambda(data->val[index_ij1],lambda);
    //        //                        glColor4ub( r,g,b,alpha);
    //        //                        lx=(float)i/(data->column-1);
    //        //                        ly=(float)(j+1)/(data->row-1);
    //        //                        px=long0+lx*(long1-long0)+ly*(long3-long0)+lx*ly*(long0-long1+long2-long3);
    //        //                        py=lat0+lx*(lat1-lat0)+ly*(lat3-lat0)+lx*ly*(lat0-lat1+lat2-lat3);
    //        //                        glVertex3f( px,py,0.0);
    //        //                        //glVertex3f( long0+(long1-long0)*i/(data->column-1),lat0+(lat1-lat0)*(j+1)/(data->row-1),0.0);
    //        //                    }
    //        //                glEnd();


    //        if(mark.show){
    //            //glEnable(GL_POLYGON_SMOOTH);
    //            for(int i=0; i<nTuple[0].size(); i++){
    //                //for(int i=0; i<10; i++){
    //                glColor4ub(mark.R,mark.G,mark.B,mark.A);
    //                glPushMatrix();
    //                glTranslatef(nTuple[0][i],nTuple[1][i],0.01);
    //                //glTranslatef(0.0,0.0,0.0);
    //                glBegin(GL_POLYGON);
    //                for(auto& v: mark.v){
    //                    glVertex3f(v.first,v.second,0.0);
    //                }
    //                glEnd();
    //                glPopMatrix();
    //            }
    //            //glDisable(GL_POLYGON_SMOOTH);
    //        }




    //        //glDisable(GL_CULL_FACE);
    //    }

    //    glUseProgramObjectARB(0);
    //    glDisable(GL_BLEND);

    //    glDisable(GL_DEPTH_TEST);
}


int Interpolation::buildLambda(double c){

    //std::function<float(float)> phi;
    //phi=std::function<float(float)>([](float r){return sqrt(r*r+0.000001*0.000001);});//Multiquadric

    //    int nVar=nTuple.size();
    //int nData=nTuple[0].size();
    //int nX=nVar-1;
    int indexY=nX;

    if(nVar<2){
        cout<<"Error: Se requieren por lo menos 2 variable"<<endl;
        return 0;
    }

    for(int i=0;i<nX;i++){
        if(nTuple[i].size()!=nTuple[i+1].size()){
            cout<<"Error: No coinciden los tamaños de las variables"<<endl;
            return 0;
        }
    }

    /*
    for(int i=0;i<nTuple.size();i++){
        for(int j=0;j<nTuple[i].size();j++){
            cout<<fixed<<std::setprecision(4)<<nTuple[i][j]<<" ";
            }
        cout<<endl;
    }
    */

    nXP=vInterpolation.size();
    //nXRG=vInterpolationRG.size();
    MatrixXd G(nData+nX+nXP+nXRG+1,nData+nX+nXP+nXRG+1);
    VectorXd b(nData+nX+nXP+nXRG+1);
    //VectorXf lambda(nData+nX+1);
    //lambda.push_back( VectorXf(nData+nX+1) );
    lambda.resize(nData+nX+nXP+nXRG+1);

    //G.setZero();
    //b.setZero();

    cout<<"llenando Gij"<<endl;
    for(int i=0; i<nData; ++i){
        b(i)=nTuple[indexY][i];
        //G(i,i)=phi(0.0);
        //G(i,i)=c;
        G(i,i)=phi(0.0,ck);
        for(int j=i+1; j<nData; ++j){
            double rr=0.0;
            for(int k=0;k<nX;k++){//datos dispersos
                rr+=pow( nTuple[k][i] - nTuple[k][j] , 2);
            }

            if(nXP!=0 || nXRG!=0){
                vector< double > node1(nX);
                vector< double > node2(nX);
                for(int k=0;k<nX;k++){
                    node1[k]=nTuple[k][i];
                    node2[k]=nTuple[k][j];
                }
                for(int l=0;l<nXP;l++){//datos de una interpolación previa
                    double v1=vInterpolation[l]->interpolate(node1);
                    double v2=vInterpolation[l]->interpolate(node2);
                    rr+=pow( v1 - v2 , 2);
                }
                for(int l=0;l<nXRG;l++){//datos de una malla regular
                    double v1=interpolateRG(l,node1);
                    double v2=interpolateRG(l,node2);
                    rr+=pow( v1 - v2 , 2);
                }
            }

            double r=sqrt( rr );
            G(i,j)=G(j,i)=phi(r,ck);
            //G(i,j)=G(j,i)=sqrt(rr+c*c);
        }
    }


    cout<<"llenando GNij"<<endl;
    for(int i=0; i<nData; ++i){
        for(int k=0;k<nX;k++)
            G(nData+k,i)=G(i,nData+k)=nTuple[k][i];

        if(nXP!=0 || nXRG!=0){
            vector< double > node(nX);
            for(int k=0;k<nX;k++)
                node[k]=nTuple[k][i];
            for(int l=0;l<nXP;l++)
                G(nData+nX+l,i)=G(i,nData+nX+l)=vInterpolation[l]->interpolate(node);
            for(int l=0;l<nXRG;l++)
                G(nData+nX+nXP+l,i)=G(i,nData+nX+nXP+l)=interpolateRG(l,node);
        }
        G(nData+nX+nXP+nXRG,i)=G(i,nData+nX+nXP+nXRG)= 1.0;
    }

    for(int i=0; i<nX+nXP+nXRG+1; ++i){
        b(nData+i)=0.0;
        for(int j=0; j<nX+nXP+nXRG+1; ++j)
            G(nData+i,nData+j)=0.0;
    }

    //cout<<"b="<<b<<endl;
    //cout<<"G="<<G<<endl;
    cout<<"resolviendo sistema"<<endl;

    lambda = G.colPivHouseholderQr().solve(b);
    //lambda = G.householderQr().solve(b);


    //cout<<"lambda="<<lambda<<endl;
    cout<<"calculando error relativo"<<endl;
    //cout<<lambda<<endl;
    double relative_error = (G*lambda - b).norm() / b.norm(); // norm() is L2 norm
    cout <<std::scientific<<std::setprecision(8)<< "error relativo = " << relative_error << " "<<endl;

    double err,emax=0.0;
    for(int i=0; i<nData; ++i){
        double u_aprox=0;
        for(int j=0; j<nData; ++j){
            double rr=0.0;
            for(int k=0;k<nX;k++){
                rr+=pow( nTuple[k][i] - nTuple[k][j] , 2);
            }

            if(nXP!=0 || nXRG!=0){
                vector< double > node1(nX);
                vector< double > node2(nX);
                for(int k=0;k<nX;k++){
                    node1[k]=nTuple[k][i];
                    node2[k]=nTuple[k][j];
                }
                for(int l=0;l<nXP;l++){
                    double v1=vInterpolation[l]->interpolate(node1);
                    double v2=vInterpolation[l]->interpolate(node2);
                    rr+= ( v1 - v2 )*( v1 - v2 );
                }
                for(int l=0;l<nXRG;l++){
                    double v1=interpolateRG(l,node1);
                    double v2=interpolateRG(l,node2);
                    rr+= ( v1 - v2 )*( v1 - v2 );
                }
            }

            double r=sqrt( rr );
            u_aprox+=phi(r,ck)*lambda(j);
            //u_aprox+=sqrt(rr+c*c)*lambda(j);
        }
        for(int k=0;k<nX;k++)
            u_aprox+=lambda(nData+k)*nTuple[k][i];
        if(nXP!=0 || nXRG!=0){
            vector< double > node(nX);
            for(int k=0;k<nX;k++){
                node[k]=nTuple[k][i];
            }
            for(int l=0;l<nXP;l++)
                u_aprox+=lambda(nData+nX+l)*vInterpolation[l]->interpolate(node);
            for(int l=0;l<nXRG;l++)
                u_aprox+=lambda(nData+nX+nXP+l)*interpolateRG(l,node);
        }
        u_aprox+=lambda(nData+nX+nXP+nXRG);

        err = fabs((u_aprox-nTuple[indexY][i])*(nTmax[nX]-nTmin[nX]));
        //cout<<"x="<<x[indexY][i]<<" xaprox="<<u_aprox<<" err="<<err<<endl;
       // cout<<"f="<<nTuple[indexY][i]<<" f_aprox="<<u_aprox<<" err="<<err<<endl;
        emax = err>emax? err : emax;
    }
    std::cout<<std::fixed<<"error máximo="<<emax<<std::endl;
    return 0;
}

int Interpolation::readFileRegularGrid(string fileName ){
    std::ifstream myfile(fileName.c_str(),std::ios::in);

    rg *datarg=new rg;
    int nodata_value;
    int column,row;

    nTmin.resize(3);
    nTmax.resize(3);

    if (myfile.is_open()){
        cout<<"Abriendo archivo asc:"<<fileName<<endl;
        //int iLine=1;
        std::string line;
        vector <string> fields;

        getline(myfile,line);
        split( fields, line, is_any_of( " " ) );
        stringstream(fields[1])>>column;

        getline(myfile,line);
        split( fields, line, is_any_of( " " ) );
        stringstream(fields[1])>>row;

        getline(myfile,line);
        split( fields, line, is_any_of( " " ) );
        stringstream(fields[1])>>datarg->xi;

        getline(myfile,line);
        split( fields, line, is_any_of( " " ) );
        stringstream(fields[1])>>datarg->yi;

        getline(myfile,line);
        split( fields, line, is_any_of( " " ) );
        stringstream(fields[1])>>datarg->cellsize;

        getline(myfile,line);
        split( fields, line, is_any_of( " " ) );
        stringstream(fields[1])>>nodata_value;

        cout<<"ncols:"<<column<<" nrow:"<<row;
        cout<<" xllcorner:"<<datarg->xi<<" yllcorner:"<<datarg->yi;
        cout<<" cellsize:"<<datarg->cellsize<<endl;

        datarg->data2D=new DataRaw2D;
        datarg->data2D->column=column;
        datarg->data2D->row=row-1;
        datarg->xf = datarg->xi + datarg->cellsize*datarg->data2D->column;
        datarg->yf = datarg->yi + datarg->cellsize*datarg->data2D->row;

        datarg->data2D->val.resize(datarg->data2D->column*datarg->data2D->row);
        double h;
        int i=0,j=datarg->data2D->row-1;
        for(unsigned int k=0;k<datarg->data2D->val.size();k++){
            myfile>>h;
            datarg->data2D->val[j*datarg->data2D->column+i]=h;
            if( datarg->data2D->min > h )datarg->data2D->min=h;
            if( datarg->data2D->max < h )datarg->data2D->max=h;
            i++;
            if(i>=datarg->data2D->column){
                i=0;
                j--;
            }
            //if(j>=datarg->data2D->row)j=0;
        }

        nTmin[0]=datarg->xi; nTmax[0]=datarg->xf;
        nTmin[1]=datarg->yi; nTmax[1]=datarg->yf;
        nTmin[0]=datarg->data2D->min; nTmax[0]=datarg->data2D->max;

        cout<<"datos:"<<datarg->data2D->val[0]<<" "<<datarg->data2D->val[1]<<" "<<datarg->data2D->val[2]<<" ... ";
        cout<<datarg->data2D->val[ datarg->data2D->val.size()-3 ]<<" ";
        cout<<datarg->data2D->val[ datarg->data2D->val.size()-2 ]<<" ";
        cout<<datarg->data2D->val[ datarg->data2D->val.size()-1 ]<<endl;


        /*
        while ( getline(myfile,sline)){
            vector <string> fields;
            switch(iLine){
            case 1:

                break;
            case 2:
                split( fields, line, is_any_of( " " ) );
                stringstream(fields[1])>>nrows;
                break;
            case 3:
                split( fields, line, is_any_of( " " ) );
                stringstream(fields[1])>>xllcorner;
                break;
            case 4:
                split( fields, line, is_any_of( " " ) );
                stringstream(fields[1])>>yllcorner;
                break;
            case 5:
                split( fields, line, is_any_of( " " ) );
                stringstream(fields[1])>>cellsize;
                break;
            case 6:
                split( fields, line, is_any_of( " " ) );
                stringstream(fields[1])>>nodata_value;
                break;
            }

            iLine++;


        }
        */
        vInterpolationRG.push_back(datarg);

    }
    else
        cout<<"No se pudo leer el archivo: "<<fileName<<endl;


    return 0;
}

//regular grid
double Interpolation::interpolateRG(int l,vector< double > &node){
    //static int kk=0;
    if(node.size()==2){
        double di=(double)(node[0]-vInterpolationRG[l]->xi)*(vInterpolationRG[l]->data2D->column-1)/(vInterpolationRG[l]->xf-vInterpolationRG[l]->xi);
        double dj=(double)(node[1]-vInterpolationRG[l]->yi)*(vInterpolationRG[l]->data2D->row-1)/(vInterpolationRG[l]->yf-vInterpolationRG[l]->yi);
        int i=di;
        int j=dj;
        if(i < 0)i=0;
        if(j < 0)j=0;
        if(i > vInterpolationRG[l]->data2D->column-1) i = vInterpolationRG[l]->data2D->column-1;
        if(j > vInterpolationRG[l]->data2D->row-1) j = vInterpolationRG[l]->data2D->row-1;

        //        if(kk<100){
        //            cout<<node[0]<<" "<<node[1];
        //            cout<<" ("<< vInterpolationRG[l]->xi<<","<<vInterpolationRG[l]->xf<<")";
        //            cout<<" ("<< vInterpolationRG[l]->yi<<","<<vInterpolationRG[l]->yf<<")";
        //            cout<<" di="<<di<<" dj="<<dj<<" i="<<i<<" j="<<j<<" y=";
        //            cout<<vInterpolationRG[l]->data2D->val[j*vInterpolationRG[l]->data2D->column+i]<<" ";
        //            cout<<(vInterpolationRG[l]->data2D->val[j*vInterpolationRG[l]->data2D->column+i]-vInterpolationRG[l]->data2D->min)/(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min)<<endl;
        //        }
        //        kk++;

        return (vInterpolationRG[l]->data2D->val[j*vInterpolationRG[l]->data2D->column+i]-vInterpolationRG[l]->data2D->min)/(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min);
    }
    else{
        cout<<"tamaño de nodo incorrecto en interpolación"<<endl;
        return 0.0;
    }


}

int Interpolation::readFileScattered(string fileName){
    //string fileName="../../DatosGeo/DeepDepositsDepth/salida_Contours.txt";

    std::ifstream fileLines(fileName.c_str(),std::ios::in);

    //vector< VectorXf > x;

    if (fileLines.is_open()){
        cout<<"Abriendo archivo de datos:"<<fileName<<endl;
        std::string sline;
        std::stringstream ssline;
        int iLine=0;

        vector< vector<double> > _x;
        vector<double> _xtemp;
        _x.resize(nVar);
        _xtemp.resize(nVar);
        nTmin.resize(nVar);
        nTmax.resize(nVar);
        for(int iVar=0;iVar<nVar;iVar++){
            nTmin[iVar]=std::numeric_limits<double>::max();
            nTmax[iVar]=-std::numeric_limits<double>::max();
        }




        while ( getline(fileLines,sline)){
            //cout<<iLine;
            //cout<<" ss="<<(int)sline.c_str()[1]<<" "<<sline.empty()<<endl;
            if(!sline.empty()){
                std::stringstream ssline(sline);

                for(int iVar=0;iVar<nVar;iVar++){
                    if( !(ssline>>_xtemp[iVar]) ){
                        cout<<endl<<"Error en la línea "<<iLine<<" del archivo:"<<endl;
                        cout<<fileName<<endl;
                        cout<<"Se esperaban leer "<<nVar<<" datos"<<endl;
                        cout<<sline<<endl;
                        return 0;
                    }
                }

                bool repeated=false;
                for(unsigned int i=0;i<_x[0].size();i++){
                    bool equal=true;
                    for(int iVar=0;iVar<nVar-1;iVar++){
                        //cout<<_x[iVar][i]<<" "<<_xtemp[iVar]<<" / ";
                        if(_x[iVar][i]!=_xtemp[iVar]){
                            equal=false;
                            break;
                        }
                    }
                    //cout<<endl;
                    if(equal==true){
                        //cout<<"datos repetidos"<<endl;
                        repeated=true;
                        break;
                    }
                }


                if(!repeated){
                    for(int iVar=0;iVar<nVar;iVar++){
                        _x[iVar].push_back(_xtemp[iVar]);
                        if( nTmin[iVar] > _xtemp[iVar] ) nTmin[iVar] = _xtemp[iVar];
                        if( nTmax[iVar] < _xtemp[iVar] ) nTmax[iVar] = _xtemp[iVar];

                        //cout<<" x["<<iVar<<"]["<<_x[iVar].size()-1<<"]="<<_x[iVar][_x[iVar].size()-1];
                    }

                }
                else{
                    //cout<<"**Repeated";
                }
            }
            iLine++;
            //cout<<endl;
            //sline.clear();
        }


        cout<<"min max de datos:"<<endl;
        for(int iVar=0;iVar<nVar;iVar++){
            cout<<iVar<<" min="<<nTmin[iVar]<<" max="<<nTmax[iVar]<<endl;
        }
        cout<<"min max definida"<<endl;
        if(bmin)nTmin[nVar-1]=Tmin;
        if(bmax)nTmax[nVar-1]=Tmax;
        cout<<nVar-1<<" min="<<nTmin[nVar-1]<<" max="<<nTmax[nVar-1]<<endl;

        nTuple.resize(nVar);
        /*for(int i=0;i<nVar;i++) {
            nTuple[i]=VectorXf::Map(_x[i].data(), _x[i].size());
        }*/

        nData=_x[0].size();
        nX=nVar-1;

        cout<<"nData="<<nData<<endl;
        cout<<"nVar="<<nVar<<endl;
        cout<<"nX="<<nX<<endl;

        for(int iVar=0;iVar<nVar;iVar++){
            nTuple[iVar].resize(nData);
            double dmaxmin=nTmax[iVar]-nTmin[iVar];
            if(iVar==nVar-1){
                threshold_min=(threshold_min-nTmin[iVar])/dmaxmin;
                threshold_max=(threshold_max-nTmin[iVar])/dmaxmin;
            }
            for(int j=0;j<nData;j++){
                nTuple[iVar][j]=_x[iVar][j];
                if(iVar==nVar-1)nTuple[iVar][j]=(_x[iVar][j]-nTmin[iVar])/dmaxmin;
            }
        }





        /*
        VectorXf vv = VectorXf::Map(x);

        std::vector<std::complex<double> > v2(10);
  VectorXcd v3 = VectorXcd::Map(v2.data(), v2.size());
  */

    }
    else
        cout<<"No se pudo leer el archivo: "<<fileName<<endl;


    return 0;
}


double Interpolation::interpolate(vector< double > &node ){

    if(typeOfData==0){
        return interpolateRG(0,node);
    }
    double y=0;
    vector<double> vP(nXP+nXRG);
    for(int l=0;l<nXP;l++)
        vP[l]=vInterpolation[l]->interpolate(node);
    for(int l=0;l<nXRG;l++)
        vP[nXP+l]=interpolateRG(l,node);

    for(int j=0; j<nData; ++j){
        double rr=0.0;
        for(int k=0;k<nX;k++){
            rr+=(node[k] - nTuple[k][j])*(node[k] - nTuple[k][j]);
        }

        if(nXP!=0 || nXRG!=0){
            //cout<<"nXP="<<nXP<<endl;
            /*
            vector< float > node2(nX);
            for(int k=0;k<nX;k++){
                node2[k]=nTuple[k][j];
            }
            for(int l=0;l<nXP;l++){
                float v2=vInterpolation[l]->interpolate(node2);
                rr+=(vP[l] - v2)*(vP[l] - v2);
            }
            */
            for(int l=0;l<nXP;l++)
                rr+=(vP[l] - nV[l][j])*(vP[l] - nV[l][j]);
            for(int l=0;l<nXRG;l++)
                rr+=(vP[nXP+l] - nVRG[l][j])*(vP[nXP+l] - nVRG[l][j]);


        }
        double r=sqrt( rr );
        //x2[indexY][i]+=phi(r)*lambda(j);
        //y+=sqrt(rr+c*c)*lambda(j);
        y+=phi(r,ck)*lambda(j);
        //cout<<sqrt(rr+c*c)<<" "<<phi(sqrt(rr),c)<<endl;
        //y+=phi(rr,c)*lambda(j);
    }

    for(int k=0;k<nX;k++)
        y+=lambda(nData+k)*node[k];

    for(int l=0;l<nXP;l++)
        y+=lambda(nData+nX+l)*vP[l];

    for(int l=0;l<nXRG;l++)
        y+=lambda(nData+nX+nXP+l)*vP[nXP+l];

    y+=lambda(nData+nX+nXP+nXRG);
    /*
        cout<<"(";
        for(int k=0;k<nX-1;k++){
                cout<<node[k]<<",";
            }
        cout<<node[nX-1]<<")="<<y<<endl;
*/
    if(b_th_min && y<=threshold_min)return threshold_min;
    if(b_th_max && y>=threshold_max)return threshold_max;
    return y;
}

//void Interpolation::setTexture2D(){
//    //int N=data.3D->Nx*_data3D->Ny*_data3D->Nz;
//    int N=data->column*data->row;
//    unsigned char *__data=new unsigned char[N];

//    float min=color_map->min();
//    float max=color_map->max();
//    if(min==0.0 && max==0.0){
//        min=data->min;
//        max=data->max;

//    }

//    for(int i=0;i<N;i++){
//        if( data->val[i] < min ) __data[i]=0;
//        else{
//            if( data->val[i] > max ) __data[i]=255;
//            else
//                __data[i]=255.0*(data->val[i]-min)/(max-min);
//        }
//    }

//    glGenTextures(1, &tex);
//    glActiveTexture(GL_TEXTURE0);
//    //glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,tex);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,data->column,data->row,0,GL_ALPHA,GL_UNSIGNED_BYTE,__data );
//    //glDisable(GL_TEXTURE_2D);
//    delete[] __data;
//}

//void Interpolation::setColorMap(){
//    std::cout<<"Carga de ColorTransferFunction"<<std::endl;
//    //shaderh.textureLink("ColorTransferFunction",color_map->colorTextureEnum);

//    /*
//  glActiveTexture(GL_TEXTURE0+color_map->colorTextureEnum);
//  glEnable(GL_TEXTURE_1D);
//  glBindTexture(GL_TEXTURE_1D,color_map->colorTextureID);
//  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//  glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,color_map->lutColor.size()/3 ,0,GL_RGB,GL_UNSIGNED_BYTE,&(color_map->lutColor[0]) );
//  glDisable(GL_TEXTURE_1D);
//  glBindTexture(GL_TEXTURE_1D, 0);
//  */

//    glGenTextures(1, &texColorMap);

//    glActiveTexture(GL_TEXTURE1);
//    //glEnable(GL_TEXTURE_1D);
//    glBindTexture(GL_TEXTURE_1D,texColorMap);
//    //    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    //    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    //    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    //    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, color_map->lutColor.size()/3, 0, GL_RGB,
//    //     GL_UNSIGNED_BYTE, &(color_map->lutColor[0]));
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,color_map->lutColor.size()/3 ,0,GL_RGB,GL_UNSIGNED_BYTE,&(color_map->lutColor[0]) );
//    //glDisable(GL_TEXTURE_1D);

//    std::cout<<"color_map->lutColor.size()"<<color_map->lutColor.size()<<std::endl;
//    for(int i=0;i<color_map->lutColor.size();i++)
//        std::cout<<(unsigned int)color_map->lutColor[i]<<" ";
//    std::cout<<std::endl;

//    std::cout<<"color_map->lutAlpha.size()"<<color_map->lutAlpha.size()<<std::endl;
//    for(int i=0;i<color_map->lutAlpha.size();i++)
//        std::cout<<(unsigned int)color_map->lutAlpha[i]<<" ";
//    std::cout<<std::endl;

//    glGenTextures(1, &texAlphaMap);
//    glActiveTexture(GL_TEXTURE2);
//    //glEnable(GL_TEXTURE_1D);
//    glBindTexture(GL_TEXTURE_1D,texAlphaMap);
//    //    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    //    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    //    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    //    glTexImage1D(GL_TEXTURE_1D, 0, GL_ALPHA, color_map->lutAlpha.size(), 0, GL_ALPHA,
//    //     GL_UNSIGNED_BYTE, &(color_map->lutAlpha[0]));

//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//LINEAR NEAREST
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexImage1D(GL_TEXTURE_1D,0,GL_ALPHA,color_map->lutAlpha.size(),0,GL_ALPHA,GL_UNSIGNED_BYTE,&(color_map->lutAlpha[0]) );
//    //glDisable(GL_TEXTURE_1D);

//}

