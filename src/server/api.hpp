#pragma once
#include "include/nlohmann/json.hpp"
using json = nlohmann::json;

#include <string>
using std::string;

#include <optional>
using std::optional;

#include "database/service/SeedDB.hpp"
#include "encrypt/md5.h"

namespace api
{
class response_t
{
public:
    virtual string body() = 0;
    virtual int status() { return 404; }
};

static int operation(const string& op)
{
    if (op == "create") return 1;
    if (op == "read" || op == "login")   return 2;
    if (op == "update") return 3;
    if (op == "delete") return 4;
}

/**
 * Available operations:
 * Create | Read | Update | Delete
 * Available entities:
 * username | password | new_password | token
*/
class usr_handler : response_t
{
public:
    usr_handler(const string& req, SeedDB& db)
    {
        json j = json::parse(req);
        switch (operation(j["operation"]))
        {
        case 1:
            db.new_user(j["name"], j["password"]);
            break;
        case 2:
            if (j.contains("username") && j.contains("password"))
            { 
                auto id = db.login(j["username"], j["password"]);
                
            }
            break;
        case 3:
            if(j.contains("username")) 
            { db.update_username(j["id"], j["username"]); }
            if(j.contains("password"))
            { db.update_password(j["id"], j["password"], j["new_password"]); }
            break;
        case 4:
            db.delete_user(j["id"]);
            break;

        default:
            break;
        }
    }

    

private:
    string body;
    int status;
};
// need to handle any parse error

}