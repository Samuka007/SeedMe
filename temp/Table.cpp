#include "Table.hpp"

Table::Table(std::string_view filename) 
    : num_rows(0), Pager(filename) {
    num_rows = file_length / ROW_SIZE;
}

Table::~Table(){
}
/**/
void* Table::row_data(uint32_t row_ID) {
    uint32_t page_num = row_ID / ROWS_PER_PAGE;
    void* page = get_page(page_num);

    uint32_t row_offset = row_ID % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return /*(char*)*/page + byte_offset;
}
//TODO: Unused ID
void Table::push_back(const Row& row){
    if (num_rows >= TABLE_MAX_ROWS){
        //std::cerr << "Error: Table full." << std::endl;
        return ;
    }
    void *destination = row_data(num_rows);
    row.serialize(destination);
    ++num_rows;
}
