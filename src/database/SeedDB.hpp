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
using status = uint32_t;
using source = std::tuple<uint32_t, std::string, std::string>;
constexpr uint32_t SUCCESS = 200;
constexpr uint32_t NOT_FOUND = 404;
constexpr uint32_t CHANGE_ERROR = 110;
constexpr uint32_t CREATE_ERROR = 120;
constexpr uint32_t GET_SRC_ERROR = 210;
constexpr uint32_t GET_USR_ERROR = 220;
constexpr uint32_t FIND_DB_ERROR = 310;
constexpr uint32_t WRONG_PASSWORD = 400;

class SeedDB : public Data, public User, public Metadata {
    public:
        SeedDB(std::string filename)
        :   Data(filename + ".data"), 
            User(filename + ".user"),
            Metadata(filename + ".meta")
        {}
/*
        std::vector<uint32_t> find_src_by_name(std::string_view name){
            //使用正则表达式与分词
            bool a = true;
        }*/

        bool magnet_exist(std::string_view magnet){
            bool find = false;
            for(uint32_t i=1;i<Data::getRowNum();++i){
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
            for(uint32_t i=1;i<this->User::getRowNum();++i){
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
        pair<status, uint32_t> new_user(
            std::string_view username,
            std::string_view password
        ){
            if(username_exist(username)){
                //throw error
                return {CREATE_ERROR, NULL};
            }
            User::addUser(username, password);
        }

        pair<status, uint32_t> update_username(
            uint32_t userid,
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
        pair<status, uint32_t> update_password(
            uint32_t userid,
            std::string_view old_password,
            std::string_view new_password
        ){
            User::setPassword(userid, old_password, new_password);
            return {SUCCESS, userid};
        }
        
        pair<status,std::vector<uint32_t>> get_user_source_id(
            uint32_t userid
        ){
            return {SUCCESS, std::move(getUserSrc(Data::table.row_data(userid)))};
        }

        pair<status, pair<uint32_t, std::vector<source>>> get_sources_by_ids(
            vector<uint32_t> id_list
        ){
            std::vector<source> srcs;
            for(auto srcid : id_list){
                srcs.push_back(get_source(srcid).second);
            }
            return {SUCCESS, srcs};
        }

        
        
        pair<status, uint32_t> delete_user(
            uint32_t userid
        ){
            User::deleteUser(userid);
            return {SUCCESS, userid};
        }
        

        /**
         * SourceController:
        */
        pair<status, uint32_t> create_source(
            std::string_view SrcName,
            std::string_view SrcMagnet
        ){
            uint32_t srcid = addSrc(SrcName, SrcMagnet);
            Metadata::Log(srcid, SrcName);
            return {SUCCESS, srcid};
        }

        pair<status, source> get_source(
            uint32_t srcid
        ){
            if(srcid > Data::getRowNum())      { return {NOT_FOUND, {}};}
            if(Data::deleted.contains(srcid))  { return {NOT_FOUND, {}};}
            void* cur = Data::table.row_data(srcid);
            return {SUCCESS, {srcid, Data::getName(cur), Data::getMagnet(cur)}};
        }

        pair<status, uint32_t> update_source(
            uint32_t id,
            std::string_view SrcName,
            std::string_view SrcMagnet
        ){
            Data::setName(id, SrcName);
            Data::setMagnet(id, SrcMagnet);
            Metadata::Delete(id);
            Metadata::Log(id, SrcName);
            if(Data::getName(id) != SrcName || Data::getMagnet(id) != SrcMagnet){
                return {CHANGE_ERROR, id};
            }
            return {SUCCESS, id};
        }

        pair<status, uint32_t> delete_source(
            uint32_t id
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