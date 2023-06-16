#ifndef Cursor_hpp
#define Cursor_hpp

#include "./Table.hpp"
//#include<string_view>


template<class Row>
class Cursor
{
public:
    Table *table;
    unsigned int row_num;
    bool end_of_table;

    Cursor(Table<Row> *&table, bool option);
    void *value();
    void next();
};

template<class Row>
Cursor<Row>::Cursor(Table<Row> *&table, bool option)
{
    this->table = table;
    if (option)
    {
        // start at the beginning of the table
        row_num = 0;
        end_of_table = (table->num_rows == 0);
    }
    else
    {
        // end of the table
        row_num = table->num_rows;
        end_of_table = true;
    }
}

template<class Row>
void *Cursor<Row>::value()
{
    unsigned int page_num = row_num / Table<Row>::ROWS_PER_PAGE;
    void *page = table->pager.get_page(page_num);
    unsigned int row_offset = row_num % Table<Row>::ROWS_PER_PAGE;
    unsigned int byte_offset = row_offset * Row::ROW_SIZE;
    return (char *)page + byte_offset;
}

template<class Row>
void Cursor<Row>::next()
{
    row_num += 1;
    if (row_num >= table->num_rows)
    {
        end_of_table = true;
    }
}

#endif