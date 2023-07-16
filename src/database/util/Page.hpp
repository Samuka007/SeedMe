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
#include "util/Table.hpp"
#include "util/DatabaseError.hpp"
using std::array;
using std::shared_ptr;

// typedef int int; // int is the number of page

//Row Type T
template <typename T>
class Pager{
    public:
        static constexpr unsigned page_size = 4096;
        static constexpr unsigned meta_size = sizeof(unsigned) * 2;
        static constexpr unsigned rows_per_page = page_size / sizeof(T);

        Pager(unsigned p, shared_ptr<int> f_d)
        : p_num( p ), file_descriptor( f_d ), rows{}
        {
            //read in file
            off_t seek_p = lseek(*file_descriptor, p_num * page_size, SEEK_SET);
            ssize_t bytes_read;
            unsigned f_p_num;
            bytes_read = read(*file_descriptor, &f_p_num, sizeof(unsigned));
            if ( f_p_num != p_num ) {
                throw file_error("Page number not match");
            }
            bytes_read = read(*file_descriptor, &call_frequency, sizeof(unsigned));
            bytes_read = read(*file_descriptor, rows.data(), rows_per_page*sizeof(T));
            if(bytes_read == -1){
                throw file_error();
            }
        }

        virtual ~Pager() {
            //write back to file
            lseek(*file_descriptor, p_num * page_size, SEEK_SET);
            ssize_t bytes_written;
            bytes_written = write(*file_descriptor, &p_num, sizeof(unsigned));
            bytes_written = write(*file_descriptor, &call_frequency, sizeof(unsigned));
            bytes_written = write(*file_descriptor, rows.data(), rows_per_page*sizeof(T));
        }

        T& operator[] (unsigned row_num) 
        {
            call_frequency += 1;
            return rows.at(row_num);
        }

        // Shouldn't be used
        void push_back(T r)
        {
            rows[row_num] = r;
            row_num += 1;
            //what if is full?
        }

        bool operator< (const Pager& p) const{
            return call_frequency < p.call_frequency;
        }

        inline int page_num() { return p_num; }
        
    private:
        unsigned p_num; //number of page

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