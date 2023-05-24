#include "Compute.hpp"

void Compute::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("Data")==0 ){
            std::string IdAName,IdBName,IdCName,IdOutName,OutFileName;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssign(ait, "inA", IdAName);
                ifTheAttributeIsFoundAssign(ait, "inB", IdBName);
                ifTheAttributeIsFoundAssign(ait, "inC", IdCName);
                ifTheAttributeIsFoundAssign(ait, "out", IdOutName);
                ifTheAttributeIsFoundAssign(ait, "operation", operation);
                ifTheAttributeIsFoundAssign(ait, "K", pK);
                ifTheAttributeIsFoundAssign(ait, "E", pE);
                ifTheAttributeIsFoundAssign(ait, "save", OutFileName);
            }

            if(!IdAName.empty())
                DrawableIdentifier::data(IdAName,data_inA);

            if(!IdBName.empty())
                DrawableIdentifier::data(IdBName,data_inB);

            if(!IdCName.empty())
                DrawableIdentifier::data(IdCName,data_inC);
            //DrawableIdentifier::data(IdOutName,data_out);

            if( FnvHash(operation.c_str())==FnvHash("scale") ){
                data_out=new DataRaw2D();
                if(!IdOutName.empty()){
                    DrawableIdentifier::insert(IdOutName,data_out);
                }
                data_out->column=data_inA->column;
                data_out->row=data_inA->row;
                data_out->val.resize(data_out->column*data_out->row);

                for(int j=0;j<data_out->row;j++)
                    for(int i=0;i<data_out->column;i++){
                        int ij=j*data_out->column+i;
                        data_out->val[ij]=pK*data_inA->val[ij];
                    }
            }
            else{
                if(data_inA->column==data_inB->column && data_inA->row==data_inB->row){
                    data_out=new DataRaw2D();
                    if(!IdOutName.empty()){
                        DrawableIdentifier::insert(IdOutName,data_out);
                    }
                    data_out->column=data_inA->column;
                    data_out->row=data_inA->row;
                    data_out->val.resize(data_out->column*data_out->row);

                    std::unordered_map<int,vector<double> > mapReg;
                    if(data_inC!=0 && data_inC->column==data_out->column && data_inC->row==data_out->row){
                        for(int j=0;j<data_out->row;j++)
                            for(int i=0;i<data_out->column;i++){
                                int ij=j*data_out->column+i;
                                vector<double> y;
                                y.resize(5,0);
                                //mapReg.insert( std::make_pair<int,vector<double>() >(data_inC->val[ij],y) );
                                mapReg.insert( std::make_pair(data_inC->val[ij],y) );
                            }
                        cout<<"tamaño map ="<<mapReg.size()<<endl;
                    }

                    switch( FnvHash(operation.c_str()) ){


                    case FnvHash("vulnerability"):{
                        float vul=0.0;
                        for(int j=0;j<data_out->row;j++)
                            for(int i=0;i<data_out->column;i++){
                                int ij=j*data_out->column+i;
                                //data_out->val[ij]=(6.79e-6)*pow(data_inA->val[ij],1.6);
                                //data_out->val[ij]=(6.79e-6)*pow(data_inA->val[ij],1.782285);//1N
                                //data_out->val[ij]=(6.79e-6)*pow(data_inA->val[ij],1.8788865);//2N
                                data_out->val[ij]=(6.79e-6)*pow(data_inA->val[ij],1.70381);//3N
                                data_out->val[ij]=(pK)*pow(data_inA->val[ij],pE);
                                vul+=data_out->val[ij];
                            }
                        cout<<"Vulnerability "<<vul<<endl;
                    }
                        break;

                    case FnvHash("damage"):{
                        float dam=0.0;
                        data_out->max=-std::numeric_limits<float>::max();
                        data_out->min=std::numeric_limits<float>::max();
                        cout<<"Damage "<<endl;

                        for(int j=0;j<data_out->row;j++)
                            for(int i=0;i<data_out->column;i++){
                                int ij=j*data_out->column+i;
                                if(data_inA->val[ij]<1.0){
                                    data_inA->val[ij]=1.0;
                                }
                                float vul=(pK)*pow(data_inA->val[ij],pE);
                                //data_out->val[ij]=data_inB->val[ij]*(6.79e-6)*pow(data_inA->val[ij],1.9);
                                //float vul=(2.927906e-11)*pow(data_inA->val[ij],4.387671097);



                                //float vul=(6.79e-6)*pow(data_inA->val[ij],1.782285);//1N 3128
                                //float vul=0.4827365729*(6.79e-6)*pow(data_inA->val[ij],1.782285);//1N Daños menores
                                //float vul=0.3631713555*(6.79e-6)*pow(data_inA->val[ij],1.782285);//1N Daños parciales
                                //float vul=0.1540920716*(6.79e-6)*pow(data_inA->val[ij],1.782285);//1N Daños totales
                                //float vul=(6.79e-6)*pow(data_inA->val[ij],1.8788865);//2N 10677
                                //float vul=0.6513065468*(6.79e-6)*pow(data_inA->val[ij],1.8788865);//2N Daños menores
                                //float vul=0.2942774188*(6.79e-6)*pow(data_inA->val[ij],1.8788865);//2N Daños parciales
                                //float vul=00.0544160345*(6.79e-6)*pow(data_inA->val[ij],1.8788865);//2N Daños totales
                                //float vul=(6.79e-6)*pow(data_inA->val[ij],1.70381);//3N 1350
                                //float vul=0.6074074074*(6.79e-6)*pow(data_inA->val[ij],1.70381);//3N Daños menores
                                //float vul=0.2466666667*(6.79e-6)*pow(data_inA->val[ij],1.70381);//3N Daños parciales
                                //float vul=0.1459259259*(6.79e-6)*pow(data_inA->val[ij],1.70381);//3N Daños totales

                                //float vul=(0.00469)*pow(data_inA->val[ij],0.685);//N1 368x488
                                //float vul=(0.0306)*pow(data_inA->val[ij],0.329);//N2 368x488
                                //float vul=(0.0287)*pow(data_inA->val[ij],0.423);//N3 368x488

                                //float vul=(0.00071)*pow(data_inA->val[ij],0.897);//N1 184x244
                                //float vul=(0.0196)*pow(data_inA->val[ij],0.288);//N2 184x244
                                //float vul=(0.00481)*pow(data_inA->val[ij],0.568);//N3 184x244

                                //float vul=(0.00356)*pow(data_inA->val[ij],0.674);//N1 368x488
                                //float vul=(0.0277)*pow(data_inA->val[ij],0.221);//N2 368x488
                                //float vul=(0.8)*pow(data_inA->val[ij],-0.294);//N3 368x488

                                //float vul=(0.000000776)*pow(data_inA->val[ij],1.83);//N1 46x61
                                //float vul=(0.0000000385)*pow(data_inA->val[ij],2.31);//N2 46x61
                                //float vul=(0.00000471)*pow(data_inA->val[ij],1.41);//N3 46x61

                                //Eliminando puntos
                                //float vul=(0.000075)*pow(data_inA->val[ij],0.783);//N3 46x61
                                //float vul=(0.251)*pow(data_inA->val[ij],-0.0909);//N3 368x488





                                data_out->val[ij]=data_inB->val[ij]*vul;
                                if(data_out->max < data_out->val[ij])data_out->max = data_out->val[ij];
                                if(data_out->min > data_out->val[ij])data_out->min = data_out->val[ij];
                                //data_out->val[ij]=data_inB->val[ij]*(2.927906e-11)*pow(data_inA->val[ij],4.387671097);
                                //escalar por 4
                                //data_out->val[ij]=data_inB->val[ij]*(6.79e-6)*pow(data_inA->val[ij],1.6);
                                mapReg.at(data_inC->val[ij])[0]+=data_out->val[ij];//daño
                                mapReg.at(data_inC->val[ij])[1]+=data_inA->val[ij];//Sa
                                mapReg.at(data_inC->val[ij])[2]+=data_inB->val[ij];//Predios
                                mapReg.at(data_inC->val[ij])[3]++;//
                                mapReg.at(data_inC->val[ij])[4]+=vul;//Vul total
                                dam+=data_out->val[ij];
                            }
                        cout<<"Daños DF "<<dam<<endl;

                        cout<<"Datos totales por delegación"<<endl;
                        cout<<"Delegacion ΣDaños ΣSa ΣPredios ΣCeldas ΣVulnerabilidad"<<endl;
                        //x.first Delegacion
                        //x.second[0] ΣDaños
                        //x.second[1] ΣSa    x.second[1]/x.second[3] Sa promedio
                        //x.second[2] ΣPredios
                        //x.second[3] ΣCeldas
                        //x.second[4] ΣVulnerabilidad    x.second[4]/x.second[3] Vulnerabilidad promedio
                        float acum=0;
                        for (auto& x: mapReg) {
                            //std::cout << std::fixed<< std::setprecision(0) << x.first << " " << x.second[0] << " " << x.second[1]/x.second[3] << " " << x.second[2] << " " << x.second[4]/x.second[3] <<  std::endl;
                            std::cout << std::fixed<< std::setprecision(6) << x.first << " " << x.second[0] << " " << x.second[1] << " " << x.second[2] << " " << x.second[3]<<" "<<x.second[4] <<  std::endl;
                            acum+=x.second[0];
                        }

                        std::cout << std::fixed<< std::setprecision(17) << "Daños Iztapalapa "<<mapReg.at(10)[0]<<" pK="<<pK<<" pE="<<pE<<std::endl;


                        cout<<"Daños DF "<<acum<<endl;
                        cout<<"Daños min="<<data_out->min<<" max="<<data_out->max<<endl;
                    }
                        break;

                    case FnvHash("properties"):{
                        float prop=0.0;
                        for(int j=0;j<data_inB->row;j++)
                            for(int i=0;i<data_inB->column;i++){
                                int ij=j*data_inB->column+i;
                                mapReg.at(data_inC->val[ij])[0]+=data_inB->val[ij];
                                prop+=data_inB->val[ij];
                            }
                        cout<<"Properties "<<prop<<endl;
                        float acum=0;
                        for (auto& x: mapReg) {
                            std::cout << x.first << " " << x.second[0] << std::endl;
                            acum+=x.second[0];
                        }
                        cout<<"Acum "<<acum<<endl;
                    }
                        break;


                    default:
                        cout<<"Se requiere que defina una operación:"<<endl;
                        cout<<"operation=\"vulnerability\""<<endl;
                        cout<<"operation=\"damage\""<<endl;
                        break;
                    }

                    if(!OutFileName.empty()){
                        ofstream outputFile(OutFileName,std::ios::out);
                        if (outputFile.is_open()){
                            for(int j=0;j<data_out->row;j++){
                                for(int i=0;i<data_out->column-1;i++){
                                    int ij=j*data_out->column+i;
                                    outputFile<<data_out->val[ij]<<" ";
                                }
                                int ij=j*data_out->column+data_out->column-1;
                                outputFile<<data_out->val[ij]<<std::endl;
                            }
                        }
                        else{
                            std::cout << "No se puede construir el archivo:"<<OutFileName<<std::endl;
                        }
                    }

                }
            }
        }

        std::cout <<"/>"<< std::endl;
    }

}
