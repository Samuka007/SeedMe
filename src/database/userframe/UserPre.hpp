#ifndef UserPre_hpp
#define UserPre_hpp

#include <vector>
#include "database/userframe/UserRow.hpp"
#include "database/frame/Table.hpp"
#include "database/frame/Deleted.hpp"

class UserPre : Table<UserRow>, Deleted{
public:
    UserPre(std::string_view filename)
    :   Table<UserRow>(filename), Deleted(filename.data()) {}

    void setUsername(
        uint32_t id, 
        std::string_view name
    ){
        __setName(Table::row_data(id), name.substr(0, User::NAME_SIZE-1).data());
    }

    void setPassword(
        uint32_t id,
        std::string_view origin_pw, 
        std::string_view change_pw
    ){
        void* row_cur = Table::row_data(id);
        if(origin_pw.data() == getPassword(row_cur)){
            __setPassword(row_cur, change_pw.data());
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
            temp.serialize(Table::row_data(id));
            Deleted::erase(id);
        }else{
            id = Table::getSum()+1;
            UserRow temp(id, username, password);
            Table::push_back(temp);
        }
        return id;
    }

    void addSrcID(uint32_t SrcID, uint32_t UserID){
        void* cur = Table::row_data(UserID);
        UserRow temp;
        temp.deserialize(cur);
        uint32_t* src = temp.getSource();
        for(uint32_t i = 0; i<User::MAX_OWN_SOURCE; ++i){
            if(src[i] != 0){
                src[i] = SrcID;
                break;
            }
        }
        temp.setSource(src);
        temp.serialize(cur);
    }

    uint32_t    getUserID(void* cur)     { UserRow temp; temp.deserialize(cur); return temp.getID();}
    char*       getUsername(void* cur)       { UserRow temp; temp.deserialize(cur); return temp.getName();}
    char*       getPassword(void* cur)   { UserRow temp; temp.deserialize(cur); return temp.getPassword();}

    std::vector<uint32_t>& getUserSrc(void* cur) {
        std::vector<uint32_t> Usersrc;
        UserRow temp;
        temp.deserialize(cur);
        for(uint32_t i=0;i<User::MAX_OWN_SOURCE; ++i){
            if(!(temp.getSource()[i])){
                break;
            }
            Usersrc.push_back(temp.getSource()[i]);
        }
        return Usersrc;
    }

    void __setName(void* cur, const char* name)    { UserRow temp; temp.deserialize(cur); temp.setName(name); temp.serialize(cur); }
    void __setPassword(void* cur, const char* pw)  { UserRow temp; temp.deserialize(cur); temp.setPassword(pw); temp.serialize(cur); }
};

#endif