#ifndef Table_hpp
#define Table_hpp

#include<iostream>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h> 

//#include "./Pager.hpp"
//#include <string_view>

template<typename ROW>
class Table{
private:

    uint32_t num_rows;

    int file_descriptor;

    uint32_t file_length;

    //void *pages[TABLE_MAX_PAGES];

public:
    constexpr uint32_t TABLE_MAX_PAGES = 4096;
    constexpr uint32_t PAGE_SIZE = 4096;
    constexpr uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW::GET_ROW_SIZE();
    constexpr uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

    Table(std::string_view filename)
    : num_rows(0) {
        num_rows = file_length / ROW::GET_ROW_SIZE();
        file_descriptor = open(filename.data(), O_RDWR|O_CREAT, S_IWUSR|S_IRUSR);
        if (file_descriptor == -1){
            std::cerr << "Error: cannot open file " << filename << std::endl;
            throw "Unabled to open file.";
        }
        file_length = lseek(file_descriptor, 0, SEEK_END);
        for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i){
            pages[i] = nullptr;
        }
    }
        
    ~Table(){
        uint32_t num_full_pages = num_rows / ROWS_PER_PAGE;

        for (uint32_t i = 0; i < num_full_pages; i++)
        {
            if (pages[i] == nullptr)
            {
                continue;
            }
            pager_flush(i, PAGE_SIZE);
            free(pages[i]);
            pages[i] = nullptr;
        }

        // There may be a partial page to write to the end of the file
        // This should not be needed after we switch to a B-tree
        uint32_t num_additional_rows = num_rows % ROWS_PER_PAGE;
        if (num_additional_rows > 0)
        {
            uint32_t page_num = num_full_pages;
            if (pages[page_num] != nullptr)
            {
                pager_flush(page_num, num_additional_rows * ROW::GET_ROW_SIZE());
                free(pages[page_num]);
                pages[page_num] = nullptr;
            }
        }
        int result = close(file_descriptor);
        if (result == -1)
        {
            //std::cout << "Error closing db file." << std::endl;
            //exit(EXIT_FAILURE);
        }
        for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
        {
            void *page = pages[i];
            if (page)
            {
                free(page);
                pages[i] = nullptr;
            }
        }
    }
    
    void* row_data(uint32_t row_num){
        uint32_t page_num = row_num / ROWS_PER_PAGE;
        void* page = get_page(page_num);

        uint32_t row_offset = row_num % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW::GET_ROW_SIZE();
        return /*(char*)*/page + byte_offset;
    }

    void push_back(const ROW& row){
        if (num_rows >= TABLE_MAX_ROWS){
            //std::cerr << "Error: Table full." << std::endl;
            return ;
        }
        void *destination = row_data(num_rows);
        row.serialize(destination);
        ++num_rows;
    }

    uint32_t getSum(){
        return num_rows;
    }

    void* pages[TABLE_MAX_PAGES];

    void* get_page(uint32_t page_num){
        //TODO: Check page_num legal?
        if (page_num > TABLE_MAX_PAGES){
            std::cerr << "Tried to fetch page number out of bounds. " 
                << page_num << " > "
                << TABLE_MAX_PAGES << std::endl;
            return nullptr;
        }
        if(pages[page_num] == nullptr){
            void *page = malloc(PAGE_SIZE);
            uint32_t num_pages = file_length / PAGE_SIZE;
            if(file_length % PAGE_SIZE){
                ++num_pages;
            }
            if(page_num <= num_pages){
                lseek(file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
                ssize_t bytes_read = read(file_descriptor, page, PAGE_SIZE);
                if(bytes_read == -1){
                    std::cerr<< "Error reading file: " << errno << std::endl;
                    return nullptr;
                }
            }
            pages[page_num] = page;
        }
        return pages[page_num];
    }

    void pager_flush(uint32_t page_num, uint32_t size){
        if(pages[page_num] == nullptr){
            //error
        }
        off_t offset = lseek(this->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
        if(offset == -1){
            //error
        }
        ssize_t bytes_written = write(this->file_descriptor, pages[page_num], size);
        if(bytes_written == -1){
            //error
        }
    }

};

#endif