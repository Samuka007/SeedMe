#ifndef SeedDB_hpp
#define SeedDB_hpp

#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include "encrypt/md5.h"
#include "database/service/Data.hpp"
#include "database/service/User.hpp"
#include "database/util/Metadata.hpp"

using std::pair;
using status = size_t;
struct source_t {unsigned srcid; std::string srcname; std::string magnet; unsigned owner;};
struct user_t   {std::string usrname; std::string password};

constexpr unsigned int OK = 200;
constexpr unsigned int NOT_FOUND = 404;

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
                if(Data::deleted.contains(i))
                    continue;
                if(Data::getMagnet(i) == magnet){
                    find = true;
                    break;
                }
            }
            return find;
        }

        bool username_exist(std::string_view username){
            bool find = false;
            for(unsigned int i=1;i<this->User::get_last_user();++i){
                if(User::deleted.contains(i))
                    continue;
                if(User::getUsername(User::table[i]) == username){
                    find = true;
                    break;
                }
            }
            return find;
        }

        /**
         * UserController
        */
        
        unsigned login (user_t user) {
            for(unsigned int id=1; id<this->User::get_last_user(); ++id){
                if(User::deleted.contains(id))
                    continue;
                if(User::getUsername(User::table[id]) == user.usrname){
                    if(MD5(User::getPassword(User::table[id])).toStr()
                    == user.password){
                        return id;
                    }
                    else{
                        throw std::invalid_argument("wrong password");
                    }
                }
            }
        }

        inline unsigned new_user (user_t user) {
            if(username_exist(user.usrname)) {
                throw std::invalid_argument("username already exist");
            }
            return User::addUser(user.usrname, user.password);
        }

        inline void update_username(unsigned usrid, std::string_view username_new){
            User::setUsername(usrid, username_new);
        }

        //TODO: update password
        inline void update_password(
            unsigned userid,
            std::string_view old_password,
            std::string_view new_password
        ){
            User::setPassword(userid, MD5(old_password).toStr(), MD5(new_password).toStr());
        }

        std::vector<source_t>& get_sources_by_ids(
            std::vector<unsigned int> id_list
        ){
            std::vector<source_t> srcs;
            for(auto srcid : id_list){
                srcs.push_back(get_source(srcid));
            }
            return srcs;
        }
        
        void delete_user (unsigned int userid) {
            User::deleteUser(userid);
        }

        /**
         * SourceController:
        */
        unsigned int create_source(
            std::string_view SrcName,
            std::string_view SrcMagnet,
            unsigned owner
        ){
            unsigned srcid = addSrc(SrcName, SrcMagnet, owner);
            Metadata::Log(srcid, SrcName.data());
            return srcid;
        }

        source_t& get_source (unsigned srcid){

            if(srcid > Data::getRowNum())      { throw std::invalid_argument("id out of range"); }
            if(Data::deleted.contains(srcid))  { throw std::invalid_argument("id deleted");}

            return source_t {
                srcid,
                Data::getName(srcid),
                Data::getMagnet(srcid),
                Data::getOwner(srcid)
            };
        }

        void update_src_name(
            unsigned int id,
            std::string_view SrcName
        ){

            if(Data::deleted.contains(id))  { throw std::invalid_argument("id deleted"); }
            

            Data::setName(id, SrcName);
            Metadata::Delete(id);
            Metadata::Log(id, SrcName.data());
        }
        
        void update_src_mag(
            unsigned int id,
            std::string_view SrcMagnet
        ){

            if(Data::deleted.contains(id))  { throw std::invalid_argument("id deleted"); }

            Data::setMagnet(id, SrcMagnet);
        }

        void delete_src(
            unsigned int id
        ){
            Data::deleted.insert(id);
            Metadata::Delete(id);
        }

        std::vector<source_t>& get_usr_src_list(std::string_view userid){
            std::vector<source_t> srcs;
            for(unsigned id=1; id <= Data::get_last_src(); ++id ){
                if(Data::deleted.contains(id))
                    continue;
                if(Data::getOwner(id) == userid){
                    srcs.push_back(move(get_source(id)));
                }
            }
            if(srcs.empty()){
                throw std::invalid_argument("no source found");
            }
            return srcs;
        }

        std::vector<source_t>& get_src_by_ids(
            const std::vector<unsigned int>& id_list
        ){
            std::vector<source_t> srcs;
            for(auto srcid : id_list){
                srcs.push_back(get_source(srcid));
            }
            return srcs;
        }
};
#endif