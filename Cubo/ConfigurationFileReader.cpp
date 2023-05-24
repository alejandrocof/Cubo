#include "ConfigurationFileReader.hpp"

bool ConfigurationFileReader::load(char *fileName){
  pugi::xml_document doc;
  if ( !doc.load_file(fileName) ){
    std::cout<<"Error loading XML file: "<<fileName<<std::endl;
    return false;
  }
  std::cout<<"Starts loading XML file: "<<fileName<<std::endl;

  for (pugi::xml_node_iterator it = doc.begin(); it != doc.end(); ++it){
    std::string nodeName( it->name() );
    std::cout<<"name:"<<it->name()<<std::endl;
    
    string Id,nameFile;
    for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
      //if( std::string("Id").compare( ait->name() )==0 ){
      //  Id=ait->value();
      //}
      ifTheAttributeIsFoundAssignString(ait, "Id", Id);
      ifTheAttributeIsFoundAssign(ait, "file", nameFile);
    }
    

    if( nodeName.compare("Settings")==0 ){
      std::cout<<"Abriendo otro archivo xml"<<std::endl;
      ConfigurationFileReader configFile;
      configFile.load( (char *)nameFile.c_str() );
    }

    if( nodeName.compare("Squencer")==0 ){
      std::cout<<"Squencer"<<std::endl;
      Squencer *sq=new Squencer();
      sq->loadXML( it );
      Shapes::push_back_2D( &Squencer::draw , sq );
      //Updates::push_back_Updates( &Squencer::update , sq );
      //Squencer::loadXML(it);
    }

    if( nodeName.compare("Global")==0 ){
        std::cout<<"**Global"<<std::endl;
        Global *glb=new Global();
        glb->loadXML(it);
        //Global::imagePath;
    }

    if( nodeName.compare("DisplaySettings")==0 ){
      std::cout<<"**Display Settings"<<std::endl;
      DisplaySettings *ds=new DisplaySettings();
      ds->loadXML(it);
      Shapes::push_front_3D( &DisplaySettings::draw , ds );
      //Shapes::push_back_3D( &DisplaySettings::draw , lxml );
    }
    
    
    if( nodeName.compare("VolumeRendering")==0 ){
      std::cout<<"Creating VolumeRendering on the map"<<std::endl;
      VolumeRendering *vr=new VolumeRendering();
      vr->loadXML( it );
      //vr->init();
      Shapes::push_back_3D( &VolumeRendering::draw , vr );
      //shapes.push_back( std::bind( &VolumeRendering::draw , vr ) );
    }
    
    if( nodeName.compare("ColorMap")==0 ){
      std::cout<<"Creating ColorMap on the map"<<std::endl;
      ColorMapClass *color_map=new ColorMapClass();
      color_map->loadXML( it );
      //drawable_id.insert(Id,static_cast<const void*>(color_map) );
      std::cout<<"direccion inicial:"<<color_map<<std::endl;
      std::cout<<"insert Id="<<Id<<" antes de insertar tipo:"<<typeid(color_map).name()<<std::endl;
      DrawableIdentifier::insert(Id,color_map);
    }
    
    if( nodeName.compare("ColorBar")==0 ){
      std::cout<<"Creating ColorBar on the map"<<std::endl;
      ColorBar *color_bar=new ColorBar();
      color_bar->loadXML( it );
      Shapes::push_back_2D( &ColorBar::draw , color_bar );
      //drawable_id.insert(Id,static_cast<const void*>(color_map) );
    }
    
    if( nodeName.compare("ViewingPlane")==0 ){
      std::cout<<"Creating ViewingPlane on the map"<<std::endl;
      ViewingPlane *vp=new ViewingPlane();
      vp->loadXML( it );
      Shapes::push_back_3D( &ViewingPlane::draw , vp );
    }

    if( nodeName.compare("Compute")==0 ){
      std::cout<<"Creating Compute on the map"<<std::endl;
      Compute *comp=new Compute();
      comp->loadXML( it );
      //Shapes::push_back_3D( &Compute::draw , cp );
    }
    
    if( nodeName.compare("Interpolation")==0 ){
      std::cout<<"Creating Interpolation on the map"<<std::endl;
      Interpolation *in=new Interpolation(Id);
      in->loadXML( it );
      Shapes::push_back_3D( &Interpolation::draw , in );
    }
    
    if( nodeName.compare("Data2D")==0 ){
      std::cout<<"Creating Data2D on the map"<<std::endl;
      Data2D *data2D=new Data2D();

      data2D->loadXML( it );
      std::cout<<"Dir afuera DataRaw"<<data2D->data<<std::endl;
      DataRaw2D *_data2D=data2D->data;
      _data2D->name=Id;
      std::cout<<"Dir afuera DataRaw"<<_data2D<<std::endl;
      DrawableIdentifier::insert(Id,_data2D);
    }
    
    
    if( nodeName.compare("MapPoint")==0 ){
      std::cout<<"Creating points on the map"<<std::endl;
      MapPoint *mp=new MapPoint();
      mp->loadXML( it );
      Shapes::push_back_3D( &MapPoint::draw , mp );
      //shapes.push_back( std::bind( &MapPoint::draw , mp ) );
    }
    
    if( nodeName.compare("Stroke")==0 ){
      std::cout<<"Creating stroke on the map"<<std::endl;
      Stroke2D *stroke2D=new Stroke2D();
      stroke2D->loadXML( it );
      Shapes::push_back_3D( &Stroke2D::draw , stroke2D );
      //shapes.push_back( std::bind( &Stroke2D::draw , stroke2D ) );
    }
    
    if( nodeName.compare("Label3D")==0 ){
      std::cout<<"Creating label on the map"<<std::endl;
      Label3DClass *label=new Label3DClass();
      label->loadXML( it );
      Shapes::push_back_3D( &Label3DClass::draw , label );
      //shapes.push_back( std::bind( &Label::draw , label ) );
    }
    
    if( nodeName.compare("Label2D")==0 ){
      std::cout<<"Creating label on the map"<<std::endl;
      Label3DClass *label=new Label3DClass();
      label->loadXML( it );
      Shapes::push_back_2D( &Label3DClass::draw , label );
      //shapes.push_back( std::bind( &Label::draw , label ) );
    }
    

    if( nodeName.compare("Grid2D")==0 ){
      std::cout<<"Creating Grid2D on the map"<<std::endl;
      Grid2D *g=new Grid2D();
      g->loadXML( it );
      Shapes::push_back_3D( &Grid2D::draw , g );
    }
    
    if( nodeName.compare("Format")==0 ){
      std::cout<<"Creating Format on the map"<<std::endl;
      Format *f=new Format();
      f->loadXML( it );
      
    }
    
    if( nodeName.compare("SimulationData")==0 ){
      std::cout<<"Creating SimulationData on the map"<<std::endl;
      LoadSimulationData *sd=new LoadSimulationData();
      sd->loadXML( it );
      //DataRaw2D *_data2D=sd->data2D;
      DataRaw3D *_data3D=sd->data3D;
      //std::cout<<"dir de data2d proveniente de LoadSimulationData="<<_data2D<<std::endl;
      //DrawableIdentifier::insert(Id2D,_data2D);
      DrawableIdentifier::insert(Id,_data3D);
      Updates::push_back_Updates( &LoadSimulationData::setTimeIndex , sd );
      
         // void setTimeIndex();
      //DrawableIdentifier::insert(Id,sd);
    }

//    if( nodeName.compare("LoadNetCDF")==0 ){
//      std::cout<<"Creating LoadNetCDF on the map"<<std::endl;
//      LoadNetCDF *lncdf=new LoadNetCDF();
//      lncdf->loadXML( it );
//      DataRaw2D *_data2D=lncdf->data2D;
//      _data2D->name=Id;
//      //std::cout<<"dir de data2d proveniente de LoadSimulationData="<<_data2D<<std::endl;
//      //DrawableIdentifier::insert(Id2D,_data2D);
//      DrawableIdentifier::insert(Id,_data2D);
//      Updates::push_back_Updates( &LoadNetCDF::setTimeIndex , lncdf );

//         // void setTimeIndex();
//      //DrawableIdentifier::insert(Id,sd);
//    }

    if( nodeName.compare("ShapeFileCSV")==0 ){
      std::cout<<"Creating CSV on the map"<<std::endl;
      LoadShapeFileCSV *shapeFileCSV=new LoadShapeFileCSV();
      shapeFileCSV->loadXML( it );
      Shapes::push_back_3D( &LoadShapeFileCSV::draw , shapeFileCSV );
      DrawableIdentifier::insert(Id,shapeFileCSV);
    }

    if( nodeName.compare("LoadArcASCII")==0 ){
      std::cout<<"Creating LoadArcASCII on the map"<<std::endl;
      LoadArcASCII *arcASCII=new LoadArcASCII();
      arcASCII->loadXML( it );
      DataRaw2D *_data2D=arcASCII->data2D;
      _data2D->name=Id;
      std::cout<<"dir de data2d proveniente de LoadArcASCII="<<_data2D<<std::endl;
      cout<<"row="<<_data2D->row<<" column="<<_data2D->column<<endl;
      //DrawableIdentifier::insert(Id2D,_data2D);
      DrawableIdentifier::insert(Id,_data2D);
    }


    if( nodeName.compare("Data3D")==0 ){
      std::cout<<"Creating Data3D on the map"<<std::endl;
      Data3D *d3d=new Data3D();
      d3d->loadXML( it );
      DataRaw3D *_data3D=d3d->data;
      DrawableIdentifier::insert(Id,_data3D);
    }

    if( nodeName.compare("RegistryGraph")==0 ){
      std::cout<<"Creating RegistryGraph on the map"<<std::endl;
      RegistryGraph *registry_graph=new RegistryGraph();
      registry_graph->loadXML( it );
      Shapes::push_back_2D( &RegistryGraph::draw , registry_graph );
      //drawable_id.insert(Id,static_cast<const void*>(color_map) );
    }

    if( nodeName.compare("Projection")==0 ){
      std::cout<<"Creating Projection"<<std::endl;
      Projection *proj=new Projection();
      proj->loadXML( it );
      DrawableIdentifier::insert(Id,proj);
    }

    if( nodeName.compare("BarScale")==0 ){
      std::cout<<"Creating BarScale on the map"<<std::endl;
      BarScale *bs=new BarScale();
      bs->loadXML( it );
      Shapes::push_back_3D( &BarScale::draw , bs );
    }

    if( nodeName.compare("Text3D")==0 ){
      std::cout<<"Creating Text3D on the map"<<std::endl;
      Text3D *t3d=new Text3D();
      t3d->loadXML( it );
      Shapes::push_back_3D( &Text3D::draw , t3d );
    }

    if( nodeName.compare("Distance")==0 ){
      std::cout<<"Creating Distance on the map"<<std::endl;
      Distance *dis=new Distance();
      dis->loadXML( it );
      Shapes::push_back_3D( &Distance::draw , dis );
    }

    if( nodeName.compare("Mesh")==0 ){
      std::cout<<"Creating Mesh on the map"<<std::endl;
      Mesh *m=new Mesh();
      m->loadXML( it );
      Shapes::push_back_3D( &Mesh::draw , m );
    }
    


        /*
        
        std::cout<<"<"<<it->name();
        for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
        {
            std::cout<<" "<< ait->name() << "=" << ait->value();
        }
        std::cout <<">"<< std::endl;
        
        for (pugi::xml_node_iterator it2 = it->begin(); it2 != it->end(); ++it2){
          std::cout<<"    <"<<it2->name();
          //std::cout<<" text="<<it2->text().get()<<std::endl;
          
          for (pugi::xml_attribute_iterator ait = it2->attributes_begin(); ait != it2->attributes_end(); ++ait)
          {
            std::cout <<" "<< ait->name() << "=" << ait->value();
          }
          std::cout <<"/>"<< std::endl;
          
        }
        
        std::cout<<"<"<<it->name()<<"/>"<<std::endl;
        */
        
    }

//  font=C3_Font::add("/usr/share/fonts/truetype/Nakula/nakula.ttf",100);
//  C3_GL_Text *GLtext=new C3_GL_Text(1,1);
//  GLtext->font(font);
//  GLtext->colorT(0,255,0);
//  GLtext->scaleT(4.0f);
//  GLtext->verticalAlign(VERTICAL_ALIGN_MIDDLE);
//  GLtext->horizontalAlign(HORIZONTAL_ALIGN_RIGHT);
//  GLtext->directionT(DIRECTION_HORIZONTAL);
//  GLtext->postionT(0.0,0.5);
//  GLtext->push_text("Hola Mundo");
//  //GLtext.draw();
//  Shapes::push_back_2D( &C3_GL_Text::draw , GLtext );

////  ColorMapClass *color_map=new ColorMapClass();
////  color_map->min(0.0);
////  color_map->max(1.0);

////  ColorBar *color_bar=new ColorBar();
////  color_bar->color_map=color_map;
////  color_bar->titleText="HOLA MUNDO";
////  Shapes::push_back_2D( &ColorBar::draw , color_bar );

//  C3_GL_Text *GLtextA;
//  GLtextA=new C3_GL_Text(1, 1);
//  GLtextA->font(font);
//  GLtextA->colorT(255,255,255);
//  GLtextA->scaleT(1.0);
//  GLtextA->postionT(0.0,0.0);
//  GLtextA->verticalAlign(VERTICAL_ALIGN_TOP);
//  GLtextA->horizontalAlign(HORIZONTAL_ALIGN_CENTER);
//  GLtextA->directionT(DIRECTION_ROTATE90);
//  GLtextA->push_text("Aceleración (m/s²)");


//  GLtextA->scaleT(2.0);
//  GLtextA->postionT(0.0,0.0);
//  GLtextA->verticalAlign(VERTICAL_ALIGN_TOP);
//  GLtextA->horizontalAlign(HORIZONTAL_ALIGN_RIGHT);

//  GLtextA->colorT(128,0,0);
//  GLtextA->directionT(DIRECTION_HORIZONTAL);
//  //GLtextA->push_text("Horizontal");
//  GLtextA->push_text("Aspecto");

//  GLtextA->colorT(0,128,0);
//  GLtextA->directionT(DIRECTION_ROTATE90);
//  //GLtextA->push_text("Giro 90°");
//  GLtextA->push_text("Aspecto");

//  GLtextA->colorT(0,0,128);
//  GLtextA->directionT(DIRECTION_ROTATE270);
//  //GLtextA->push_text("Giro 270°");
//  GLtextA->push_text("Aspecto");

//  GLtextA->scaleT(1.0);
//  GLtextA->directionT(DIRECTION_HORIZONTAL);

//  GLtextA->colorT(128,0,0);
//  GLtextA->postionT(-0.5,0.5);
//  GLtextA->horizontalAlign(HORIZONTAL_ALIGN_LEFT);
//  GLtextA->push_text("Izquierda");

//  GLtextA->colorT(0,128,0);
//  GLtextA->postionT(-0.5,0.4);
//  GLtextA->horizontalAlign(HORIZONTAL_ALIGN_CENTER);
//  GLtextA->push_text("Centro");

//  GLtextA->colorT(0,0,128);
//  GLtextA->postionT(-0.5,0.3);
//  GLtextA->horizontalAlign(HORIZONTAL_ALIGN_RIGHT);
//  GLtextA->push_text("Derecha");

//  GLtextA->directionT(DIRECTION_ROTATE270);

//  GLtextA->colorT(128,0,0);
//  GLtextA->postionT(-0.6,-0.7);
//  GLtextA->verticalAlign(VERTICAL_ALIGN_TOP);
//  GLtextA->push_text("Arriba");

//  GLtextA->colorT(0,128,0);
//  GLtextA->postionT(-0.6,-0.4);
//  GLtextA->verticalAlign(VERTICAL_ALIGN_MIDDLE);
//  GLtextA->push_text("Medio");

//  GLtextA->colorT(0,0,128);
//  GLtextA->postionT(-0.6,-0.1);
//  GLtextA->verticalAlign(VERTICAL_ALIGN_BOTTOM);
//  GLtextA->push_text("Abajo");

//  GLtextA->verticalAlign(VERTICAL_ALIGN_TOP);
//  GLtextA->horizontalAlign(HORIZONTAL_ALIGN_RIGHT);
//  GLtextA->directionT(DIRECTION_HORIZONTAL);
//  GLtextA->font(font);
//  GLtextA->colorT(0,0,0);
//  GLtextA->postionT(0.2,-0.5);
//  GLtextA->push_text("ax²+bx+c=0");

//  //display->addListDraw(&C3_GL_Text::draw,GLtextA);
//  Shapes::push_back_2D( &C3_GL_Text::draw ,GLtextA);
  return true;
}



bool ifTheAttributeIsFoundAssignString(const pugi::xml_attribute_iterator &it, const std::string expectedName,std::string &val){
  if( expectedName.compare(it->name())==0 ){

//      std::unordered_map<string,string>::const_iterator itLabel= Global::mapLabel.find (it->value());

//      if ( itLabel == Global::mapLabel.end() )
//          val=it->value();
//      else
//          val=itLabel->second;

      val=replaceGlobalId(it->value());

      //val=it->value();
      std::cout<<" "<<it->name()<<"=\""<<val<<"\"";
      return true;
  }
  return false;
}



