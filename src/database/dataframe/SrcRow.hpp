#ifndef SrcRow_hpp
#define SrcRow_hpp

#include<string_view>

class SrcRow { //32+160+64 = 256
public:
    SrcRow() = default;
    SrcRow(uint32_t id, std::string_view name, std::string_view magnet)
    :   ID(id), 
        Name(name.substr(0,LENGTH_OF_NAME-1).data()), 
        Magnet(magnet.substr(0,LENGTH_OF_MAGNET-1).data()) 
    {}

    uint32_t getID() { return ID;}
    char* getName() { return Name;}
    char* getMagnet() { return Magnet;}

    void setID(uint32_t id) { ID = id;}
    void setName(char* name){ std::strcpy(Name, name);}
    void setMagnet(char* mg){std::strcpy(Magnet, mg);}
    
    void serialize(void* destination);
    void deserialize(const void* source);
    //TODO: illegal check

    constexpr size_t LENGTH_OF_NAME   = 160;
    constexpr size_t LENGTH_OF_MAGNET = 64;

    constexpr uint32_t ID_SIZE = sizeof(ID);
    constexpr uint32_t NAME_SIZE = sizeof(Name);
    constexpr uint32_t MAGNET_SIZE = sizeof(Magnet);
    constexpr uint32_t ID_OFFSET = 0;
    constexpr uint32_t NAME_OFFSET = ID_OFFSET + ID_SIZE;
    constexpr uint32_t MAGNET_OFFSET = NAME_OFFSET + NAME_SIZE;
    constexpr uint32_t ROW_SIZE = ID_SIZE + NAME_SIZE + MAGNET_SIZE;
private:
    uint32_t ID;
//Metadata:
    char Name   [LENGTH_OF_NAME];
    char Magnet [LENGTH_OF_MAGNET];
    //uint8_t Rate;
};

#endif