/**
 * Loadin.hpp
 * Handle start up arguments and environment variables 
 */
#pragma once

#include "server/Seedme.hpp"
#include <string>
#include <vector>
#include <regex>
#include <exception>

using std::string;
using std::vector;

namespace SeedSvr {
    void init(int argc, char* argv[]) {
        if (argc < 1) {
            throw std::invalid_argument {"lacks of arguments"};
        }
        vector<string> arg;
        for(int i {0}; i < argc; ++i ){
            arg.push_back(string(argv[i]));
        }
        string db_name;
        int change_limit;
        string sudoer_api;
        unsigned port;
        bool debug = false;
        for(auto par : arg){
            if (par.starts_with("name=")) {
                db_name = par.substr(6);
            }else if(par.starts_with("max_ram=")) {
                change_limit = std::stoi(par.substr(9));
            }else if(par.starts_with("sudo=")) {
                sudoer_api = par.substr(6);
            }else if(par.starts_with("port=")) {
                port = std::stoul(par.substr(6));
            }else if(par == "--debug") {
                debug = true;
            }
        }
        if (db_name.empty()) {
            if (argc > 3) {
                port = std::stoul(arg[3]);
                sudoer_api = arg[2];
                default_buffer_size = std::stoi(arg[1]);
                db_name = arg[0];
            }
            if (argc > 2) {
                sudoer_api = arg[2];
                default_buffer_size = std::stoi(arg[1]);
                db_name = arg[0];
            }
            if (argc > 1) {
                default_buffer_size = std::stoi(arg[1]);
                db_name = arg[0];
            }
            db_name = arg[0];
        }
        Seedme seed("try");
    }
};