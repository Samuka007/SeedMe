//SQL and Web server starter
#ifndef Seedme_hpp
#define Seedme_hpp
#include "database/SeedDB.hpp"

//#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "server/httplib.h"
namespace Seedme{
    void tryrun(){
        httplib::Server seedsvr;
    
        seedsvr.Get("/", [](const httplib::Request &, httplib::Response &res) {
            res.set_content("Hello SeedMe!", "text/plain");
        });
    
        seedsvr.listen("0.0.0.0", 8080);
    }
}
#endif