#ifndef Metadata_hpp
#define Metadata_hpp

#include <map>
#include <set>
#include <array>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <memory>
#include <vector>
#include <algorithm>
#include "database/data/Data.hpp"

class Metadata {

    private:

        std::string filename;
        std::map<std::string, std::set<unsigned int>> tag_map;
    
    public:
        
        constexpr static unsigned int TAG_LENGTH = 32;
        constexpr static unsigned int MAX_TAGMAP_NUM = 1023;
        constexpr static unsigned int SIZE_OF_NAME = sizeof(char[TAG_LENGTH]);
        constexpr static unsigned int SIZE_OF_LIST = sizeof(int[MAX_TAGMAP_NUM]);
        constexpr static unsigned int SIZE_OF_TAG = SIZE_OF_NAME + SIZE_OF_LIST;

        Metadata(std::string_view filename)
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
                char name[SIZE_OF_NAME] {0};
                rd = read(file_d, name, SIZE_OF_NAME);
                if(rd == -1){
                    throw "read tag error";
                }
                if(rd == 0){
                    break;
                }
                unsigned int list[SIZE_OF_LIST] {0};
                rd = read(file_d, list, SIZE_OF_LIST);
                if(rd == -1){
                    throw "read tag error";
                }
                if(rd == 0){
                    break;
                }
                std::set<unsigned int> tag_set;
                for(auto iter : list){
                    tag_set.insert(iter);
                }
                std::string tag_name {name};
                for(auto iter = tag_name.rbegin(); iter != tag_name.rend(); iter++){
                    if(*iter != 0){
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
        ~Metadata(){
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
                std::array<unsigned int, SIZE_OF_LIST> list {0};
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

        void Reflash(Table<SrcRow> &);

        void Log(unsigned int id, std::string name){
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            for(auto pair : tag_map){
                if(name.find(pair.first)){
                    pair.second.insert(id);
                }
            }
        }

        void Delete(unsigned int id){
        for(auto pair : tag_map){
            pair.second.erase(id);
        }
    }

        void Add_tag(std::string tag){
        std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
        std::set<unsigned int> empty;
        tag_map[tag] = empty;
    }

        void Remove_tag(std::string tag){
            std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
            tag_map.erase(tag);
        }

        std::set<unsigned int> get_src_by_tag(std::string_view tag_name){
            if(tag_map.contains(tag_name.data())){
                return tag_map[tag_name.data()];
            }
            return {};
        }

        std::vector<std::string> get_tag_list(){
            std::vector<std::string> key_list;
            for(auto it = tag_map.begin(); it!=tag_map.end(); ++it){
                key_list.push_back(it->first);
            }
            return key_list;
        }
};
#endif