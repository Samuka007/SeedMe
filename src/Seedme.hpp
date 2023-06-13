//SQL and Web server starter
#ifndef Seedme_hpp
#define Seedme_hpp
#include <iostream>
#include "database/SeedDB.hpp"
#include "server/json.hpp"
//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "server/httplib.h"

namespace Seedme{
    using namespace httplib;
    using json_t = nlohmann::json;
    void tryrun(){
        
        std::cout<<"input db name:";
        std::string filename;
        std::cin>>filename;
        std::cout<<"loading server..."<<std::endl;
        SeedDB Database(filename);
        
        std::cout<<"starting server..."<<std::endl;
        // For server launch
        httplib::Server seedsvr;
        
         
        /**
         * Get methods:
        */
        //Default page
        seedsvr.Get("/", [](const Request &, Response &res) {
            res.set_content("Hello SeedMe!", "text/plain");
        });

        //Get Source by id
        seedsvr.Get(R"(/numbers/(\d+))", [&](const Request& req, Response& res) {
            auto srcid = std::stoi(req.matches[1]);
            Database.get_source(srcid);
            res.set_content(req.matches[1], "text/plain");
        });

        //usr authentic -> update
        std::cout<<"start listening..."<<std::endl;
        seedsvr.listen("0.0.0.0", 8080);
    }
}
#endif