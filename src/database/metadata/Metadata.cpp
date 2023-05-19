#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<memory>

#include"Metadata.hpp"

Metadata::Metadata(std::string_view filename){
    ssize_t file_d = open(filename.data(), O_RDWR|O_CREAT, S_IWUSR|S_IRUSR);
    //file readin
    if(file_d == -1){
        throw "read error.";
    }

    if(lseek(file_d, 0, SEEK_SET) == -1){
        throw "seek error.";
    }

    //std::array<uint32_t, MAX_TAGMAP_NUM> buf;

    int endread = 1;

    do{
        if(endread == -1)
            throw "read tag error.";
        auto buf = std::make_unique<std::array<uint32_t, MAX_TAGMAP_NUM>>;
        
    }while(endread = read(file_d, buf->data(), SIZE_OF_TAG))
}

