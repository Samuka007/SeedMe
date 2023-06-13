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
constexpr uint32_t SUCCESS = 100;
constexpr uint32_t NOTFOUND = 200;
constexpr uint32_t CHANGE_ERROR = 110;
constexpr uint32_t CREATE_ERROR = 120;
constexpr uint32_t GET_SRC_ERROR = 210;
constexpr uint32_t GET_USR_ERROR = 220;
constexpr uint32_t FIND_DB_ERROR = 310;

class SeedDB : public Data, public User, public Metadata {
    public:
        SeedDB(std::string filename)
        :   Data(filename + ".data"), 
            User(filename + ".user"),
            Metadata(filename + ".meta")
        {}
/*
        std::vector<uint32_t> src_name(std::string_view name){
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
                return std::make_pair(CREATE_ERROR, NULL);
            }
            User::addUser(username, password);
        }

        pair<status, uint32_t> update_username(
            uint32_t userid,
            std::string_view username
        ){
            User::setUsername(userid, username);
            return std::make_pair(SUCCESS, userid);
        }
        
        pair<status,std::vector<uint32_t>> get_user_source(
            uint32_t userid
        ){
            return std::make_pair(SUCCESS, std::move(getUserSrc(Data::table.row_data(userid))));
        }
        
        pair<status, uint32_t> delete_user(
            uint32_t userid
        ){
            User::deleteUser(userid);
            return std::make_pair(SUCCESS, userid);
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
            return std::make_pair(SUCCESS, srcid);
        }

        pair<status, std::tuple<std::string_view, std::string_view>> get_source(
            uint32_t id
        ){
            if(id > Data::getRowNum())      { return std::make_pair(NOTFOUND, std::tuple(NULL,NULL));}
            if(Data::deleted.contains(id))  { return std::make_pair(NOTFOUND, std::tuple(NULL,NULL));}
            void* cur = Data::table.row_data(id);
            auto src = std::make_tuple(Data::getName(cur), Data::getMagnet(cur));
            return std::make_pair(SUCCESS, src);
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
            return std::make_pair(SUCCESS, id);
        }

        pair<status, uint32_t> delete_source(
            uint32_t id
        ){
            Data::deleted.insert(id);
            Metadata::Delete(id);
            return std::make_pair(SUCCESS, id);
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