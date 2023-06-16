#ifndef Data_hpp
#define Data_hpp

#include "database/data/SrcRow.hpp"
#include "database/frame/Table.hpp"
#include "database/frame/Deleted.hpp"

class Data {
public:
    Data(std::string_view filename)
    :   table(filename), deleted(filename.data()) {}

    void setName(unsigned int id, std::string_view name){
        __setName(table.row_data(id), name.substr(0, SrcRow::NAME_SIZE-1).data());
    }

    void setMagnet(unsigned int id, std::string_view magnet){
        if(magnet.starts_with("magnet:?xt=")){
            __setMagnet(table.row_data(id), magnet.substr(0, SrcRow::MAGNET_SIZE-1).data());
        }
    }

    void removeSrc(unsigned int id){
        deleted.insert(id);
    }

    unsigned int addSrc(std::string_view name, std::string_view magnet){
        //should do legal check
        unsigned int id = deleted.get();
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

    unsigned int    getRowNum() { return table.getSum();}

    unsigned int            getID(void *cur)        { return SrcRow(cur).getID();}
    std::string_view    getName(void *cur)      { return SrcRow(cur).getName();}
    std::string_view    getMagnet(void *cur)    { return SrcRow(cur).getMagnet();}
    void __setName(void* cur, const char* name)   { SrcRow temp; temp.deserialize(cur); temp.setName(name); temp.serialize(cur); }
    void __setMagnet(void* cur, const char* mg)   { SrcRow temp; temp.deserialize(cur); temp.setMagnet(mg); temp.serialize(cur); }

    Deleted deleted;
    Table<SrcRow> table;
};

#endif