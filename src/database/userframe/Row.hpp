#ifndef Row_hpp
#define Row_hpp

#include<cstring>

constexpr size_t LENGTH_OF_NAME   = 32;
constexpr size_t LENGTH_OF_PASSWORD = 64;
constexpr size_t MAX_OWN_SOURCE = 384;

template<typename Struct, typename Attribute>
constexpr uint32_t size_of_attribute() {
    return sizeof(decltype(std::declval<Struct*>()->Attribute));
}

constexpr uint32_t ID_SIZE = size_of_attribute(&Row::ID);
constexpr uint32_t NAME_SIZE = size_of_attribute(&Row::Name);
constexpr uint32_t PASSWORD_SIZE = size_of_attribute(&Row::Password);
constexpr uint32_t SOURCE_SIZE = size_of_attribute(&Row::Source);
constexpr uint32_t ID_OFFSET = 0;
constexpr uint32_t NAME_OFFSET = ID_OFFSET + ID_SIZE;
constexpr uint32_t PASSWORD_OFFSET = NAME_OFFSET + NAME_SIZE;
constexpr uint32_t SOURCE_OFFSET = PASSWORD_OFFSET + PASSWORD_SIZE;
constexpr uint32_t ROW_SIZE = ID_SIZE + NAME_SIZE + PASSWORD_SIZE + SOURCE_SIZE;

class Row {
private:
    uint32_t ID;
//Metadata:
    char Name       [LENGTH_OF_NAME];
    char Password   [LENGTH_OF_PASSWORD];
    uint32_t  Source[MAX_OWN_SOURCE];
    //uint8_t Rate;
public:
    Row();

    uint32_t getID() { return ID;}
    char* getName() { return Name;}
    char* getPassword() { return Password;}
    uint32_t* getSource() { return Source;}

    void setID(uint32_t id) { ID = id;}
    void setName(char* name){ std::strcpy(Name, name);}
    void setPassword(char* pw){std::strcpy(Password, pw);}
    void setSource(uint32_t* src){std::memcpy(Source, src, MAX_OWN_SOURCE);}

    void serialize(void* destination);
    void deserialize(const void* source);
    //TODO: illegal check
};

constexpr uint32_t TABLE_MAX_PAGES = 4096;
constexpr uint32_t PAGE_SIZE = 4096;
constexpr uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
constexpr uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

#endif