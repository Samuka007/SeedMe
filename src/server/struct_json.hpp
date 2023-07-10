#pragma once

#include "include/nlohmann/json.hpp"
#include <string>
#include <optional>

using std::optional;
using std::string;
using nlohmann::json;

namespace jsfmt
{
// Output struct => json

struct source_t {
    unsigned ID; 
    string srcname; 
    string magnet; 
    unsigned owner;
};

    // source_t => json
void to_json(json& j, const source_t& src) {
    j = json{{"ID", src.ID}, {"Name", src.srcname}, {"Magnet", src.magnet}, {"Owner", src.owner}};
}

    // tag list => json (array)


// Input json => struct
// For handle post request

struct src_operation_t 
{
    // construct from json
    src_operation_t(const json& j) 
    : token(j.at("token").get<string>())
    , operation(j.at("operation").get<string>())
    , ID(j.at("ID").get<unsigned>())
    , srcname(j.at("Name").get<string>())
    , magnet(j.at("Magnet").get<string>())
    , owner(j.at("Owner").get<unsigned>()) {}

    string token;
    string operation;
    unsigned ID; 
    string srcname; 
    string magnet; 
    unsigned owner;
};

struct usr_operation_t 
{
    // construct from json
    usr_operation_t(const json& j)
    : token(j.at("token").get<string>())
    , operation(j.at("operation").get<string>())
    , ID(j.at("ID").get<unsigned>())
    , username(j.at("Username").get<string>())
    , password(j.at("Password").get<string>()) {}

    // construct from request body
    usr_operation_t(const string& body)
    : usr_operation_t(json::parse(body)) {}

    string token;
    string operation;
    string password_old;
    unsigned ID;
    string username;
    string password;
};

struct tag_operation_t {
    string tag;
    string operation;
};

} // namespace jsfmt