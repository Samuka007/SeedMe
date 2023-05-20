#ifndef Table_hpp
#define Table_hpp

#include "./Pager.hpp"

class Table : Pager {

private:

    uint32_t num_rows;

    //void *pages[TABLE_MAX_PAGES];

public:

    Table(std::string_view filename);

    ~Table();
    //nolonger used
    char* row_slot(uint32_t row_num);

    void push_back(const Row& row);

    uint32_t getSum() {return num_rows;}

};
#endif