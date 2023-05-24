#include "Format.hpp"



void Format::loadXML(pugi::xml_node_iterator node_it){
  std::cout<<"name:"<<node_it->name()<<std::endl;
  for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
    std::cout<<"    <"<<it->name();
    std::string nodeName( it->name() );

    if( nodeName.compare("Notation")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "notation", notation);
      }
    }
    
    if( nodeName.compare("Fill")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "fill", fill);
      }
    }
    
    if( nodeName.compare("Width")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "width", width);
      }
    }
    
    if( nodeName.compare("Precision")==0 ){
      for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
        ifTheAttributeIsFoundAssign(ait, "precision", precision);
      }
    }

    std::cout <<"/>"<< std::endl;
  }

  //switch( FnvHash(notation) ){
  switch( FnvHash(notation.c_str()) ){
    case FnvHash("fixed"):
      stringFormat<<std::fixed;
      break;
    case FnvHash("scientific"):
      stringFormat<<std::scientific;
      break;
    default:
      std::cout<<"Error: Notation "<<notation<<std::endl;
      break;
  }
  stringFormat.fill(fill);
  stringFormat.width(width);
  stringFormat.precision(precision);


  
}


