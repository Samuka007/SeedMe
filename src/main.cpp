#include "src/server/Seedme.hpp"
#include "src/server/Loadin.hpp"

int main( int argc, char* argv[] ){
    try{Seedme db("filename");}catch(...){}
    
    return 0;
}