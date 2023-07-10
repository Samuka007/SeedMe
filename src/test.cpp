#include "server/Seedme.hpp"
#include "server/Loadin.hpp"

int main(){
    int argc = 4;
    char* argv[] = {"try", "8080", "1", "try"};
    SeedSvr::init(argc, argv);
    return 0;
}