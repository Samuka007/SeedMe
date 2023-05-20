#ifndef Pager_hpp
#define Pager_hpp

#include <string_view>

#include"./Row.hpp"

class Pager
{
private:

    int file_descriptor;

    uint32_t file_length;

public:

    void* pages[TABLE_MAX_PAGES];

    Pager(std::string_view filename);

    void* get_page(uint32_t page_num);

    void pager_flush(uint32_t page_num, uint32_t size);

};

#endif