//SQL and Web server starter
#ifndef Seedme_hpp
#define Seedme_hpp
#include <iostream>
#include <vector>
#include "database/service/SeedDB.hpp"
#include "server/json/json.hpp"
//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "server/network/httplib.h"
#include "encrypt/TokenHandler.hpp"
using namespace httplib;
using json = nlohmann::json;
using std::string;
using std::string_view;
using std::vector;

class Seedme{
private:
    SeedDB Database;
    Server seedsvr;
    TokenHandler tokens;

public:
    Seedme(string database_name, size_t port = 8080)
    : Database(database_name), tokens(Database)
    {
        /**
         * Get methods:
        */

        // Default page
        // seedsvr.Get("/", [](const Request &, Response &res) {
            // res.set_content("Hello SeedMe!", "text/plain");
        // });

        //Get Source by id
        seedsvr.Get(R"(/get/source/(\d+))", [&](const Request& req, Response& res) {
            auto srcid = std::stoi(req.matches[1]);
            try{
                auto src = Database.get_source(srcid);
                res.set_content(json(src.second).dump(), "application/json");
                res.status = OK;
            }catch(const std::exception& e){
                res.status = NOT_FOUND;
            }
        });

        // Get Sources of user
        // seedsvr.Get(R"(/get/user/(\d+))", [&](const Request& req, Response& res) {
        //     //res = user's sources list json
        //     try{
        //         auto list = Database.get_src_by_ids(
        //                         Database.get_user_src_ids(std::stoi(req.matches[1])));
        //         res.status = OK;
        //     }catch(const std::exception& e){
        //         res.status = NOT_FOUND;
        //     }
        // });

        // Get Tags
        seedsvr.Get("/get/taglist", [&](const Request& req, Response& res) {
            if(req.has_param("tag")){
                string tag = req.get_param_value("tag");
            }
        });

        /**
         * Post methods:
        */
        
        //Login Api, return token
        seedsvr.Post("/post/login", [&](const Request& req, Response& res) {
            json body = json::parse(req.body);
            if(body.contains("username") && body.contains("password")){
                try{
                    auto userid = Database.login(body["username"],body["password"]);
                    string token = tokens.generate_token(userid);
                    res.set_content(json(
                                    {
                                        {"token", token},
                                        {"usrid", userid}
                                    }
                                ).dump(), "application/json");
                    res.status = OK;
                }catch(const std::exception& e){
                    res.status = NOT_FOUND;
                }
            }
        });
        
        //Post Source operation
        seedsvr.Post("/post/source", [&](const Request& req, Response& res) {
            json body = json::parse(req.body);
            if (!body.contains("token")) {
                res.status = NOT_FOUND;
                return;
            }
            if (body.contains("token")
                && body["token"].is_string()
                && body.contains("Operation") 
                && body["Operation"].is_string()
                && body.contains("ID")
                && body["ID"].is_number_unsigned()){

                std::string token = body["token"];
                std::string operation = body["Operation"];
                unsigned srcid = body["ID"];
                try{
                    // src'onwer is not this usr 
                    // or the token is out of date
                    if(!tokens.verify_token(body["token"], 
                                            Database.get_source(srcid).owner)){
                        throw std::invalid_argument("Invalid token");
                    }
                    res.status = handle_src_operation(body);
                } catch(const std::exception& e) {
                    res.set_content(json(
                                    {
                                        {"error", e.what()}
                                    }
                                ).dump(), "application/json");
                    res.status = NOT_FOUND;
                }
            } else {
                res.status = 404;
            }
        });

        //Post User operation
        seedsvr.Post("/post/user", [&](const Request& req, Response& res) {
            json body = json::parse(req.body);
            if(body.contains("Operation") 
            && body["Operation"].is_string()){
                res.status = handle_user_operation(body);
            }else{
                res.status = 404;
            }
        });

        std::cout<<"start listening..."<<std::endl;
        seedsvr.listen("0.0.0.0", port);
    }

    void handle_src_operation(json body){
        /*TODO: add handler*/
        if(!(body.contains("Operation") 
        && body["Operation"].is_string())){
                throw std::invalid_argument("Invalid argument");
        }
        string oper {body["Operation"]};
        if(oper == "Create"){
            //create
            if(!(body.contains("Name") && body.contains("Magnet"))){
                throw std::invalid_argument("Invalid argument");
            }else{
                Database.create_source(string(body["Name"]), string(body["Magnet"]))
            }
        }else if(!body.contains("ID")){
                throw std::invalid_argument("Invalid argument");
        }else if(oper == "Update"){
            if(body.contains("Name")){
                Database.update_src_name(body["ID"], string(body["Name"]));
            }
            if(body.contains("Magnet")){
                Database.update_src_magnet(body["ID"], string(body["Magnet"]));
            }
        }else if(oper == "Delete"){
            Database.delete_src(body["ID"]);
        }
        throw std::invalid_argument("Invalid argument");
    }

    void handle_user_operation(json body){
        if(!(body.contains("Operation") 
            && body["Operation"].is_string())){
                throw std::invalid_argument("Invalid argument");
        }
        string oper {body["Operation"]};
        if(oper == "Create"){
            //create
            if(!(body.contains("Name") && body.contains("Password"))){
                throw std::invalid_argument("Invalid argument");
            }else{
                Database.create_source(string(body["Name"]), string(body["Password"]));
            }
        }else if(!body.contains("ID")){
                throw std::invalid_argument("Invalid argument");
        }else{
            /*TODO: varified id and source owner with token*/
            if(oper == "UpdateUsername"){
                if(body.contains("Name") && body.contains("Password")){
                    Database.update_username(body["ID"], string(body["NewName"]));
                }
            }else if(oper == "Delete"){
                Database.delete_user(body["ID"]);
            }else if(oper == "UpdatePassword"){
                if(body.contains("OldPassword") && body.contains("NewPassword")){
                    Database.update_password(body["ID"], 
                                            string{body["OldPassword"]}, 
                                            string{body["NewPassword"]});
                }
            }
        }
        throw std::invalid_argument("Invalid argument");
    }
};
#endif