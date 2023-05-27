#ifndef UserPre_hpp
#define UserPre_hpp

#include "./UserRow.hpp"
#include "../frame/Table.hpp"
#include "../frame/Deleted.hpp"

class UserPre : Table<UserRow>, Deleted{
    UserPre(std::string_view filename)
    :   Table<UserRow>(filename), Deleted(filename) {}

    void setUsername(std::string_view name){
        __setName(name.substr(0, UserRow::NAME_SIZE-1).data());
    }

    void setPassword(std::string_view origin_pw, 
        std::string_view change_pw){
        if(origin_pw.data() == getPassword()){
            __setPassword(change_pw.data());
        }else{
            //error;
        }
    }

    void deleteUser(uint32_t id){
        Deleted::insert(id);
    }

    uint32_t addUser(std::string_view username, std::string_view password){
        //should do legal check
        uint32_t id = Deleted::get();
        if(id){
            UserRow temp(id, username, password);
            temp.serialize(Table::row_slot(id));
            Deleted::erase(id);
        }else{
            id = Table::getSum()+1;
            UserRow temp(id, username, password);
            Table::push_back(temp);
        }
        return id;
    }

    uint32_t    getUserID(void* cur)     { UserRow temp; temp.deserialize(cur); return temp.getID();}
    char*       getUsername(void* cur)       { UserRow temp; temp.deserialize(cur); return temp.getName();}
    char*       getPassword(void* cur)   { UserRow temp; temp.deserialize(cur); return temp.getPassword();}

    void __setName(void* cur, char* name)    { UserRow temp; temp.deserialize(cur); temp.setName(name); temp.serialize(cur); }
    void __setPassword(void* cur, char* pw)  { UserRow temp; temp.deserialize(cur); temp.setPassword(pw); temp.serialize(cur); }
}