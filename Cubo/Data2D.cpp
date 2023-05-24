#include "Data2D.hpp"
/*
Data2D::Data2D(string name){
    data->name=name;
}*/

void Data2D::loadXML(pugi::xml_node_iterator node_it){
    isStaticName=true;
    data=new DataRaw2D();
    std::cout<<"dir Data="<<data<<std::endl;
    std::cout<<"name:"<<node_it->name()<<std::endl;
    //data->name=node_it->name();
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();
        std::string nodeName( it->name() );

        if( nodeName.compare("FileName")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "fileName", fileName);
                ifTheAttributeIsFoundAssign(ait, "locationFile", locationFile);
                ifTheAttributeIsFoundAssign(ait, "fileNamePrefix", fileNamePrefix);
                ifTheAttributeIsFoundAssign(ait, "numberOfIntegerDigits", numberOfIntegerDigits);
                ifTheAttributeIsFoundAssign(ait, "numberOfDecimalDigits", numberOfDecimalDigits);
                ifTheAttributeIsFoundAssign(ait, "variableIndexGetId", variableIndexGetId);
                ifTheAttributeIsFoundAssign(ait, "fileNameSuffix", fileNameSuffix);
            }
        }
        if( nodeName.compare("Grid")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "column", data->column);
                ifTheAttributeIsFoundAssign(ait, "row", data->row);
                ifTheAttributeIsFoundAssign(ait, "xmin", data->xmin);
                ifTheAttributeIsFoundAssign(ait, "xmax", data->xmax);
                ifTheAttributeIsFoundAssign(ait, "ymin", data->ymin);
                ifTheAttributeIsFoundAssign(ait, "ymax", data->ymax);
            }
        }
        if( nodeName.compare("StartWith")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "startWith", startWith);
                ifTheAttributeIsFoundAssign(ait, "horizontal", horizontal);
                ifTheAttributeIsFoundAssign(ait, "vertical", vertical);
            }
        }

        if( nodeName.compare("JumpLines")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "n", jumpLines);
            }
        }

        //      <Grid column="94" row="85"/>
        //  <StartWith startWith="column"/>

        std::cout <<"/>"<< std::endl;
    }

    std::cout<<"Data row="<<data->row<<std::endl;

    if(!isStaticName){
        std::stringstream ss;
        spaceDecimalPoint=(numberOfDecimalDigits>0)?1:0;
        std::cout<<locationFile<<fileNamePrefix<<std::fixed<<std::setfill('0')<<std::setw(numberOfIntegerDigits+numberOfDecimalDigits+spaceDecimalPoint)<<std::setprecision(numberOfDecimalDigits)<<21.278<<fileNameSuffix<<std::endl;
        fileName=ss.str();
    }

    std::ifstream fileLines(fileName.c_str(),std::ios::in);

    if (fileLines.is_open()){


        data->val.resize(data->column*data->row);
        data->mask.resize(data->column*data->row);
        data->xy_linear_o.resize(2*data->column*data->row);
        data->xy_linear_t.resize(2*data->column*data->row);
        data->xy_nearest_o.resize(2*(data->column+1)*(data->row+1));
        data->xy_nearest_t.resize(2*(data->column+1)*(data->row+1));
        //falta llenar mask, xy_linear_o, xy_linear_t, xy_nearest_o, xy_nearest_t

        float y;
        string line;
        for(int k=0;k<jumpLines;k++)
            getline(fileLines,line);
        
        if( startWith.compare("column")==0 ){
            for(int j=0;j<data->row;j++){
                for(int i=0;i<data->column;i++){
                    //fileLines.getline()
                    //fileLines>>y;
                    string temp;
                          fileLines>>temp;
                          if( !(stringstream(temp)>>y) ){
                            y=0.0;
                          }


                        //cout<<y<<" ";
                        int index,ii,jj;
                        if(horizontal==0)ii=i;else ii=data->column-1-i;
                        if(vertical==0)jj=j;else jj=data->row-1-j;
                        index=jj*data->column+ii;
                        data->val[index]=y;
                        if( data->max < data->val[index] )
                            data->max = data->val[index];
                        if( data->min > data->val[index] )
                            data->min = data->val[index];

                }
                //cout<<endl;
            }
        }
        if( startWith.compare("row")==0 ){
            for(int i=0;i<data->column;i++)
                for(int j=0;j<data->row;j++){
                    fileLines>>y;
                    int index,ii,jj;
                    if(horizontal==0)ii=i;else ii=data->column-1-i;
                    if(vertical==0)jj=j;else jj=data->row-1-j;
                    index=jj*data->column+ii;
                    data->val[index]=y;
                    if( data->max < data->val[index] )
                        data->max = data->val[index];
                    if( data->min > data->val[index] )
                        data->min = data->val[index];
                    //if(k<5)data->val[j*data->column+i]=100;
                    //k++;
                }
        }
        std::cout<<" min="<<data->min<<" max="<<data->max<<std::endl;
    }
    else{
        std::cout<<std::endl<<"Error opening file:"<<fileName<<std::endl;

    }
}


void Data2D::draw(){

}
