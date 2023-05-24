#include "Data3D.hpp"

void Data3D::loadXML(pugi::xml_node_iterator node_it){
    data=new DataRaw3D();
    std::cout<<"dir Data="<<data<<std::endl;
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();
        std::string nodeName( it->name() );

        if( nodeName.compare("Data")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "fileName", fileName);
                ifTheAttributeIsFoundAssign(ait, "Nx", data->Nx);
                ifTheAttributeIsFoundAssign(ait, "Ny", data->Ny);
                ifTheAttributeIsFoundAssign(ait, "Nz", data->Nz);
                ifTheAttributeIsFoundAssign(ait, "jumpLines", jumpLines);
                //ifTheAttributeIsFoundAssign(ait, "startWith", startWith);
            }
        }
        std::cout <<"/>"<< std::endl;
    }

    std::cout<<"Data("<<data->Nx<<","<<data->Ny<<","<<data->Nz<<")"<<std::endl;

    std::ifstream fileLines(fileName.c_str(),std::ios::in);

    if (fileLines.is_open()){
        data->val.resize(data->Nx * data->Ny * data->Nz);
        float y;
        string line;
        for(int k=0;k<jumpLines;k++)
            getline(fileLines,line);


        for(int k=0;k<data->Nz;k++)
            for(int j=0;j<data->Ny;j++)
                for(int i=0;i<data->Nx;i++)
                {
                    int index=k*data->Ny*data->Nx + j*data->Nx + i;
                    fileLines>>y;
                    data->val[index]=y;
                    if( data->max < data->val[index] )
                        data->max = data->val[index];
                    if( data->min > data->val[index] )
                        data->min = data->val[index];
                    cout<<"index="<<index<<"=("<<i<<","<<j<<","<<k<<")="<<data->val[index]<<endl;
                }
        data->minAxis=data->min;
        data->maxAxis=data->max;
        std::cout<<" min="<<data->min<<" max="<<data->max<<std::endl;
    }
    else{
        std::cout<<std::endl<<"Error opening file:"<<fileName<<std::endl;

    }
}


void Data3D::draw(){

}
