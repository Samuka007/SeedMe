/**
 * for testing the database lib
*/
#include "service/SeedDB.hpp"
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <string_view>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h> 

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

    db.new_user(user1);
    db.new_user(user2);
    db.new_user(user3);

    try{
        db.new_user(user1);
    }catch(const std::exception& e){
        cout << e.what() << endl;
    }
    // should throw exception

    auto userid1 = db.login(user1);
    auto userid2 = db.login(user2);
    auto userid3 = db.login(user3);

    db.update_username(userid1, "user1_new");
    db.update_password(userid1, "password1", "password1_new");

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

    cout << "src1: " << src1.srcname << " " << src1.magnet << " " << src1.owner << endl;
// src create & get OK
    db.update_src_name(src1.srcid, "name1_new");
    db.update_src_magnet(src1.srcid, "magnet1_new");

    cout << "src1: " << src1.srcname << " " << src1.magnet << " " << src1.owner << endl;
// src update OK
    for(auto src : db.get_sources_by_ids({srcid1, srcid2, srcid3})){
        cout << "src: " << src.srcname << " " << src.magnet << " " << src.owner << endl;
    }

    for(auto src : db.get_usr_src_list(userid2)){
        cout << "src: " << src.srcname << " " << src.magnet << " " << src.owner << endl;
    }

    db.delete_src(srcid1);

    try{
        auto src1 = db.get_source(srcid1);
        // cout << "src1: " << src1.srcname << " " << src1.magnet << " " << src1.owner << endl;
    }catch(const SourceNotFoundError& e){
        cout << e.what() << endl;
    }
    // should throw exception
}

void test_read(){
    SeedDB db("test");
    user_t user2 = {"user2", "password2"};
    auto userid2 = db.login(user2);
    for(auto src : db.get_usr_src_list(userid2)){
        cout << "src: " << src.srcname << " " << src.magnet << " " << src.owner << endl;
    }
}


int main(){
    remove("test.data");
    remove("test.data.deleted");
    remove("test.user");
    remove("test.user.deleted");
    remove("test.meta");
    try{
        test_origin();
        test_read();
    }catch(const std::exception& e){
        cout << e.what() << endl;
    }
    return 0;
}