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
        unsigned port = 0;
        bool debug = false;
        for(auto par : arg){
            if (par.starts_with("name=")) {
                db_name = par.substr(5);
            }else if(par.starts_with("max_ram=")) {
                default_buffer_size = std::stoi(par.substr(8));
            }else if(par.starts_with("sudo=")) {
                sudo_token = par.substr(5);
            }else if(par.starts_with("port=")) {
                port = std::stoul(par.substr(5));
            }else if(par == "--debug") {
                debug = true;
            }
        }
        if (db_name.empty()) {
            if (argc > 3) {
                port = std::stoul(arg[3]);
                sudo_token = arg[2];
                default_buffer_size = std::stoi(arg[1]);
                db_name = arg[0];
            }
            if (argc > 2) {
                sudo_token = arg[2];
                default_buffer_size = std::stoi(arg[1]);
                db_name = arg[0];
            }
            if (argc > 1) {
                default_buffer_size = std::stoi(arg[1]);
                db_name = arg[0];
            }
            db_name = arg[0];
        }
        if(port > 1000 || port < 65535) {
            Seedme seedsvr(db_name, port);
        }else{
            Seedme seedsvr(db_name);
        }
    }
};