#ifndef User_hpp
#define User_hpp

#include <string_view>
#include <vector>
#include "database/model/Rows.hpp"
#include "database/util/Table.hpp"
#include "database/util/Deleted.hpp"
using std::string;
using std::string_view;


class User {
    public:
    User(string_view filename)
    : table(filename), deleted(filename.data()) {}

    const string_view getUsername(unsigned id) {
        return table[id].username;
    }

    const string_view getPassword(unsigned id) {
        return table[id].password;
    }
    
    void setUsername(unsigned id, string_view username) {
        table[id].username = username;
    }

    void setPassword(unsigned id, string_view password_old, string_view password_new) {
        if(password_old != table[id].password) {
            throw std::invalid_argument("wrong password");
        }
        table[id].password = password_new;
    }

    void deleteUser(unsigned id) {
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

    private:
    Table<UsrRow> table;
    Deleted deleted;
};

#endif /* User_hpp */