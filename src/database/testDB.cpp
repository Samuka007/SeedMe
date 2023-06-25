/**
 * for testing the database lib
*/

#include "database/service/SeedDB.hpp"
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <string_view>

using std::vector;
using std::cout;
using std::endl;
using std::string;

void test_origin() {
    SeedDB db("test");

// test user functions
    user_t user1 = {"user1", "password1"};
    user_t user2 = {"user2", "password2"};
    user_t user3 = {"user3", "password3"};

    db.create_user(user1);
    db.create_user(user2);
    db.create_user(user3);

    try{
        db.create_user(user1);
    }catch(const std::exception& e){
        cout << e.what() << endl;
    }
    // should throw exception

    auto userid1 = db.login(user1);
    auto userid2 = db.login(user2);
    auto userid3 = db.login(user3);

    db.update_username(user1, "user1_new");
    db.update_password(user1, "password1_new");

    try{
        db.login(user1);
    }catch(const std::exception& e){
        cout << e.what() << endl;
    }

    db.delete_user(userid3);

    try{
        userid3 = db.login(user3);
        cout << "userid3: " << userid3 << endl;
    }catch(const std::exception& e){
        cout << e.what() << endl;
    }
    // should not login successfully

    auto srcid1 = db.create_source("name1", "magnet1", userid2);
    auto srcid2 = db.create_source("name2", "magnet2", userid2);
    auto srcid3 = db.create_source("name3", "magnet3", userid2);

    auto src1 = db.get_source(srcid1);

    cout << "src1: " << src1.name << " " << src1.magnet << " " << src1.owner << endl;

    db.update_src_name("name1_new", src1.srcid);
    db.update_src_magnet("magnet1_new", src1.srcid);

    cout << "src1: " << src1.name << " " << src1.magnet << " " << src1.owner << endl;

    for(auto src : db.get_sources_by_ids({srcid1, srcid2, srcid3})){
        cout << "src: " << src.name << " " << src.magnet << " " << src.owner << endl;
    }

    for(auto src : db.get_sources_by_ids(db.get_usr_src_list(userid2))){
        cout << "src: " << src.name << " " << src.magnet << " " << src.owner << endl;
    }

    db.delete_src(srcid1);

    try{
        auto src1 = db.get_source(srcid1);
        cout << "src1: " << src1.name << " " << src1.magnet << " " << src1.owner << endl;
    }catch(const std::exception& e){
        cout << e.what() << endl;
    }
    // should throw exception
}

void test_read(){
    SeedDB db("test");
    user_t user2 = {"user2", "password2"};
    auto userid2 = db.login(user2);
    for(auto src : db.get_sources_by_ids(db.get_usr_src_list(userid2))){
        cout << "src: " << src.name << " " << src.magnet << " " << src.owner << endl;
    }
}


int main(){
    int choice;
    test_origin();
    test_read();
    return 0;
}