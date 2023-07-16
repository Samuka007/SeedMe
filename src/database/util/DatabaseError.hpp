#ifndef DatabaseError_hpp
#define DatabaseError_hpp

#include <exception>
#include <string>
#include <cstring>
#include <errno.h>
#include <format>
using std::string;
using std::exception;
using std::move;
using std::invalid_argument;
using std::runtime_error;

class file_error : public runtime_error {
    public:
        file_error() : runtime_error {strerror(errno)} {}
        file_error(const string& m) : runtime_error {m} {}
}

class database_error : public invalid_argument {
    public:
        database_error(const string& m) : invalid_argument {m} {}
}

class too_long : public database_error {
    public:
        too_long(const string& arg = "Argument") : database_error { arg + " too long." } {}
};

class too_short : public database_error {
    public:
        too_short(const string& arg = "Argument") : database_error { arg + " too short." } {}
}

class wrong_password : public database_error {
    public:
        wrong_password() : database_error { "Wrong password." } {}
};

class invalid_username : public database_error {
    public:
        invalid_username() : database_error { "Invalid username." } {}
};

class login_error : public database_error {
    public:
        login_error() : database_error { "Login failed! Please checkout your username and password." } {}
}

class invalid_resource : public database_error {
    public:
        invalid_resource(const string& e) : database_error {e} {}
}

class resource_unfound : public invalid_resource {
    public:
        resource_unfound() : invalid_resource {"Resource unfound."} {}
}

class resource_deleted : public invalid_resource {
    public:
        resource_deleted() : invalid_resource {"Resource has been deleted."} {}
}

#endif //DatabaseError.hpp