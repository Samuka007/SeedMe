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
#include <memory>
#include "database/Page.hpp"
#include "database/ErrorHandler.hpp"

using std::string_view;
using std::string;
using std::priority_queue;
using std::map;
using std::shared_ptr;
using std::unique_ptr;

template <class T, class Page = Pager<T>>
class Table{
    public:
        typedef shared_ptr<Page> page_p; //page_p is the pointer to the page
        static constexpr size_t default_buffer_size = 1 /*G*/* 1024 /*M*/* 1024 /*K*/* 1024 /*byte*/;
        using Page::page_size;
        using Page::rows_per_page;

        Table(string_view filename, size_t buffer_size = default_buffer_size)
        : file_descriptor(nullptr), file_length(0), buffer_page_limit(buffer_size / page_size)
        {
            file_descriptor = make_shared<int>(open(filename.c_str(), O_RDWR | O_CREAT, S_IWUSR | S_IRUSR));
            if(*file_descriptor == -1){
                throw FileOpenError("Error opening file: ", errno);
            }
            auto file_length = lseek(*file_descriptor, 0, SEEK_END);
            if(file_length == -1){
                throw FileError("Error seeking file: ", errno);
            }
            last_row_num = file_length / page_size;
            //pages = vector<Page> {nullptr};
        }

        inline unsigned last_row(){
            return last_row_num;
        }

        T& operator[] (unsigned row_num){
            if(row_num > last_row_num){
                throw std::out_of_range("Row number out of range");
            }
            int page_num = row_num / rows_per_page;
            page_p page = get_page(page_num);
            unsigned int row_offset = row_num % rows_per_page - 1;
            return (*page)[row_offset];
        }

        shared_ptr<Page> get_page(unsigned page_num){
            //TODO: Check page_num legal?
            if(!pages.contains(page_num)){ //page is not in memory
                //read page from file
                auto temp_page = make_shared<Page>(page_num, file_descriptor);
                pages[page_num] = make_shared<Page> (temp_page);
                page_queue.push(make_shared<Page> (temp_page));
            }
            if(pages.size() > buffer_page_limit){ //page is out of memory
                //release page from memory
                pages.erase(page_queue.top()->p_num);
                page_queue.pop();
            }
            return pages[page_num];
        }

        void new_row(T r){
            if(last_row_num % rows_per_page == 0){
                //create new page
                auto new_page = make_shared<Page>(last_row_num / rows_per_page, file_descriptor);
                pages[last_row_num / rows_per_page] = new_page;
                page_queue.push(new_page);
            }
            (*pages[last_row_num / rows_per_page])[last_row_num % rows_per_page] = r;
            last_row_num += 1;
        }
        
    private:

        shared_ptr<int> file_descriptor;

        //buffer for pages, sorted by page number
        map<page_t, page_p> pages;

        //buffer list for pages, sorted by call frequency
        priority_queue<page_p> page_queue;

        //unsigned int file_length;

        size_t buffer_page_limit;

        unsigned last_row_num;
};

#endif