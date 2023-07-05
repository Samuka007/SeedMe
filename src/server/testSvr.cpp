/**
 * @file testSvr.cpp
 * @brief testSvr.cpp
*/

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <string_view>
#include "server/network/httplib.h"
#include "server/json/json.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::string_view;

using httplib::Headers;
// using httplib::Server;
using httplib::Client;
using httplib::Request;
using httplib::Response;
using httplib::Params;
using httplib::Logger;

void client () {
    Client cli("localhost", 8080);
    
}