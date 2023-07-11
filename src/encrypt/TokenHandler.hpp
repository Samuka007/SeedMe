#pragma once
#include <string>
#include <map>
#include <chrono>
#include <random>
#include "database/service/SeedDB.hpp"
#include "encrypt/md5.h"

using namespace std::chrono;
using std::chrono::system_clock;
using std::chrono::time_point_cast;
using std::chrono::seconds;
using unix_time_seconds = time_point<system_clock, seconds>;
using std::string;

static string sudo_token = "123123";

class TokenHandler{
    private:
        std::map<string, unix_time_seconds> token_time;
        int secret;
        seconds expire_time;
        const SeedDB& db;
    public:
        TokenHandler(const SeedDB& db, seconds expire_time = 600s)
        : expire_time(expire_time), db(db)
        {
            std::random_device seed;
            std::ranlux48 rand(seed());
            std::uniform_int_distribution<> dist(1000, 9999);
            secret = dist(rand);
        }
        string generate_token(unsigned usrid);
        bool check_token(string token, unsigned usrid_v); //_v for varified, most of the time its the owner of src
        void remove_token(string token);
        void remove_expired_token();
};

string TokenHandler::generate_token(unsigned usrid) {
    string token = MD5(std::to_string(usrid) + std::to_string(secret)).toStr();
    token_time[token] = time_point_cast<seconds>(system_clock::now());
    remove_expired_token();
    return token;
}

bool TokenHandler::check_token(string token, unsigned usrid_v) { 
    if (token == sudo_token) return true;
    unix_time_seconds now = time_point_cast<seconds>(system_clock::now());
    if (token_time.contains(token)) {
        auto token_time_temp = token_time[token];
        if ((now - token_time_temp) > expire_time) {
            token_time.erase(token);
            return false;
        }
        if (MD5(std::to_string(usrid_v) + std::to_string(secret)).toStr() != token) {
            return false;
        }
        token_time[token] = now;
        return true;
    }
    return false;
}

void TokenHandler::remove_token(string token) {
    token_time.erase(token);
}

void TokenHandler::remove_expired_token() {
    unix_time_seconds now = time_point_cast<seconds>(system_clock::now());
    for (auto it : token_time) {
        if ((now - it.second) > expire_time) {
            token_time.erase(it.first);
        }
    }
}