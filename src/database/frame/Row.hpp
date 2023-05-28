#ifndef Row_hpp
#define Row_hpp

#include <bits/stdint-uintn.h>

class Row{
public:
    constexpr virtual uint32_t GET_ROW_SIZE() = 0;
    virtual void serialize(void*) = 0;
    virtual void deserialize(void*) = 0;
}

#endif /*Row_hpp*/