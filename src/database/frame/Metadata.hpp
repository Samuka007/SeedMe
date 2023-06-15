#ifndef Metadata_hpp
#define Metadata_hpp

#include <map>
#include <set>
#include <array>
#include <string>
#include <string_view>

#include "database/data/Data.hpp"

class Metadata {

    private:

        std::string filename;
        std::map<std::string, std::set<uint32_t>> tag_map;
    
    public:
        
        constexpr static uint32_t TAG_LENGTH = 32;
        constexpr static uint32_t MAX_TAGMAP_NUM = 1023;
        constexpr static uint32_t SIZE_OF_NAME = sizeof(char[TAG_LENGTH]);
        constexpr static uint32_t SIZE_OF_LIST = sizeof(int[MAX_TAGMAP_NUM]);
        constexpr static uint32_t SIZE_OF_TAG = SIZE_OF_NAME + SIZE_OF_LIST;

        Metadata(std::string_view filename);
        ~Metadata();

        void Reflash(Table<SrcRow> &);

        void Log(uint32_t, std::string_view);

        void Delete(uint32_t);

        void Add_tag(std::string_view);

        void Remove_tag(std::string_view);

        std::set<uint32_t> get_src_by_tag(std::string_view);

        std::vector<std::string> get_tag_list();
};
#endif