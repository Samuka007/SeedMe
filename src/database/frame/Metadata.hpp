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
        std::map<std::string, std::set<unsigned int>> tag_map;
    
    public:
        
        constexpr static unsigned int TAG_LENGTH = 32;
        constexpr static unsigned int MAX_TAGMAP_NUM = 1023;
        constexpr static unsigned int SIZE_OF_NAME = sizeof(char[TAG_LENGTH]);
        constexpr static unsigned int SIZE_OF_LIST = sizeof(int[MAX_TAGMAP_NUM]);
        constexpr static unsigned int SIZE_OF_TAG = SIZE_OF_NAME + SIZE_OF_LIST;

        Metadata(std::string_view filename);
        ~Metadata();

        void Reflash(Table<SrcRow> &);

        void Log(unsigned int, std::string_view);

        void Delete(unsigned int);

        void Add_tag(std::string_view);

        void Remove_tag(std::string_view);

        std::set<unsigned int> get_src_by_tag(std::string_view);

        std::vector<std::string> get_tag_list();
};
#endif