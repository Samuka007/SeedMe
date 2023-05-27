#ifndef SeedDB_hpp
#define SeedDB_hpp

#include <vector>

#include "./dataframe/DataPre.hpp"
#include "./userframe/UserPre.hpp"
#include "./frame/Metadata.hpp"
#include "./Search.hpp"

class SeedDB : public DataPre, public UserPre, public Metadata {
    public:
        SeedDB(std::string filename)
        :   DataPre(filename+".data"s), 
            UserPre(filename+".user"s),
            Metadata(filename+".meta"s)
        {}

        std::vector<uint32_t> src_name(std::string_view name){
            //使用正则表达式与分词
            bool a = true;
        }

        bool magnet_exist(std::string_view magnet){
            bool find = false;
            for(uint32_t i=1;i<this->getSum();++i){
                if(this->Deleted::contains(i))
                    continue;
                if(DataPre::getMagnet(this->row_data(i)) == magnet){
                    find = true;
                    break;
                }
            }
            return find;
        }

        bool username_exist(std::string_view username){
            bool find = false;
            for(uint32_t i=1;i<this->getSum();++i){
                if(this->Deleted::contains(i))
                    continue;
                if(DataPre::getUsername(this->row_data(i)) == username){
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
            std::string_view password,
            std::string_view password_varified
        ){
            if(username_exist(username)){
                //throw error
                return;
            }
            UserPre::addUser(username, password);
        }

        void UpdateUsername(
            uint32_t userid,
            std::string_view username
        ){
            UserPre::setUsername(userid, username);
        }

        void GetUserSource();
        void DeleteUser();
        
        /**
         * SourceController:
        */
        void CreateSource();
        void GetSource();
        void UpdateSource();
        void DeleteSource();



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