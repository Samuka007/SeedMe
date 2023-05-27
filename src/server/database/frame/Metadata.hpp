#ifndef Metadata_hpp
#define Metadata_hpp

#include <map>
#include <set>
#include <array>
#include <string>
#include <string_view>

#include "../dataframe/DataPre.hpp"

constexpr uint32_t TAG_LENGTH = 32;
constexpr uint32_t MAX_TAGMAP_NUM = 1023;
constexpr uint32_t SIZE_OF_NAME = sizeof(char[TAG_LENGTH]);
constexpr uint32_t SIZE_OF_LIST = sizeof(int[MAX_TAGMAP_NUM]);
constexpr uint32_t SIZE_OF_TAG = SIZE_OF_NAME + SIZE_OF_LIST;
        

class Metadata {
    private:

        std::string filename;
        std::map<std::string, std::set<uint32_t>> tag_map;
    
    public:
        
        Metadata(std::string_view filename);
        ~Metadata();

        void Reflash(const Table<SrcRow> &);

        void Log(const SrcRow &);

        void Delete(const SrcRow &);

        void Add_tag(std::string);

        void Remove_tag(std::string);

};

#endif