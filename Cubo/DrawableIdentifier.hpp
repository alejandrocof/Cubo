#ifndef DRAWABLE_IDENTIFIER_HPP
#define DRAWABLE_IDENTIFIER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <GL/glut.h>
#include <typeinfo>

using namespace std;
typedef struct classType{
    void *pointer;
    string name;
    classType(void *_pointer, string _name):pointer{_pointer},name{_name}{}
}classType;

class DrawableIdentifier{

public:
    //inline GLenum newId(){return account++;};
    //void deleteTexture(int _id);
    //void insert(std::string name, const void* drawable);
    //inline void insert(std::string name, void* drawable){ mapId.insert( std::make_pair<std::string,void*>("hola",drawable) ); };
    
    template<typename T>
    static void insert(string name,T drawable){
        //classType obj;
                //std::pair< std::string, const void*> myDataId (name,static_cast<const void*>(&drawable) );
                //mapId.insert(myDataId);
                cout<<"direccion en insert:"<<drawable<<endl;
        cout<<"insert Id="<<name<<" de tipo:"<<typeid(drawable).name()<<endl;

        //mapId.insert( { name, static_cast< void* >( drawable ) } );

        //classType xxx(static_cast< void* >( drawable ), typeid(drawable).name() );
        //mapId2.insert( { name, xxx }  );
        mapId2.insert( { name, classType(static_cast< void* >( drawable ), typeid(drawable).name() ) }  );
    }
    
/*
    template<typename T>
    static T data(std::string name){
      std::unordered_map< std::string, const void* >::const_iterator it = mapId.find(name);
      if ( it == mapId.end() ){
        std::cout << "Id: "<< name <<" not found"<<std::endl;
        //return 0;
      }
      
      //else{
      //  std::cout<<"get Id="<<it->first<<std::endl;
      //  return static_cast<T>(it->second);
      //}
      std::cout<<"get Id="<<it->first<<std::endl;
      return static_cast<T>(it->second);
    };
    
  */  
    
    
    
    template<typename T>
    static bool data(string name, T &d){
//      unordered_map< string, void* >::const_iterator it = mapId.find(name);
//      if ( it == mapId.end() ){
//        cout << "Id: "<< name <<" not found"<<endl;
//        //return 0;
//      }
      
//      //else{
//      //  std::cout<<"get Id="<<it->first<<std::endl;
//      //  return static_cast<T>(it->second);
//      //}
//      cout<<"direccion en data:"<<it->second<<endl;
//      cout<<"get Id="<<it->first<<endl;
//      cout<<"*get Id="<<name<<" de tipo:"<<typeid(d).name()<<" - "<<typeid(it->second).name()<<endl;

//      d=static_cast<T>(it->second);

      unordered_map< string, classType >::const_iterator it = mapId2.find(name);
      if ( it == mapId2.end() ){
        cout << "Id: "<< name <<" not found"<<endl;
        //return 0;
      }

      //else{
      //  std::cout<<"get Id="<<it->first<<std::endl;
      //  return static_cast<T>(it->second);
      //}
      cout<<"direccion en data:"<<it->second.pointer<<endl;
      cout<<"get Id="<<it->first<<endl;
      cout<<"*get Id="<<name<<" de tipo:"<<typeid(d).name()<<" - "<<it->second.name<<endl;
      if(typeid(d).name()==it->second.name){
          d=static_cast<T>(it->second.pointer);
          return true;
      }
      return false;
    }
    
    
    /*
    static const void *data(std::string name){
      std::unordered_map< std::string, const void* >::const_iterator it = mapId.find(name);
      if ( it == mapId.end() ){
        std::cout << "Id: "<< name <<" not found"<<std::endl;
        //return 0;
      }
      
      //else{
      //  std::cout<<"get Id="<<it->first<<std::endl;
      //  return static_cast<T>(it->second);
      //}
      return static_cast<const void*>(it->second);
    };*/
/*
    static const void* data(std::string name){
      std::unordered_map< std::string, const void* >::const_iterator it = mapId.find(name);
      if ( it == mapId.end() ){
        std::cout << "Id: "<< name <<" not found"<<std::endl;
        return 0;
      }
      else{
        std::cout<<"get Id="<<it->first<<std::endl;
        return it->second;
      }
    };
  */  
  private:

    //static unordered_map< string, void* > mapId;
    static unordered_map< string, classType > mapId2;
};

#endif

/*
//http://stackoverflow.com/questions/4480788/c-c-switch-case-with-string
void loadCSV(std::istringstream &iss) {
  std::string s;
  iss>>s;
  std::cout<<"LoadCSV "<<s<<std::endl;
}

std::unordered_map< std::string, std::function<void(std::istringstream &)> > hash_map={
  {"LoadCSV", &loadCSV}
};
*/
