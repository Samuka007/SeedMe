#pragma once

#include <string_view>
#include <vector>
#include "database/model/Rows.hpp"
#include "database/util/Table.hpp"
#include "database/util/Deleted.hpp"
using std::string;
using std::string_view;


class Data {
    public:
    Data(string_view filename)
    : table(filename), deleted(filename.data()) {}

    inline const string_view getSrcname(const unsigned id) {
        return table[id].name;
    }

    inline const string_view getMagnet(const unsigned id) {
        return table[id].magnet;
    }

    inline const unsigned getOwner(const unsigned id) {
        return table[id].owner;
    }

    inline unsigned get_last_src() {
        return table.last_row();
    }
    
    void setSrcname(unsigned id, const string_view srcname) {
        std::strcpy(table[id].name, srcname.data());
    }

    void setMagnet(unsigned id, const string_view magnet) {
        std::strcpy(table[id].magnet, magnet.data());
    }

    void deleteSrc(unsigned id) {
        deleted.insert(id);
    }

    unsigned addSrc(const string_view srcname, const string_view magnet, const unsigned owner) {
        unsigned id = deleted.get();
        if (id == 0) {
            id = table.last_row() + 1;
            
            table.new_row(SrcRow {id, srcname.data(), magnet.data(), owner});
        } else {
            table[id] = {id, srcname.data(), magnet.data(), owner};
        }
        return id;
    }

    bool isDeleted(unsigned id) {
        return deleted.contains(id);
    }

    inline auto last_row() {
        return table.last_row();
    }

    Table<SrcRow> table;
    Deleted deleted;
};