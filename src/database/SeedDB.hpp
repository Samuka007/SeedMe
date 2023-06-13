#ifndef SeedDB_hpp
#define SeedDB_hpp

#include <vector>
#include <string>
#include <pair>
#include "database/data/Data.hpp"
#include "database/user/User.hpp"
#include "database/frame/Metadata.hpp"
using std::pair;
using status = uint32_t;
constexpr SUCCESS 100;
constexpr CHANGE_ERROR 110;
constexpr CREATE_ERROR 120;
constexpr GET_SRC_ERROR 210;
constexpr GET_USR_ERROR 220;
constexpr FIND_DB_ERROR 310;

namespace Seeddb {

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
        pair<uint32_t, status> CreateUser(
            std::string_view username,
            std::string_view password
        ){
            if(username_exist(username)){
                //throw error
                return;
            }
            User::addUser(username, password);
        }

        pair<status, uint32_t> UpdateUsername(
            uint32_t userid,
            std::string_view username
        ){
            User::setUsername(userid, username);
            return std::make_pair(SUCCESS, userid);
        }
        
        pair<status,std::vector<uint32_t>> GetUserSource(
            uint32_t userid
        ){
            return std::make_pair(SUCCESS, std::move(getUserSrc(Data::table.row_data(userid))));
        }
        
        pair<status, uint32_t> DeleteUser(
            uint32_t userid
        ){
            User::deleteUser(userid);
            return std::make_pair(SUCCESS, userid);
        }
        

        /**
         * SourceController:
        */
        pair<status, uint32_t> CreateSource(
            std::string_view SrcName,
            std::string_view SrcMagnet
        ){
            uint32_t srcid = addSrc(SrcName, SrcMagnet);
            Metadata::Log(srcid, SrcName);
            return std::make_pair(SUCCESS, srcid);
        }

        pair<status, std::string> GetSource(
            uint32_t id
        ){
            return std::make_pair(SUCCESS, Data::getMagnet(Data::table.row_data(id)));
        }

        pair<status, uint32_t> UpdateSource(
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

        pair<status, uint32_t> DeleteSource(
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

}
#endif