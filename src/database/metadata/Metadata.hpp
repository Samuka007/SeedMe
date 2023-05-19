#ifndef Metadata_hpp
#define Metadata_hpp

#include <map>
#include <set>
#include <string>
#include <string_view>

namespace Src{
    #include "../dataframe/Cursor.hpp"
}



class Metadata {
    private:

        constexpr uint32_t TAG_LENGTH = 32;
        constexpr uint32_t MAX_TAGMAP_NUM = 1023;
        constexpr uint32_t SIZE_OF_TAG = 
            (sizeof(char[TAG_LENGTH]) + sizeof(std::array<uint32_t, MAX_TAGMAP_NUM>));
        
        std::map<std::string, std::array<uint32_t, MAX_TAGMAP_NUM>> tag_map;
    
    public:
        
        Metadata(std::string_view filename);

        void Ergodic(const Src::Cursor &);

        void Log(const Src::Row &);

        void Delete(const Src::Row &);

        void Serialize(void* target);
};

#endif