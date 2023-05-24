#include "Global.hpp"

unordered_map< string, string > Global::mapLabel;
string Global::imagePath="./../imagenes/";
string Global::imageName;


Global::Global()
{

}

void Global::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;
    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );


        if( nodeName.compare("Save")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "imagePath", Global::imagePath);
                ifTheAttributeIsFoundAssignString(ait, "imageName", Global::imageName);
            }
        }

        if( nodeName.compare("AddLabel")==0 ){
            string id,str;
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "id", id);
                ifTheAttributeIsFoundAssignString(ait, "str", str);
            }

            mapLabel.insert({ id,str});
        }
    }

}


//string Global::strFind(string value){
//    string strVal("Hola");

////    unordered_map<string,string>::const_iterator itLabel= Global::mapLabel.find (value);
////    if ( itLabel == Global::mapLabel.end() )
////        strVal=value;
////    else
////        strVal=itLabel->second;

////    return strVal;
////}

////string strFindqwert(string value){
////    string strVal;

////    unordered_map<string,string>::const_iterator itLabel= Global::mapLabel.find (value);
////    if ( itLabel == Global::mapLabel.end() )
////        strVal=value;
////    else
////        strVal=itLabel->second;

//    return strVal;
////        return value;
//}
//string ReplaceString(string subject, const string& search,const string& replace) {
//    size_t pos = 0;
//    while ((pos = subject.find(search, pos)) != string::npos) {
//         subject.replace(pos, search.length(), replace);
//         pos += replace.length();
//    }
//    return subject;
//}


template <typename T, typename U>
T &replace (
          T &str,
    const U &from,
    const U &to)
{
    size_t pos;
    size_t offset = 0;
    const size_t increment = to.size();

    while ((pos = str.find(from, offset)) != T::npos)
    {
        str.replace(pos, from.size(), to);
        offset = pos + increment;
    }

    return str;
}


string replaceGlobalId(string value){
    //unordered_map<string,string>::const_iterator itLabel= Global::mapLabel.find (value);
    //auto itLabel= Global::mapLabel.find (value);

    for (auto x : Global::mapLabel){
        size_t pos;
        size_t offset = 0;
        const size_t increment = x.second.size();
        //cout << x.first << " " << x.second << endl;

        while ((pos = value.find(x.first, offset)) != string::npos)
        {
            value.replace(pos, x.first.size(), x.second);
            offset = pos + increment;
        }

    }
    return value;



//    stringstream strVal;
//    if ( itLabel == Global::mapLabel.end() )
//        strVal<<value;
//    else
//        strVal<<itLabel->second;

//    return strVal;
    //return "Hola";
}


