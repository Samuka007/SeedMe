#include"Pager.hpp"

#include<iostream>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h> 

//#include"Row.hpp"

Pager::Pager(std::string_view filename){
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

void* Pager::get_page(uint32_t page_num){
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

void Pager::pager_flush(uint32_t page_num, uint32_t size){
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