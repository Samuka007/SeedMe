#ifndef SeeDB_hpp
#define SeeDB_hpp

#include "./dataframe/Cursor.hpp"

#include <string_view>

class SeeDB{
public:
    SeeDB(std::string_view filename);

    Row& 

    //input-output socket
private:
    Cursor cursor;

};

#endif