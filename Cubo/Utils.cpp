#include "Utils.hpp"


void cleanString( string &field){
    field.erase(std::remove(field.begin(), field.end(), '\r'), field.end());
    field.erase(std::remove(field.begin(), field.end(), '\n'), field.end());
}

void toLowerString( string &str){
    std::for_each(str.begin(), str.end(), [](char & c){
        c = ::tolower(c);
    });
}
