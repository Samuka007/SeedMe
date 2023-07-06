#pragma once
#include <string>

struct source_t {
    unsigned srcid; 
    std::string srcname; 
    std::string magnet; 
    unsigned owner;
};
struct user_t   {
    std::string usrname; 
    std::string password;
};

struct src_operation_t {
    std::string token;
    std::string oper;
    source_t src;
};

struct usr_operation_t {
    std::string token;
    std::string oper;
    std::string password_old;
    unsigned usrid;
    user_t usr;
};

struct tag_operation_t {
    std::string tag;
    std::string oper;
};