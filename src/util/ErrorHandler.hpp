#ifndef ErrorHandler_hpp
#define ErrorHandler_hpp

#include <exception>
#include <string>
#include <cstring>
#include <errno.h>
#include <format>
using std::string;
using std::exception;
using std::invalid_argument;

namespace HttpStatus {
    constexpr unsigned OK = 200;
    constexpr unsigned CREATED = 201;
    constexpr unsigned ACCEPTED = 202;
    constexpr unsigned NO_CONTENT = 204;
    constexpr unsigned BAD_REQUEST = 400;
    constexpr unsigned UNAUTHORIZED = 401;
    constexpr unsigned FORBIDDEN = 403;
    constexpr unsigned NOT_FOUND = 404;
    constexpr unsigned METHOD_NOT_ALLOWED = 405;
    constexpr unsigned NOT_ACCEPTABLE = 406;
    constexpr unsigned REQUEST_TIMEOUT = 408;
    constexpr unsigned CONFLICT = 409;
    constexpr unsigned INTERNAL_SERVER_ERROR = 500;
    constexpr unsigned NOT_IMPLEMENTED = 501;
    constexpr unsigned BAD_GATEWAY = 502;
}

class invalid_request : public invalid_argument {
    public:
        invalid_request(int _status = HttpStatus::BAD_REQUEST, const string& m = "Request Error." ) 
        :   invalid_argument { m }, status_ { _status } {}
        int status() const noexcept { return status_; }
    private:
        int status_;
}

class invalid_token : public invalid_request {
    public:
        invalid_token() : invalid_request { "Invalid token.", HttpStatus::UNAUTHORIZED } {}
}

#endif //ErrorHandler.hpp