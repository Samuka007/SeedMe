//SQL and Web server starter
#ifndef Seedme_hpp
#define Seedme_hpp
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <map>
#include "database/service/SeedDB.hpp"
#include "server/api.hpp"
//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "include/httplib/httplib.h"
#include "encrypt/TokenHandler.hpp"
#include "encrypt/md5.h"
// #include "server/json/json.hpp"
#include "util/ErrorHandler.hpp"

using httplib::Headers;
using httplib::Server;
using httplib::Request;
using httplib::Response;
using httplib::Params;
using httplib::Logger;


using std::string;
using std::string_view;
using std::vector;

class Seedme{
private:
    SeedDB Database;
    Server seedsvr;
    TokenHandler tokens;

public:
    Seedme(string database_name, size_t port = 8080, int debug = 0)
    : Database(database_name), tokens(Database)
    {
        /**
         * Get methods:
        */

        // Default page
        seedsvr.Get("/hello", [](const Request &, Response &res) {
            res.set_content("Hello SeedMe!", "text/plain");
        });

        //Get Source by id
        seedsvr.Get(R"(/get/source/(\d+))", [&](const Request& req, Response& res) {
            auto srcid = std::stol(req.matches[1]);
            try{
                json src = Database.get_source(srcid);
                res.set_content (src.dump() , "application/json");
                res.status = OK;
            }catch(const std::exception& e){
                res.status = NOT_FOUND;
            }
        });

        //Get Sources of user
        seedsvr.Get(R"(/get/user/(\d+))", [&](const Request& req, Response& res) {
            //res = user's sources list json
            try{
                json srclist;
                for(auto src : Database.get_usr_src_list(std::stol(req.matches[1]))){
                    srclist.emplace_back(json(src));
                }
                res.set_content (srclist.dump(), "application/json" );
                res.status = OK;
            }catch(const std::exception& e){
                res.status = NOT_FOUND;
            }
        });

        // Get Tag / Get src by tag
        seedsvr.Get("/get/taglist", [&](const Request& req, Response& res) {
            if(req.has_param("tag")){
                try{
                    json taglist;
                    for(auto src : Database.get_src_by_tag(req.get_param_value("tag"))){
                        taglist.emplace_back(json(src));
                    }
                    res.set_content ( taglist.dump(), "application/json" );
                    res.status = OK;
                }catch(const std::exception& e){
                    res.status = NOT_FOUND;
                }
            }else{
                try{
                    json taglist;
                    for(auto tag : Database.get_tag_list()){
                        taglist.emplace_back(json(tag));
                    }
                    res.set_content ( taglist.dump(), "application/json" );
                    res.status = OK;
                }catch(const std::exception& e){
                    res.status = NOT_FOUND;
                }
            }
        });

        /**
         * Post methods:
        */
        
        // Login Api, return token
        // Post User operation
        // if call create, then name, password are needed
        // if call login, then name, password are needed
        // if call update, then id and name or password 
        //      ("password" and "old_password") are needed
        // if call delete, then id is needed
        seedsvr.Post("/post/user", [&](const Request& req, Response& res) {
            try{
                std::cout << req.body << std::endl;
                auto handle = api::usr_handler(req.body, Database, tokens);
                res.set_content (handle.dump(), "application/json");
                res.status = handle.status();
            }catch(const std::exception& e){
                res.status = NOT_FOUND;
            }
        });
        
        // Post Source operation
        // always needs token
        // if call create, then name, magnet and userid are needed, write userid in owner.
        // if call read, then id is needed
        // if call update, then id and name or magnet are needed
        // if call delete, then id is needed
        seedsvr.Post("/post/source", [&](const Request& req, Response& res) {
            try{
                std::cout << req.body << std::endl;
                auto handle = api::src_handler(req.body, Database, tokens);
                res.set_content (handle.dump(), "application/json");
                res.status = handle.status();
            } catch(const std::exception& e) {
                res.status = NOT_FOUND;
            }
        });

        // Post Tag operation
        // ID and token are needed
        seedsvr.Post("/post/", [&](const Request& req, Response& res) {
            try{
                std::cout << req.body << std::endl;
                auto handle = api::tag_handler(req.body, Database, tokens);
                res.set_content (handle.dump(), "application/json");
                res.status = handle.status();
            } catch(const std::exception& e) {
                res.status = NOT_FOUND;
            }
        });


        if(debug == 1){
            int userid = Database.new_user("admin", "admin");
            std::cout << userid << std::endl;
            // std::cout << Database.create_source("test", "magnet", userid) << std::endl;
        }

        std::cout<<"start listening on " << port <<std::endl;
        seedsvr.listen("0.0.0.0", port);
        
    }
};
#endif


    // void handle_src_operation(const src_operation_t body){
    //     /*TODO: add handler*/
    //     if(body.token.empty()){
    //             throw std::invalid_argument("Invalid argument");
    //     }
    //     //varified token
    //     if(!tokens.check_token(body.token, body.src.owner)){
    //         throw std::invalid_argument("Invalid argument");
    //     }
    //     if(body.oper == "Create"){
    //         //create
    //         if(body.src.magnet.empty() || body.src.srcname.empty()){
    //             throw std::invalid_argument("Invalid argument");
    //         }else{
    //             Database.create_source(body.src.srcname, body.src.magnet, body.src.owner);
    //         }

    //     }else if(body.src.srcid == 0){
    //             throw std::invalid_argument("Invalid argument");

    //     }else if(body.oper == "Update"){
    //         if(!body.src.srcname.empty()){
    //             Database.update_src_name(body.src.srcid, body.src.srcname);
    //         }
    //         if(!body.src.magnet.empty()){
    //             Database.update_src_magnet(body.src.srcid, body.src.magnet);
    //         }

    //     }else if(body.oper == "Delete"){
    //         Database.delete_src(body.src.srcid);
    //     }else{
    //         throw std::invalid_argument("Invalid argument");
    //     }
    // }

    // void handle_user_operation(usr_operation_t& body){
    //     if(body.oper.empty()){
    //             throw std::invalid_argument("Invalid argument");
    //     }

    //     if(body.oper == "Create"){
    //         //create
    //         if(body.usr.usrname.empty() || body.usr.password.empty()){
    //             throw std::invalid_argument("Invalid argument");
    //         }else{
    //             body.usr.password = MD5(body.usr.password).toStr();
    //             Database.new_user(body.usr);
    //         }
    //     }else if(body.usrid == 0){
    //             throw std::invalid_argument("Invalid argument");
    //     }else{
    //         /*varified id with token*/
    //         if(!tokens.check_token(body.token, body.usrid)){
    //             throw std::invalid_argument("Invalid argument");
    //         }
    //         if(body.oper == "UpdateUsername"){
    //             if(body.usr.usrname.empty() || body.usr.password.empty()){
    //                 throw std::invalid_argument("Invalid argument");
    //             }else{
    //                 Database.update_username(body.usrid, body.usr.usrname);
    //             }
    //         }else if(body.oper == "Delete"){
    //             Database.delete_user(body.usrid);
    //         }else if(body.oper == "UpdatePassword"){
    //             if(body.usr.password.empty() || body.password_old.empty()){
    //                 throw std::invalid_argument("Invalid argument");
    //             }else{
    //                 Database.update_password(body.usrid, MD5(body.password_old).toStr(), MD5(body.usr.password).toStr());
    //             }
    //         }
    //     }
    //     throw std::invalid_argument("Invalid argument");
    // }

    /* TODO: add sudoer */
    /* Sudoer has a constant token which is 114514 by default or decided on started */

    // void handle_tag_operation(const tag_operation_t& body) {
    //     if(body.oper.empty()){
    //         throw std::invalid_argument("Invalid argument");
    //     }else if(body.tag.empty()){
    //         throw std::invalid_argument("Invalid argument");
    //     }else if(body.oper == "Create"){
    //         Database.add_tag(body.tag);
    //     }else if(body.oper == "Delete"){
    //         Database.delete_tag(body.tag);
    //     }else throw std::invalid_argument("Invalid argument");
    // }