#ifndef Cursor_hpp
#define Cursor_hpp

#include "./Table.hpp"
#include<string_view>

class Cursor : Table{
private:

    uint32_t row_num;
    
    bool end_of_table;

public:

    Cursor(std::string_view filename, bool option);
    
    void* getpoint();
    
    uint32_t    getID()     { return deserialize().getID();}
    char*       getMagnet() { return deserialize().getMagnet();}
    char*       getName()   { return deserialize().getName();}

    void setName(char* name)    { deserialize().setName(name);}
    void setMagnet(char* mg){ deserialize().setMagnet(mg);}

    void next();
};

#endif