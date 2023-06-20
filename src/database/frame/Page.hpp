#ifndef Page_hpp
#define Page_hpp
#include <unistd.h> 
#include "database/frame/Table.hpp"
#include "database/frame/ErrorHandler.hpp"


class Page : Table {
    private:
        int p_file_d;
    public:
        Page(int file_d) : p_file_d {file_d} {

        }
};

#endif /*Page_hpp*/