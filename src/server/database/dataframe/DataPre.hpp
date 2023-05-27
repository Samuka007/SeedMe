#ifndef DataPre_hpp
#define DataPre_hpp

#include "./SrcRow.hpp"
#include "../frame/Table.hpp"
#include "../frame/Deleted.hpp"

class DataPre : public Table<SrcRow>, public Deleted{
public:
    DataPre(std::string_view filename)
    :   Table<SrcRow>(filename), Deleted(filename) {}

    void setName(std::string_view name){
        __setName(name.substr(0, SrcRow::NAME_SIZE-1).data());
    }

    void setMagnet(std::string_view magnet){
        if(magnet.starts_with("magnet:?xt="sv)){
            __setMagnet(magnet.substr(0, SrcRow::MAGNET_SIZE-1).data());
        }
    }

    void removeSrc(uint32_t id){
        Deleted::insert(id);
    }

    uint32_t addSrc(std::string_view name, std::string_view magnet){
        //should do legal check
        uint32_t id = Deleted::get();
        if(id){
            SrcRow temp(id, name, magnet);
            temp.serialize(Table::row_slot(id));
            Deleted::erase(id);
        }else{
            id = Table::getSum()+1;
            SrcRow temp(id, name, magnet);
            Table::push_back(temp);
        }
        return id;
    }
    
    uint32_t    getID(void* cur)        { SrcRow temp; temp.deserialize(cur); return temp.getID();}
    char*       getName(void* cur)      { SrcRow temp; temp.deserialize(cur); return temp.getName();}
    char*       getMagnet(void* cur)    { SrcRow temp; temp.deserialize(cur); return temp.getMagnet();}

    void __setName(void* cur, char* name)   { SrcRow temp; temp.deserialize(cur); temp.setName(name); temp.serialize(cur); }
    void __setMagnet(void* cur, char* mg)   { SrcRow temp; temp.deserialize(cur); temp.setMagnet(mg); temp.serialize(cur); }
}