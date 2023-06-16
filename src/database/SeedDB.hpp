#ifndef SeedDB_hpp
#define SeedDB_hpp

#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include "database/data/Data.hpp"
#include "database/user/User.hpp"
#include "database/frame/Metadata.hpp"
using std::pair;
using status = unsigned int;
using source = std::tuple<unsigned int, std::string, std::string>;
constexpr unsigned int SUCCESS = 200;
constexpr unsigned int NOT_FOUND = 404;
constexpr unsigned int CHANGE_ERROR = 110;
constexpr unsigned int CREATE_ERROR = 120;
constexpr unsigned int GET_SRC_ERROR = 210;
constexpr unsigned int GET_USR_ERROR = 220;
constexpr unsigned int FIND_DB_ERROR = 310;
constexpr unsigned int WRONG_PASSWORD = 400;

class SeedDB : public Data, public User, public Metadata {
    public:
        SeedDB(std::string filename)
        :   Data(filename + ".data"), 
            User(filename + ".user"),
            Metadata(filename + ".meta")
        {}
/*
        std::vector<unsigned int> find_src_by_name(std::string_view name){
            //使用正则表达式与分词
            bool a = true;
        }*/

        bool magnet_exist(std::string_view magnet){
            bool find = false;
            for(unsigned int i=1;i<Data::getRowNum();++i){
                if(this -> Data::deleted.contains(i))
                    continue;
                if(Data::getMagnet(Data::table.row_data(i)) == magnet){
                    find = true;
                    break;
                }
            }
            return find;
        }

        bool username_exist(std::string_view username){
            bool find = false;
            for(unsigned int i=1;i<this->User::getRowNum();++i){
                if(this->User::deleted.contains(i))
                    continue;
                if(User::getUsername(User::table.row_data(i)) == username){
                    find = true;
                    break;
                }
            }
            return find;
        }

        /**
         * UserController
        */
        pair<status, unsigned int> new_user(
            std::string_view username,
            std::string_view password
        ){
            if(username_exist(username)){
                //throw error
                return {CREATE_ERROR, NULL};
            }
            User::addUser(username, password);
        }

        pair<status, unsigned int> update_username(
            unsigned int userid,
            std::string_view username,
            std::string_view password
        ){
            if(password != User::getPassword(User::table.row_data(userid))){
                return {WRONG_PASSWORD, userid};
            }
            User::setUsername(userid, username);
            return {SUCCESS, userid};
        }

        //TODO: update password
        pair<status, unsigned int> update_password(
            unsigned int userid,
            std::string_view old_password,
            std::string_view new_password
        ){
            User::setPassword(userid, old_password, new_password);
            return {SUCCESS, userid};
        }
        
        pair<status,std::vector<unsigned int>> get_user_source_id(
            unsigned int userid
        ){
            return {SUCCESS, std::move(getUserSrc(Data::table.row_data(userid)))};
        }

        pair<status, std::vector<source>> get_sources_by_ids(
            std::vector<unsigned int> id_list
        ){
            std::vector<source> srcs;
            for(auto srcid : id_list){
                srcs.push_back(get_source(srcid).second);
            }
            return {SUCCESS, srcs};
        }

        
        
        pair<status, unsigned int> delete_user(
            unsigned int userid
        ){
            User::deleteUser(userid);
            return {SUCCESS, userid};
        }
        

        /**
         * SourceController:
        */
        pair<status, unsigned int> create_source(
            std::string_view SrcName,
            std::string_view SrcMagnet
        ){
            unsigned int srcid = addSrc(SrcName, SrcMagnet);
            Metadata::Log(srcid, SrcName);
            return {SUCCESS, srcid};
        }

        pair<status, source> get_source(
            unsigned int srcid
        ){
            if(srcid > Data::getRowNum())      { return {NOT_FOUND, {}};}
            if(Data::deleted.contains(srcid))  { return {NOT_FOUND, {}};}
            void* cur = Data::table.row_data(srcid);
            return {SUCCESS, {srcid, Data::getName(cur).data(), Data::getMagnet(cur).data()}};
        }

        pair<status, unsigned int> update_source(
            unsigned int id,
            std::string_view SrcName,
            std::string_view SrcMagnet
        ){
            Data::setName(id, SrcName);
            Data::setMagnet(id, SrcMagnet);
            Metadata::Delete(id);
            Metadata::Log(id, SrcName);
            //void* cur = Data::table.row_data(id);
            //if(Data::getName(cur) != SrcName || Data::getMagnet(cur) != SrcMagnet){
            //    return {CHANGE_ERROR, id};
            //}
            return {SUCCESS, id};
        }

        pair<status, unsigned int> delete_source(
            unsigned int id
        ){
            Data::deleted.insert(id);
            Metadata::Delete(id);
            return {SUCCESS, id};
        }



        /**--finished
        * store-
        * read-
        * generate -
        * change metadata-
        * 
        * log metadata and store source-
        * 
        */

        /*
        *   two filters:
        *       - userid                    read    write
        *           - 0 = visitor           all     0
        *           - 1 = suporior          all     all
        *           - 100+ = user           all     1
        *       - tags(metadatas)
        */
};
#endif