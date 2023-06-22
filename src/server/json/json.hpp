#pragma once
#include <string>
#include <map>
//#include <format>
#include <vector>
#include "database/model/struct.hpp"
#include "util/fmt/format.h"

using std::string;
using fmt::format;
using std::map;
using std::vector;
using std::move;

/**
 * @brief A class to handle JSON data.
 * should have:
 *  json to string
 *  
*/

bool isNumber(const string& str)
{
    return str.find_first_not_of("0123456789") == string::npos;
}

class Json {
    public:
        Json() = default;
        virtual ~Json() = default;
        virtual string dump() = 0;
};

class Object_s : public Json {
    public:
        Object_s() = default;
        ~Object_s() = default;
        string dump() override {
            string result = "{";
            for (auto& [key, value] : this->data) {
                if (isNumber(value))
                    result += format("\"{}\": {},", key, value);
                else
                    result += format("\"{}\": \"{}\",", key, value);
            }
            result.pop_back();
            result += "}";
            return result;
        }

        void add(string key, string value) {
            this->data[key] = value;
        }
    private:
        map<string, string> data;
};

class Object : public Json {
    public:
        Object() = default;
        ~Object() = default;
        string dump() override {
            string result = "{";
            for (auto& [key, value] : this->data) {
                if (isNumber(value))
                    result += format("\"{}\": {},", key, value);
                else
                    result += format("\"{}\": \"{}\",", key, value);
            }
            result.pop_back();
            result += "}";
            return result;
        }

        void add(string key, string value) {
            this->data[key] = value;
        }
    private:
        map<string, string> data;
};

class Json_src : public Json {
    public:
        Json_src(const source_t& src) 
        : data (src) {}
        Json_src() = default;
        string dump() override {
            string result = "{";
            result += "\"ID\":";result += data.srcid;
            result += ",\"Name\":\"";result += data.srcname;
            result += "\",\"Magnet\":\"";result += data.magnet;
            result += "\",\"Owner\":";result += data.owner;
            result += "}";
            return result;
        }
    private:
        source_t data;
};

class Body_handler : public Json {
    public:
        Body_handler(const string& body)
        : body(body) {}

        virtual string dump() override {
            return body;
        }

        string contains(const string& key) {
            string result = "";
            string key_ = format("\"{}\":", key);
            size_t pos = body.find(key_);
            if (pos != string::npos) {
                pos += key_.size();
                while (body[pos] != ',' && body[pos] != '}') {
                    result += body[pos];
                    pos++;
                }
            }
            return result;
        }

        source_t parse_to_source() {
            source_t result;
            result.srcid = (contains("ID") == "" ? std::stol(contains("ID")) : 0);
            result.srcname = contains("Name");
            result.magnet = contains("Magnet");
            result.owner = (contains("ID") == "" ? std::stol(contains("Owner")) : 0);
            return result;
        }

        user_t parse_to_user() {
            user_t result;
            result.usrname = contains("Username");
            result.password = contains("Password");
            return result;
        }

        src_operation_t parse_to_src_operation() {
            src_operation_t result;
            result.token = contains("Token");
            result.oper = contains("Operation");
            result.src = parse_to_source();
            return result;
        }

        usr_operation_t parse_to_usr_operation() {
            usr_operation_t result;
            result.token = contains("Token");
            result.oper = contains("Operation");
            result.usrid = (contains("ID") == "" ? std::stol(contains("ID")) : 0);
            result.usr = parse_to_user();
            result.password_old = contains("Password_old");
            return result;
        }

    private:
        string body;
};

class login_response : public Json {
    public:
        login_response(const string& token, unsigned usrid)
        : token(token), usrid(usrid) {}

        string dump() override {
            string result = "{";
            result += "\"Token\":\"";result += token;
            result += "\",\"ID\":";result += usrid;
            result += "}";
            return result;
        }
    private:
        string token;
        unsigned usrid;
};

class List_for_src : public Json{
    public:
        List_for_src() = default;
        ~List_for_src() = default;
        List_for_src(vector<source_t> src) {
            this->data = src;
        }

        string dump() override {
            string result = "[";
            for (auto& value : this->data) {
                result += Json_src(value).dump();
                result += ",";
            }
            result.pop_back();
            result += "]";
            return result;
        }

        void add(source_t value) {
            this->data.push_back(value);
        }
    private:
        vector<source_t> data;
};