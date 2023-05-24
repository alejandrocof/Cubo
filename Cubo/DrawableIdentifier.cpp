
#include "DrawableIdentifier.hpp"

//unordered_map< string, void* > DrawableIdentifier::mapId;
unordered_map< string, classType > DrawableIdentifier::mapId2;

/*
void DrawableIdentifier::insert( std::string name, const void* drawable){
  std::pair< std::string, const void*> myDataId (name,drawable);
  mapId.insert(myDataId);
 // mapId.insert( {name,drawable} );
};

*/
