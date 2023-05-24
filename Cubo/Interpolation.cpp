#include "Interpolation.hpp"


//float Interpolation::c=0.000001;
//punto interno de un polígono
//https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html

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
                else ck=c;
                break;
            case FnvHash("PlacaDelgada"):
                phi= [](double r,double k){return (r>0.0)?pow(r,k)*log(r):0.0;};
                if(k<0.0)ck=2.0;
                else ck=k;
                break;
            case FnvHash("Potencia"):
                phi= [](double r,double k){return pow(r,k);};
                if(k<0.0)ck=1.0;
                else ck=k;
                break;
            case FnvHash("Gaussiana"):
                phi= [](double r,double c){return exp(-r*r*c*c);};
                if(c<0.0)ck=0.000001;
                else ck=c;
                break;
            case FnvHash("InvMuticuadrica"):
                phi= [](double r,double c){return 1.0/sqrt(r*r+c*c);};
                if(c<0.0)ck=0.000001;
                else ck=c;
                break;
            default:
                cout<<"Función invalida, se asignara multicuádrica"<<endl;
                break;
            }
        }

        if( nodeName.compare("Projection")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "getId", IdProy);
            }
            DrawableIdentifier::data(IdProy,proj);
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
            std::string file;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", p.latitude);
                ifTheAttributeIsFoundAssign(ait, "long", p.longitude);
                ifTheAttributeIsFoundAssign(ait, "name", p.name);
                //ifTheAttributeIsFoundAssign(ait, "value", p.value);
                if(ifTheAttributeIsFoundAssign(ait, "fileIn",file)){
                    readFileApprox(file);
                }
                ifTheAttributeIsFoundAssign(ait, "fileOut", fileNameSaveApprox);
            }
            if( file.empty() )vApprox.push_back(p);
        }
        
        if( nodeName.compare("DataFile")==0 ){
            string name;
            string stype;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "type", stype);
                ifTheAttributeIsFoundAssign(ait, "name", name);
                ifTheAttributeIsFoundAssign(ait, "nVar", nVar);
                ifTheAttributeIsFoundAssign(ait, "cellsize", cellsize);
                ifTheAttributeIsFoundAssign(ait, "average", averaging_i);
                ifTheAttributeIsFoundAssign(ait, "positionDependence", positionDependence);
                ifTheAttributeIsFoundAssign(ait, "mask", cellsizemask);
                ifTheAttributeIsFoundAssign(ait, "scale", scale);


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
                readFileRegularGrid(name,scale);//nueva
                break;
                
            case FnvHash("scattered"):
                cout<<"*nVar="<<nVar<<endl;
                typeOfData=1;
                readFileScattered(name,scale);
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

                ifTheAttributeIsFoundAssign(ait, "mask", mask);

                ifTheAttributeIsFoundAssign(ait, "column", column);
                ifTheAttributeIsFoundAssign(ait, "row", row);
                ifTheAttributeIsFoundAssign(ait, "average",averaging_o );//submalla de tamaño averagingxaveraging de donde se obtiene los valores para promediar
                ifTheAttributeIsFoundAssignString(ait, "setID", outputDataID);
                if(ifTheAttributeIsFoundAssign(ait, "fileNameRG", fileNameSaveRG)){
                    save=true;
                }
                if(ifTheAttributeIsFoundAssign(ait, "fileNameSaveScatt", fileNameSaveScatt)){
                    save=true;
                }

                if(ifTheAttributeIsFoundAssign(ait, "threshold_mask_min",threshold_mask_min)){
                    b_th_msk_min=true;
                }
                if(ifTheAttributeIsFoundAssign(ait, "threshold_mask_max",threshold_mask_max)){
                    b_th_msk_max=true;
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


    if(typeOfData==1)//nuevo scattered
        buildLambda();
    
    cout<<"nData="<<nData<<" nX="<<nX<<" nXP="<<nXP<<" nXRG="<<nXRG<<endl;
    if(calculate){
//        nV.resize(nXP);
//        for(int l=0;l<nXP;l++){
//            nV[l].resize(nData);
//            for(int i=0;i<nData; ++i){
//                vector< double > node(nX);
//                for(int k=0;k<nX;k++)
//                    node[k]=nTuple[k][i];
//                nV[l][i]=vInterpolation[l]->interpolate(node);
//                double v=vInterpolation[l]->nTmin[vInterpolation[l]->cVar]+nV[l][i]*(vInterpolation[l]->nTmax[vInterpolation[l]->cVar]-vInterpolation[l]->nTmin[vInterpolation[l]->cVar]);
//                cout<<"V["<<l<<"]("<<nTuple[0][i]<<","<<nTuple[1][i]<<")="<<nV[l][i]<<" -> "<<v<<endl;
//            }
//        }

//        nVRG.resize(nXRG);
//        for(int l=0;l<nXRG;l++){
//            nVRG[l].resize(nData);
//            for(int i=0;i<nData; ++i){
//                vector< double > node(nX);
//                for(int k=0;k<nX;k++)
//                    node[k]=nTuple[k][i];
//                nVRG[l][i]=interpolateRG(l,node);
//                double v=vInterpolationRG[l]->data2D->min+nVRG[l][i]*(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min);
//                cout<<"VRG["<<l<<"]("<<nTuple[0][i]<<","<<nTuple[1][i]<<")="<<nVRG[l][i]<<" -> "<<v<<endl;
//            }
//        }

//        vPointsCovexHull.resize(nData);
//        for(int j=0;j<nData;j++){
//            vPointsCovexHull[j].longitude=_x[0][j];
//            vPointsCovexHull[j].latitude=_x[1][j];
//        }
//        convex_hull(vPointsCovexHull);

        data=new DataRaw2D();

        if(!outputDataID.empty()){
            DrawableIdentifier::insert(outputDataID,data);
        }

        data->column=column;
        data->row=row;
        data->val.resize(data->column*data->row);
        data->mask.resize(data->column*data->row);
        data->xy_linear_o.resize(2*data->column*data->row);
        data->xy_linear_t.resize(2*data->column*data->row);
        data->xy_nearest_o.resize(2*(data->column+1)*(data->row+1));
        data->xy_nearest_t.resize(2*(data->column+1)*(data->row+1));
        data->xmin=xmin;
        data->ymin=ymin;
        data->xmax=xmax;
        data->ymax=ymax;
        data->name=outputDataID;

        vector< double > node(2);
        vector< double > subNode(2);

        data->xtmin=std::numeric_limits<double>::max();
        data->xtmax=-std::numeric_limits<double>::max();
        data->ytmin=std::numeric_limits<double>::max();
        data->ytmax=-std::numeric_limits<double>::max();

        for(int j=0;j<=data->row;j++){
            for(int i=0;i<=data->column;i++){
                int k=j*(data->column+1)+i;
                node[0]=data->xmin+(data->xmax-data->xmin)*i/data->column;
                node[1]=data->ymin+(data->ymax-data->ymin)*j/data->row;

                data->xy_nearest_o[2*k]=node[0];
                data->xy_nearest_o[2*k+1]=node[1];
//                node[0]*=DEG_TO_RAD;
//                node[1]*=DEG_TO_RAD;
                proj->transform(node);
                data->xy_nearest_t[2*k]=node[0];
                data->xy_nearest_t[2*k+1]=node[1];
            }
        }

        point_interpola.resize(nData);
        for(int j=0;j<data->row;j++){
            //double ly=(double)j/(data->row-1);
            for(int i=0;i<data->column;i++){
                //double lx=(double)i/(data->column-1);
                //node[0]=plong0+lx*dlong10+ly*dlong30+lx*ly*dlong0123;
                //node[1]=plat0+lx*dlat10+ly*dlat30+lx*ly*dlat0123;
                //data->val[j*data->column+i]=nTmin[nX]+interpolate(node)*(nTmax[nX]-nTmin[nX]);
                node[0]=data->xmin+(data->xmax-data->xmin)*(i+0.5)/data->column;
                node[1]=data->ymin+(data->ymax-data->ymin)*(j+0.5)/data->row;
                //cout<<"node("<<i<<","<<j<<")=coord("<<node[0]<<","<<node[1]<<")"<<endl;

                int k=j*data->column+i;
                if(averaging_o==1){
                    //cout<<"data="<<nTmin[nX]<<endl;

                    //data->val[k]=nTmin[cVar]+interpolate(node)*(nTmax[cVar]-nTmin[cVar]);
                    data->val[k]=interpolateCoordinate(node);

                    //data->val[k]=interpolate(node);
                    // cout<<data->val[j*data->column+i]<<endl;
                    //data->val[j*data->column+i]=nTmin[nX];
                }
                else{
                    data->val[k]=0.0;
                    for(int jAverage=0;jAverage<averaging_o;jAverage++){
                        float ja=((float)jAverage+0.5)/averaging_o;
                        subNode[1]=data->ymin+(data->ymax-data->ymin)*(j+ja)/data->row;
                        for(int iAverage=0;iAverage<averaging_o;iAverage++){
                            float ia=((float)iAverage+0.5)/averaging_o;
                            subNode[0]=data->xmin+(data->xmax-data->xmin)*(i+ia)/data->column;

                            //double v=nTmin[cVar]+interpolate(subNode)*(nTmax[cVar]-nTmin[cVar]);
                            double v=interpolateCoordinate(subNode);

                            data->val[k]+=v;
                            //data->val[k]+=nTmin[nX]+interpolate(subNode)*(nTmax[nX]-nTmin[nX]);
                            //data->val[k]+=interpolate(subNode);
                            //cout<<"  subnode("<<iAverage<<","<<jAverage<<") coord("<<subNode[0]<<","<<subNode[1]<<") val="<<v<<endl;
                            //cout<<subNode[0]<<" "<<subNode[1]<<" "<<v<<endl;
                        }
                    }
                    data->val[k]/=(averaging_o*averaging_o);
                    //cout<<"val="<<data->val[k]<<endl;
                    //data->val[j*data->column+i]=(6.79e-6)*pow(data->val[j*data->column+i],1.6);
                    //6.79 x 10^-6  x Sa^1.6 ; ;
                }

                if(mask){
                    if( pnpoly(node[0],node[1]) )
                        data->mask[k]=255;
                    else
                        data->mask[k]=0;
                }
                else{
                    data->mask[k]=255;
                }
                if(b_th_msk_min && data->val[k]<threshold_mask_min)
                    data->mask[k]=0;

                if(b_th_msk_max && data->val[k]<threshold_mask_max)
                    data->mask[k]=0;

                bool toDraw=false;
                for(int ll=0;ll<nData; ++ll){
                    double x=nTmin[0]+nTuple[0][ll]*(nTmax[0]-nTmin[0]);
                    double y=nTmin[1]+nTuple[1][ll]*(nTmax[1]-nTmin[1]);
                    double l=x-0.5*cellsize;
                    double r=x+0.5*cellsize;
                    double d=y-0.5*cellsize;
                    double u=y+0.5*cellsize;
                    //cout<<l<<"<"<<node[0]<<"<"<<r<<"   "<<d<<"<"<<node[1]<<"<"<<u<<endl;
                    if( l<node[0] && node[0]<r && d<node[1] && node[1]<u ){
                        pointi temp;
                        temp.longitude=node[0];
                        temp.latitude=node[1];
                        temp.value=data->val[k];
                        point_interpola[ll].push_back(temp);
                        toDraw=true;
                        break;
                    }
                }

                if(cellsizemask){
                    if(toDraw){
                        data->mask[k]=255;
                    }
                    else
                        data->mask[k]=0;
                }


                if( data->max < data->val[k] )data->max = data->val[k];
                if( data->min > data->val[k] )data->min = data->val[k];

                data->xy_linear_o[2*k]=node[0];
                data->xy_linear_o[2*k+1]=node[1];
//                node[0]*=DEG_TO_RAD;
//                node[1]*=DEG_TO_RAD;
                proj->transform(node);
                data->xy_linear_t[2*k]=node[0];
                data->xy_linear_t[2*k+1]=node[1];
                if(data->xtmin > data->xy_linear_t[2*k]) data->xtmin = data->xy_linear_t[2*k];
                if(data->xtmax < data->xy_linear_t[2*k]) data->xtmax = data->xy_linear_t[2*k];
                if(data->ytmin > data->xy_linear_t[2*k+1]) data->ytmin = data->xy_linear_t[2*k+1];
                if(data->ytmax < data->xy_linear_t[2*k+1]) data->ytmax = data->xy_linear_t[2*k+1];

            }
        }


        cout<<"mínimos y máximos"<<endl;
        for(int l=0;l<nXP;l++){
            //vP[l]=0.5;
            //vP[l]=(vInterpolation[l]->vmin+vInterpolation[l]->vmax)/2.0;
            cout<<vInterpolation[l]->vmin<<" "<<vInterpolation[l]->vmax<<endl;
        }

        for(int l=0;l<nXRG;l++){
            //vP[nXP+l]=0.5;
            //vP[nXP+l]=(vInterpolationRG[l]->data2D->min+vInterpolationRG[l]->data2D->max)/2.0;
            cout<<fixed<<setprecision(8)<<vInterpolationRG[l]->data2D->min<<" "<<vInterpolationRG[l]->data2D->max<<endl;
        }
        cout<<endl;

        for(int l=0;l<nXP;l++)
            cout<<"nXP "<<l<<" min="<<nV_min[l]<<" max="<<nV_max[l]<<endl;
        for(int l=0;l<nXRG;l++)
            cout<<"nXRG "<<l<<" min="<<nVRG_min[l]<<" max="<<nVRG_max[l]<<" -> "<<vInterpolationRG[l]->data2D->min<<" "<<vInterpolationRG[l]->data2D->max<<endl;

        bool guarda_datos_funcion_interpolacion=false;
        if(guarda_datos_funcion_interpolacion){
            if(positionDependence){
                std::ofstream fileSave;//(fileName.c_str(),std::ios::out);
                fileSave.open("salidaInterp.txt",std::ios::out);
                vector<double> vP( nX+nXP+nXRG );
                int N=3;
                unsigned long int pow_N_n=pow(N,nX+nXP+nXRG);
                vector<int> index(nX+nXP+nXRG,0);
                if (fileSave.is_open()){
                    for(int i=0;i<pow_N_n;i++){
                        for(int l=0;l<nX;l++){
                            vP[l]=nTmin[l]+double(index[l])*(nTmax[l]-nTmin[l])/double(N-1);
                            fileSave<<fixed<<setprecision(8)<<vP[l]<<"\t";
                        }
                        for(int l=0;l<nXP;l++){
                            vP[nX+l]=vInterpolation[l]->vmin+double(index[nX+l])*(vInterpolation[l]->vmax-vInterpolation[l]->vmin)/double(N-1);
                            fileSave<<fixed<<setprecision(8)<<vP[nX+l]<<"\t";
                        }
                        for(int l=0;l<nXRG;l++){
                            vP[nX+nXP+l]=vInterpolationRG[l]->data2D->min+double(index[nX+nXP+l])*(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min)/double(N-1);
                            fileSave<<fixed<<setprecision(8)<<vP[nX+nXP+l]<<"\t";
                        }
                        fileSave<<fixed<<setprecision(8)<<interpolateParameter(vP)<<endl;
                        index[0]++;
                        for(int l=0;l<nX+nXP+nXRG-1;l++){
                            if(index[l]>=N){
                                //fileSave<<endl;
                                index[l]=0;
                                index[l+1]++;
                            }
                        }
                    }
                }
            }
            else{
                vector<double> vP( nXP+nXRG );
    //            for(int l=0;l<nXP;l++){
    //                vP[l]=vInterpolation[l]
    //                        vInterpolation[l]->vmin+double(index[l])*(vInterpolation[l]->vmax-vInterpolation[l]->vmin)/double(NN[l]-1);
    //                fileSave<<fixed<<setprecision(8)<<vP[l]<<"\t";
    //            }
    //            for(int l=0;l<nXRG;l++){
    //                vP[nXP+l]=vInterpolationRG[l]->data2D->min+double(index[nXP+l])*(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min)/double(NN[nXP+l]-1);
    //                fileSave<<fixed<<setprecision(8)<<vP[nXP+l]<<"\t";
    //            }
                for(int i=0;i<nData; ++i){
            //        cout<<fixed<<setprecision(8)<<nTuple[0][i];
            //        for(int k=1;k<nX;k++)
            //            cout<<"\t "<<fixed<<setprecision(8)<<nTuple[k][i];
                    cout<<fixed<<setprecision(8)<<nTmin[0]+nTuple[0][i]*(nTmax[0]-nTmin[0]);
                    cout<<"\t"<<fixed<<setprecision(8)<<nTmin[1]+nTuple[1][i]*(nTmax[1]-nTmin[1]);
                    for(int l=0;l<nXP;l++){
                        vP[l]=nV_min[l]+nV[l][i]*(nV_max[l]-nV_min[l]);
                        cout<<"\t "<<fixed<<setprecision(8)<<vP[l];

                    }
                        //cout<<"\t "<<fixed<<setprecision(8)<<vInterpolation[l]->vmin+nV[l][i]*(vInterpolation[l]->vmax-vInterpolation[l]->vmin);
                    for(int l=0;l<nXRG;l++){
                        vP[nXP+l]=nVRG_min[l]+nVRG[l][i]*(nVRG_max[l]-nVRG_min[l]);
                        cout<<"\t "<<fixed<<setprecision(8)<<vP[nXP+l];
                        //cout<<"\t "<<fixed<<setprecision(8)<<vInterpolationRG[l]->data2D->min+nVRG[l][i]*(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min);
                    }
                    cout<<"\t "<<fixed<<setprecision(8)<<vmin+nTuple[cVar][i]*(vmax-vmin);
                    cout<<"\t "<<fixed<<interpolateParameter(vP)<<endl;

                    cout<<"*";
                    for(auto &val:vP){
                        cout<<fixed<<setprecision(8)<<val<<"\t";
                    }
                    cout<<fixed<<setprecision(8)<<interpolateParameter(vP)<<endl;

                }

                int id=11;
                std::ofstream fileSave;//(fileName.c_str(),std::ios::out);
                stringstream namefile;
                if(id==0)
                    namefile<<"salidaInterp.txt";
                else
                    namefile<<"salidaInterp_id"<<setfill('0')<<setw(2)<<id<<".txt";

                //fileSave.open("salidaInterp.txt",std::ios::out);
                fileSave.open(namefile.str(),std::ios::out);

                //int N=10;
                int nM=1;
                vector<int> NN(nXP+nXRG,nM);//nM=3
                int NNAll=1;
                if(NN.size()>2){
                    NN[0]=50;
                    NN[1]=50;
                }
                for(int l=0;l<NN.size();l++){
                    NNAll*=NN[l];
                }
                //unsigned long int pow_N_n=pow(N,nXP+nXRG);
                vector<int> index(nXP+nXRG,0);
                if (fileSave.is_open()){
                    for(int i=0;i<NNAll;i++){
                        for(int l=0;l<nXP;l++){
                            vP[l]=vInterpolation[l]->vmin+double(index[l])*(vInterpolation[l]->vmax-vInterpolation[l]->vmin)/double(NN[l]-1);
                            fileSave<<fixed<<setprecision(8)<<vP[l]<<"\t";
                        }
                        for(int l=0;l<nXRG;l++){
                            vP[nXP+l]=vInterpolationRG[l]->data2D->min+double(index[nXP+l])*(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min)/double(NN[nXP+l]-1);
                            if(id!=0){
    //                            if(l<2)
    //                                vP[nXP+l]=nVRG_min[l]+nVRG[l][id-1]*(nVRG_max[l]-nVRG_min[l])-0.001+0.002*double(index[nXP+l])/double(NN[nXP+l]-1);
                                if(l>=2)
                                    vP[nXP+l]=nVRG_min[l]+nVRG[l][id-1]*(nVRG_max[l]-nVRG_min[l]);
                            }
                            fileSave<<fixed<<setprecision(8)<<vP[nXP+l]<<"\t";
                        }
                        fileSave<<fixed<<setprecision(8)<<interpolateParameter(vP)<<endl;
                        index[0]++;
                        for(int l=0;l<index.size()-1;l++){
                            if(index[l]>=NN[l]){
                                index[l]=0;
                                index[l+1]++;
                            }
                        }
                    }
                }
            }
        }




//        int N=10;
//        vector<double> vP( nXP+nXRG );
//        vInterpolation[l]->vmin;
//        vector<double> vP(nXP+nXRG);

//        for(int l=0;l<nXP;l++)
//            vP[l]=vInterpolation[l]->vmin;
//        for(int l=0;l<nXRG;l++)
//            vP[nXP+l]=vInterpolationRG[l]->data2D->min;

//        for(int i=0;i<N;i++){
//            vP[]
//        }

//        for(int j=0;j<data->row;j++){
//            for(int i=0;i<data->column;i++){
//                int k=j*data->column+i;
//                if(k<10)
//                    data->val[k]=data->max;
//            }
//        }

        cout<<"Interpolación"<<endl;
        cout<<"Mínimo= "<<data->min<<endl;
        cout<<"Máximo= "<<data->max<<endl;

        if(save){
            std::ofstream fileSave;//(fileName.c_str(),std::ios::out);
            if(!fileNameSaveRG.empty()){
                fileSave.open(fileNameSaveRG.c_str(),std::ios::out);
                if (fileSave.is_open()){
                    cout<<"Guardando interpolación en el archivo RG:"<<fileNameSaveRG<<endl;
                    fileSave<<"ncols "<<data->column<<endl;
                    fileSave<<"nrows "<<data->row<<endl;
                    fileSave<<"xllcorner "<<xmin<<endl;
                    fileSave<<"yllcorner "<<ymin<<endl;
                    fileSave<<"xcellsize "<<(xmax-xmin)/(data->column-1)<<endl;//cambiar
                    fileSave<<"ycellsize "<<(ymax-ymin)/(data->row-1)<<endl;
                    for(int j=data->row-1;j>=0;j--){
                        for(int i=0;i<data->column-1;i++){
                            fileSave<<data->val[j*data->column+i]<<" ";
                        }
                        fileSave<<data->val[j*data->column+data->column-1]<<endl;
                    }
                }
                else{
                    cout<<"No se pudo crear el archivo RG: "<<fileNameSaveRG<<endl;
                    save=false;
                }
            }
            if(!fileNameSaveScatt.empty()){
                fileSave.open(fileNameSaveScatt.c_str(),std::ios::out);
                if (fileSave.is_open()){
                    cout<<"Guardando interpolación en el archivo Scattered:"<<fileNameSaveScatt<<endl;

//                    for(int j=data->row-1;j>=0;j--){
//                        for(int i=0;i<data->column;i++){
//                            int k=j*data->column+i;
//                            //if(data->mask[k]==255){
//                                fileSave<<fixed<<setprecision(8)<<data->xy_linear_o[2*k]<<" ";
//                                fileSave<<fixed<<setprecision(8)<<data->xy_linear_o[2*k+1]<<" ";
//                                fileSave<<fixed<<setprecision(8)<<data->val[j*data->column+i]<<endl;
//                            //}
//                        }
//                    }

                    for(int j=data->row-1;j>=0;j--){
                        for(int i=0;i<data->column-1;i++){
                            fileSave<<fixed<<setprecision(8)<<data->val[j*data->column+i]<<" ";
                        }
                        fileSave<<fixed<<setprecision(8)<<data->val[j*data->column+data->column-1]<<endl;
                    }
                }
                else{
                    cout<<"No se pudo crear el archivo Scattered: "<<fileNameSaveScatt<<endl;
                    save=false;
                }
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
        if(fileNameSaveApprox.empty()){
            for (auto &nApprox : vApprox){
                node[0]=nApprox.longitude;
                node[1]=nApprox.latitude;
                //cout<<nApprox.name<<" "<<node[0]<<" "<<node[1]<<" "<<nTmin[cVar]+interpolate(node)*(nTmax[cVar]-nTmin[cVar])<<endl;
                cout<<nApprox.name<<" "<<node[0]<<" "<<node[1]<<" "<<interpolateCoordinate(node)<<endl;
            }
        }
        else{
            ofstream outFile;
            outFile.open(fileNameSaveApprox.c_str(),std::ios::out);
            cout<<"Generando archivo:"<<fileNameSaveApprox<<endl;
            if (!outFile.fail())
            {
                cout<<"Guardando aproximaciones en el archivo:"<<fileNameSaveApprox<<endl;
                for (auto &nApprox : vApprox){
                    node[0]=nApprox.longitude;
                    node[1]=nApprox.latitude;
                    //outFile.precision(8);
                    outFile<<fixed<<setprecision(8)<<node[0]<<" ";
                    outFile<<fixed<<setprecision(8)<<node[1]<<" ";
                    //outFile<<fixed<<setprecision(8)<<nTmin[cVar]+interpolate(node)*(nTmax[cVar]-nTmin[cVar])<<endl;
                    outFile<<fixed<<setprecision(8)<<interpolateCoordinate(node)<<endl;
                }
            }
            else{
                cout<<"No se pudo crear el archivo de aprox: "<<fileNameSaveApprox<<endl;
            }
        }

        ofstream outFileInt;
        outFileInt.open("SalidaInterpolacionCeldas.txt",std::ios::out);
        cout<<"Generando archivo:SalidaInterpolacionCeldas.txt"<<endl;
        cout<<"c="<<fixed<<setprecision(6)<<ck<<endl;
        if (!outFileInt.fail())
        {
            for(int ll=0;ll<nData; ++ll){
                double rango_min=std::numeric_limits<double>::max();
                double rango_max=-std::numeric_limits<double>::max();
                double sum=0.0;
                for(int k=0;k<point_interpola[ll].size();k++){
                    outFileInt<<fixed<<setprecision(8)<<point_interpola[ll][k].longitude<<"\t";
                    outFileInt<<fixed<<setprecision(8)<<point_interpola[ll][k].latitude<<"\t";
                    outFileInt<<fixed<<setprecision(8)<<point_interpola[ll][k].value<<endl;
                    sum+=point_interpola[ll][k].value;
                    rango_min=(rango_min<point_interpola[ll][k].value)?rango_min:point_interpola[ll][k].value;
                    rango_max=(rango_max>point_interpola[ll][k].value)?rango_max:point_interpola[ll][k].value;
                }
                //outFileInt<<endl;
                cout<<fixed<<setprecision(8)<<sum/point_interpola[ll].size()<<"\t"<<rango_min<<"\t"<<rango_max<<"\t"<<fabs(rango_max-rango_min)<<endl;
            }
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

    nXP=vInterpolation.size();
    nXRG=vInterpolationRG.size();

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

    //nXP=vInterpolation.size();
    //nXRG=vInterpolationRG.size();
    //VectorXf lambda(nData+nX+1);
    //lambda.push_back( VectorXf(nData+nX+1) );


    //G.setZero();
    //b.setZero();

    nV.resize(nXP);nV_min.resize(nXP);nV_max.resize(nXP);
    nVRG.resize(nXRG);nVRG_min.resize(nXRG);nVRG_max.resize(nXRG);
    for(int l=0;l<nXP;l++){
        nV[l].resize(nData);
        nV_min[l]=std::numeric_limits<double>::max();
        nV_max[l]=-std::numeric_limits<double>::max();
    }
    for(int l=0;l<nXRG;l++){
        nVRG[l].resize(nData);
        nVRG_min[l]=std::numeric_limits<double>::max();
        nVRG_max[l]=-std::numeric_limits<double>::max();
    }

    if(averaging_i==1){
        for(int i=0;i<nData; ++i){
            vector< double > node(nX);
//            for(int k=0;k<nX;k++)
//                node[k]=nTuple[k][i];
            for(int iVar=0;iVar<nX;iVar++)
                node[iVar]=nTmin[iVar]+nTuple[iVar][i]*(nTmax[iVar]-nTmin[iVar]);
            for(int l=0;l<nXP;l++){
                nV[l][i]=vInterpolation[l]->interpolateCoordinate(node);
                nV_min[l]=(nV_min[l]<nV[l][i])?nV_min[l]:nV[l][i];
                nV_max[l]=(nV_max[l]>nV[l][i])?nV_max[l]:nV[l][i];
            }
            for(int l=0;l<nXRG;l++){
                nVRG[l][i]=interpolateRG(l,node);
                nVRG_min[l]=(nVRG_min[l]<nVRG[l][i])?nVRG_min[l]:nVRG[l][i];
                nVRG_max[l]=(nVRG_max[l]>nVRG[l][i])?nVRG_max[l]:nVRG[l][i];
            }
        }
    }
    else{
        for(int i=0;i<nData; ++i){
            vector< double > node(nX);
            vector< double > subNode(nX);
//            for(int k=0;k<nX;k++)
//                node[k]=nTuple[k][i];
            node[0]=nTmin[0]+nTuple[0][i]*(nTmax[0]-nTmin[0]);
            node[1]=nTmin[1]+nTuple[1][i]*(nTmax[1]-nTmin[1]);
            double yi=(node[1]-0.5*cellsize);
            double yf=(node[1]+0.5*cellsize);
            double xi=(node[0]-0.5*cellsize);
            double xf=(node[0]+0.5*cellsize);
            for(int l=0;l<nXP;l++)
                nV[l][i]=0.0;
            for(int l=0;l<nXRG;l++)
                nVRG[l][i]=0.0;
            //int averaging=80;
            for(int jAverage=0;jAverage<averaging_i;jAverage++){
                subNode[1]=yi+(yf-yi)*(jAverage+0.5)/averaging_i;
                for(int iAverage=0;iAverage<averaging_i;iAverage++){
                    subNode[0]=xi+(xf-xi)*(iAverage+0.5)/averaging_i;
                    for(int l=0;l<nXP;l++)
                        nV[l][i]+=vInterpolation[l]->interpolateCoordinate(subNode);
                    for(int l=0;l<nXRG;l++)
                        nVRG[l][i]+=interpolateRG(l,subNode);
                }
            }
            for(int l=0;l<nXP;l++){
                nV[l][i]/=(averaging_i*averaging_i);
                nV_min[l]=(nV_min[l]<nV[l][i])?nV_min[l]:nV[l][i];
                nV_max[l]=(nV_max[l]>nV[l][i])?nV_max[l]:nV[l][i];
            }
            for(int l=0;l<nXRG;l++){
                nVRG[l][i]/=(averaging_i*averaging_i);
                nVRG_min[l]=(nVRG_min[l]<nVRG[l][i])?nVRG_min[l]:nVRG[l][i];
                nVRG_max[l]=(nVRG_max[l]>nVRG[l][i])?nVRG_max[l]:nVRG[l][i];
            }
        }
    }

    //Normaliza
    for(int i=0;i<nData; ++i){
        //nVRG[l][i]=(interpolateRG(l,node)-vInterpolationRG[l]->data2D->min)/(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min);//
        for(int l=0;l<nXP;l++)
            nV[l][i]=(nV[l][i]-nV_min[l])/(nV_max[l]-nV_min[l]);//
        for(int l=0;l<nXRG;l++)
            nVRG[l][i]=(nVRG[l][i]-nVRG_min[l])/(nVRG_max[l]-nVRG_min[l]);//
    }

    for(int i=0;i<nData; ++i){
//        cout<<fixed<<setprecision(8)<<nTuple[0][i];
//        for(int k=1;k<nX;k++)
//            cout<<"\t "<<fixed<<setprecision(8)<<nTuple[k][i];
        cout<<fixed<<setprecision(8)<<nTmin[0]+nTuple[0][i]*(nTmax[0]-nTmin[0]);
        cout<<"\t"<<fixed<<setprecision(8)<<nTmin[1]+nTuple[1][i]*(nTmax[1]-nTmin[1]);
        for(int l=0;l<nXP;l++)
            cout<<"\t "<<fixed<<setprecision(8)<<nV_min[l]+nV[l][i]*(nV_max[l]-nV_min[l]);
            //cout<<"\t "<<fixed<<setprecision(8)<<vInterpolation[l]->vmin+nV[l][i]*(vInterpolation[l]->vmax-vInterpolation[l]->vmin);
        for(int l=0;l<nXRG;l++)
            cout<<"\t "<<fixed<<setprecision(8)<<nVRG_min[l]+nVRG[l][i]*(nVRG_max[l]-nVRG_min[l]);
            //cout<<"\t "<<fixed<<setprecision(8)<<vInterpolationRG[l]->data2D->min+nVRG[l][i]*(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min);
        cout<<"\t "<<fixed<<setprecision(8)<<vmin+nTuple[cVar][i]*(vmax-vmin)<<endl;
    }

//        0.5*cellsize;
//        for(int l=0;l<nXP;l++)
//            nV[l][i]=vInterpolation[l]->interpolate(node);
//        for(int l=0;l<nXRG;l++)
//            nVRG[l][i]=interpolateRG(l,node);
//        //double v=vInterpolation[l]->nTmin[vInterpolation[l]->cVar]+nV[l][i]*(vInterpolation[l]->nTmax[vInterpolation[l]->cVar]-vInterpolation[l]->nTmin[vInterpolation[l]->cVar]);
//        //cout<<"V["<<l<<"]("<<nTuple[0][i]<<","<<nTuple[1][i]<<")="<<nV[l][i]<<" -> "<<v<<endl;
//    }


    MatrixXd G;
    VectorXd b;

    if(positionDependence){
        G.resize(nData+nX+nXP+nXRG+1,nData+nX+nXP+nXRG+1);
        b.resize(nData+nX+nXP+nXRG+1);
        lambda.resize(nData+nX+nXP+nXRG+1);
        cout<<"llenando Gij"<<endl;
        for(int i=0; i<nData; ++i){
            b(i)=nTuple[cVar][i];
            G(i,i)=phi(0.0,ck);
            for(int j=i+1; j<nData; ++j){
                double rr=0.0;
                for(int k=0;k<nX;k++)
                    rr+=pow( nTuple[k][i] - nTuple[k][j] , 2);
                for(int l=0;l<nXP;l++)
                    rr+=pow( nV[l][i] - nV[l][j] , 2);
                for(int l=0;l<nXRG;l++)
                    rr+=pow( nVRG[l][i] - nVRG[l][j] , 2);
                double r=sqrt( rr );
                G(i,j)=G(j,i)=phi(r,ck);
            }
        }
        cout<<"llenando GNij"<<endl;
        for(int i=0; i<nData; ++i){
            for(int k=0;k<nX;k++)
                G(nData+k,i)=G(i,nData+k)=nTuple[k][i];
            for(int l=0;l<nXP;l++)
                G(nData+nX+l,i)=G(i,nData+nX+l)=nV[l][i];
            for(int l=0;l<nXRG;l++)
                G(nData+nX+nXP+l,i)=G(i,nData+nX+nXP+l)=nVRG[l][i];
            G(nData+nX+nXP+nXRG,i)=G(i,nData+nX+nXP+nXRG)= 1.0;
        }

        for(int i=0; i<nX+nXP+nXRG+1; ++i){
            b(nData+i)=0.0;
            for(int j=0; j<nX+nXP+nXRG+1; ++j)
                G(nData+i,nData+j)=0.0;
        }
    }
    else{
        G.resize(nData+nXP+nXRG+1,nData+nXP+nXRG+1);
        b.resize(nData+nXP+nXRG+1);
        lambda.resize(nData+nXP+nXRG+1);
        cout<<"llenando Gij"<<endl;
        for(int i=0; i<nData; ++i){
            b(i)=nTuple[cVar][i];
            G(i,i)=phi(0.0,ck);
            for(int j=i+1; j<nData; ++j){
                double rr=0.0;
                for(int l=0;l<nXP;l++)
                    rr+=pow( nV[l][i] - nV[l][j] , 2);
                for(int l=0;l<nXRG;l++)
                    rr+=pow( nVRG[l][i] - nVRG[l][j] , 2);
                double r=sqrt( rr );
                G(i,j)=G(j,i)=phi(r,ck);
            }
        }
        cout<<"llenando GNij"<<endl;
        for(int i=0; i<nData; ++i){
            for(int l=0;l<nXP;l++)
                G(nData+l,i)=G(i,nData+l)=nV[l][i];
            for(int l=0;l<nXRG;l++)
                G(nData+nXP+l,i)=G(i,nData+nXP+l)=nVRG[l][i];
            G(nData+nXP+nXRG,i)=G(i,nData+nXP+nXRG)= 1.0;
        }

        for(int i=0; i<nXP+nXRG+1; ++i){
            b(nData+i)=0.0;
            for(int j=0; j<nXP+nXRG+1; ++j)
                G(nData+i,nData+j)=0.0;
        }
    }





    //cout<<"b="<<b<<endl;
    //cout<<"G="<<G<<endl;
    cout<<"resolviendo sistema"<<endl;

    lambda = G.colPivHouseholderQr().solve(b);
    //lambda = G.householderQr().solve(b);
    cout<<"lambda="<<lambda<<endl;
    cout<<"calculando error relativo"<<endl;
    double relative_error = (G*lambda - b).norm() / b.norm(); // norm() is L2 norm
    cout <<std::scientific<<std::setprecision(8)<< "error relativo = " << relative_error << " "<<endl;

    double err,emax=0.0;

    if(positionDependence){
        for(int i=0; i<nData; ++i){
            double u_aprox=0;
            for(int j=0; j<nData; ++j){
                double rr=0.0;
                for(int k=0;k<nX;k++)
                    rr+=pow( nTuple[k][i] - nTuple[k][j] , 2);
                for(int l=0;l<nXP;l++)
                    rr+=pow( nV[l][i] - nV[l][j] , 2);
                for(int l=0;l<nXRG;l++)
                    rr+=pow( nVRG[l][i] - nVRG[l][j] , 2);
                double r=sqrt( rr );
                u_aprox+=phi(r,ck)*lambda(j);
            }
            for(int k=0;k<nX;k++)
                u_aprox+=lambda(nData+k)*nTuple[k][i];
            for(int l=0;l<nXP;l++)
                u_aprox+=lambda(nData+nX+l)*nV[l][i];
            for(int l=0;l<nXRG;l++)
                u_aprox+=lambda(nData+nX+nXP+l)*nVRG[l][i];
            u_aprox+=lambda(nData+nX+nXP+nXRG);

            err = fabs((u_aprox-nTuple[cVar][i])*(nTmax[cVar]-nTmin[cVar]));
            if(i==0)cout<<"id RAW Inter"<<endl;
            cout<<i<<fixed<<setprecision(8)<<" "<<nTmin[cVar]+nTuple[cVar][i]*(nTmax[cVar]-nTmin[cVar])<<" "<<nTmin[cVar]+u_aprox*(nTmax[cVar]-nTmin[cVar])<<endl;
            //cout<<"x="<<x[indexY][i]<<" xaprox="<<u_aprox<<" err="<<err<<endl;
           // cout<<"f="<<nTuple[indexY][i]<<" f_aprox="<<u_aprox<<" err="<<err<<endl;
            emax = err>emax? err : emax;
        }
    }
    else{
        for(int i=0; i<nData; ++i){
            double u_aprox=0;
            for(int j=0; j<nData; ++j){
                double rr=0.0;
                for(int l=0;l<nXP;l++)
                    rr+=pow( nV[l][i] - nV[l][j] , 2);
                for(int l=0;l<nXRG;l++)
                    rr+=pow( nVRG[l][i] - nVRG[l][j] , 2);
                double r=sqrt( rr );
                u_aprox+=phi(r,ck)*lambda(j);
            }
            for(int l=0;l<nXP;l++)
                u_aprox+=lambda(nData+l)*nV[l][i];
            for(int l=0;l<nXRG;l++)
                u_aprox+=lambda(nData+nXP+l)*nVRG[l][i];
            u_aprox+=lambda(nData+nXP+nXRG);

            err = fabs((u_aprox-nTuple[cVar][i])*(nTmax[cVar]-nTmin[cVar]));
            if(i==0)cout<<"id RAW Inter"<<endl;
            cout<<i<<fixed<<setprecision(8)<<" "<<nTmin[cVar]+nTuple[cVar][i]*(nTmax[cVar]-nTmin[cVar])<<" "<<nTmin[cVar]+u_aprox*(nTmax[cVar]-nTmin[cVar])<<endl;
            //cout<<"x="<<x[indexY][i]<<" xaprox="<<u_aprox<<" err="<<err<<endl;
           // cout<<"f="<<nTuple[indexY][i]<<" f_aprox="<<u_aprox<<" err="<<err<<endl;
            emax = err>emax? err : emax;
        }
    }

    std::cout<<std::fixed<<"error máximo="<<emax<<std::endl;
    return 0;
}



int Interpolation::readFileRegularGrid(string fileName, double scale){
    std::ifstream myfile(fileName.c_str(),std::ios::in);

    rg *datarg=new rg;
    int nodata_value;
    int column,row;

    nX=2;
    cVar=2;

    nTmin.resize(3);
    nTmax.resize(3);

    if (myfile.is_open()){
        cout<<"Abriendo archivo asc:"<<fileName<<endl;
        //int iLine=1;
        std::string line;
        //vector <string> fields;
        string field,val;

        getline(myfile,line);
        if(readLine(line,column,"ncols")==0)return 0;

        getline(myfile,line);
        if(readLine(line,row,"nrows")==0)return 0;

        getline(myfile,line);
        if(readLine(line,datarg->xi,"xllcorner")==0)return 0;

        getline(myfile,line);
        if(readLine(line,datarg->yi,"yllcorner")==0)return 0;

        getline(myfile,line);
        int k=readLine(line,datarg->xcellsize,"cellsize","xcellsize");
        if(k==0)return 0;

        getline(myfile,line);
        if(k==1){
            datarg->ycellsize=datarg->xcellsize;
            if(readLine(line,nodata_value,"nodata_value")==0)return 0;
        }
        else{
            if(k==2){
                if(readLine(line,datarg->ycellsize,"ycellsize")==0)return 0;
            }
        }



//        stringstream sline=stringstream(line);
//        //split( fields, line, is_any_of( " " ) );
//        //stringstream(fields[1])>>column;
//        getline(sline, field,' ');
//        if(field!="ncols"){std::cout<<"Error en la lectura del archivo, se esperaba ncols y se encontro:"<<field<<std::endl;return 0;}
//        getline(sline, val,' ');
//        stringstream(val)>>column;
//        std::cout<<"line="<<sline.str()<<" field="<<field<<" val="<<val<<" -> "<<column<<std::endl;

//        getline(myfile,line);
//        //split( fields, line, is_any_of( " " ) );
//        //stringstream(fields[1])>>row;
//        getline(stringstream(line), field,' ');
//        stringstream(field)>>row;
//        std::cout<<"line "<<line<<" field "<<field<<" -> "<<row<<std::endl;

//        getline(myfile,line);
//        //split( fields, line, is_any_of( " " ) );
//        //stringstream(fields[1])>>datarg->xi;
//        getline(stringstream(line), field,' ');
//        stringstream(field)>>datarg->xi;
//        std::cout<<"line "<<line<<" field "<<field<<" -> "<<datarg->xi<<std::endl;

//        getline(myfile,line);
//        //split( fields, line, is_any_of( " " ) );
//        //stringstream(fields[1])>>datarg->yi;
//        getline(stringstream(line), field,' ');
//        stringstream(field)>>datarg->yi;
//        std::cout<<"line "<<line<<" field "<<field<<" -> "<<datarg->yi<<std::endl;

//        getline(myfile,line);
//        //split( fields, line, is_any_of( " " ) );
//        //stringstream(fields[1])>>datarg->cellsize;
//        getline(stringstream(line), field,' ');
//        stringstream(field)>>datarg->cellsize;
//        std::cout<<"line "<<line<<" field "<<field<<" -> "<<datarg->cellsize<<std::endl;

//        getline(myfile,line);
//        //split( fields, line, is_any_of( " " ) );
//        //stringstream(fields[1])>>nodata_value;
//        getline(stringstream(line), field,' ');
//        stringstream(field)>>nodata_value;
//        std::cout<<"line "<<line<<" field "<<field<<" -> "<<nodata_value<<std::endl;

        cout<<"ncols:"<<column<<" nrow:"<<row;
        cout<<" xllcorner:"<<datarg->xi<<" yllcorner:"<<datarg->yi;
        cout<<" cellsize:"<<datarg->xcellsize<<"x"<<datarg->ycellsize<<endl;

        datarg->data2D=new DataRaw2D;
        datarg->data2D->column=column;
        datarg->data2D->row=row-1;
        datarg->xf = datarg->xi + datarg->xcellsize*datarg->data2D->column;
        datarg->yf = datarg->yi + datarg->ycellsize*datarg->data2D->row;

        datarg->data2D->val.resize(datarg->data2D->column*datarg->data2D->row);
        double h;
        int i=0,j=datarg->data2D->row-1;
        for(unsigned int k=0;k<datarg->data2D->val.size();k++){
            myfile>>h;
            datarg->data2D->val[j*datarg->data2D->column+i]=h*scale;
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
        nTmin[2]=datarg->data2D->min; nTmax[2]=datarg->data2D->max;
        cout<<"RegularGrid:"<<endl;
        cout<<"\tx("<<datarg->xi<<","<<datarg->xf<<")"<<endl;
        cout<<"\ty("<<datarg->yi<<","<<datarg->yf<<")"<<endl;
        cout<<"\tv("<<datarg->data2D->min<<","<<datarg->data2D->max<<")"<<endl;
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
//        int i=di;
//        int j=dj;
//        if(i < 0)i=0;
//        if(j < 0)j=0;
//        if(i > vInterpolationRG[l]->data2D->column-1) i = vInterpolationRG[l]->data2D->column-1;
//        if(j > vInterpolationRG[l]->data2D->row-1) j = vInterpolationRG[l]->data2D->row-1;
//return (vInterpolationRG[l]->data2D->val[j*vInterpolationRG[l]->data2D->column+i]-vInterpolationRG[l]->data2D->min)/(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min);
        //        if(kk<100){
        //            cout<<node[0]<<" "<<node[1];
        //            cout<<" ("<< vInterpolationRG[l]->xi<<","<<vInterpolationRG[l]->xf<<")";
        //            cout<<" ("<< vInterpolationRG[l]->yi<<","<<vInterpolationRG[l]->yf<<")";
        //            cout<<" di="<<di<<" dj="<<dj<<" i="<<i<<" j="<<j<<" y=";
        //            cout<<vInterpolationRG[l]->data2D->val[j*vInterpolationRG[l]->data2D->column+i]<<" ";
        //            cout<<(vInterpolationRG[l]->data2D->val[j*vInterpolationRG[l]->data2D->column+i]-vInterpolationRG[l]->data2D->min)/(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min)<<endl;
        //        }
        //        kk++;

        int i=floor(di);
        int j=floor(dj);
        di-=(double)i;
        dj-=(double)j;
        if(i < 0)i=0;
        if(j < 0)j=0;

        int i1=i;
        int i2=i+1;
        int j1=j;
        int j2=j+1;
        if(i1 > vInterpolationRG[l]->data2D->column-1) i1=vInterpolationRG[l]->data2D->column-1;
        if(i2 > vInterpolationRG[l]->data2D->column-1) i2=vInterpolationRG[l]->data2D->column-1;
        if(j1 > vInterpolationRG[l]->data2D->row-1) j1=vInterpolationRG[l]->data2D->row-1;
        if(j2 > vInterpolationRG[l]->data2D->row-1) j2=vInterpolationRG[l]->data2D->row-1;
        double a=vInterpolationRG[l]->data2D->val[j1*vInterpolationRG[l]->data2D->column+i1];
        double b=vInterpolationRG[l]->data2D->val[j1*vInterpolationRG[l]->data2D->column+i2];
        double c=vInterpolationRG[l]->data2D->val[j2*vInterpolationRG[l]->data2D->column+i1];
        double d=vInterpolationRG[l]->data2D->val[j2*vInterpolationRG[l]->data2D->column+i2];
        double ab=a+di*(b-a);
        double cd=c+di*(d-c);
        double v=ab+dj*(cd-ab);
        //return (v-vInterpolationRG[l]->data2D->min)/(vInterpolationRG[l]->data2D->max-vInterpolationRG[l]->data2D->min);
        return v;
        //vInterpolationRG[l]->data2D->val[j*vInterpolationRG[l]->data2D->column+i]

    }
    else{
        cout<<"tamaño de nodo incorrecto en interpolación"<<endl;
        return 0.0;
    }


}

int Interpolation::readFileScattered(string fileName, double scale){
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
                _xtemp[nVar-1]*=scale;

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
        vmin=nTmin[nVar-1];
        vmax=nTmax[nVar-1];
        cout<<nVar-1<<" min="<<nTmin[nVar-1]<<" max="<<nTmax[nVar-1]<<endl;

        nTuple.resize(nVar);
        /*for(int i=0;i<nVar;i++) {
            nTuple[i]=VectorXf::Map(_x[i].data(), _x[i].size());
        }*/

        nData=_x[0].size();
        nX=nVar-1;
        cVar=nVar-1;

        cout<<"nData="<<nData<<endl;
        cout<<"nVar="<<nVar<<endl;
        cout<<"nX="<<nX<<endl;
        vPointsCovexHull.resize(nData);
        for(int j=0;j<nData;j++){
            vPointsCovexHull[j].longitude=_x[0][j];
            vPointsCovexHull[j].latitude=_x[1][j];
        }


        vStrokeCovexHull=convex_hull(vPointsCovexHull);

//        Stroke
//        stroke2D=new Stroke2D();
//        stroke2D->setColor(255,0,0,255);
//        stroke2D->setWidth(0.1);
//        cout<<"Proyection:"<<IdProy<<endl;
//        stroke2D->projection(IdProy);//aqui el error!
//        //cout<<"vStrokeCovexHull.size()"<<vStrokeCovexHull.size()<<endl;
//        for(int j=0;j<vStrokeCovexHull.size();j++){
//            stroke2D->setPoint(vStrokeCovexHull[j].longitude,vStrokeCovexHull[j].latitude);
//        }
//        stroke2D->make();
//        Shapes::push_back_3D( &Stroke2D::draw , stroke2D );

//        cout<<"Original"<<endl;
//        for(int j=0;j<nData;j++){
//            cout<<_x[nVar-1][j]<<endl;
//        }

        for(int iVar=0;iVar<nVar;iVar++){
            nTuple[iVar].resize(nData);
            double dmaxmin=nTmax[iVar]-nTmin[iVar];
//            if(iVar==cVar){
//                threshold_min=(threshold_min-nTmin[iVar])/dmaxmin;
//                threshold_max=(threshold_max-nTmin[iVar])/dmaxmin;
//            }
            for(int j=0;j<nData;j++){
                //nTuple[iVar][j]=_x[iVar][j];
                //if(iVar==cVar)nTuple[iVar][j]=(_x[iVar][j]-nTmin[iVar])/dmaxmin;
                nTuple[iVar][j]=(_x[iVar][j]-nTmin[iVar])/dmaxmin;
                //if(iVar==nVar-1)nTuple[iVar][j]=_x[iVar][j];
            }
        }
//        cout<<"Normalizada"<<endl;
//        for(int j=0;j<nData;j++){
//            cout<<nTuple[nVar-1][j]<<endl;
//        }



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


double Interpolation::interpolateCoordinate(vector< double > &node ){

    if(typeOfData==0){
        return interpolateRG(0,node);
    }
    double f=0;
    vector<double> vP(nXP+nXRG);
    for(int l=0;l<nXP;l++)
        vP[l]=(vInterpolation[l]->interpolateCoordinate(node)-nV_min[l])/(nV_max[l]-nV_min[l]);
    for(int l=0;l<nXRG;l++)
        vP[nXP+l]=(interpolateRG(l,node)-nVRG_min[l])/(nVRG_max[l]-nVRG_min[l]);
    if(positionDependence){
        vector<double> x(nX);
        for(int iVar=0;iVar<nX;iVar++)
            x[iVar]=(node[iVar]-nTmin[iVar])/(nTmax[iVar]-nTmin[iVar]);
        for(int j=0; j<nData; ++j){
            double rr=0.0;
            for(int iVar=0;iVar<nX;iVar++)
                rr+=(x[iVar] - nTuple[iVar][j])*(x[iVar] - nTuple[iVar][j]);
            for(int l=0;l<nXP;l++)
                rr+=(vP[l] - nV[l][j])*(vP[l] - nV[l][j]);
            for(int l=0;l<nXRG;l++)
                rr+=(vP[nXP+l] - nVRG[l][j])*(vP[nXP+l] - nVRG[l][j]);
            double r=sqrt( rr );
            f+=phi(r,ck)*lambda(j);
        }
        for(int iVar=0;iVar<nX;iVar++)
            f+=lambda(nData+iVar)*x[iVar];
        //f+=lambda(nData+iVar)*node[iVar];
        for(int l=0;l<nXP;l++)
            f+=lambda(nData+nX+l)*vP[l];
        for(int l=0;l<nXRG;l++)
            f+=lambda(nData+nX+nXP+l)*vP[nXP+l];
        f+=lambda(nData+nX+nXP+nXRG);
    }
    else{
        for(int j=0; j<nData; ++j){
            double rr=0.0;
            for(int l=0;l<nXP;l++)
                rr+=(vP[l] - nV[l][j])*(vP[l] - nV[l][j]);
            for(int l=0;l<nXRG;l++)
                rr+=(vP[nXP+l] - nVRG[l][j])*(vP[nXP+l] - nVRG[l][j]);
            double r=sqrt( rr );
            f+=phi(r,ck)*lambda(j);
        }
        for(int l=0;l<nXP;l++)
            f+=lambda(nData+l)*vP[l];
        for(int l=0;l<nXRG;l++)
            f+=lambda(nData+nXP+l)*vP[nXP+l];
        f+=lambda(nData+nXP+nXRG);
    }
    f=nTmin[cVar]+f*(nTmax[cVar]-nTmin[cVar]);
    if(b_th_min && f<=threshold_min)return threshold_min;
    if(b_th_max && f>=threshold_max)return threshold_max;
    return f;
}

double Interpolation::interpolateParameter(vector<double> &vP_temp ){




    double f=0;

    if(positionDependence){
        vector<double> x(nX);
        vector<double> vP( nXP+nXRG );
        for(int iVar=0;iVar<nX;iVar++)
            x[iVar]=(vP_temp[iVar]-nTmin[iVar])/(nTmax[iVar]-nTmin[iVar]);
        for(int l=0;l<nXP;l++)
            vP[l]=(vP_temp[nX+l]-nV_min[l])/(nV_max[l]-nV_min[l]);
        for(int l=0;l<nXRG;l++)
            vP[nXP+l]=(vP_temp[nX+nXP+l]-nVRG_min[l])/(nVRG_max[l]-nVRG_min[l]);

        for(int j=0; j<nData; ++j){
            double rr=0.0;
            for(int iVar=0;iVar<nX;iVar++)
                rr+=(x[iVar] - nTuple[iVar][j])*(x[iVar] - nTuple[iVar][j]);
            for(int l=0;l<nXP;l++)
                rr+=(vP[l] - nV[l][j])*(vP[l] - nV[l][j]);
            for(int l=0;l<nXRG;l++)
                rr+=(vP[nXP+l] - nVRG[l][j])*(vP[nXP+l] - nVRG[l][j]);
            double r=sqrt( rr );
            f+=phi(r,ck)*lambda(j);
        }
        for(int iVar=0;iVar<nX;iVar++)
            f+=lambda(nData+iVar)*x[iVar];
        //f+=lambda(nData+iVar)*node[iVar];
        for(int l=0;l<nXP;l++)
            f+=lambda(nData+nX+l)*vP[l];
        for(int l=0;l<nXRG;l++)
            f+=lambda(nData+nX+nXP+l)*vP[nXP+l];
        f+=lambda(nData+nX+nXP+nXRG);
    }
    else{
        vector<double> vP( nXP+nXRG );
        for(int l=0;l<nXP;l++)
            vP[l]=(vP_temp[l]-nV_min[l])/(nV_max[l]-nV_min[l]);
        for(int l=0;l<nXRG;l++)
            vP[nXP+l]=(vP_temp[nXP+l]-nVRG_min[l])/(nVRG_max[l]-nVRG_min[l]);
        for(int j=0; j<nData; ++j){
            double rr=0.0;
            for(int l=0;l<nXP;l++)
                rr+=(vP[l] - nV[l][j])*(vP[l] - nV[l][j]);
            for(int l=0;l<nXRG;l++)
                rr+=(vP[nXP+l] - nVRG[l][j])*(vP[nXP+l] - nVRG[l][j]);
            double r=sqrt( rr );
            f+=phi(r,ck)*lambda(j);
        }
        for(int l=0;l<nXP;l++)
            f+=lambda(nData+l)*vP[l];
        for(int l=0;l<nXRG;l++)
            f+=lambda(nData+nXP+l)*vP[nXP+l];
        f+=lambda(nData+nXP+nXRG);
    }
    f=nTmin[cVar]+f*(nTmax[cVar]-nTmin[cVar]);
    if(b_th_min && f<=threshold_min)return threshold_min;
    if(b_th_max && f>=threshold_max)return threshold_max;
    return f;
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




int Interpolation::readFileApprox(string fileName){

    std::ifstream fileLines(fileName.c_str(),std::ios::in);

    if (fileLines.is_open()){
        cout<<"Abriendo archivo de datos a aproximar:"<<fileName<<endl;
        std::string sline;
        std::stringstream ssline;
        int iLine=0;
        float _long,_lat;

        while ( getline(fileLines,sline)){
            if(!sline.empty()){
                std::stringstream ssline(sline);
                if( !(ssline>>_long>>_lat) ){
                    cout<<endl<<"Error en la línea "<<iLine<<" del archivo de aproximación:"<<endl;
                    cout<<fileName<<endl;
                    cout<<"Se esperaban leer por lo menos 2 valores"<<endl;
                    cout<<sline<<endl;
                    return 0;
                }
                point p;
                p.longitude=_long;
                p.latitude=_lat;
                vApprox.push_back(p);
            }
            iLine++;
        }


    }
    else
        cout<<"No se pudo leer el archivo de datos para aproximación: "<<fileName<<endl;


    return 0;
}
