#ifndef Table_hpp
#define Table_hpp

#include <iostream>
#include <string_view>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h> 
#include <errno.h>
#include <stdexcept>
#include <cstring>
#include <queue>
#include <map>
#include <memory>
#include "util/Page.hpp"
#include "util/ErrorHandler.hpp"

using std::string_view;
using std::string;
using std::priority_queue;
using std::map;
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;

static size_t default_buffer_size = 1 /*G*/* 1024 /*M*/* 1024 /*K*/* 1024 /*byte*/;
template <class T>
class Table{
    public:
        typedef shared_ptr<Pager<T>> page_p; //page_p is the pointer to the page

        Table(string_view filename, size_t buffer_size = default_buffer_size)
        : file_descriptor(std::make_shared<int>(
                            open(filename.data(), O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)
                        )), 
            buffer_page_limit( buffer_size / Pager<T>::page_size )
        {
            if(*file_descriptor == -1){
                throw file_error();
            }
            auto file_length = lseek(*file_descriptor, 0, SEEK_END);
            if(file_length == -1){ 
                throw file_error();
            }
            num_of_row = file_length / Pager<T>::page_size * Pager<T>::rows_per_page;
            if(num_of_row == 0){
                new_row(T {});
            }else{
                for(unsigned i=1;i<num_of_row;++i){
                    if((*this)[i].id == 0){
                        num_of_row = i;
                        break;
                    }
                }
            }
            //pages = vector<Pager<T>> {nullptr};
        }

        inline unsigned last_row() const { return num_of_row - 1; }

        T& operator[] (unsigned row_num)
        {
            if(row_num + 1 > num_of_row){
                throw std::out_of_range("Row number out of range");
            }
            unsigned page_num = row_num / Pager<T>::rows_per_page;
            page_p page = get_page(page_num);
            // offset = row index of page = index of (actual num of row % rows per page)
            unsigned row_offset = (row_num + 1) % Pager<T>::rows_per_page - 1; 
            // std::cout << "row_offset" << row_offset << std::endl;
            return (*page)[row_offset];
        }

        T operator[] (unsigned row_num) const
        {
            if(row_num + 1 > num_of_row){
                throw std::out_of_range("Row number out of range");
            }
            unsigned page_num = row_num / Pager<T>::rows_per_page;
            page_p page = get_page( page_num );
            // offset = row index of page = index of (actual num of row % rows per page)
            unsigned row_offset = (row_num + 1) % Pager<T>::rows_per_page - 1; 
            // std::cout << "row_offset" << row_offset << std::endl;
            return (*page)[row_offset];
        }

        page_p get_page(unsigned page_num) const {
            //TODO: Check page_num legal?
            if(!pages.contains(page_num)){ //page is not in memory
                //read page from file
                page_p temp_page = make_shared<Pager<T>>(page_num, file_descriptor);
                pages[page_num] = temp_page;
                // pages.emplace(page_num, temp_page);
                page_queue.push(temp_page);
            }
            if(pages.size() > buffer_page_limit){ //page is out of memory
                //release page from memory
                pages.erase(page_queue.top()->page_num());
                page_queue.pop();
            }
            return pages[page_num];
        }

        void new_row(T row){
            if(num_of_row % Pager<T>::rows_per_page == 0 ){
                //create new page
                page_p new_page = make_shared<Pager<T>>(
                                    num_of_row / Pager<T>::rows_per_page, file_descriptor
                                );
                pages[(num_of_row + 1) / Pager<T>::rows_per_page] = new_page;
                page_queue.push(new_page);
            }
            // (*pages[num_of_row / Pager<T>::rows_per_page])[num_of_row % Pager<T>::rows_per_page] = row;
            num_of_row += 1;
            (*this)[last_row()] = row;
        }
        
    private:

        shared_ptr<int> file_descriptor;

        //buffer for pages, sorted by page number
        mutable map<int, page_p> pages;

        //buffer list for pages, sorted by call frequency
        mutable priority_queue<page_p> page_queue;

        //unsigned int file_length;

        size_t buffer_page_limit;

        unsigned num_of_row;
};

#endif