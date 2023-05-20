#ifndef Search_hpp
#define Search_hpp

#include <vector>
#include "./dataframe/DataPre.hpp"
#include "./userframe/UserPre.hpp"

namespace Search{
    std::vector<uint32_t> src_name(Table<SrcRow> *&table, std::string_view name){
        //使用正则表达式与分词
    }

    bool magnet_exist(DataPre *&database, std::string_view magnet){
        bool find = false;
        for(uint32_t i=1;i<database->getSum();++i){
            if(database->Deleted::contains(i))
                continue;
            if(DataPre::getMagnet(database->row_data(i)) == magnet){
                find = true;
                break;
            }
        }
        return find;
    }

    bool username_exist(UserPre *&userbase, std::string_view username){
        bool find = false;
        for(uint32_t i=1;i<userbase->getSum();++i){
            if(database->Deleted::contains(i))
                continue;
            if(DataPre::getUsername(database->row_data(i)) == username){
                find = true;
                break;
            }
        }
        return find;
    }
};

#endif