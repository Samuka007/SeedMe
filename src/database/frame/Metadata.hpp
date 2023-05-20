#ifndef Metadata_hpp
#define Metadata_hpp

#include <map>
#include <set>
#include <array>
#include <string>
#include <string_view>

#include "../dataframe/SrcRow.hpp"
#include "./Table.hpp"


class Metadata {
    private:

        constexpr uint32_t TAG_LENGTH = 32;
        constexpr uint32_t MAX_TAGMAP_NUM = 1023;
        constexpr uint32_t SIZE_OF_NAME = sizeof(char[TAG_LENGTH]);
        constexpr uint32_t SIZE_OF_LIST = sizeof(int[MAX_TAGMAP_NUM]);
        constexpr uint32_t SIZE_OF_TAG = SIZE_OF_NAME + SIZE_OF_LIST;
        
        std::string filename;
        std::map<std::array<char, SIZE_OF_NAME>, std::set<uint32_t>> tag_map;
    
    public:
        
        Metadata(std::string_view filename);
        ~Metadata();

        void Ergodic(const Table<SrcRow> &);

        void Log(const Row &);

        void Delete(const Row &);

        void Remove_tag(std::string_view Tag);

};

#endif