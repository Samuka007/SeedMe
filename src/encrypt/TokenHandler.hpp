#pragma once
#include <string>
#include <map>
#include <ctime>
#include <random>
#include "database/service/SeedDB.hpp"
#include "encrypt/md5.h"

using std::string;

class TokenHandler{
    private:
        std::map<time_t, string> tokens;
        int secret;
        time_t expire_time;
        const SeedDB& db;
    public:
        TokenHandler(const SeedDB& db, time_t expire_time = 3600)
        : expire_time(expire_time), db(db)
        {
            std::random_device seed;
            std::ranlux48 rand(seed());
            std::uniform_int_distribution<> dist(1000, 9999);
            secret = dist(rand);
        }
        string generate_token();
        bool check_token(string token);
        void remove_token(string token);
        void remove_expired_token();
};

string TokenHandler::generate_token(unsigned usrid) {
    time_t now = time(nullptr);
    string token = MD5(std::to_string(usrid) + std::to_string(secret)).toStr();
    tokens[now] = token;
}

bool chech_token(string token) {
    time_t now = time(nullptr);
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
        if (it->second == token) {
            if (now - it->first > expire_time) {
                tokens.erase(it);
                return false;
            }
            return true;
        }
    }
    return false;
}