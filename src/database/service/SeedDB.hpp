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
#include "util/ErrorHandler.hpp"
#include "database/model/struct.hpp"

using std::pair;
using status = size_t;

constexpr unsigned int OK = 200;
constexpr unsigned int NOT_FOUND = 404;

class SeedDB : public Data, public User, public Metadata {
    public:
        SeedDB(std::string filename)
        :   Data(filename + ".data"), 
            User(filename + ".user"),
            Metadata(filename + ".meta")
        {}
        
        // std::vector<unsigned int> find_src_by_name(std::string_view name){
        //     //使用正则表达式与分词
        //     bool a = true;
        // }

        bool magnet_exist(std::string_view magnet){
            bool find = false;
            for(unsigned int i=1;i<Data::table.last_row();++i){
                if(Data::isDeleted(i))
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
                if(User::table[i].username == username){
                    find = true;
                    break;
                }
            }
            return find;
        }

        /**
         * UserController
        */
        
        unsigned login (const user_t& user) {
            for(unsigned int id=1; id<this->User::get_last_user(); ++id){
                if(User::deleted.contains(id))
                    continue;
                if(User::table[id].username == user.usrname){
                    if(MD5(User::table[id].password).toStr()
                    == user.password){
                        return id;
                    }
                    else{
                        throw LoginError(user.usrname);
                    }
                }
            }
        }

        inline unsigned new_user (const user_t& user) {
            if(username_exist(user.usrname)) {
                throw std::invalid_argument("username already exist");
            }
            return User::addUser(user.usrname, user.password);
        }

        inline void update_username(unsigned usrid, std::string_view username_new){
            User::setUsername(usrid, username_new);
        }

        inline void update_password( unsigned userid,
                                    std::string_view old_password,
                                    std::string_view new_password ) {
            User::setPassword(userid, MD5(old_password.data()).toStr(), MD5(new_password.data()).toStr());
        }
        
        void delete_user (unsigned int userid) { User::deleteUser(userid); }

        // std::vector<unsigned>& get_user_src_ids(unsigned userid){
            // std::vector<unsigned> srcids;
            // for(unsigned id=1; id<=Data::get_last_src(); ++id){
                // if(Data::deleted.contains(id))
                    // continue;
                // if(Data::getOwner(id) == userid){
                    // srcids.push_back(id);
                // }
            // }
            // return srcids;
        // }

        /**
         * SourceController:
        */
        unsigned int create_source( std::string_view    SrcName,
                                    std::string_view    SrcMagnet,
                                    unsigned            owner ) {
            unsigned srcid = addSrc(SrcName, SrcMagnet, owner);
            Metadata::Log(srcid, SrcName.data());
            return srcid;
        }

        source_t get_source (unsigned srcid) { 
            if(srcid > Data::table.last_row())      { throw SourceNotFoundError {srcid}; }
            if(Data::deleted.contains(srcid))  { throw SourceNotFoundError {srcid}; }

            return source_t {
                srcid,
                Data::table[srcid].name,
                Data::table[srcid].magnet,
                Data::table[srcid].owner
            };
        }

        std::vector<source_t> get_sources_by_ids( std::vector<unsigned int> id_list ){
            std::vector<source_t> srcs;
            for(auto srcid : id_list) { srcs.push_back(get_source(srcid)); }
            return srcs;
        }

        void update_src_name ( unsigned int id, std::string_view SrcName ) { 
            if(Data::deleted.contains(id))  { throw SourceNotFoundError {}; }
            
            std::strcpy(Data::table[id].name, SrcName.data());
            Metadata::Delete(id);
            Metadata::Log(id, SrcName.data());
        }
        
        void update_src_magnet ( unsigned int id, std::string_view SrcMagnet){
            if(Data::deleted.contains(id))  { throw SourceNotFoundError {};}

            Data::setMagnet(id, SrcMagnet);
        }

        void delete_src ( unsigned int id ) {
            Data::deleted.insert(id);
            Metadata::Delete(id);
        }

        std::vector<source_t> get_usr_src_list( unsigned userid ) {
            std::vector<source_t> srcs;
            for(unsigned id=1; id <= Data::get_last_src(); ++id ){
                if(Data::deleted.contains(id))
                    continue;
                if( Data::table[id].owner == userid ) {
                    srcs.push_back(move(get_source(id)));
                }
            }
            if(srcs.empty()){
                throw SourceNotFoundError {};
            }
            return srcs;
        }

        std::vector<source_t> get_src_by_ids ( const std::vector<unsigned int>& id_list ) {
            std::vector<source_t> srcs;
            for(auto srcid : id_list){
                srcs.push_back(get_source(srcid));
            }
            return srcs;
        }
};
#endif