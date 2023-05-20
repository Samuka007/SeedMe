#include"UserRow.hpp"

void UserRow::serialize(void* destination) {
    std::memcpy(static_cast<char*>(destination) + ID_OFFSET, this -> ID, ID_SIZE);
    std::memcpy(static_cast<char*>(destination) + NAME_OFFSET, this -> Name, NAME_SIZE);
    std::memcpy(static_cast<char*>(destination) + PASSWORD_OFFSET, this -> Password, PASSWORD_SIZE);
    std::memcpy(static_cast<char*>(destination) + SOURCE_OFFSET, this -> Source, SOURCE_SIZE);
}

void UserRow::deserialize(const void* source) {
    std::memcpy(this -> id, static_cast<const char*>(source) + ID_OFFSET, ID_SIZE);
    std::memcpy(this -> Name, static_cast<const char*>(source) + NAME_OFFSET, NAME_SIZE);
    std::memcpy(this -> Password, static_cast<const char*>(source) + PASSWORD_OFFSET, PASSWORD_SIZE);
    std::memcpy(this -> Source, static_cast<const char*>(source) + SOURCE_OFFSET, SOURCE_SIZE);
}