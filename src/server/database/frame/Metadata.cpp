#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<cstring>
#include<memory>
#include<algorithm>

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
    /**
     *  void check(ssize_t result) {
            if (result == -1) {
                throw std::system_error(errno, std::system_category());
            }
        }
    */

    ssize_t rd = 1;
    while(true){
        std::array<char, SIZE_OF_NAME> name {0};
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
        std::string tag_name = name;
        for(auto& iter = tag_name.rbegin(), iter != tag_name.rend(); iter++){
            if(iter != 0){
                break;
            }else{
                tag_name.pop_back();
            }
        }
        tag_map[tag_name] = tag_set;
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

    ssize_t rd = 1;
    for(auto pair : tag_map){
        std::array<char, SIZE_OF_NAME> name {0};
        std::copy(pair.first.begin(), pair.first.end(), name.begin());
        rd = write(file_d, name.data(), SIZE_OF_NAME);
        if(rd == -1){
            throw "read tag error";
        }
        if(rd == 0){
            break;
        }
        std::array<uint32_t, SIZE_OF_LIST> list {0};
        std::copy(pair.second.begin(), pair.second.end(), list.begin());
        rd = read(file_d, list.data(), SIZE_OF_LIST);
        if(rd == -1){
            throw "read tag error";
        }
        if(rd == 0){
            break;
        }
    }
    if(close(file_d) == -1){
        throw "close error.";
    }
}

void Metadata::Log(const SrcRow &row){
    std::string name = row.getName();
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    for(auto pair : tag_map){
        if(name.find(pair.first)){
            pair.second.insert(row.getID());
        }
    }
}

void Metadata::Delete(const SrcRow &row){
    for(auto pair : tag_map){
        pair.second.erase(row.getID());
    }
}

void Metadata::Add_tag(std::string tag){
    std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
    std::set<uint32_t> empty;
    tag_map[tag] = empty;
}

void Metadata::Remove_tag(std::string tag){
    std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
    tag_map.erase(tag);
}

void Metadata::Reflash(const Table<SrcRow> &table){
    for(uint32_t i = 0; i < table.getSum(); ++i){
        Log(DataPre::getName(table.row_data(i)));
    }
}