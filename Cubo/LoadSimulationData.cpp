#include "LoadSimulationData.hpp"

void LoadSimulationData::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    data2D=new DataRaw2D();
    data3D=new DataRaw3D();
    std::cout<<"dir de data2d en LoadSimulationData="<<data2D<<std::endl;

    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("MinMaxValues")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "min", data3D->minAxis);
                ifTheAttributeIsFoundAssign(ait, "max", data3D->maxAxis);
            }
            useMinimumMaximum=false;
        }

        if( nodeName.compare("FileNameFormat")==0 ){
            std::string fileNP;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "locationFiles", locationFiles);
                if( ifTheAttributeIsFoundAssign(ait, "filenamePrefix", fileNP) ){
                    filenamePrefix.push_back(fileNP);
                }
                ifTheAttributeIsFoundAssign(ait, "numberDigitsSubdomainIdentifier", numberDigitsSubdomainIdentifier);
                ifTheAttributeIsFoundAssign(ait, "numberDigitsTimeIndex", numberDigitsTimeIndex);
            }
            //setFileNameFormat(locationFiles,filenamePrefix,ndsi,ndti);
        }

        if( nodeName.compare("TimeIndexParameters")==0 ){
            int initialTimeIndex;
            int endTimeIndex;
            int increaseTimeIndex;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "initialTimeIndex", initialTimeIndex);
                ifTheAttributeIsFoundAssign(ait, "endTimeIndex", endTimeIndex);
                ifTheAttributeIsFoundAssign(ait, "increaseTimeIndex", increaseTimeIndex);
            }
            setTimeIndexParameters(initialTimeIndex,endTimeIndex,increaseTimeIndex);
        }

        if( nodeName.compare("NumberOfSubdomains")==0 ){
            int SubdomainsX,SubdomainsY,SubdomainsZ;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "SubdomainsX", SubdomainsX);
                ifTheAttributeIsFoundAssign(ait, "SubdomainsY", SubdomainsY);
                ifTheAttributeIsFoundAssign(ait, "SubdomainsZ", SubdomainsZ);
            }
            setTheNumberOfSubdomains(SubdomainsX,SubdomainsY,SubdomainsZ);
        }

        if( nodeName.compare("DataTransformation")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "Scale", Scale);
            }
        }

        if( nodeName.compare("DomainTransformation")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "Scale", Scale);
            }
        }

        if( nodeName.compare("ExtractData")==0 ){
            Point p;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "lat", p.latitude);
                ifTheAttributeIsFoundAssign(ait, "long",p.longitude);
                ifTheAttributeIsFoundAssign(ait, "z", p.z);
                ifTheAttributeIsFoundAssign(ait, "i", p.i);
                ifTheAttributeIsFoundAssign(ait, "j", p.j);
                ifTheAttributeIsFoundAssign(ait, "k", p.k);
                ifTheAttributeIsFoundAssign(ait, "show", p.show);

                if( ifTheAttributeIsFoundAssign(ait, "min", p.minAxis)){
                    useMinimumMaximumCell=false;
                }
                if( ifTheAttributeIsFoundAssign(ait, "max", p.maxAxis)){
                    useMinimumMaximumCell=false;
                }
            }
            data3D->registry.push_back(p);
            data3D->registry[data3D->registry.size()-1].y.resize(Squencer::sControl.indexN);
        }

        if( nodeName.compare("SubdomainIdentifierAndSize")==0 ){
            int Id,Sx,Sy,Sz;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "Id", Id);
                ifTheAttributeIsFoundAssign(ait, "Sx", Sx);
                ifTheAttributeIsFoundAssign(ait, "Sy", Sy);
                ifTheAttributeIsFoundAssign(ait, "Sz", Sz);
            }
            pushSubdomainIdentifierAndSize(Id,Sx,Sy,Sz);
        }

        std::cout <<"/>"<< std::endl;
    }


//    cout<<"Llenando acelerogramas"<<endl;
//    for(int _index=0;_index<Squencer::sControl.indexN;_index++){
//    //for(int _index=0;_index<10;_index++){
//        int _timeIndex=initialTimeIndex+_index*increaseTimeIndex;

////        unsigned int subdomainSizeX=od[subX][subY][subZ].Nx;
////        unsigned int subdomainSizeY=od[subX][subY][subZ].Ny;
////        unsigned int subdomainSizeZ=od[subX][subY][subZ].Nz;
////        unsigned int startSubdomainX=od[subX][subY][subZ].Sx;
////        unsigned int startSubdomainY=od[subX][subY][subZ].Sy;
////        unsigned int startSubdomainZ=od[subX][subY][subZ].Sz;

//        for(subX=0;subX<numberSubdomainsX;subX++){
//            for(subY=0;subY<numberSubdomainsY;subY++){
//                for(subZ=0;subZ<numberSubdomainsZ;subZ++){
//                    loadSubdomain(_timeIndex);
//                }
//            }
//        }

//        for (auto it = data3D->registry.begin(); it!=data3D->registry.end(); ++it) {

//            int h = it->k*domainSizeX*domainSizeY + it->j*domainSizeX + it->i;
//            it->y[_index] = data3D->val[h];
//            if(it->min > data3D->val[h]) it->min = data3D->val[h];
//            if(it->max < data3D->val[h]) it->max = data3D->val[h];

//            if(useMinimumMaximumCell){
//                it->minAxis=it->min;
//                it->maxAxis=it->max;
//            }
//            //std::cout<<"("<<it->i<<","<<it->j<<","<<it->k<<") min="<<it->min<<" max="<<it->max<<std::endl;
//            if( it->show ) data3D->val[h] = 255;
//        }
//        if( _index%100==0 )
//            cout<<100.0*_index/Squencer::sControl.indexN<<"% completado"<<endl;
//    }
//    cout<<"Acelerogramas completados"<<endl;

    setTimeIndex();


}


void LoadSimulationData::setTheNumberOfSubdomains(
        unsigned int _numberSubdomainsX,
        unsigned int _numberSubdomainsY,
        unsigned int _numberSubdomainsZ){
    numberSubdomainsX=_numberSubdomainsX;
    numberSubdomainsY=_numberSubdomainsY;
    numberSubdomainsZ=_numberSubdomainsZ;
    od.resize(_numberSubdomainsX,std::vector<std::vector<organizationData> >
              (_numberSubdomainsY,std::vector<organizationData>
               (_numberSubdomainsZ)));
}

bool LoadSimulationData::pushSubdomainIdentifierAndSize(
        unsigned int subdomainId,
        unsigned int subdomainSizeX,
        unsigned int subdomainSizeY,
        unsigned int subdomainSizeZ){
    bool correctFilling=true;
    static unsigned int i=0;
    static unsigned int j=0;
    static unsigned int k=0;
    static unsigned int subdomainStartX=0;
    static unsigned int subdomainStartY=0;
    static unsigned int subdomainStartZ=0;



    od[i][j][k].id=subdomainId;
    od[i][j][k].Nx=subdomainSizeX;
    od[i][j][k].Ny=subdomainSizeY;
    od[i][j][k].Nz=subdomainSizeZ;
    od[i][j][k].Sx=subdomainStartX;
    od[i][j][k].Sy=subdomainStartY;
    od[i][j][k].Sz=subdomainStartZ;

    unsigned int bufferSize=subdomainSizeX*subdomainSizeY*subdomainSizeZ+2*subdomainSizeZ;
    if(maximumBufferSize<bufferSize)
        maximumBufferSize=bufferSize;

    std::cout<<subdomainId<<" "
            <<i<<"x"<<j<<"x"<<k<<" "
           <<subdomainSizeX<<"x"
          <<subdomainSizeY<<"x"
         <<subdomainSizeZ<<" "
        <<subdomainStartX<<"x"
       <<subdomainStartY<<"x"
      <<subdomainStartZ;
    k++;
    subdomainStartZ+=subdomainSizeZ;
    if( k==numberSubdomainsZ ){
        k=0;j++;
        if( domainSizeZ!=0 && domainSizeZ!=subdomainStartZ)correctFilling=false;
        domainSizeZ=subdomainStartZ;
        subdomainStartZ=0;
        subdomainStartY+=subdomainSizeY;
        if( j==numberSubdomainsY ){
            j=0;i++;
            if( domainSizeY!=0 && domainSizeY!=subdomainStartY)correctFilling=false;
            domainSizeY=subdomainStartY;
            subdomainStartY=0;
            subdomainStartX+=subdomainSizeX;
            if( i==numberSubdomainsX ){
                domainSizeX=subdomainStartX;
                completedFilling=true;
                std::cout<<std::endl<<"Definition of subdomain sizes completed"<<std::endl;
                std::cout<<numberSubdomainsX<<"x"<<numberSubdomainsY<<"x"<<numberSubdomainsZ<<std::endl;
                std::cout<<domainSizeX<<"x"<<domainSizeY<<"x"<<domainSizeZ;
                dataRaw.resize(domainSizeX*domainSizeY*domainSizeZ);
                dataMax.resize(domainSizeX*domainSizeY*domainSizeZ);
                buffer.resize(filenamePrefix.size());
                for (auto it = buffer.begin(); it!=buffer.end(); ++it) {
                    it->resize(maximumBufferSize);
                }
                data2D->column=domainSizeX;
                data2D->row=domainSizeY;
                data2D->val.resize(domainSizeX*domainSizeY);

                data3D->Nx=domainSizeX;
                data3D->Ny=domainSizeY;
                data3D->Nz=domainSizeZ;
                data3D->val.resize(domainSizeX*domainSizeY*domainSizeZ);

                //for (auto registry : data3D->registry)
                //  registry.y.resize();
            }
        }
    }
    return correctFilling;
}

bool LoadSimulationData::loadSubdomain(int _timeIndex){
    unsigned int subdomainSizeX=od[subX][subY][subZ].Nx;
    unsigned int subdomainSizeY=od[subX][subY][subZ].Ny;
    unsigned int subdomainSizeZ=od[subX][subY][subZ].Nz;
    unsigned int startSubdomainX=od[subX][subY][subZ].Sx;
    unsigned int startSubdomainY=od[subX][subY][subZ].Sy;
    unsigned int startSubdomainZ=od[subX][subY][subZ].Sz;

    std::vector<std::stringstream> nameFile( filenamePrefix.size() );

    //std::cout<<"filenamePrefix "<<filenamePrefix.size()<<" "<< filenamePrefix[0].c_str() <<std::endl;

    for (unsigned int i=0;i<nameFile.size();i++) {
        nameFile[i]<<locationFiles.c_str()<<filenamePrefix[i].c_str()
                  <<std::setw( numberDigitsSubdomainIdentifier )
                 << std::setfill( '0' )
                 << od[subX][subY][subZ].id
                 << std::setw( numberDigitsTimeIndex )
                 << std::setfill( '0' )
                 << _timeIndex;
        //std::cout<<"Opening: "<<nameFile[i].str()<<std::endl;

        std::ifstream file(nameFile[i].str().c_str(),std::ios::binary|std::ios::in);
        if( file.fail() ){
            std::cout<<" ERROR: "<<nameFile[i].str().c_str()<<std::endl;
            std::cout<<"Can not open file"<<std::endl;
            return false;
        }

        std::filebuf* pbuf = file.rdbuf();
        std::size_t size = pbuf->pubseekoff (0,file.end,file.in);
        //std::cout<<"Buffer"<<std::endl;

        if(size!=4*subdomainSizeX*subdomainSizeY*subdomainSizeZ+8*subdomainSizeZ){
            std::cout<<" ERROR: "<<nameFile[i].str().c_str()<<std::endl;
            std::cout<<"File size does not match"<<std::endl;
            return false;
        }


        pbuf->pubseekpos (0,file.in);
        pbuf->sgetn ((char*)&buffer[i][0],size);
        pbuf->close();
        //std::cout<<"Close buffer"<<std::endl;
        file.close();
        //std::cout<<"Close File"<<std::endl;
    }


    /*  std::cout<<"  "<<filenamePrefix.c_str()
          <<std::setw( numberDigitsSubdomainIdentifier )
          << std::setfill( '0' )
          << od[subX][subY][subZ].id
          << std::setw( numberDigitsTimeIndex )
          << std::setfill( '0' )
          << timeIndex<<std::endl;
*/


    int l=0;
    for( int k = subdomainSizeZ-1; k>=0 ; --k ){
        int kk=domainSizeX*domainSizeY*(startSubdomainZ+k);
        l++;
        for( unsigned int j = 0; j < subdomainSizeY; ++j ){
            int jj=domainSizeX*(startSubdomainY+j);
            for( unsigned int i = 0; i < subdomainSizeX; ++i ){
                unsigned int ii=startSubdomainX+i;
                unsigned int h=ii+jj+kk;
                //int h=domainSizeX*domainSizeY*(startSubdomainZ+k)+domainSizeX*(startSubdomainY+j)+(startSubdomainX+i);

                if(buffer.size()>1){
                    data3D->val[h]=0.0;
                    for (auto it = buffer.begin(); it!=buffer.end(); ++it) {
                        data3D->val[h]+= (it->operator[](l))*(it->operator[](l));//[l]);
                    }
                    data3D->val[h]=Scale*sqrt(data3D->val[h]);
                }
                else{
                    data3D->val[h]=Scale*buffer[0][l];
                }
                //dataRaw[h]=Scale*buffer[][l];
                //data3D->val[h]=Scale*buffer[][l];
                /*
        if(od[subX][subY][subZ].id==23)data3D->val[h]=23.0;
        if(od[subX][subY][subZ].id==27)data3D->val[h]=27.0;
        if(od[subX][subY][subZ].id==39)data3D->val[h]=39.0;
        if(od[subX][subY][subZ].id==43)data3D->val[h]=43.0;
        */
                if( data3D->min > data3D->val[h] ) data3D->min = data3D->val[h];
                if( data3D->max < data3D->val[h] ) data3D->max = data3D->val[h];

                //std::cout<<startSubdomainX+i<<"x"<<startSubdomainY+j<<"="<<std::fixed<< std::setprecision(8)<<dataRaw[h]<<", ";

                if( dataMax[h] < fabs(dataRaw[h]) ){
                    dataMax[h] = fabs(dataRaw[h]);
                }
                //std::cout<<"P("<<i<<", "<<j<<", "<<k<<")="<<dataRaw[h]<<std::endl;



                //data[h]=128;
                //data[h]=std::max(0.0f, std::min((dataRaw[l++]-min)*c, 255.0f));

                /*
        if(od[subX][subY][subZ].id==7){
          //if(pp==0)
          //if(pp<subdomainSizeX)
          //if(pp<subdomainSizeX*subdomainSizeY)
          //if(pp<subdomainSizeX*subdomainSizeY*subdomainSizeZ/2)
          //if(pp<subdomainSizeX*subdomainSizeY*subdomainSizeZ/2)//<subdomainSizeX*subdomainSizeY*subdomainSizeZ/2
            data[h]=255;
          //else
          //  data[h]=255;
        }*/
                //pp++;
                l++;
            }
        }
        l++;
    }

    /*
  //std::cout<<"Domain size="<<domainSizeX<<"x"<<domainSizeY<<std::endl;
  float __max=-100000.0,__min=100000.0;
  for( int j = 0; j < domainSizeY; ++j )
    for( int i = 0; i < domainSizeX; ++i ){
      int index=j*domainSizeX+i;
      //data2D->val[index]=dataRaw[domainSizeX*domainSizeY*domainSizeZ/2+index];
      //data2D->val[index]=dataRaw[index];
      data2D->val[index]=dataRaw[index];
      
      //std::cout<<i<<"x"<<j<<"="<<data2D->val[index]<<", ";
      if(data2D->val[index]<__min)__min=data2D->val[index];
      if(data2D->val[index]>__max)__max=data2D->val[index];
      //if(i<domainSizeX/2)data2D->val[index]=10.0;
    }
  //std::cout<<std::fixed<< std::setprecision(4)<<"min,max="<<__min<<","<<__max<<std::endl;
  */



    return true;
}

/*
void LoadSimulationData::setFileNameFormat(
  std::string _locationFiles,
  std::string _filenamePrefix,
  unsigned int _numberDigitsSubdomainIdentifier,
  unsigned int _numberDigitsTimeIndex){
  locationFiles=_locationFiles;
  filenamePrefix=_filenamePrefix;
  numberDigitsSubdomainIdentifier=_numberDigitsSubdomainIdentifier;
  numberDigitsTimeIndex=_numberDigitsTimeIndex;
}
*/

void LoadSimulationData::setTimeIndexParameters(
        unsigned int _initialTimeIndex,
        unsigned int _endTimeIndex,
        unsigned int _increaseTimeIndex){
    initialTimeIndex=_initialTimeIndex;
    endTimeIndex=_endTimeIndex;
    increaseTimeIndex=_increaseTimeIndex;
    timeIndex=_initialTimeIndex;
}

void LoadSimulationData::setTimeSecondsParameters(
        float _initialTimeSeconds,
        float _increaseTimeSeconds){
    initialTimeSeconds=_initialTimeSeconds;
    increaseTimeSeconds=_increaseTimeSeconds;
}

void LoadSimulationData::increaseTimeAndUpdateData(){
    unsigned int _timeIndex=timeIndex+increaseTimeIndex;
    if(_timeIndex>endTimeIndex)
        _timeIndex=initialTimeIndex;
    //setTimeIndex(_timeIndex);
}

//void LoadSimulationData::loadDataAtTheTimeIndex(unsigned int _timeindex)
//void LoadSimulationData::setTimeIndex(unsigned int _timeIndex){
void LoadSimulationData::setTimeIndex(){
    //unsigned int initialTimeIndex;
    //    unsigned int endTimeIndex;
    //    unsigned int increaseTimeIndex;
    timeIndex=initialTimeIndex+Squencer::sControl.index*increaseTimeIndex;
    //timeIndex=_timeIndex;
    std::cout<<"Time Index="<<timeIndex<<std::endl;

    //data3D->min=std::numeric_limits<float>::max();
    //data3D->max=-std::numeric_limits<float>::max();
    //data3D->min=-0.0049;
    //data3D->max=0.0049;
    for(subX=0;subX<numberSubdomainsX;subX++){
        for(subY=0;subY<numberSubdomainsY;subY++){
            for(subZ=0;subZ<numberSubdomainsZ;subZ++){
                loadSubdomain(timeIndex);
            }
        }
    }

    if(useMinimumMaximum){
        data3D->minAxis=data3D->min;
        data3D->maxAxis=data3D->max;
    }
    std::cout<<"min="<<data3D->min<<" max="<<data3D->max<<std::endl;
    //for (auto registry : data3D->registry)
    for (auto it = data3D->registry.begin(); it!=data3D->registry.end(); ++it) {

        int h = it->k*domainSizeX*domainSizeY + it->j*domainSizeX + it->i;
        it->y[Squencer::sControl.index] = data3D->val[h];
        if(it->min > data3D->val[h]) it->min = data3D->val[h];
        if(it->max < data3D->val[h]) it->max = data3D->val[h];

        if(useMinimumMaximumCell){
            it->minAxis=it->min;
            it->maxAxis=it->max;
        }
        std::cout<<"("<<it->i<<","<<it->j<<","<<it->k<<") min="<<it->min<<" max="<<it->max<<std::endl;
        if( it->show ) data3D->val[h] = 255;
    }




    /*
  for (int i=0;i<data3D->registry.size();i++) {
    int h = data3D->registry[i].k*domainSizeX*domainSizeY + data3D->registry[i].j*domainSizeX + data3D->registry[i].i;
    data3D->registry[i].y.push_back(data3D->val[h]);
    data3D->val[h]=255;
    
    std::cout<<"LoadSimulationData size data"<<data3D->registry[i].y.size()<<std::endl;
  }
  */

    /*
    for( int k = 0; k<domainSizeZ; ++k )
    for( int j = 0; j < domainSizeY; ++j )
      for( int i = 0; i < domainSizeX; ++i ){
        int ij=k*domainSizeX*domainSizeY+j*domainSizeX+i;
        int ji=k*domainSizeX*domainSizeY+i*domainSizeX+j;
        unsigned char temp=data[ij];
        data[ij]=data[ji];
        data[ji]=temp;
      }
*/
}


