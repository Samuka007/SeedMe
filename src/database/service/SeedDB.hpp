#ifndef SeedDB_hpp
#define SeedDB_hpp

#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include <regex>
#include "service/Data.hpp"
#include "service/User.hpp"
// #include "util/Metadata.hpp"
#include "util/ErrorHandler.hpp"

using std::string;
using std::regex;
using std::pair;
using std::vector;

constexpr unsigned int OK = 200;
constexpr unsigned int NOT_FOUND = 404;


class SeedDB {
    private:
    // change relation ship from "is a" to "has a"
        Data data;
        User user;
        // Metadata metadata;

    public:
        SeedDB(std::string filename)
        :   data(filename + ".data"), 
            user(filename + ".user")
            // metadata(filename + ".meta")
        {}
        
        // std::vector<unsigned int> find_src_by_name(std::string_view name){
        //     //使用正则表达式与分词
        //     bool a = true;
        // }

        bool magnet_exist(std::string_view magnet){
            bool find = false;
            for(unsigned int i=1;i<data.table.last_row();++i){
                if(data.isDeleted(i))
                    continue;
                if(data.getMagnet(i) == magnet){
                    find = true;
                    break;
                }
            }
            return find;
        }

        bool username_exist(std::string_view username){
            bool find = false;
            for(unsigned int i=1;i<=this->user.get_last_user();++i){
                if(user.deleted.contains(i))
                    continue;
                if( username.compare(user.table[i].username) == 0 ){
                    find = true;
                    break;
                }
            }
            return find;
        }

        /**
         * UserController
        */
        
        unsigned login (string_view username, string_view password) {
            for(unsigned int id=1; id<=this->user.get_last_user(); ++id){
                if(user.deleted.contains(id))
                    continue;
                if( username.compare(user.table[id].username) == 0 ){// compare rule
                    if( password.compare(user.table[id].password) == 0 ){
                        return id;
                    }
                }
            }
            throw login_error();
        }

        inline unsigned new_user (string_view username, string_view password) {
            if(username_exist(username)) {
                throw invalid_username();
            }
            if (username.length() > 127) {
                throw too_long("Username");
            }
            return user.addUser(username, password);
        }

        inline void update_username(unsigned usrid, std::string_view username_new){
            if (username_new.length() > 127) {
                throw too_long("Username");
            }
            user.setUsername(usrid, username_new);
        }

        inline void update_password( unsigned userid,
                                    std::string_view old_password,
                                    std::string_view new_password ) {
            user.setPassword(userid, 
                                old_password.data(), 
                                new_password.data());
        }
        
        void delete_user (unsigned int userid) { user.deleteUser(userid); }

        /**
         * SourceController:
        */
        unsigned int create_source( std::string_view    SrcName,
                                    std::string_view    SrcMagnet,
                                    unsigned            owner ) {
            unsigned srcid = data.addSrc(SrcName, SrcMagnet, owner);
            // metadata.Log(srcid, SrcName.data());
            return srcid;
        }

        source_t get_source (unsigned srcid) const { 
            if(srcid > data.table.last_row())  { throw resource_unfound(); }
            if(data.deleted.contains(srcid))   { throw resource_deleted(); }

            return source_t { data.table[srcid] };
        }

        std::vector<source_t> get_sources_by_ids( std::vector<unsigned int> id_list ) const{
            std::vector<source_t> srcs;
            for(auto srcid : id_list)   { srcs.push_back(get_source(srcid)); }
            return srcs;
        }

        void update_src_name ( unsigned int id, std::string_view SrcName ) { 
            if(data.deleted.contains(id))  { throw resource_deleted(); }
            
            std::strcpy(data.table[id].name, SrcName.data());
            // metadata.Delete(id);
            // metadata.Log(id, SrcName.data());
        }
        
        void update_src_magnet ( unsigned int id, std::string_view SrcMagnet){
            if(data.deleted.contains(id))  { throw resource_deleted();}

            data.setMagnet(id, SrcMagnet);
        }

        void delete_src ( unsigned int id ) {
            data.deleted.insert(id);
            // metadata.Delete(id);
        }

        std::vector<source_t> get_usr_src_list( unsigned userid ) const {
            std::vector<source_t> srcs;
            for(unsigned id=1; id <= data.get_last_src(); ++id ){
                if(data.deleted.contains(id))
                    continue;
                if( data.getOwner(id) == userid ) {
                    srcs.push_back(move(get_source(id)));
                }
            }
            if(srcs.empty()){
                throw resource_unfound();
            }
            return srcs;
        }

        std::vector<source_t> get_src_by_ids ( const std::vector<unsigned int>& id_list ) const {
            std::vector<source_t> srcs;
            for(auto srcid : id_list){
                try{ srcs.push_back(get_source(srcid)); }
                catch(const invalid_resource& e) { continue; }
            }
            if (srcs.empty()){
                throw resource_unfound();
            }
            return srcs;
        }

        friend auto search ( const SeedDB& db, const string& reg_str )
        {
            auto patten = regex(reg_str);
            vector<source_t> result;
            for (unsigned iter {0}; iter <= db.data.table.last_row(); ++iter) {
                try{ 
                    auto temp = db.get_source(iter); 
                    if(std::regex_search(temp.srcname, patten)){
                        result.push_back(temp);
                    } 
                }
                catch(const invalid_resource& e){ continue; }
            }
            if (result.empty()) { throw resource_unfound(); }
            else { return result; }
        }

        /**
         * MetadataController:
        */

        // inline void add_tag ( std::string_view tag ) {
        //     metadata.add_tag(tag.data());
        // }

        // inline void delete_tag ( std::string_view tag ) {
        //     metadata.remove_tag(tag.data());
        // }

        // inline std::vector<std::string> get_tag_list () {
        //     return metadata.get_tag_list();
        // }
        
        // inline std::vector<unsigned> get_ids_by_tag ( std::string_view tag ) {
        //     return metadata.get_ids_by_tag(tag.data());
        // }

        // inline std::vector<source_t> get_src_by_tag ( std::string_view tag ) {
        //     return get_src_by_ids(get_ids_by_tag(tag));
        // }
};
#endif