#include "LoadArcASCII.hpp"

//LoadArcASCII::LoadArcASCII()
//{

//}

void LoadArcASCII::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    data2D=new DataRaw2D();
    std::cout<<"dir de data2d en LoadArcASCII="<<data2D<<std::endl;

    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("File")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "name", fileName);
                ifTheAttributeIsFoundAssign(ait, "scale", scale);
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
    std::cout << std::endl;
    std::cout << "****name:"<<fileName<<std::endl;
    if(!fileName.empty()){
        std::cout << fileName<<std::endl;
        if(load(fileName)){

        }
    }

}

bool LoadArcASCII::load(string name){
    float nodata_value;
    int column,row;
    double xi,yi,xcellsize,ycellsize;
    std::ifstream myfile(name.c_str(),std::ios::in);
    cout<<"Abriendo archivo asc:"<<name<<endl;
    if (myfile.is_open()){
        cout<<"Abriendo archivo asc:"<<name<<endl;
        //int iLine=1;
        std::string line;
        //vector <string> fields;
        string field,val;

        getline(myfile,line);

        if(readLine(line,column,"ncols")==0)return false;

        getline(myfile,line);
        if(readLine(line,row,"nrows")==0)return false;

        getline(myfile,line);
        if(readLine(line,xi,"xllcorner")==0)return false;

        getline(myfile,line);
        if(readLine(line,yi,"yllcorner")==0)return false;

        getline(myfile,line);
        int k=readLine(line,xcellsize,"cellsize","xcellsize");
        if(k==0)return false;

        getline(myfile,line);
        if(k==1){
            ycellsize=xcellsize;
            if(readLine(line,nodata_value,"nodata_value")==0)return false;
        }
        else{
            if(k==2){
                if(readLine(line,ycellsize,"ycellsize")==0)return false;
            }
        }

        cout<<"column:"<<column<<endl;
        cout<<"row:"<<row<<endl;

        /****************************************************************************/
        /*****Error al reservar el espacio para xy_linear_o **************************/
        /****************************************************************************/

        data2D=new DataRaw2D();
        data2D->row=row;
        data2D->column=column;
        cout<<"row="<<data2D->row<<" column="<<data2D->column<<endl;
        data2D->val.resize(column*row);
        cout<<"resize="<<data2D->row * data2D->column<<endl;
        data2D->mask.resize(column*row);
        cout<<"mask resize="<<data2D->row * data2D->column<<endl;
        data2D->xy_linear_o.resize(2*column*row);
        cout<<"o resize="<<2*data2D->row * data2D->column<<endl;
        data2D->xy_linear_t.resize(2*column*row);
        cout<<"t resize="<<2*data2D->row * data2D->column<<endl;
        data2D->xy_nearest_o.resize(2*(data2D->column+1)*(data2D->row+1));
        data2D->xy_nearest_t.resize(2*(data2D->column+1)*(data2D->row+1));
        data2D->min=std::numeric_limits<float>::max();
        data2D->max=-std::numeric_limits<float>::max();
        data2D->wearMask=false;
        cout<<"asignando"<<endl;
        float h;
        vector< double > node(2);
        for(int j=0,jr=data2D->row-1;j<data2D->row;j++,jr--){
            for(int i=0;i<data2D->column;i++){
                //int kr=jr*data2D->column+i;
                int kr=j*data2D->column+i;
                myfile>>h;
                data2D->val[kr]=h*scale;
                if(h!=nodata_value){
                    if(data2D->min > data2D->val[kr]) data2D->min = data2D->val[kr];
                    if(data2D->max < data2D->val[kr]) data2D->max = data2D->val[kr];
                    data2D->mask[kr]=255;
                }
                else{
                    data2D->mask[kr]=0;
                }

                int k=j*data2D->column+i;
                data2D->xy_linear_o[2*k]=xi+xcellsize*i;
                data2D->xy_linear_o[2*k+1]=yi+ycellsize*j;
//                data2D->xy_linear_t[2*k]=data2D->xy_linear_o[2*k]*DEG_TO_RAD;
//                data2D->xy_linear_t[2*k+1]=data2D->xy_linear_o[2*k+1]*DEG_TO_RAD;
                data2D->xy_linear_t[2*k]=data2D->xy_linear_o[2*k];
                data2D->xy_linear_t[2*k+1]=data2D->xy_linear_o[2*k+1];

            }
        }

        cout<<"val_min="<<data2D->min<<" val_max="<<data2D->max<<endl;
        data2D->xmin=xi;
        data2D->ymin=yi;
        data2D->xmax=xi+xcellsize*(data2D->column-1);
        data2D->ymax=yi+ycellsize*(data2D->row-1);

        for(int j=0;j<=data2D->row;j++){
            for(int i=0;i<=data2D->column;i++){
                int k=j*(data2D->column+1)+i;
                node[0]=data2D->xmin+(data2D->xmax-data2D->xmin)*i/data2D->column;
                node[1]=data2D->ymin+(data2D->ymax-data2D->ymin)*j/data2D->row;
                proj->transform(node);
                data2D->xy_nearest_t[2*k]=node[0];
                data2D->xy_nearest_t[2*k+1]=node[1];
            }
        }



        proj->transform(data2D->xy_linear_t);

        data2D->xtmin=std::numeric_limits<double>::max();
        data2D->xtmax=-std::numeric_limits<double>::max();
        data2D->ytmin=std::numeric_limits<double>::max();
        data2D->ytmax=-std::numeric_limits<double>::max();
        for(int j=0;j<data2D->row;j++){
            for(int i=0;i<data2D->column;i++){
                int k=j*data2D->column+i;
                if(data2D->xtmin > data2D->xy_linear_t[2*k]) data2D->xtmin = data2D->xy_linear_t[2*k];
                if(data2D->xtmax < data2D->xy_linear_t[2*k]) data2D->xtmax = data2D->xy_linear_t[2*k];
                if(data2D->ytmin > data2D->xy_linear_t[2*k+1]) data2D->ytmin = data2D->xy_linear_t[2*k+1];
                if(data2D->ytmax < data2D->xy_linear_t[2*k+1]) data2D->ytmax = data2D->xy_linear_t[2*k+1];
            }
        }

        if(Shapes::xtmin>data2D->xtmin)Shapes::xtmin=data2D->xtmin;
        if(Shapes::xtmax<data2D->xtmax)Shapes::xtmax=data2D->xtmax;
        if(Shapes::ytmin>data2D->ytmin)Shapes::ytmin=data2D->ytmin;
        if(Shapes::ytmax<data2D->ytmax)Shapes::ytmax=data2D->ytmax;

//        for(unsigned int k=0;k<data2D->val.size();k++){
//            myfile>>h;
//            data2D->val[j*data2D->column+i]=h;
//            if( data2D->min > h )data2D->min=h;
//            if( data2D->max < h )data2D->max=h;
//            i++;
//            if(i>=data2D->column){
//                i=0;
//                j--;
//            }
//        }

        return true;
    }
    else{
        cout<<"No se pudo leer el archivo: "<<fileName<<endl;
        return false;
    }

    //dataFile=new NcFile(name, NcFile::read);
}
