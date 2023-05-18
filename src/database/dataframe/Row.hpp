#ifndef Row_hpp
#define Row_hpp

#include<memory>

constexpr size_t LENGTH_OF_NAME   = 160;
constexpr size_t LENGTH_OF_MAGNET = 64;

template<typename Struct, typename Attribute>
constexpr uint32_t size_of_attribute() {
    return sizeof(decltype(std::declval<Struct*>()->Attribute));
}

constexpr uint32_t ID_SIZE = size_of_attribute(&Row::ID);
constexpr uint32_t NAME_SIZE = size_of_attribute(&Row::Name);
constexpr uint32_t MAGNET_SIZE = size_of_attribute(&Row::Magnet);
constexpr uint32_t ID_OFFSET = 0;
constexpr uint32_t NAME_OFFSET = ID_OFFSET + ID_SIZE;
constexpr uint32_t MAGNET_OFFSET = NAME_OFFSET + NAME_SIZE;
constexpr uint32_t ROW_SIZE = ID_SIZE + NAME_SIZE + MAGNET_SIZE;

class Row { //32+160+64 = 256
private:
    uint32_t ID;
//Metadata:
    char Name   [LENGTH_OF_NAME];
    char Magnet [LENGTH_OF_MAGNET];
    //uint8_t Rate;
public:

    uint32_t getID() { return ID;}
    char* getName() { return Name;}
    char* getMagnet() { return Magnet;}

    void setID(uint32_t id) { ID = id;}
    void setName(char* name){ std::strcpy(Name, name);}
    void setMagnet(char* mg){std::strcpy(Magnet, mg);}
    
    void serialize(void* destination);
    void deserialize(const void* source);
    //TODO: illegal check
};

constexpr uint32_t TABLE_MAX_PAGES = 4096;
constexpr uint32_t PAGE_SIZE = 4096;
constexpr uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
constexpr uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

#endif