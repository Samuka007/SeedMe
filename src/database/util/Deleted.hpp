#ifndef Deleted_hpp
#define Deleted_hpp

#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string_view>
#include<cstring>
#include<set>
#include<array>
#include<string>
#include"util/DatabaseError.hpp"

using std::exception;

constexpr unsigned int LIST_LENGTH = 1024;
constexpr unsigned int SIZE_OF_LIST = sizeof(unsigned int[LIST_LENGTH]);

class Deleted{
    private:
        //unsigned int DeletedList[LIST_LENGTH] {0};
        std::string filename;

        std::set<unsigned int> deleted_set;
    
    public:
        Deleted(std::string filename)
        :   filename(filename + ".deleted") {
            deleted_set.clear();
            ssize_t file_d = open(filename.data(), O_RDWR|O_CREAT, S_IWUSR|S_IRUSR);
            if(file_d == -1){
                throw file_error();
            }
            
            if( lseek(file_d, 0, SEEK_SET) == -1){
                throw file_error();
            } 
            std::array<unsigned int, LIST_LENGTH> buf {0};
            if( read(file_d, buf.data(), SIZE_OF_LIST) == -1){
                throw file_error();
            }
            
            close(file_d);
            deleted_set.insert(buf.begin(), buf.end());
            deleted_set.erase(0);
        }
        ~Deleted(){
            ssize_t file_d = open(filename.data(), O_RDWR|O_CREAT|O_TRUNC, S_IWUSR|S_IRUSR);
            std::array<unsigned int, LIST_LENGTH> buf {0};
            std::copy(deleted_set.begin(), deleted_set.end(), buf.begin());
            close(file_d);
        }

        bool insert(unsigned int id){
            return deleted_set.insert(id).second;
        }

        bool erase(unsigned int id){
            return deleted_set.erase(id);
        }

        bool contains(unsigned int id) const {
            return deleted_set.contains(id);
        }

        unsigned int get(){
            if(!deleted_set.empty())
            {
                unsigned empty_id = *deleted_set.begin();
                deleted_set.erase(empty_id);
                return empty_id;
            }
            else{ return 0; }
        }
};

constexpr size_t DELETED_SIZE = sizeof(Deleted);

#endif /*Deleted.hpp*/