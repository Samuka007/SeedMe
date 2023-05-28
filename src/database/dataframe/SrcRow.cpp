#include"SrcRow.hpp"

#include<cstring>

void SrcRow::serialize(void* destination) override {
    std::memcpy(static_cast<char*>(destination) + ID_OFFSET, this -> ID, ID_SIZE);
    std::memcpy(static_cast<char*>(destination) + NAME_OFFSET, this -> Name, NAME_SIZE);
    std::memcpy(static_cast<char*>(destination) + MAGNET_OFFSET, this -> Magnet, MAGNET_SIZE);
}

void SrcRow::deserialize(const void* source) override {
    std::memcpy(this -> id, static_cast<const char*>(source) + ID_OFFSET, ID_SIZE);
    std::memcpy(this -> Name, static_cast<const char*>(source) + NAME_OFFSET, NAME_SIZE);
    std::memcpy(this -> Magnet, static_cast<const char*>(source) + MAGNET_OFFSET, MAGNET_SIZE);
}