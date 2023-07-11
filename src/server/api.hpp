/**
 * Change ABI to API
 * with md5 password encrypt and token variefied
*/
#pragma once
#include "include/nlohmann/json.hpp"
using json = nlohmann::json;

#include <string>
using std::string;

#include <optional>
using std::optional;

#include "database/service/SeedDB.hpp"
#include "encrypt/TokenHandler.hpp"
#include "encrypt/md5.h"

static void to_json(json& j, const source_t& s)
{
    j["ID"] = s.ID;
    j["srcname"] = s.srcname;
    j["magnet"] = s.magnet;
    j["owner"] = s.owner;
}

namespace api
{
    // source_t => json



class response_t
{
public:
    virtual string dump() = 0;
    virtual int status() { return 404; }
};

// static int operation(const string& op)
// {
//     if (op == "create") return 1;
//     if (op == "read" || op == "login")   return 2;
//     if (op == "update") return 3;
//     if (op == "delete") return 4;
//     return 0;
// }

/**
 * Available operations:
 * Create | Read | Update | Delete
 * Available entities:
 * username | password | new_password | token
*/
class usr_handler : response_t
{
public:
    usr_handler(const string& req, SeedDB& db, TokenHandler& tk)
    {
        json j = json::parse(req);
        json res;
        if(j.at("operation") == "create")
        {
            int id = db.new_user(
                string(j.at("username")), MD5(j.at("password")).toStr());
            res.emplace("ID", id);
            res.emplace("token", tk.generate_token(id));
        }
        if(j.at("operation") == "read")
        {
            int id = db.login(string(j.at("username")), 
                    MD5(j.at("password")).toStr());
            res.emplace("ID", id);
            res.emplace("token", tk.generate_token(id));
        }
        if(j.at("operation") == "update")
        {
            tk.check_token(j.at("token").get<string>(),j.at("ID").get<int>());
            if(j.contains("username")) 
            { db.update_username(j.at("ID").get<int>(), string(j.at("username")) ); }
            if(j.contains("password"))
            { db.update_password(j.at("ID"), 
                MD5(j.at("old_password")).toStr(), 
                MD5(j.at("password")).toStr()); }
        }
        if (j.at("operation") == "delete")
        {
            string token = j.at("token");
            int id = j.at("ID").get<int>();
            tk.check_token(token, id);
            tk.remove_token(token);
            db.delete_user(j.at("ID"));
        }
        if(!res.empty())
            body_ = res.dump();
        status_ = 200;
    }

    string dump() { return body_; }
    int status()  { return status_; }

private:
    string body_ = "";
    int status_;
};
// need to handle any parse error

class src_handler : response_t
{
public:
    src_handler(const string& req, SeedDB& db, TokenHandler& tk)
    {
        json j = json::parse(req);
        json res;
        if(j.at("operation") == "create")
        {
            tk.check_token(j.at("token"), j.at("owner").get<int>());
            int id = db.create_source(j.at("name").get<string>(), 
                    j.at("magnet").get<string>(), 
                    j.at("owner").get<int>());
            res.emplace("ID", id);
        }

        if(j.at("operation") == "read")
        { res = db.get_source(j.at("ID").get<int>()); }

        if(j.at("operation") == "update")
        {
            tk.check_token(j.at("token"), j.at("owner").get<int>());
            if(j.contains("name")) 
            { db.update_src_name(j.at("ID").get<int>(), string(j.at("name"))); }
            if(j.contains("magnet"))
            { db.update_src_magnet(j.at("ID").get<int>(), string(j.at("magnet"))); }
        }
        
        if(j.at("operation") == "delete")
        {
            tk.check_token(j.at("token"), j.at("owner").get<int>());
            db.delete_src(j.at("ID").get<int>());
        }

        if(!res.empty())
            body_ = res.dump();
        status_ = 200;
    }

    string dump() { return body_; }
    int status()  { return status_; }
private:
    string body_ = "";
    int status_;
};

class tag_handler : response_t
{
public:
    tag_handler(const string& req, SeedDB& db, TokenHandler& tk)
    {
        json j = json::parse(req);
        tk.check_token(j.at("token"), j.at("ID").get<int>());
        if(j.at("operation") == "create")
            db.add_tag(j.at("name").get<string>());
        
        if(j.at("operation") == "delete")
            db.delete_tag(j.at("name").get<string>());
    }

    string dump() { return {}; }
    int status()  { return 200; }
};
}