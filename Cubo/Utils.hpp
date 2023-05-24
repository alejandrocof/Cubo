#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>


using namespace std;

class FnvHash{
    static const unsigned int FNV_PRIME = 16777619u;
    static const unsigned int OFFSET_BASIS = 2166136261u;
  
    template <unsigned int N>
    static constexpr unsigned int fnvHashConst(const char (&str)[N], unsigned int I = N){
      return I == 1 ? (OFFSET_BASIS ^ str[0]) * FNV_PRIME : (fnvHashConst(str, I - 1) ^ str[I - 1]) * FNV_PRIME;
    }
  
    static unsigned int fnvHash(const char* str){
      //const size_t length = strlen(str) + 1;
      const size_t length = std::char_traits<char>::length(str) + 1;
      unsigned int hash = OFFSET_BASIS;
      for (size_t i = 0; i < length; ++i){
        hash ^= *str++;
        hash *= FNV_PRIME;
      }
      return hash;
    }
  
    struct Wrapper{
      Wrapper(const char* str) : str (str) { }
      const char* str;
    };

    unsigned int hash_value;
    
  public:
  
    // calulate in run-time
    FnvHash(Wrapper wrapper) : hash_value(fnvHash(wrapper.str)) { }
    
    // calulate in compile-time
    template <unsigned int N>
    constexpr FnvHash(const char (&str)[N]) : hash_value(fnvHashConst(str)) { }
    
    // output result
    constexpr operator unsigned int() const { return this->hash_value; }
};

void cleanString( string &field);
void toLowerString( string &str);

//template<typename T> int readLine(string line, T &x, string name1 , string name2="");
template<typename T>
int readLine(string line, T &x , string name1 , string name2=""){
    //cleanString(line);
    stringstream sline=stringstream(line);
    string field,val;
    int id=0;

    getline(sline, field,' ');
    toLowerString(field);
    toLowerString(name1);



    if(field==name1){
        id=1;
    }
    else{
        if(name2==""){
            std::cout<<"Error en la lectura del archivo, se esperaba \""<<name1<<"\" y se encontró:"<<field<<std::endl;
            return id;
        }

        if(field==name2){
            id=2;
        }
        else{
            std::cout<<"Error en la lectura del archivo, se esperaba \""<<name1<<"\" o \""<<name2<<"\" y se encontró:"<<field<<std::endl;
            return id;
        }
    }

    getline(sline, val);
    stringstream sval=stringstream(val);
    sval>>x;
    if(sval.fail()){
        std::cout<<"Error el tipo de dato esperado era "<<typeid(x).name()<<" para \""<<field<<"\": \""<<val<<"\""<<std::endl;
        return id;
    }
    std::cout<<"line="<<sline.str()<<" field="<<field<<" val="<<val<<" -> "<<x<<std::endl;
    return id;
}




#endif
