#ifndef SrcRow_hpp
#define SrcRow_hpp

#include <string_view>
#include <cstring>
#include "database/frame/Row.hpp"

namespace Src{
constexpr size_t LENGTH_OF_NAME   = 160;
constexpr size_t LENGTH_OF_MAGNET = 64;
constexpr uint32_t ID_SIZE = sizeof(uint32_t);
constexpr uint32_t NAME_SIZE = sizeof(char[LENGTH_OF_NAME]);
constexpr uint32_t MAGNET_SIZE = sizeof(char[LENGTH_OF_MAGNET]);
constexpr uint32_t ID_OFFSET = 0;
constexpr uint32_t NAME_OFFSET = ID_OFFSET + ID_SIZE;
constexpr uint32_t MAGNET_OFFSET = NAME_OFFSET + NAME_SIZE;
constexpr uint32_t ROW_SIZE = ID_SIZE + NAME_SIZE + MAGNET_SIZE;

class SrcRow : public Row { //32+160+64 = 256
public:
    SrcRow() = default;
    SrcRow(uint32_t id, std::string_view name, std::string_view magnet)
    :   ID(id)
    {
        setName(name.substr(0,LENGTH_OF_NAME-1).data());
        setMagnet(magnet.substr(0,LENGTH_OF_MAGNET-1).data());
    }

    constexpr uint32_t GET_ROW_SIZE() override { return ROW_SIZE;}

    uint32_t getID() { return ID;}
    char* getName() { return Name;}
    char* getMagnet() { return Magnet;}

    SrcRow& setID(uint32_t id) { ID = id;return *this;}
    SrcRow& setName(const char* name){ std::strcpy(Name, name);return *this;}
    SrcRow& setMagnet(const char* mg){std::strcpy(Magnet, mg);return *this;}
    
    void serialize (void* destination) override {
        std::memcpy(static_cast<char*>(destination) + ID_OFFSET, &(this -> ID), ID_SIZE);
        std::memcpy(static_cast<char*>(destination) + NAME_OFFSET, this -> Name, NAME_SIZE);
        std::memcpy(static_cast<char*>(destination) + MAGNET_OFFSET, this -> Magnet, MAGNET_SIZE);
    }
    void deserialize(const void* source) override {
        std::memcpy(&(this -> ID), static_cast<const char*>(source) + ID_OFFSET, ID_SIZE);
        std::memcpy(this -> Name, static_cast<const char*>(source) + NAME_OFFSET, NAME_SIZE);
        std::memcpy(this -> Magnet, static_cast<const char*>(source) + MAGNET_OFFSET, MAGNET_SIZE);
    } 
    //TODO: illegal check
private:
    uint32_t ID;
//Metadata:
    char Name   [LENGTH_OF_NAME];
    char Magnet [LENGTH_OF_MAGNET];
    //uint8_t Rate;
};
};
using Src::SrcRow;
#endif