//SQL and Web server starter
#ifndef Seedme_hpp
#define Seedme_hpp
#include <iostream>
#include "database/SeedDB.hpp"
#include "server/json.hpp"
//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "server/httplib.h"
using namespace httplib;
using json = nlohmann::json;

class Seedme{
private:
    SeedDB Database;
    Server seedsvr;

public:
    Seedme(std::string database_name, unsigned port = 8080)
    : Database(database_name)
    {
        /**
         * Get methods:
        */
        //Default page
        seedsvr.Get("/", [](const Request &, Response &res) {
            res.set_content("Hello SeedMe!", "text/plain");
        });

        //Get Source by id
        seedsvr.Get(R"(/source/(\d+))", [&](const Request& req, Response& res) {
            auto srcid = std::stoi(req.matches[1]);
            auto src = Database.get_source(srcid);
            if(src.first == SUCCESS){
                res.set_content(json(src.second).dump(), "application/json");
            }
        });

        //Get Sources of user
        seedsvr.Get(R"(/user/(\d+))", [&](const Request& req, Response& res) {
            //res = user's sources list json
            auto list = Database.get_sources_by_ids(Database.get_user_source_id(std::stoi(req.matches[1])).second);
            res.status = list.first;
            if(res.status == SUCCESS){
                res.set_content(json(list.second), "application/json");
            }
        });

        //Get Tags
        seedsvr.Get("/taglist", [&](const Request& req, Response& res) {
            if(req.has_param("tag")){
                std::string tag = req.get_param_value("tag");
            }
        });

        //Get login page
        //seedsvr.Get("/login", [&](const Request& req, Response& res) {
        //    res.set_content()
        //});

        /**
         * Post methods:
        */
        
        //Post Source operate
        seedsvr.Post("/source", [&](const Request& req, Response& res) {
            json body = json::parse(req.body);
            if(body.contains("Operate") 
            && body["Operate"].is_string()
            && body.contains("ID")
            && body["ID"].is_number_unsigned()){
                res.status = handle_src_operation(body);
            }else{
                res.status = 404;
            }
        });

        //Post User operate
        seedsvr.Post("/user", [&](const Request& req, Response& res) {
            json body = json::parse(req.body);
            if(body.contains("Operate") 
            && body["Operate"].is_string()){
                res.status = handle_user_operation(body);
            }else{
                res.status = 404;
            }
        });

        //usr authentic -> update
        std::cout<<"start listening..."<<std::endl;
        seedsvr.listen("0.0.0.0", port);
    }

    status handle_src_operation(json body){
        if(!(body.contains("Operate") 
        && body["Operate"].is_string())){
                return 404;
        }
        std::string oper {body["Operate"]};
        if(oper == "Create"){
            //create
            if(!(body.contains("Name") && body.contains("Magnet"))){
                return 404;
            }else{
                return Database.create_source(std::string(body["Name"]), std::string(body["Magnet"])).first;
            }
        }else if(!body.contains("ID")){
                return 404;
        }else if(oper == "Update"){
            if(body.contains("Name") && body.contains("Magnet")){
                return Database.update_source(body["ID"], std::string(body["Name"]), std::string(body["Magnet"])).first;
            }
        }else if(oper == "Delete"){
            return Database.delete_source(body["ID"]).first;
        }
        return 404;
    }

    status handle_user_operation(json body){
        if(!(body.contains("Operate") 
            && body["Operate"].is_string())){
                return 404;
        }
        std::string oper {body["Operate"]};
        if(oper == "Create"){
            //create
            if(!(body.contains("Name") && body.contains("Password"))){
                return 404;
            }else{
                return Database.create_source(std::string(body["Name"]), std::string(body["Password"])).first;
            }
        }else if(!body.contains("ID")){
                return 404;
        }else if(oper == "UpdateUsername"){
            if(body.contains("Name") && body.contains("Password")){
                return Database.update_username(body["ID"], std::string(body["Name"]), std::string(body["Password"])).first;
            }
        }else if(oper == "Delete"){
            return Database.delete_user(body["ID"]).first;
        }else if(oper == "UpdatePassword"){
            if(body.contains("OldPassword") && body.contains("NewPassword")){
                return Database.update_password(body["ID"], std::string(body["OldPassword"]), std::string(body["NewPassword"])).first;
            }
        }
        return 404;
    }
};
#endif