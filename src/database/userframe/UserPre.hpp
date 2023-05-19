#ifndef UserPre_hpp
#define UserPre_hpp

#include "./Cursor.hpp"
#include "../metadata/Deleted.hpp"

class UserPre : Cursor, Deleted{
    UserPre(std::string_view filename)
    :   Cursor(filename), Deleted(filename) {}

    void setUsername(std::string_view name){
        Cursor::set(name.substr(0,NAME_SIZE-1).data());
    }

    void setPassword(std::string_view origin_pw, 
        std::string_view change_pw){
        if(origin_pw.data() == getPassword()){
            Cursor::setPassword(change_pw.data());
        }else{
            throw std::bad_exception "Password check failed.";
        }
    }

    void deleteUser(uint32_t id){
        Deleted::insert(id);
    }

    uint32_t addUser(std::string_view username, std::string_view password){
        //should do legal check
        uint32_t id = Deleted::get();
        if(id){
            Row temp(id,username,password);
            temp.serialize(Table::row_slot(id));
            Deleted::erase(id);
        }else{
            id = Table::getSum()+1;
            Row temp(id,username,password);
            Table::push_back(temp);
        }
        return id;
    }
}