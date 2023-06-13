#ifndef Data_hpp
#define Data_hpp

#include "database/data/SrcRow.hpp"
#include "database/frame/Table.hpp"
#include "database/frame/Deleted.hpp"

class Data {
public:
    Data(std::string_view filename)
    :   table(filename), deleted(filename.data()) {}

    void setName(uint32_t id, std::string_view name){
        __setName(table.row_data(id), name.substr(0, SrcRow::NAME_SIZE-1).data());
    }

    void setMagnet(uint32_t id, std::string_view magnet){
        if(magnet.starts_with("magnet:?xt=")){
            __setMagnet(table.row_data(id), magnet.substr(0, SrcRow::MAGNET_SIZE-1).data());
        }
    }

    void removeSrc(uint32_t id){
        deleted.insert(id);
    }

    uint32_t addSrc(std::string_view name, std::string_view magnet){
        //should do legal check
        uint32_t id = deleted.get();
        if(id){
            setName(id, name);
            setMagnet(id, magnet);
            deleted.erase(id);
        }else{
            id = table.getSum()+1;
            SrcRow temp(id, name, magnet);
            table.push_back(temp);
        }
        return id;
    }

    uint32_t    getRowNum() { return table.getSum();}

    uint32_t    getID(void *cur)        { return SrcRow(cur).getID();}
    char*       getName(void *cur)      { return SrcRow(cur).getName();}
    char*       getMagnet(void *cur)    { return SrcRow(cur).getMagnet();}
    void __setName(void* cur, const char* name)   { SrcRow temp; temp.deserialize(cur); temp.setName(name); temp.serialize(cur); }
    void __setMagnet(void* cur, const char* mg)   { SrcRow temp; temp.deserialize(cur); temp.setMagnet(mg); temp.serialize(cur); }

    Deleted deleted;
    Table<SrcRow> table;
};

#endif