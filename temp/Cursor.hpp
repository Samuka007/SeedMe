#pragma once

#include "./Table.hpp"
#include "./Deleted.hpp"

class Cursor : Table{
private:

    uint32_t row_num;
    
    bool end_of_table;

public:

    Cursor(std::string_view filename, bool option);
    
    void* getpoint();

    Row& deserialize();

    uint32_t    getuserID()     { return deserialize().getID();}
    char*       getPassword()   { return deserialize().getPassword();}
    char*       getName()       { return deserialize().getName();}
    uint32_t*   getSource()     { return deserialize().getSource();}

    void setPassword(char* pw)      { deserialize().setPassword(pw);}
    void setName(char* name)        { deserialize().setName(name);}
    void setSource(uint32_t* src)   { deserialize().setSource(src);}
    
    //should set 遍历 function.

    void next();
};
