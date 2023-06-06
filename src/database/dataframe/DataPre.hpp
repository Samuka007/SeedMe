#ifndef DataPre_hpp
#define DataPre_hpp

#include "./SrcRow.hpp"
#include "../frame/Table.hpp"
#include "../frame/Deleted.hpp"

class DataPre : public Table<SrcRow>, public Deleted{
public:
    DataPre(std::string_view filename)
    :   Table<SrcRow>(filename), Deleted(filename.data()) {}

    void setName(uint32_t id, std::string_view name){
        __setName(row_data(id), name.substr(0, Src::NAME_SIZE-1).data());
    }

    void setMagnet(uint32_t id, std::string_view magnet){
        if(magnet.starts_with("magnet:?xt=")){
            __setMagnet(row_data(id), magnet.substr(0, Src::MAGNET_SIZE-1).data());
        }
    }

    void removeSrc(uint32_t id){
        Deleted::insert(id);
    }

    uint32_t addSrc(std::string_view name, std::string_view magnet){
        //should do legal check
        uint32_t id = Deleted::get();
        if(id){
            setName(id, name);
            setMagnet(id, magnet);
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

    void __setName(void* cur, const char* name)   { SrcRow temp; temp.deserialize(cur); temp.setName(name); temp.serialize(cur); }
    void __setMagnet(void* cur, const char* mg)   { SrcRow temp; temp.deserialize(cur); temp.setMagnet(mg); temp.serialize(cur); }
};

#endif