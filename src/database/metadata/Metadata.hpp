#ifndef Metadata_hpp
#define Metadata_hpp

#include <map>
#include <vector>
#include <array>
#include <string>
#include <string_view>

constexpr uint32_t MAX_TAGLIST_NUM = 1024;

class Metadata {
    private:
    
        std::vector< std::array<uint32_t, MAX_TAGLIST_NUM> > tag_list;
    
    public:
        
        Metadata(std::string_view filename){

        }
};

#endif