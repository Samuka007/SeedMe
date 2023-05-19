#ifndef DataPre_hpp
#define DataPre_hpp

#include "./Cursor.hpp"
#include "../metadata/Deleted.hpp"

class DataPre : Cursor, Deleted{
    DataPre(std::string_view filename)
    :   Cursor(filename), Deleted(filename) {}

    void setName(std::string_view name){
        Cursor::setName(name.substr(0,NAME_SIZE-1).data());
    }

    void setMagnet(std::string_view magnet){
        if(magnet.starts_with("magnet:?xt="sv)){
            Cursor::setMagnet(magnet.substr(0,MAGNET_SIZE-1).data());
        }
    }

    void removeSrc(uint32_t id){
        Deleted::insert(id);
    }

    uint32_t addSrc(std::string_view name, std::string_view magnet){
        //should do legal check
        uint32_t id = Deleted::get();
        if(id){
            Row temp(id, name, magnet);
            temp.serialize(Table::row_slot(id));
            Deleted::erase(id);
        }else{
            id = Table::getSum()+1;
            Row temp(id, name, magnet);
            Table::push_back(temp);
        }
        return id;
    }
}