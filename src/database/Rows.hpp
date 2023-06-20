#ifndef Rows_hpp
#define Rows_hpp
#include <bitset>

struct UsrRow {
    constexpr static size_t LENGTH_OF_NAME   = 32;
    constexpr static size_t LENGTH_OF_PASSWORD = 32;

    unsigned    id;
    char        username[LENGTH_OF_NAME];
    char        password[LENGTH_OF_PASSWORD];
};

constexpr size_t SIZE_OF_USRROW = sizeof(UsrRow);

struct SrcRow {
    constexpr static size_t LENGTH_OF_NAME   = 64;
    constexpr static size_t LENGTH_OF_MAGNET = 64;

    unsigned    id;
    char        name   [LENGTH_OF_NAME];
    char        magnet [LENGTH_OF_MAGNET];
    unsigned    owner;
};

constexpr size_t SIZE_OF_SRCROW = sizeof(SrcRow);
#endif /*Rows_hpp*/