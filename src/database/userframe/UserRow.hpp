#ifndef UserRow_hpp
#define UserRow_hpp

#include<cstring>
#include<string_view>
#include<database/frame/Row.hpp>

namespace User{
constexpr size_t LENGTH_OF_NAME   = 32;
constexpr size_t LENGTH_OF_PASSWORD = 64;
constexpr size_t MAX_OWN_SOURCE = 384;

constexpr uint32_t ID_SIZE = sizeof(uint32_t);
constexpr uint32_t NAME_SIZE = sizeof(char[LENGTH_OF_NAME]);
constexpr uint32_t PASSWORD_SIZE = sizeof(char[LENGTH_OF_PASSWORD]);
constexpr uint32_t SOURCE_SIZE = sizeof(uint32_t[MAX_OWN_SOURCE]);
constexpr uint32_t ID_OFFSET = 0;
constexpr uint32_t NAME_OFFSET = ID_OFFSET + ID_SIZE;
constexpr uint32_t PASSWORD_OFFSET = NAME_OFFSET + NAME_SIZE;
constexpr uint32_t SOURCE_OFFSET = PASSWORD_OFFSET + PASSWORD_SIZE;
constexpr uint32_t ROW_SIZE = ID_SIZE + NAME_SIZE + PASSWORD_SIZE + SOURCE_SIZE;

class UserRow : public Row {
public:
    
    UserRow()=default;
    UserRow(uint32_t id, std::string_view name, std::string_view password)
    :   ID(id)
    {
        setName(name.substr(0,LENGTH_OF_NAME-1).data());
        setPassword(password.substr(0,LENGTH_OF_PASSWORD-1).data());
    }

    constexpr uint32_t GET_ROW_SIZE() override { return ROW_SIZE;}

    uint32_t getID() { return ID;}
    char* getName() { return Name;}
    char* getPassword() { return Password;}
    uint32_t* getSource() { return Source;}

    UserRow& setID(uint32_t id) { ID = id; return *this;}
    UserRow& setName(const char* name){ std::strcpy(Name, name); return *this;}
    UserRow& setPassword(const char* pw){std::strcpy(Password, pw); return *this;}
    UserRow& setSource(uint32_t* src){std::memcpy(Source, src, MAX_OWN_SOURCE); return *this;}

    void serialize (void* destination) override{
        std::memcpy(static_cast<char*>(destination) + ID_OFFSET, &(this -> ID), ID_SIZE);
        std::memcpy(static_cast<char*>(destination) + NAME_OFFSET, this -> Name, NAME_SIZE);
        std::memcpy(static_cast<char*>(destination) + PASSWORD_OFFSET, this -> Password, PASSWORD_SIZE);
        std::memcpy(static_cast<char*>(destination) + SOURCE_OFFSET, this -> Source, SOURCE_SIZE);
    }

    void deserialize(const void* source) override{
        std::memcpy(&(this -> ID), static_cast<const char*>(source) + ID_OFFSET, ID_SIZE);
        std::memcpy(this -> Name, static_cast<const char*>(source) + NAME_OFFSET, NAME_SIZE);
        std::memcpy(this -> Password, static_cast<const char*>(source) + PASSWORD_OFFSET, PASSWORD_SIZE);
        std::memcpy(this -> Source, static_cast<const char*>(source) + SOURCE_OFFSET, SOURCE_SIZE);
    }
    //TODO: illegal check
private:
    uint32_t ID;
//Metadata:
    char Name       [LENGTH_OF_NAME];
    char Password   [LENGTH_OF_PASSWORD];
    uint32_t  Source[MAX_OWN_SOURCE] = {0};
    //uint8_t Rate;
};
};
using User::UserRow;
#endif