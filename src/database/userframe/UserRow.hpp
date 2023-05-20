#ifndef UserRow_hpp
#define UserRow_hpp

#include<cstring>
#include<string_view>



class UserRow {
public:

constexpr size_t LENGTH_OF_NAME   = 32;
constexpr size_t LENGTH_OF_PASSWORD = 64;
constexpr size_t MAX_OWN_SOURCE = 384;

constexpr uint32_t ID_SIZE = sizeof(ID);
constexpr uint32_t NAME_SIZE = sizeof(Name);
constexpr uint32_t PASSWORD_SIZE = sizeof(Password);
constexpr uint32_t SOURCE_SIZE = sizeof(Source);
constexpr uint32_t ID_OFFSET = 0;
constexpr uint32_t NAME_OFFSET = ID_OFFSET + ID_SIZE;
constexpr uint32_t PASSWORD_OFFSET = NAME_OFFSET + NAME_SIZE;
constexpr uint32_t SOURCE_OFFSET = PASSWORD_OFFSET + PASSWORD_SIZE;
constexpr uint32_t ROW_SIZE = ID_SIZE + NAME_SIZE + PASSWORD_SIZE + SOURCE_SIZE;

    UserRow()=default;
    UserRow(uint32_t id, std::string_view name, std::string_view password)
    :   ID(id), 
        Name(name.substr(0,LENGTH_OF_NAME-1).data()), 
        Password(password.substr(0,LENGTH_OF_PASSWORD-1).data()) 
    {}

    uint32_t getID() { return ID;}
    char* getName() { return Name;}
    char* getPassword() { return Password;}
    uint32_t* getSource() { return Source;}

    UserRow& setID(uint32_t id) { ID = id; return this;}
    UserRow& setName(char* name){ std::strcpy(Name, name); return this;}
    UserRow& setPassword(char* pw){std::strcpy(Password, pw); return this;}
    UserRow& setSource(uint32_t* src){std::memcpy(Source, src, MAX_OWN_SOURCE); return this;}

    void serialize(void* destination);
    void deserialize(const void* source);
    //TODO: illegal check
private:
    uint32_t ID;
//Metadata:
    char Name       [LENGTH_OF_NAME];
    char Password   [LENGTH_OF_PASSWORD];
    uint32_t  Source[MAX_OWN_SOURCE];
    //uint8_t Rate;
};

#endif