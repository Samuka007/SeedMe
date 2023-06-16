#ifndef SrcRow_hpp
#define SrcRow_hpp

#include <string_view>
#include <cstring>

class SrcRow { 
public:
    //32+160+64 = 256
    constexpr static size_t LENGTH_OF_NAME   = 160;
    constexpr static size_t LENGTH_OF_MAGNET = 64;
    constexpr static unsigned int ID_SIZE = sizeof(unsigned int);
    constexpr static unsigned int NAME_SIZE = sizeof(char[LENGTH_OF_NAME]);
    constexpr static unsigned int MAGNET_SIZE = sizeof(char[LENGTH_OF_MAGNET]);
    constexpr static unsigned int ID_OFFSET = 0;
    constexpr static unsigned int NAME_OFFSET = ID_OFFSET + ID_SIZE;
    constexpr static unsigned int MAGNET_OFFSET = NAME_OFFSET + NAME_SIZE;
    constexpr static unsigned int ROW_SIZE = ID_SIZE + NAME_SIZE + MAGNET_SIZE;

    SrcRow() = default;
    SrcRow(unsigned int id, std::string_view name, std::string_view magnet)
    :   ID(id)
    {
        setName(name.substr(0,LENGTH_OF_NAME-1).data());
        setMagnet(magnet.substr(0,LENGTH_OF_MAGNET-1).data());
    }

    SrcRow(void* cur){deserialize(cur);}

    //constexpr unsigned int GET_ROW_SIZE() override { return ROW_SIZE;}

    unsigned int getID() { return ID;}
    char* getName() { return Name;}
    char* getMagnet() { return Magnet;}

    SrcRow& setID(unsigned int id) { ID = id;return *this;}
    SrcRow& setName(const char* name){ std::strcpy(Name, name);return *this;}
    SrcRow& setMagnet(const char* mg){std::strcpy(Magnet, mg);return *this;}
    
    void serialize (void* destination) {
        std::memcpy(static_cast<char*>(destination) + ID_OFFSET, &(this -> ID), ID_SIZE);
        std::memcpy(static_cast<char*>(destination) + NAME_OFFSET, &(this -> Name), NAME_SIZE);
        std::memcpy(static_cast<char*>(destination) + MAGNET_OFFSET, &(this -> Magnet), MAGNET_SIZE);
    }
    void deserialize(const void* source) {
        std::memcpy(&(this -> ID), static_cast<const char*>(source) + ID_OFFSET, ID_SIZE);
        std::memcpy(&(this -> Name), static_cast<const char*>(source) + NAME_OFFSET, NAME_SIZE);
        std::memcpy(&(this -> Magnet), static_cast<const char*>(source) + MAGNET_OFFSET, MAGNET_SIZE);
    } 
    //TODO: illegal check
private:
    unsigned int ID;
//Metadata:
    char Name   [LENGTH_OF_NAME];
    char Magnet [LENGTH_OF_MAGNET];
    //uint8_t Rate;
};
#endif