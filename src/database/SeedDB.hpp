#ifndef SeedDB_hpp
#define SeedDB_hpp

#include <vector>
#include <string>
#include "database/data/Data.hpp"
#include "database/user/User.hpp"
#include "database/frame/Metadata.hpp"

namespace Seeddb {

class SeedDB : public Data, public User, public Metadata {
    public:
        SeedDB(std::string filename)
        :   Data(filename + ".data"), 
            User(filename + ".user"),
            Metadata(filename + ".meta")
        {}

        std::vector<uint32_t> src_name(std::string_view name){
            //使用正则表达式与分词
            bool a = true;
        }

        bool magnet_exist(std::string_view magnet){
            bool find = false;
            for(uint32_t i=1;i<Data::getRowNum();++i){
                if(this -> Data::deleted.contains(i))
                    continue;
                if(Data::getMagnet(Src::deserialize(Data::table.row_data(i))) == magnet){
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
        void CreateUser(
            std::string_view username,
            std::string_view password
        ){
            if(username_exist(username)){
                //throw error
                return;
            }
            User::addUser(username, password);
        }

        void UpdateUsername(
            uint32_t userid,
            std::string_view username
        ){
            User::setUsername(userid, username);
        }
        
        std::vector<uint32_t> GetUserSource(
            uint32_t userid
        ){
            return getUserSrc(Data::table.row_data(userid));
        }
        
        void DeleteUser(
            uint32_t userid
        ){
            User::deleteUser(userid);
        }
        

        /**
         * SourceController:
        */
        void CreateSource(
            std::string_view SrcName,
            std::string_view SrcMagnet
        ){
            Metadata::Log(addSrc(SrcName, SrcMagnet), SrcName);
        }

        std::string_view GetSource(
            uint32_t id
        ){
            return Data::getMagnet(Data::deserialize(Data::table.row_data(id)));
        }

        void UpdateSource(
            uint32_t id,
            std::string_view SrcName,
            std::string_view SrcMagnet
        ){
            setName(id, SrcName);
            setMagnet(id, SrcMagnet);
            Matadata::Delete(id);
            Metadata::Log(id, SrcName);
        }

        void DeleteSource(
            uint32_t id
        ){
            Deleted::insert(id);
            Metadata::Delete(id);
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