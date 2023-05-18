#ifndef UserDB_hpp
#define UserDB_hpp

namespace USER{
    #include "./userframe/Cursor.hpp"
}
#include<string_view>

class UserDB : USER::Cursor{
    public:
        UserDB(std::string_view filename)
        :   Cursor(filename)

        //inputoutput
};

#endif