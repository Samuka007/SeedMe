#ifndef UserPre_hpp
#define UserPre_hpp

#include "./Cursor.hpp"

class UserPre : Cursor{
    UserPre(std::string_view filename)
    :   Cursor(filename) {}

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
}