#include "Cursor.hpp"

Cursor::Cursor(std::string_view filename, bool option)
    :   Table(filename) {
    if (option){
        // start at the beginning of the table
        row_num = 0;
        end_of_table = (num_rows == 0);
    }else{
        // end of the table
        row_num = num_rows;
        end_of_table = true;
    }
}

void* Cursor::getpoint(){
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    auto page = static_cast<char*>(pager.get_page(page_num)/*should add error checking*/);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}

Row& Cursor::deserialize(){
    Row temp;
    temp.deserialize(getpoint());
    return temp;
}

void Cursor::next(){
    row_num += 1;
    if (row_num >= num_rows){
        end_of_table = true;
    }
}