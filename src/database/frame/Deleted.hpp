#pragma once

#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string_view>
#include<cstring>
#include<set>
#include<array>
#include<string>

constexpr uint32_t LIST_LENGTH = 1024;
constexpr uint32_t SIZE_OF_LIST = sizeof(uint32_t[LIST_LENGTH]);

class Deleted{
    private:
        //uint32_t DeletedList[LIST_LENGTH] {0};
        std::string filename;

        std::set<uint32_t> deleted_set;
    
    public:
        Deleted(std::string filename)
        :   filename(filename) {
            filename += ".deleted";
            ssize_t file_d = open(filename.data(), O_RDWR|O_CREAT, S_IWUSR|S_IRUSR);
            if(file_d == -1){
                throw "read error.";
            }
            
            if( lseek(file_d, 0, SEEK_SET) == -1){
                throw "seek error.";
            } 
            std::array<uint32_t, LIST_LENGTH> buf;
            if( read(file_d, buf.data(), SIZE_OF_LIST) == -1){
                throw "read list error.";
            }
            
            close(file_d);
            deleted_set.insert(buf.begin(), buf.end());
        }
        ~Deleted(){
            ssize_t file_d = open(filename.data(), O_RDWR|O_CREAT|O_TRUNC, S_IWUSR|S_IRUSR);
            if(file_d == -1){
                throw "read error.";
            }

            if( lseek(file_d, 0, SEEK_SET) == -1){
                throw "seek error.";
            } 

            std::array<uint32_t, LIST_LENGTH> buf {0};
            std::copy(deleted_set.begin(), deleted_set.end(), buf.begin());
            if( write(file_d, buf.data(), SIZE_OF_LIST) == -1){
                throw "write list error.";
            }
            close(file_d);
        }

        bool insert(uint32_t id){
            return deleted_set.insert(id).second;
        }

        bool erase(uint32_t id){
            return deleted_set.erase(id);
        }

        bool contains(uint32_t id){
            return deleted_set.contains(id);
        }

        uint32_t get(uint32_t id){
            if(!deleted_set.empty()){
                return *deleted_set.begin();
            }else{
                return 0;
            }
        }
};