#ifndef Page_hpp
#define Page_hpp
#include <cstring>
#include <string>
#include <array>
#include <map>
#include <set>
#include <queue>
#include <iostream>
#include <memory>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h> 
#include <errno.h>
#include "database/util/Table.hpp"
#include "util/ErrorHandler.hpp"
using std::array;
using std::shared_ptr;

// typedef int int; // int is the number of page

//Row Type T
template <typename T>
class Pager{
    public:
        static constexpr int page_size = 4096;
        static constexpr int class_size = 32;
        static constexpr int rows_per_page = (page_size-class_size) / sizeof(T);

        Pager(int p, shared_ptr<int> f_d)
        :p_num(p), file_descriptor(f_d)
        {
            //read in file
            lseek(*file_descriptor, p_num * page_size, SEEK_SET);
            ssize_t bytes_read = read(*file_descriptor, this, page_size);
            if(bytes_read == -1){
                std::cerr<< "Error reading file: " << errno << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        virtual ~Pager() {
            //write back to file
            lseek(*file_descriptor, p_num * page_size, SEEK_SET);
            ssize_t bytes_written = write(*file_descriptor, this, page_size);
            if(bytes_written == -1){
                std::cerr<< "Error writing file: " << errno << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        T& operator[] (int row_num) {
            call_frequency += 1;
            return rows.at(row_num);
        }

        void push_back(T r){
            rows[row_num] = r;
            row_num += 1;
            //what if is full?
        }

        bool operator< (const Pager& p) const{
            return call_frequency < p.call_frequency;
        }

        inline int page_num() { return p_num; }
        
    private:
        int p_num; //number of page

        array<T, rows_per_page> rows;
        
        shared_ptr<int> file_descriptor;
        
        int row_num = 0;

        unsigned call_frequency = 0;
};

////template page should have static page_size, static rows_per_page
//template <typename Page>
//concept PageType = requires(Page p){
//    {p.page_size} -> std::convertible_to<size_t>;
//    {p.rows_per_page} -> std::convertible_to<size_t>;
//};

#endif /*Page_hpp*/