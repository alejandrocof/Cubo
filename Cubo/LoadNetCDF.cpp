#include "LoadNetCDF.hpp"

struct tm  LoadNetCDF::addinterval(struct tm x, int y, int m, int d) {
    cout<<"add interval: "<<x.tm_mday<<"/"<<x.tm_mon<<"/"<<x.tm_year;
    cout<<" "<<x.tm_hour<<":"<<x.tm_min<<":"<<x.tm_sec<<" + "<<d<<"/"<<m<<"/"<<y<<endl;
    x.tm_year += y;
    x.tm_mon += m;
    x.tm_mday += d;
    mktime(&x);
    cout<<"              "<<x.tm_mday<<"/"<<x.tm_mon<<"/"<<x.tm_year<<endl;
    return x;
}

void LoadNetCDF::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    data2D=new DataRaw2D();
    std::cout<<"dir de data2d en LoadNetCDF="<<data2D<<std::endl;

    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("File")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "name", fileName);
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

    if(!fileName.empty()){
        if(load(fileName)){
            setTimeIndex();
        }
    }

}


void LoadNetCDF::setTimeIndex(){
    std::cout<<"Time Index="<<vtime[Squencer::sControl.index]<<std::endl;
    netCDFTime(Squencer::sControl.index);
    struct tm datef;
    datef.tm_hour = 0;   datef.tm_min = 0; datef.tm_sec = 0;

    if(increaseYear){
        cout<<"Year a incrementar"<<endl;
        datef=addinterval(datei,vtime[Squencer::sControl.index],0,0);
    }
    if(increaseMon){
        cout<<"Mon a incrementar"<<endl;
        datef=addinterval(datei,0,vtime[Squencer::sControl.index],0);
    }
    if(increaseDay){
        cout<<"Day a incrementar"<<endl;
        datef=addinterval(datei,0,0,vtime[Squencer::sControl.index]);
    }
    stringstream ss;
    ss<< datef.tm_mday<<"/"<<datef.tm_mon+1<<"/"<<datef.tm_year<<endl;

    cout<< datei.tm_mday<<"/"<<datei.tm_mon+1<<"/"<<datei.tm_year<<" + "<<vtime[Squencer::sControl.index]<<endl;
    cout<< datef.tm_mday<<"/"<<datef.tm_mon+1<<"/"<<datef.tm_year<<endl;
    Squencer::sControl.otherText=ss.str();
}



int LoadNetCDF::netCDFTime(){
  return *iTime;
}

int LoadNetCDF::timeMax(){
  return nTimes;
}

void LoadNetCDF::netCDFTime(int _time){
    *iTime=_time;
    cout<<" Start("<<nTimes<<")=";
    for (auto& d: start)cout<<d<<",";
    cout<<endl;
    varData.getVar( start,count, vdata.data() );
    data2D->min=std::numeric_limits<float>::max();
    data2D->max=-std::numeric_limits<float>::max();
    data2D->wearMask=true;
    for(int j=0,jr=data2D->row-1;j<data2D->row;j++,jr--){
        for(int i=0;i<data2D->column;i++){
            int k=j*data2D->column+i;
            data2D->val[k]=vdata[jr*data2D->column+i];

            //if(data2D->val[k]<9999.01e+20){
            if(data2D->val[k]<9998.0 and data2D->val[k]>-9998.0){
                if(data2D->min > data2D->val[k]) data2D->min = data2D->val[k];
                if(data2D->max < data2D->val[k]) data2D->max = data2D->val[k];
                data2D->mask[k]=255;
            }
            else{
                data2D->mask[k]=0;
            }
        }
    }

}

vector<float> &LoadNetCDF::operator()(){
    //return vdata;
    return data2D->val;
}

bool LoadNetCDF::load(string name){
    cout<<"Abriendo el archivo NetCDF"<<endl;
    dataFile=new NcFile(name, NcFile::read);

    multimap< string,NcVar > vars=dataFile->getVars();

    for (auto&& x: vars){

        NcVar& var=x.second;
        if(var.isNull()){
            cout<<"Error "<<NC_ERR<<": datos invalidos "<< var.getName() <<endl;
            return false;
        }

        vector< NcDim > vDims=var.getDims();

        if(boost::iequals(var.getName(),"lon")){
            if(vDims.size()!=1){
                cout<<"Error en lon, exceso de dimensiones"<<endl;
                return false;
            }
            else{
                vlon.resize(vDims[0].getSize());
                var.getVar( &vlon.front() );
                data2D->column=vDims[0].getSize();
                if(vlon.front()<vlon.back()){
                    Shapes::xmin=data2D->xmin=vlon.front();
                    Shapes::xmax=data2D->xmax=vlon.back();
                }
                else{
                    Shapes::xmin=data2D->xmin=vlon.back();
                    Shapes::xmax=data2D->xmax=vlon.front();
                }
            }
        }
        else
            if(boost::iequals(var.getName(),"lat")){
                if(vDims.size()!=1){
                    cout<<"Error en lat, exceso de dimensiones"<<endl;
                    return false;
                }
                else{
                    vlat.resize(vDims[0].getSize());
                    var.getVar( &vlat.front() );
                    data2D->row=vDims[0].getSize();
                    if(vlat.front()<vlat.back()){
                        Shapes::ymin=data2D->ymin=vlat.front();
                        Shapes::ymax=data2D->ymax=vlat.back();
                    }
                    else{
                        Shapes::ymin=data2D->ymin=vlat.back();
                        Shapes::ymax=data2D->ymax=vlat.front();
                    }

                }
            }
            else
                if(boost::iequals(var.getName(),"time")){
                    if(vDims.size()!=1){
                        cout<<"Error en time, exceso de dimensiones"<<endl;
                        return false;
                    }
                    else{
                        vtime.resize(vDims[0].getSize());
                        var.getVar( &vtime.front() );
                        Squencer::sControl.indexN=vDims[0].getSize()-1;

                        string str;
                        var.getAtt("units").getValues(str);
                        stringstream ss(str);
                        string units,since,date,time;
                        ss>>units>>since>>date>>time;
                        string syear,smonth,sday;
                        string shour,smin,ssec;
                        int year,month,day;
                        int hour,min,sec;
                        stringstream ssDate(date);
                        getline(ssDate, syear, '-');stringstream(syear)>>year;
                        getline(ssDate, smonth, '-');stringstream(smonth)>>month;
                        getline(ssDate, sday, '-');stringstream(sday)>>day;
                        getline(ssDate, shour, ':');stringstream(shour)>>hour;
                        getline(ssDate, smin, ':');stringstream(smin)>>min;
                        getline(ssDate, ssec, ':');stringstream(ssec)>>sec;
                        datei.tm_year = year; datei.tm_mon = month-1; datei.tm_mday = day;
                        cout<<"Unidad: "<<units<<endl;
                        if(units.compare("years")==0)increaseYear=true;
                        if(units.compare("months")==0)increaseMon=true;
                        if(units.compare("days")==0)increaseDay=true;
                    }
                }
                else
                {
                    varData=var;

                    size_t size=1;
                    start.resize(vDims.size());
                    count.resize(vDims.size());
                    for(size_t i=0;i<vDims.size();i++){
                        if(boost::iequals(vDims[i].getName(),"time")){
                            start[i]=0;
                            count[i]=1;
                            iTime=&start[i];
                            nTimes=vDims[i].getSize();
                        }
                        else{
                            start[i]=0;
                            count[i]=vDims[i].getSize();
                            size*=vDims[i].getSize();
                        }
                    }
                    cout<<" Tamaño total "<<size<<endl;
                    data2D->val.resize(size);
                    data2D->mask.resize(size);
                    data2D->xy_linear_o.resize(2*size);
                    data2D->xy_linear_t.resize(2*size);
                    vdata.resize(size);
                    netCDFTime(0);
                }
    }

    if(proj!=0){
        for(int j=0,jr=data2D->row-1;j<data2D->row;j++,jr--){
            for(int i=0;i<data2D->column;i++){
                int kr=jr*data2D->column+i;
                data2D->xy_linear_o[2*kr]=vlon[i];
                data2D->xy_linear_o[2*kr+1]=vlat[j];
//                data2D->xy_linear_t[2*kr]=vlon[i]*DEG_TO_RAD;
//                data2D->xy_linear_t[2*kr+1]=vlat[j]*DEG_TO_RAD;
                data2D->xy_linear_t[2*kr]=vlon[i];
                data2D->xy_linear_t[2*kr+1]=vlat[j];
            }
        }

        proj->transform(data2D->xy_linear_t);

        data2D->xtmin=std::numeric_limits<double>::max();
        data2D->xtmax=-std::numeric_limits<double>::max();
        data2D->ytmin=std::numeric_limits<double>::max();
        data2D->ytmax=-std::numeric_limits<double>::max();
        for(int j=0,jr=data2D->row-1;j<data2D->row;j++,jr--){
            for(int i=0;i<data2D->column;i++){
                int kr=jr*data2D->column+i;
                if(data2D->xtmin > data2D->xy_linear_t[2*kr]) data2D->xtmin = data2D->xy_linear_t[2*kr];
                if(data2D->xtmax < data2D->xy_linear_t[2*kr]) data2D->xtmax = data2D->xy_linear_t[2*kr];
                if(data2D->ytmin > data2D->xy_linear_t[2*kr+1]) data2D->ytmin = data2D->xy_linear_t[2*kr+1];
                if(data2D->ytmax < data2D->xy_linear_t[2*kr+1]) data2D->ytmax = data2D->xy_linear_t[2*kr+1];
            }
        }

        if(Shapes::xtmin>data2D->xtmin)Shapes::xtmin=data2D->xtmin;
        if(Shapes::xtmax<data2D->xtmax)Shapes::xtmax=data2D->xtmax;
        if(Shapes::ytmin>data2D->ytmin)Shapes::ytmin=data2D->ytmin;
        if(Shapes::ytmax<data2D->ytmax)Shapes::ytmax=data2D->ytmax;

//        double xtmiddle=(data2D->xtmin+data2D->xtmax)/2.0;
//        double ytmiddle=(data2D->ytmin+data2D->ytmax)/2.0;
//        double dxt=data2D->xtmax-data2D->xtmin;
//        double dyt=data2D->ytmax-data2D->ytmin;
//        double dt=(dxt>dyt)?dxt/2.0:dyt/2.0;

//        double xttmin=xtmiddle-dt;
//        double xttmax=xtmiddle+dt;
//        double yttmin=ytmiddle-dt;
//        double yttmax=ytmiddle+dt;

//        data2D->xtmin=xttmin;
//        data2D->xtmax=xttmax;
//        data2D->ytmin=yttmin;
//        data2D->ytmax=yttmax;

//        Shapes::xtmin=xttmin;
//        Shapes::xtmax=xttmax;
//        Shapes::ytmin=yttmin;
//        Shapes::ytmax=yttmax;

//        for(int j=0,jr=data2D->row-1;j<data2D->row;j++,jr--){
//            for(int i=0;i<data2D->column;i++){
//                int kr=jr*data2D->column+i;
//                data2D->xyt[2*kr]=(data2D->xyt[2*kr]-data2D->xtmin)/(data2D->xtmax-data2D->xtmin);
//                data2D->xyt[2*kr+1]=(data2D->xyt[2*kr+1]-data2D->ytmin)/(data2D->ytmax-data2D->ytmin);
//            }
//        }

        cout<<"Shapesx("<<Shapes::xtmin<<","<<Shapes::xtmax<<")";
        cout<<"-Shapesy("<<Shapes::ytmin<<","<<Shapes::ytmax<<")"<<endl;

    }

    return true;
}
