#pragma once

#include <string_view>
#include <vector>
#include "model/Rows.hpp"
#include "util/Table.hpp"
#include "util/Deleted.hpp"
using std::string;
using std::string_view;


class Data {
public:
    Data(string_view filename)
    : table(filename), deleted(filename.data()) {}

    inline string_view getSrcname(const unsigned id) const {
        if(isDeleted(id))
            throw resource_deleted();
        return table[id].getSrcname();
    }

    inline string_view getMagnet(const unsigned id) const {
        if(isDeleted(id))
            throw resource_deleted();
        return table[id].getMagnet();
    }

    inline unsigned getOwner(const unsigned id) const {
        if(isDeleted(id))
            throw resource_deleted();
        return table[id].getOwner();
    }

    inline unsigned get_last_src() const {
        return table.last_row();
    }
    
    void setSrcname(unsigned id, const string_view srcname) {
        if(isDeleted(id))
            throw resource_deleted();
        std::strcpy(table[id].name, srcname.data());
    }

    void setMagnet(unsigned id, const string_view magnet) {
        if(isDeleted(id))
            throw resource_deleted();
        std::strcpy(table[id].magnet, magnet.data());
    }

    void deleteSrc(unsigned id) {
        deleted.insert(id);
    }

    unsigned addSrc(const string_view srcname, const string_view magnet, const unsigned owner) {
        unsigned id = deleted.get();
        if (id == 0) {
            id = table.last_row() + 1;
            table.new_row(SrcRow {id, srcname, magnet, owner});
        } else {
            table[id] = SrcRow {id, srcname, magnet, owner};
            deleted.erase(id);
        }
        return id;
    }

    bool isDeleted(unsigned id) const {
        return deleted.contains(id);
    }

    Table<SrcRow> table;
    Deleted deleted;
};