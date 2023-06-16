#ifndef UserRow_hpp
#define UserRow_hpp

#include<cstring>
#include<string_view>

class UserRow {
public:
    
    constexpr static size_t LENGTH_OF_NAME   = 32;
    constexpr static size_t LENGTH_OF_PASSWORD = 64;
    constexpr static size_t MAX_OWN_SOURCE = 384;
    constexpr static unsigned int ID_SIZE = sizeof(unsigned int);
    constexpr static unsigned int NAME_SIZE = sizeof(char[LENGTH_OF_NAME]);
    constexpr static unsigned int PASSWORD_SIZE = sizeof(char[LENGTH_OF_PASSWORD]);
    constexpr static unsigned int SOURCE_SIZE = sizeof(unsigned int[MAX_OWN_SOURCE]);
    constexpr static unsigned int ID_OFFSET = 0;
    constexpr static unsigned int NAME_OFFSET = ID_OFFSET + ID_SIZE;
    constexpr static unsigned int PASSWORD_OFFSET = NAME_OFFSET + NAME_SIZE;
    constexpr static unsigned int SOURCE_OFFSET = PASSWORD_OFFSET + PASSWORD_SIZE;
    constexpr static unsigned int ROW_SIZE = ID_SIZE + NAME_SIZE + PASSWORD_SIZE + SOURCE_SIZE;

    UserRow()=default;
    UserRow(unsigned int id, std::string_view name, std::string_view password)
    :   ID(id)
    {
        setName(name.substr(0,LENGTH_OF_NAME-1).data());
        setPassword(password.substr(0,LENGTH_OF_PASSWORD-1).data());
    }

    UserRow(void* cur){deserialize(cur);}

    //constexpr unsigned int GET_ROW_SIZE() override { return ROW_SIZE;}

    unsigned int getID() { return ID;}
    char* getName() { return Name;}
    char* getPassword() { return Password;}
    unsigned int* getSource() { return Source;}

    UserRow& setID(unsigned int id) { ID = id; return *this;}
    UserRow& setName(const char* name){ std::strcpy(Name, name); return *this;}
    UserRow& setPassword(const char* pw){std::strcpy(Password, pw); return *this;}
    UserRow& setSource(unsigned int* src){std::memcpy(Source, src, MAX_OWN_SOURCE); return *this;}

    void serialize (void* destination) {
        std::memcpy(static_cast<char*>(destination) + ID_OFFSET, &(this -> ID), ID_SIZE);
        std::memcpy(static_cast<char*>(destination) + NAME_OFFSET, &(this -> Name), NAME_SIZE);
        std::memcpy(static_cast<char*>(destination) + PASSWORD_OFFSET, &(this -> Password), PASSWORD_SIZE);
        std::memcpy(static_cast<char*>(destination) + SOURCE_OFFSET, &(this -> Source), SOURCE_SIZE);
    }

    void deserialize(const void* source) {
        std::memcpy(&(this -> ID), static_cast<const char*>(source) + ID_OFFSET, ID_SIZE);
        std::memcpy(&(this -> Name), static_cast<const char*>(source) + NAME_OFFSET, NAME_SIZE);
        std::memcpy(&(this -> Password), static_cast<const char*>(source) + PASSWORD_OFFSET, PASSWORD_SIZE);
        std::memcpy(&(this -> Source), static_cast<const char*>(source) + SOURCE_OFFSET, SOURCE_SIZE);
    }
    //TODO: illegal check
private:
    unsigned int ID;
//Metadata:
    char Name       [LENGTH_OF_NAME];
    char Password   [LENGTH_OF_PASSWORD];
    unsigned int  Source[MAX_OWN_SOURCE] = {0};
    //uint8_t Rate;
};
#endif