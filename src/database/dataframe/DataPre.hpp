#ifndef DataPre_hpp
#define DataPre_hpp

#include "Cursor.hpp"

class DataPre : Cursor{
    DataPre(std::string_view filename)
    :   Cursor(filename) {}

    void setName(std::string_view name){
        Cursor::setName(name.substr(0,NAME_SIZE-1).data());
    }

    void setMagnet(std::string_view magnet){
        if(magnet.starts_with("magnet:?xt="sv)){
            Cursor::setMagnet(magnet.substr(0,MAGNET_SIZE-1).data());
        }
    }
}