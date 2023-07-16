#ifndef User_hpp
#define User_hpp

#include <string_view>
#include <exception>
#include <format>
#include <vector>
#include "model/Rows.hpp"
#include "util/Table.hpp"
#include "util/Deleted.hpp"
using std::string;
using std::string_view;


class User {
    public:
    User(string_view filename)
    : table(filename), deleted(filename.data()) {}

    const string_view getUsername(unsigned id) {
        if(isDeleted(id))
            throw resource_deleted();
        return table[id].username;
    }

    const string_view getPassword(unsigned id) {
        if(isDeleted(id))
            throw resource_deleted();
        return table[id].password;
    }
    
    void setUsername(unsigned id, string_view username) {
        if(isDeleted(id))
            throw resource_deleted();
        if (username.size() > STRING_LENGTH) {
            throw std::invalid_argument("Username too long");
        }
        std::strcpy(table[id].username, username.data());
    }

    void setPassword(unsigned id, string_view password_old, string_view password_new) {
        if(isDeleted(id))
            throw resource_deleted();
        if (password_old != table[id].password) {
            throw wrong_password();
        }
        if (password_new.size() > STRING_LENGTH) {
            throw too_long("Password");
        }
        std::strcpy(table[id].password, password_new.data());
    }

    inline void deleteUser(unsigned id) {
        deleted.insert(id);
    }

    inline unsigned get_last_user() {
        return table.last_row();
    }

    unsigned addUser(string_view username, string_view password) {
        unsigned id = deleted.get();
        if(id == 0) {
            id = table.last_row() + 1;
            table.new_row(UsrRow {id, username, password});
        } else {
            table[id] = UsrRow {id, username, password};
            deleted.erase(id);
        }
        return id;
    }
    
    bool isDeleted(unsigned id) {
        return deleted.contains(id);
    }

    Table<UsrRow> table;
    Deleted deleted;
};

#endif /* User_hpp */