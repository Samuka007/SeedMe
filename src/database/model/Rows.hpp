#ifndef Rows_hpp
#define Rows_hpp
#include <bitset>
#include <cstring>
#include <cmath>
#include <string>
#include <string_view>

struct source_t {
    unsigned ID; 
    std::string srcname; 
    std::string magnet; 
    unsigned owner;
};

static constexpr size_t STRING_LENGTH = 128;

struct UsrRow {
    UsrRow() : id{0}, username {0}, password {0} {}

    explicit UsrRow(unsigned _id, const char* _name, size_t len1, const char* _pw, size_t len2)
    :   id{_id}, username {0}, password {0} 
    {
        std::memcpy(username, _name , std::min(len1, STRING_LENGTH));
        std::memcpy(password, _pw,    std::min(len2, STRING_LENGTH));
    }

    explicit UsrRow (unsigned _id, const std::string& _name, const std::string& _pw) 
    :   id{_id}, username {0}, password {0}
    {
        std::memcpy(username, _name.c_str(), std::min(_name.length(), STRING_LENGTH));
        std::memcpy(password, _pw.c_str(),   std::min(_pw.length(), STRING_LENGTH));
    }

    explicit UsrRow (unsigned _id, const std::string_view _name, const std::string_view _pw)
    :   id{_id}, username {0}, password {0}
    {
        std::memcpy(username, _name.data(), std::min(_name.length(), STRING_LENGTH));
        std::memcpy(password, _pw.data(),   std::min(_pw.length(), STRING_LENGTH));
    }

    unsigned    id;
    char        username[STRING_LENGTH];
    char        password[STRING_LENGTH];

};

constexpr size_t SIZE_OF_USRROW = sizeof(UsrRow);

struct SrcRow {
    SrcRow () : id{0}, owner {0}, name {0}, magnet {0} {}

    explicit SrcRow ( unsigned _id, const char* _name, size_t len1, const char* _magnet, size_t len2, unsigned _owner)
    :   id{_id}, owner{_owner}, name {0}, magnet {0} {
        std::memcpy(name,   _name ,  std::min(len1, STRING_LENGTH));
        std::memcpy(magnet, _magnet, std::min(len2, STRING_LENGTH));
    }

    explicit SrcRow ( unsigned _id, const std::string& _name, const std::string& _magnet, unsigned _owner)
    :   id{_id}, owner{_owner}, name {0}, magnet {0} {
        std::memcpy(name,   _name.c_str() ,  std::min(_name.length(), STRING_LENGTH));
        std::memcpy(magnet, _magnet.c_str(), std::min(_magnet.length(), STRING_LENGTH));
    }

    explicit SrcRow ( unsigned _id, const std::string_view _name, const std::string_view _magnet, unsigned _owner)
    :   id{_id}, owner{_owner}, name {0}, magnet {0} {
        std::memcpy(name,   _name.data() ,  std::min(_name.length(), STRING_LENGTH));
        std::memcpy(magnet, _magnet.data(), std::min(_magnet.length(), STRING_LENGTH));
    }

    unsigned    id;
    char        name   [STRING_LENGTH];
    char        magnet [STRING_LENGTH];
    unsigned    owner;

};

constexpr size_t SIZE_OF_SRCROW = sizeof(SrcRow);
#endif /*Rows_hpp*/