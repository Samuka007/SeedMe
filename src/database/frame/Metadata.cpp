#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<cstring>
#include<memory>

#include"Metadata.hpp"

Metadata::Metadata(std::string_view filename)
:   filename(filename) {
    ssize_t file_d = open(filename.data(), O_RDWR|O_CREAT, S_IWUSR|S_IRUSR);
    //file readin
    if(file_d == -1){
        throw "read error.";
    }

    if(lseek(file_d, 0, SEEK_SET) == -1){
        throw "seek error.";
    }
    //std::array<uint32_t, MAX_TAGMAP_NUM> buf;
    /**
     *  void check(ssize_t result) {
            if (result == -1) {
                throw std::system_error(errno, std::system_category());
            }
        }
    */

    ssize_t rd = 1;
    while(true){
        std::array<char, SIZE_OF_NAME> name;
        rd = read(file_d, name.data(), SIZE_OF_NAME);
        if(rd == -1){
            throw "read tag error";
        }
        if(rd == 0){
            break;
        }
        uint32_t list[SIZE_OF_LIST] {0};
        rd = read(file_d, list, SIZE_OF_LIST);
        if(rd == -1){
            throw "read tag error";
        }
        if(rd == 0){
            break;
        }
        std::set<uint32_t> tag_set;
        for(auto iter : list){
            tag_set.insert(iter);
        }
        tag_map[name] = tag_set;
    }
    if(close(file_d) == -1){
        throw "close error.";
    }
}

Metadata::~Metadata(){
    ssize_t file_d = open(filename.data(), O_RDWR|O_CREAT|O_TRUNC, S_IWUSR|S_IRUSR);
    //file readin
    if(file_d == -1){
        throw "read error.";
    }

    if(lseek(file_d, 0, SEEK_SET) == -1){
        throw "seek error.";
    }
    //std::array<uint32_t, MAX_TAGMAP_NUM> buf;
    /**
     *  void check(ssize_t result) {
            if (result == -1) {
                throw std::system_error(errno, std::system_category());
            }
        }
    */

    ssize_t rd = 1;
    while(true){
        std::array<char, SIZE_OF_NAME> name;
        rd = read(file_d, name.data(), SIZE_OF_NAME);
        if(rd == -1){
            throw "read tag error";
        }
        if(rd == 0){
            break;
        }
        uint32_t list[SIZE_OF_LIST] {0};
        rd = read(file_d, list, SIZE_OF_LIST);
        if(rd == -1){
            throw "read tag error";
        }
        if(rd == 0){
            break;
        }
        std::set<uint32_t> tag_set;
        for(auto iter : list){
            tag_set.insert(iter);
        }
        tag_map[name] = tag_set;
    }
    if(close(file_d) == -1){
        throw "close error.";
    }
}

void 