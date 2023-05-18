#include"SeeDB.hpp"

#include<string_view>

SeeDB::SeeDB(std::string_view filename)
    :   cursor(filename){}

